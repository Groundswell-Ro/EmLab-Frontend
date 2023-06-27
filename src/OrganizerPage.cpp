#include "include/OrganizerPage.h"

#include <Wt/WText.h>
#include <Wt/WTemplate.h>
#include <Wt/WApplication.h>
#include <Wt/WPushButton.h>

OrganizerPage::OrganizerPage(std::shared_ptr<Login> login)
    : WContainerWidget(),
	login_(login)
{
    addStyleClass("");

    // createButtonsExampla();
	createEventsTable();
}

void OrganizerPage::createEventsTable()
{
	auto table_wrapper = addWidget(std::make_unique<Wt::WContainerWidget>());
	table_wrapper->setStyleClass("max-w-screen-xl m-auto");
	table_wrapper->addWidget(std::make_unique<Wt::WText>("Events"));
	events_table_ = table_wrapper->addWidget(std::make_unique<Wt::WTable>());
	events_table_->setStyleClass("table");
	events_table_->setHeaderCount(1);
	auto refresh = events_table_->elementAt(0, 0)->addNew<Wt::WPushButton>(tr("refresh-svg-sm"));
	events_table_->elementAt(0, 1)->addNew<Wt::WText>("client-id");
	events_table_->elementAt(0, 2)->addNew<Wt::WText>("date-time");
	events_table_->elementAt(0, 3)->addNew<Wt::WText>("duration");
	events_table_->elementAt(0, 4)->addNew<Wt::WText>("location");
	events_table_->elementAt(0, 5)->addNew<Wt::WText>("Description");

	refresh->setTextFormat(Wt::TextFormat::XHTML);
	refresh->clicked().connect(this, &OrganizerPage::populateEventsTable);

	if(!login_->isLoggedIn()) return;
	populateEventsTable();
	

} 

void OrganizerPage::populateEventsTable()
{
	Emlab::SeqEventData seqEventData = Emlab::Comunication::getEventsData(login_->userToken());
	while(events_table_->rowCount() > 1)
	{
		events_table_->removeRow(1);
	}

	// add events to this view
	if (seqEventData.size() != 0)
	{
		auto size = seqEventData.size();
		for (int i = 0; i < size; ++i)
		{
			auto event = seqEventData.at(i);

			auto del_btn = events_table_->elementAt(i + 1, 0)->addNew<Wt::WPushButton>(tr("trash-svg-sm"));
			events_table_->elementAt(i + 1, 1)->addNew<Wt::WText>(std::to_string(event.eventInfo.clientId));
			events_table_->elementAt(i + 1, 2)->addNew<Wt::WText>(event.eventInfo.dateTime);
			events_table_->elementAt(i + 1, 3)->addNew<Wt::WText>(std::to_string(event.eventInfo.duration));
			events_table_->elementAt(i + 1, 4)->addNew<Wt::WText>(event.eventInfo.location);
			events_table_->elementAt(i + 1, 5)->addNew<Wt::WText>(event.eventInfo.description);

			del_btn->setTextFormat(Wt::TextFormat::XHTML);
			del_btn->setStyleClass("btn btn-danger");
			del_btn->clicked().connect([=] {
				Emlab::Comunication::delEvent(login_->userToken(), event.eventInfo.id);
				populateEventsTable();
			});
		}
	}
}

