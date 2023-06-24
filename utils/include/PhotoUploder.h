#pragma once
#include "Utils.h"
#include <Wt/WTemplate.h>
#include <Wt/WFileUpload.h>
#include <Wt/WText.h>
#include <Wt/WImage.h>
#include <Wt/WLink.h>
#include <Wt/WFileDropWidget.h>

class PhotoUploder : public Wt::WTemplate
{
public:
    PhotoUploder(std::string tamplateName = "photo.uploder.template");
    ~PhotoUploder();
    Wt::WFileUpload *fileUpload_;
    Emlab::ImageData getImageData();
private:
    void uploderChanged();
    void uploderUploded();
    void uploderFileToLarge();
    Wt::WText *status_;
    Wt::WImage *photo_;
    Wt::WString destination = "resources/temp_images/";
    
};


