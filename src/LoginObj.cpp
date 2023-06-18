#include "include/LoginObj.h"
#include <Wt/WApplication.h>
#include <fstream>

#include <Ice/Ice.h>
#include <stdexcept>



Login::Login()
{
	getConnectionStrings();
	user_.loginResponse = LoginResponse::NotIdentified;
	user_.name = "";
	user_.token = "";
}

void Login::login(LoginReturn loginReturn)
{
	std::cout << "\n Login obj here to tell you that the user LOGGED IN succesfully :D \n";

	user_.loginResponse = loginReturn.loginResponse;
	user_.name = loginReturn.name;
	user_.token = loginReturn.token;
	changed_.emit();
}

void Login::logout()
{
	std::cout << "\n Login obj here to tell you that the user LOGGED OUT succesfully :D \n";
	user_.loginResponse = LoginResponse::NotIdentified;
	user_.name = "";
	user_.token = "";
	changed_.emit();
}

bool Login::isLoggedIn()
{
	auto status = user_.loginResponse;
	if (status == LoginResponse::LoggedIn)
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

// // Ice communication for getting clients from server
// SeqClientInfo Login::getClientsByName(std::string name)
// {
// 	SeqClientInfo seqClientInfo;
//     try
//     {
//         Ice::CommunicatorHolder ich = Ice::initialize();
//         auto base = ich->stringToProxy(eventConnString_);
//         auto eventsDataInterface = Ice::checkedCast<EventInterfacePrx>(base);
//         if (!eventsDataInterface)
//         {
//             throw std::runtime_error("Invalid proxy");
//         }
//         // get clients from server by name
//         seqClientInfo = eventsDataInterface->getClientsByName(userToken(), name);
//     }
//     catch (const std::exception &e)
//     {
//         std::cerr << e.what() << std::endl;
//     }
// 	return seqClientInfo;
// }

// SeqClientInfo Login::getClientsByPhone(std::string phone)
// {
// 	SeqClientInfo seqClientInfo;
// 	try
// 	{
// 		Ice::CommunicatorHolder ich = Ice::initialize();
// 		auto base = ich->stringToProxy(eventConnString_);
// 		auto eventsDataInterface = Ice::checkedCast<EventInterfacePrx>(base);
// 		if (!eventsDataInterface)
// 		{
// 			throw std::runtime_error("Invalid proxy");
// 		}
// 		// get clients from server by phone
// 		seqClientInfo = eventsDataInterface->getClientsByPhone(userToken(), phone);
// 	}
// 	catch (const std::exception &e)
// 	{
// 		std::cerr << e.what() << std::endl;
// 	}
// 	return seqClientInfo;
// }

// int Login::registerClient(ClientInfo clientInfo)
// {
// 	int clientId = 0;
// 	try
// 	{
// 		Ice::CommunicatorHolder ich = Ice::initialize();
// 		auto base = ich->stringToProxy(eventConnString_);
// 		auto eventsDataInterface = Ice::checkedCast<EventInterfacePrx>(base);
// 		if (!eventsDataInterface)
// 		{
// 			throw std::runtime_error("Invalid proxy");
// 		}
// 		// add client to server
// 		clientId = eventsDataInterface->registerClient(userToken(), clientInfo);
// 	}
// 	catch (const std::exception &e)
// 	{
// 		std::cerr << e.what() << std::endl;
// 	}
// 	return clientId;
// }

EventData Login::registerEvent(EventData eventData)
{
	try
	{
		Ice::CommunicatorHolder ich = Ice::initialize();
		auto base = ich->stringToProxy(eventConnString_);
		auto eventsDataInterface = Ice::checkedCast<EventInterfacePrx>(base);
		if (!eventsDataInterface)
		{
			throw std::runtime_error("Invalid proxy");
		}
		// get events from server by client
		eventData = eventsDataInterface->addEvent(userToken(), eventData);
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << std::endl;
	}
	return eventData;
}

SeqEventData Login::getEventsData()
{
	SeqEventData events_data;
	// Ice communication for getting events data
	try
	{
		Ice::CommunicatorHolder ich = Ice::initialize();
		auto base = ich->stringToProxy(eventConnString_);
		auto eventsDataInterface = Ice::checkedCast<EventInterfacePrx>(base);
		if (!eventsDataInterface)
		{
			throw std::runtime_error("Invalid proxy");
		}
		// get events data
		 events_data = eventsDataInterface->getEventsData(userToken());
	}
	catch (const std::exception &e)
	{
		std::cerr << "\n\n"
				  << e.what() << "\n\n";
	}
	return events_data;
}

void Login::delEvent(int eventId)
{
	try
	{
		Ice::CommunicatorHolder ich = Ice::initialize();
		auto base = ich->stringToProxy(eventConnString_);
		auto eventsDataInterface = Ice::checkedCast<EventInterfacePrx>(base);
		if (!eventsDataInterface)
		{
			throw std::runtime_error("Invalid proxy");
		}
		// delete event
		eventsDataInterface->delEvent(userToken(), eventId);
		// refresh table
	}
	catch (const std::exception &e)
	{
		std::cerr << "\n\n"
					<< e.what() << "\n\n";
	}
}

LoginReturn Login::loginUser(LoginInfo authInfo)
{
	LoginReturn loginReturn;
	try
	{
		Ice::CommunicatorHolder ich = Ice::initialize();
		auto base = ich->stringToProxy(authConnString_);
		auto authInterface = Ice::checkedCast<AuthInterfacePrx>(base);
		if (!authInterface)
		{
			throw std::runtime_error("Invalid proxy");
		}
		// login user
		loginReturn = authInterface->loginUser(authInfo);
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << std::endl;
	}
	return loginReturn;
}

RegistrationResponse Login::registerUser(RegistrationInfo regInfo)
{
	RegistrationResponse registrationResponse;
	try
	{
		Ice::CommunicatorHolder ich = Ice::initialize();
		auto base = ich->stringToProxy(authConnString_);
		auto authInterface = Ice::checkedCast<AuthInterfacePrx>(base);
		if (!authInterface)
		{
			throw std::runtime_error("Invalid proxy");
		}
		// register user
		registrationResponse = authInterface->registerUser(regInfo);
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << std::endl;
	}
	return registrationResponse;
}

 
