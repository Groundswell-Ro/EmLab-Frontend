#include "include/Auth.h"
#include "../../comunication/comm/AuthInterface.h"
#include "../utils/include/PhotoUploder.h"

#include <Wt/WTemplate.h>
#include <Wt/WTemplate.h>
#include <Wt/WPushButton.h>
#include <Wt/WSignal.h>
#include <Wt/WLineEdit.h>
#include <Wt/WEmailEdit.h>
#include <Wt/WImage.h>
#include <Wt/WAnimation.h>

#include <stdexcept>
#include <Ice/Ice.h>

Auth::Auth(std::shared_ptr<Login> login) 
  :  login_(login)
{
    setTitleBarEnabled(false);
    setOffsets(20, Wt::Side::Top | Wt::Side::Left | Wt::Side::Right);
    // setStyleClass("");
    // contents()->setStyleClass("w-full relative");
    auto close_btn = contents()->addWidget(std::make_unique<Wt::WPushButton>("X"));
    close_btn->setStyleClass("btn btn-danger rounded-full absolute p-1 top-0 right-0 m-2");
    close_btn->clicked().connect(this, &Auth::closeDialog);

    email_validator_ = Wt::WRegExpValidator("(?!(^[.-].*|[^@]*[.-]@|.*\\.{2,}.*)|^.{254}.)([a-zA-Z0-9!#$%&'*+\\/=?^_`{|}~.-]+@)(?!-.*|.*-\\.)([a-zA-Z0-9-]{1,63}\\.)+[a-zA-Z]{2,15}");
    email_validator_.setMandatory(true);
    email_validator_.setInvalidNoMatchText("Email is not valid address, \"example.address@mail.com\"");
    email_validator_.setInvalidBlankText("Email is empty");

    password_validator_ = Wt::WRegExpValidator("^(?=.*[A-Za-z])(?=.*\\d)[A-Za-z\\d]{6,}$");
    password_validator_.setMandatory(true);
    password_validator_.setInvalidNoMatchText("Password must contain at least 6 characters, including letters and numbers");
    password_validator_.setInvalidBlankText("Password is empty");

    name_validator_ = Wt::WRegExpValidator("^[a-zA-Z ]{3,50}$");
    name_validator_.setMandatory(true);
    name_validator_.setInvalidNoMatchText("Name must containe between 3 and 50 characters");
    name_validator_.setInvalidBlankText("Name is empty");

    phone_validator_ = Wt::WRegExpValidator("^\\d{10}$");
    phone_validator_.setMandatory(true);
    phone_validator_.setInvalidNoMatchText("Phone number must contain 10 digits");
    phone_validator_.setInvalidBlankText("Phone number is empty");

    rejectWhenEscapePressed();
    createLogin();
    enterPressed().connect(this, [=](){ submit_btn_->clicked().emit(Wt::WMouseEvent()); });

    setHidden(false, Wt::WAnimation(Wt::AnimationEffect::SlideInFromTop, Wt::TimingFunction::EaseInOut, 500));

}

Auth::~Auth() {
    std::cout << "\n\n\n auth deconstructor here \n\n\n";
}

