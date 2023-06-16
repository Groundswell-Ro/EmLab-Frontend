#pragma once
#include "LoginObj.h"

#include <Wt/WFormModel.h>
#include <Wt/WTemplateFormView.h>

// Validators
#include <Wt/WValidator.h>
#include <Wt/WLengthValidator.h>
#include <Wt/WRegExpValidator.h>
#include <Wt/WEmailValidator.h>
#include <Wt/WEmailEdit.h>

// Form Widgets
#include <Wt/WLineEdit.h>
#include <Wt/WPushButton.h>
#include <Wt/WFileUpload.h>
#include <Wt/WRadioButton.h>
#include <Wt/WRadioButton.h>

// Ice
#include <Ice/Ice.h>
#include <stdexcept>


#include <Ice/Ice.h> // For ZeroC Ice
#include <IceUtil/IceUtil.h> // For IceUtil::byteSeqFromSTLContainer and Ice::createInputStream
// #include <opencv2/opencv.h> // For OpenCV


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
	LoginFormView(std::string temp_str, std::shared_ptr<Login> login);
	Wt::WEmailEdit* email_;	
	Wt::WLineEdit* password_;

	void process();
private:
	std::shared_ptr<Login> login_;
	std::shared_ptr<LoginFormModel> model_;
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
	RegistrationFormView(std::string temp_str, std::shared_ptr<Login> login);
	Wt::WText* profile_photo_status_;
	Wt::WText* profile_registration_status_;
	Wt::WLineEdit* username_;
	Wt::WEmailEdit* email_;
	Wt::WLineEdit* phone_;
	Wt::WLineEdit* password_;
	Wt::WLineEdit* password_repeat_;
	Wt::WFileUpload* profile_photo_uploder_;
	Wt::WImage* profile_photo_;
	std::shared_ptr<Wt::WButtonGroup> role_;
	void process();
	void dev_setValues();
private:

	AuthModule::ImageData photo_bytes_interface_;

	std::shared_ptr<RegistrationFormModel> model_;
	std::shared_ptr<Login> login_;
	void setPhotoUploder();
};