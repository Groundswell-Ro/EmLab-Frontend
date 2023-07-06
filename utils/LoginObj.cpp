#include "include/LoginObj.h"
#include "../../comunication/comm/ProviderInterface.h"

#include <fstream>
#include <Wt/WApplication.h>
// #include <Wt/WString.h>
#include <stdexcept>
#include <Ice/Ice.h>

Login::Login()
{
	user_.loginResponse = Emlab::LoginResponse::NotIdentified;
	user_.userInfo.name = "";
	user_.token = "";
}

Login::~Login()
{
}

std::string Login::getUserPhotoPath()
{
	return  temp_images_path_ +  Wt::WApplication::instance()->sessionId() + "/";
}

void Login::login(Emlab::LoginReturn loginReturn)
{
	std::cout << "\n Login obj here to tell you that the user LOGGED IN succesfully :D \n";
	user_.loginResponse = loginReturn.loginResponse;
	user_.userInfo.email = loginReturn.userInfo.email;
	user_.userInfo.name = loginReturn.userInfo.name;
	user_.userInfo.phone = loginReturn.userInfo.phone;
	user_.userInfo.role = loginReturn.userInfo.role;
	user_.token = loginReturn.token;
	user_.userInfo.darkMode = loginReturn.userInfo.darkMode;
	Emlab::bytesToImage(Emlab::Comunication::getUserPhotoWithToken(loginReturn.token), getUserPhotoPath() + "profile.jpg");

	if(user_.userInfo.role == Emlab::PROVIDERROLE){
		setUserProvider();
	
	}
	changed_.emit();
}

void Login::logout()
{
	std::cout << "\n Login obj here to tell you that the user LOGGED OUT succesfully :D \n";

	Emlab::deleteFolder(temp_images_path_, Wt::WApplication::instance()->sessionId());
	user_.loginResponse = Emlab::LoginResponse::NotIdentified;
	user_.userInfo.name = "";
	user_.token = "";
	user_.userInfo.email = "";
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
	user_.userInfo.email = value;
}

void Login::setUserName(std::string value)
{
	user_.userInfo.name = value;
}

void Login::setUserPhone(std::string value)
{
	user_.userInfo.phone = value;
}

void Login::setDarkMode(bool darkMode)
{
	user_.userInfo.darkMode = darkMode;
}

std::string Login::getUserRole()
{
	return user_.userInfo.role;
}

std::string Login::getUserToken()
{
	return user_.token;
}

std::string Login::getUserIdentity()
{
	return user_.userInfo.email;
}

void Login::setUserProvider()
{
	try {
        Ice::CommunicatorHolder ich = Ice::initialize();
        auto base = ich->stringToProxy(PROVIDER_CONN_STRING);
        auto providerInterface = Ice::checkedCast<Emlab::ProviderInterfacePrx>(base);
        if (!providerInterface)
        {
            throw std::runtime_error("Invalid proxy");
        }
        auto providerInfo = providerInterface->getProfileAsUser(user_.token);
		provider_.id = providerInfo.id;
		provider_.name = providerInfo.name;
		provider_.username = providerInfo.username;
		provider_.email = providerInfo.email;
		provider_.phone = providerInfo.phone;
		provider_.description = providerInfo.description;

		std::cout << "\n\n";
		std::cout << "provider_.id = " << provider_.id << "\n";
		std::cout << "provider_.name = " << provider_.name << "\n";
		std::cout << "provider_.username = " << provider_.username << "\n";
		std::cout << "provider_.email = " << provider_.email << "\n";
		std::cout << "provider_.phone = " << provider_.phone << "\n";
		std::cout << "provider_.description = " << provider_.description << "\n";
		std::cout << "\n\n";
		
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
	
}

