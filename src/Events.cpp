#include "include/Events.h"
#include "include/EventView.h"
#include "include/AddEventDialog.h"
#include <Wt/WTemplate.h>
#include <Wt/WMenu.h>
#include <Wt/WStackedWidget.h>
#include <Wt/WText.h>
#include <Wt/WPushButton.h>

#include "../../comunication/EventDataModule.h"
#include <Ice/Ice.h>
#include <stdexcept>
#include <Wt/WLabel.h>
#include <Wt/WMessageBox.h>
// #include <Wt/WMenuItem.h>
using namespace std;

Events::Events(std::shared_ptr<Login> login)
	: WContainerWidget(),
	  login_(login)
{
	setStyleClass("events position-relative");

	// Events Navigation
	auto eventsNavContainer = addWidget(std::make_unique<Wt::WContainerWidget>());
	eventsNavContainer->setStyleClass("events-nav");

	// Devider between navigation and content of the event
	addWidget(std::make_unique<Wt::WContainerWidget>())->setStyleClass("event-divider");

	// Events Content Stack
	auto eventsContentWrapper = addWidget(std::make_unique<Wt::WContainerWidget>());
	eventsContentWrapper->setStyleClass("events-content");

	auto eventHeaderToolbar = eventsContentWrapper->addWidget(std::make_unique<Wt::WContainerWidget>());
	eventHeaderToolbar->setStyleClass("bg-light");

	delEventBtn_ = eventHeaderToolbar->addWidget(std::make_unique<Wt::WPushButton>("Delete Event"));
	addServiceBtn_ = eventHeaderToolbar->addWidget(std::make_unique<Wt::WPushButton>("Add Service"));

	delEventBtn_->setStyleClass("btn btn-outline-danger me-5");
	addServiceBtn_->setStyleClass("btn btn-outline-primary");

	delEventBtn_->disable();
	addServiceBtn_->disable();

	eventsContentStack_ = eventsContentWrapper->addWidget(std::make_unique<Wt::WStackedWidget>());

	// Navigation Header
	auto navHeader = eventsNavContainer->addWidget(std::make_unique<Wt::WContainerWidget>());
	navHeader->setStyleClass("events-nav-header d-block");

	auto refreshBtn = navHeader->addWidget(std::make_unique<Wt::WPushButton>("&#8634;", Wt::TextFormat::XHTML));
	refreshBtn->setStyleClass("btn btn-outline-success me-auto");

	auto title = navHeader->addWidget(std::make_unique<Wt::WText>("Events"));
	title->setStyleClass("events-nav-header");

	auto addEventBtn = navHeader->addWidget(std::make_unique<Wt::WPushButton>("+", Wt::TextFormat::XHTML));
	addEventBtn->setStyleClass("btn btn-outline-primary ms-auto");
	addEventBtn->clicked().connect(this, [=]()
								   {
									   
									   auto addEventDialog = addChild(std::make_unique<AddEventDialog>(login_));
									   addEventDialog->show();
										addEventDialog->finished().connect([=](){
											addEventDialog->removeFromParent();
											displayEventsData();
										}); });

	refreshBtn->clicked().connect(this, &Events::displayEventsData);
	delEventBtn_->clicked().connect(this, &Events::removeEvent);
	addServiceBtn_->clicked().connect(this, &Events::addServiceToEventDialog);
	// Navigation Menu
	eventsMenu_ = eventsNavContainer->addWidget(std::make_unique<Wt::WContainerWidget>());

	eventsMenu_->setStyleClass("events-nav-item-list");
	displayEventsData();
}

// display all events data, first removes all the events if there is any and then brings all the events data from the server
void Events::displayEventsData()
{
	// std::cout << "\n\n displayEventsData -------------------------------------------\n\n";
	EventDataModule::SeqEventDataPack seqEventDataPack;

	// clean events view if there is any
	if (eventsContentStack_->count() > 0)
	{
		eventsMenu_->clear();
		eventsContentStack_->clear();
	}

	// Ice communication for getting events data
	try
	{
		Ice::CommunicatorHolder ich = Ice::initialize();
		auto base = ich->stringToProxy(login_->eventConnString_);
		auto eventsDataInterface = Ice::checkedCast<EventDataModule::EventsDataInterfacePrx>(base);
		if (!eventsDataInterface)
		{
			throw std::runtime_error("Invalid proxy");
		}

		// get events data
		seqEventDataPack = eventsDataInterface->getEventsData(login_->userToken());
	}
	catch (const std::exception &e)
	{
		std::cerr << "\n\n"
				  << e.what() << "\n\n";
	}

	// add events to this view
	if (seqEventDataPack.size() != 0)
	{
		delEventBtn_->enable();
		addServiceBtn_->enable();
		for (int i = 0; i < seqEventDataPack.size(); ++i)
		{
			addEventToList(seqEventDataPack.at(i));
		}
		eventsContentStack_->setCurrentIndex(0);
		eventsMenu_->children().at(0)->addStyleClass("active");
	}
	else
	{
		delEventBtn_->disable();
		addServiceBtn_->disable();
	}
}

