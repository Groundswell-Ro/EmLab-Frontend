#include "include/UserSettingsPage.h"

#include <Wt/WMenu.h>
#include <Wt/WMenuItem.h>
#include <Wt/WStackedWidget.h>
#include <Wt/WLink.h>
#include <Wt/WCssDecorationStyle.h>
#include <Wt/WText.h>
#include <Wt/WPushButton.h>
#include <Wt/WAnimation.h>
#include <Wt/WLineEdit.h>
#include <Wt/WDialog.h>
#include <Wt/WBreak.h>
#include <Wt/WDialog.h>
#include <Wt/WTextArea.h>

UserSettingsPage::UserSettingsPage(std::shared_ptr<Login> login)
:    login_(login)
{
    setStyleClass("mx-auto w-full max-w-7xl overflow-y-auto");

    auto tmp = addWidget(std::make_unique<Wt::WTemplate>(tr("profile-content-widget")));
    tmp->setCondition("if-no-profile", true);
    auto create_profile_btn = tmp->bindWidget("create-profile-btn", std::make_unique<Wt::WPushButton>("create provider profile"));
    auto stack = tmp->bindWidget("stack", std::make_unique<Wt::WStackedWidget>());
    auto menu = tmp->bindWidget("menu", std::make_unique<Wt::WMenu>(stack));
    stack->setStyleClass("flex-grow mt-3");

    
    auto settings_general_tmp = createSettingsGeneralWidget("settings-widget");
    auto change_email_tmp = createChangeEmailWidget("settings-widget");
    // auto change_username_tmp = createChangeUsernameWidget("settings-widget");
    auto change_name_tmp = createChangeNameWidget("settings-widget");
    auto change_phone_tmp = createChangePhoneWidget("settings-widget");
    auto change_password_tmp = createChangePasswordWidget("settings-widget");


    auto menu_item_general = menu->addItem("General", std::move(settings_general_tmp));
    auto menu_item_email = menu->addItem("Change Email", std::move(change_email_tmp));
    // auto menu_item_username = menu->addItem("Change Username", std::move(change_username_tmp));
    auto menu_item_name = menu->addItem("Change Name", std::move(change_name_tmp));
    auto menu_item_phone = menu->addItem("Change Phone", std::move(change_phone_tmp));
    auto menu_item_password = menu->addItem("Change Password", std::move(change_password_tmp));

    Wt::WString menu_item_styles = "bg-body-hover-border simple-menu-item text-bold [&>a]:px-8 [&>a]:py-2";
    menu_item_general->setStyleClass(menu_item_styles);
    menu_item_email->setStyleClass(menu_item_styles);
    // menu_item_username->setStyleClass(menu_item_styles);
    menu_item_name->setStyleClass(menu_item_styles);
    menu_item_phone->setStyleClass(menu_item_styles);
    menu_item_password->setStyleClass(menu_item_styles);

    create_profile_btn->clicked().connect(this, &UserSettingsPage::createProfileDialog);

    menu->select(0);
    createProfileDialog();

}

std::unique_ptr<Wt::WTemplate> UserSettingsPage::createSettingsGeneralWidget(std::string tempName)
{
    auto returnWidget = std::make_unique<Wt::WTemplate>(tr(tempName));
    returnWidget->bindString("title", "general");

    auto tmp = returnWidget->bindWidget("data-form-widget", std::make_unique<Wt::WTemplate>(tr("settings-general-data-widget")));
    

    return returnWidget;
}

// those widgets are created into the settings page
std::unique_ptr<Wt::WTemplate> UserSettingsPage::createChangeEmailWidget(std::string tempName)
{
    auto widget_tmp = std::make_unique<Wt::WTemplate>(tr(tempName));
    widget_tmp->bindString("title", "change email");
    
    auto tmp = widget_tmp->bindWidget("data-form-widget", std::make_unique<Wt::WTemplate>(tr("data-form-widget")));
    tmp->bindString("title", "email");
    tmp->bindString("current-data", "croitoriu.93@gmail.com");
    tmp->bindWidget("icon-svg", std::make_unique<Wt::WTemplate>(tr("email-svg")));
    tmp->bindEmpty("submit-info");
    tmp->bindWidget("validation-state", std::make_unique<Wt::WTemplate>("<div class=\"text-green-400 font-semibold\">Validated</div>"));
    auto email_input = tmp->bindWidget("input", std::make_unique<Wt::WLineEdit>());
    auto submit_btn = tmp->bindWidget("submit-btn", std::make_unique<Wt::WPushButton>("submit"));

    submit_btn->clicked().connect(this, [=](){
        tmp->bindString("submit-info", "Email changed successfully!");
    });
    return widget_tmp;
}

