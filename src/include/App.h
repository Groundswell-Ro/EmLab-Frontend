#pragma once
#include "LoginObj.h"
// #include "ProfilePage.h"
// #include "Events.h"
#include "Auth.h"
// #include "AuthWidget.h"
#include "HomePage.h"
#include "TestPage.h"

#include <Wt/WContainerWidget.h>
#include <Wt/WStackedWidget.h>
#include <Wt/WMenu.h>
#include <Wt/WTemplate.h>
#include <iostream>
#include <Wt/WApplication.h>
#include <Wt/WCheckBox.h>

class EventManagerLab : public Wt::WContainerWidget
{
public:
    EventManagerLab();


private:
    std::shared_ptr<Login> login_;

    Wt::WTemplate* navbar_;
    Wt::WStackedWidget* stack_;
    Wt::WMenu* nav_menu_;
    Wt::WMenu* user_menu_;
    Wt::WTemplate* user_menu_wrapper_;

    Wt::WMenuItem* current_menu_item_;
    void createAuth();
    void createApp();
    void handleUserAuth();
    std::unique_ptr<Wt::WPushButton> createThemeSwitcher();
    // this only handles the underline styles of the nav menu and user menu
    void menuItemSelected(Wt::WMenuItem* selecter_item); 
};