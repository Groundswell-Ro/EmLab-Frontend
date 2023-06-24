#pragma once
#include "../../../comunication/comm/DataTypes.h"
#include <string>

namespace Emlab
{
    ImageData imageToBytes(const std::string& imagePath);
 	bool bytesToImage(const ImageData& bytes, const std::string& imagePath);
    bool validateImage(const std::string& imagePath);
    bool deleteFile(const std::string& filePath);
    bool deleteFolder(const std::string& folderPath, const std::string& folderName);
    namespace Comunication {
        EventInfo addEventInfo(std::string userToken, EventInfo eventInfo);
        EventData addEventData(std::string userToken, EventData eventData);
        SeqEventData getEventsData(std::string userToken);
        void delEvent(std::string userToken, int eventId);
        
        // Auth Interface
        LoginReturn loginUser(LoginInfo loginInfo);
        RegistrationResponse registerUser(RegistrationInfo regInfo);
        ChangePasswordResponse changePassword(std::string userToken, std::string oldPassword, std::string newPassword);
        Emlab::ImageData getUserPhotoWithEmail(std::string userEmail);
	    Emlab::ImageData getUserPhotoWithToken(std::string userToken);
        void setUserPhoto(std::string userToken, ImageData photo);
    }

};