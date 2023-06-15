#pragma once
#include "../../../comunication/AuthModule.h"
#include "../../../comunication/EventModule.h"
#include <Wt/WSignal.h>

class Login
{
public:
	Login();

	void login(AuthModule::LoginReturn structLoginReturn);
	void logout();
	bool isLoggedIn();

	std::string eventConnString_;
	std::string authConnString_;

	AuthModule::LoginReturn &user() { return user_; };
	Wt::Signal<> &changed() { return changed_; };
	std::string userToken() { return user_.token; };
	bool loggedInOnce_ = false;
	
    EventModule::SeqClientInfo getClientsByName(std::string name);
	EventModule::SeqClientInfo getClientsByPhone(std::string phone);
	int registerClient(EventModule::ClientInfo client);
	EventModule::EventData registerEvent(EventModule::EventData eventData);
	EventModule::SeqEventData getEventsData();
	void deleteRecord(EventModule::Table table, int id);
	
	// Login/Registration
	AuthModule::LoginReturn loginUser(AuthModule::LoginInfo authInfo);
	AuthModule::RegistrationResponse registerUser(AuthModule::RegistrationInfo regInfo);

	// Image
    Ice::ByteSeq imageToBytes(const std::string& imagePath);
 	bool bytesToImage(const Ice::ByteSeq& bytes, const std::string& imagePath);
private:
	AuthModule::LoginReturn user_;
	Wt::Signal<> changed_;
	void getConnectionStrings();



};