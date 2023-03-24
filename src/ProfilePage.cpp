#include "include/ProfilePage.h"
#include <Wt/WDialog.h>
#include <Wt/WLineEdit.h>
#include <Wt/WTextArea.h>
#include <Wt/WLabel.h>
#include <Wt/WValidator.h>
#include <Wt/WLengthValidator.h>
#include <Wt/WIntValidator.h>

#include <Wt/WPushButton.h>
#include <Wt/WSpinBox.h>

#include <Ice/Ice.h>
#include <stdexcept>

ProfilePage::ProfilePage(std::shared_ptr<Login> login)
    : Wt::WTemplate(tr("profile-page-template")),
      login_(login)
{
    setStyleClass("profile");

    header_ = bindWidget("profile-header-widget", std::make_unique<Wt::WTemplate>(tr("profile-header-template")));
    menu_ = bindWidget("profile-menu-widget", std::make_unique<Wt::WTemplate>(tr("profile-menu-template")));
    stats_ = bindWidget("profile-stats-widget", std::make_unique<Wt::WTemplate>(tr("profile-stats-template")));
    description_ = bindWidget("profile-service-description-widget", std::make_unique<Wt::WTemplate>(tr("profile-service-description-template")));
    work_ = bindWidget("profile-service-work-widget", std::make_unique<Wt::WTemplate>(tr("profile-service-work-template")));
    settings_profile_menu_ = bindWidget("profile-settings-menu-widget", std::make_unique<Wt::WTemplate>(tr("profile-menu-settings-template")));

    settings_profile_ = bindWidget("profile-settings-widget", std::make_unique<ProfileSettings>(login_));
    setSettingsMenu();
    setProfileData();

    bindEmpty("dialog-widget");
    work_->hide();

    settings_profile_->hide();
    settings_profile_menu_->hide();
}

void ProfilePage::setProfileData()
{
    setServicesMenu();
    setWorkHistoryMenu();

    auto updateInfoBtn = description_->bindWidget("copy-button-widget", std::make_unique<Wt::WPushButton>("Edit Service"));
    updateInfoBtn->clicked().connect(this, &ProfilePage::modifyServiceDialog);

    header_->bindString("profile-name", Wt::WString(login_->user().userName));
    header_->bindEmpty("profile-link-one");
    header_->bindEmpty("profile-link-two");

    auto headerBtns = header_->bindWidget("settings-button-widget", std::make_unique<Wt::WTemplate>(tr("settings-button-template")));
    auto settingsBtn = headerBtns->bindWidget("settings-btn", std::make_unique<Wt::WPushButton>("Settings"));
    auto profileBtn = headerBtns->bindWidget("profile-btn", std::make_unique<Wt::WPushButton>("Profile"));
    // auto previewBtn = headerBtns->bindWidget("preview-profile-btn", std::make_unique<Wt::WPushButton>("Preview Profile"));
    headerBtns->bindEmpty("preview-profile-btn");

    settingsBtn->clicked().connect(this, [=]()
                                   {
                                       menu_->hide();
                                       stats_->hide();
                                       description_->hide();
                                       settings_profile_->show();
                                       settings_profile_menu_->show(); });

    profileBtn->clicked().connect(this, [=]()
                                  {
        menu_->show();
        stats_->show();
        description_->show();
        settings_profile_->hide();
        settings_profile_menu_->hide(); });
}

