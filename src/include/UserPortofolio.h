#pragma once
#include "LoginObj.h"
#include "../../../comunication/comm/ReviewInterface.h"

#include <Wt/WTemplate.h>
#include <Wt/WContainerWidget.h>
#include <Wt/WMenu.h>
#include <Wt/WMenuItem.h>
#include <Wt/WStackedWidget.h>
#include <Wt/WLink.h>
#include <Wt/WCssDecorationStyle.h>
#include <Wt/WText.h>
#include <Wt/WImage.h>
#include <Wt/WPushButton.h>
#include <Wt/WAnimation.h>
#include <Wt/WLineEdit.h>

class UserPortofolio : public Wt::WTemplate
{
public:
        UserPortofolio(std::shared_ptr<Login> login);
private:
    Wt::WImage* profile_image_;

    std::unique_ptr<Wt::WContainerWidget> createInfo();
    std::unique_ptr<Wt::WContainerWidget> createGalery();
    std::unique_ptr<Wt::WContainerWidget> createServices();
    std::unique_ptr<Wt::WContainerWidget> createReviews();

private:
    std::shared_ptr<Login> login_;
};