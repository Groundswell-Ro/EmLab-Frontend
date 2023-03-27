#include "include/EventView.h"
#include <Wt/WDate.h>
#include "../../comunication/EventDataModule.h"
#include <Ice/Ice.h>
#include <stdexcept>
#include <Wt/WDialog.h>
#include <Wt/WMessageBox.h>

EventView::EventView(std::shared_ptr<Login> login)
    : WContainerWidget(),
      login_(login)
{
    setStyleClass("d-flex overflow-auto h-100");
    eventFormWrapper_ = addWidget(std::make_unique<Wt::WContainerWidget>());
    servicesWrapper_ = addWidget(std::make_unique<Wt::WContainerWidget>());

    auto eventForm = std::make_unique<EventFormView>(login);

    clientForm_ = eventFormWrapper_->addWidget(std::make_unique<ClientFormView>(login));
    eventFormWrapper_->addWidget(std::make_unique<Wt::WText>("<hr class=\"border-top border-primary m-0\" />"));
    eventForm_ = eventFormWrapper_->addWidget(std::move(eventForm));

    servicesWrapper_->setStyleClass("flex-grow-1 accordion overflow-auto");
    servicesWrapper_->setHeight(Wt::WLength(80, Wt::LengthUnit::ViewportHeight));
    eventFormWrapper_->setStyleClass("");
}

void EventView::setData(EventDataModule::EventDataPack eventDataPack)
{

    eventForm_->setData(eventDataPack.eventData);
    clientForm_->setData(eventDataPack.clientData);

    if (firstSetData_)
    {
        clientForm_->clientChanged().connect(this, &EventView::clientChanged);
        eventForm_->setFieldsSignals();
        firstSetData_ = false;
        eventChangedClient_.emit(eventDataPack.clientData.name, eventDataPack.eventData.id);
    }
    else
    {
        servicesWrapper_->clear();
    }

    for (int i = 0; i < eventDataPack.seqServices.size(); ++i)
    {
        auto serviceData = eventDataPack.seqServices.at(i);

        auto serviceFormWidget = servicesWrapper_->addWidget(std::make_unique<ServiceFormWidget>(login_, eventForm_->model_));
        serviceFormWidget->serviceForm_->setFieldsSignals();

        // Set Service Values
        serviceFormWidget->serviceForm_->setData(serviceData);
        serviceFormWidget->deleteService().connect(this, [=]()
                                                   { removeServiceDialog(serviceData); });
        serviceFormWidget->setSettingsMenu();
    }
}

void EventView::addService()
{
    if (login_->user().servicesInfoSq.size() == 0)
    {
        createNoServicesMessage();
    }
    else
    {
        auto serviceFormWidget = servicesWrapper_->addWidget(std::make_unique<ServiceFormWidget>(login_, eventForm_->model_));
    }
}

void EventView::createNoServicesMessage()
{
    auto messageBox = addChild(std::make_unique<Wt::WMessageBox>(
        "Nu ai nici un serviciu!",
        "<p>Pentru a adauga servicii unui eveniment ai doua optiuni</p>"
        "<p>1. Trebuie tu sa ai servicii, pe care le poti adauga din pagina de profil</p>"
        "<p>2. Daca ai prieteni pe platforma care au servicii poti adauga din serviciile lor</p>"
        "<p>Din pacate observ ca nu ai nici tu servicii si nici prieteni</p>"
        "Prin urmare va trebui sa creezi un serviciu pentru a putea adauga servicii la evenimente",
        Wt::Icon::Information,
        Wt::StandardButton::Ok));

    messageBox->setMovable(false);

    messageBox->buttonClicked().connect([=]
                                        {
			if (messageBox->buttonResult() == Wt::StandardButton::Ok)
				removeChild(messageBox); });
    messageBox->show();
}

