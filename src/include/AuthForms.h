#pragma once
#include <Wt/WFormModel.h> // used to create the Model of the form
#include <Wt/WTemplateFormView.h>
#include <Wt/WValidator.h> // used to create a Validator
#include "LoginObj.h"

#include "../../../comunication/AuthModule.h"

std::shared_ptr<Wt::WValidator> createUserPasswordValidator();

// Login Form Model Definition
class LoginFormModel : public Wt::WFormModel
{
public:
	static const Field UserEmail;
	static const Field UserPassword;

	LoginFormModel();
	AuthModule::LoginInfo getData();

private:
};

// Login View
class LoginFormView : public Wt::WTemplateFormView
{
public:
	LoginFormView(std::shared_ptr<Login> login);

	std::shared_ptr<LoginFormModel> model_;
	std::shared_ptr<Login> login_;

	void process();
private:
};

// Registration Form Model Definition
class RegistrationFormModel : public Wt::WFormModel
{
public:
	static const Field UserName;
	static const Field UserEmail;
	static const Field UserPhone;
	static const Field UserPassword;
	static const Field UserPasswordRepeat;

	RegistrationFormModel();

	AuthModule::RegistrationInfo getData();

private:
};

// Registration View
class RegistrationFormView : public Wt::WTemplateFormView
{
public:
	RegistrationFormView(std::shared_ptr<Login> login);

	std::shared_ptr<RegistrationFormModel> model_;

	std::shared_ptr<Login> login_;

private:
	void process();
};