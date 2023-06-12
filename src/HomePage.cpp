#include "include/HomePage.h"
#include <Wt/WText.h>
#include <Wt/WTemplate.h>

HomePage::HomePage(std::shared_ptr<Login> login)
    : WContainerWidget(),
	login_(login)
{
	setStyleClass("relative h-full w-full flex");
	createSidebar();
	createProfile();
}

void HomePage::createSidebar()
{
	sidebar_ = addWidget(std::make_unique<Wt::WTemplate>(tr("home-sidebar")));
	sidebar_->addStyleClass("absolute left-0 h-full sm:relative max-w-64 w-64 min-w-[16rem] bg-body transition-spacing ease-in-out delay-75 duration-300 shadow-2xl rounded");
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
	calendar_->setStyleClass("calendar flex justify-center w-full");
	calendar_->setSelectionMode(Wt::SelectionMode::Single);
	calendar_->setHorizontalHeaderFormat(Wt::CalendarHeaderFormat::SingleLetterDayNames);
	calendar_->select(Wt::WDate::currentDate());
	selected_date_ = sidebar_->bindWidget("selected-date", std::make_unique<Wt::WText>(Wt::WDate::currentDate().toString(Emlab::DATE_FORMAT)));
	add_event_btn_ = sidebar_->bindWidget("add-event-btn", std::make_unique<Wt::WPushButton>("Add event"));
	add_event_btn_->clicked().connect(this, &HomePage::addEvent);
	save_event_btn_ = sidebar_->bindWidget("save-event-btn", std::make_unique<Wt::WPushButton>("Save"));
	save_event_btn_->clicked().connect(this, &HomePage::saveEvent);
	save_event_btn_->disable();
	cancel_event_btn_ = sidebar_->bindWidget("cancel-event-btn", std::make_unique<Wt::WPushButton>(tr("x-svg-sm")));
	cancel_event_btn_->setTextFormat(Wt::TextFormat::XHTML);
	cancel_event_btn_->clicked().connect(this, &HomePage::cancelEvent);
	cancel_event_btn_->setDisabled(true);
	selected_date_ = sidebar_->bindWidget("selected-date", std::make_unique<Wt::WText>(Wt::WDate::currentDate().toString(Emlab::DATE_FORMAT)));
	calendar_->clicked().connect(this, &HomePage::calendarClicked);
	addEvent();
}

void HomePage::saveEvent()
{
	std::cout << "\n save event \n";
	add_event_btn_->removeStyleClass("-translate-x-28");
	save_event_btn_->addStyleClass("-translate-x-28");
	sidebar_content_->setTemplateText("");
	cancel_event_btn_->setDisabled(true);
}

