#include "include/AuthWidget.h"
#include <Wt/WPushButton.h>
#include <Wt/WApplication.h>
#include <iostream>
#include <fstream>

AuthWidget::AuthWidget(std::shared_ptr<Login> login)
	: WDialog("Autentificare"),
	  login_(login)
{
	titleBar()->setStyleClass("d-flex justify-content-center align-items-center");

	setMinimumSize(Wt::WLength(767, Wt::LengthUnit::Pixel), Wt::WLength::Auto);

	Wt::WApplication::instance()->setInternalPath("/Authentification");

	setMovable(false);
	contents()->setStyleClass("d-flex justify-content-center align-items-center");
	authTemp_ = contents()->addWidget(std::make_unique<Wt::WTemplate>(tr("login-template")));
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