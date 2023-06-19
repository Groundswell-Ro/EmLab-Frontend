#include "include/LoginObj.h"
#include <Wt/WApplication.h>
#include <fstream>

#include <Ice/Ice.h>
#include <stdexcept>

Login::Login()
{
	getConnectionStrings();
	user_.loginResponse = Emlab::LoginResponse::NotIdentified;
	user_.name = "";
	user_.token = "";
}

void Login::login(Emlab::LoginReturn loginReturn)
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
	user_.loginResponse = Emlab::LoginResponse::NotIdentified;
	user_.name = "";
	user_.token = "";
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

void Login::getConnectionStrings()
{
	//  Get connection strings from files
	// std::ifstream authCommFile("/etc/evntmgr/comm-auth.txt");
	// std::ifstream eventCommFile("/etc/evntmgr/comm-event.txt");

	// if (!eventCommFile || !authCommFile)
	// {
	// 	authCommFile = std::ifstream("comm-auth.txt");
	// 	eventCommFile = std::ifstream("comm-event.txt");
	// 	if(!eventCommFile || !authCommFile){
	// 		std::cout << "\n\n - ERROR - Give me the EVENT and AUTH COMUNICATION FILES and i will work :) - \n\n";
	// 	return;
	// 	}
	// }

	// getline(authCommFile, authConnString_);
	// getline(eventCommFile, eventConnString_);
	Wt::WString connection = ":default -p 10000";
	authConnString_ = Emlab::AUTHADAPTER + connection.toUTF8();
	eventConnString_ = Emlab::EVENTADAPTER + connection.toUTF8();
	serviceConnString_ = Emlab::SERVICEADAPTER + connection.toUTF8();
	clientConnString_ = Emlab::CLIENTADAPTER + connection.toUTF8();
	ReviewConnString_ = Emlab::REVIEWADAPTER + connection.toUTF8();

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

Emlab::EventData Login::addEventData(Emlab::EventData eventData)
{
	try
	{
		Ice::CommunicatorHolder ich = Ice::initialize();
		auto base = ich->stringToProxy(eventConnString_);
		auto eventsDataInterface = Ice::checkedCast<Emlab::EventInterfacePrx>(base);
		if (!eventsDataInterface)
		{
			throw std::runtime_error("Invalid proxy");
		}
		eventData.eventInfo.id =  eventsDataInterface->addEventData(userToken(), eventData);
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << std::endl;
	}
	return eventData;
}

Emlab::EventInfo Login::addEventInfo(Emlab::EventInfo eventInfo)
{
	try
	{
		Ice::CommunicatorHolder ich = Ice::initialize();
		auto base = ich->stringToProxy(eventConnString_);
		auto eventsDataInterface = Ice::checkedCast<Emlab::EventInterfacePrx>(base);
		if (!eventsDataInterface)
		{
			throw std::runtime_error("Invalid proxy");
		}
		eventInfo.id = eventsDataInterface->addEventInfo(userToken(), eventInfo);
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << std::endl;
	}
	return eventInfo;
}

Emlab::SeqEventData Login::getEventsData()
{
	Emlab::SeqEventData events_data;
	// Ice communication for getting events data
	try
	{
		Ice::CommunicatorHolder ich = Ice::initialize();
		auto base = ich->stringToProxy(eventConnString_);
		auto eventsDataInterface = Ice::checkedCast<Emlab::EventInterfacePrx>(base);
		if (!eventsDataInterface)
		{
			throw std::runtime_error("Invalid proxy");
		}
		// get events data
		events_data = eventsDataInterface->getSeqEventData(userToken());
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
		auto eventsDataInterface = Ice::checkedCast<Emlab::EventInterfacePrx>(base);
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

Emlab::LoginReturn Login::loginUser(Emlab::LoginInfo authInfo)
{
	Emlab::LoginReturn loginReturn;
	try
	{
		Ice::CommunicatorHolder ich = Ice::initialize();
		auto base = ich->stringToProxy(authConnString_);
		auto authInterface = Ice::checkedCast<Emlab::AuthInterfacePrx>(base);
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

Emlab::RegistrationResponse Login::registerUser(Emlab::RegistrationInfo regInfo)
{
	Emlab::RegistrationResponse registrationResponse;
	try
	{
		Ice::CommunicatorHolder ich = Ice::initialize();
		auto base = ich->stringToProxy(authConnString_);
		auto authInterface = Ice::checkedCast<Emlab::AuthInterfacePrx>(base);
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

 
