#include "AuthForms.h"
#include <Wt/WTemplate.h>
#include <Wt/WTemplate.h>
#include <Wt/WPushButton.h>
#include <Wt/WSignal.h>

class Auth : public Wt::WTemplate
{
public:
    Auth(std::shared_ptr<Login> login);
    
    LoginFormView* login_veiw;
    RegistrationFormView* registration_view;

    void dev_loginUser(Wt::WString user_email, Wt::WString user_password);
private:
    void createLogin();
    void createSignUp();
    std::shared_ptr<Login> login_;
};
