#include "include/UserProfile.h"

UserProfile::UserProfile()
: WTemplate(tr("home-profile"))
{
    setStyleClass("mx-auto w-full max-w-7xl !overflow-y-scroll");
	header_ = bindWidget("profile-header", std::make_unique<Wt::WTemplate>(tr("profile-header")));
	header_cover_ = header_->bindWidget("header-cover", std::make_unique<Wt::WTemplate>(tr("header-cover")));
	header_cover_->decorationStyle().setBackgroundImage(Wt::WLink("resources/images/profile-cover/blue-lemon.jpg"));
	// auto profile_calendar = header_with_cover_->bindWidget("calendar", std::make_unique<Calendar>());
	profile_image_ = header_cover_->bindWidget("image", std::make_unique<Wt::WImage>(Wt::WLink("resources/images/alex.jpg")));
    auto back_btn = header_cover_->bindWidget("back-btn", std::make_unique<Wt::WPushButton>("Back to search"));
    
    // profile buttons 
    auto connect_btn = header_cover_->bindWidget("connect-btn", std::make_unique<Wt::WPushButton>(tr("connect-svg-sm")));
    auto calendar_btn = header_cover_->bindWidget("calendar-btn", std::make_unique<Wt::WPushButton>(tr("calendar-svg-sm")));
    auto add_to_event_btn = header_cover_->bindWidget("add-to-event-btn", std::make_unique<Wt::WPushButton>(tr("plus-svg-sm")));
    connect_btn->setTextFormat(Wt::TextFormat::XHTML);
    calendar_btn->setTextFormat(Wt::TextFormat::XHTML);
    add_to_event_btn->setTextFormat(Wt::TextFormat::XHTML);


    header_cover_->setStyleClass("rounded-t-md");

    stack_ = bindWidget("profile-stack", std::make_unique<Wt::WStackedWidget>());
	menu_ = bindWidget("profile-menu", std::make_unique<Wt::WMenu>(stack_));

	auto info_menu = menu_->addItem("Info", createInfoPage()); 
	auto galery_menu = menu_->addItem("Galery", createGaleryPage());
	auto services_menu = menu_->addItem("Services", createServicesPage());
	auto reviews_menu = menu_->addItem("Reviews", createRreviewsPage());
    auto settings_menu = menu_->addItem("Settings", createSettingsPage());

	auto menu_item_styles = "bg-body-hover-border simple-menu-item py-2 px-3";
	info_menu->setStyleClass(menu_item_styles);
	galery_menu->setStyleClass(menu_item_styles);
	services_menu->setStyleClass(menu_item_styles);
	reviews_menu->setStyleClass(menu_item_styles);
    settings_menu->setStyleClass(menu_item_styles);

    menu_->select(reviews_menu);
}

std::unique_ptr<Wt::WContainerWidget> UserProfile::createInfoPage()
{
    std::unique_ptr<Wt::WContainerWidget> returnWidget = std::make_unique<Wt::WContainerWidget>();
    returnWidget->setStyleClass("m-4 p-4 bg-teal-400");
    
    
    return returnWidget;
}

std::unique_ptr<Wt::WContainerWidget> UserProfile::createGaleryPage()
{
    std::unique_ptr<Wt::WContainerWidget> returnWidget = std::make_unique<Wt::WContainerWidget>();
    returnWidget->setStyleClass("m-4 p-4 bg-blue-400");
    
    
    return returnWidget;
}

std::unique_ptr<Wt::WContainerWidget> UserProfile::createServicesPage()
{
    std::unique_ptr<Wt::WContainerWidget> returnWidget = std::make_unique<Wt::WContainerWidget>();
    returnWidget->setStyleClass("m-4 p-4 bg-red-400");
    
    
    return returnWidget;
}

std::unique_ptr<Wt::WContainerWidget> UserProfile::createRreviewsPage()
{
    std::unique_ptr<Wt::WContainerWidget> returnWidget = std::make_unique<Wt::WContainerWidget>();
    returnWidget->setStyleClass("");
    
    auto reviews_tmp = returnWidget->addWidget(std::make_unique<Wt::WTemplate>(tr("reviews-widget")));
    auto reviews_container = reviews_tmp->bindWidget("reviews-container", std::make_unique<Wt::WContainerWidget>());
    auto nav_btn_bar = reviews_tmp->bindWidget("nav-btn-bar", std::make_unique<Wt::WToolBar>());
    nav_btn_bar->setStyleClass("flex justify-start items-center ");

    auto prev_btn = std::make_unique<Wt::WPushButton>(tr("arrow-left-svg-sm"));
    auto page_1_btn = std::make_unique<Wt::WPushButton>("1");
    auto page_2_btn = std::make_unique<Wt::WPushButton>("2");
    auto next_btn = std::make_unique<Wt::WPushButton>(tr("arrow-right-svg-sm"));

    prev_btn->setTextFormat(Wt::TextFormat::XHTML);
    next_btn->setTextFormat(Wt::TextFormat::XHTML);

    Wt::WString btns_styles = "btn rounded-none flex justify-center items-center m-0";
    prev_btn->setStyleClass(btns_styles);
    page_1_btn->setStyleClass(btns_styles + " active");
    page_2_btn->setStyleClass(btns_styles);
    next_btn->setStyleClass(btns_styles);

    nav_btn_bar->addButton(std::move(prev_btn));
    nav_btn_bar->addButton(std::move(page_1_btn));
    nav_btn_bar->addButton(std::move(page_2_btn));
    nav_btn_bar->addButton(std::move(next_btn));

    for(int i = 0; i < 5; ++i){
        addReview(reviews_container);
    }
    return returnWidget;
}

std::unique_ptr<Wt::WContainerWidget> UserProfile::createSettingsPage()
{
    std::unique_ptr<Wt::WContainerWidget> returnWidget = std::make_unique<Wt::WContainerWidget>();
    returnWidget->setStyleClass("m-4 p-4 bg-yellow-400");   
    return returnWidget;
}

void UserProfile::addReview(Wt::WContainerWidget* container)
{
    auto review = container->addWidget(std::make_unique<Wt::WTemplate>(tr("review")));
    review->bindString("author", "Alexandru Dan Croitoriu");
    review->bindString("title", "Great service!");
    review->bindString("review", "Lorem ipsum dolor sit amet consectetur adipisicing elit. Quisquam, voluptatum.");
}