void EventView::clientChanged(Wt::WString clientName, int clientId)
{
    // Ice communication for modifying event client
    try
    {
        Ice::CommunicatorHolder ich = Ice::initialize();
        auto base = ich->stringToProxy(login_->eventConnString_);
        auto eventsDataInterface = Ice::checkedCast<EventDataModule::EventsDataInterfacePrx>(base);
        if (!eventsDataInterface)
        {
            throw std::runtime_error("Invalid proxy");
        }
        // modify event client
        eventsDataInterface->modifyEventIntField(login_->userToken(), eventForm_->model_->id, EventDataModule::EventField::clientId, clientId);
        eventChangedClient_.emit(clientName, eventForm_->model_->id);
    }
    catch (const std::exception &e)
    {
        std::cerr << "\n\n"
                  << e.what() << "\n\n";
    }
}

void EventView::refreshData()
{
    int eventId = eventForm_->model_->id;

    // Ice communication for getting event data
    try
    {
        Ice::CommunicatorHolder ich = Ice::initialize();
        auto base = ich->stringToProxy(login_->eventConnString_);
        auto eventsDataInterface = Ice::checkedCast<EventDataModule::EventsDataInterfacePrx>(base);
        if (!eventsDataInterface)
        {
            throw std::runtime_error("Invalid proxy");
        }
        // get event data
        auto eventDataPack = eventsDataInterface->getEventData(login_->userToken(), eventId);
        setData(eventDataPack);
    }
    catch (const std::exception &e)
    {
        std::cerr << "\n\n"
                  << e.what() << "\n\n";
    }
}

void EventView::removeService(int serviceId)
{
    // Ice communication for removing service
    try
    {
        Ice::CommunicatorHolder ich = Ice::initialize();
        auto base = ich->stringToProxy(login_->eventConnString_);
        auto eventsDataInterface = Ice::checkedCast<EventDataModule::EventsDataInterfacePrx>(base);
        if (!eventsDataInterface)
        {
            throw std::runtime_error("Invalid proxy");
        }
        // remove service
        eventsDataInterface->deleteRecord(login_->userToken(), EventDataModule::Table::services, serviceId);
        refreshData();
    }
    catch (const std::exception &e)
    {
        std::cerr << "\n\n"
                  << e.what() << "\n\n";
    }
}

void EventView::registerNewService(EventDataModule::ServiceData serviceData)
{
    serviceData.eventId = eventForm_->model_->id;
    // Ice communication for registering new service
    try
    {
        Ice::CommunicatorHolder ich = Ice::initialize();
        auto base = ich->stringToProxy(login_->eventConnString_);
        auto eventsDataInterface = Ice::checkedCast<EventDataModule::EventsDataInterfacePrx>(base);
        if (!eventsDataInterface)
        {
            throw std::runtime_error("Invalid proxy");
        }
        // register new service
        eventsDataInterface->addServiceToEvent(login_->userToken(), serviceData);
        refreshData();
    }
    catch (const std::exception &e)
    {
        std::cerr << "\n\n"
                  << e.what() << "\n\n";
    }
}

void EventView::removeServiceDialog(EventDataModule::ServiceData serviceData)
{
    auto dialog = addChild(std::make_unique<Wt::WDialog>("Delete Service"));

    auto serviceFormView = dialog->contents()->addWidget(std::make_unique<ServiceFormView>(login_, eventForm_->model_));
    serviceFormView->setData(serviceData);
    serviceFormView->hideServiceChangeBtns(true);
    auto delBtn = dialog->footer()->addWidget(std::make_unique<Wt::WPushButton>("Delete"));
    auto closeBtn = dialog->footer()->addWidget(std::make_unique<Wt::WPushButton>("Close"));

    delBtn->clicked().connect(dialog, &Wt::WDialog::accept);
    closeBtn->clicked().connect(dialog, &Wt::WDialog::reject);

    dialog->finished().connect([=]()
                               {
        if (dialog->result() == Wt::DialogCode::Accepted)
        {
            removeService(serviceData.id);
        }
        removeChild(dialog); });
    dialog->show();
}