#include "include/LoginObj.h"

#include <fstream>
#include <Wt/WApplication.h>
// #include <Wt/WString.h>

Login::Login()
{
	user_.loginResponse = Emlab::LoginResponse::NotIdentified;
	user_.name = "";
	user_.token = "";
}

Login::~Login()
{
}

std::string Login::getUserPhotoPath()
{
	return  temp_images_path_ + user_.email;;
}

void Login::login(Emlab::LoginReturn loginReturn)
{
	std::cout << "\n Login obj here to tell you that the user LOGGED IN succesfully :D \n";
	user_.loginResponse = loginReturn.loginResponse;
	user_.email = loginReturn.email;
	user_.name = loginReturn.name;
	user_.phone = loginReturn.phone;
	user_.token = loginReturn.token;
	user_.darkMode = loginReturn.darkMode;
	std::cout << "\n\n email: " << user_.email << "\n\n";
	std::cout << "\n\n name: " << user_.name << "\n\n";
	std::cout << "\n\n phone: " << user_.phone << "\n\n";
	std::cout << "\n\n token: " << user_.token << "\n\n";
	Emlab::bytesToImage(Emlab::Comunication::getUserPhotoWithToken(loginReturn.token), getUserPhotoPath() + "/profile.jpg");
	changed_.emit();
}

void Login::logout()
{
	std::cout << "\n Login obj here to tell you that the user LOGGED OUT succesfully :D \n";

	Emlab::deleteFolder(temp_images_path_, user_.email);

	user_.loginResponse = Emlab::LoginResponse::NotIdentified;
	user_.name = "";
	user_.token = "";
	user_.email = "";
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
	user_.email = value;
}

void Login::setUserName(std::string value)
{
	user_.name = value;
}

void Login::setUserPhone(std::string value)
{
	user_.phone = value;
}
