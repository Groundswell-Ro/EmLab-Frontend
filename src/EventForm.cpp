#include "include/EventForm.h"

#include <Ice/Ice.h>
#include <stdexcept>

// Validators
#include <Wt/WLengthValidator.h> // for Name
#include <Wt/WDateValidator.h>   // for Date
#include <Wt/WDoubleValidator.h> // for Duration
#include <Wt/WTimeValidator.h>   // for Start hour

std::shared_ptr<Wt::WValidator> EventFormModel::createDateValidator()
{
    auto v = std::make_shared<Wt::WDateValidator>();
    v->setMandatory(true);
    // v->setBottom(Wt::WDate::currentDate());
    // v->setTop(Wt::WDate::currentDate().addMonths(5));
    v->setFormat("dd/MM/yyyy");
    v->setMandatory(true);
    return v;
}

std::shared_ptr<Wt::WValidator> EventFormModel::createTimeValidator()
{
    auto v = std::make_shared<Wt::WTimeValidator>("HH:mm AP");
    v->setMandatory(true);
    return v;
}

std::shared_ptr<Wt::WValidator> EventFormModel::createDoubleValidator(double min, double max)
{
    auto v = std::make_shared<Wt::WDoubleValidator>();
    v->setMandatory(true);
    v->setTop(max);
    v->setBottom(min);
    return v;
}

std::shared_ptr<Wt::WValidator> EventFormModel::createTextValidator(bool mandatory, int min, int max)
{
    auto v = std::make_shared<Wt::WLengthValidator>();
    v->setMandatory(mandatory);
    v->setMinimumLength(min);
    v->setMaximumLength(max);
    return v;
}

// Event fields
const Wt::WFormModel::Field EventFormModel::EventDateField = "event-date";
const Wt::WFormModel::Field EventFormModel::EventStartField = "event-start";
const Wt::WFormModel::Field EventFormModel::EventDurationField = "event-duration";
const Wt::WFormModel::Field EventFormModel::EventLocationField = "event-location";
const Wt::WFormModel::Field EventFormModel::EventObservationsField = "event-observations";

// E V E N T _____________ M O D E L
EventFormModel::EventFormModel()
    : WFormModel()
{
    addField(EventDateField);
    addField(EventLocationField);
    addField(EventStartField);
    addField(EventDurationField);
    addField(EventObservationsField);

    setValidator(EventDateField, createDateValidator());
    setValidator(EventStartField, createTimeValidator()); // "h:mm AP"
    setValidator(EventDurationField, createDoubleValidator(1, 10.0));
    setValidator(EventLocationField, createTextValidator(false, 0, 100));
    setValidator(EventObservationsField, createTextValidator(false, 1, 300));

    setValue(EventDateField, Wt::WDate::currentServerDate().addDays(0));
    setValue(EventDurationField, "1");
    setValue(EventStartField, "12:00 PM");
}

EventDataModule::EventData EventFormModel::getData()
{
    EventDataModule::EventData eventData;

    eventData.id = id;
    eventData.dateTime = getDateTime();
    eventData.duration = ::atof(valueText(EventDurationField).toUTF8().c_str());
    eventData.location = valueText(EventLocationField).toUTF8();
    eventData.observations = valueText(EventObservationsField).toUTF8();

    return eventData;
}

std::string EventFormModel::getDateTime()
{

    auto date = Wt::WDate().fromString(valueText(EventDateField), dateFormat_);
    auto time = Wt::WTime().fromString(valueText(EventStartField), timeFormat_);
    Wt::WDateTime dateTime{date, time};

    return dateTime.toString(dateTimeFormat_).toUTF8();
}

