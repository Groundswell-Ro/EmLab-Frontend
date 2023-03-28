#include "include/ProfileSettings.h"
#include "include/ChangePasswordForm.h"
#include <Wt/WDialog.h>

ProfileSettings::ProfileSettings(std::shared_ptr<Login> login)
    : Wt::WTemplate(tr("profile-settings-widget-template")),
      login_(login)
{
    bindEmpty("profile-settings-dialog");

    profile_ = bindWidget("profile-settings-widget",
                          std::make_unique<Wt::WTemplate>(tr("profile-settings-template")));

    other_ = bindWidget("profile-other-widget",
                        std::make_unique<Wt::WTemplate>(tr("profile-settings-template")));
    setProfileMenu();
    setOtherMenu();

    other_->hide();
}

void ProfileSettings::setProfileMenu()
{
    // binding templates to the menu
    profile_->bindString("settings-title", Wt::WString("Profile Settings"));

    // bind templates to profile_ template widget
    auto userDataMenuItem = profile_->bindWidget("settings-nav-item-submenu-1",
                                                 std::make_unique<Wt::WTemplate>(tr("settings-nav-item-template")));
    auto seccondMenuItem = profile_->bindWidget("settings-nav-item-submenu-2",
                                                std::make_unique<Wt::WTemplate>(tr("settings-nav-item-template")));

    auto changePasswordWidget = profile_->bindWidget("change-password-widget",
                                                     std::make_unique<ChangePasswordFormView>(login_));

    // setting title of the menu
    userDataMenuItem->bindString("settings-item-title", Wt::WString("User Data"));

    // react to the click event
    userDataMenuItem->clicked().connect([=]()
                                        {
        if(userDataMenuItem->hasStyleClass("active")){
            return;
        }else {
            userDataMenuItem->addStyleClass("active");
            seccondMenuItem->removeStyleClass("active");
            changePasswordWidget->show();
        } });
    seccondMenuItem->clicked().connect([=]()
                                       {
        if(seccondMenuItem->hasStyleClass("active")){
            return;
        }else {
            seccondMenuItem->addStyleClass("active");
            userDataMenuItem->removeStyleClass("active");
            changePasswordWidget->hide();
        } });
}

void ProfileSettings::setOtherMenu()
{
    // binding templates to the menu
    other_->bindString("settings-title", Wt::WString("Other Settings"));

    // bind templates to other_ template widget
    auto otherMenuItem_1 = other_->bindWidget("settings-nav-item-submenu-1",
                                              std::make_unique<Wt::WTemplate>(tr("settings-nav-item-template")));

    // setting title of the menu
    otherMenuItem_1->bindString("settings-item-title", Wt::WString("menu one"));
}
