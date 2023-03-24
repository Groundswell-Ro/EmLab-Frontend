#include "include/AddEventDialog.h"

#include "../../comunication/EventDataModule.h"
#include <Ice/Ice.h>
#include <stdexcept>

AddEventDialog::AddEventDialog(std::shared_ptr<Login> login)
    : WDialog("Register Event"),
      login_(login)
{
  setStyleClass("shadow-lg");

  setMinimumSize(Wt::WLength(98, Wt::LengthUnit::ViewportWidth), Wt::WLength(98, Wt::LengthUnit::ViewportHeight));
  setMaximumSize(Wt::WLength(98, Wt::LengthUnit::ViewportWidth), Wt::WLength(98, Wt::LengthUnit::ViewportHeight));

  setOffsets(Wt::WLength(1, Wt::LengthUnit::ViewportWidth), Wt::Side::Left);
  setOffsets(Wt::WLength(1, Wt::LengthUnit::ViewportWidth), Wt::Side::Right);
  setOffsets(Wt::WLength(1, Wt::LengthUnit::ViewportHeight), Wt::Side::Top);
  setOffsets(Wt::WLength(1, Wt::LengthUnit::ViewportHeight), Wt::Side::Bottom);

  contents()->setStyleClass("p-0");
  setMovable(false);

  auto addServiceBtn = titleBar()->addWidget(std::make_unique<Wt::WPushButton>("Add Service"));
  addServiceBtn->setStyleClass("btn btn-outline-primary ms-auto");

  auto closeBtn = titleBar()->insertBefore(std::make_unique<Wt::WPushButton>("Close"), titleBar()->children().at(0));
  closeBtn->setStyleClass("btn btn-outline-danger me-auto");
  closeBtn->clicked().connect(this, [=]()
                              { reject(); });
  rejectWhenEscapePressed();

  eventView_ = contents()->addWidget(std::make_unique<EventView>(login_));

  addServiceBtn->clicked().connect(this, [=]()
                                   { eventView_->addService(); });

  auto submitBtn = footer()->addWidget(std::make_unique<Wt::WPushButton>("Submit Data"));
  submitBtn->setStyleClass("btn btn-lg btn-success");
  submitBtn->clicked().connect(this, &AddEventDialog::submitBtnClicked);
}

void AddEventDialog::submitBtnClicked()
{
  EventDataModule::EventDataPack eventDataPack;

  // update event model
  eventView_->eventForm_->updateModel(eventView_->eventForm_->model_.get());

  // validate event and client model
  if (!eventView_->clientForm_->validate() || !eventView_->eventForm_->validate())
  {
    return;
  }

  // get event and client data
  eventDataPack.eventData = eventView_->eventForm_->model_->getData();
  eventDataPack.clientData = eventView_->clientForm_->model_->getData();

  // update event and client view
  eventView_->eventForm_->updateView(eventView_->eventForm_->model_.get());
  eventView_->clientForm_->updateView(eventView_->clientForm_->model_.get());

  // get services form widgets
  auto servicesWidgets = eventView_->servicesWrapper_->children();

  // iterate through services form widgets and validate and update data
  for (int i = 0; i < servicesWidgets.size(); ++i)
  {
    auto servFormWidget = static_cast<ServiceFormWidget *>(servicesWidgets.at(i));
    // update model and validate
    servFormWidget->serviceForm_->updateModel(servFormWidget->serviceForm_->model_.get());
    if (!servFormWidget->serviceForm_->validate())
    {
      return;
    }
    // update view
    servFormWidget->serviceForm_->updateView(servFormWidget->serviceForm_->model_.get());

    // get service data
    eventDataPack.seqServices.push_back(servFormWidget->serviceForm_->model_->getData());
  }

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

  accept();
  eventRegisterd_.emit();
}
