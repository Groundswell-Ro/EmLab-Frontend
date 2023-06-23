#include "include/UserPortofolio.h"

UserPortofolio::UserPortofolio(std::shared_ptr<Login> login)
:   Wt::WTemplate(tr("profile")),
    login_(login)
{
    setStyleClass("mx-auto w-full max-w-7xl overflow-y-auto");
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

    menu->select(info_menu);
}

std::unique_ptr<Wt::WContainerWidget> UserPortofolio::createInfo()
{
    std::unique_ptr<Wt::WContainerWidget> returnWidget = std::make_unique<Wt::WContainerWidget>();
    returnWidget->setStyleClass("basic-widget mt-0");
    auto info_tmp = returnWidget->addWidget(std::make_unique<Wt::WTemplate>(tr("info-content-widget")));
    return returnWidget;
}

std::unique_ptr<Wt::WContainerWidget> UserPortofolio::createGalery()
{
    std::unique_ptr<Wt::WContainerWidget> returnWidget = std::make_unique<Wt::WContainerWidget>();
    auto tmp = returnWidget->addWidget(std::make_unique<Wt::WTemplate>(tr("profile-content-widget")));

    auto stack = tmp->bindWidget("stack", std::make_unique<Wt::WStackedWidget>());
    auto menu = tmp->bindWidget("menu", std::make_unique<Wt::WMenu>(stack));
    stack->setTransitionAnimation(Wt::WAnimation(Wt::AnimationEffect::Fade), true);
    stack->setStyleClass("basic-widget mt-0 ms-0 flex-grow");
    auto all_photos_container = std::make_unique<Wt::WContainerWidget>();
    all_photos_container->setStyleClass("grid grid-cols-2 md:grid-cols-3 gap-4");
    for(int i = 0; i < 10; i++) {
        auto photo = all_photos_container->addWidget(std::make_unique<Wt::WImage>(Wt::WLink("resources/images/profile-cover/cover1.jpg")));
        photo->setStyleClass("h-auto max-w-full rounded-lg");
    }

    auto all_photos_menu_item = menu->addItem("All Photos", std::move(all_photos_container));

    Wt::WString menu_item_styles = "bg-body-hover-border simple-menu-item py-2 px-3 text-bold";
    all_photos_menu_item->setStyleClass(menu_item_styles);

    menu->select(0);
    return returnWidget;
}

std::unique_ptr<Wt::WContainerWidget> UserPortofolio::createServices()
{
    std::unique_ptr<Wt::WContainerWidget> returnWidget = std::make_unique<Wt::WContainerWidget>();
    
    auto tmp = returnWidget->addWidget(std::make_unique<Wt::WTemplate>(tr("profile-content-widget")));
    auto stack = tmp->bindWidget("stack", std::make_unique<Wt::WStackedWidget>());
    auto menu = tmp->bindWidget("menu", std::make_unique<Wt::WMenu>(stack));
    stack->setTransitionAnimation(Wt::WAnimation(Wt::AnimationEffect::Fade), true);
    stack->setStyleClass("basic-widget mt-0 ms-0 flex-grow");

    // add service to provider 
    auto service_one = std::make_unique<Wt::WTemplate>(tr("service"));

    auto title = service_one->bindWidget("title", std::make_unique<Wt::WText>("Animator"));
    auto description = service_one->bindWidget("description", std::make_unique<Wt::WText>(tr("service-description-test")));
    auto price = service_one->bindWidget("price", std::make_unique<Wt::WText>("200 Ron/hour"));
    auto number_of_reviews = service_one->bindWidget("number-of-reviews", std::make_unique<Wt::WText>("24"));
    auto rating = service_one->bindWidget("rating", std::make_unique<Wt::WText>("4.5 ★★★★★"));
    auto photo_stack = service_one->bindWidget("service-galery-stack", std::make_unique<Wt::WStackedWidget>());
    photo_stack->setTransitionAnimation(Wt::WAnimation(Wt::AnimationEffect::Fade), true);
    // photo_stack->setTransitionAnimation(Wt::WAnimation(Wt::AnimationEffect::SlideInFromLeft), true);
    auto prev_btn = service_one->bindWidget("prev-btn", std::make_unique<Wt::WPushButton>(tr("arrow-left-svg")));
    auto next_btn = service_one->bindWidget("next-btn", std::make_unique<Wt::WPushButton>(tr("arrow-right-svg")));
    prev_btn->setTextFormat(Wt::TextFormat::XHTML);
    next_btn->setTextFormat(Wt::TextFormat::XHTML);

    
    for(int i = 0 ; i < 2; i++) {
        auto galery_page_1 = photo_stack->addWidget(std::make_unique<Wt::WContainerWidget>());
        auto img_1 = galery_page_1->addWidget(std::make_unique<Wt::WImage>(Wt::WLink("resources/images/profile-cover/cover1.jpg")));
        auto img_2 = galery_page_1->addWidget(std::make_unique<Wt::WImage>(Wt::WLink("resources/images/profile-cover/cover2.jpg")));
        auto img_3 = galery_page_1->addWidget(std::make_unique<Wt::WImage>(Wt::WLink("resources/images/profile-cover/cover3.jpg")));
        auto img_4 = galery_page_1->addWidget(std::make_unique<Wt::WImage>(Wt::WLink("resources/images/profile-cover/cover4.jpg")));

        auto images_styles = "w-1/4 h-32 object-cover";
        img_1->setStyleClass(images_styles);
        img_2->setStyleClass(images_styles);
        img_3->setStyleClass(images_styles);
        img_4->setStyleClass(images_styles);
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


    auto menu_item_one = menu->addItem("Animator", std::move(service_one));

    Wt::WString menu_item_styles = "bg-body-hover-border simple-menu-item py-2 px-3 text-bold";
    menu_item_one->setStyleClass(menu_item_styles);

    menu->select(0);


    return returnWidget;
}

std::unique_ptr<Wt::WContainerWidget> UserPortofolio::createReviews()
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