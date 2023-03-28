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
    void addEventDialog();

private:
    void addServiceToEventDialog(int eventId);
    void displayEventsData();
    void addEventToList(EventDataModule::EventDataPack eventDataPack);
    void removeEvent(int eventId);
    void eventClientChanged(Wt::WString clientName, int eventId);
    void eventDateChanged(Wt::WDate eventDate, int eventId);
    void registerEvent(EventDataModule::EventDataPack eventDataPack);
};