#include "include/Utils.h"

#include "../../comunication/comm/AuthInterface.h"
#include "../../comunication/comm/EventInterface.h"

#include <vector>
#include <Ice/Ice.h>
#include <iostream>
#include <cstdio>
#include <cstring>
#include <dirent.h>
#include <unistd.h>
#include <stdexcept>
#include <string>
#include <fstream>
#include <boost/filesystem.hpp>
#include <Wt/WString.h>

namespace Emlab
{
    
    ImageData imageToBytes(const std::string& imagePath)
    {
    std::ifstream imageFile(imagePath, std::ios::binary);
    ImageData imageData(
        (std::istreambuf_iterator<char>(imageFile)),
        (std::istreambuf_iterator<char>()));
    return imageData;
    }

 	bool bytesToImage(const ImageData& bytes, const std::string& imagePath)
    {
    // std::ofstream imageFile(imagePath, std::ios::binary);
    // imageFile.write(reinterpret_cast<const char*>(bytes.data()), bytes.size());

    // return imageFile.fail();

    boost::filesystem::path path(imagePath);
    boost::filesystem::create_directories(path.parent_path());

    std::ofstream imageFile(imagePath, std::ios::binary);
    imageFile.write(reinterpret_cast<const char*>(bytes.data()), bytes.size());

    return !imageFile.fail();
    }

    bool validateImage(const std::string& imagePath) {
        std::ifstream inFile(imagePath, std::ios::binary | std::ios::ate);

        if (!inFile.is_open()) {
        // Unable to open the file
        return false;
        }

        auto fileSize = inFile.tellg();
        inFile.seekg(0, std::ios::beg);

        char header[12];
        inFile.read(header, sizeof(header));

        // Check if the data starts with the JPEG header
        if (fileSize > 2 && (unsigned char)header[0] == 0xff && (unsigned char)header[1] == 0xd8) {
        return true;
        }

        // // Check if the data starts with the BMP header
        // if (fileSize > 1 && header[0] == 0x42 && header[1] == 0x4d) {
        //     // The file is a BMP file
        //     return true;
        // }

        // // Check if the data starts with the PNG header
        // if (fileSize > 7 && header[0] == 0x89 && header[1] == 0x50 && header[2] == 0x4e && header[3] == 0x47 && header[4] == 0x0d && header[5] == 0x0a && header[6] == 0x1a && header[7] == 0x0a) {
        //     // The file is a PNG file
        //     return true;
        // }

        // // Check if the data starts with the GIF header
        // if (fileSize > 2 && header[0] == 0x47 && header[1] == 0x49 && header[2] == 0x46) {
        //     // The file is a GIF file
        //     return true;
        // }

        // // Check if the data starts with the TIFF header
        // if (fileSize > 1 && (header[0] == 0x49 && header[1] == 0x49) || (header[0] == 0x4d && header[1] == 0x4d)) {
        //     // The file is a TIFF file
        //     return true;
        // }

        // If none of the above checks passed, assume the file is not a valid image
        return false;
    }

    bool deleteFile(const std::string& filePath) {
        std::cout << "\n\n path of file to be deleted : " << filePath;
        const char* filename = filePath.c_str();
        int result = std::remove(filename);
        if (result != 0) {
            std::perror("\n\n Error deleting file\n\n ");
            return false;
        } else {
            std::puts("\n\n File successfully deleted \n\n");
            return true;
        }
    }

    bool deleteFolder(const std::string& basePath, const std::string& folderName) {
        std::string folderPath = basePath + "/" + folderName;
        std::cout << "\n\n path of folder to be deleted : " << folderPath;

        DIR* dir = opendir(folderPath.c_str());
        if (dir == nullptr) {
            std::perror("\n\n Error opening folder\n\n ");
            return false;
        }

        dirent* entry;
        while ((entry = readdir(dir)) != nullptr) {
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
                continue;
            }
            std::string path = folderPath + "/" + entry->d_name;
            if (entry->d_type == DT_DIR) {
                deleteFolder(folderPath, entry->d_name);
            } else {
                const char* filename = path.c_str();
                int result = std::remove(filename);
                if (result != 0) {
                    std::perror("\n\n Error deleting folder\n\n ");
                    return false;
                }
            }
        }

