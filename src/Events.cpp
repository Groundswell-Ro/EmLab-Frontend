#include "include/Events.h"
#include "include/EventView.h"
#include <Wt/WTemplate.h>
#include <Wt/WMenu.h>
#include <Wt/WStackedWidget.h>
#include <Wt/WText.h>
#include <Wt/WPushButton.h>
#include <Wt/WPopupMenu.h>

#include "../../comunication/EventDataModule.h"
#include <Ice/Ice.h>
#include <stdexcept>
#include <Wt/WLabel.h>
#include <Wt/WMessageBox.h>

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

	eventsContentStack_ = addWidget(std::make_unique<Wt::WStackedWidget>());
	eventsContentStack_->addStyleClass("events-content");
	eventsContentStack_->setTransitionAnimation(Wt::WAnimation(Wt::AnimationEffect::SlideInFromLeft, Wt::TimingFunction::Linear, 400), true);
	// Navigation Header
	auto navHeader = eventsNavContainer->addWidget(std::make_unique<Wt::WContainerWidget>());
	navHeader->setStyleClass("events-nav-header");

	auto prevBtn = navHeader->addWidget(std::make_unique<Wt::WPushButton>("<i class=\"bi bi-arrow-bar-left fs-3 \"></i>", Wt::TextFormat::XHTML));
	prevBtn->setStyleClass("btn btn-light me-auto");

	auto title = navHeader->addWidget(std::make_unique<Wt::WText>("Azi"));
	// title->setStyleClass("events-nav-header");

	auto nextBtn = navHeader->addWidget(std::make_unique<Wt::WPushButton>("<i class=\"bi bi-arrow-bar-right fs-4 \"></i>", Wt::TextFormat::XHTML));
	nextBtn->setStyleClass("btn btn-light ms-auto");

	auto addEventBtn = navHeader->addWidget(std::make_unique<Wt::WPushButton>("<i class=\"bi bi-journal-plus fs-4 \"></i>", Wt::TextFormat::XHTML));
	addEventBtn->setStyleClass("btn btn-success ms-auto");

	// tooltip for add event button
	addEventBtn->setAttributeValue("data-bs-toggle", "tooltip");
	addEventBtn->setAttributeValue("data-bs-placement", "right");
	addEventBtn->setAttributeValue("title", "Adauga eveniment");

	addEventBtn->clicked().connect(this, &Events::addEventDialog);

	// delEventBtn_->clicked().connect(this, &Events::removeEvent);
	// addServiceBtn_->clicked().connect(this, &Events::addServiceToEventDialog);
	// Navigation Menu
	eventsMenu_ = eventsNavContainer->addWidget(std::make_unique<Wt::WContainerWidget>());

	eventsMenu_->setStyleClass("events-nav-item-list");
	displayEventsData();
}

void Events::addEventDialog()
{
	auto addEventDialog = addChild(std::make_unique<Wt::WDialog>("Inregistreaza un eveniment"));
	addEventDialog->setMinimumSize(Wt::WLength(98, Wt::LengthUnit::ViewportWidth), Wt::WLength(98, Wt::LengthUnit::ViewportHeight));
	addEventDialog->setMaximumSize(Wt::WLength(98, Wt::LengthUnit::ViewportWidth), Wt::WLength(98, Wt::LengthUnit::ViewportHeight));

	addEventDialog->setOffsets(Wt::WLength(1, Wt::LengthUnit::ViewportWidth), Wt::Side::Left);
	addEventDialog->setOffsets(Wt::WLength(1, Wt::LengthUnit::ViewportWidth), Wt::Side::Right);
	addEventDialog->setOffsets(Wt::WLength(1, Wt::LengthUnit::ViewportHeight), Wt::Side::Top);
	addEventDialog->setOffsets(Wt::WLength(1, Wt::LengthUnit::ViewportHeight), Wt::Side::Bottom);

	addEventDialog->contents()->setStyleClass("p-0");
	addEventDialog->setMovable(false);
	auto addServiceBtn = addEventDialog->titleBar()->addWidget(std::make_unique<Wt::WPushButton>("Adauga Serviciu"));
	addServiceBtn->setStyleClass("btn btn-outline-primary ms-auto");

	auto closeBtn = addEventDialog->titleBar()->insertBefore(std::make_unique<Wt::WPushButton>("esc"), addEventDialog->titleBar()->children().at(0));
	closeBtn->setStyleClass("btn btn-outline-danger me-auto");
	closeBtn->clicked().connect(addEventDialog, &Wt::WDialog::reject);
	addEventDialog->rejectWhenEscapePressed();

	auto eventView = addEventDialog->contents()->addWidget(std::make_unique<EventView>(login_));

	addServiceBtn->clicked().connect(this, [=]()
									 { eventView->addService(); });

	auto submitBtn = addEventDialog->footer()->addWidget(std::make_unique<Wt::WPushButton>("Submit Data"));
	submitBtn->setStyleClass("btn btn-lg btn-success");
	submitBtn->clicked().connect(this, [=]()
								 {
		if(eventView->validateFormsData()){
			addEventDialog->accept();
		} });

	addEventDialog->finished().connect(this, [=]()
									   {
					if (addEventDialog->result() == Wt::DialogCode::Accepted)
					{
						registerEvent(eventView->getData());
					}
					addEventDialog->removeFromParent(); });

	addEventDialog->animateShow(Wt::WAnimation(Wt::AnimationEffect::SlideInFromTop, Wt::TimingFunction::Linear, 400));
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
		for (int i = 0; i < seqEventDataPack.size(); ++i)
		{
			addEventToList(seqEventDataPack.at(i));
		}
		eventsContentStack_->setCurrentIndex(0);
		eventsMenu_->children().at(0)->addStyleClass("active");
	}
}

