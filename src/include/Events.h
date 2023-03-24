#pragma once
#include <Wt/WContainerWidget.h>
#include <Wt/WStackedWidget.h>
#include "LoginObj.h"
#include "AddEventDialog.h"

class Events : public Wt::WContainerWidget
{
public:
    Events(std::shared_ptr<Login> login);
    std::shared_ptr<Login> login_;

    Wt::WContainerWidget *eventsMenu_;
    Wt::WStackedWidget *eventsContentStack_;

    Wt::WPushButton *delEventBtn_;
    Wt::WPushButton *addServiceBtn_;

private:
    void addServiceToEventDialog();
    void displayEventsData();
    void addEventToList(EventDataModule::EventDataPack eventDataPack);
    void removeEvent();
    void eventClientChanged(Wt::WString clientName, int eventId);
    void eventDateChanged(Wt::WDate eventDate, int eventId);
};