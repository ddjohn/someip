#include "server.h"
#include <iostream>

Server::Server() {
    rtm = vsomeip::runtime::get();
    app = rtm->create_application();

    app->init();

    app->register_message_handler(0x1234, 0x0001, 0x0002,
        std::bind(&Server::onMessage, this, std::placeholders::_1));

    app->register_state_handler(
        std::bind(&Server::onStateChange, this, std::placeholders::_1));

    app->start();
}

Server::~Server() {
    app->stop();
}

void Server::onStateChange(vsomeip::state_type_e state) {
    std::cout << "onStateChange: " << (int)state << std::endl;

    if(state == vsomeip::state_type_e::ST_REGISTERED) {
        app->offer_service(0x1234, 0x0001);
    }
}

void Server::onMessage(const std::shared_ptr<vsomeip::message> &request) {
    std::cout << "onMessage: " << request << std::endl;

    std::shared_ptr<vsomeip::message> resp = rtm->create_response(request);
    std::string str("Hello ");
    str.append(reinterpret_cast<const char*>(request->get_payload()->get_data()), 0, request->get_payload()->get_length());

    std::shared_ptr<vsomeip::payload> resp_pl = rtm->create_payload();
    std::vector<vsomeip::byte_t> pl_data(str.begin(), str.end());
    resp_pl->set_data(pl_data);
    resp->set_payload(resp_pl);

    app->send(resp);

//    exit(0);
}

int main() {
    Server server;
    return 0;
}
