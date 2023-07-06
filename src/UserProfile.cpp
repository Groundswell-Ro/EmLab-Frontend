#include "include/UserProfile.h"
#include "../../comunication/comm/ProviderInterface.h"

#include <Wt/WMenu.h>
#include <Wt/WMenuItem.h>
#include <Wt/WStackedWidget.h>
#include <Wt/WLink.h>
#include <Wt/WCssDecorationStyle.h>
#include <Wt/WText.h>
#include <Wt/WPushButton.h>
#include <Wt/WAnimation.h>
#include <Wt/WLineEdit.h>

#include <stdexcept>
#include <Ice/Ice.h>

UserProfile::UserProfile(std::shared_ptr<Login> login, int profileId)
:   Wt::WTemplate(tr("profile")),
    login_(login)
{
    setStyleClass("mx-auto w-full max-w-7xl overflow-y-auto");
    getProfileData(profileId);
	auto header = bindWidget("profile-header", std::make_unique<Wt::WTemplate>(tr("profile-header")));
	auto header_cover = header->bindWidget("header-cover", std::make_unique<Wt::WTemplate>(tr("header-cover")));
	header_cover->decorationStyle().setBackgroundImage(Wt::WLink("resources/images/profile-cover/cover1.jpg"));
	// auto profile_calendar = header_with_cover_->bindWidget("calendar", std::make_unique<Calendar>());
	profile_image_ = header_cover->bindWidget("image", std::make_unique<Wt::WImage>(Wt::WLink("resources/images/alex.jpg")));
    auto back_btn = header_cover->bindWidget("back-btn", std::make_unique<Wt::WPushButton>("Back to search"));
    
    // profile buttons 
    auto connect_btn = header_cover->bindWidget("connect-btn", std::make_unique<Wt::WPushButton>(tr("connect-svg-sm")));
    auto calendar_btn = header_cover->bindWidget("calendar-btn", std::make_unique<Wt::WPushButton>(tr("calendar-svg-sm")));
    auto add_to_event_btn = header_cover->bindWidget("add-to-event-btn", std::make_unique<Wt::WPushButton>(tr("plus-svg-sm")));
    connect_btn->setTextFormat(Wt::TextFormat::XHTML);
    calendar_btn->setTextFormat(Wt::TextFormat::XHTML);
    add_to_event_btn->setTextFormat(Wt::TextFormat::XHTML);

    header_cover->setStyleClass("rounded-t-md");

    auto stack = bindWidget("profile-stack", std::make_unique<Wt::WStackedWidget>());
	auto menu = bindWidget("profile-menu", std::make_unique<Wt::WMenu>(stack));
    stack->setTransitionAnimation(Wt::WAnimation(Wt::AnimationEffect::Fade), true);
	auto info_menu = menu->addItem("Info", createInfo()); 
	auto galery_menu = menu->addItem("Galery", createGalery());
	auto services_menu = menu->addItem("Services", createServices());
	auto reviews_menu = menu->addItem("Reviews", createReviews());

	auto menu_item_styles = "bg-body-hover-border simple-menu-item py-2 px-3";
	info_menu->setStyleClass(menu_item_styles);
	galery_menu->setStyleClass(menu_item_styles);
	services_menu->setStyleClass(menu_item_styles);
	reviews_menu->setStyleClass(menu_item_styles);

    menu->select(services_menu);
}

std::unique_ptr<Wt::WContainerWidget> UserProfile::createInfo()
{
    std::unique_ptr<Wt::WContainerWidget> returnWidget = std::make_unique<Wt::WContainerWidget>();
    returnWidget->setStyleClass("basic-widget mt-0");
    auto info_tmp = returnWidget->addWidget(std::make_unique<Wt::WTemplate>(tr("info-content-widget")));

    info_tmp->bindString("name", profileData_.profile.name);
    info_tmp->bindString("username", profileData_.profile.username);
    info_tmp->bindString("phone", profileData_.profile.phone);
    info_tmp->bindString("email", profileData_.profile.email);

    return returnWidget;
}

