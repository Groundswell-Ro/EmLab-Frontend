#pragma once
#include "LoginObj.h"
// #include "ProfilePage.h"
// #include "Events.h"
#include "Auth.h"
// #include "AuthWidget.h"
#include "PortofoliosPage.h"
#include "TestPage.h"
#include "UserSettingsPage.h"

#include <Wt/WContainerWidget.h>
#include <Wt/WStackedWidget.h>
#include <Wt/WMenu.h>
#include <Wt/WTemplate.h>
#include <iostream>
#include <Wt/WApplication.h>
#include <Wt/WCheckBox.h>
#include <Wt/WPopupMenu.h>
#include <Wt/WMessageBox.h>
class EventManagerLab : public Wt::WContainerWidget
{
public:
    EventManagerLab();


private:
    std::shared_ptr<Login> login_;

    Wt::WTemplate* navbar_;
    Wt::WStackedWidget* stack_;
    Wt::WMenu* nav_menu_;

    Wt::WPopupMenu* user_menu_;

    Wt::WMenuItem* current_menu_item_;
    void createAuth();
    void createApp();
    void handleUserAuth();
    std::unique_ptr<Wt::WPushButton> createThemeSwitcher();
    // this only handles the underline styles of the nav menu and user menu
    void menuItemSelected(Wt::WMenuItem* selecter_item); 
};