        closedir(dir);
        int result = rmdir(folderPath.c_str());
        if (result != 0) {
            std::perror("\n\n Error deleting folder\n\n ");
            return false;
        } else {
            std::puts("\n\n Folder successfully deleted \n\n");
            return true;
        }
    }

    namespace Comunication {

    Wt::WString connection = ":default -p 10000";
	std::string AUTH_CONN_STRING = Emlab::AUTHADAPTER + connection.toUTF8();
	std::string EVENT_CONN_STRING = Emlab::EVENTADAPTER + connection.toUTF8();
	std::string SERVICE_CONN_STRING = Emlab::SERVICEADAPTER + connection.toUTF8();
	std::string REVIEW_CONN_STRING = Emlab::REVIEWADAPTER + connection.toUTF8();


        EventData addEventData(std::string userToken, EventData eventData)
        {
            try
            {
                Ice::CommunicatorHolder ich = Ice::initialize();
                auto base = ich->stringToProxy(EVENT_CONN_STRING);
                auto eventsDataInterface = Ice::checkedCast<EventInterfacePrx>(base);
                if (!eventsDataInterface)
                {
                    throw std::runtime_error("Invalid proxy");
                }
                eventData.eventInfo.id =  eventsDataInterface->addEventData(userToken, eventData);
            }
            catch (const std::exception &e)
            {
                std::cerr << e.what() << std::endl;
            }
            return eventData;
        }

        EventInfo addEventInfo(std::string userToken, EventInfo eventInfo)
        {
            try
            {
                Ice::CommunicatorHolder ich = Ice::initialize();
                auto base = ich->stringToProxy(EVENT_CONN_STRING);
                auto eventsDataInterface = Ice::checkedCast<EventInterfacePrx>(base);
                if (!eventsDataInterface)
                {
                    throw std::runtime_error("Invalid proxy");
                }
                eventInfo.id = eventsDataInterface->addEventInfo(userToken, eventInfo);
            }
            catch (const std::exception &e)
            {
                std::cerr << e.what() << std::endl;
            }
            return eventInfo;
        }

        SeqEventData getEventsData(std::string userToken)
        {
            SeqEventData seqEventData;
            // Ice communication for getting events data
            try
            {
                Ice::CommunicatorHolder ich = Ice::initialize();
                auto base = ich->stringToProxy(EVENT_CONN_STRING);
                auto eventsDataInterface = Ice::checkedCast<EventInterfacePrx>(base);
                if (!eventsDataInterface)
                {
                    throw std::runtime_error("Invalid proxy");
                }
                // get events data
                seqEventData = eventsDataInterface->getSeqEventData(userToken);
            }
            catch (const std::exception &e)
            {
                std::cerr << "\n\n"
                        << e.what() << "\n\n";
            }
            return seqEventData;
        }

        void delEvent(std::string userToken, int eventId)
        {
            try
            {
                Ice::CommunicatorHolder ich = Ice::initialize();
                auto base = ich->stringToProxy(EVENT_CONN_STRING);
                auto eventsDataInterface = Ice::checkedCast<EventInterfacePrx>(base);
                if (!eventsDataInterface)
                {
                    throw std::runtime_error("Invalid proxy");
                }
                // delete event
                eventsDataInterface->delEvent(userToken, eventId);
                // refresh table
            }
            catch (const std::exception &e)
            {
                std::cerr << "\n\n"
                            << e.what() << "\n\n";
            }
        }



        // Auth Interface
        LoginReturn loginUser(LoginInfo loginInfo)
        {
            LoginReturn loginReturn;
            try
            {
                Ice::CommunicatorHolder ich = Ice::initialize();
                auto base = ich->stringToProxy(AUTH_CONN_STRING);
                auto authInterface = Ice::checkedCast<AuthInterfacePrx>(base);
                if (!authInterface)
                {
                    throw std::runtime_error("Invalid proxy");
                }
                // login user
                loginReturn = authInterface->loginUser(loginInfo);
            }
            catch (const std::exception &e)
            {
                std::cerr << e.what() << std::endl;
            }
            return loginReturn;
        }

        RegistrationResponse registerUser(RegistrationInfo regInfo)
        {
            RegistrationResponse registrationResponse;
            try
            {
                Ice::CommunicatorHolder ich = Ice::initialize();
                auto base = ich->stringToProxy(AUTH_CONN_STRING);
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
		
        ChangePasswordResponse changePassword(std::string userToken, std::string oldPassword, std::string newPassword)
        {
            ChangePasswordResponse changePasswordResponse;
            try
            {
                Ice::CommunicatorHolder ich = Ice::initialize();
                auto base = ich->stringToProxy(AUTH_CONN_STRING);
                auto authInterface = Ice::checkedCast<AuthInterfacePrx>(base);
                if (!authInterface)
                {
                    throw std::runtime_error("Invalid proxy");
                }
                // change password
                changePasswordResponse = authInterface->changePassword(userToken, oldPassword, newPassword);
            }
            catch (const std::exception &e)
            {
                std::cerr << e.what() << std::endl;
            }
            return changePasswordResponse;
        }
		
        ImageData getUserPhotoWithEmail(std::string userEmail)
        {
            ImageData imageData;
            try
            {
                Ice::CommunicatorHolder ich = Ice::initialize();
                auto base = ich->stringToProxy(AUTH_CONN_STRING);
                auto authInterface = Ice::checkedCast<AuthInterfacePrx>(base);
                if (!authInterface)
                {
                    throw std::runtime_error("Invalid proxy");
                }
                // get user photo
                imageData = authInterface->getUserPhotoWithEmail(userEmail);
            }
            catch (const std::exception &e)
            {
                std::cerr << e.what() << std::endl;
            }
            return imageData;
        }
		
        Emlab::ImageData getUserPhotoWithToken(std::string userToken)
        {
            ImageData imageData;
            try
            {
                Ice::CommunicatorHolder ich = Ice::initialize();
                auto base = ich->stringToProxy(AUTH_CONN_STRING);
                auto authInterface = Ice::checkedCast<AuthInterfacePrx>(base);
                if (!authInterface)
                {
                    throw std::runtime_error("Invalid proxy");
                }
                // get user photo
                imageData = authInterface->getUserPhotoWithToken(userToken);
            }
            catch (const std::exception &e)
            {
                std::cerr << e.what() << std::endl;
            }
            return imageData;
        }

        void setUserPhoto(std::string userToken, ImageData photo)
        {
            try
            {
                Ice::CommunicatorHolder ich = Ice::initialize();
                auto base = ich->stringToProxy(AUTH_CONN_STRING);
                auto authInterface = Ice::checkedCast<AuthInterfacePrx>(base);
                if (!authInterface)
                {
                    throw std::runtime_error("Invalid proxy");
                }
                // set user photo
                authInterface->setUserPhoto(userToken, photo);
            }
            catch (const std::exception &e)
            {
                std::cerr << e.what() << std::endl;
            }
        }
    }
}