std::unique_ptr<Wt::WTemplate> UserSettingsPage::createChangeUsernameWidget(std::string tempName)
{
auto widget_tmp = std::make_unique<Wt::WTemplate>(tr(tempName));
    widget_tmp->bindString("title", "change username");

    auto tmp = widget_tmp->bindWidget("data-form-widget", std::make_unique<Wt::WTemplate>(tr("data-form-widget")));
    tmp->bindString("title", "username");

    tmp->bindString("current-data", "maxuli");

    tmp->bindWidget("icon-svg", std::make_unique<Wt::WTemplate>(tr("human-svg")));
    tmp->bindEmpty("submit-info");
    tmp->bindWidget("validation-state", std::make_unique<Wt::WTemplate>("<div class=\"text-green-400 font-semibold\">Validated</div>"));
    auto email_input = tmp->bindWidget("input", std::make_unique<Wt::WLineEdit>());
    auto submit_btn = tmp->bindWidget("submit-btn", std::make_unique<Wt::WPushButton>("submit"));


    submit_btn->clicked().connect(this, [=](){
        tmp->bindString("submit-info", "Email changed successfully!");
    });
return widget_tmp;
}

std::unique_ptr<Wt::WTemplate> UserSettingsPage::createChangeNameWidget(std::string tempName)
{
auto widget_tmp = std::make_unique<Wt::WTemplate>(tr(tempName));
    widget_tmp->bindString("title", "change name");

    auto tmp = widget_tmp->bindWidget("data-form-widget", std::make_unique<Wt::WTemplate>(tr("data-form-widget")));
    tmp->bindString("title", "name");
    
    tmp->bindString("current-data", "alexandru dan croitoriu");

    tmp->bindWidget("icon-svg", std::make_unique<Wt::WTemplate>(tr("human-svg")));
    tmp->bindEmpty("submit-info");
    tmp->bindWidget("validation-state", std::make_unique<Wt::WTemplate>("<div class=\"text-green-400 font-semibold\">Validated</div>"));
    auto email_input = tmp->bindWidget("input", std::make_unique<Wt::WLineEdit>());
    auto submit_btn = tmp->bindWidget("submit-btn", std::make_unique<Wt::WPushButton>("submit"));

    submit_btn->clicked().connect(this, [=](){
        tmp->bindString("submit-info", "Email changed successfully!");
    });
return widget_tmp;
}

std::unique_ptr<Wt::WTemplate> UserSettingsPage::createChangePhoneWidget(std::string tempName)
{
auto widget_tmp = std::make_unique<Wt::WTemplate>(tr(tempName));
    widget_tmp->bindString("title", "change phone");

    auto tmp = widget_tmp->bindWidget("data-form-widget", std::make_unique<Wt::WTemplate>(tr("data-form-widget")));
    tmp->bindString("title", "phone");

    tmp->bindString("current-data", "0773 132 455");

    tmp->bindWidget("icon-svg", std::make_unique<Wt::WTemplate>(tr("phone-svg")));
    tmp->bindEmpty("submit-info");
    tmp->bindWidget("validation-state", std::make_unique<Wt::WTemplate>("<div class=\"text-green-400 font-semibold\">Validated</div>"));
    auto email_input = tmp->bindWidget("input", std::make_unique<Wt::WLineEdit>());
    auto submit_btn = tmp->bindWidget("submit-btn", std::make_unique<Wt::WPushButton>("submit"));

    submit_btn->clicked().connect(this, [=](){
        tmp->bindString("submit-info", "Email changed successfully!");
    });
    return widget_tmp;
}