void ProfilePage::setServicesMenu()
{

    auto addServiceBtn = menu_->bindWidget("add-service-button", std::make_unique<Wt::WPushButton>("&#43;", Wt::TextFormat::XHTML));
    addServiceBtn->addStyleClass("btn-sm fs-5");
    addServiceBtn->clicked().connect(this, &ProfilePage::addServiceDialog);

    for (int i = 0; i <= 9; ++i)
    {
        auto serviceString = Wt::WString("services-nav-item-" + std::to_string(i));

        if (i >= login_->userServices_.size())
        {
            menu_->bindEmpty("services-nav-item-" + std::to_string(i));
            continue;
        }
        auto service = login_->userServices_.at(i);
        auto serviceNavItem = menu_->bindWidget(serviceString.toUTF8(), std::make_unique<Wt::WTemplate>(tr("profile-menu-nav-item-template")));
        serviceNavItem->setObjectName("services-" + Wt::WString(service.title).toUTF8());

        serviceNavItem->bindString("service-title", Wt::WString(service.title));
        if (i == 0)
        {
            serviceNavItem->addStyleClass("active");
            description_->bindString("service-title", Wt::WString(login_->userServices_.at(i).title));
            description_->bindString("service-description", Wt::WString(login_->userServices_.at(i).description));
        }

        serviceNavItem->clicked().connect([=]()
                                          {
                    if(description_->isHidden()){
                        description_->show();
                        work_->show();
                        settings_profile_->hide();
                    }

                    if(serviceNavItem->hasStyleClass("active")){
                        return;
                    }
                    for(int j = 0; j < login_->userServices_.size(); ++j){

                        auto serviceName = Wt::WString("services-" + Wt::WString(login_->userServices_.at(j).title));
                        auto serviceNavItem = menu_->find(serviceName.toUTF8());
                        if(serviceNavItem->hasStyleClass("active")){
                            serviceNavItem->removeStyleClass("active");
                        }
                    }
                    serviceNavItem->addStyleClass("active");
                    description_->bindString("service-title", Wt::WString(service.title));
                    description_->bindString("service-description", Wt::WString(service.description)); });
    }
}

void ProfilePage::addServiceDialog()
{
    auto dialog = bindWidget("dialog-widget", std::make_unique<Wt::WDialog>("Add Service"));

    auto title = dialog->contents()->addWidget(std::make_unique<Wt::WLineEdit>());
    auto description = dialog->contents()->addWidget(std::make_unique<Wt::WTextArea>());
    auto price = dialog->contents()->addWidget(std::make_unique<Wt::WSpinBox>());

    title->setValidator(std::make_unique<Wt::WLengthValidator>(3, 15));
    title->setPlaceholderText("Service Title");
    title->validator()->setMandatory(true);

    description->setValidator(std::make_unique<Wt::WLengthValidator>(3, 200));
    description->setPlaceholderText("Service Description");
    description->validator()->setMandatory(true);
    auto priceValidator = std::make_unique<Wt::WIntValidator>();
    priceValidator->setMandatory(true);
    priceValidator->setRange(0, 10000);

    price->setValidator(std::move(priceValidator));
    price->setPlaceholderText("Service Price RON");
    price->setMaximum(10000);
    price->setSingleStep(50);

    auto saveBtn = dialog->footer()->addWidget(std::make_unique<Wt::WPushButton>("Save"));
    auto cancelBtn = dialog->footer()->addWidget(std::make_unique<Wt::WPushButton>("Cancel"));

    saveBtn->clicked().connect(this, [=]()
                               {
                                        if(title->validate() != Wt::ValidationState::Valid ||
                                        description->validate() != Wt::ValidationState::Valid ||
                                        price->validate() != Wt::ValidationState::Valid){
                                            std::cout << "\n\n Fields not valid \n\n";
                                            return;
                                        }
                                        dialog->accept(); });

    cancelBtn->clicked().connect(this, [=]()
                                 { dialog->reject(); });
    dialog->rejectWhenEscapePressed();

    dialog->finished().connect([=]
                               { 
                                if(dialog->result() == Wt::DialogCode::Accepted)
                                    {
                                        AuthModule::ServiceInfo serviceInfo;
                                        serviceInfo.title = title->text().toUTF8();
                                        serviceInfo.description = description->text().toUTF8();
                                        serviceInfo.price = price->value();

                                        try
                                        {
                                            Ice::CommunicatorHolder ich = Ice::initialize();
                                            auto base = ich->stringToProxy(login_->authConnString_);
                                            auto authInterface = Ice::checkedCast<AuthModule::AuthInterfacePrx>(base);
                                            if (!authInterface)
                                            {
                                                throw std::runtime_error("Invalid proxy");
                                            }
                                            authInterface->addUserService(login_->userToken(), serviceInfo);
                                            setServicesMenu();
                                        }
                                        catch (const std::exception &e)
                                        {
                                            std::cerr << e.what() << std::endl;
                                        }

                                    }else {
                                        std::cout << "\n\n Rejected \n\n";
                                    }

                                    bindEmpty("dialog-widget"); });

    dialog->show();
}

