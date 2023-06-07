#include "include/AuthForms.h"
#include <Wt/WLengthValidator.h>
#include <Wt/WRegExpValidator.h>
#include <Wt/WEmailValidator.h>
#include <Wt/WEmailEdit.h>

#include <Wt/WLineEdit.h>
#include <Wt/WPushButton.h>

#include <Ice/Ice.h>
#include <stdexcept>

// login form fields
const Wt::WFormModel::Field LoginFormModel::UserEmail = "user-email";
const Wt::WFormModel::Field LoginFormModel::UserPassword = "user-password";

std::shared_ptr<Wt::WEmailValidator> createUserEmailValidator()
{
	auto v = std::make_shared<Wt::WEmailValidator>();
	v->setMandatory(true);
	v->setPattern("(?!(^[.-].*|[^@]*[.-]@|.*\\.{2,}.*)|^.{254}.)([a-zA-Z0-9!#$%&'*+\\/=?^_`{|}~.-]+@)(?!-.*|.*-\\.)([a-zA-Z0-9-]{1,63}\\.)+[a-zA-Z]{2,15}");
	v->setInvalidNotAnEmailAddressText("Invalid email address");
	return v;
}

std::shared_ptr<Wt::WValidator> createUserPasswordValidator()
{
	auto v = std::make_shared<Wt::WRegExpValidator>("^(?=.*[A-Za-z])(?=.*\\d)[A-Za-z\\d]{6,}$");
	v->setMandatory(true);
	return v;
}

std::shared_ptr<Wt::WValidator> createUserNameValidator()
{
	auto v = std::make_shared<Wt::WRegExpValidator>("[a-zA-Z ]{3,30}");
	v->setMandatory(true);
	return v;
}

std::shared_ptr<Wt::WValidator> createTextValidator(bool mandatory, int min, int max)
{
    auto v = std::make_shared<Wt::WLengthValidator>();
    v->setMandatory(mandatory);
    v->setMinimumLength(min);
    v->setMaximumLength(max);
    return v;
}

// Login Form Model Implementation
LoginFormModel::LoginFormModel()
	: WFormModel()
{
	// add Line Edit field, Validator, and some value to the input if you want
	addField(UserEmail);
	addField(UserPassword);

	setValidator(UserEmail, createUserEmailValidator());
	setValidator(UserPassword, createUserPasswordValidator());

}

AuthModule::LoginInfo LoginFormModel::getData()
{
	std::cout << "\n\n AuthentificationWidget::loginUser() Started \n";

	AuthModule::LoginInfo loginInfo;
	loginInfo.email = valueText(UserEmail).toUTF8();
	loginInfo.password = valueText(UserPassword).toUTF8();
	return loginInfo;
}

LoginFormView::LoginFormView(std::string temp_str, std::shared_ptr<Login> login)
	: Wt::WTemplateFormView (tr(temp_str)),
	model_(std::make_shared<LoginFormModel>()),
	login_(login)
{
	// decoration icons
    bindWidget("email-svg", std::make_unique<Wt::WText>(tr("email-svg")));
    bindWidget("key-svg", std::make_unique<Wt::WText>(tr("key-svg")));

	// Email field
    auto userEmailLineEdit = std::make_unique<Wt::WEmailEdit>();
	userEmailLineEdit->setPlaceholderText("Enter Your Email");
	std::cout << "\n\n email patter: " << userEmailLineEdit->pattern() << "\n\n";
	setFormWidget(LoginFormModel::UserEmail, std::move(userEmailLineEdit));

	// Password field
    auto userPasswordLineEdit = std::make_unique<Wt::WLineEdit>();
	userPasswordLineEdit->setEchoMode(Wt::EchoMode::Password);
	userPasswordLineEdit->setPlaceholderText("Enter Your Password");
	auto password = userPasswordLineEdit.get();
	auto eye_btn = bindWidget("user-password-eye", std::make_unique<Wt::WTemplate>(tr("eye-crossed-svg")));
	eye_btn->clicked().connect(this, [=](){
        if(password->echoMode() == Wt::EchoMode::Password)
        {
            password->setEchoMode(Wt::EchoMode::Normal);
            eye_btn->setTemplateText(tr("eye-svg"));
                }else {
            password->setEchoMode(Wt::EchoMode::Password);
            eye_btn->setTemplateText(tr("eye-crossed-svg"));
        }
    });
	setFormWidget(LoginFormModel::UserPassword, std::move(userPasswordLineEdit));

	// Login Btn
    auto login_btn  = bindWidget("login-button", std::make_unique<Wt::WPushButton>("Login"));
    login_btn->clicked().connect(this, &LoginFormView::process);
	bindEmpty("submit-info");
	updateView(model_.get());
}

