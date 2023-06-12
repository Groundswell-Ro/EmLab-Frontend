#pragma once
#include "LoginObj.h"
#include "Utils.h"
#include "../../../comunication/EventDataModule.h"

#include <Wt/WFormModel.h>
#include <Wt/WTemplateFormView.h>

#include <Wt/WLineEdit.h>
#include <Wt/WTimeEdit.h>
#include <Wt/WDateEdit.h>
#include <Wt/WPushButton.h>
#include <Wt/WDoubleSpinBox.h>
#include <Wt/WTextArea.h>

// Validators
#include <Wt/WValidator.h>
#include <Wt/WLengthValidator.h> // for Name
#include <Wt/WDateValidator.h>   // for Date
#include <Wt/WDoubleValidator.h> // for Duration
#include <Wt/WTimeValidator.h>   // for Start hour

#include <Ice/Ice.h>
#include <stdexcept>

// E V E N T _____________ M O D E L
class EventFormModel : public Wt::WFormModel
{
public:

    // Associate each field with a unique string literal.
    static const Field EventDateField;
    static const Field EventStartField;
    static const Field EventDurationField;
    static const Field EventLocationField;
    static const Field EventObservationsField;
    int id = 0;

    EventFormModel();

    EventDataModule::EventData getData();

    std::string getDateTime();

private:
    std::shared_ptr<Wt::WValidator> createDateValidator();
    std::shared_ptr<Wt::WValidator> createTimeValidator();
    std::shared_ptr<Wt::WValidator> createDoubleValidator(double min, double max);
    std::shared_ptr<Wt::WValidator> createTextValidator(bool mandatory, int min, int max);
};

// E V E N T _____________ V I E W
class EventFormView : public Wt::WTemplateFormView
{
public:
    EventFormView(std::shared_ptr<Login> login);

    std::shared_ptr<Login> login_;
    std::shared_ptr<EventFormModel> model_;

    Wt::WDateEdit *eventDate_;
    Wt::WTimeEdit *eventStart_;
    Wt::WDoubleSpinBox *eventDuration_;
    Wt::WLineEdit *eventLocation_;
    Wt::WTextArea *eventObservations_;

    void setData(EventDataModule::EventData eventData);

    void setReadOnlyAll(bool readOnly = true);
    void changeEventData(EventDataModule::EventField field);
    void confirmEventData(EventDataModule::EventField field);

    Wt::Signal<Wt::WDate, int> &dateChanged() { return dateChanged_; };

    bool validate();
    bool firstSetData_ = true;

private:
    Wt::Signal<Wt::WDate, int> dateChanged_;
};