void ProfilePage::modifyServiceDialog()
{
    AuthModule::ServiceInfo serviceInfo;
    serviceInfo.title = description_->resolveStringValue("service-title").toUTF8();

    // iterate over login_->userServices_ and find the service with the same title
    for (auto service : login_->userServices_)
    {

        if (Wt::WString(service.title) == Wt::WString(serviceInfo.title))
        {
            serviceInfo.id = service.id;
            serviceInfo.description = service.description;
            serviceInfo.price = service.price;
        }
    }

    auto dialog = bindWidget("dialog-widget", std::make_unique<Wt::WDialog>("Modify Service Details"));

    auto title = dialog->contents()->addWidget(std::make_unique<Wt::WLineEdit>());
    auto description = dialog->contents()->addWidget(std::make_unique<Wt::WTextArea>());
    auto price = dialog->contents()->addWidget(std::make_unique<Wt::WSpinBox>());

    title->setText(serviceInfo.title);
    description->setText(serviceInfo.description);
    price->setValue(serviceInfo.price);

    title->setValidator(std::make_unique<Wt::WLengthValidator>(3, 15));
    title->setPlaceholderText("Service Title");
    title->validator()->setMandatory(true);

    description->setValidator(std::make_unique<Wt::WLengthValidator>(3, 200));
    description->setPlaceholderText("Service Description");
    description->validator()->setMandatory(true);
    auto priceValidator = std::make_unique<Wt::WIntValidator>();
    priceValidator->setMandatory(true);
    priceValidator->setRange(0, 10000);

    price->setValidator(std::move(priceValidator));
    price->setPlaceholderText("Service Price RON");
    price->setMaximum(10000);
    price->setSingleStep(50);

    auto saveBtn = dialog->footer()->addWidget(std::make_unique<Wt::WPushButton>("Save"));
    auto cancelBtn = dialog->footer()->addWidget(std::make_unique<Wt::WPushButton>("Cancel"));
    auto deleteBtn = dialog->footer()->addWidget(std::make_unique<Wt::WPushButton>("Delete"));

    saveBtn->clicked().connect(this, [=]()
                               {
                                        if(title->validate() != Wt::ValidationState::Valid ||
                                        description->validate() != Wt::ValidationState::Valid ||
                                        price->validate() != Wt::ValidationState::Valid){
                                            std::cout << "\n\n Fields not valid \n\n";
                                            return;
                                        }
                                        dialog->accept(); });

    cancelBtn->clicked().connect(this, [=]()
                                 { dialog->reject(); });
    dialog->rejectWhenEscapePressed();

    deleteBtn->clicked().connect(this, [=]()
                                 {
                                        try
                                        {
                                            Ice::CommunicatorHolder ich = Ice::initialize();
                                            auto base = ich->stringToProxy(login_->authConnString_);
                                            auto authInterface = Ice::checkedCast<AuthModule::AuthInterfacePrx>(base);
                                            if (!authInterface)
                                            {
                                                throw std::runtime_error("Invalid proxy");
                                            }
                                            authInterface->removeUserService(login_->userToken(), serviceInfo.id);
                                            login_->getUserServices();
                                            setServicesMenu();
                                            dialog->reject(); 
                                        }
                                        catch (const std::exception &e)
                                        {
                                            std::cerr << e.what() << std::endl;
                                        } });

    dialog->finished().connect([=]()
                               { 
                                if(dialog->result() == Wt::DialogCode::Accepted)
                                    {
                                        
                                        auto newServiceInfo = serviceInfo;
                                        newServiceInfo.title = title->text().toUTF8();
                                        newServiceInfo.description = description->text().toUTF8();
                                        newServiceInfo.price = price->value();


                                        try
                                        {
                                            Ice::CommunicatorHolder ich = Ice::initialize();
                                            auto base = ich->stringToProxy(login_->authConnString_);
                                            auto authInterface = Ice::checkedCast<AuthModule::AuthInterfacePrx>(base);
                                            if (!authInterface)
                                            {
                                                throw std::runtime_error("Invalid proxy");
                                            }
                                            authInterface->updateUserService(login_->userToken(), newServiceInfo);
                                            login_->getUserServices();
                                            setServicesMenu();
                                            }
                                        catch (const std::exception &e)
                                        {
                                            std::cerr << e.what() << std::endl;
                                        }
                                    }else {
                                        std::cout << "\n\n Rejected \n\n";
                                    }
                                    bindEmpty("dialog-widget"); });
    dialog->show();
}

