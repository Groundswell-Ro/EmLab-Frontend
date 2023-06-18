#pragma once
#include "../../../comunication/generated/AuthInterface.h"
#include "../../../comunication/generated/EventInterface.h"
#include "../../../comunication/Utils.h"
#include <Wt/WSignal.h>

using namespace Emlab;

class Login
{
public:
	Login();

	void login(LoginReturn structLoginReturn);
	void logout();
	bool isLoggedIn();

	std::string eventConnString_;
	std::string authConnString_;

	LoginReturn &user() { return user_; };
	Wt::Signal<> &changed() { return changed_; };
	std::string userToken() { return user_.token; };
	bool loggedInOnce_ = false;
	
    // SeqClientInfo getClientsByName(std::string name);
	// SeqClientInfo getClientsByPhone(std::string phone);
	// int registerClient(ClientInfo client);
	EventData registerEvent(EventData eventData);
	SeqEventData getEventsData();
	void delEvent(int eventId);
	
	// Login/Registration
	LoginReturn loginUser(LoginInfo authInfo);
	RegistrationResponse registerUser(RegistrationInfo regInfo);

private:



	LoginReturn user_;
	Wt::Signal<> changed_;
	void getConnectionStrings();



};