#include <vsomeip/vsomeip.hpp>

class Server {
public:
    Server();
    ~Server();

    void onStateChange(vsomeip::state_type_e state);
    void onMessage(const std::shared_ptr<vsomeip::message> &request);

private:
    std::shared_ptr<vsomeip::runtime> rtm;
    std::shared_ptr<vsomeip::application> app;
};