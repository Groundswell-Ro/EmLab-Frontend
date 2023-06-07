#include "include/App.h"

// Application is a containerWidget
EventManagerLab::EventManagerLab()
	: WContainerWidget(),
	  login_(std::make_shared<Login>())
{
	// past
	// addStyleClass("application");

	// authWidget_ = addChild(std::make_unique<AuthWidget>(login_));

	// login_->changed().connect(this, &EventManagerLab::handleUserAuth);
	// login_->changed().emit();
	// authWidget_->loginFormView_->process();
	//past
	setStyleClass("static");
	
	login_->changed().connect(this, &EventManagerLab::handleUserAuth);
	login_->changed().emit();
	// createAuth();
}

void EventManagerLab::createDevControlers()
{
	auto controlers_container = addWidget(std::make_unique<Wt::WContainerWidget>());
	controlers_container->setStyleClass("flex justify-end");
	bool loginStatus = login_->isLoggedIn(); 

	auto auth_btn = controlers_container->addWidget(std::make_unique<Wt::WPushButton>("Auth"));
	auth_btn->clicked().connect([=] {
		createAuth();
	});

	auto app_btn = controlers_container->addWidget(std::make_unique<Wt::WPushButton>("App"));
	app_btn->clicked().connect([=] {
		handleUserAuth();
	});

	auto login_checkbox = controlers_container->addWidget(std::make_unique<Wt::WCheckBox>("Login"));
	login_checkbox->setChecked(loginStatus);
	login_checkbox->clicked().connect([=] {
		if(login_checkbox->isChecked())
			login_->setUserLogin(true);
		else
			login_->setUserLogin(false);
	});

	auto user_menu_toggle = controlers_container->addWidget(std::make_unique<Wt::WPushButton>("User Menu"));
	user_menu_toggle->clicked().connect(this, [=](){
		if(user_menu_wrapper_->hasStyleClass("hidden")){
			user_menu_wrapper_->removeStyleClass("hidden");
		} else {
			user_menu_wrapper_->addStyleClass("hidden");
		}
	});
}

void EventManagerLab::createAuth() {
	this->clear();
	createDevControlers();
	addWidget(std::make_unique<Auth>(login_));
}

void EventManagerLab::handleUserAuth() 
{	
	createApp();

	if (login_->isLoggedIn())
	{
		// user menu_
		user_menu_ = user_menu_wrapper_->bindWidget("user-menu", std::make_unique<Wt::WMenu>(stack_));
		user_menu_->setStyleClass("list-none m-0 p-1 text-center");

		auto profile = user_menu_->addItem("Profile",  std::make_unique<Wt::WText>("Profile"));
		auto settings = user_menu_->addItem("Settings", std::make_unique<Wt::WText>("Settings"));

		Wt::WString userMenuItemStyles = "bg-body-hover block rounded-md text-inherit mb-1 flex items-center px-4 py-2 text-sm";
		profile->setStyleClass(userMenuItemStyles);
		settings->setStyleClass(userMenuItemStyles);

		auto log_out = user_menu_wrapper_->bindWidget("auth-btn", std::make_unique<Wt::WPushButton>("Log Out"));
		log_out->clicked().connect(this, [=](){
			login_->logout();
		});
	}else {
		// remove user_menu_ widget
		user_menu_wrapper_->bindEmpty("user-menu");

		auto log_in = user_menu_wrapper_->bindWidget("auth-btn", std::make_unique<Wt::WPushButton>("Log In"));
		log_in->clicked().connect(this, &EventManagerLab::createAuth);
	}
	
}

void EventManagerLab::createApp() {
	this->clear();
	createDevControlers();
	// Create navigation
	navbar_ = addWidget(std::make_unique<Wt::WTemplate>(tr("navbar")));
	auto logo = navbar_->bindWidget("logo", std::make_unique<Wt::WImage>("https://tailwindui.com/img/logos/workflow-mark.svg?color=indigo&shade=500"));
	
	// create nan_menu_ and stack_
	stack_ = addWidget(std::make_unique<Wt::WStackedWidget>());
	nav_menu_ = navbar_->bindWidget("menu", std::make_unique<Wt::WMenu>(stack_));

	// add menu_ items
	auto home_page_menu_item = nav_menu_->addItem("Home", std::make_unique<HomePage>());
	auto page_two_menu_item = nav_menu_->addItem("Test", std::make_unique<TestPage>());
	current_menu_item_ = home_page_menu_item;

	// menu_ Item styles
	Wt::WString list_item_styles = "bg-body-hover w-4/5 rounded-md sm:rounded-none h-6 h-full flex items-center justify-center";
	home_page_menu_item->setStyleClass(list_item_styles);
	page_two_menu_item->setStyleClass(list_item_styles);
	Wt::WString menuItemStyles = "sm:!rounded-none p-3 text-inherit !m-0 font-medium";
	home_page_menu_item->anchor()->setStyleClass(menuItemStyles);
	page_two_menu_item->anchor()->setStyleClass(menuItemStyles);

	// Hamburger Button to toggle menu_ opn mobile
	auto hamburger_btn = navbar_->bindWidget("hamburger-button", std::make_unique<Wt::WPushButton>(tr("hamburger-svg")));
	hamburger_btn->setTextFormat(Wt::TextFormat::XHTML);
	hamburger_btn->clicked().connect(this, [=](){
		if(nav_menu_->hasStyleClass("hidden")){
			nav_menu_->removeStyleClass("hidden");
			hamburger_btn->setText(tr("x-svg"));
		} else {
			nav_menu_->addStyleClass("hidden");
			hamburger_btn->setText(tr("hamburger-svg"));
		}
	});

	auto user_img = navbar_->bindWidget("user-image", std::make_unique<Wt::WImage>("./resources/images/blank-profile-picture.png"));
	user_menu_wrapper_ = navbar_->bindWidget("user-menu-wrapper", std::make_unique<Wt::WTemplate>(tr("user-menu-wrapper")));

	// user menu_ toggle open/closed
	user_img->clicked().connect(this, [=](){
		if(user_menu_wrapper_->hasStyleClass("hidden")){
			user_menu_wrapper_->removeStyleClass("hidden");
		} else {
			user_menu_wrapper_->addStyleClass("hidden");
		}
	});

	createThemeSwitcher();
}

void EventManagerLab::createThemeSwitcher(){
	auto theme_switcher = user_menu_wrapper_->bindWidget("theme-switcher", std::make_unique<Wt::WPushButton>(tr("sun-svg")));
    theme_switcher->setTextFormat(Wt::TextFormat::XHTML);
    theme_switcher->setStyleClass("relative w-10 h-5 m-1 mr-2 p-0 bg-white rounded-full shadow border-0");
    theme_switcher->setText(tr("sun-svg"));

	// theme switcher toggle dark/light mode
    theme_switcher->clicked().connect(this, [=](){
        if(Wt::WApplication::instance()->htmlClass() == "dark"){
            theme_switcher->setText(tr("sun-svg"));
            Wt::WApplication::instance()->setHtmlClass("");
         }else {
            theme_switcher->setText(tr("moon-svg"));
            Wt::WApplication::instance()->setHtmlClass("dark");
        }
    });
	
}
