#include "include/HomePage.h"
#include <Wt/WText.h>
#include <Wt/WTemplate.h>
#include <Wt/WPushButton.h>

HomePage::HomePage()
    : WContainerWidget()
{
	// setStyleClass("bg-basic");
    addWidget(std::make_unique<Wt::WText>("<div class='text-current'>HomePage</div>"));
    
    auto btn_Wrapper = addWidget(std::make_unique<Wt::WContainerWidget>());
	btn_Wrapper->addStyleClass("block");
	Wt::WString btn_wrapper_styles = "flex flex-wrap";
	// DEFAULT
	auto defaultBtn_Wrapper = btn_Wrapper->addWidget(std::make_unique<Wt::WContainerWidget>());
	defaultBtn_Wrapper->addStyleClass(btn_wrapper_styles);
	defaultBtn_Wrapper->addWidget(std::make_unique<Wt::WPushButton>("light"))->setStyleClass("btn btn-light-default");
	defaultBtn_Wrapper->addWidget(std::make_unique<Wt::WPushButton>("dark"))->setStyleClass("btn btn-dark-default");
	auto default_disabled = defaultBtn_Wrapper->addWidget(std::make_unique<Wt::WPushButton>("disabled"));
	default_disabled->setStyleClass("btn");
	default_disabled->setDisabled(true);
	defaultBtn_Wrapper->addWidget(std::make_unique<Wt::WPushButton>("simple"))->setStyleClass("btn");
	
	// PRIMARY
	
	auto primaryBtn_Wrapper = btn_Wrapper->addWidget(std::make_unique<Wt::WContainerWidget>());
	primaryBtn_Wrapper->addStyleClass(btn_wrapper_styles);
	primaryBtn_Wrapper->addWidget(std::make_unique<Wt::WPushButton>("light"))->setStyleClass("btn btn-primary-light");
	primaryBtn_Wrapper->addWidget(std::make_unique<Wt::WPushButton>("dark"))->setStyleClass("btn btn-primary-dark");
	auto primary_disabled = primaryBtn_Wrapper->addWidget(std::make_unique<Wt::WPushButton>("disabled"));
	primary_disabled->setStyleClass("btn btn-primary");
	primary_disabled->setDisabled(true);
	primaryBtn_Wrapper->addWidget(std::make_unique<Wt::WPushButton>("primary"))->setStyleClass("btn btn-primary");


	// SECCONDARY
	auto secondaryBtn_Wrapper = btn_Wrapper->addWidget(std::make_unique<Wt::WContainerWidget>());
	secondaryBtn_Wrapper->addStyleClass(btn_wrapper_styles);
	secondaryBtn_Wrapper->addWidget(std::make_unique<Wt::WPushButton>("light"))->setStyleClass("btn btn-seccondary-light");
	secondaryBtn_Wrapper->addWidget(std::make_unique<Wt::WPushButton>("dark"))->setStyleClass("btn btn-seccondary-dark");
	auto seccondary_disabled = secondaryBtn_Wrapper->addWidget(std::make_unique<Wt::WPushButton>("disabled"));
	seccondary_disabled->setStyleClass("btn btn-secondary");
	seccondary_disabled->setDisabled(true);
	secondaryBtn_Wrapper->addWidget(std::make_unique<Wt::WPushButton>("seccondary"))->setStyleClass("btn btn-seccondary");

	// SUCCESS
	auto successBtn_Wrapper = btn_Wrapper->addWidget(std::make_unique<Wt::WContainerWidget>());
	successBtn_Wrapper->addStyleClass(btn_wrapper_styles);
	successBtn_Wrapper->addWidget(std::make_unique<Wt::WPushButton>("light"))->setStyleClass("btn btn-success-light");
	successBtn_Wrapper->addWidget(std::make_unique<Wt::WPushButton>("dark"))->setStyleClass("btn btn-success-dark");
	auto success_disabled = successBtn_Wrapper->addWidget(std::make_unique<Wt::WPushButton>("disabled"));
	success_disabled->setStyleClass("btn btn-success");
	success_disabled->setDisabled(true);
	successBtn_Wrapper->addWidget(std::make_unique<Wt::WPushButton>("success"))->setStyleClass("btn btn-success");

	// DANGER
	auto dangerBtn_Wrapper = btn_Wrapper->addWidget(std::make_unique<Wt::WContainerWidget>());
	dangerBtn_Wrapper->addStyleClass(btn_wrapper_styles);
	dangerBtn_Wrapper->addWidget(std::make_unique<Wt::WPushButton>("light"))->setStyleClass("btn btn-danger-light");
	dangerBtn_Wrapper->addWidget(std::make_unique<Wt::WPushButton>("dark"))->setStyleClass("btn btn-danger-dark");
	auto danger_disabled = dangerBtn_Wrapper->addWidget(std::make_unique<Wt::WPushButton>("disabled"));
	danger_disabled->setStyleClass("btn btn-danger");
	danger_disabled->setDisabled(true);
	dangerBtn_Wrapper->addWidget(std::make_unique<Wt::WPushButton>("danger"))->setStyleClass("btn btn-danger");

	// WARNING
	auto warningBtn_Wrapper = btn_Wrapper->addWidget(std::make_unique<Wt::WContainerWidget>());
	warningBtn_Wrapper->addStyleClass(btn_wrapper_styles);
	warningBtn_Wrapper->addWidget(std::make_unique<Wt::WPushButton>("light"))->setStyleClass("btn btn-warning-light");
	warningBtn_Wrapper->addWidget(std::make_unique<Wt::WPushButton>("dark"))->setStyleClass("btn btn-warning-dark");
	auto warning_disabled = warningBtn_Wrapper->addWidget(std::make_unique<Wt::WPushButton>("disabled"));
	warning_disabled->setStyleClass("btn btn-warning");
	warning_disabled->setDisabled(true);
	warningBtn_Wrapper->addWidget(std::make_unique<Wt::WPushButton>("warning"))->setStyleClass("btn btn-warning");

	// INFO
	auto infoBtn_Wrapper = btn_Wrapper->addWidget(std::make_unique<Wt::WContainerWidget>());
	infoBtn_Wrapper->addStyleClass(btn_wrapper_styles);
	infoBtn_Wrapper->addWidget(std::make_unique<Wt::WPushButton>("light"))->setStyleClass("btn btn-info-light");
	infoBtn_Wrapper->addWidget(std::make_unique<Wt::WPushButton>("dark"))->setStyleClass("btn btn-info-dark");
	auto info_disabled = infoBtn_Wrapper->addWidget(std::make_unique<Wt::WPushButton>("disabled"));
	info_disabled->setStyleClass("btn btn-info");
	info_disabled->setDisabled(true);
	infoBtn_Wrapper->addWidget(std::make_unique<Wt::WPushButton>("info"))->setStyleClass("btn btn-info");

	// DARK
	auto darkBtn_Wrapper = btn_Wrapper->addWidget(std::make_unique<Wt::WContainerWidget>());
	darkBtn_Wrapper->addStyleClass(btn_wrapper_styles);
	darkBtn_Wrapper->addWidget(std::make_unique<Wt::WPushButton>("light"))->setStyleClass("btn btn-dark-light");
	darkBtn_Wrapper->addWidget(std::make_unique<Wt::WPushButton>("dark"))->setStyleClass("btn btn-dark-dark");
	auto dark_disabled = darkBtn_Wrapper->addWidget(std::make_unique<Wt::WPushButton>("disabled"));
	dark_disabled->setStyleClass("btn btn-dark");
	dark_disabled->setDisabled(true);
	darkBtn_Wrapper->addWidget(std::make_unique<Wt::WPushButton>("dark"))->setStyleClass("btn btn-dark");
	


}