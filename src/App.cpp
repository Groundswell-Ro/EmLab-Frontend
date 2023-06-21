#include "include/App.h"

EventManagerLab::EventManagerLab()
	: WContainerWidget(),
	  login_(std::make_shared<Login>())
{
	setStyleClass("flex flex-col h-screen");
	login_->changed().connect(this, &EventManagerLab::handleUserAuth);
	login_->changed().emit();

	// createAuth();
}

// create Authentification/Registration page
void EventManagerLab::createAuth() {
	this->clear();
	auto auth = addWidget(std::make_unique<Auth>(login_));
	// auth->dev_loginUser("client@gmail.com", "asdfghj1");
	// auth->dev_loginUser("provider@gmail.com", "asdfghj1");
}

// handle app state depending on user login status
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

		profile->clicked().connect(this, [=](){ menuItemSelected(profile); });
		settings->clicked().connect(this, [=](){ menuItemSelected(settings); });
		
		Wt::WString userMenuItemStyles = "bg-body-hover-border block rounded-md text-inherit mb-1 flex items-center px-4 py-2 text-sm";
		profile->setStyleClass(userMenuItemStyles);
		settings->setStyleClass(userMenuItemStyles);

		auto log_out = user_menu_wrapper_->bindWidget("auth-btn", std::make_unique<Wt::WPushButton>("Log Out"));
		log_out->clicked().connect(this, [=](){
			login_->logout();
		});
		
	}else {
		// remove user_menu_ widget
		user_menu_wrapper_->bindEmpty("user-menu");
		auto test = navbar_->bindWidget("user-menu", std::make_unique<Wt::WContainerWidget>());
		test->setStyleClass("flex justify-center items-center");
		test->addWidget(createThemeSwitcher());

		auto log_in = test->addWidget(std::make_unique<Wt::WPushButton>("Log In"));
		log_in->setStyleClass("btn btn-primary ms-auto");
		log_in->clicked().connect(this, &EventManagerLab::createAuth);
	}
	// select start page
	// nav_menu_->select(1);
	// menuItemSelected(nav_menu_->itemAt(1));
}

// create Application Page
void EventManagerLab::createApp() {

	this->clear();

	// Create navigation
	navbar_ = addWidget(std::make_unique<Wt::WTemplate>(tr("navbar")));
	auto logo = navbar_->bindWidget("logo", std::make_unique<Wt::WImage>("https://tailwindui.com/img/logos/workflow-mark.svg?color=indigo&shade=500"));
	
	// create nan_menu_ and stack_
	stack_ = addWidget(std::make_unique<Wt::WStackedWidget>());
	nav_menu_ = navbar_->bindWidget("menu", std::make_unique<Wt::WMenu>(stack_));
	stack_->setStyleClass("flex-grow");
	
	// add menu_ items
	auto home_page_menu_item = nav_menu_->addItem("Home", std::make_unique<HomePage>(login_));
	auto page_two_menu_item = nav_menu_->addItem("Test", std::make_unique<TestPage>(login_));

	// menu_ Item styles
	Wt::WString list_item_styles = "bg-body-hover-border w-4/5 rounded-md sm:rounded-none h-6 h-full flex items-center justify-center";
	home_page_menu_item->setStyleClass(list_item_styles);
	page_two_menu_item->setStyleClass(list_item_styles);
	Wt::WString menuItemStyles = "sm:!rounded-none p-3 text-inherit !m-0 font-medium";
	home_page_menu_item->anchor()->setStyleClass(menuItemStyles); 
	page_two_menu_item->anchor()->setStyleClass(menuItemStyles);
	home_page_menu_item->clicked().connect(this, [=](){ menuItemSelected(home_page_menu_item); });
	page_two_menu_item->clicked().connect(this, [=](){ menuItemSelected(page_two_menu_item); });
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

		auto user_menu_temp = navbar_->bindWidget("user-menu", std::make_unique<Wt::WTemplate>(tr("user-menu")));
		auto user_img = user_menu_temp->bindWidget("user-image", std::make_unique<Wt::WImage>("./resources/images/blank-profile-picture.png"));
		user_menu_wrapper_ = user_menu_temp->bindWidget("user-menu-wrapper", std::make_unique<Wt::WTemplate>(tr("user-menu-wrapper")));
		
		// user menu_ toggle open/closed
		user_img->clicked().connect(this, [=](){
			if(user_menu_wrapper_->hasStyleClass("hidden")){
				user_menu_wrapper_->removeStyleClass("hidden");
			} else {
				user_menu_wrapper_->addStyleClass("hidden");
			}
		});
		
		user_menu_wrapper_->bindWidget("theme-switcher", createThemeSwitcher());

	
	current_menu_item_ = home_page_menu_item;
	current_menu_item_->addStyleClass("bg-body-active");
}

// handles styles for selected menu item from nav manu and user menu
void EventManagerLab::menuItemSelected(Wt::WMenuItem *item) {
	if(current_menu_item_ == item) return;

	current_menu_item_->removeStyleClass("bg-body-active");
	current_menu_item_ = item;
	current_menu_item_->addStyleClass("bg-body-active");
}

// create theme switcher light/dark mode
std::unique_ptr<Wt::WPushButton> EventManagerLab::createThemeSwitcher(){
	auto theme_switcher = std::make_unique<Wt::WPushButton>(tr("sun-svg"));
    theme_switcher->setTextFormat(Wt::TextFormat::XHTML);
    theme_switcher->setStyleClass("rounded-full border-0 p-0 flex justify-center items-center");
	auto theme_switcher_ptr = theme_switcher.get();
	// theme switcher toggle dark/light mode
    theme_switcher_ptr->clicked().connect(this, [=](){
        if(Wt::WApplication::instance()->htmlClass() == "dark"){
            theme_switcher_ptr->setText(tr("sun-svg"));
            Wt::WApplication::instance()->setHtmlClass("");
         }else {
            theme_switcher_ptr->setText(tr("moon-svg"));
            Wt::WApplication::instance()->setHtmlClass("dark");
        }
    });
	return theme_switcher;
}


