#include "include/TimeInput.h"
#include <Wt/WPushButton.h>
#include <Wt/WText.h>

TimeInput::TimeInput(DisplayType displayType, std::string tempName) 
    : Wt::WTemplate(tr(tempName))
{
    auto sub_btn = bindWidget("sub-btn", std::make_unique<Wt::WPushButton>("-"));
    display_ = bindWidget("display", std::make_unique<Wt::WText>(""));
    auto add_btn = bindWidget("add-btn", std::make_unique<Wt::WPushButton>("+"));

    if(displayType == DisplayType::START) {
        std::cout << "\n\n time input \n\n";
        bindWidget("label", std::make_unique<Wt::WText>("start"));
        bindWidget("svg", std::make_unique<Wt::WTemplate>(tr("clock-svg-sm")));
    }else if(displayType == DisplayType::DURATION) {
        std::cout << "\n\n duration input \n\n";
        bindWidget("label", std::make_unique<Wt::WText>("duration"));
        bindWidget("svg", std::make_unique<Wt::WTemplate>(tr("hourglass-svg-sm")));
    }else if(displayType == DisplayType::END) {
        std::cout << "\n\n end input \n\n";
        bindWidget("label", std::make_unique<Wt::WText>("end"));
        bindWidget("svg", std::make_unique<Wt::WTemplate>(tr("clock-svg-sm")));
    }

}

TimeInput::~TimeInput()
{
}

std::string TimeInput::getValue()
{
    return display_->text().toUTF8();
}

void TimeInput::setValue(std::string value)
{
    display_->setText(value);
}

void TimeInput::hideButtons()
{
    resolveWidget("sub-btn")->hide();
    resolveWidget("add-btn")->hide();
}
