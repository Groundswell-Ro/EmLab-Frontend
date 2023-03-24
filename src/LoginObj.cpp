#include "include/LoginObj.h"
#include <Wt/WApplication.h>
#include <fstream>

#include <Ice/Ice.h>
#include <stdexcept>

Login::Login()
{
	getConnectionStrings();
	user_.loginResponse = AuthModule::LoginResponse::NotIdentified;
	user_.userName = "";
	user_.userToken = "";
}

void Login::login(AuthModule::StructLoginReturn structLoginReturn)
{
	std::cout << "\n Login obj here to tell you that the user LOGGED IN succesfully :D \n";

	user_.loginResponse = structLoginReturn.loginResponse;
	user_.userName = structLoginReturn.userName;
	user_.userToken = structLoginReturn.userToken;
	getUserServices();
	changed_.emit();
}

void Login::logout()
{
	std::cout << "\n Login obj here to tell you that the user LOGGED OUT succesfully :D \n";
	user_.loginResponse = AuthModule::LoginResponse::NotIdentified;
	user_.userName = "";
	user_.userToken = "";
	userServices_.clear();
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
	std::ifstream authCommFile("comm-auth-dev.txt");
	std::ifstream eventCommFile("comm-event-dev.txt");

	if (!eventCommFile || !authCommFile)
	{
		authCommFile.open("comm-auth.txt");
		eventCommFile.open("comm-event.txt");
		if (!eventCommFile || !authCommFile)
		{
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
		userServices_ = authDataInterface->getSelfServices(user_.userToken);
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << std::endl;
	}
}
// void Login::getEventsData()
// {
// 	// Ice communication for loging in a user starts here
// 	try
// 	{
// 		Ice::CommunicatorHolder ich = Ice::initialize();
// 		auto base = ich->stringToProxy(eventConnString_);
// 		auto eventsDataInterface = Ice::checkedCast<EventDataModule::EventsDataInterfacePrx>(base);
// 		if (!eventsDataInterface)
// 		{
// 			throw std::runtime_error("Invalid proxy");
// 		}
// 		// Try to register Data
// 		seqEventDataPack_ = eventsDataInterface->getEventsData(user_.userToken);
// 	}
// 	catch (const std::exception &e)
// 	{
// 		std::cerr << e.what() << std::endl;
// 	}
// }

// void Login::getClientsData()
// {
// 	// Ice communication for loging in a user starts here
// 	try
// 	{
// 		Ice::CommunicatorHolder ich = Ice::initialize();
// 		auto base = ich->stringToProxy(eventConnString_);
// 		auto eventsDataInterface = Ice::checkedCast<EventDataModule::EventsDataInterfacePrx>(base);
// 		if (!eventsDataInterface)
// 		{
// 			throw std::runtime_error("Invalid proxy");
// 		}
// 		// Try to register Data
// 		seqClients_ = eventsDataInterface->getClientsData(user_.userToken);
// 	}
// 	catch (const std::exception &e)
// 	{
// 		std::cerr << e.what() << std::endl;
// 	}
// }

// EventDataModule::EventDataPack Login::registerEvent(EventDataModule::EventDataPack eventDataPack)
// {
// 	EventDataModule::EventDataPack new_eventDataPack;
// 	new_eventDataPack.eventData.id = 0;
// 	std::cout << "\n\n register event from login obj got called \n\n";
// 	// Ice communication for loging in a user starts here
// 	try
// 	{
// 		Ice::CommunicatorHolder ich = Ice::initialize();
// 		auto base = ich->stringToProxy(eventConnString_);
// 		auto eventsDataInterface = Ice::checkedCast<EventDataModule::EventsDataInterfacePrx>(base);
// 		if (!eventsDataInterface)
// 		{
// 			throw std::runtime_error("Invalid proxy");
// 		}
// 		// Try to register Data
// 		new_eventDataPack = eventsDataInterface->registerEvent(user_.userToken, eventDataPack);
// 		seqEventDataPack_.push_back(new_eventDataPack);
// 		return new_eventDataPack;
// 	}
// 	catch (const std::exception &e)
// 	{
// 		std::cerr << e.what() << std::endl;
// 	}
// 	return new_eventDataPack;
// }

// EventDataModule::ClientData Login::registerClient(EventDataModule::ClientData clientData)
// {
// 	EventDataModule::ClientData new_clientData;
// 	new_clientData.id = 0;

// 	std::cout << "\n\n register client from login obj got called \n\n";
// 	// Ice communication for loging in a user starts here
// 	try
// 	{
// 		Ice::CommunicatorHolder ich = Ice::initialize();
// 		auto base = ich->stringToProxy(eventConnString_);
// 		auto eventsDataInterface = Ice::checkedCast<EventDataModule::EventsDataInterfacePrx>(base);
// 		if (!eventsDataInterface)
// 		{
// 			throw std::runtime_error("Invalid proxy");
// 		}
// 		// Try to register Data
// 		new_clientData = eventsDataInterface->registerClient(user_.userToken, clientData);
// 		seqClients_.push_back(new_clientData);
// 		return new_clientData;
// 	}
// 	catch (const std::exception &e)
// 	{
// 		std::cerr << e.what() << std::endl;
// 	}
// 	return new_clientData;
// }
