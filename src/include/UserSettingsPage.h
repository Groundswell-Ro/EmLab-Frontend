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
    void createSettingsPage();
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
    std::unique_ptr<Wt::WPushButton> createThemeSwitcher();
    // void createProviderProfile();

    // those methods call the backend to make changes
    Emlab::ChangeUniqueDataResponse changeEmail(std::string email);
    Emlab::ChangeUniqueDataResponse changePhone(std::string phone);
    Emlab::ChangePasswordResponse changePassword(std::string oldPassword, std::string newPassword);

    int setUserProviderProfile(Emlab::ProviderProfileInfo providerProfileInfo);
    int setProviderService(Emlab::ProviderServiceInfo providerServiceInfo);
    void addPhotoToService(int serviceId, Emlab::ImageInfo imageInfo);

    bool setName(std::string name);
    bool changePhoto(std::string photoPath);
    


};