std::unique_ptr<Wt::WTemplate> UserSettingsPage::createChangePasswordWidget(std::string tempName)
{
    auto widget_tmp = std::make_unique<Wt::WTemplate>(tr(tempName));
    widget_tmp->bindString("title", "change password");

    auto tmp = widget_tmp->bindWidget("data-form-widget", std::make_unique<Wt::WTemplate>(tr("data-change-password-widget")));
    tmp->bindString("title", "password");

    tmp->bindWidget("key-svg", std::make_unique<Wt::WText>(tr("key-svg")));
    tmp->bindWidget("password-requirments", std::make_unique<Wt::WTemplate>(tr("password-requirments")));
    tmp->bindEmpty("submit-info");
    auto old_password = tmp->bindWidget("old-password", std::make_unique<Wt::WLineEdit>());
    auto new_password = tmp->bindWidget("new-password", std::make_unique<Wt::WLineEdit>());
    auto repeat_password = tmp->bindWidget("repeat-password", std::make_unique<Wt::WLineEdit>());
    
    auto old_pass_eye_btn = tmp->bindWidget("old-password-eye", std::make_unique<Wt::WTemplate>(tr("eye-crossed-svg")));
    auto new_pass_eye_btn = tmp->bindWidget("new-password-eye", std::make_unique<Wt::WTemplate>(tr("eye-crossed-svg")));
    auto repeat_pass_eye_btn = tmp->bindWidget("repeat-password-eye", std::make_unique<Wt::WTemplate>(tr("eye-crossed-svg")));

    auto submit_btn = tmp->bindWidget("submit-btn", std::make_unique<Wt::WPushButton>("Submit"));

    old_password->setEchoMode(Wt::EchoMode::Password);
    new_password->setEchoMode(Wt::EchoMode::Password);
    repeat_password->setEchoMode(Wt::EchoMode::Password);

	old_pass_eye_btn->clicked().connect(this, [=](){
        if(old_password->echoMode() == Wt::EchoMode::Password)
        {
            old_password->setEchoMode(Wt::EchoMode::Normal);
            old_pass_eye_btn->setTemplateText(tr("eye-svg"));
                }else {
            old_password->setEchoMode(Wt::EchoMode::Password);
            old_pass_eye_btn->setTemplateText(tr("eye-crossed-svg"));
        }
    });

    new_pass_eye_btn->clicked().connect(this, [=](){
        if(new_password->echoMode() == Wt::EchoMode::Password)
        {
            new_password->setEchoMode(Wt::EchoMode::Normal);
            new_pass_eye_btn->setTemplateText(tr("eye-svg"));
                }else {
            new_password->setEchoMode(Wt::EchoMode::Password);
            new_pass_eye_btn->setTemplateText(tr("eye-crossed-svg"));
        }
    });

    repeat_pass_eye_btn->clicked().connect(this, [=](){
        if(repeat_password->echoMode() == Wt::EchoMode::Password)
        {
            repeat_password->setEchoMode(Wt::EchoMode::Normal);
            repeat_pass_eye_btn->setTemplateText(tr("eye-svg"));
                }else {
            repeat_password->setEchoMode(Wt::EchoMode::Password);
            repeat_pass_eye_btn->setTemplateText(tr("eye-crossed-svg"));
        }
    });


return widget_tmp;
}

void UserSettingsPage::createProfileDialog()
{
    auto dialog = addChild(std::make_unique<Wt::WDialog>());
    dialog->setHidden(false, Wt::WAnimation(Wt::AnimationEffect::SlideInFromTop, Wt::TimingFunction::EaseInOut, 500));
    dialog->setOffsets(20, Wt::Side::Top | Wt::Side::Left | Wt::Side::Right);
    dialog->rejectWhenEscapePressed();
    
    auto header = dialog->titleBar();
    auto content = dialog->contents();
    auto footer = dialog->footer();

    dialog->setResizable(false);
    dialog->setMovable(false);
    // dialog->setStyleClass("basic-widget relative max-w-[95vw] min-h-[50%] max-h-[90%] shadow-lg overflow-x-hidden border-0");
    content->setStyleClass("overflow-y-scroll my-5");
    // footer->setStyleClass("flex justify-between items-center absolute bottom-0 left-0 w-full");

    auto content_temp = content->addWidget(std::make_unique<Wt::WTemplate>(tr("profile-dialog-content")));

    auto photo_uploder = content_temp->bindWidget("photo-uploder-template", std::make_unique<PhotoUploder>());
    photo_uploder->setPhoto(login_->getUserPhotoPath() + "/profile.jpg");
    photo_uploder->status_->setText("Is this photo ok as your profile photo?");

    auto username_input_temp = content_temp->bindWidget("username-input", std::make_unique<Wt::WTemplate>(tr("input-template-normal")));
    username_input_temp->setCondition("if-start-svg", true);
    username_input_temp->bindString("label", "Username");
    username_input_temp->bindWidget("start-svg", std::make_unique<Wt::WTemplate>(tr("human-svg")));
    auto username_input = username_input_temp->bindWidget("input", std::make_unique<Wt::WLineEdit>());
    username_input->setAutoComplete(false);


    auto about_self_temp = content_temp->bindWidget("about-self-input", std::make_unique<Wt::WTemplate>(tr("input-template-normal")));
    about_self_temp->bindString("label", "About self as a sevice\'s provider");
    auto about_self_input = about_self_temp->bindWidget("input", std::make_unique<Wt::WTextArea>());
    about_self_input->addStyleClass("resize-none resize-y");
    // header / footer setup
    header->clear();
    header->addWidget(std::make_unique<Wt::WText>("Create Provider Profile"))->setStyleClass("text-cemter text-2xl font-semibold");
    header->setStyleClass("text-center");
    // auto cancel_btn = footer->addWidget(std::make_unique<Wt::WPushButton>("Cancel"));
    // auto create_btn = footer->addWidget(std::make_unique<Wt::WPushButton>("Create"));

    // create_btn->setStyleClass("btn btn-primary");
    // cancel_btn->setStyleClass("btn btn-danger");

    // create_btn->clicked().connect([=]{ dialog->accept(); });
    // cancel_btn->clicked().connect([=]{ dialog->reject(); });

    dialog->finished().connect([=]{
        if(dialog->result() == Wt::DialogCode::Accepted)
        {
            std::cout << "\n\n dialog accepted protocol here \n\n";
        }else 
        {
            std::cout << "\n\n dialog rejected protocol here \n\n";
        }
        removeWidget(dialog);
    });
    // dialog->show();

}
