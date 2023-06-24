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
	user_.token = loginReturn.token;
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