// adds events to the view
void Events::addEventToList(EventDataModule::EventDataPack eventDataPack)
{
	// std::cout << "\n\n addEventToList -------------------------------------------\n\n";
	auto navItem = eventsMenu_->addWidget(std::make_unique<Wt::WTemplate>(tr("event-nav-item")));
	navItem->setStyleClass("events-nav-item");

	auto eventFormWidgetWrapper = eventsContentStack_->addWidget(std::make_unique<Wt::WContainerWidget>());
	auto eventView = eventFormWidgetWrapper->addWidget(std::make_unique<EventView>(login_));

	auto eventWrapperId_name = Wt::WString(std::to_string(eventDataPack.eventData.id) + "_event_data");
	auto navItemEventId_name = Wt::WString(std::to_string(eventDataPack.eventData.id) + "_event_nav_item");

	eventView->setObjectName(eventWrapperId_name.toUTF8());
	navItem->setObjectName(navItemEventId_name.toUTF8());

	eventView->eventChangedClient().connect(this, &Events::eventClientChanged);
	eventView->eventForm_->dateChanged().connect(this, &Events::eventDateChanged);
	eventView->setData(eventDataPack);

	if (eventsContentStack_->count() == 1)
	{
		navItem->addStyleClass("active");
		delEventBtn_->enable();
		addServiceBtn_->enable();
	}

	navItem->clicked().connect(this, [=]()
							   {
								   int curentItemIndex = eventsContentStack_->currentIndex();
								   int clickedItemIndex = eventsMenu_->indexOf(navItem);

								   if (curentItemIndex == clickedItemIndex)
								   {
									   return;
								   }

								   eventsMenu_->widget(curentItemIndex)->removeStyleClass("active");
								   eventsMenu_->widget(clickedItemIndex)->addStyleClass("active");
								   eventsContentStack_->setCurrentIndex(clickedItemIndex); });
}

// remove current active event from the view and backend
void Events::removeEvent()
{
	// Event content wrapper has a child EventView with name of [event_id]_event_data
	// menu item has a name of [event_id]_event_nav_item
	Wt::WWidget *eventDataWrapper = eventsContentStack_->currentWidget();

	std::string eventData_name = eventDataWrapper->children().at(0)->objectName();
	std::size_t separatorPos = eventData_name.find('_');
	string eventIdString = eventData_name.substr(0, separatorPos);
	// std::cout << "\n\n event id to be dleted = " << eventIdString << "\n";
	Wt::WWidget *navItem = eventsMenu_->find(Wt::WString(eventIdString + "_event_nav_item").toUTF8());

	// Ice communication for deleting event
	try
	{
		Ice::CommunicatorHolder ich = Ice::initialize();
		auto base = ich->stringToProxy(login_->eventConnString_);
		auto eventsDataInterface = Ice::checkedCast<EventDataModule::EventsDataInterfacePrx>(base);
		if (!eventsDataInterface)
		{
			throw std::runtime_error("Invalid proxy");
		}
		eventsDataInterface->deleteRecord(login_->userToken(), EventDataModule::Table::events, std::stoi(eventIdString));
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << std::endl;
	}
	displayEventsData();
}

void Events::eventClientChanged(Wt::WString clientName, int eventId)
{
	std::cout << "\n\n eventClientChanged ------------------------------------------- from Events \n\n";
	auto navItem_name = Wt::WString(std::to_string(eventId) + "_event_nav_item");
	auto navItem = static_cast<Wt::WTemplate *>(eventsMenu_->find(navItem_name.toUTF8()));
	navItem->bindString("client-name", clientName);
}

void Events::eventDateChanged(Wt::WDate eventDate, int eventId)
{
	auto navItem_name = Wt::WString(std::to_string(eventId) + "_event_nav_item");
	auto eventView_name = Wt::WString(std::to_string(eventId) + "_event_data");

	auto navItem = static_cast<Wt::WTemplate *>(eventsMenu_->find(navItem_name.toUTF8()));
	auto eventView = static_cast<EventView *>(eventsContentStack_->find(eventView_name.toUTF8()));
	auto dateFormat = eventView->eventForm_->model_->dateFormat_;
	navItem->bindString("event-date", eventDate.toString(dateFormat));
}

void Events::addServiceToEventDialog()
{

	auto eventView = static_cast<EventView *>(eventsContentStack_->currentWidget()->children().at(0));

	if (login_->user().servicesInfoSq.size() == 0)
	{
		eventView->createNoServicesMessage();
		return;
	}

	auto dialog = addChild(std::make_unique<Wt::WDialog>("Add service"));
	auto serviceFormView = dialog->contents()->addWidget(std::make_unique<ServiceFormView>(login_, eventView->eventForm_->model_));

	auto submitBtn = dialog->footer()->addWidget(std::make_unique<Wt::WPushButton>("Submit"));
	auto cancelBtn = dialog->footer()->addWidget(std::make_unique<Wt::WPushButton>("Cancel"));
	dialog->rejectWhenEscapePressed();

	submitBtn->clicked().connect(this, [=]()
								 {
									 if (serviceFormView->validate())
									 {
										 dialog->accept();
									 }
									 else
									 {
										 std::cout << "\n\n service form not valid \n\n";
									 } });
	cancelBtn->clicked().connect(dialog, &Wt::WDialog::reject);

	dialog->finished().connect([=]()
							   {
								   if (dialog->result() == Wt::DialogCode::Accepted)
								   {
										auto serviceData = serviceFormView->model_->getData();
									   	eventView->registerNewService(serviceData);
									  
								   }else {
										std::cout << "\n\n add service to event dialog rejected \n\n";
								   }
								   removeChild(dialog); });

	dialog->show();
}
