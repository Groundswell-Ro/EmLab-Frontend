#include "LoginObj.h"
#include "ProfileSettings.h"
#include "../../../comunication/AuthModule.h"
#include "../../../comunication/EventDataModule.h"
#include <Wt/WTemplate.h>
#include <Wt/WPopupMenu.h>
#include <Wt/WPushButton.h>

class ProfilePage : public Wt::WTemplate
{
public:
    ProfilePage(std::shared_ptr<Login> login);
    std::shared_ptr<Login> login_;

    Wt::WTemplate *header_;
    Wt::WTemplate *menu_;
    Wt::WTemplate *stats_;
    Wt::WTemplate *description_;
    Wt::WTemplate *work_;
    ProfileSettings *settings_profile_;
    Wt::WTemplate *settings_profile_menu_;

    AuthModule::ServiceInfoSq services_;

private:
    void setProfileData();
    void setServicesMenu();
    void setWorkHistoryMenu();
    void setSettingsMenu();
    void addServiceDialog();
    void modifyServiceDialog();
};
