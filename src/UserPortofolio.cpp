#include "include/UserPortofolio.h"

UserPortofolio::UserPortofolio(std::shared_ptr<Login> login)
:   Wt::WTemplate(tr("profile")),
    login_(login)
{
    setStyleClass("mx-auto w-full max-w-7xl !overflow-y-scroll");
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
    // auto settings_menu = menu->addItem("Settings", createSettings());

	auto menu_item_styles = "bg-body-hover-border simple-menu-item py-2 px-3";
	info_menu->setStyleClass(menu_item_styles);
	galery_menu->setStyleClass(menu_item_styles);
	services_menu->setStyleClass(menu_item_styles);
	reviews_menu->setStyleClass(menu_item_styles);
    // settings_menu->setStyleClass(menu_item_styles);

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
    menu->setStyleClass("basic-widget mt-0 flex flex-col text-center min-w-[200px] max-w-[600px] list-none");
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
    menu->setStyleClass("basic-widget mt-0 flex flex-col text-center min-w-[200px] max-w-[600px] list-none");
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

std::unique_ptr<Wt::WContainerWidget> UserPortofolio::createSettings()
{
    std::unique_ptr<Wt::WContainerWidget> returnWidget = std::make_unique<Wt::WContainerWidget>();
    returnWidget->setStyleClass("mt-0");
    
    auto tmp = returnWidget->addWidget(std::make_unique<Wt::WTemplate>(tr("profile-content-widget")));
    auto stack = tmp->bindWidget("stack", std::make_unique<Wt::WStackedWidget>());
    auto menu = tmp->bindWidget("menu", std::make_unique<Wt::WMenu>(stack));
    // stack->setTransitionAnimation(Wt::WAnimation(Wt::AnimationEffect::Fade), true);
    menu->setStyleClass("basic-widget mt-0 flex flex-col text-center min-w-[200px] max-w-[600px] list-none");
    stack->setStyleClass("flex-grow");

    auto settings_general_tmp = createSettingsGeneralWidget("settings-widget");
    auto change_email_tmp = createChangeEmailWidget("settings-widget");
    auto change_username_tmp = createChangeUsernameWidget("settings-widget");
    auto change_name_tmp = createChangeNameWidget("settings-widget");
    auto change_phone_tmp = createChangePhoneWidget("settings-widget");
    auto change_password_tmp = createChangePasswordWidget("settings-widget");

    auto menu_item_general = menu->addItem("General", std::move(settings_general_tmp));
    auto menu_item_email = menu->addItem("Change Email", std::move(change_email_tmp));
    auto menu_item_username = menu->addItem("Change Username", std::move(change_username_tmp));
    auto menu_item_name = menu->addItem("Change Name", std::move(change_name_tmp));
    auto menu_item_phone = menu->addItem("Change Phone", std::move(change_phone_tmp));
    auto menu_item_password = menu->addItem("Change Password", std::move(change_password_tmp));

    Wt::WString menu_item_styles = "bg-body-hover-border simple-menu-item py-2 px-3 text-bold";
    menu_item_general->setStyleClass(menu_item_styles);
    menu_item_email->setStyleClass(menu_item_styles);
    menu_item_username->setStyleClass(menu_item_styles);
    menu_item_name->setStyleClass(menu_item_styles);
    menu_item_phone->setStyleClass(menu_item_styles);
    menu_item_password->setStyleClass(menu_item_styles);
    

    menu->select(0);

    return returnWidget;
}

std::unique_ptr<Wt::WTemplate> UserPortofolio::createSettingsGeneralWidget(std::string tempName)
{
    auto returnWidget = std::make_unique<Wt::WTemplate>(tr(tempName));
    returnWidget->bindString("title", "general");

    auto tmp = returnWidget->bindWidget("data-form-widget", std::make_unique<Wt::WTemplate>(tr("settings-general-data-widget")));
    

    return returnWidget;
}

// those widgets are created into the settings page
std::unique_ptr<Wt::WTemplate> UserPortofolio::createChangeEmailWidget(std::string tempName)
{
    auto widget_tmp = std::make_unique<Wt::WTemplate>(tr(tempName));
    widget_tmp->bindString("title", "change email");
    
    auto tmp = widget_tmp->bindWidget("data-form-widget", std::make_unique<Wt::WTemplate>(tr("data-form-widget")));
    tmp->bindString("title", "email");
    tmp->bindString("current-data", "croitoriu.93@gmail.com");
    tmp->bindWidget("icon-svg", std::make_unique<Wt::WTemplate>(tr("email-svg")));
    tmp->bindEmpty("submit-info");
    tmp->bindWidget("validation-state", std::make_unique<Wt::WTemplate>("<div class=\"text-green-400 font-semibold\">Validated</div>"));
    auto email_input = tmp->bindWidget("input", std::make_unique<Wt::WLineEdit>());
    auto submit_btn = tmp->bindWidget("submit-btn", std::make_unique<Wt::WPushButton>("submit"));

    submit_btn->clicked().connect(this, [=](){
        tmp->bindString("submit-info", "Email changed successfully!");
    });
    return widget_tmp;
}

std::unique_ptr<Wt::WTemplate> UserPortofolio::createChangeUsernameWidget(std::string tempName)
{
auto widget_tmp = std::make_unique<Wt::WTemplate>(tr(tempName));
    widget_tmp->bindString("title", "change username");

    auto tmp = widget_tmp->bindWidget("data-form-widget", std::make_unique<Wt::WTemplate>(tr("data-form-widget")));
    tmp->bindString("title", "username");

    tmp->bindString("current-data", "maxuli");

    tmp->bindWidget("icon-svg", std::make_unique<Wt::WTemplate>(tr("human-svg")));
    tmp->bindEmpty("submit-info");
    tmp->bindWidget("validation-state", std::make_unique<Wt::WTemplate>("<div class=\"text-green-400 font-semibold\">Validated</div>"));
    auto email_input = tmp->bindWidget("input", std::make_unique<Wt::WLineEdit>());
    auto submit_btn = tmp->bindWidget("submit-btn", std::make_unique<Wt::WPushButton>("submit"));


    submit_btn->clicked().connect(this, [=](){
        tmp->bindString("submit-info", "Email changed successfully!");
    });
return widget_tmp;
}

std::unique_ptr<Wt::WTemplate> UserPortofolio::createChangeNameWidget(std::string tempName)
{
auto widget_tmp = std::make_unique<Wt::WTemplate>(tr(tempName));
    widget_tmp->bindString("title", "change name");

    auto tmp = widget_tmp->bindWidget("data-form-widget", std::make_unique<Wt::WTemplate>(tr("data-form-widget")));
    tmp->bindString("title", "name");
    
    tmp->bindString("current-data", "alexandru dan croitoriu");

    tmp->bindWidget("icon-svg", std::make_unique<Wt::WTemplate>(tr("human-svg")));
    tmp->bindEmpty("submit-info");
    tmp->bindWidget("validation-state", std::make_unique<Wt::WTemplate>("<div class=\"text-green-400 font-semibold\">Validated</div>"));
    auto email_input = tmp->bindWidget("input", std::make_unique<Wt::WLineEdit>());
    auto submit_btn = tmp->bindWidget("submit-btn", std::make_unique<Wt::WPushButton>("submit"));

    submit_btn->clicked().connect(this, [=](){
        tmp->bindString("submit-info", "Email changed successfully!");
    });
return widget_tmp;
}

std::unique_ptr<Wt::WTemplate> UserPortofolio::createChangePhoneWidget(std::string tempName)
{
auto widget_tmp = std::make_unique<Wt::WTemplate>(tr(tempName));
    widget_tmp->bindString("title", "change phone");

    auto tmp = widget_tmp->bindWidget("data-form-widget", std::make_unique<Wt::WTemplate>(tr("data-form-widget")));
    tmp->bindString("title", "phone");

    tmp->bindString("current-data", "0773 132 455");

    tmp->bindWidget("icon-svg", std::make_unique<Wt::WTemplate>(tr("phone-svg")));
    tmp->bindEmpty("submit-info");
    tmp->bindWidget("validation-state", std::make_unique<Wt::WTemplate>("<div class=\"text-green-400 font-semibold\">Validated</div>"));
    auto email_input = tmp->bindWidget("input", std::make_unique<Wt::WLineEdit>());
    auto submit_btn = tmp->bindWidget("submit-btn", std::make_unique<Wt::WPushButton>("submit"));

    submit_btn->clicked().connect(this, [=](){
        tmp->bindString("submit-info", "Email changed successfully!");
    });
    return widget_tmp;
}

std::unique_ptr<Wt::WTemplate> UserPortofolio::createChangePasswordWidget(std::string tempName)
{
    auto widget_tmp = std::make_unique<Wt::WTemplate>(tr(tempName));
    widget_tmp->bindString("title", "change password");

    auto tmp = widget_tmp->bindWidget("data-form-widget", std::make_unique<Wt::WTemplate>(tr("data-change-password-widget")));
    tmp->bindString("title", "password");

    tmp->bindWidget("key-svg", std::make_unique<Wt::WText>(tr("key-svg")));
    tmp->bindWidget("password-requirments", std::make_unique<Wt::WTemplate>(tr("password-requirments")));
    tmp->bindEmpty("submit-info");
    auto old_password = tmp->bindWidget("old-password", std::make_unique<Wt::WLineEdit>());
    auto new_password = tmp->bindWidget("new-password", std::make_unique<Wt::WLineEdit>());
    auto repeat_password = tmp->bindWidget("repeat-password", std::make_unique<Wt::WLineEdit>());
    
    auto old_pass_eye_btn = tmp->bindWidget("old-password-eye", std::make_unique<Wt::WTemplate>(tr("eye-crossed-svg")));
    auto new_pass_eye_btn = tmp->bindWidget("new-password-eye", std::make_unique<Wt::WTemplate>(tr("eye-crossed-svg")));
    auto repeat_pass_eye_btn = tmp->bindWidget("repeat-password-eye", std::make_unique<Wt::WTemplate>(tr("eye-crossed-svg")));

    auto submit_btn = tmp->bindWidget("submit-btn", std::make_unique<Wt::WPushButton>("Submit"));

    old_password->setEchoMode(Wt::EchoMode::Password);
    new_password->setEchoMode(Wt::EchoMode::Password);
    repeat_password->setEchoMode(Wt::EchoMode::Password);

	old_pass_eye_btn->clicked().connect(this, [=](){
        if(old_password->echoMode() == Wt::EchoMode::Password)
        {
            old_password->setEchoMode(Wt::EchoMode::Normal);
            old_pass_eye_btn->setTemplateText(tr("eye-svg"));
                }else {
            old_password->setEchoMode(Wt::EchoMode::Password);
            old_pass_eye_btn->setTemplateText(tr("eye-crossed-svg"));
        }
    });

    new_pass_eye_btn->clicked().connect(this, [=](){
        if(new_password->echoMode() == Wt::EchoMode::Password)
        {
            new_password->setEchoMode(Wt::EchoMode::Normal);
            new_pass_eye_btn->setTemplateText(tr("eye-svg"));
                }else {
            new_password->setEchoMode(Wt::EchoMode::Password);
            new_pass_eye_btn->setTemplateText(tr("eye-crossed-svg"));
        }
    });

    repeat_pass_eye_btn->clicked().connect(this, [=](){
        if(repeat_password->echoMode() == Wt::EchoMode::Password)
        {
            repeat_password->setEchoMode(Wt::EchoMode::Normal);
            repeat_pass_eye_btn->setTemplateText(tr("eye-svg"));
                }else {
            repeat_password->setEchoMode(Wt::EchoMode::Password);
            repeat_pass_eye_btn->setTemplateText(tr("eye-crossed-svg"));
        }
    });


return widget_tmp;
}