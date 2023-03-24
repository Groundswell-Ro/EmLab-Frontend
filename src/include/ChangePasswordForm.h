#pragma once
#include "LoginObj.h"
#include <Wt/WFormModel.h>
#include <Wt/WTemplateFormView.h>
#include <Wt/WLineEdit.h>
#include <Wt/WValidator.h>
#include <Wt/WPushButton.h>

// E V E N T _____________ M O D E L
class ChangePasswordFormModel : public Wt::WFormModel
{
public:
    // Associate each field with a unique string literal.
    static const Field OldPasswordField;
    static const Field NewPasswordField;
    static const Field RepeatPasswordField;
    ChangePasswordFormModel();
    // EventDataModule::EventData getData();
private:
};

// E V E N T _____________ V I E W
class ChangePasswordFormView : public Wt::WTemplateFormView
{
public:
    ChangePasswordFormView(std::shared_ptr<Login> login);
    
    std::shared_ptr<Login> login_;
    std::shared_ptr<ChangePasswordFormModel> model_;
    
    Wt::WLineEdit *oldPasswordEdit_;
    Wt::WLineEdit *newPasswordEdit_;
    Wt::WLineEdit *repeatPasswordEdit_;

    Wt::WPushButton *submitBtn_;
    Wt::WPushButton *changeBtn_;

private:
    void process();
    void opecAccess(bool access = true);
};
