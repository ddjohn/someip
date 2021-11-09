#include "client.h"
#include <iostream>

Client::Client() {
    rtm = vsomeip::runtime::get();
    app = rtm->create_application();

    app->init();

    app->register_message_handler(0x1234, 0x0001, 0x0002,
        std::bind(&Client::onMessage, this, std::placeholders::_1));

    app->register_state_handler(
        std::bind(&Client::onStateChange, this, std::placeholders::_1));

    app->register_availability_handler(0x1234, 0x0001,
        std::bind(&Client::onAvailabilityChange, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    
    app->start();
}

Client::~Client() {
    app->stop();
}

void Client::onStateChange(vsomeip::state_type_e state) {
    std::cout << "onStateChange: " << (int)state << std::endl;
    if(state == vsomeip::state_type_e::ST_REGISTERED) {
        app->request_service(0x1234, 0x0001);
    }
}

void Client::onMessage(const std::shared_ptr<vsomeip::message> &response) {
    std::cout << "onMessage: " << response << std::endl;
    if(0x1234 == response->get_service() && 0x0001 == response->get_instance()
                && vsomeip::message_type_e::MT_RESPONSE == response->get_message_type()
                && vsomeip::return_code_e::E_OK == response->get_return_code()) {

        std::shared_ptr<vsomeip::payload> pl = response->get_payload();
        std::string resp = std::string(reinterpret_cast<const char*>(pl->get_data()), 0, pl->get_length());

        std::cout << "Received: " << resp << std::endl;
               
        exit(0);
    }
}

void Client::onAvailabilityChange(vsomeip::service_t service, vsomeip::instance_t instance, bool available) {
    std::cout << "onAvailabilityChange: " << available << std::endl;
    if(0x1234 == service && 0x0001 == instance && available) {
        std::shared_ptr<vsomeip::message> rq = rtm->create_request();
        rq->set_service(0x1234);
        rq->set_instance(0x0001);
        rq->set_method(0x0002);

        std::shared_ptr<vsomeip::payload> pl = rtm->create_payload();
        std::string str("World");
        std::vector<vsomeip::byte_t> pl_data(std::begin(str), std::end(str));

        pl->set_data(pl_data);
        rq->set_payload(pl);

        app->send(rq);
    }
}

int main() {
    Client client;
    return 0;
}