void LoginFormView::process()
{
	// Update Model, for next step
	updateModel(model_.get());

	// Validate Model to send data to Authentification Server
	if (model_->validate())
	{
		std::cout << "\n\n model VALID \n\n";
		// Ice communication for loging in a user starts here
		try
		{
			Ice::CommunicatorHolder ich = Ice::initialize();
			auto base = ich->stringToProxy(login_->authConnString_);
			auto authInterface = Ice::checkedCast<AuthModule::AuthInterfacePrx>(base);
			if (!authInterface)
			{
				throw std::runtime_error("Invalid proxy");
			}

			// Try to login in server
			AuthModule::LoginReturn loginReturn = authInterface->loginUser(model_->getData());
			switch (loginReturn.loginResponse)
			{
			case AuthModule::LoginResponse::NotIdentified:
				bindWidget("submit-info", std::make_unique<Wt::WText>("No user with that Email"));
				break;
			case AuthModule::LoginResponse::LoggedIn:
				bindEmpty("submit-info");
				login_->login(loginReturn);
				break;
			case AuthModule::LoginResponse::Identified:
				bindWidget("submit-info", std::make_unique<Wt::WText>("User Succesfuly Identified"));
			case AuthModule::LoginResponse::IncorectPassword:
				bindWidget("submit-info", std::make_unique<Wt::WText>("Incorect Password, reset Password ?"));
				break;
			case AuthModule::LoginResponse::ThrottlingActivated:
				bindWidget("submit-info", std::make_unique<Wt::WText>("Password Trottling is activated, you can try again in 5 minutes, this is for your accout protection"));
				break;
			}
		}
		catch (const std::exception &e)
		{
			std::cerr << e.what() << std::endl;
		}
	}else {
		std::cout << "\n\n model NOT Valid \n\n";
		
		if(!model_->validateField(LoginFormModel::UserEmail))
		{
			std::cout << "\n\n Email NOT valid \n\n";
			bindWidget("submit-info", std::make_unique<Wt::WText>("Try a propper email addres example@mail.com"));
		}
		if(!model_->validateField(LoginFormModel::UserPassword))
		{
			std::cout << "\n\n Password NOT valid \n\n";
			auto password_requirments = bindWidget("submit-info", std::make_unique<Wt::WTemplate>(tr("password-requirments")));
		}
	}
}

// registration form fields
const Wt::WFormModel::Field RegistrationFormModel::UserName = "user-name";
const Wt::WFormModel::Field RegistrationFormModel::UserEmail = "user-email";
const Wt::WFormModel::Field RegistrationFormModel::UserPhone = "user-phone";
const Wt::WFormModel::Field RegistrationFormModel::UserPassword = "user-password";
const Wt::WFormModel::Field RegistrationFormModel::UserPasswordRepeat = "user-password-repeat";

// Registration Form Model Implementation
RegistrationFormModel::RegistrationFormModel()
	: WFormModel()
{
	// add Line Edit field, Validator, and some value to the input if you want
	addField(UserName);
	addField(UserEmail);
	addField(UserPhone);
	addField(UserPassword);
	addField(UserPasswordRepeat);

	setValidator(UserName, createUserNameValidator());
	setValidator(UserEmail, createUserEmailValidator());
	setValidator(UserPhone, createTextValidator(true, 10, 10));
	setValidator(UserPassword, createUserPasswordValidator());
	setValidator(UserPasswordRepeat, createUserPasswordValidator());
}

AuthModule::RegistrationInfo RegistrationFormModel::getData()
{
	AuthModule::RegistrationInfo registrationInfo;

	registrationInfo.name = valueText(UserName).toUTF8();
	registrationInfo.email = valueText(UserEmail).toUTF8();
	registrationInfo.phone = valueText(UserPhone).toUTF8();
	registrationInfo.password = valueText(UserPassword).toUTF8();

	return registrationInfo;
}