std::unique_ptr<Wt::WContainerWidget> UserProfile::createGalery()
{
    std::unique_ptr<Wt::WContainerWidget> returnWidget = std::make_unique<Wt::WContainerWidget>();
    auto tmp = returnWidget->addWidget(std::make_unique<Wt::WTemplate>(tr("layout-content-widget")));

    auto stack = tmp->bindWidget("stack", std::make_unique<Wt::WStackedWidget>());
    auto menu = tmp->bindWidget("menu", std::make_unique<Wt::WMenu>(stack));
    stack->setTransitionAnimation(Wt::WAnimation(Wt::AnimationEffect::Fade), true);
    stack->setStyleClass("basic-widget mt-0 ms-0 flex-grow");

    auto container = std::make_unique<Wt::WContainerWidget>();
    auto container_styles = "grid grid-cols-2 md:grid-cols-3 gap-4";
    auto menu_item_styles = "bg-body-hover-border simple-menu-item py-2 px-3 text-bold";
    for(auto servicePhoto : servicePhotos_)
    {   
        container->setStyleClass(container_styles);
        for(auto photo : servicePhoto.second)
        {
            container->addWidget(std::make_unique<Wt::WImage>(Wt::WLink(photo.path)))->setStyleClass("h-auto max-w-full rounded-lg");
        }
        auto menu_item = menu->addItem(servicePhoto.first, std::move(container));
        menu_item->setStyleClass(menu_item_styles);

    }

    menu->select(0);
    return returnWidget;
}

std::unique_ptr<Wt::WContainerWidget> UserProfile::createServices()
{
    std::unique_ptr<Wt::WContainerWidget> returnWidget = std::make_unique<Wt::WContainerWidget>();
    
    auto tmp = returnWidget->addWidget(std::make_unique<Wt::WTemplate>(tr("layout-content-widget")));
    auto stack = tmp->bindWidget("stack", std::make_unique<Wt::WStackedWidget>());
    auto menu = tmp->bindWidget("menu", std::make_unique<Wt::WMenu>(stack));
    stack->setTransitionAnimation(Wt::WAnimation(Wt::AnimationEffect::Fade), true);
    stack->setStyleClass("basic-widget mt-0 ms-0 flex-grow");

    // add service to provider 
    for(auto service : profileData_.services){
        auto service_tmp = std::make_unique<Wt::WTemplate>(tr("service"));
        auto title = service_tmp->bindWidget("title", std::make_unique<Wt::WText>(service.title));
        auto description = service_tmp->bindWidget("description", std::make_unique<Wt::WText>(service.description));
        auto price = service_tmp->bindWidget("service-price", std::make_unique<Wt::WText>(std::to_string(service.price) + " Ron/hour"));
        auto number_of_reviews = service_tmp->bindWidget("number-of-reviews", std::make_unique<Wt::WText>("24 i"));
        auto rating = service_tmp->bindWidget("rating", std::make_unique<Wt::WText>("4.5 ★★★★★ i"));
        auto photo_stack = service_tmp->bindWidget("service-galery-stack", std::make_unique<Wt::WStackedWidget>());
        photo_stack->setTransitionAnimation(Wt::WAnimation(Wt::AnimationEffect::Fade), true);
        auto prev_btn = service_tmp->bindWidget("prev-btn", std::make_unique<Wt::WPushButton>(tr("arrow-left-svg")));
        auto next_btn = service_tmp->bindWidget("next-btn", std::make_unique<Wt::WPushButton>(tr("arrow-right-svg")));
        prev_btn->setTextFormat(Wt::TextFormat::XHTML);
        next_btn->setTextFormat(Wt::TextFormat::XHTML);

        
        auto images_styles = "w-1/4 h-32 object-cover";
        auto galery_page = std::make_unique<Wt::WContainerWidget>();
        int count = 0;

        for (const auto& pair : servicePhotos_) {
            if (pair.first == service.title) {
                const std::vector<ServicePhoto>& servicePhotos = pair.second;
                for (const auto& servicePhoto : servicePhotos) {
                    ++count;
                    if (count == 4) {
                        photo_stack->addWidget(std::move(galery_page));
                        galery_page = std::make_unique<Wt::WContainerWidget>();
                        count = 0;
                    }
                    galery_page->addWidget(std::make_unique<Wt::WImage>(Wt::WLink(servicePhoto.path)))->setStyleClass(images_styles);
                }
            }
        }
        if(count != 0){
            photo_stack->addWidget(std::move(galery_page));
        }

        prev_btn->clicked().connect(this, [=](){
            if(photo_stack->currentIndex() == 0) {
                photo_stack->setCurrentIndex(photo_stack->count() - 1);
            } else {
                photo_stack->setCurrentIndex(photo_stack->currentIndex() - 1);
            }
        });

        next_btn->clicked().connect(this, [=](){
            if(photo_stack->currentIndex() == photo_stack->count() - 1) {
                photo_stack->setCurrentIndex(0);
            } else {
                photo_stack->setCurrentIndex(photo_stack->currentIndex() + 1);
            }
        });


        auto menu_item_one = menu->addItem(service.title, std::move(service_tmp));

        Wt::WString menu_item_styles = "bg-body-hover-border simple-menu-item py-2 px-3 text-bold";
        menu_item_one->setStyleClass(menu_item_styles);
    }

    menu->select(0);
    return returnWidget;
}

