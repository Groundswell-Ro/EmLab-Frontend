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

void Login::login(AuthModule::LoginReturn loginReturn)
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


// Ice communication for getting clients from server
EventModule::SeqClientInfo Login::getClientsByName(std::string name)
{
	EventModule::SeqClientInfo seqClientInfo;
    try
    {
        Ice::CommunicatorHolder ich = Ice::initialize();
        auto base = ich->stringToProxy(eventConnString_);
        auto eventsDataInterface = Ice::checkedCast<EventModule::EventInterfacePrx>(base);
        if (!eventsDataInterface)
        {
            throw std::runtime_error("Invalid proxy");
        }
        // get clients from server by name
        seqClientInfo = eventsDataInterface->getClientsByName(userToken(), name);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
	return seqClientInfo;
}

EventModule::SeqClientInfo Login::getClientsByPhone(std::string phone)
{
	EventModule::SeqClientInfo seqClientInfo;
	try
	{
		Ice::CommunicatorHolder ich = Ice::initialize();
		auto base = ich->stringToProxy(eventConnString_);
		auto eventsDataInterface = Ice::checkedCast<EventModule::EventInterfacePrx>(base);
		if (!eventsDataInterface)
		{
			throw std::runtime_error("Invalid proxy");
		}
		// get clients from server by phone
		seqClientInfo = eventsDataInterface->getClientsByPhone(userToken(), phone);
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << std::endl;
	}
	return seqClientInfo;
}

int Login::registerClient(EventModule::ClientInfo clientInfo)
{
	int clientId = 0;
	try
	{
		Ice::CommunicatorHolder ich = Ice::initialize();
		auto base = ich->stringToProxy(eventConnString_);
		auto eventsDataInterface = Ice::checkedCast<EventModule::EventInterfacePrx>(base);
		if (!eventsDataInterface)
		{
			throw std::runtime_error("Invalid proxy");
		}
		// add client to server
		clientId = eventsDataInterface->registerClient(userToken(), clientInfo);
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << std::endl;
	}
	return clientId;
}

EventModule::EventData Login::registerEvent(EventModule::EventData eventData)
{
	try
	{
		Ice::CommunicatorHolder ich = Ice::initialize();
		auto base = ich->stringToProxy(eventConnString_);
		auto eventsDataInterface = Ice::checkedCast<EventModule::EventInterfacePrx>(base);
		if (!eventsDataInterface)
		{
			throw std::runtime_error("Invalid proxy");
		}
		// get events from server by client
		eventData = eventsDataInterface->registerEvent(userToken(), eventData);
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << std::endl;
	}
	return eventData;
}

EventModule::SeqEventData Login::getEventsData()
{
	EventModule::SeqEventData events_data;
	// Ice communication for getting events data
	try
	{
		Ice::CommunicatorHolder ich = Ice::initialize();
		auto base = ich->stringToProxy(eventConnString_);
		auto eventsDataInterface = Ice::checkedCast<EventModule::EventInterfacePrx>(base);
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

void Login::deleteRecord(EventModule::Table table, int id)
{
	try
	{
		Ice::CommunicatorHolder ich = Ice::initialize();
		auto base = ich->stringToProxy(eventConnString_);
		auto eventsDataInterface = Ice::checkedCast<EventModule::EventInterfacePrx>(base);
		if (!eventsDataInterface)
		{
			throw std::runtime_error("Invalid proxy");
		}
		// delete event
		eventsDataInterface->deleteRecord(userToken(),EventModule::Table::events, id);
		// refresh table
	}
	catch (const std::exception &e)
	{
		std::cerr << "\n\n"
					<< e.what() << "\n\n";
	}
}

AuthModule::LoginReturn Login::loginUser(AuthModule::LoginInfo authInfo)
{
	AuthModule::LoginReturn loginReturn;
	try
	{
		Ice::CommunicatorHolder ich = Ice::initialize();
		auto base = ich->stringToProxy(authConnString_);
		auto authInterface = Ice::checkedCast<AuthModule::AuthInterfacePrx>(base);
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

AuthModule::RegistrationResponse Login::registerUser(AuthModule::RegistrationInfo regInfo)
{
	AuthModule::RegistrationResponse registrationResponse;
	try
	{
		Ice::CommunicatorHolder ich = Ice::initialize();
		auto base = ich->stringToProxy(authConnString_);
		auto authInterface = Ice::checkedCast<AuthModule::AuthInterfacePrx>(base);
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

 
Ice::ByteSeq Login::imageToBytes(const std::string& imagePath)
{
std::ifstream imageFile(imagePath, std::ios::binary);
Ice::ByteSeq imageData(
    (std::istreambuf_iterator<char>(imageFile)),
    (std::istreambuf_iterator<char>()));
return imageData;
}

bool Login::bytesToImage(const Ice::ByteSeq& bytes, const std::string& imagePath)
{
    std::ofstream imageFile(imagePath, std::ios::binary);
    imageFile.write(reinterpret_cast<const char*>(bytes.data()), bytes.size());

    return !imageFile.fail();
}