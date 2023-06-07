#include "include/LoginObj.h"
#include <Wt/WApplication.h>
#include <fstream>

#include <Ice/Ice.h>
#include <stdexcept>

Login::Login()
{
	getConnectionStrings();
	user_.loginResponse = AuthModule::LoginResponse::NotIdentified;
	user_.name = "";
	user_.token = "";
}

void Login::setUserLogin(bool login)
{
	if(login) {
		user_.loginResponse = AuthModule::LoginResponse::LoggedIn;
		changed_.emit();
	}else {
		user_.loginResponse = AuthModule::LoginResponse::NotIdentified;
		changed_.emit();
	}
}


void Login::login(AuthModule::LoginReturn loginReturn)
{
	std::cout << "\n Login obj here to tell you that the user LOGGED IN succesfully :D \n";

	user_.loginResponse = loginReturn.loginResponse;
	user_.name = loginReturn.name;
	user_.token = loginReturn.token;
	user_.servicesInfoSq = loginReturn.servicesInfoSq;
	changed_.emit();
}

void Login::logout()
{
	std::cout << "\n Login obj here to tell you that the user LOGGED OUT succesfully :D \n";
	user_.loginResponse = AuthModule::LoginResponse::NotIdentified;
	user_.name = "";
	user_.token = "";
	changed_.emit();
}

bool Login::isLoggedIn()
{
	auto status = user_.loginResponse;
	if (status == AuthModule::LoginResponse::LoggedIn)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void Login::getConnectionStrings()
{
	//  Get connection strings from files
	std::ifstream authCommFile("/etc/evntmgr/comm-auth.txt");
	std::ifstream eventCommFile("/etc/evntmgr/comm-event.txt");

	if (!eventCommFile || !authCommFile)
	{
		authCommFile = std::ifstream("comm-auth.txt");
		eventCommFile = std::ifstream("comm-event.txt");
		if(!eventCommFile || !authCommFile){
			std::cout << "\n\n - ERROR - Give me the EVENT and AUTH COMUNICATION FILES and i will work :) - \n\n";
		return;
		}
	}

	getline(authCommFile, authConnString_);
	getline(eventCommFile, eventConnString_);
}

void Login::getUserServices()
{

	// Ice communication for getting userServices_ starts here
	try
	{
		Ice::CommunicatorHolder ich = Ice::initialize();
		auto base = ich->stringToProxy(authConnString_);
		auto authDataInterface = Ice::checkedCast<AuthModule::AuthInterfacePrx>(base);
		if (!authDataInterface)
		{
			throw std::runtime_error("Invalid proxy");
		}
		// get data in local variable
		user_.servicesInfoSq = authDataInterface->getSelfServices(user_.token);
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << std::endl;
	}
}