void HomePage::addEvent()
{
	std::cout << "\n add event \n";
	add_event_btn_->addStyleClass("-translate-x-28");
	save_event_btn_->removeStyleClass("-translate-x-28");
	sidebar_content_->setTemplateText(tr("sidebar-event-open"));
	cancel_event_btn_->setDisabled(false);

	auto toggle_size_btn = sidebar_content_->bindWidget("toggle-size-btn", std::make_unique<Wt::WPushButton>(tr("maximize-svg-sm")));
	auto event_form = sidebar_content_->bindWidget("event-form", std::make_unique<Wt::WTemplate>(tr("event-form")));

	auto date = *(calendar_->selection().begin());
	toggle_size_btn->setTextFormat(Wt::TextFormat::XHTML);

	

	// Event Information Form
	{

		start_time_input_ = event_form->bindWidget("start-time-input", std::make_unique<Wt::WTimeEdit>());
		auto end_time_input_ = event_form->bindWidget("end-time-input", std::make_unique<Wt::WTimeEdit>());
		auto start_add_time_btn = event_form->bindWidget("start-add-time-btn", std::make_unique<Wt::WPushButton>(tr("plus-svg-sm")));
		auto start_sub_time_btn = event_form->bindWidget("start-sub-time-btn", std::make_unique<Wt::WPushButton>(tr("minus-svg-sm")));
		auto end_sub_time_btn = event_form->bindWidget("end-sub-time-btn", std::make_unique<Wt::WPushButton>(tr("minus-svg-sm")));
		auto end_add_time_btn = event_form->bindWidget("end-add-time-btn", std::make_unique<Wt::WPushButton>(tr("plus-svg-sm")));
		location_input_ = event_form->bindWidget("location-input", std::make_unique<Wt::WLineEdit>());
		description_input_ = event_form->bindWidget("description-input", std::make_unique<Wt::WTextArea>());


		start_time_input_->setFormat(Emlab::TIME_FORMAT);
		start_time_input_->setTime(Wt::WTime(12,0,0));
		start_time_input_->setDisabled(true);

		end_time_input_->setFormat(Emlab::TIME_FORMAT);
		end_time_input_->setTime(Wt::WTime(12+(event_duration/60),0,0));
		end_time_input_->setDisabled(true);


		start_add_time_btn->setTextFormat(Wt::TextFormat::XHTML);
		start_add_time_btn->clicked().connect(this, [=](){
			auto time = start_time_input_->time();
			time = time.addSecs(60*15);
			time.hour() > 23 ? time = Wt::WTime(0,0,0) : time = time;
			start_time_input_->setTime(time);
			end_time_input_->setTime(time.addSecs(60*event_duration));
			
		});

		start_sub_time_btn->setTextFormat(Wt::TextFormat::XHTML);	
		start_sub_time_btn->clicked().connect(this, [=](){
			auto time = start_time_input_->time();
			time.minute() == 0 && time.hour() == 0 ? time = Wt::WTime(24,0,0) : time = time;
			time = time.addSecs(-(60*15));
			start_time_input_->setTime(time);
			end_time_input_->setTime(time.addSecs(60*event_duration));
		});


		end_add_time_btn->setTextFormat(Wt::TextFormat::XHTML);
		end_add_time_btn->clicked().connect(this, [=](){
			auto end_time = end_time_input_->time();
			auto start_time = start_time_input_->time();
			event_duration < 60*12 ? event_duration += 15 : event_duration = event_duration;
			end_time.hour() > 23 ? end_time = Wt::WTime(0,0,0) : end_time = end_time;
			end_time = start_time.addSecs(60*event_duration);
			end_time_input_->setTime(end_time);
		});

		end_sub_time_btn->setTextFormat(Wt::TextFormat::XHTML);
		end_sub_time_btn->clicked().connect(this, [=](){
			auto end_time = end_time_input_->time();
			auto start_time = start_time_input_->time();
			event_duration > 15 ? event_duration -= 15 : event_duration = event_duration;
			end_time = start_time.addSecs(60*event_duration);
			
			end_time_input_->setTime(end_time);
			
		});



	}


	toggle_size_btn->clicked().connect(this, [=](){
		if(sidebar_content_->hasStyleClass("expand")){
			toggle_size_btn->setText(tr("maximize-svg-sm"));
			sidebar_content_->removeStyleClass("expand");
		}else {
			toggle_size_btn->setText(tr("minimize-svg-sm"));
			sidebar_content_->addStyleClass("expand");
		}
	});

}

void HomePage::cancelEvent()
{
	std::cout << "\n cancel event \n";
	add_event_btn_->removeStyleClass("-translate-x-28");
	save_event_btn_->addStyleClass("-translate-x-28");
	sidebar_content_->setTemplateText("");
	cancel_event_btn_->setDisabled(true);
}

void HomePage::calendarClicked(const Wt::WDate date)
{
	selected_date_->setText(date.toString(Emlab::DATE_FORMAT));
}

void HomePage::createProfile()
{
	auto profile_ = addWidget(std::make_unique<Wt::WTemplate>(tr("home-profile")));
	profile_->setStyleClass("max-w-screen-xl min-w-[16rem] flex-grow p-5 m-5 mb-0 rounded-b-none bg-green-200 rounded-lg shadow-lg mx-auto");
	auto header = profile_->bindWidget("profile-header", std::make_unique<Wt::WTemplate>(tr("home-profile-header")));
	auto main = profile_->bindWidget("profile-main", std::make_unique<Wt::WTemplate>(tr("home-profile-main")));
	auto footer = profile_->bindWidget("profile-footer", std::make_unique<Wt::WTemplate>(tr("home-profile-footer")));
}