void Auth::createLogin()
{
    contents()->clear();
    
	login_template_ = contents()->addWidget(std::make_unique<Wt::WTemplate>(tr("login-content")));

    auto email_temp = login_template_->bindWidget("email-input", std::make_unique<Wt::WTemplate>(tr("input-template-normal")));
    email_temp->bindString("label", "Email");
    email_temp->setCondition("if-start-svg", true);
    email_temp->bindWidget("start-svg", std::make_unique<Wt::WTemplate>(tr("email-svg")));
    auto email_input = email_temp->bindWidget("input", std::make_unique<Wt::WEmailEdit>());
    email_input->setPlaceholderText("Your Email Address");    

    auto password_temp = login_template_->bindWidget("password-input", std::make_unique<Wt::WTemplate>(tr("input-template-normal")));
    password_temp->bindString("label", "Password");
    password_temp->setCondition("if-start-svg", true);
    password_temp->bindWidget("start-svg", std::make_unique<Wt::WTemplate>(tr("key-svg")));
    password_temp->setCondition("if-end-svg", true);
    auto password_input = password_temp->bindWidget("input", std::make_unique<Wt::WLineEdit>());
    password_input->setPlaceholderText("Your Password");
    password_input->setEchoMode(Wt::EchoMode::Password);
    auto eye_btn = password_temp->bindWidget("end-svg", std::make_unique<Wt::WTemplate>(tr("eye-svg")));
    eye_btn->clicked().connect(this, [=](){ 
        if(password_input->echoMode() == Wt::EchoMode::Password)
        {
            password_input->setEchoMode(Wt::EchoMode::Normal);
            eye_btn->setTemplateText(tr("eye-crossed-svg"));
        }
        else
        {
            password_input->setEchoMode(Wt::EchoMode::Password);
            eye_btn->setTemplateText(tr("eye-svg"));
        }
        });


    submit_btn_ = login_template_->bindWidget("login-button", std::make_unique<Wt::WPushButton>("Login"));
    submit_btn_->clicked().connect(this, [=](){
        // validation
        auto email_validation_result = email_validator_.validate(email_input->valueText());
        auto password_validation_result = password_validator_.validate(password_input->valueText());
        if(email_validation_result.state() == Wt::ValidationState::Invalid)
        {
            login_template_->bindString("submit-info", email_validation_result.message());
            return;
        }else if (email_validation_result.state() == Wt::ValidationState::InvalidEmpty)
        {
            login_template_->bindString("submit-info", email_validation_result.message());
            return;
        }

        if(password_validation_result.state() == Wt::ValidationState::Invalid)
        {
            login_template_->bindString("submit-info", password_validation_result.message());
            return;
        }else if (password_validation_result.state() == Wt::ValidationState::InvalidEmpty)
        {
            login_template_->bindString("submit-info", password_validation_result.message());
            return;
        }

        // get data
        Emlab::LoginInfo loginInfo;
        loginInfo.email = email_input->valueText().toUTF8();
        loginInfo.password = password_input->valueText().toUTF8();

        // submit data
        auto loginReturn = tryLogin(loginInfo);

        if (loginReturn.loginResponse == Emlab::LoginResponse::NotIdentified) {
            login_template_->bindString("submit-info", "No user with that Email");
        } else if (loginReturn.loginResponse == Emlab::LoginResponse::Identified) {
            login_template_->bindString("submit-info", "Password is incorrect");
        } else if (loginReturn.loginResponse == Emlab::LoginResponse::LoggedIn) {
            login_template_->bindString("submit-info", "Congrats you are logged in");
            login_->login(loginReturn);
            closeDialog();
        } else if (loginReturn.loginResponse == Emlab::LoginResponse::IncorectPassword) {
            login_template_->bindString("submit-info", "Password is incorrect");
        } else if (loginReturn.loginResponse == Emlab::LoginResponse::ThrottlingActivated) {
            login_template_->bindString("submit-info", "for your safety, we blocked yout access for 5 minutes because of too many failed login attempts");
        } else {
            login_template_->bindString("submit-info", "Something went wrong");
        }
    });


    auto sign_up_btn = login_template_->bindWidget("sign-up-button", std::make_unique<Wt::WPushButton>("Sign Up"));
    sign_up_btn->clicked().connect(this, [=](){
        createSignUp();
    });
    login_template_->bindEmpty("submit-info");
}

