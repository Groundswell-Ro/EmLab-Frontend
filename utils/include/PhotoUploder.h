#pragma once
#include "Utils.h"
#include <Wt/WTemplate.h>
#include <Wt/WFileUpload.h>
#include <Wt/WText.h>
#include <Wt/WImage.h>
#include <Wt/WLink.h>
#include <Wt/WFileDropWidget.h>
#include <Wt/WPushButton.h>

class PhotoUploder : public Wt::WTemplate
{
public:
    PhotoUploder(std::string userPath = "", bool singlePhoto = true);
    ~PhotoUploder();
    Emlab::ImageData getImageData();

    Wt::WFileUpload *file_uploder_;
    Wt::WContainerWidget* uploder_content_;
    Wt::WText *status_;

    void setSinglePhoto(Wt::WString photo_path = "");
    void setMultiplePhotosDisplay();
    
private:
    void uploderChanged();
    void uploderUploded();
    void uploderFileToLarge();
    Wt::WString destination = "resources/temp_images/";
    Wt::WString default_photo_path_ = "resources/images/blank-profile-picture.png";

    bool singlePhoto_;
    // photos path are stored here
    std::vector<std::string> photos_;
};


