#include "include/AuthWidget.h"
#include <Wt/WPushButton.h>

#include <iostream>
#include <fstream>

AuthWidget::AuthWidget(std::shared_ptr<Login> login)
	: WContainerWidget(),
	  login_(login)
{
	setStyleClass("auth-widget");
	addWidget(std::make_unique<Wt::WText>("<h2 class='h2-login'><b>Event Manager Lab</b></h2>"));
	authTemp_ = addWidget(std::make_unique<Wt::WTemplate>());
	authTemp_->setTemplateText(tr("login-template"));
	authTemp_->setStyleClass("auth-container");

	auto signInBtn = authTemp_->bindWidget("btn-sign-in", std::make_unique<Wt::WPushButton>("Login"));
	auto signUpBtn = authTemp_->bindWidget("btn-sign-up", std::make_unique<Wt::WPushButton>("Create Account"));
	loginFormView_ = authTemp_->bindWidget("login-form-view", std::make_unique<LoginFormView>(login_));
	registrationFormView_ = authTemp_->bindWidget("registration-form-view", std::make_unique<RegistrationFormView>(login_));

	loginFormView_->setStyleClass("w-100");
	registrationFormView_->setStyleClass("w-100");

	signInBtn->clicked().connect(this, [=]()
								 { authTemp_->removeStyleClass("right-panel-active"); });
	signUpBtn->clicked().connect(this, [=]()
								 { authTemp_->addStyleClass("right-panel-active"); });
}