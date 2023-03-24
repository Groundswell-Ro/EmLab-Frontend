#pragma once
#include "LoginObj.h"
#include "AuthWidget.h"
#include "../../../comunication/EventDataModule.h"
#include "AddEventDialog.h"

#include <Wt/WContainerWidget.h>
#include <Wt/WStackedWidget.h>
#include <Wt/WMenu.h>
#include <Wt/WButtonGroup.h>

class EventManagerLab : public Wt::WContainerWidget
{
public:
    EventManagerLab();

    std::shared_ptr<Login> login_;

    AuthWidget *authWidget_;

    Wt::WNavigationBar *navigation_;
    Wt::WStackedWidget *contentsStack_;
    EventDataModule::SeqClients seqClients_;

private:
    void handleUserAuth();
    void createApp();
};