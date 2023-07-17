#pragma once
#include "../../utils/include/LoginObj.h"
#include "FindServicesPage.h"
#include "OrganizerPage.h"

#include <Wt/WApplication.h>
#include <Wt/WContainerWidget.h>

#include <Wt/WStackedWidget.h>
#include <Wt/WTemplate.h>
#include <Wt/WString.h>
#include <Wt/WMenu.h>
#include <Wt/WPopupMenu.h>
#include <Wt/WMenuItem.h>
#include <Wt/WEnvironment.h>
#include <Wt/WServer.h>
#include <Wt/Dbo/Exception.h>

class EventManagerLab : public Wt::WApplication
{
public:
    EventManagerLab(const Wt::WEnvironment &env);
    ~EventManagerLab();
private:
    std::shared_ptr<Login> login_;

    Wt::WTemplate* navbar_;
    Wt::WStackedWidget* stack_;
    Wt::WMenu* nav_menu_;

    Wt::WContainerWidget* portofolio_page_;

    FindServicesPage* services_page_;
    OrganizerPage* organizer_page_;
    
    Wt::WPopupMenu* user_menu_;

    Wt::WMenuItem* current_menu_item_;
    void createAuth();
    void createApp();
    void handleUserAuth();
    std::unique_ptr<Wt::WPushButton> createThemeSwitcher();
    // this only handles the underline styles of the nav menu and user menu
    void menuItemSelected(Wt::WMenuItem* selecter_item); 
    void handleInternalPathChange(const std::string& path);
};