void OrganizerPage::createButtonsExampla() 
{
    auto btn_Wrapper = addWidget(std::make_unique<Wt::WContainerWidget>());
	btn_Wrapper->addStyleClass("block");
	Wt::WString btn_wrapper_styles = "flex flex-wrap";
	// DEFAULT
	auto defaultBtn_Wrapper = btn_Wrapper->addWidget(std::make_unique<Wt::WContainerWidget>());
	defaultBtn_Wrapper->addStyleClass(btn_wrapper_styles);
	defaultBtn_Wrapper->addWidget(std::make_unique<Wt::WPushButton>("light"))->setStyleClass("btn btn-light-default");
	defaultBtn_Wrapper->addWidget(std::make_unique<Wt::WPushButton>("dark"))->setStyleClass("btn btn-dark-default");
	auto default_disabled = defaultBtn_Wrapper->addWidget(std::make_unique<Wt::WPushButton>("disabled"));
	default_disabled->setStyleClass("btn");
	default_disabled->setDisabled(true);
	defaultBtn_Wrapper->addWidget(std::make_unique<Wt::WPushButton>("simple"))->setStyleClass("btn");
	
	// PRIMARY
	
	auto primaryBtn_Wrapper = btn_Wrapper->addWidget(std::make_unique<Wt::WContainerWidget>());
	primaryBtn_Wrapper->addStyleClass(btn_wrapper_styles);
	primaryBtn_Wrapper->addWidget(std::make_unique<Wt::WPushButton>("light"))->setStyleClass("btn btn-primary-light");
	primaryBtn_Wrapper->addWidget(std::make_unique<Wt::WPushButton>("dark"))->setStyleClass("btn btn-primary-dark");
	auto primary_disabled = primaryBtn_Wrapper->addWidget(std::make_unique<Wt::WPushButton>("disabled"));
	primary_disabled->setStyleClass("btn btn-primary");
	primary_disabled->setDisabled(true);
	primaryBtn_Wrapper->addWidget(std::make_unique<Wt::WPushButton>("primary"))->setStyleClass("btn btn-primary");


	// SECCONDARY
	auto secondaryBtn_Wrapper = btn_Wrapper->addWidget(std::make_unique<Wt::WContainerWidget>());
	secondaryBtn_Wrapper->addStyleClass(btn_wrapper_styles);
	secondaryBtn_Wrapper->addWidget(std::make_unique<Wt::WPushButton>("light"))->setStyleClass("btn btn-seccondary-light");
	secondaryBtn_Wrapper->addWidget(std::make_unique<Wt::WPushButton>("dark"))->setStyleClass("btn btn-seccondary-dark");
	auto seccondary_disabled = secondaryBtn_Wrapper->addWidget(std::make_unique<Wt::WPushButton>("disabled"));
	seccondary_disabled->setStyleClass("btn btn-secondary");
	seccondary_disabled->setDisabled(true);
	secondaryBtn_Wrapper->addWidget(std::make_unique<Wt::WPushButton>("seccondary"))->setStyleClass("btn btn-seccondary");

	// SUCCESS
	auto successBtn_Wrapper = btn_Wrapper->addWidget(std::make_unique<Wt::WContainerWidget>());
	successBtn_Wrapper->addStyleClass(btn_wrapper_styles);
	successBtn_Wrapper->addWidget(std::make_unique<Wt::WPushButton>("light"))->setStyleClass("btn btn-success-light");
	successBtn_Wrapper->addWidget(std::make_unique<Wt::WPushButton>("dark"))->setStyleClass("btn btn-success-dark");
	auto success_disabled = successBtn_Wrapper->addWidget(std::make_unique<Wt::WPushButton>("disabled"));
	success_disabled->setStyleClass("btn btn-success");
	success_disabled->setDisabled(true);
	successBtn_Wrapper->addWidget(std::make_unique<Wt::WPushButton>("success"))->setStyleClass("btn btn-success");

	// DANGER
	auto dangerBtn_Wrapper = btn_Wrapper->addWidget(std::make_unique<Wt::WContainerWidget>());
	dangerBtn_Wrapper->addStyleClass(btn_wrapper_styles);
	dangerBtn_Wrapper->addWidget(std::make_unique<Wt::WPushButton>("light"))->setStyleClass("btn btn-danger-light");
	dangerBtn_Wrapper->addWidget(std::make_unique<Wt::WPushButton>("dark"))->setStyleClass("btn btn-danger-dark");
	auto danger_disabled = dangerBtn_Wrapper->addWidget(std::make_unique<Wt::WPushButton>("disabled"));
	danger_disabled->setStyleClass("btn btn-danger");
	danger_disabled->setDisabled(true);
	dangerBtn_Wrapper->addWidget(std::make_unique<Wt::WPushButton>("danger"))->setStyleClass("btn btn-danger");

	// WARNING
	auto warningBtn_Wrapper = btn_Wrapper->addWidget(std::make_unique<Wt::WContainerWidget>());
	warningBtn_Wrapper->addStyleClass(btn_wrapper_styles);
	warningBtn_Wrapper->addWidget(std::make_unique<Wt::WPushButton>("light"))->setStyleClass("btn btn-warning-light");
	warningBtn_Wrapper->addWidget(std::make_unique<Wt::WPushButton>("dark"))->setStyleClass("btn btn-warning-dark");
	auto warning_disabled = warningBtn_Wrapper->addWidget(std::make_unique<Wt::WPushButton>("disabled"));
	warning_disabled->setStyleClass("btn btn-warning");
	warning_disabled->setDisabled(true);
	warningBtn_Wrapper->addWidget(std::make_unique<Wt::WPushButton>("warning"))->setStyleClass("btn btn-warning");

	// INFO
	auto infoBtn_Wrapper = btn_Wrapper->addWidget(std::make_unique<Wt::WContainerWidget>());
	infoBtn_Wrapper->addStyleClass(btn_wrapper_styles);
	infoBtn_Wrapper->addWidget(std::make_unique<Wt::WPushButton>("light"))->setStyleClass("btn btn-info-light");
	infoBtn_Wrapper->addWidget(std::make_unique<Wt::WPushButton>("dark"))->setStyleClass("btn btn-info-dark");
	auto info_disabled = infoBtn_Wrapper->addWidget(std::make_unique<Wt::WPushButton>("disabled"));
	info_disabled->setStyleClass("btn btn-info");
	info_disabled->setDisabled(true);
	infoBtn_Wrapper->addWidget(std::make_unique<Wt::WPushButton>("info"))->setStyleClass("btn btn-info");

	// DARK
	auto darkBtn_Wrapper = btn_Wrapper->addWidget(std::make_unique<Wt::WContainerWidget>());
	darkBtn_Wrapper->addStyleClass(btn_wrapper_styles);
	darkBtn_Wrapper->addWidget(std::make_unique<Wt::WPushButton>("light"))->setStyleClass("btn btn-dark-light");
	darkBtn_Wrapper->addWidget(std::make_unique<Wt::WPushButton>("dark"))->setStyleClass("btn btn-dark-dark");
	auto dark_disabled = darkBtn_Wrapper->addWidget(std::make_unique<Wt::WPushButton>("disabled"));
	dark_disabled->setStyleClass("btn btn-dark");
	dark_disabled->setDisabled(true);
	darkBtn_Wrapper->addWidget(std::make_unique<Wt::WPushButton>("dark"))->setStyleClass("btn btn-dark");
	

}