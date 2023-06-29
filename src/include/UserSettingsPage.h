#pragma once

#include "../../utils/include/LoginObj.h"
#include "../../utils/include/PhotoUploder.h"

#include <Wt/WImage.h>
#include <Wt/WContainerWidget.h>
#include <Wt/WTemplate.h>


// #include 

class UserSettingsPage : public Wt::WContainerWidget
{
public:
        UserSettingsPage(std::shared_ptr<Login> login);
private:
    Wt::WImage* profile_image_;

    // those widgets are created into the settings page
    std::unique_ptr<Wt::WTemplate> createSettingsGeneralWidget(std::string tempName);
    std::unique_ptr<Wt::WTemplate> createChangeEmailWidget(std::string tempName);
    std::unique_ptr<Wt::WTemplate> createChangeNameWidget(std::string tempName);
    std::unique_ptr<Wt::WTemplate> createChangePhoneWidget(std::string tempName);
    std::unique_ptr<Wt::WTemplate> createChangePasswordWidget(std::string tempName);
private:
    std::shared_ptr<Login> login_;
    void createProfileDialog();
    void createProviderProfile();

    Emlab::ChangeUniqueDataResponse changeEmail(std::string email);


};