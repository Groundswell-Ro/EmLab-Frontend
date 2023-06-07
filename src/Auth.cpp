#include "include/Auth.h"
#include <Wt/WLineEdit.h>

Auth::Auth(std::shared_ptr<Login> login) 
    : Wt::WTemplate(tr("auth")),
    login_(login)
{
	setStyleClass("bg-body min-h-screen");
    createLogin();
}

void Auth::createLogin()
{
    bindWidget("header", std::make_unique<Wt::WText>("<div>Login</div>"));
	auto login = bindWidget("content", std::make_unique<LoginFormView>("login-content" ,login_));
    auto sign_up_btn = login->bindWidget("sign-up-button", std::make_unique<Wt::WPushButton>("Sign Up"));
    
    sign_up_btn->clicked().connect(this, [=](){
        createSignUp();
    });
    
}

void Auth::createSignUp() {
    bindWidget("header", std::make_unique<Wt::WText>("<div>Registration</div>"));

    auto registration = bindWidget("content", std::make_unique<RegistrationFormView>("sign-up-content",login_));

	// auto content = bindWidget("content", std::make_unique<Wt::WTemplate>(tr("sign-up-content")));
    
    // content->bindWidget("password-requirments", std::make_unique<Wt::WTemplate>(tr("password-requirments")));

    auto back_to_login = registration->bindWidget("back-to-login", std::make_unique<Wt::WPushButton>("Back to login"));

    back_to_login->clicked().connect(this, [=](){
        createLogin();
    });
}