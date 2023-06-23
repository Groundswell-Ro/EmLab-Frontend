#pragma once
#include "LoginObj.h"
#include "../../../comunication/comm/ReviewInterface.h"

#include <Wt/WTemplate.h>
#include <Wt/WContainerWidget.h>
#include <Wt/WMenu.h>
#include <Wt/WMenuItem.h>
#include <Wt/WStackedWidget.h>
#include <Wt/WLink.h>
#include <Wt/WCssDecorationStyle.h>
#include <Wt/WText.h>
#include <Wt/WImage.h>
#include <Wt/WPushButton.h>
#include <Wt/WAnimation.h>
#include <Wt/WLineEdit.h>
#include <Wt/WDialog.h>
#include <Wt/WBreak.h>

class UserSettingsPage : public Wt::WContainerWidget
{
public:
        UserSettingsPage(std::shared_ptr<Login> login);
private:
    Wt::WImage* profile_image_;

    // those widgets are created into the settings page
    std::unique_ptr<Wt::WTemplate> createSettingsGeneralWidget(std::string tempName);
    std::unique_ptr<Wt::WTemplate> createChangeEmailWidget(std::string tempName);
    std::unique_ptr<Wt::WTemplate> createChangeUsernameWidget(std::string tempName);
    std::unique_ptr<Wt::WTemplate> createChangeNameWidget(std::string tempName);
    std::unique_ptr<Wt::WTemplate> createChangePhoneWidget(std::string tempName);
    std::unique_ptr<Wt::WTemplate> createChangePasswordWidget(std::string tempName);

private:
    std::shared_ptr<Login> login_;
    void createProfileDialog();

};