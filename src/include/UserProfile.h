#pragma once
#include "../../utils/include/LoginObj.h"

#include <Wt/WImage.h>
#include <Wt/WTemplate.h>
#include <Wt/WContainerWidget.h>
 
struct ServicePhoto 
{
    int id;
    int serviceId;
    std::string path;
    std::string name;
    
};

class UserProfile : public Wt::WTemplate
{
public:
        UserProfile(std::shared_ptr<Login> login, int profileId);
private:
    Wt::WImage* profile_image_;

    std::unique_ptr<Wt::WContainerWidget> createInfo();
    std::unique_ptr<Wt::WContainerWidget> createGalery();
    std::unique_ptr<Wt::WContainerWidget> createServices();
    std::unique_ptr<Wt::WContainerWidget> createReviews();

private:
    std::shared_ptr<Login> login_;
    Emlab::ProfileData profileData_;
    std::vector<std::pair<std::string, std::vector<ServicePhoto>>> servicePhotos_;
    

    void getProfileData(int profileId);
};