RegistrationFormView::RegistrationFormView(std::string temp_str, std::shared_ptr<Login> login)
	: Wt::WTemplateFormView(tr(temp_str)),
	model_(std::make_shared<RegistrationFormModel>()),
	login_(login)
{
	bindWidget("human-svg", std::make_unique<Wt::WText>(tr("human-svg")));
	bindWidget("key-svg", std::make_unique<Wt::WText>(tr("key-svg")));
	bindWidget("email-svg", std::make_unique<Wt::WText>(tr("email-svg")));
	bindWidget("phone-svg", std::make_unique<Wt::WText>(tr("phone-svg")));
	auto password_indicator = bindWidget("password-security-indicator", std::make_unique<Wt::WTemplate>(tr("password-security-indicator")));
	password_indicator->bindString("indicator-1", "");
	password_indicator->bindString("indicator-2", "");
	password_indicator->bindString("indicator-3", "");
	bindString("password-status", "to short");
	bindWidget("password-requirments", std::make_unique<Wt::WText>(tr("password-requirments")));

	// link Form Model Fields to FormWidget and set up the view
	auto usernameLineEdit = std::make_unique<Wt::WLineEdit>();
	usernameLineEdit->setPlaceholderText("Username");
	setFormWidget(RegistrationFormModel::UserName, std::move(usernameLineEdit));

	auto userEmailLineEdit = std::make_unique<Wt::WEmailEdit>();
	userEmailLineEdit->setPlaceholderText("Email");
	setFormWidget(RegistrationFormModel::UserEmail, std::move(userEmailLineEdit));

	// Phone field
	auto userPhoneLineEdit = std::make_unique<Wt::WLineEdit>();
	userPhoneLineEdit->setPlaceholderText("Phone");
	userPhoneLineEdit->setInputMask("9999999999");
	setFormWidget(RegistrationFormModel::UserPhone, std::move(userPhoneLineEdit));


	// Password field
    auto userPasswordLineEdit = std::make_unique<Wt::WLineEdit>();
	userPasswordLineEdit->setEchoMode(Wt::EchoMode::Password);
	userPasswordLineEdit->setPlaceholderText("Repeat Password to make sure");
	auto password = userPasswordLineEdit.get();
	auto eye_btn = bindWidget("password-eye", std::make_unique<Wt::WTemplate>(tr("eye-crossed-svg")));
	eye_btn->clicked().connect(this, [=](){
        if(password->echoMode() == Wt::EchoMode::Password)
        {
            password->setEchoMode(Wt::EchoMode::Normal);
            eye_btn->setTemplateText(tr("eye-svg"));
                }else {
            password->setEchoMode(Wt::EchoMode::Password);
            eye_btn->setTemplateText(tr("eye-crossed-svg"));
        }
    });

	setFormWidget(RegistrationFormModel::UserPassword, std::move(userPasswordLineEdit));
	
	// Password Repeat field
	auto userPasswordRepeatLineEdit = std::make_unique<Wt::WLineEdit>();
	userPasswordRepeatLineEdit->setEchoMode(Wt::EchoMode::Password);
	userPasswordRepeatLineEdit->setPlaceholderText("Repeat Your Password");
	auto password_repeat = userPasswordRepeatLineEdit.get();
	auto eye_btn_repeat = bindWidget("password-repeat-eye", std::make_unique<Wt::WTemplate>(tr("eye-crossed-svg")));
	eye_btn_repeat->clicked().connect(this, [=](){
		if(password_repeat->echoMode() == Wt::EchoMode::Password)
		{
			password_repeat->setEchoMode(Wt::EchoMode::Normal);
			eye_btn_repeat->setTemplateText(tr("eye-svg"));
				}else {
			password_repeat->setEchoMode(Wt::EchoMode::Password);
			eye_btn_repeat->setTemplateText(tr("eye-crossed-svg"));
		}
	});
	setFormWidget(RegistrationFormModel::UserPasswordRepeat, std::move(userPasswordRepeatLineEdit));

	// Login Btn
	auto submitBtn = std::make_unique<Wt::WPushButton>("Sign Up");
	submitBtn->clicked().connect(this, &RegistrationFormView::process);
	bindWidget("sign-up-btn", std::move(submitBtn));

	bindEmpty("submit-info");

	updateView(model_.get());
}

void RegistrationFormView::process()
{
	// Update Model, for next step
	updateModel(model_.get());

	// Validate Model to send data to Authentification Server
	if (model_->validate())
	{
		auto password = model_->valueText(model_->UserPassword);
		auto repeatPassword = model_->valueText(model_->UserPasswordRepeat);
		if (password != repeatPassword)
		{
			bindWidget("submit-info", std::make_unique<Wt::WText>("Passwords are not matching, try again"));
			return;
		}
		// Ice communication for loging in a user starts here
		try
		{
			Ice::CommunicatorHolder ich = Ice::initialize();
			auto base = ich->stringToProxy(login_->authConnString_);
			auto authInterface = Ice::checkedCast<AuthModule::AuthInterfacePrx>(base);
			if (!authInterface)
			{
				throw std::runtime_error("Invalid proxy");
			}

			// Try to login in server
			AuthModule::RegistrationResponse registrationResponse = authInterface->registerUser(model_->getData());

			if (registrationResponse == AuthModule::RegistrationResponse::RegistrationSuccessful)
			{
				bindEmpty("submit-info");
				// Log User In
				AuthModule::LoginInfo loginInfo;
				loginInfo.email = model_->valueText(model_->UserEmail).toUTF8();
				loginInfo.password = model_->valueText(model_->UserPassword).toUTF8();

				AuthModule::LoginReturn loginReturn = authInterface->loginUser(loginInfo);
				login_->login(loginReturn);
			}else if (registrationResponse == AuthModule::RegistrationResponse::EmailAlreadyExists)
			{
				bindWidget("submit-info", std::make_unique<Wt::WText>("User with same Email identified, forgot Password ?"));
			}
		}
		catch (const std::exception &e)
		{
			std::cerr << e.what() << std::endl;
		}
	}else {
		std::cout << "\n\n Model is not valid \n\n";
		if(!model_->validateField(model_->UserName))
		{
			std::cout << "\n\n Username is valid \n\n";
		}
		if(!model_->validateField(model_->UserPhone))
		{
			std::cout << "\n\n Phone is valid \n\n";
		}
		if(!model_->validateField(model_->UserEmail))
		{
			std::cout << "\n\n Email is valid \n\n";
		}
		if(!model_->validateField(model_->UserPassword))
		{
			std::cout << "\n\n Password is valid \n\n";
		}
		if(!model_->validateField(model_->UserPasswordRepeat))
		{
			std::cout << "\n\n Password Repeat is valid \n\n";
		}

	}
}
