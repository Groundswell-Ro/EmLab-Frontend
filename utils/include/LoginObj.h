#pragma once
#include "Utils.h"
#include <Wt/WSignal.h>

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
private:
	Emlab::LoginReturn user_;
	Wt::Signal<> changed_;
    std::string temp_images_path_ = "resources/temp_images/";

};