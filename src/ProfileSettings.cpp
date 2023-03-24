#include "include/ProfileSettings.h"
#include "include/ChangePasswordForm.h"
#include <Wt/WDialog.h>

ProfileSettings::ProfileSettings(std::shared_ptr<Login> login)
    : Wt::WTemplate(tr("profile-settings-widget-template")),
      login_(login)
{
    bindEmpty("profile-settings-dialog");

    profile_ = bindWidget("profile-settings-widget", 
        std::make_unique<Wt::WTemplate>(tr("profile-settings-template")));

    other_ = bindWidget("profile-other-widget",
        std::make_unique<Wt::WTemplate>(tr("profile-settings-template")));
    setProfileMenu();
    setOtherMenu();

    other_->hide();
}

void ProfileSettings::setProfileMenu()
{
    // binding templates to the menu
    profile_->bindString("settings-title", Wt::WString("Profile Settings"));

    // bind templates to profile_ template widget
    auto userDataMenuItem = profile_->bindWidget("settings-nav-item-submenu-1", 
        std::make_unique<Wt::WTemplate>(tr("settings-nav-item-template"))); 
    auto seccondMenuItem = profile_->bindWidget("settings-nav-item-submenu-2", 
        std::make_unique<Wt::WTemplate>(tr("settings-nav-item-template")));
     
    auto changePasswordWidget = profile_->bindWidget("change-password-widget", 
        std::make_unique<ChangePasswordFormView>(login_));

    auto subscriptionPreview = profile_->bindWidget("subscription-preview-widget", 
        std::make_unique<Wt::WTemplate>(tr("subscription-preview-template")));
    
    subscriptionPreview->bindWidget("free-plan", std::make_unique<Wt::WText>("Free Plan"))->addStyleClass("active");
    subscriptionPreview->bindWidget("monthly-plan", std::make_unique<Wt::WText>("Monthly Plan"));
    subscriptionPreview->bindWidget("yearly-plan", std::make_unique<Wt::WText>("Yearly Plan"));

    auto changeSubsriptionButton = subscriptionPreview->bindWidget("change-subscription-button", 
        std::make_unique<Wt::WPushButton>("Change Plan"));

    changeSubsriptionButton->clicked().connect([=](){
        addSubscriptionDialog();
    });


    // setting title of the menu
    userDataMenuItem->bindString("settings-item-title", Wt::WString("User Data"));

 
    // react to the click event
    userDataMenuItem->clicked().connect([=](){
        if(userDataMenuItem->hasStyleClass("active")){
            return;
        }else {
            userDataMenuItem->addStyleClass("active");
            seccondMenuItem->removeStyleClass("active");
            changePasswordWidget->show();
        }
    });
    seccondMenuItem->clicked().connect([=](){
        if(seccondMenuItem->hasStyleClass("active")){
            return;
        }else {
            seccondMenuItem->addStyleClass("active");
            userDataMenuItem->removeStyleClass("active");
            changePasswordWidget->hide();
        }
    });

}

void ProfileSettings::addSubscriptionDialog()
{
    auto dialog = bindWidget("profile-settings-dialog", 
        std::make_unique<Wt::WDialog>("Change Subscription"));
    dialog->setMinimumSize(Wt::WLength(98, Wt::LengthUnit::ViewportWidth), Wt::WLength(98, Wt::LengthUnit::ViewportHeight));
    dialog->setMaximumSize(Wt::WLength(98, Wt::LengthUnit::ViewportWidth), Wt::WLength(98, Wt::LengthUnit::ViewportHeight));

    dialog->setOffsets(Wt::WLength(1, Wt::LengthUnit::ViewportWidth), Wt::Side::Left);
    dialog->setOffsets(Wt::WLength(1, Wt::LengthUnit::ViewportWidth), Wt::Side::Right);
    dialog->setOffsets(Wt::WLength(1, Wt::LengthUnit::ViewportHeight), Wt::Side::Top);
    dialog->setOffsets(Wt::WLength(1, Wt::LengthUnit::ViewportHeight), Wt::Side::Bottom);
    dialog->setMovable(false);
    dialog->titleBar()->addStyleClass("d-flex justify-content-between");
    dialog->contents()->addStyleClass("overflow-auto");

    auto closeBtn = dialog->titleBar()->insertBefore(std::make_unique<Wt::WPushButton>("esc"), dialog->titleBar()->children().at(0));
    closeBtn->addStyleClass("btn btn-outline-danger");
    dialog->titleBar()->addWidget(std::make_unique<Wt::WText>(""));
    
    closeBtn->clicked().connect([=](){
        dialog->reject();
    });
    dialog->rejectWhenEscapePressed();
    auto subscriptionPlans = dialog->contents()->addWidget(std::make_unique<Wt::WTemplate>(tr("subscription-payment-dialog-template")));
    subscriptionPlans->addStyleClass("w-100 d-flex justify-content-between");

    auto freePlan = subscriptionPlans->bindWidget("pricing-plan-free", 
        std::make_unique<Wt::WTemplate>(tr("pricing-plan-template")));

    freePlan->bindString("title", "Free Plan");
    freePlan->bindString("price", "0 Ron");
    freePlan->bindString("option-1", "Profile Page");
    freePlan->bindString("option-2", "Events basic view");
    freePlan->bindString("option-3", "Calendar");

    auto freePlanBtn = freePlan->bindWidget("plan-btn", std::make_unique<Wt::WPushButton>("Active"));
    freePlanBtn->addStyleClass("active");
    // freePlanBtn->setThemeStyleEnabled(false);
    freePlanBtn->setDisabled(true);

    auto monthlyPlan = subscriptionPlans->bindWidget("pricing-plan-monthly", 
        std::make_unique<Wt::WTemplate>(tr("pricing-plan-template")));

    monthlyPlan->bindString("title", "Monthly Plan");
    monthlyPlan->bindString("price", "10 Ron");
    monthlyPlan->bindString("option-1", "Profile Page");
    monthlyPlan->bindString("option-2", "Events basic view");
    monthlyPlan->bindString("option-3", "Calendar");

    auto monthlyPlanBtn = monthlyPlan->bindWidget("plan-btn", std::make_unique<Wt::WPushButton>("Select"));
    monthlyPlanBtn->setThemeStyleEnabled(false);
    
    auto yearlyPlan = subscriptionPlans->bindWidget("pricing-plan-yearly",
        std::make_unique<Wt::WTemplate>(tr("pricing-plan-template")));

    yearlyPlan->bindString("title", "Yearly Plan");
    yearlyPlan->bindString("price", "100 Ron");
    yearlyPlan->bindString("option-1", "Profile Page");
    yearlyPlan->bindString("option-2", "Events basic view");
    yearlyPlan->bindString("option-3", "Calendar");

    auto yearlyPlanBtn = yearlyPlan->bindWidget("plan-btn", std::make_unique<Wt::WPushButton>("Select"));
    yearlyPlanBtn->setThemeStyleEnabled(false);
    
    dialog->finished().connect([=](){
        bindEmpty("profile-settings-dialog");
    });

        dialog->show();
}

void ProfileSettings::setOtherMenu()
{
    // binding templates to the menu
    other_->bindString("settings-title", Wt::WString("Other Settings"));

    // bind templates to other_ template widget
    auto otherMenuItem_1 = other_->bindWidget("settings-nav-item-submenu-1", 
        std::make_unique<Wt::WTemplate>(tr("settings-nav-item-template")));


    // setting title of the menu
    otherMenuItem_1->bindString("settings-item-title", Wt::WString("menu one"));


}

