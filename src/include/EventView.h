#pragma once
#include "LoginObj.h"
#include "ClientForm.h"
#include "ServiceForm.h"
#include "EventForm.h"

#include <Wt/WTemplate.h>
#include <Wt/WText.h>
#include <Wt/WPushButton.h>
#include <Wt/WSignal.h>
#include <Wt/WString.h>
#include <Wt/WContainerWidget.h>
#include <Wt/WPanel.h>

// EventView captures whole event forms (client,event,service)
class EventView : public Wt::WContainerWidget
{
public:
    EventView(std::shared_ptr<Login> login);
    std::shared_ptr<Login> login_;

    ClientFormView *clientForm_;
    EventFormView *eventForm_;

    Wt::WContainerWidget *eventFormWrapper_;
    Wt::WContainerWidget *servicesWrapper_;

    Wt::WPushButton *submitBtn_;

    void addService();
    void setData(EventDataModule::EventDataPack eventDataPack);
    void refreshData();
    void registerNewService(EventDataModule::ServiceData serviceData);
    void removeService(int serviceId);
    void removeServiceDialog(EventDataModule::ServiceData serviceData);
    bool firstSetData_ = true;
    Wt::Signal<Wt::WString, int> &eventChangedClient() { return eventChangedClient_; };

private:
    void clientChanged(Wt::WString clientName, int newClient);
    Wt::Signal<Wt::WString, int> eventChangedClient_;

};