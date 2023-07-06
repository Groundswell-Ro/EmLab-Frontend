#include "include/App.h"

#include "include/UserSettingsPage.h"
#include "include/UserProfile.h"
#include "include/Auth.h"

#include <iostream>
#include <Wt/WCheckBox.h>
#include <Wt/WMessageBox.h>
#include <Wt/WPushButton.h>

EventManagerLab::EventManagerLab(const Wt::WEnvironment &env)
	: Wt::WApplication(env),
	  login_(std::make_shared<Login>())
{
	
	
	// Set up application resourses and settings
	setTitle("Event Manager Lab");
	// setCssTheme("");
	// setCssTheme("polished");
	// include tailwind css file
	useStyleSheet("resources/themes/tailwind/dist/tailwind.css");



	// import resources
	messageResourceBundle().use(appRoot() + "resources/xml/Auth");
	messageResourceBundle().use(appRoot() + "resources/xml/General");
	messageResourceBundle().use(appRoot() + "resources/xml/Navbar");
	messageResourceBundle().use(appRoot() + "resources/xml/HomePage");
	messageResourceBundle().use(appRoot() + "resources/xml/ProfilePage");
	messageResourceBundle().use(appRoot() + "resources/xml/Svg");
	messageResourceBundle().use(appRoot() + "resources/xml/CreateProfileDialog");
	messageResourceBundle().use(appRoot() + "resources/xml/SettingsPage");
	// require("resources/Js/CopyToClipboard.js");
	require("resources/Js/Utility.js");
	require("https://cdn.jsdelivr.net/npm/alpinejs@3.x.x/dist/cdn.min.js");

	// 

	root()->setStyleClass("app");
	login_->changed().connect(this, &EventManagerLab::handleUserAuth);
	login_->changed().emit();

	// createAuth();
}

EventManagerLab::~EventManagerLab()
{
	if(login_->isLoggedIn()) {
		login_->logout();
	}
}

// create Authentification/Registration page
void EventManagerLab::createAuth() {
	auto auth = root()->addChild(std::make_unique<Auth>(login_));
	// auth->dev_loginUser("client1@gmail.com", "asdfghj1");
	// auth->dev_loginUser("alex@gmail.com", "asdfghj1");

}

// handle app state depending on user login status
void EventManagerLab::handleUserAuth() 
{	
	if (login_->isLoggedIn())
	{
		auto profile_img = Wt::WLink(login_->getUserPhotoPath() + "profile.jpg");

		auto profile_btn = std::make_unique<Wt::WPushButton>();
		profile_btn->setIcon(profile_img);
		profile_btn->setObjectName("profile-btn");
		services_page_->addSidebar();
		profile_btn->setStyleClass("btn-user-image my-1 me-1 p-0 after:bg-green-400 ");
		auto user_menu_ptr = std::make_unique<Wt::WPopupMenu>(stack_);

		user_menu_ = user_menu_ptr.get();

		auto logout_btn = std::make_unique<Wt::WMenuItem>("Log Out");

		user_menu_->setStyleClass("basic-widget px-1 py-2");
		auto settings = user_menu_->addItem("Settings", std::make_unique<UserSettingsPage>(login_));

		settings->clicked().connect(this, [=](){ menuItemSelected(settings); });
		
		Wt::WString user_menu_item_styles = "bg-body-hover-border block rounded-md text-inherit flex items-center text-sm [&>a]:px-8 [&>a]:py-2";
		settings->setStyleClass(user_menu_item_styles);

		if(login_->getUserRole() == Emlab::PROVIDERROLE)
		{
			auto profile_content = std::make_unique<UserProfile>(login_, login_->provider().id);
			auto profile = user_menu_->addItem("Profile",  std::move(profile_content));
			profile->clicked().connect(this, [=](){ menuItemSelected(profile); });
			profile->setStyleClass(user_menu_item_styles);

		}
		auto logout = user_menu_->addItem(std::move(logout_btn));
		logout->clicked().connect(this, [=]() { login_->logout(); });
		logout->setStyleClass("btn btn-primary text-xs ms-auto w-fit");

		profile_btn->setMenu(std::move(user_menu_ptr));
		navbar_->bindWidget("user-menu", std::move(profile_btn));

		user_menu_->select(user_menu_->itemAt(1));
		// user_menu_->select(settings);
		// nav_menu_->select(0);
		
	}else {
		root()->clear();
		createApp();
		auto login_btn = navbar_->bindWidget("user-menu", std::make_unique<Wt::WPushButton>("Log In"));
		login_btn->setStyleClass("btn btn-primary");
		services_page_->removeSidebar();
		login_btn->clicked().connect(this, &EventManagerLab::createAuth);
	}

}

// create Application Page
void EventManagerLab::createApp() 
{
	// Create navigation
	navbar_ = root()->addWidget(std::make_unique<Wt::WTemplate>(Wt::WString::tr("navbar")));
	auto logo = navbar_->bindWidget("logo", std::make_unique<Wt::WImage>("https://tailwindui.com/img/logos/workflow-mark.svg?color=indigo&shade=500"));
	navbar_->setStyleClass("app-navbar");
	// create nan_menu_ and stack_
	stack_ = root()->addWidget(std::make_unique<Wt::WStackedWidget>());
	nav_menu_ = navbar_->bindWidget("menu", std::make_unique<Wt::WMenu>(stack_));
	stack_->setStyleClass("app-content");
	
	// add menu_ items
	auto services_page = std::make_unique<FindServicesPage>(login_);
	auto organizer_page = std::make_unique<OrganizerPage>(login_);
	services_page_ = services_page.get();
	organizer_page_ = organizer_page.get();
	auto portofolio_menu_item = nav_menu_->addItem("Find services", std::move(services_page));
	auto page_two_menu_item = nav_menu_->addItem("Organizer", std::move(organizer_page));


	// internal base paths
	portofolio_menu_item->setLink(Wt::WLink(Wt::LinkType::InternalPath, "/portofolios"));

	// menu_ Item styles
	std::string list_item_styles = "bg-body-hover-border w-4/5 rounded-md sm:rounded-none h-6 h-full flex items-center justify-center";
	portofolio_menu_item->setStyleClass(list_item_styles);
	page_two_menu_item->setStyleClass(list_item_styles);
	std::string menuItemStyles = "sm:!rounded-none p-3 text-inherit !m-0 font-medium";
	portofolio_menu_item->anchor()->setStyleClass(menuItemStyles); 
	page_two_menu_item->anchor()->setStyleClass(menuItemStyles);


	portofolio_menu_item->clicked().connect(this, [=](){ menuItemSelected(portofolio_menu_item); });
	page_two_menu_item->clicked().connect(this, [=](){ menuItemSelected(page_two_menu_item); });
	
	// Hamburger Button to toggle menu_ opn mobile
	auto hamburger_btn = navbar_->bindWidget("hamburger-button", std::make_unique<Wt::WPushButton>(Wt::WString::tr("hamburger-svg")));
	hamburger_btn->setTextFormat(Wt::TextFormat::XHTML);
	hamburger_btn->clicked().connect(this, [=](){
		if(nav_menu_->hasStyleClass("hidden")){
			nav_menu_->removeStyleClass("hidden");
			hamburger_btn->setText(Wt::WString::tr("x-svg"));
		} else {
			nav_menu_->addStyleClass("hidden");
			hamburger_btn->setText(Wt::WString::tr("hamburger-svg"));
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

