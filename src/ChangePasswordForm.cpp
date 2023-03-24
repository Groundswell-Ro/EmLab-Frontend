#include "include/ChangePasswordForm.h"
#include "include/AuthForms.h"

#include <Ice/Ice.h>
#include <stdexcept>


// Event fields
const Wt::WFormModel::Field ChangePasswordFormModel::OldPasswordField = "last-password";
const Wt::WFormModel::Field ChangePasswordFormModel::NewPasswordField = "new-password";
const Wt::WFormModel::Field ChangePasswordFormModel::RepeatPasswordField = "new-password-repeat";


// E V E N T _____________ M O D E L
ChangePasswordFormModel::ChangePasswordFormModel()
    : WFormModel()
{
    addField(OldPasswordField);
    addField(NewPasswordField);
    addField(RepeatPasswordField);
    
    setValidator(OldPasswordField, createUserPasswordValidator());
    setValidator(NewPasswordField, createUserPasswordValidator());
    setValidator(RepeatPasswordField, createUserPasswordValidator());
}

// E V E N T _____________ V I E W
ChangePasswordFormView::ChangePasswordFormView(std::shared_ptr<Login> login)
    : WTemplateFormView(tr("change-password-template")),
    model_(std::make_shared<ChangePasswordFormModel>()),
    login_(login)
{
    addFunction("id", &WTemplate::Functions::id);
    setMinimumSize(Wt::WLength(280, Wt::LengthUnit::Pixel), Wt::WLength::Auto);
    setMaximumSize(Wt::WLength(350, Wt::LengthUnit::Pixel), Wt::WLength::Auto);

    /*
     * Old Password
     */
    auto oldPasswordEdit = std::make_unique<Wt::WLineEdit>();
    oldPasswordEdit->setPlaceholderText("old password");
    oldPasswordEdit->setEchoMode(Wt::EchoMode::Password);
    oldPasswordEdit_ = oldPasswordEdit.get();
    setFormWidget(model_->OldPasswordField, std::move(oldPasswordEdit));

    /*
     * New Password
     */
    auto newPasswordEdit = std::make_unique<Wt::WLineEdit>();
    newPasswordEdit->setEchoMode(Wt::EchoMode::Password);
    newPasswordEdit->setPlaceholderText("new password");
    newPasswordEdit_ = newPasswordEdit.get();
    setFormWidget(model_->NewPasswordField, std::move(newPasswordEdit));

    /*
     * Repeat Password
     */
    auto repeatPasswordEdit = std::make_unique<Wt::WLineEdit>();
    repeatPasswordEdit->setEchoMode(Wt::EchoMode::Password);
    repeatPasswordEdit->setPlaceholderText("repeat new password");
    repeatPasswordEdit_ = repeatPasswordEdit.get();
    setFormWidget(model_->RepeatPasswordField, std::move(repeatPasswordEdit));

    bindString("submit-info", Wt::WString(""));
    submitBtn_ = bindWidget("submit-btn", std::make_unique<Wt::WPushButton>("Submit"));
    changeBtn_ = bindWidget("change-btn", std::make_unique<Wt::WPushButton>("Change"));

    submitBtn_->clicked().connect(this, &ChangePasswordFormView::process);
    changeBtn_->clicked().connect(this, [=](){
    opecAccess(true);});

    opecAccess(false);

}

void ChangePasswordFormView::opecAccess(bool access)
{
    if(access){
        model_->setReadOnly(model_->OldPasswordField, false);
        model_->setReadOnly(model_->NewPasswordField, false);
        model_->setReadOnly(model_->RepeatPasswordField, false);
        submitBtn_->show();
        changeBtn_->hide();

    }else {
        model_->setReadOnly(model_->OldPasswordField, true);
        model_->setReadOnly(model_->NewPasswordField, true);
        model_->setReadOnly(model_->RepeatPasswordField, true);
        submitBtn_->hide();
        changeBtn_->show();
    } 
    updateView(model_.get());
}

void ChangePasswordFormView::process()
{
    updateModel(model_.get());
    auto oldPassword = model_->valueText(model_->OldPasswordField);
    auto newPassword = model_->valueText(model_->NewPasswordField);
	auto repeatPassword = model_->valueText(model_->RepeatPasswordField);
    AuthModule::ChangePasswordResponse response;

    if(newPassword != repeatPassword){
        bindString("submit-info", Wt::WString("Passwords do not match"));
        model_->setValue(model_->NewPasswordField, "");
        model_->setValue(model_->RepeatPasswordField, "");
        newPasswordEdit_->setFocus();
        model_->setValidation(model_->NewPasswordField, Wt::WValidator::Result(Wt::ValidationState::Invalid, "Passwords do not match"));
        model_->setValidation(model_->RepeatPasswordField, Wt::WValidator::Result(Wt::ValidationState::Invalid, "Passwords do not match"));
        updateView(model_.get());
        return;
    }
    // Ice communication for getting clients from server
    try
    {
        Ice::CommunicatorHolder ich = Ice::initialize();
        auto base = ich->stringToProxy(login_->authConnString_);
		auto authInterface = Ice::checkedCast<AuthModule::AuthInterfacePrx>(base);
        if (!authInterface)
			{
				throw std::runtime_error("Invalid proxy");
			}
            response = authInterface->tryChangePassword(login_->userToken(), oldPassword.toUTF8(), newPassword.toUTF8());
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }

    if(response == AuthModule::ChangePasswordResponse::PasswordChanged){
        bindString("submit-info", Wt::WString("Password changed succesfuly"));
        model_->setValidation(model_->NewPasswordField, Wt::WValidator::Result(Wt::ValidationState::Valid));
        model_->setValidation(model_->RepeatPasswordField, Wt::WValidator::Result(Wt::ValidationState::Valid));
        model_->setValidation(model_->OldPasswordField, Wt::WValidator::Result(Wt::ValidationState::Valid));
        opecAccess(false);
        updateView(model_.get());
    }else if (response == AuthModule::ChangePasswordResponse::OldPasswordIncorrect){
        bindString("submit-info", Wt::WString("Wrong old password"));
        model_->setValue(model_->OldPasswordField, "");
        model_->setValidation(model_->OldPasswordField, Wt::WValidator::Result(Wt::ValidationState::Invalid, "Wrong password"));
        oldPasswordEdit_->setFocus();
        updateView(model_.get());
    }else if (response == AuthModule::ChangePasswordResponse::ThrottlingActivated){
        bindString("submit-info", Wt::WString("Throttling activated"));
        model_->setValue(model_->OldPasswordField, "");
        model_->setValidation(model_->OldPasswordField, Wt::WValidator::Result(Wt::ValidationState::Invalid, "Throttling activated"));
        oldPasswordEdit_->setFocus();
        updateView(model_.get());
    }


}