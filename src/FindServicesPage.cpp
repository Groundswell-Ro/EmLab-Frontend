#include "include/FindServicesPage.h"
#include "include/UserProfile.h"

#include <Wt/WString.h>
#include <Wt/WDateTime.h>
#include <Wt/WDoubleSpinBox.h>
#include <Wt/WText.h>
#include <Wt/WPanel.h>
#include <Wt/WMenu.h>
#include <Wt/WStackedWidget.h>
#include <Wt/WAnimation.h>
#include <iomanip>

string format_time(int minutes) {
    // Calculate the hours and minutes
    int hours = minutes / 60;
    minutes %= 60;
    
    // Determine the AM or PM designation
    std::string period = (hours < 12) ? "AM" : "PM";
    if (hours > 12) {
        hours -= 12;
    }
    
    // Format the time as a string
    stringstream ss;
    ss << setfill('0') << setw(2) << hours << ":" << setw(2) << minutes << " " << period;
    return ss.str();
}

FindServicesPage::FindServicesPage(std::shared_ptr<Login> login)
    : WContainerWidget(),
	login_(login)
{
	setStyleClass("relative w-full h-full flex");
	// addWidget(std::make_unique<UserProfile>(login_, login_->provider().name));
}

void FindServicesPage::addSidebar()
{
	sidebar_ = insertWidget(0, std::make_unique<Wt::WTemplate>(tr("home-sidebar")));
	sidebar_->addStyleClass("m-3 absolute  left-0 top-0 sm:relative max-w-64 w-64 min-w-[16rem] bg-body z-20 transition-spacing ease-in-out delay-75 duration-300 shadow-2xl rounded-xl -ms-64");
	sidebar_content_ = sidebar_->bindWidget("sidebar-content", std::make_unique<Wt::WTemplate>());
	auto toggle_sidebar_btn = sidebar_->bindWidget("sidebar-toggler", std::make_unique<Wt::WPushButton>(tr("book-svg")));
	toggle_sidebar_btn->setTextFormat(Wt::TextFormat::XHTML);
	toggle_sidebar_btn->clicked().connect(this, [=](){
		if(sidebar_->hasStyleClass("-ms-64")){
			sidebar_->removeStyleClass("-ms-64");
		} else {
			sidebar_->addStyleClass("-ms-64");
		}
	});
	calendar_ = sidebar_->bindWidget("calendar", std::make_unique<Calendar>());
	calendar_->setStyleClass("calendar");

	selected_date_ = sidebar_->bindWidget("selected-date", std::make_unique<Wt::WText>(event_date.toString(Emlab::DATEFORMAT)));
	add_event_btn_ = sidebar_->bindWidget("add-event-btn", std::make_unique<Wt::WPushButton>("Add event"));
	add_event_btn_->clicked().connect(this, &FindServicesPage::addEvent);
	save_event_btn_ = sidebar_->bindWidget("save-event-btn", std::make_unique<Wt::WPushButton>("Save"));
	save_event_btn_->clicked().connect(this, &FindServicesPage::saveEvent);
	// save_event_btn_->disable();
	cancel_event_btn_ = sidebar_->bindWidget("cancel-event-btn", std::make_unique<Wt::WPushButton>(tr("x-svg-sm")));
	cancel_event_btn_->setTextFormat(Wt::TextFormat::XHTML);
	cancel_event_btn_->clicked().connect(this, &FindServicesPage::cancelEvent);
	cancel_event_btn_->setDisabled(true);
	selected_date_ = sidebar_->bindWidget("selected-date", std::make_unique<Wt::WText>(event_date.toString(Emlab::DATEFORMAT)));
	calendar_->clicked().connect(this, &FindServicesPage::calendarClicked);

	// bellow code is used to toggle open the sidebar and start the process of adding an event
	sidebar_->addStyleClass("-ms-64");
	// addEvent();

}

void FindServicesPage::removeSidebar()
{
	removeWidget(sidebar_);
}

void FindServicesPage::saveEvent()
{
	std::cout << "\n save event \n";
	add_event_btn_->removeStyleClass("-translate-x-32");
	save_event_btn_->addStyleClass("-translate-x-28");
	sidebar_content_->setTemplateText("");
	cancel_event_btn_->setDisabled(true);
	Emlab::EventInfo eventInfo;

	auto dateTime = Wt::WDateTime(event_date, event_time);
	eventInfo.dateTime = dateTime.toString(Emlab::DATETIMEFORMAT).toUTF8();
	eventInfo.duration = (double)event_duration/60;
	eventInfo.location = location_input_->text().toUTF8();
	eventInfo.description = description_input_->text().toUTF8();

	auto event = Emlab::Comunication::addEventInfo(login_->userToken(), eventInfo);
	calendar_->enable();
}

