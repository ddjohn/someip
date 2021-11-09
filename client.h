#include <vsomeip/vsomeip.hpp>

class Client {
public:
    Client();
    ~Client();

    void onStateChange(vsomeip::state_type_e state);
    void onMessage(const std::shared_ptr<vsomeip::message> &request);
    void onAvailabilityChange(vsomeip::service_t service, vsomeip::instance_t instance, bool available);

private:
    std::shared_ptr<vsomeip::runtime> rtm;
    std::shared_ptr<vsomeip::application> app;
};