#pragma once
#include <comm/AuthInterface.h>
#include <comm/EventInterface.h>
#include <utils/Utils.h>
#include <Wt/WSignal.h>

class Login
{
public:
	Login();

	void login(Emlab::LoginReturn structLoginReturn);
	void logout();
	bool isLoggedIn();

	std::string authConnString_;
	std::string eventConnString_;
	std::string serviceConnString_;
	std::string clientConnString_;
	std::string ReviewConnString_;

	Emlab::LoginReturn &user() { return user_; };
	Wt::Signal<> &changed() { return changed_; };
	std::string userToken() { return user_.token; };
	bool loggedInOnce_ = false;
	
    // SeqClientInfo getClientsByName(std::string name);
	// SeqClientInfo getClientsByPhone(std::string phone);
	// int registerClient(ClientInfo client);
	Emlab::EventInfo addEventInfo(Emlab::EventInfo eventInfo);
	Emlab::EventData addEventData(Emlab::EventData eventData);
	Emlab::SeqEventData getEventsData();
	void delEvent(int eventId);
	
	// Login/Registration
	Emlab::LoginReturn loginUser(Emlab::LoginInfo authInfo);
	Emlab::RegistrationResponse registerUser(Emlab::RegistrationInfo regInfo);

private:
	Emlab::LoginReturn user_;
	Wt::Signal<> changed_;
	void getConnectionStrings();
};