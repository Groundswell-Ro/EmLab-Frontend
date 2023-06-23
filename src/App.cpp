#include "include/App.h"

EventManagerLab::EventManagerLab()
	: WContainerWidget(),
	  login_(std::make_shared<Login>())
{
	setStyleClass("flex flex-col h-screen");
	login_->changed().connect(this, &EventManagerLab::handleUserAuth);
	login_->changed().emit();

	createAuth();
}

// create Authentification/Registration page
void EventManagerLab::createAuth() {
	this->clear();
	auto auth = addWidget(std::make_unique<Auth>(login_));
	auth->dev_loginUser("testfsdf1@gmail.com", "asdfghj1");
	// auth->dev_loginUser("client@gmail.com", "asdfghj1");
	// auth->dev_loginUser("provider@gmail.com", "asdfghj1");
}

// handle app state depending on user login status
void EventManagerLab::handleUserAuth() 
{	
	createApp();

	if (login_->isLoggedIn())
	{
		auto profile_btn = std::make_unique<Wt::WPushButton>();

		auto profile_img = Wt::WLink("resources/images/alex.jpg");


		profile_btn->setIcon(profile_img);

		profile_btn->setStyleClass("btn-user-image my-1 me-1 p-0 after:bg-green-400 ");
		auto user_menu_ptr = std::make_unique<Wt::WPopupMenu>(stack_);
		// user_menu_ptr->
		user_menu_ = user_menu_ptr.get();

		auto logout_btn = std::make_unique<Wt::WMenuItem>("Log Out");

		user_menu_->setStyleClass("basic-widget px-1 py-2");
		auto profile = user_menu_->addItem("Profile",  std::make_unique<UserPortofolio>(login_));
		auto settings = user_menu_->addItem("Settings", std::make_unique<UserSettingsPage>(login_));
		auto logout = user_menu_->addItem(std::move(logout_btn));

		logout->clicked().connect(this, [=]() { login_->logout(); });
		profile->clicked().connect(this, [=](){ menuItemSelected(profile); });
		settings->clicked().connect(this, [=](){ menuItemSelected(settings); });
		
		Wt::WString user_menu_item_styles = "bg-body-hover-border block rounded-md text-inherit flex items-center text-sm [&>a]:px-8 [&>a]:py-2";
		profile->setStyleClass(user_menu_item_styles);
		settings->setStyleClass(user_menu_item_styles);
		logout->setStyleClass("btn btn-primary text-xs ms-auto w-fit");


		profile_btn->setMenu(std::move(user_menu_ptr));
		navbar_->bindWidget("user-menu", std::move(profile_btn));

		// settings->clicked().emit(Wt::WMouseEvent());
		user_menu_->select(settings);
		
	}else {
		// remove user_menu_ widget
		auto login_btn = navbar_->bindWidget("user-menu", std::make_unique<Wt::WPushButton>("Log In"));
		login_btn->setStyleClass("btn btn-primary");

		login_btn->clicked().connect(this, &EventManagerLab::createAuth);
	}

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
	stack_->setStyleClass("flex-grow !overflow-y-scroll");
	
	// add menu_ items
	auto portofolio_menu_item = nav_menu_->addItem("Home", std::make_unique<PortofoliosPage>(login_));
	auto page_two_menu_item = nav_menu_->addItem("Test", std::make_unique<TestPage>(login_));

	// menu_ Item styles
	Wt::WString list_item_styles = "bg-body-hover-border w-4/5 rounded-md sm:rounded-none h-6 h-full flex items-center justify-center";
	portofolio_menu_item->setStyleClass(list_item_styles);
	page_two_menu_item->setStyleClass(list_item_styles);
	Wt::WString menuItemStyles = "sm:!rounded-none p-3 text-inherit !m-0 font-medium";
	portofolio_menu_item->anchor()->setStyleClass(menuItemStyles); 
	page_two_menu_item->anchor()->setStyleClass(menuItemStyles);

	portofolio_menu_item->clicked().connect(this, [=](){ menuItemSelected(portofolio_menu_item); });
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
	current_menu_item_ = portofolio_menu_item;
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


