#include "include/Auth.h"

#include <Wt/WTemplate.h>
#include <Wt/WTemplate.h>
#include <Wt/WPushButton.h>
#include <Wt/WSignal.h>
#include <Wt/WLineEdit.h>

Auth::Auth(std::shared_ptr<Login> login) 
    : Wt::WTemplate(tr("auth")),
    login_(login)
{
	setStyleClass("bg-body min-h-screen");
    createLogin();
    // createSignUp();
}

void Auth::createLogin()
{
    bindWidget("header", std::make_unique<Wt::WText>("<div>Login</div>"));
	login_veiw = bindWidget("content", std::make_unique<LoginFormView>("login-content" ,login_));
    auto sign_up_btn = login_veiw->bindWidget("sign-up-button", std::make_unique<Wt::WPushButton>("Sign Up"));
    
    sign_up_btn->clicked().connect(this, [=](){
        createSignUp();
    });
    
}

void Auth::createSignUp() {
    bindWidget("header", std::make_unique<Wt::WText>("<div>Registration</div>"));

    registration_view = bindWidget("content", std::make_unique<RegistrationFormView>("sign-up-content",login_));
    auto back_to_login = registration_view->bindWidget("back-to-login", std::make_unique<Wt::WPushButton>("Back to login"));

    back_to_login->clicked().connect(this, [=](){
        createLogin();
    });
    // registration_view->dev_setValues();
}

void Auth::dev_loginUser(Wt::WString user_email, Wt::WString user_password) {
    createLogin();
    login_veiw->email_->setValueText(user_email);
    login_veiw->password_->setValueText(user_password);
    login_veiw->process();

}