void FindServicesPage::addEvent()
{
	std::cout << "\n add event \n";
	// auto date = *(calendar_->selection().begin());
	add_event_btn_->addStyleClass("-translate-x-32");
	save_event_btn_->removeStyleClass("-translate-x-28");
	sidebar_content_->setTemplateText(tr("sidebar-event-open"));

	auto panel = sidebar_content_->bindWidget("event-view", std::make_unique<Wt::WPanel>());

	panel->setTitle("Event Info");
	panel->setCollapsible(true);
	panel->setCollapsed(false);
	panel->setAnimation(Wt::WAnimation(Wt::AnimationEffect::SlideInFromTop, Wt::TimingFunction::EaseInOut, 150));
	auto event_form = panel->setCentralWidget(std::make_unique<Wt::WTemplate>(tr("event-form")));
	
	panel->setStyleClass("panel");
	panel->titleBarWidget()->setStyleClass("panel-head");
	panel->centralWidget()->setStyleClass("panel-body");
	
	cancel_event_btn_->setDisabled(false);

	// Event Information Form
	{
		// event_form->addFunction("id", &Wt::WTemplate::Functions::id);

		start_input_ = event_form->bindWidget("time-input", std::make_unique<TimeInput>(DisplayType::START));
		duration_input_ = event_form->bindWidget("duration-input", std::make_unique<TimeInput>(DisplayType::DURATION));
		end_input_ = event_form->bindWidget("end-input", std::make_unique<TimeInput>(DisplayType::END));
		end_input_->hideButtons();
		std::cout << "\n\n event time :" << event_time.toString(Emlab::TIMEFORMAT).toUTF8() << "\n\n";
		start_input_->setValue(event_time.toString(Emlab::TIMEFORMAT).toUTF8());
		duration_input_->setValue(std::to_string(event_duration/60) + " h");
		end_input_->setValue(event_time.addSecs(event_duration*60).toString(Emlab::TIMEFORMAT).toUTF8());


		// cast 
		dynamic_cast<Wt::WPushButton*>(start_input_->resolveWidget("add-btn"))->clicked().connect(this, [=](){
			event_time = event_time.addSecs(60*15);
			start_input_->setValue(event_time.toString(Emlab::TIMEFORMAT).toUTF8());
			end_input_->setValue(event_time.addSecs(event_duration*60).toString(Emlab::TIMEFORMAT).toUTF8());
		});

		dynamic_cast<Wt::WPushButton*>(start_input_->resolveWidget("sub-btn"))->clicked().connect(this, [=](){
			event_time = event_time.addSecs(-60*15);
			start_input_->setValue(event_time.toString(Emlab::TIMEFORMAT).toUTF8());
			end_input_->setValue(event_time.addSecs(event_duration*60).toString(Emlab::TIMEFORMAT).toUTF8());
		});

		dynamic_cast<Wt::WPushButton*>(duration_input_->resolveWidget("add-btn"))->clicked().connect(this, [=](){
			event_duration < 60*12 ? event_duration += 15 : event_duration = 60*12;
			duration_input_->setValue(format_time(event_duration));
			end_input_->setValue(event_time.addSecs(event_duration*60).toString(Emlab::TIMEFORMAT).toUTF8());
		});

		dynamic_cast<Wt::WPushButton*>(duration_input_->resolveWidget("sub-btn"))->clicked().connect(this, [=](){
			event_duration > 15 ? event_duration -= 15 : event_duration = 15;
			duration_input_->setValue(format_time(event_duration));
			end_input_->setValue(event_time.addSecs(event_duration*60).toString(Emlab::TIMEFORMAT).toUTF8());
		});

		location_input_ = event_form->bindWidget("location-input", std::make_unique<Wt::WLineEdit>());
		description_input_ = event_form->bindWidget("description-input", std::make_unique<Wt::WTextArea>());

		event_form->bindWidget("location-pin-svg", std::make_unique<Wt::WText>(tr("location-pin-svg-sm")));
		event_form->bindWidget("book-open-svg", std::make_unique<Wt::WText>(tr("book-open-svg-sm")));

		
		location_input_->setPlaceholderText(" ");
		description_input_->setPlaceholderText(" ");
	}
	calendar_->disable();

}

void FindServicesPage::cancelEvent()
{
	std::cout << "\n cancel event \n";
	add_event_btn_->removeStyleClass("-translate-x-32");
	save_event_btn_->addStyleClass("-translate-x-28");
	sidebar_content_->setTemplateText("");
	cancel_event_btn_->setDisabled(true);
	calendar_->enable();
}

void FindServicesPage::calendarClicked(const Wt::WDate date)
{
	selected_date_->setText(date.toString(Emlab::DATEFORMAT));
}

