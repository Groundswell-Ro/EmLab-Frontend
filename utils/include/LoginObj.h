#pragma once
#include "Utils.h"
#include <Wt/WSignal.h>
#include <Wt/WString.h>
class Login
{
public:
	Login();
	~Login();

    std::string getUserPhotoPath();

	void login(Emlab::LoginReturn structLoginReturn);
	void logout();
	bool isLoggedIn();

	Emlab::LoginReturn &user() { return user_; };
	Emlab::ProviderProfileInfo &provider() { return provider_; };
	Wt::Signal<> &changed() { return changed_; };
	std::string userToken() { return user_.token; };
	bool loggedInOnce_ = false;


    Wt::WString connection = ":default -p 10000";
	std::string AUTH_CONN_STRING = Emlab::AUTHADAPTER + connection.toUTF8();
	std::string EVENT_CONN_STRING = Emlab::EVENTADAPTER + connection.toUTF8();
	std::string PROVIDER_CONN_STRING = Emlab::PROVIDERADAPTER + connection.toUTF8();

	void setUserEmail(std::string value);
	void setUserName(std::string value);
	void setUserPhone(std::string value);
	void setDarkMode(bool darkMode);

	std::string getUserRole();
	std::string getUserToken();
	std::string getUserIdentity();

private:
	Emlab::LoginReturn user_;
	Emlab::ProviderProfileInfo provider_;

	Wt::Signal<> changed_;
    std::string temp_images_path_ = "resources/temp_images/";

	void setUserProvider();

};