#include "include/LoginObj.h"

#include <fstream>
#include <Wt/WApplication.h>
// #include <Wt/WString.h>

Login::Login()
{
	user_.loginResponse = Emlab::LoginResponse::NotIdentified;
	user_.userInfo.name = "";
	user_.token = "";
}

Login::~Login()
{
}

std::string Login::getUserPhotoPath()
{
	return  temp_images_path_ +  Wt::WApplication::instance()->sessionId() + "/";
}

void Login::login(Emlab::LoginReturn loginReturn)
{
	std::cout << "\n Login obj here to tell you that the user LOGGED IN succesfully :D \n";
	user_.loginResponse = loginReturn.loginResponse;
	user_.userInfo.email = loginReturn.userInfo.email;
	user_.userInfo.name = loginReturn.userInfo.name;
	user_.userInfo.phone = loginReturn.userInfo.phone;
	user_.userInfo.role = loginReturn.userInfo.role;
	user_.token = loginReturn.token;
	user_.userInfo.darkMode = loginReturn.userInfo.darkMode;
	Emlab::bytesToImage(Emlab::Comunication::getUserPhotoWithToken(loginReturn.token), getUserPhotoPath() + "profile.jpg");
	changed_.emit();
	std::cout << "\n\n\n role = " << user_.userInfo.role << "\n\n\n";
}

void Login::logout()
{
	std::cout << "\n Login obj here to tell you that the user LOGGED OUT succesfully :D \n";

	Emlab::deleteFolder(temp_images_path_, Wt::WApplication::instance()->sessionId());
	user_.loginResponse = Emlab::LoginResponse::NotIdentified;
	user_.userInfo.name = "";
	user_.token = "";
	user_.userInfo.email = "";
	changed_.emit();
}

bool Login::isLoggedIn()
{
	auto status = user_.loginResponse;
	if (status == Emlab::LoginResponse::LoggedIn)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void Login::setUserEmail(std::string value)
{
	user_.userInfo.email = value;
}

void Login::setUserName(std::string value)
{
	user_.userInfo.name = value;
}

void Login::setUserPhone(std::string value)
{
	user_.userInfo.phone = value;
}

void Login::setDarkMode(bool darkMode)
{
	user_.userInfo.darkMode = darkMode;
}