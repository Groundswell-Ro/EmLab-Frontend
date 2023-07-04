#include "../../utils/include/LoginObj.h"

#include <Wt/WTemplate.h>
#include <Wt/WEmailValidator.h>
#include <Wt/WRegExpValidator.h>
#include <Wt/WDialog.h>
#include <Wt/WPushButton.h>

class Auth : public Wt::WDialog
{
public:
    Auth(std::shared_ptr<Login> login);
    ~Auth();
    
    Wt::WTemplate* registration_template_;
    Wt::WTemplate* login_template_;
    
    void dev_loginUser(Wt::WString user_email, Wt::WString user_password);

private:
    void createLogin();
    void createSignUp();

    void closeDialog();

    Emlab::LoginReturn tryLogin(Emlab::LoginInfo loginInfo);
    Emlab::RegistrationResponse trySignUp(Emlab::RegistrationInfo registrationInfo);

    Wt::WPushButton* submit_btn_;

    std::shared_ptr<Login> login_;
    
    Wt::WRegExpValidator email_validator_;
    Wt::WRegExpValidator password_validator_;
    
    Wt::WRegExpValidator name_validator_;
    Wt::WRegExpValidator phone_validator_;

};
