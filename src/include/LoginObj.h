#pragma once
#include "../../../comunication/AuthModule.h"
#include "../../../comunication/EventDataModule.h"
#include <Wt/WSignal.h>

class Login
{
public:
	Login();

	void login(AuthModule::StructLoginReturn structLoginReturn);
	void logout();
	bool isLoggedIn();

	std::string eventConnString_;
	std::string authConnString_;

	AuthModule::StructLoginReturn &user() { return user_; };
	Wt::Signal<> &changed() { return changed_; };
	std::string userToken() { return user_.userToken; };
	bool loggedInOnce_ = false;

	AuthModule::ServiceInfoSq userServices_;
	void getUserServices();

private:
	AuthModule::StructLoginReturn user_;
	Wt::Signal<> changed_;
	void getConnectionStrings();
};