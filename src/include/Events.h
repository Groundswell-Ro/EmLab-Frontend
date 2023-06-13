#pragma once
#include <Wt/WContainerWidget.h>
#include <Wt/WStackedWidget.h>
#include "LoginObj.h"
#include <Wt/WDate.h>
#include "../../../comunication/EventDataModule.h"
class Events : public Wt::WContainerWidget
{
public:
    Events(std::shared_ptr<Login> login);
    std::shared_ptr<Login> login_;

    Wt::WContainerWidget *eventsMenu_;
    Wt::WStackedWidget *eventsContentStack_;
    void addEventDialog();
    Wt::WText *eventsMenuTitle_;

    Wt::WDate currentDiplayDate_ = Wt::WDate::currentServerDate();
    std::string dateFormat = "dd/MM/yyyy";
    std::string dateFormat_presentation = "dd-MMMM";
private:
    void addServiceToEventDialog(int eventId);
    void copyEventDataDialog(EventDataModule::EventDataPack eventDataPack);
    void displayEventsData();
    void addEventToList(EventDataModule::EventDataPack eventDataPack, bool active = false);
    void removeEvent(int eventId);
    void eventClientChanged(Wt::WString clientName, int eventId);
    void eventDateChanged(Wt::WDate eventDate, int eventId);
    void registerEvent(EventDataModule::EventDataPack eventDataPack);
    
};