void ProfilePage::setSettingsMenu()
{
    settings_profile_menu_->bindString("menu-title", Wt::WString("Settings"));
    auto settingsProfile = settings_profile_menu_->bindWidget("settings-profile-nav-item", std::make_unique<Wt::WTemplate>(tr("profile-menu-nav-item-template")));
    auto settingsOther = settings_profile_menu_->bindWidget("settings-other-nav-item", std::make_unique<Wt::WTemplate>(tr("profile-menu-nav-item-template")));

    settingsProfile->bindString("service-title", Wt::WString("Profile"));
    settingsOther->bindString("service-title", Wt::WString("Other"));

    settingsProfile->clicked().connect([=]()
                                       {
        if(settingsProfile->hasStyleClass("active")){
            return;
        }else {
            settingsProfile->addStyleClass("active");
            settingsOther->removeStyleClass("active");
            settings_profile_->profile_->show();
            settings_profile_->other_->hide();
        } });

    settingsOther->clicked().connect([=]()
                                     {
        if(settingsOther->hasStyleClass("active")){
            return;
        }else {
            settingsOther->addStyleClass("active");
            settingsProfile->removeStyleClass("active");
            settings_profile_->profile_->hide();
            settings_profile_->other_->show();
        } });
}

void ProfilePage::setWorkHistoryMenu()
{
    auto photos = work_->bindWidget("work-nav-item-photos", std::make_unique<Wt::WTemplate>(tr("work-nav-item-template")));
    photos->bindString("nav-item-title", Wt::WString("Photos(42)"));

    auto videos = work_->bindWidget("work-nav-item-videos", std::make_unique<Wt::WTemplate>(tr("work-nav-item-template")));
    videos->bindString("nav-item-title", Wt::WString("Videos(5)"));

    auto reviews = work_->bindWidget("work-nav-item-reviews", std::make_unique<Wt::WTemplate>(tr("work-nav-item-template")));
    reviews->bindString("nav-item-title", Wt::WString("Reviews(72)"));

    photos->clicked().connect([=]()
                              {
        if(photos->hasStyleClass("active")){
            return;
        }
        if(videos->hasStyleClass("active")){
            videos->removeStyleClass("active");
        }
        if(reviews->hasStyleClass("active")){
            reviews->removeStyleClass("active");
        }
        photos->addStyleClass("active"); });
    videos->clicked().connect([=]()
                              {
        if(videos->hasStyleClass("active")){
            return;
        }
        if(photos->hasStyleClass("active")){
            photos->removeStyleClass("active");
        }
        if(reviews->hasStyleClass("active")){
            reviews->removeStyleClass("active");
        }
        videos->addStyleClass("active"); });
    reviews->clicked().connect([=]()
                               {
        if(reviews->hasStyleClass("active")){
            return;
        }
        if(photos->hasStyleClass("active")){
            photos->removeStyleClass("active");
        }
        if(videos->hasStyleClass("active")){
            videos->removeStyleClass("active");
        }
        reviews->addStyleClass("active"); });
}