#pragma once
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
#include <Wt/WToolBar.h>

class UserProfile : public Wt::WTemplate
{
public:
        UserProfile();
private:
    Wt::WTemplate* header_;
    Wt::WTemplate* header_cover_;
    Wt::WImage* profile_image_;
    Wt::WMenu* menu_;
    Wt::WStackedWidget* stack_;

    std::unique_ptr<Wt::WContainerWidget> createInfoPage();
    std::unique_ptr<Wt::WContainerWidget> createGaleryPage();
    std::unique_ptr<Wt::WContainerWidget> createServicesPage();
    std::unique_ptr<Wt::WContainerWidget> createRreviewsPage();
    std::unique_ptr<Wt::WContainerWidget> createSettingsPage();

    void addReview(Wt::WContainerWidget* container);
};