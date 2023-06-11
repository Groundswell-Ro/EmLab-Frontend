#pragma once
#include "Calendar.h"
#include <Wt/WContainerWidget.h>
#include <Wt/WTemplate.h>
#include <Wt/WLineEdit.h>
#include <Wt/WString.h>

class HomePage : public Wt::WContainerWidget
{
public:
    HomePage();
    Wt::WTemplate* profile_;
private:
    Wt::WTemplate *sidebar_;
    void createProfile();
    void createSidebar();

    void createButtonsExampla();
    Calendar* calendar_;
    Wt::WLineEdit* start_time_;
    Wt::WLineEdit* end_time_;
    Wt::WString time_format_ = "hh:mm AP";
    Wt::WLineEdit* createTimeEdit(Wt::WString comp_string);
};
