#include "include/UserSettingsPage.h"

UserSettingsPage::UserSettingsPage(std::shared_ptr<Login> login)
:    login_(login)
{
    setStyleClass("mx-auto w-full max-w-7xl !overflow-y-scroll");

    auto tmp = addWidget(std::make_unique<Wt::WTemplate>(tr("profile-content-widget")));
    auto stack = tmp->bindWidget("stack", std::make_unique<Wt::WStackedWidget>());
    auto menu = tmp->bindWidget("menu", std::make_unique<Wt::WMenu>(stack));
    // stack->setTransitionAnimation(Wt::WAnimation(Wt::AnimationEffect::Fade), true);
    menu->setStyleClass("basic-widget mt-0 flex flex-col text-center min-w-[200px] max-w-[600px] list-none");
    stack->setStyleClass("flex-grow");

    auto settings_general_tmp = createSettingsGeneralWidget("settings-widget");
    auto change_email_tmp = createChangeEmailWidget("settings-widget");
    auto change_username_tmp = createChangeUsernameWidget("settings-widget");
    auto change_name_tmp = createChangeNameWidget("settings-widget");
    auto change_phone_tmp = createChangePhoneWidget("settings-widget");
    auto change_password_tmp = createChangePasswordWidget("settings-widget");

    auto menu_item_general = menu->addItem("General", std::move(settings_general_tmp));
    auto menu_item_email = menu->addItem("Change Email", std::move(change_email_tmp));
    auto menu_item_username = menu->addItem("Change Username", std::move(change_username_tmp));
    auto menu_item_name = menu->addItem("Change Name", std::move(change_name_tmp));
    auto menu_item_phone = menu->addItem("Change Phone", std::move(change_phone_tmp));
    auto menu_item_password = menu->addItem("Change Password", std::move(change_password_tmp));

    Wt::WString menu_item_styles = "bg-body-hover-border simple-menu-item py-2 px-3 text-bold";
    menu_item_general->setStyleClass(menu_item_styles);
    menu_item_email->setStyleClass(menu_item_styles);
    menu_item_username->setStyleClass(menu_item_styles);
    menu_item_name->setStyleClass(menu_item_styles);
    menu_item_phone->setStyleClass(menu_item_styles);
    menu_item_password->setStyleClass(menu_item_styles);
    

    menu->select(0);

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