#pragma once
#include "../../../comunication/AuthModule.h"
#include "../../../comunication/EventDataModule.h"
#include <Wt/WSignal.h>

class Login
{
public:
	Login();

	void login(AuthModule::LoginReturn structLoginReturn);
	void logout();
	bool isLoggedIn();

	std::string eventConnString_;
	std::string authConnString_;

	AuthModule::LoginReturn &user() { return user_; };
	Wt::Signal<> &changed() { return changed_; };
	std::string userToken() { return user_.token; };
	bool loggedInOnce_ = false;

	void getUserServices();

	void setUserLogin(bool login = true);
private:
	AuthModule::LoginReturn user_;
	Wt::Signal<> changed_;
	void getConnectionStrings();
	// delete on production
};