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
#include <Wt/WApplication.h>

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
		auto size = seqEventDataPack.size();
		for (int i = 0; i < size; ++i)
		{
			if (i == 0)
				addEventToList(seqEventDataPack.at(i), true);
			else
				addEventToList(seqEventDataPack.at(i));
		}
	}
}

// adds events to the view
void Events::addEventToList(EventDataModule::EventDataPack eventDataPack, bool active)
{
	auto navItemWidget = std::make_unique<Wt::WTemplate>(tr("event-nav-item"));
	auto navItem = navItemWidget.get();
	auto eventContentWrapperWidget = std::make_unique<Wt::WContainerWidget>();
	auto eventContentWrapper = eventContentWrapperWidget.get();
	eventsContentStack_->addWidget(std::move(eventContentWrapperWidget));
	eventsMenu_->addWidget(std::move(navItemWidget));
	navItem->setStyleClass("events-nav-item");
	std::cout << "\n\n eventsContentStack_->currentIndex() = " << eventsContentStack_->currentIndex() << "\n\n";

	if (active)
	{
		if (eventsContentStack_->count() > 1)
		{
			int clickedItemIndex = eventsMenu_->indexOf(navItem);
			int curentItemIndex = eventsContentStack_->currentIndex();
			eventsMenu_->widget(curentItemIndex)->removeStyleClass("active");
			eventsContentStack_->setCurrentIndex(clickedItemIndex);
		}
		navItem->addStyleClass("active");
	}

	auto eventSettingsBtn = navItem->bindWidget("settings-btn", std::make_unique<Wt::WPushButton>("<i class=\"bi bi-gear\"></i>", Wt::TextFormat::XHTML));

	auto eventView = eventContentWrapper->addWidget(std::make_unique<EventView>(login_));

	auto eventWrapperId_name = Wt::WString(std::to_string(eventDataPack.eventData.id) + "_event_data");
	auto navItemEventId_name = Wt::WString(std::to_string(eventDataPack.eventData.id) + "_event_nav_item");

	eventView->setObjectName(eventWrapperId_name.toUTF8());
	navItem->setObjectName(navItemEventId_name.toUTF8());

	eventView->eventChangedClient().connect(this, &Events::eventClientChanged);
	eventView->eventForm_->dateChanged().connect(this, &Events::eventDateChanged);
	eventView->setData(eventDataPack);

	navItem->clicked().connect(this, [=]()
							   {
		int curentItemIndex = eventsContentStack_->currentIndex();
		int clickedItemIndex = eventsMenu_->indexOf(navItem);

		if (curentItemIndex == clickedItemIndex)
		{
			return;
		}

		eventsContentStack_->setCurrentIndex(clickedItemIndex);
		eventsMenu_->widget(curentItemIndex)->removeStyleClass("active");
		eventsMenu_->widget(clickedItemIndex)->addStyleClass("active"); });

	auto popupMenu = std::make_unique<Wt::WPopupMenu>();
	popupMenu->addStyleClass("dropdown-menu-dark");
	auto addServiceBtn = popupMenu->addItem("./resources/Icons/myIcons/file-earmark-plus.svg", "Adauga serviciu");
	auto copyDataBtn = popupMenu->addItem("./resources/Icons/myIcons/clipboard-check.svg", "Copiaza datele");
	popupMenu->addSeparator();
	auto deleteBtn = popupMenu->addItem("./resources/Icons/myIcons/trash3.svg", "Sterge evenimetul");
	addServiceBtn->clicked().connect(this, [=]()
									 { addServiceToEventDialog(eventDataPack.eventData.id); });
	copyDataBtn->clicked().connect(this, [=]()
								   { copyEventDataDialog(eventView->getData()); });
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
					removeChild(messageBox); });
					
		messageBox->show(); });

	eventSettingsBtn->setMenu(std::move(popupMenu));
	eventSettingsBtn->setStyleClass("btn btn-outline-dark btn-sm rounded-circle");
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
		displayEventsData();
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << std::endl;
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

		auto newEvDataPack = eventsDataInterface->registerEvent(login_->userToken(), eventDataPack);
		addEventToList(newEvDataPack, true);
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << std::endl;
	}
	// displayEventsData();
}

