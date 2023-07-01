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
	Wt::Signal<> &changed() { return changed_; };
	std::string userToken() { return user_.token; };
	bool loggedInOnce_ = false;


    Wt::WString connection = ":default -p 10000";
	std::string AUTH_CONN_STRING = Emlab::AUTHADAPTER + connection.toUTF8();
	std::string EVENT_CONN_STRING = Emlab::EVENTADAPTER + connection.toUTF8();
	std::string SERVICE_CONN_STRING = Emlab::SERVICEADAPTER + connection.toUTF8();
	std::string REVIEW_CONN_STRING = Emlab::REVIEWADAPTER + connection.toUTF8();

	void setUserEmail(std::string value);
	void setUserName(std::string value);
	void setUserPhone(std::string value);
	void setDarkMode(bool darkMode);

private:
	Emlab::LoginReturn user_;
	Wt::Signal<> changed_;
    std::string temp_images_path_ = "resources/temp_images/";


};