// E V E N T _____________ V I E W
EventFormView::EventFormView(std::shared_ptr<Login> login)
    : login_(login)
{
    setMinimumSize(Wt::WLength(280, Wt::LengthUnit::Pixel), Wt::WLength::Auto);
    setMaximumSize(Wt::WLength(350, Wt::LengthUnit::Pixel), Wt::WLength::Auto);

    model_ = std::make_shared<EventFormModel>();
    setTemplateText(tr("event-form-template"));
    addFunction("id", &WTemplate::Functions::id);

    /*
     * Event Date Widget
     */
    auto eventDateWidget = std::make_unique<Wt::WDateEdit>();
    eventDateWidget->setReadOnly(true);
    eventDateWidget->setPlaceholderText("this is needed for input desighn animation");
    eventDateWidget->setFormat(model_->dateFormat_);
    eventDate_ = eventDateWidget.get();
    setFormWidget(model_->EventDateField, std::move(eventDateWidget));

    /*
     * Event Starting Time Widget
     */
    auto eventStartWidget = std::make_unique<Wt::WTimeEdit>();
    eventStartWidget->setMinuteStep(15);
    eventStartWidget->setTime(Wt::WTime(12, 0, 0));
    eventStartWidget->setFormat(model_->timeFormat_);
    eventStartWidget->setReadOnly(true);
    eventStart_ = eventStartWidget.get();
    setFormWidget(model_->EventStartField, std::move(eventStartWidget));

    /*
     * Event Duration Widget
     */
    auto eventDurationWidget = std::make_unique<Wt::WDoubleSpinBox>();
    eventDurationWidget->setDecimals(1);
    eventDurationWidget->setSingleStep(0.5);
    eventDuration_ = eventDurationWidget.get();
    setFormWidget(model_->EventDurationField, std::move(eventDurationWidget));

    /*
     * Event Location Widget
     */
    auto eventLocationWidget = std::make_unique<Wt::WLineEdit>();
    eventLocationWidget->setPlaceholderText("this is needed for input desighn animation");
    eventLocation_ = eventLocationWidget.get();
    setFormWidget(model_->EventLocationField, std::move(eventLocationWidget));

    /*
     *  Observations Textarea Widget
     */
    auto observationsWidget = std::make_unique<Wt::WTextArea>();
    eventObservations_ = observationsWidget.get();
    setFormWidget(model_->EventObservationsField, std::move(observationsWidget));
    bindString("submit-info", Wt::WString(""));

    changeDateBtn_ = bindWidget("event-date-btn-change", std::make_unique<Wt::WPushButton>("x"));
    changeStartBtn_ = bindWidget("event-start-btn-change", std::make_unique<Wt::WPushButton>("x"));
    changeDurationBtn_ = bindWidget("event-duration-btn-change", std::make_unique<Wt::WPushButton>("x"));
    changeLocationBtn_ = bindWidget("event-location-btn-change", std::make_unique<Wt::WPushButton>("x"));
    changeObservationsBtn_ = bindWidget("event-observations-btn-change", std::make_unique<Wt::WPushButton>("x"));

    confirmDateBtn_ = bindWidget("event-date-btn-confirm", std::make_unique<Wt::WPushButton>("✔"));
    confirmStartBtn_ = bindWidget("event-start-btn-confirm", std::make_unique<Wt::WPushButton>("✔"));
    confirmDurationBtn_ = bindWidget("event-duration-btn-confirm", std::make_unique<Wt::WPushButton>("✔"));
    confirmLocationBtn_ = bindWidget("event-location-btn-confirm", std::make_unique<Wt::WPushButton>("✔"));
    confirmObservationsBtn_ = bindWidget("event-observations-btn-confirm", std::make_unique<Wt::WPushButton>("✔"));

    hideEventChangeBtns(true);
    hideEventConfirmBtns(true);

    updateView(model_.get());
}

void EventFormView::setFieldsSignals()
{
    hideEventChangeBtns(false);
    setReadOnlyAll(true);

    // // Event Fields Signals
    // eventDate_->changed().connect(this, [=]()
    //                               { confirmEventData(EventDataModule::EventField::date); });
    // eventStart_->changed().connect(this, [=]()
    //                                { confirmEventData(EventDataModule::EventField::time); });
    // eventDuration_->changed().connect(this, [=]()
    //                                   { confirmEventData(EventDataModule::EventField::duration); });
    eventLocation_->enterPressed().connect(this, [=]()
                                           { confirmEventData(EventDataModule::EventField::location); });
    eventObservations_->enterPressed().connect(this, [=]()
                                               { confirmEventData(EventDataModule::EventField::observations); });

    // Change Buttons Signals
    changeDateBtn_->clicked().connect(this, [=]()
                                      { changeEventData(EventDataModule::EventField::date); });
    changeStartBtn_->clicked().connect(this, [=]()
                                       { changeEventData(EventDataModule::EventField::time); });
    changeDurationBtn_->clicked().connect(this, [=]()
                                          { changeEventData(EventDataModule::EventField::duration); });
    changeLocationBtn_->clicked().connect(this, [=]()
                                          { changeEventData(EventDataModule::EventField::location); });
    changeObservationsBtn_->clicked().connect(this, [=]()
                                              { changeEventData(EventDataModule::EventField::observations); });

    // Confirm Buttons Signals
    confirmDateBtn_->clicked().connect(this, [=]()
                                       { confirmEventData(EventDataModule::EventField::date); });
    confirmStartBtn_->clicked().connect(this, [=]()
                                        { confirmEventData(EventDataModule::EventField::time); });
    confirmDurationBtn_->clicked().connect(this, [=]()
                                           { confirmEventData(EventDataModule::EventField::duration); });
    confirmLocationBtn_->clicked().connect(this, [=]()
                                           { confirmEventData(EventDataModule::EventField::location); });
    confirmObservationsBtn_->clicked().connect(this, [=]()
                                               { confirmEventData(EventDataModule::EventField::observations); });
}