void Events::copyEventDataDialog(EventDataModule::EventDataPack eventDataPack)
{
	auto dialog = addChild(std::make_unique<Wt::WDialog>());
	dialog->setResizable(true);
	dialog->setMinimumSize(850, 600);
	dialog->titleBar()->addStyleClass("bg-primary text-light text-center");
	auto closeBtn = dialog->titleBar()->addWidget(std::make_unique<Wt::WPushButton>("esc"));
	closeBtn->clicked().connect(dialog, &Wt::WDialog::reject);
	dialog->rejectWhenEscapePressed();
	dialog->setWindowTitle("Datele evenimentului");
	dialog->contents()->setStyleClass("d-flex p-3");

	auto eventDataWrapper = dialog->contents()->addWidget(std::make_unique<Wt::WContainerWidget>());
	auto servicesDataWrapper = dialog->contents()->addWidget(std::make_unique<Wt::WContainerWidget>());

	eventDataWrapper->setStyleClass("flex-grow-1");
	servicesDataWrapper->setStyleClass("flex-grow-1");

	auto clientTemp = eventDataWrapper->addWidget(std::make_unique<Wt::WTemplate>(tr("client-copy-data-template")));
	auto eventTemp = eventDataWrapper->addWidget(std::make_unique<Wt::WTemplate>(tr("event-copy-data-template")));

	// get the data ready for desplay
	auto dateTime = Wt::WDateTime().fromString(eventDataPack.eventData.dateTime, "dd/MM/yyyy HH:mm AP");
	auto dateString = dateTime.toString("dd/MMMM/yyyy");
	auto timeString = dateTime.toString("HH:mm AP");

	auto duration = eventDataPack.eventData.duration;
	auto hours = (int)duration;
	auto minutes = (int)((duration - hours) * 60);
	auto minutesString = std::to_string(minutes);
	if (minutes == 0)
		minutesString = "00";
	auto durationString = Wt::WString(std::to_string(hours) + ":" + minutesString + " H");

	clientTemp->bindString("client-name", eventDataPack.clientData.name);
	clientTemp->bindString("client-phone", eventDataPack.clientData.phone);

	eventTemp->bindString("event-date", dateString);
	eventTemp->bindString("event-time", timeString);
	eventTemp->bindString("event-duration", durationString);
	eventTemp->bindString("event-location", eventDataPack.eventData.location);
	eventTemp->bindString("event-observations", eventDataPack.eventData.observations);
	eventTemp->bindString("event-total-cost", "ajungem si aici");

	auto copyClientDataBtn = clientTemp->bindWidget("copy-client-data-btn", std::make_unique<Wt::WPushButton>("<i class=\"bi bi-clipboard\"></i>", Wt::TextFormat::XHTML));
	auto copyEventDataBtn = eventTemp->bindWidget("copy-event-data-btn", std::make_unique<Wt::WPushButton>("<i class=\"bi bi-clipboard\"></i>", Wt::TextFormat::XHTML));

	for (auto service : eventDataPack.seqServices)
	{
		auto serviceDateTime = Wt::WDateTime().fromString(service.dateTime, "dd/MM/yyyy HH:mm AP");

		auto duration = service.duration;
		auto hours = (int)duration;
		auto minutes = (int)((duration - hours) * 60);
		auto minutesString = std::to_string(minutes);
		if (minutes == 0)
			minutesString = "00";
		auto durationString = Wt::WString(std::to_string(hours) + ":" + minutesString + " H");

		auto serviceTemp = servicesDataWrapper->addWidget(std::make_unique<Wt::WTemplate>(tr("service-copy-data-template")));
		serviceTemp->bindString("service-provider", service.providerIdentity);
		serviceTemp->bindString("service-provider-service", service.providerService);
		serviceTemp->bindString("service-date", serviceDateTime.toString("dd/MMMM/yyyy"));
		serviceTemp->bindString("service-time", serviceDateTime.toString("HH:mm AP"));
		serviceTemp->bindString("service-duration", durationString);
		serviceTemp->bindString("service-cost", std::to_string(service.cost) + " RON");
		serviceTemp->bindString("service-description", service.description);
		serviceTemp->bindString("service-observations", service.observations);
		serviceTemp->bindString("service-total-cost", "ajungem si aici");
		
		Wt::WString copyServicesJsFunc = Wt::WString("setServicesCheckboxes(") + serviceTemp->id() + Wt::WString(")");
		serviceTemp->doJavaScript(copyServicesJsFunc.toUTF8());
	}
	clientTemp->doJavaScript("setClientCheckboxes()");
	eventTemp->doJavaScript("setEventCheckboxes()");
	

	dialog->finished().connect([=]()
							   { removeChild(dialog); });

		dialog->show();
}