void Auth::createSignUp() {

    contents()->clear();

    registration_template_ = contents()->addWidget(std::make_unique<Wt::WTemplate>(Wt::WString::tr("sign-up-content")));
    registration_template_->bindWidget("password-requirments", std::make_unique<Wt::WTemplate>(Wt::WString::tr("password-requirments")));
    registration_template_->bindEmpty("submit-info");

	auto photo_uploder_ = registration_template_->bindWidget("photo-uploader", std::make_unique<PhotoUploder>());

    auto name_temp = registration_template_->bindWidget("name-input", std::make_unique<Wt::WTemplate>(Wt::WString::tr("input-template-normal")));
    name_temp->bindString("label", "Name");
    name_temp->setCondition("if-start-svg", true);
    name_temp->bindWidget("start-svg", std::make_unique<Wt::WTemplate>(Wt::WString::tr("human-svg")));
    auto name_input = name_temp->bindWidget("input", std::make_unique<Wt::WLineEdit>());
    name_input->setPlaceholderText("Your Name");
    name_input->setTextSize(50);


    auto email_temp = registration_template_->bindWidget("email-input", std::make_unique<Wt::WTemplate>(Wt::WString::tr("input-template-normal")));
    email_temp->bindString("label", "Email");
    email_temp->setCondition("if-start-svg", true);
    email_temp->bindWidget("start-svg", std::make_unique<Wt::WTemplate>(tr("email-svg")));
    auto email_input = email_temp->bindWidget("input", std::make_unique<Wt::WEmailEdit>());
    email_input->setPlaceholderText("Your Email Address");

    auto phone_temp = registration_template_->bindWidget("phone-input", std::make_unique<Wt::WTemplate>(Wt::WString::tr("input-template-normal")));
    phone_temp->bindString("label", "Phone");
    phone_temp->setCondition("if-start-svg", true);
    phone_temp->bindWidget("start-svg", std::make_unique<Wt::WTemplate>(tr("phone-svg")));
    auto phone_input = phone_temp->bindWidget("input", std::make_unique<Wt::WLineEdit>());
    phone_input->setPlaceholderText("Your Phone Number");
	phone_input->setInputMask("9999999999");

    auto password_temp = registration_template_->bindWidget("password-input", std::make_unique<Wt::WTemplate>(Wt::WString::tr("input-template-normal")));
    password_temp->bindString("label", "Password");
    password_temp->setCondition("if-start-svg", true);
    password_temp->bindWidget("start-svg", std::make_unique<Wt::WTemplate>(tr("key-svg")));
    password_temp->setCondition("if-end-svg", true);
    auto password_input = password_temp->bindWidget("input", std::make_unique<Wt::WLineEdit>());
    password_input->setEchoMode(Wt::EchoMode::Password);
    password_input->setPlaceholderText("Your Password");
    auto eye_btn = password_temp->bindWidget("end-svg", std::make_unique<Wt::WTemplate>(Wt::WString::tr("eye-svg")));
    eye_btn->clicked().connect(this, [=](){ 
        if(password_input->echoMode() == Wt::EchoMode::Password)
        {
            password_input->setEchoMode(Wt::EchoMode::Normal);
            eye_btn->setTemplateText(Wt::WString::tr("eye-crossed-svg"));
        }
        else
        {
            password_input->setEchoMode(Wt::EchoMode::Password);
            eye_btn->setTemplateText(Wt::WString::tr("eye-svg"));
        }
        });

    auto password_repeat_temp = registration_template_->bindWidget("password-repeat-input", std::make_unique<Wt::WTemplate>(Wt::WString::tr("input-template-normal")));
    password_repeat_temp->bindString("label", "Repeat Password");
    password_repeat_temp->setCondition("if-start-svg", true);
    password_repeat_temp->bindWidget("start-svg", std::make_unique<Wt::WTemplate>(tr("key-svg")));
    password_repeat_temp->setCondition("if-end-svg", true);
    auto password_repeat_input = password_repeat_temp->bindWidget("input", std::make_unique<Wt::WLineEdit>());
    password_repeat_input->setEchoMode(Wt::EchoMode::Password);
    password_repeat_input->setPlaceholderText("Repeat Password");
    auto eye_btn_repeat = password_repeat_temp->bindWidget("end-svg", std::make_unique<Wt::WTemplate>(Wt::WString::tr("eye-svg")));
    eye_btn_repeat->clicked().connect(this, [=](){ 
        if(password_repeat_input->echoMode() == Wt::EchoMode::Password)
        {
            password_repeat_input->setEchoMode(Wt::EchoMode::Normal);
            eye_btn_repeat->setTemplateText(Wt::WString::tr("eye-crossed-svg"));
        }
        else
        {
            password_repeat_input->setEchoMode(Wt::EchoMode::Password);
            eye_btn_repeat->setTemplateText(Wt::WString::tr("eye-svg"));
        }
        });
    
    submit_btn_ = registration_template_->bindWidget("submit-btn", std::make_unique<Wt::WPushButton>("Submit"));
    submit_btn_->clicked().connect(this, [=](){
        // validation
        auto name_validation_result = name_validator_.validate(name_input->valueText().toUTF8());
        auto email_validation_result = email_validator_.validate(email_input->valueText().toUTF8());
        auto phone_validation_result = phone_validator_.validate(phone_input->valueText().toUTF8());
        auto password_validation_result = password_validator_.validate(password_input->valueText().toUTF8());

        // validate name
        if(name_validation_result.state() == Wt::ValidationState::InvalidEmpty)
        {
            registration_template_->bindString("submit-info", name_validation_result.message());
            return;
        }else if (name_validation_result.state() == Wt::ValidationState::Invalid)
        {
            registration_template_->bindString("submit-info", name_validation_result.message());
            return;
        }
        
        // validate email
        if(email_validation_result.state() == Wt::ValidationState::InvalidEmpty)
        {
            registration_template_->bindString("submit-info", email_validation_result.message());
            return;
        }else if (email_validation_result.state() == Wt::ValidationState::Invalid)
        {
            registration_template_->bindString("submit-info", email_validation_result.message());
            return;
        }

        // validate phone
        if(phone_validation_result.state() == Wt::ValidationState::InvalidEmpty)
        {
            registration_template_->bindString("submit-info", phone_validation_result.message());
            return;
        }else if (phone_validation_result.state() == Wt::ValidationState::Invalid)
        {
            registration_template_->bindString("submit-info", phone_validation_result.message());
            return;
        }

        // validate password
        if(password_validation_result.state() == Wt::ValidationState::InvalidEmpty)
        {
            registration_template_->bindString("submit-info", password_validation_result.message());
            return;
        }else if (password_validation_result.state() == Wt::ValidationState::Invalid)
        {
            registration_template_->bindString("submit-info", password_validation_result.message());
            return;
        }

        // validate password repeat
        if(password_input->valueText() != password_repeat_input->valueText())
        {
            registration_template_->bindString("submit-info", "Passwords do not match");
            return;
        }

        // process
        Emlab::RegistrationInfo registrationInfo;
        registrationInfo.name = name_input->valueText().toUTF8();
        registrationInfo.email = email_input->valueText().toUTF8();
        registrationInfo.phone = phone_input->valueText().toUTF8();
        registrationInfo.password = password_input->valueText().toUTF8();
        registrationInfo.photo = photo_uploder_->getImageData();

        auto registration_response = trySignUp(registrationInfo);
        if(registration_response == Emlab::RegistrationResponse::RegistrationSuccessful)
        {
            registration_template_->bindString("submit-info", "Registration successful");
            Emlab::LoginInfo loginInfo;
            loginInfo.email = registrationInfo.email;
            loginInfo.password = registrationInfo.password;
            login_->login(tryLogin(loginInfo));
            closeDialog();
        } else if (registration_response == Emlab::RegistrationResponse::EmailAlreadyExists)
        {
            registration_template_->bindString("submit-info", "Email already exists");
        } else if (registration_response == Emlab::RegistrationResponse::PhoneAlreadyExists)
        {
            registration_template_->bindString("submit-info", "Phone already exists");
        } else
        {
            registration_template_->bindString("submit-info", "Registration failed for some unknown reason");
        }

    });

    
    auto back_to_login = registration_template_->bindWidget("back-to-login", std::make_unique<Wt::WPushButton>("Back to login"));
    back_to_login->clicked().connect(this, [=](){
        createLogin();
    });
}

