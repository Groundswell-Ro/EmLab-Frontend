#pragma once
#include "Calendar.h"
#include "LoginObj.h"

#include <Wt/WContainerWidget.h>
#include <Wt/WTemplate.h>
#include <Wt/WLineEdit.h>
#include <Wt/WString.h>
#include <Wt/WPushButton.h>
#include <Wt/WTimeEdit.h>
#include <Wt/WDateTime.h>
#include <Wt/WDoubleSpinBox.h>
#include <Wt/WTextArea.h>
#include <Wt/WText.h>
#include <Wt/WTemplate.h>
#include <Wt/WPanel.h>
#include <Wt/WAnimation.h>
#include <Wt/WMenu.h>
#include <Wt/WStackedWidget.h>
#include <Ice/Ice.h>
#include <stdexcept>
class HomePage : public Wt::WContainerWidget
{
public:
    HomePage(std::shared_ptr<Login> login);
private:

    Wt::WTemplate*      profile_;
    Wt::WTemplate*      sidebar_;
    Wt::WTemplate*      sidebar_content_;

    Calendar*           calendar_;
    Wt::WText*          selected_date_;

    Wt::WPushButton*    add_event_btn_;
    Wt::WPushButton*    save_event_btn_;
    Wt::WPushButton*    cancel_event_btn_;
    
    Wt::WTimeEdit*      start_time_input_;
    Wt::WLineEdit*      duration_input_;
    Wt::WLineEdit*      location_input_;
    Wt::WTextArea*      description_input_;
    int event_duration = 60;


    void createProfile();
    void createSidebar();

    void calendarClicked(const Wt::WDate date);

    void addEvent();
    void saveEvent();
    void cancelEvent();
    
    std::shared_ptr<Login> login_;

    std::string time_format_ = "HH:mm AP"; 
    std::string date_format_ = "dd/MMM/yyyy";
    std::string datetime_format_ = date_format_ + " " + time_format_;
};
