#pragma once
#include "../../utils/include/Calendar.h"
#include "../../utils/include/LoginObj.h"
#include "../../utils/include/TimeInput.h"

#include <Wt/WContainerWidget.h>
#include <Wt/WTemplate.h>
#include <Wt/WPushButton.h>
#include <Wt/WLineEdit.h>
#include <Wt/WTimeEdit.h>
#include <Wt/WTextArea.h>
#include <Wt/WDate.h>
#include <Wt/WTime.h>
class PortofoliosPage : public Wt::WContainerWidget
{
public:
    PortofoliosPage(std::shared_ptr<Login> login);
private:

    Wt::WTemplate*      profile_;
    Wt::WTemplate*      sidebar_;
    Wt::WTemplate*      sidebar_content_;

    Calendar*           calendar_;
    Wt::WText*          selected_date_;

    Wt::WPushButton*    add_event_btn_;
    Wt::WPushButton*    save_event_btn_;
    Wt::WPushButton*    cancel_event_btn_;
    
    TimeInput*          start_input_;
    TimeInput*          duration_input_;
    TimeInput*          end_input_;

    Wt::WLineEdit*      location_input_;
    Wt::WTextArea*      description_input_;
    int event_duration = 60;

    Wt::WDate event_date = Wt::WDate::currentDate();
    Wt::WTime event_time = Wt::WTime(12,0,0);

    void createSidebar();

    void calendarClicked(const Wt::WDate date);

    void addEvent();
    void saveEvent();
    void cancelEvent();
    
    std::shared_ptr<Login> login_;
};
