#include "include/HomePage.h"

HomePage::HomePage(std::shared_ptr<Login> login)
    : WContainerWidget(),
	login_(login)
{
	setStyleClass("relative h-full w-full flex");
	createSidebar();
	createProfile();
	addEvent();
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
	// calendar_->setStyleClass("calendar flex justify-center w-full");

	selected_date_ = sidebar_->bindWidget("selected-date", std::make_unique<Wt::WText>(Wt::WDate::currentDate().toString(date_format_)));
	add_event_btn_ = sidebar_->bindWidget("add-event-btn", std::make_unique<Wt::WPushButton>("Add event"));
	add_event_btn_->clicked().connect(this, &HomePage::addEvent);
	save_event_btn_ = sidebar_->bindWidget("save-event-btn", std::make_unique<Wt::WPushButton>("Save"));
	save_event_btn_->clicked().connect(this, &HomePage::saveEvent);
	// save_event_btn_->disable();
	cancel_event_btn_ = sidebar_->bindWidget("cancel-event-btn", std::make_unique<Wt::WPushButton>(tr("x-svg-sm")));
	cancel_event_btn_->setTextFormat(Wt::TextFormat::XHTML);
	cancel_event_btn_->clicked().connect(this, &HomePage::cancelEvent);
	cancel_event_btn_->setDisabled(true);
	selected_date_ = sidebar_->bindWidget("selected-date", std::make_unique<Wt::WText>(Wt::WDate::currentDate().toString(date_format_)));
	calendar_->clicked().connect(this, &HomePage::calendarClicked);
}

void HomePage::saveEvent()
{
	std::cout << "\n save event \n";
	add_event_btn_->removeStyleClass("-translate-x-28");
	save_event_btn_->addStyleClass("-translate-x-28");
	sidebar_content_->setTemplateText("");
	cancel_event_btn_->setDisabled(true);
	EventModule::EventData event_data;

	auto date = calendar_->selection().begin();
	auto time = start_time_input_->time();
	auto dateTime = Wt::WDateTime(*date, time);

	// std::cout << "\n date string: " << dateTime.toString(datetime_format_) << "\n\n";
	event_data.eventInfo.dateTime = dateTime.toString(datetime_format_).toUTF8();
	event_data.eventInfo.duration = (double)event_duration/60;
	event_data.eventInfo.location = location_input_->text().toUTF8();
	event_data.eventInfo.description = description_input_->text().toUTF8();

	auto event = login_->registerEvent(event_data);
	calendar_->enable();
}