Emlab::LoginReturn Auth::tryLogin(Emlab::LoginInfo loginInfo)
{
    Emlab::LoginReturn loginReturn;
    try
    {
        Ice::CommunicatorHolder ich = Ice::initialize();
        auto base = ich->stringToProxy(login_->AUTH_CONN_STRING);
        auto authInterface = Ice::checkedCast<Emlab::AuthInterfacePrx>(base);
        if (!authInterface)
        {
            throw std::runtime_error("Invalid proxy");
        }
        // login user
        loginReturn = authInterface->loginUser(loginInfo);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
    return loginReturn;
}

Emlab::RegistrationResponse Auth::trySignUp(Emlab::RegistrationInfo registrationInfo)
{
    Emlab::RegistrationResponse registrationResponse;
    try
    {
        Ice::CommunicatorHolder ich = Ice::initialize();
        auto base = ich->stringToProxy(login_->AUTH_CONN_STRING);
        auto authInterface = Ice::checkedCast<Emlab::AuthInterfacePrx>(base);
        if (!authInterface)
        {
            throw std::runtime_error("Invalid proxy");
        }
        // register user
        registrationResponse = authInterface->registerUser(registrationInfo);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
    return registrationResponse;
}

void Auth::closeDialog()
{
    done(Wt::DialogCode::Accepted);
    removeFromParent();
}

void Auth::dev_loginUser(Wt::WString user_email, Wt::WString user_password) {

    Emlab::LoginInfo loginInfo;
    loginInfo.email = user_email.toUTF8();
    loginInfo.password = user_password.toUTF8();
    login_->login(tryLogin(loginInfo));
    closeDialog();

}