void EventFormView::changeEventData(EventDataModule::EventField field)
{
    if (model_->id != 0)
    {
        switch (field)
        {
        case EventDataModule::EventField::date:
            model_->setReadOnly(model_->EventDateField, false);
            updateViewField(model_.get(), model_->EventDateField);
            eventDate_->setFocus();
            changeDateBtn_->hide();
            confirmDateBtn_->show();
            break;
        case EventDataModule::EventField::time:
            model_->setReadOnly(model_->EventStartField, false);
            updateViewField(model_.get(), model_->EventStartField);
            eventStart_->setFocus();
            changeStartBtn_->hide();
            confirmStartBtn_->show();
            break;
        case EventDataModule::EventField::duration:
            model_->setReadOnly(model_->EventDurationField, false);
            updateViewField(model_.get(), model_->EventDurationField);
            eventDuration_->setFocus();
            changeDurationBtn_->hide();
            confirmDurationBtn_->show();
            break;
        case EventDataModule::EventField::location:
            model_->setReadOnly(model_->EventLocationField, false);
            updateViewField(model_.get(), model_->EventLocationField);
            eventLocation_->setFocus();
            changeLocationBtn_->hide();
            confirmLocationBtn_->show();
            break;
        case EventDataModule::EventField::observations:
            model_->setReadOnly(model_->EventObservationsField, false);
            updateViewField(model_.get(), model_->EventObservationsField);
            eventObservations_->setFocus();
            changeObservationsBtn_->hide();
            confirmObservationsBtn_->show();
            break;
        default:
            std::cout << "\n EventFormView::changeEventDate() - unknown field \n";
            break;
        }
    }
}

void EventFormView::confirmEventData(EventDataModule::EventField field)
{

    if (model_->id != 0 && validate())
    {
        // Ice comunication for update event field
        try
        {
            Ice::CommunicatorHolder ich = Ice::initialize();
            auto base = ich->stringToProxy(login_->eventConnString_);
            auto eventsDataInterface = Ice::checkedCast<EventDataModule::EventsDataInterfacePrx>(base);
            int eventId = model_->id;

            if (!eventsDataInterface)
            {
                throw std::runtime_error("Invalid proxy");
            }
            if (field == EventDataModule::EventField::date)
            {

                std::cout << "\n\n case date \n\n";
                model_->setReadOnly(model_->EventDateField, true);
                updateViewField(model_.get(), model_->EventDateField);
                auto stringField = model_->getDateTime();
                eventsDataInterface->modifyEventStringField(login_->userToken(), eventId, EventDataModule::EventField::dateTime, stringField);
                changeDateBtn_->setHidden(false);
                confirmDateBtn_->setHidden(true);
                // signal emmited to change navItem date
                dateChanged_.emit(Wt::WDate().fromString(model_->valueText(model_->EventDateField), model_->dateFormat_), model_->id);
            }
            else if (field == EventDataModule::EventField::time)
            {
                std::cout << "\n\n case time \n\n";
                model_->setReadOnly(model_->EventStartField, true);
                updateViewField(model_.get(), model_->EventStartField);
                auto stringField = model_->getDateTime();
                eventsDataInterface->modifyEventStringField(login_->userToken(), eventId, EventDataModule::EventField::dateTime, stringField);
                changeStartBtn_->setHidden(false);
                confirmStartBtn_->setHidden(true);
            }
            else if (field == EventDataModule::EventField::duration)
            {
                std::cout << "\n\n case duration \n\n";
                model_->setReadOnly(model_->EventDurationField, true);
                updateViewField(model_.get(), model_->EventDurationField);
                auto doubleField = ::atof(model_->valueText(model_->EventDurationField).toUTF8().c_str());
                eventsDataInterface->modifyEventDoubleField(login_->userToken(), eventId, field, doubleField);
                changeDurationBtn_->setHidden(false);
                confirmDurationBtn_->setHidden(true);
            }
            else if (field == EventDataModule::EventField::location)
            {

                std::cout << "\n\n case location \n\n";
                model_->setReadOnly(model_->EventLocationField, true);
                updateViewField(model_.get(), model_->EventLocationField);
                auto stringField = model_->valueText(model_->EventLocationField).toUTF8();
                eventsDataInterface->modifyEventStringField(login_->userToken(), eventId, field, stringField);
                changeLocationBtn_->setHidden(false);
                confirmLocationBtn_->setHidden(true);
            }
            else if (field == EventDataModule::EventField::observations)
            {
                std::cout << "\n\n case observations \n\n";
                model_->setReadOnly(model_->EventObservationsField, true);
                updateViewField(model_.get(), model_->EventObservationsField);
                auto stringField = model_->valueText(model_->EventObservationsField).toUTF8();
                eventsDataInterface->modifyEventStringField(login_->userToken(), eventId, field, stringField);
                changeObservationsBtn_->setHidden(false);
                confirmObservationsBtn_->setHidden(true);
            }
            else
            {
                std::cout << "\n EventFormView::confirmEventData() - unknown field \n";
            }
        }
        catch (const std::exception &e)

        {
            std::cerr << e.what() << std::endl;
        }
    }
}

