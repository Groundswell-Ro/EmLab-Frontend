#include "AuthForms.h"
#include <Wt/WTemplate.h>
#include <Wt/WTemplate.h>
#include <Wt/WPushButton.h>
#include <Wt/WSignal.h>

enum RegistrationSteps {
    step_one = 1,
    step_two = 2,
    step_three = 3,
    step_four = 4
};

class Auth : public Wt::WTemplate
{
public:
    Auth(std::shared_ptr<Login> login);

    Wt::Signal<>& userLoggedIn() { return user_logged_in_; }
private:
    void createLogin();
    void createSignUp();
    std::shared_ptr<Login> login_;
    Wt::Signal<> user_logged_in_;
};
