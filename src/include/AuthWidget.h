#pragma once
#include "AuthForms.h"
#include "LoginObj.h"

#include <Wt/WContainerWidget.h>
#include <Wt/WTemplate.h>

class AuthWidget : public Wt::WContainerWidget
{
public:
	AuthWidget(std::shared_ptr<Login> login);

	std::shared_ptr<Login> login_;


private:
	LoginFormView *loginFormView_;
	Wt::WTemplate *authTemp_;
	RegistrationFormView *registrationFormView_;
};