void EventFormView::hideEventChangeBtns(bool hide)
{
    if (hide)
    {
        changeDateBtn_->hide();
        changeStartBtn_->hide();
        changeDurationBtn_->hide();
        changeLocationBtn_->hide();
        changeObservationsBtn_->hide();
    }
    else
    {
        changeDateBtn_->show();
        changeStartBtn_->show();
        changeDurationBtn_->show();
        changeLocationBtn_->show();
        changeObservationsBtn_->show();
    }
}

void EventFormView::hideEventConfirmBtns(bool hide)
{
    if (hide)
    {
        confirmDateBtn_->hide();
        confirmStartBtn_->hide();
        confirmDurationBtn_->hide();
        confirmLocationBtn_->hide();
        confirmObservationsBtn_->hide();
    }
    else
    {
        confirmDateBtn_->show();
        confirmStartBtn_->show();
        confirmDurationBtn_->show();
        confirmLocationBtn_->show();
        confirmObservationsBtn_->show();
    }
}

void EventFormView::setReadOnlyAll(bool readOnly)
{
    if (readOnly)
    {
        model_->setReadOnly(model_->EventDateField, true);
        model_->setReadOnly(model_->EventStartField, true);
        model_->setReadOnly(model_->EventDurationField, true);
        model_->setReadOnly(model_->EventLocationField, true);
        model_->setReadOnly(model_->EventObservationsField, true);
    }
    else
    {
        model_->setReadOnly(model_->EventDateField, false);
        model_->setReadOnly(model_->EventStartField, false);
        model_->setReadOnly(model_->EventDurationField, false);
        model_->setReadOnly(model_->EventLocationField, false);
        model_->setReadOnly(model_->EventObservationsField, false);
    }
    updateView(model_.get());
}

void EventFormView::setData(EventDataModule::EventData eventData)
{
    Wt::WDateTime dateTime = Wt::WDateTime::fromString(eventData.dateTime, model_->dateTimeFormat_);

    std::string eventDateString = dateTime.date().toString(model_->dateFormat_).toUTF8();
    std::string eventTimeString = dateTime.time().toString(model_->timeFormat_).toUTF8();

    model_->id = eventData.id;
    model_->setValue(model_->EventDateField, eventDateString);
    model_->setValue(model_->EventStartField, eventTimeString);
    model_->setValue(model_->EventDurationField, eventData.duration);
    model_->setValue(model_->EventLocationField, eventData.location);
    model_->setValue(model_->EventObservationsField, eventData.observations);

    updateView(model_.get());
    if (firstSetData_)
    {
        dateChanged_.emit(dateTime.date(), model_->id);
        firstSetData_ = false;
    }
}

bool EventFormView::validate()
{
    updateModel(model_.get());
    if (model_->validate())
    {
        return true;
    }
    else
    {
        if (!model_->validateField(model_->EventDateField))
        {
            setFocus(model_->EventDateField);
            bindString("submit-info", "Date is not valid");
        }
        else if (!model_->validateField(model_->EventStartField))
        {
            setFocus(model_->EventStartField);
            bindString("submit-info", "Start time is not valid");
        }
        else if (!model_->validateField(model_->EventDurationField))
        {
            setFocus(model_->EventDurationField);
            bindString("submit-info", "Duration is not valid");
        }
        else if (!model_->validateField(model_->EventLocationField))
        {
            setFocus(model_->EventLocationField);
            bindString("submit-info", "Location is not valid");
        }
        else if (!model_->validateField(model_->EventObservationsField))
        {
            setFocus(model_->EventObservationsField);
            bindString("submit-info", "Observations is not valid");
        }
        return false;
    }
}