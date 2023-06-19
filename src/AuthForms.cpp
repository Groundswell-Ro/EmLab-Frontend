#include "include/AuthForms.h"


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

Emlab::LoginInfo LoginFormModel::getData()
{
	std::cout << "\n\n AuthentificationWidget::loginUser() Started \n";

	Emlab::LoginInfo loginInfo;
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
    auto email_input = std::make_unique<Wt::WEmailEdit>();
	email_input->setPlaceholderText("Enter Your Email");
	email_ = email_input.get();
	setFormWidget(LoginFormModel::UserEmail, std::move(email_input));

	// Password field
    auto password_input = std::make_unique<Wt::WLineEdit>();
	password_input->setEchoMode(Wt::EchoMode::Password);
	password_input->setPlaceholderText("Enter Your Password");
	password_ = password_input.get();
	auto eye_btn = bindWidget("user-password-eye", std::make_unique<Wt::WTemplate>(tr("eye-crossed-svg")));
	eye_btn->clicked().connect(this, [=](){
        if(password_->echoMode() == Wt::EchoMode::Password)
        {
            password_->setEchoMode(Wt::EchoMode::Normal);
            eye_btn->setTemplateText(tr("eye-svg"));
                }else {
            password_->setEchoMode(Wt::EchoMode::Password);
            eye_btn->setTemplateText(tr("eye-crossed-svg"));
        }
    });
	setFormWidget(LoginFormModel::UserPassword, std::move(password_input));

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

		Emlab::LoginReturn loginReturn = login_->loginUser(model_->getData());
		switch (loginReturn.loginResponse)
		{
		case Emlab::LoginResponse::NotIdentified:
			bindWidget("submit-info", std::make_unique<Wt::WText>("No user with that Email"));
			break;
		case Emlab::LoginResponse::LoggedIn:
			bindEmpty("submit-info");
			login_->login(loginReturn);
			break;
		case Emlab::LoginResponse::Identified:
			bindWidget("submit-info", std::make_unique<Wt::WText>("User Succesfuly Identified"));
		case Emlab::LoginResponse::IncorectPassword:
			bindWidget("submit-info", std::make_unique<Wt::WText>("Incorect Password, reset Password ?"));
			break;
		case Emlab::LoginResponse::ThrottlingActivated:
			bindWidget("submit-info", std::make_unique<Wt::WText>("Password Trottling is activated, you can try again in 5 minutes, this is for your accout protection"));
			break;
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

Emlab::RegistrationInfo RegistrationFormModel::getData()
{
	Emlab::RegistrationInfo registrationInfo;

	registrationInfo.name = valueText(UserName).toUTF8();
	registrationInfo.email = valueText(UserEmail).toUTF8();
	registrationInfo.phone = valueText(UserPhone).toUTF8();
	registrationInfo.password = valueText(UserPassword).toUTF8();

	return registrationInfo;
}

RegistrationFormView::RegistrationFormView(std::string temp_str, std::shared_ptr<Login> login)
	: Wt::WTemplateFormView(tr(temp_str)),
	model_(std::make_shared<RegistrationFormModel>()),
	login_(login),
	role_(std::make_shared<Wt::WButtonGroup>())
{
	bindWidget("human-svg", std::make_unique<Wt::WText>(tr("human-svg")));
	bindWidget("key-svg", std::make_unique<Wt::WText>(tr("key-svg")));
	bindWidget("email-svg", std::make_unique<Wt::WText>(tr("email-svg")));
	bindWidget("phone-svg", std::make_unique<Wt::WText>(tr("phone-svg")));
	bindWidget("camera-svg", std::make_unique<Wt::WText>(tr("camera-svg")));
	bindString("password-status", "to short");
	bindWidget("password-requirments", std::make_unique<Wt::WText>(tr("password-requirments")));

	auto client_role = bindWidget("user_role_client", std::make_unique<Wt::WRadioButton>(Emlab::CLIENTROLE));
	auto provider_role = bindWidget("user_role_provider", std::make_unique<Wt::WRadioButton>(Emlab::PROVIDERROLE));
	
	role_->addButton(client_role);
	role_->addButton(provider_role);

	role_->setCheckedButton(client_role);
	
	// link Form Model Fields to FormWidget and set up the view
	auto username_input = std::make_unique<Wt::WLineEdit>();
	username_input->setPlaceholderText("Username");
	username_ = username_input.get();
	setFormWidget(RegistrationFormModel::UserName, std::move(username_input));
	username_ = username_input.get();

	auto email_input = std::make_unique<Wt::WEmailEdit>();
	email_input->setPlaceholderText("Email");
	email_ = email_input.get();
	setFormWidget(RegistrationFormModel::UserEmail, std::move(email_input));

	// Phone field
	auto phone_input = std::make_unique<Wt::WLineEdit>();
	phone_input->setPlaceholderText("Phone");
	phone_input->setInputMask("9999999999");
	phone_ = phone_input.get();
	setFormWidget(RegistrationFormModel::UserPhone, std::move(phone_input));


	// Password field
    auto password_input = std::make_unique<Wt::WLineEdit>();
	password_input->setEchoMode(Wt::EchoMode::Password);
	password_input->setPlaceholderText("Repeat Password to make sure");
	password_ = password_input.get();

	auto eye_btn = bindWidget("password-eye", std::make_unique<Wt::WTemplate>(tr("eye-crossed-svg")));
	eye_btn->clicked().connect(this, [=](){
        if(password_->echoMode() == Wt::EchoMode::Password)
        {
            password_->setEchoMode(Wt::EchoMode::Normal);
            eye_btn->setTemplateText(tr("eye-svg"));
                }else {
            password_->setEchoMode(Wt::EchoMode::Password);
            eye_btn->setTemplateText(tr("eye-crossed-svg"));
        }
    });

	setFormWidget(RegistrationFormModel::UserPassword, std::move(password_input));
	
	// Password Repeat field
	auto password_repeat_input = std::make_unique<Wt::WLineEdit>();
	password_repeat_input->setEchoMode(Wt::EchoMode::Password);
	password_repeat_input->setPlaceholderText("Repeat Your Password");
	auto password_repeat_ = password_repeat_input.get();
	auto eye_btn_repeat = bindWidget("password-repeat-eye", std::make_unique<Wt::WTemplate>(tr("eye-crossed-svg")));
	eye_btn_repeat->clicked().connect(this, [=](){
		if(password_repeat_->echoMode() == Wt::EchoMode::Password)
		{
			password_repeat_->setEchoMode(Wt::EchoMode::Normal);
			eye_btn_repeat->setTemplateText(tr("eye-svg"));
				}else {
			password_repeat_->setEchoMode(Wt::EchoMode::Password);
			eye_btn_repeat->setTemplateText(tr("eye-crossed-svg"));
		}
	});
	setFormWidget(RegistrationFormModel::UserPasswordRepeat, std::move(password_repeat_input));

	// Login Btn
	auto submitBtn = std::make_unique<Wt::WPushButton>("Sign Up");
	submitBtn->clicked().connect(this, &RegistrationFormView::process);
	bindWidget("sign-up-btn", std::move(submitBtn));

	profile_registration_status_ = bindWidget("submit-info", std::make_unique<Wt::WText>("submit info"));
	setPhotoUploder();
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
			profile_registration_status_->setText("Passwords are not matching, try again");
			return;
		}
		auto registrationInfo = model_->getData();
		registrationInfo.photo = photo_bytes_interface_;
		registrationInfo.role = role_->checkedButton()->text().toUTF8();

		Emlab::RegistrationResponse registrationResponse = login_->registerUser(registrationInfo);

		if (registrationResponse == Emlab::RegistrationResponse::RegistrationSuccessful)
		{
			profile_registration_status_->setText("Registration Successful");
			// Log User In
			Emlab::LoginInfo loginInfo;
			loginInfo.email = model_->valueText(model_->UserEmail).toUTF8();
			loginInfo.password = model_->valueText(model_->UserPassword).toUTF8();

			Emlab::LoginReturn loginReturn = login_->loginUser(loginInfo);
			login_->login(loginReturn);
		}else if (registrationResponse == Emlab::RegistrationResponse::EmailAlreadyExists)
		{
			profile_registration_status_->setText("User with same Email identified, forgot Password ?");
		}
		
	}else {
		std::cout << "\n\n Model is not valid \n\n";
		if(!model_->validateField(model_->UserName))
		{
			profile_registration_status_->setText("Username should be betweeen 3 and 30 characters");
		}else if(!model_->validateField(model_->UserPhone))
		{
			profile_registration_status_->setText("Phone number should be 10 digits");
		}else if(!model_->validateField(model_->UserEmail))
		{
			profile_registration_status_->setText("Email is not valid, example@gmai.com");
		}else if(!model_->validateField(model_->UserPassword))
		{
			profile_registration_status_->setText("Password dose not respect the rules");
		}else if(!model_->validateField(model_->UserPasswordRepeat))
		{
			profile_registration_status_->setText("Password repeat dose not respect the rules");
		}

	}
}