// adds events to the view
void Events::addEventToList(EventDataModule::EventDataPack eventDataPack)
{
	// std::cout << "\n\n addEventToList -------------------------------------------\n\n";
	auto navItem = eventsMenu_->addWidget(std::make_unique<Wt::WTemplate>(tr("event-nav-item")));
	navItem->setStyleClass("events-nav-item");
	auto eventSettingsBtn = navItem->bindWidget("settings-btn", std::make_unique<Wt::WPushButton>("<i class=\"bi bi-gear\"></i>", Wt::TextFormat::XHTML));

	auto popupMenu = std::make_unique<Wt::WPopupMenu>();
	popupMenu->addStyleClass("dropdown-menu-dark");
	auto addServiceBtn = popupMenu->addItem("./resources/Icons/myIcons/file-earmark-plus.svg", "Adauga serviciu");
	auto copyDataBtn = popupMenu->addItem("./resources/Icons/myIcons/clipboard-check.svg", "Copiaza datele");
	popupMenu->addSeparator();
	auto deleteBtn = popupMenu->addItem("./resources/Icons/myIcons/trash3.svg", "Sterge evenimetul");
	addServiceBtn->clicked().connect(this, [=]()
									 { addServiceToEventDialog(eventDataPack.eventData.id); });
	deleteBtn->clicked().connect(this, [=]()
								 { 
		auto messageBox = addChild(std::make_unique<Wt::WMessageBox>(
        "Esti sigur ca vrei sa stergi evenimentul?",
        "<p>Evenimentul va fi sters impreuna cu toate serviciile lui</p>",
        Wt::Icon::Critical,
        Wt::StandardButton::Ok | Wt::StandardButton::Cancel));
		messageBox->titleBar()->addStyleClass("bg-danger text-light");

		messageBox->setMovable(false);

		messageBox->buttonClicked().connect([=]
											{
				if (messageBox->buttonResult() == Wt::StandardButton::Ok){
					removeEvent(eventDataPack.eventData.id);
				}
					removeChild(messageBox);
				});
    	messageBox->show(); });

	eventSettingsBtn->setMenu(std::move(popupMenu));

	eventSettingsBtn->setStyleClass("btn btn-outline-dark btn-sm rounded-circle");

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
void Events::removeEvent(int eventId)
{
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
		eventsDataInterface->deleteRecord(login_->userToken(), EventDataModule::Table::events, eventId);
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << std::endl;
	}

	auto navItem = eventsMenu_->find(Wt::WString(std::to_string(eventId) + "_event_nav_item").toUTF8());
	auto eventView = eventsContentStack_->find(Wt::WString(std::to_string(eventId) + "_event_data").toUTF8());
	eventsMenu_->removeWidget(navItem);
	eventsContentStack_->removeWidget(eventView);

	if (eventsMenu_->count() == 0)
	{
		return;
	}
	else
	{
		eventsContentStack_->setCurrentIndex(0);
		eventsMenu_->children().at(0)->addStyleClass("active");
	}
}

void Events::eventClientChanged(Wt::WString clientName, int eventId)
{
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

void Events::addServiceToEventDialog(int eventId)
{

	auto eventView = static_cast<EventView *>(eventsContentStack_->currentWidget()->children().at(0));

	if (login_->user().servicesInfoSq.size() == 0)
	{
		eventView->createNoServicesMessage();
		return;
	}

	auto dialog = addChild(std::make_unique<Wt::WDialog>("Datele serviciului"));
	dialog->titleBar()->addStyleClass("bg-primary text-light text-center");
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

void Events::registerEvent(EventDataModule::EventDataPack eventDataPack)
{
	// Ice communication for sending event date to server
	try
	{
		Ice::CommunicatorHolder ich = Ice::initialize();
		auto base = ich->stringToProxy(login_->eventConnString_);
		auto eventsDataInterface = Ice::checkedCast<EventDataModule::EventsDataInterfacePrx>(base);
		if (!eventsDataInterface)
		{
			throw std::runtime_error("Invalid proxy");
		}
		eventsDataInterface->registerEvent(login_->userToken(), eventDataPack);
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << std::endl;
	}
	displayEventsData();
}