std::unique_ptr<Wt::WContainerWidget> UserProfile::createReviews()
{
    std::unique_ptr<Wt::WContainerWidget> returnWidget = std::make_unique<Wt::WContainerWidget>();
    returnWidget->setStyleClass("basic-widget mt-0");
    
    auto tmp = returnWidget->addWidget(std::make_unique<Wt::WTemplate>(tr("reviews-content-widget")));
    auto stack = tmp->bindWidget("reviews-stack", std::make_unique<Wt::WStackedWidget>());
    auto menu = tmp->bindWidget("reviews-menu", std::make_unique<Wt::WMenu>(stack));
    stack->setStyleClass("flex flex-col list-none ps-0");
    // stack->setTransitionAnimation(Wt::WAnimation(Wt::AnimationEffect::Fade), true);
    auto prev_btn = tmp->bindWidget("prev-btn", std::make_unique<Wt::WPushButton>(tr("arrow-left-svg-sm")));
    auto next_btn = tmp->bindWidget("next-btn", std::make_unique<Wt::WPushButton>(tr("arrow-right-svg-sm")));

    prev_btn->setTextFormat(Wt::TextFormat::XHTML);
    next_btn->setTextFormat(Wt::TextFormat::XHTML);

    Emlab::SeqReviewInfo seqReviewInfo;
    for(int i = 0; i < 10; i++)
    {
        for(int i = 0; i < 5; i++)
        {
            Emlab::ReviewInfo reviewInfo;
            reviewInfo.author = "Alexandru Dan Croitoriu";
            reviewInfo.title = "Great service!";
            reviewInfo.content = "Lorem ipsum dolor sit amet consectetur adipisicing elit. Quisquam, voluptatum.";
            seqReviewInfo.push_back(reviewInfo);
        }
        auto review_container = std::make_unique<Wt::WContainerWidget>();
        for(auto reviewInfo : seqReviewInfo)
        {
            auto review = std::make_unique<Wt::WTemplate>(tr("review"));
            review->bindWidget("photo", std::make_unique<Wt::WImage>(Wt::WLink("resources/images/alex.jpg")));
            review->bindString("author", reviewInfo.author);
            review->bindString("title", reviewInfo.title);
            review->bindString("review", reviewInfo.content);
            review_container->addWidget(std::move(review));
        }
        // stack->count()
        auto menu_item = menu->addItem(std::to_string(stack->count()+1), std::move(review_container));
        menu_item->setStyleClass("btn rounded-none flex justify-center items-center m-0 text-sm [&:not(.itemselected)]:hidden");
    }

    next_btn->clicked().connect(this, [=](){
        if(menu->currentIndex() < menu->count() - 1)
            menu->select(menu->currentIndex() + 1);
    });

    prev_btn->clicked().connect(this, [=](){
        if(menu->currentIndex() > 0)
            menu->select(menu->currentIndex() - 1);
    });

    menu->select(0);

    return returnWidget;
}

void UserProfile::getProfileData(int profileId)
{
	try {
        Ice::CommunicatorHolder ich = Ice::initialize();
        auto base = ich->stringToProxy(login_->PROVIDER_CONN_STRING);
        auto providerInterface = Ice::checkedCast<Emlab::ProviderInterfacePrx>(base);
        if (!providerInterface)
        {
            throw std::runtime_error("Invalid proxy");
        }

        // auto providerInfo = providerInterface->getProfileAsUser(login_->user().token);
        auto providerInfo = providerInterface->getProfileAsClient(profileId);
		auto providerServices = providerInterface->getSeqProviderServices(profileId);

		profileData_.profile = providerInfo;

        for(auto providerService : providerServices)
        {
            profileData_.services.push_back(providerService);

            // service photos 
            std::vector<ServicePhoto> servicePhotos;
            for(auto servicePhotoId : providerService.photoIds){                
                auto photoInfo = providerInterface->getProviderServicePhoto(servicePhotoId);
                auto photoPath = "resources/portofolioImages/" + providerInfo.email + "/" + providerService.title + "/" + photoInfo.name;
                Emlab::bytesToImage(photoInfo.data, photoPath);
                ServicePhoto servicePhoto;
                servicePhoto.id = servicePhotoId;
                servicePhoto.path = photoPath;
                servicePhoto.serviceId = providerService.id;
                servicePhoto.name = photoInfo.name;
                servicePhotos.push_back(servicePhoto);
            }
            auto servicePhotoInfo = std::make_pair(providerService.title, servicePhotos);
            servicePhotos_.push_back(servicePhotoInfo);

        }
		
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}