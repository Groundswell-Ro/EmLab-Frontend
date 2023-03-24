#include "include/App.h"
#include "include/ProfilePage.h"
// #include "include/test.h"
#include "include/Events.h"
#include <Wt/WTemplate.h>
#include <Wt/WRadioButton.h>
#include <Wt/WPushButton.h>
#include <Wt/WAnimation.h>
#include <iostream>
#include <Wt/WNavigationBar.h>

// Application is a containerWidget
EventManagerLab::EventManagerLab()
	: WContainerWidget(),
	  login_(std::make_shared<Login>())
{
	// Client data for testing

	addStyleClass("application");

	authWidget_ = addWidget(std::make_unique<AuthWidget>(login_));

	login_->changed().connect(this, &EventManagerLab::handleUserAuth);
	login_->changed().emit();
}

void EventManagerLab::handleUserAuth()
{
	if (!login_->isLoggedIn())
	{
		authWidget_->animateShow(Wt::WAnimation(Wt::AnimationEffect::SlideInFromTop));
	}
	else
	{
		authWidget_->animateHide(Wt::WAnimation(Wt::AnimationEffect::SlideInFromTop));

		if (!login_->loggedInOnce_)
		{
			login_->loggedInOnce_ = true;
			createApp();
		}
	}
}

void EventManagerLab::createApp()
{

	navigation_ = addWidget(std::make_unique<Wt::WNavigationBar>());
	navigation_->setObjectName("navigation-widget");

	// navigation->setResponsive(true);
	navigation_->addStyleClass("navbar-dark bg-dark navbar-expand-sm");
	navigation_->setTitle("Event Manager Lab");

	contentsStack_ = addWidget(std::make_unique<Wt::WStackedWidget>());
	contentsStack_->addStyleClass("content");
	navigation_->setObjectName("content-widget");

	auto leftMenu_ = navigation_->addMenu(std::make_unique<Wt::WMenu>(contentsStack_));
	leftMenu_->addStyleClass("me-auto");

	// event menu content here
	auto eventsContentItem = std::make_unique<Events>(login_);
	auto eventsContentItem_ptr = eventsContentItem.get();

	// test menu content here
	// auto testContentItem = std::make_unique<Test>(login_);
	// auto testContentItem_ptr = testContentItem.get();

	auto profileContentsItem = std::make_unique<ProfilePage>(login_);

	leftMenu_->addItem("Events", std::move(eventsContentItem))->setLink(Wt::WLink(Wt::LinkType::InternalPath, "/events"));
	leftMenu_->addItem("Profile", std::move(profileContentsItem))->setLink(Wt::WLink(Wt::LinkType::InternalPath, "/profile"));

	auto logoutBtn = navigation_->addWidget(std::make_unique<Wt::WTemplate>(tr("logout-btn")));
	logoutBtn->clicked().connect(this, [=]()
								 { login_->logout(); });
}