void HomePage::addEvent()
{
	std::cout << "\n add event \n";
	// auto date = *(calendar_->selection().begin());
	add_event_btn_->addStyleClass("-translate-x-28");
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
	auto toggle_size_btn = sidebar_content_->bindWidget("toggle-size-btn", std::make_unique<Wt::WPushButton>(tr("maximize-svg-sm")));
	toggle_size_btn->setTextFormat(Wt::TextFormat::XHTML);

	// Event Information Form
	{
		event_form->addFunction("id", &Wt::WTemplate::Functions::id);

		start_time_input_ = event_form->bindWidget("start-time-input", std::make_unique<Wt::WTimeEdit>());
		duration_input_ = event_form->bindWidget("duration-time-input", std::make_unique<Wt::WLineEdit>());
		location_input_ = event_form->bindWidget("location-input", std::make_unique<Wt::WLineEdit>());
		description_input_ = event_form->bindWidget("description-input", std::make_unique<Wt::WTextArea>());
		
		auto start_add_time_btn = event_form->bindWidget("start-add-time-btn", std::make_unique<Wt::WPushButton>(tr("plus-svg-sm")));
		auto start_sub_time_btn = event_form->bindWidget("start-sub-time-btn", std::make_unique<Wt::WPushButton>(tr("minus-svg-sm")));
		auto duration_sub_time_btn = event_form->bindWidget("duration-sub-time-btn", std::make_unique<Wt::WPushButton>(tr("minus-svg-sm")));
		auto duration_add_time_btn = event_form->bindWidget("duration-add-time-btn", std::make_unique<Wt::WPushButton>(tr("plus-svg-sm")));
		auto end_hour = event_form->bindWidget("end-hour", std::make_unique<Wt::WText>("13:00 PM"));

		event_form->bindWidget("clock-svg", std::make_unique<Wt::WText>(tr("clock-svg-sm")));
		event_form->bindWidget("hourglass-svg", std::make_unique<Wt::WText>(tr("hourglass-svg-sm")));
		event_form->bindWidget("clock-svg-fill", std::make_unique<Wt::WText>(tr("clock-svg-fill-sm")));
		event_form->bindWidget("location-pin-svg", std::make_unique<Wt::WText>(tr("location-pin-svg-sm")));
		event_form->bindWidget("book-open-svg", std::make_unique<Wt::WText>(tr("book-open-svg-sm")));

		start_time_input_->setFormat(time_format_);
		start_time_input_->setTime(Wt::WTime(12,0,0));
		start_time_input_->setDisabled(true);

		duration_input_->setDisabled(true);
		duration_input_->setText(std::to_string(event_duration/60) + " Hours");

		start_add_time_btn->setTextFormat(Wt::TextFormat::XHTML);
		start_add_time_btn->clicked().connect(this, [=](){
			auto time = start_time_input_->time();
			time = time.addSecs(60*15);
			time.hour() > 23 ? time = Wt::WTime(0,0,0) : time = time;
			start_time_input_->setTime(time);
			end_hour->setText(time.addSecs(event_duration*60).toString(time_format_));
		});

		start_sub_time_btn->setTextFormat(Wt::TextFormat::XHTML);	
		start_sub_time_btn->clicked().connect(this, [=](){
			auto time = start_time_input_->time();
			time.minute() == 0 && time.hour() == 0 ? time = Wt::WTime(24,0,0) : time = time;
			time = time.addSecs(-(60*15));
			start_time_input_->setTime(time);
			end_hour->setText(time.addSecs(event_duration*60).toString(time_format_));
		});

		duration_add_time_btn->setTextFormat(Wt::TextFormat::XHTML);
		duration_add_time_btn->clicked().connect(this, [=](){
			auto start_time = start_time_input_->time();
			Wt::WString duration_string;
			event_duration < 60*12 ? event_duration += 15 : event_duration = event_duration;
			duration_string += std::to_string(event_duration/60);
			if(event_duration%60 != 0) {
				duration_string += ":" + std::to_string(event_duration%60);
			}else {
			}
			duration_string += " Hours";
			duration_input_->setText(duration_string);
			end_hour->setText(start_time.addSecs(event_duration*60).toString(time_format_));

		});

		duration_sub_time_btn->setTextFormat(Wt::TextFormat::XHTML);
		duration_sub_time_btn->clicked().connect(this, [=](){
			auto start_time = start_time_input_->time();
			Wt::WString duration_string;
			event_duration > 15 ? event_duration -= 15 : event_duration = event_duration;
			duration_string += std::to_string(event_duration/60);
			if(event_duration%60 != 0) {
				duration_string += ":" + std::to_string(event_duration%60);
			}else {
			}
			duration_string += " Hours";
			duration_input_->setText(duration_string);
			end_hour->setText(start_time.addSecs(event_duration*60).toString(time_format_));
		});

		location_input_->setPlaceholderText(" ");
		description_input_->setPlaceholderText(" ");
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
	calendar_->disable();

}

void HomePage::cancelEvent()
{
	std::cout << "\n cancel event \n";
	add_event_btn_->removeStyleClass("-translate-x-28");
	save_event_btn_->addStyleClass("-translate-x-28");
	sidebar_content_->setTemplateText("");
	cancel_event_btn_->setDisabled(true);
	calendar_->enable();
}

void HomePage::calendarClicked(const Wt::WDate date)
{
	selected_date_->setText(date.toString(date_format_));
}

void HomePage::createProfile()
{
	auto profile_ = addWidget(std::make_unique<Wt::WTemplate>(tr("home-profile")));
	profile_->setStyleClass("max-w-screen-xl min-w-[16rem] flex-grow p-5 m-5 mb-0 rounded-b-none rounded-lg shadow-lg mx-auto shadow-lg border-t border-solid border-gray-200");
	auto header = profile_->bindWidget("profile-header", std::make_unique<Wt::WTemplate>(tr("home-profile-header")));
	auto profile_calendar = header->bindWidget("calendar", std::make_unique<Calendar>());
	auto profile_image = header->bindWidget("image", std::make_unique<Wt::WImage>(Wt::WLink("resources/images/alex.jpg")));
	auto profile_services = header->bindWidget("services-list", std::make_unique<Wt::WText>(tr("home-profile-services")));

	auto profile_stack = profile_->bindWidget("profile-stack", std::make_unique<Wt::WStackedWidget>());
	auto profile_menu = profile_->bindWidget("profile-menu", std::make_unique<Wt::WMenu>(profile_stack));

	profile_menu->setStyleClass("flex flex-col");

	auto galery = std::make_unique<Wt::WContainerWidget>();
	auto services = std::make_unique<Wt::WContainerWidget>();
	auto reviews = std::make_unique<Wt::WContainerWidget>();

	galery->setStyleClass("m-5 p-5 bg-green-100");
	services->setStyleClass("m-5 p-5 bg-yellow-100");
	reviews->setStyleClass("m-5 p-5 bg-blue-100");

	auto galery_menu = profile_menu->addItem("Galery", std::move(galery));
	auto services_menu = profile_menu->addItem("Services", std::move(services));
	auto reviews_menu = profile_menu->addItem("Reviews", std::move(reviews));

}

