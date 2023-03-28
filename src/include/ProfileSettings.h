#include "LoginObj.h"
#include "../../../comunication/AuthModule.h"
#include "../../../comunication/EventDataModule.h"
#include <Wt/WTemplate.h>
#include <Wt/WPopupMenu.h>
#include <Wt/WPushButton.h>

class ProfileSettings : public Wt::WTemplate
{
public:
    ProfileSettings(std::shared_ptr<Login> login);
    std::shared_ptr<Login> login_;
    Wt::WTemplate *profile_;
    Wt::WTemplate *other_;

private:
    void setProfileMenu();
    void setOtherMenu();
};
