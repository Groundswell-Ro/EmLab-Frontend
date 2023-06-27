#pragma once
#include <Wt/WTemplate.h>

enum DisplayType {
    START,
    DURATION,
    END,
};

class TimeInput : public Wt::WTemplate
{
    public:
        TimeInput(DisplayType displayType, std::string tempName = "time-input");
        ~TimeInput();
        std::string getValue();
        void setValue(std::string value);
        void hideButtons();
    private:
        Wt::WText* display_;
};