void RegistrationFormView::setPhotoUploder()
{
	profile_photo_uploder_ = bindWidget("photo-uploader", std::make_unique<Wt::WFileUpload>());
	profile_photo_status_ = bindWidget("profile-photo-uploader-status", std::make_unique<Wt::WText>("Click to add profile picture"));
	profile_photo_ = bindWidget("profile-photo", std::make_unique<Wt::WImage>(Wt::WLink("resources/images/blank-profile-picture.png")));
	   // Upload automatically when the user entered a file.
    profile_photo_uploder_->changed().connect([=] {
        profile_photo_uploder_->upload();
        profile_photo_status_->setText("File upload is changed.");

    });

    // React to a succesprofile_photo_uploder_ll upload.
    profile_photo_uploder_->uploaded().connect([=] {
        profile_photo_status_->setText("File upload is finished.");
        photo_bytes_interface_ = Emlab::imageToBytes(profile_photo_uploder_->spoolFileName());
		Wt::WString photoPath = "resources/registrationImages/" + profile_photo_uploder_->clientFileName();
		std::cout << "\n\n Photo Path : " << photoPath.toUTF8() << "\n\n";
        auto conversion_result = Emlab::bytesToImage(photo_bytes_interface_, photoPath.toUTF8());
		conversion_result ? std::cout << "\n\n Image saved \n\n" : std::cout << "\n\n Image not saved \n\n";
		profile_photo_->setImageLink(Wt::WLink(photoPath.toUTF8()));
    });

    // React to a file upload problem.
    profile_photo_uploder_->fileTooLarge().connect([=] {
        profile_photo_status_->setText("File is too large.");
    });
}

void RegistrationFormView::dev_setValues()
{
	model_->setValue(model_->UserName, "test");
	model_->setValue(model_->UserPhone, "1234567890");
	model_->setValue(model_->UserEmail, "test1@gmail.com");
	model_->setValue(model_->UserPassword, "asdfghj1");
	model_->setValue(model_->UserPasswordRepeat, "asdfghj1");

	updateView(model_.get());

}