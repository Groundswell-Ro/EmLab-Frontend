#include "include/EventForm.h"

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
    setValidator(EventObservationsField, createTextValidator(false, 1, 500));

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

    auto date = Wt::WDate().fromString(valueText(EventDateField), Emlab::DATE_FORMAT);
    auto time = Wt::WTime().fromString(valueText(EventStartField), Emlab::TIME_FORMAT);
    Wt::WDateTime dateTime{date, time};

    return dateTime.toString(Emlab::DATE_TIME_FORMAT).toUTF8();
}

// E V E N T _____________ V I E W
EventFormView::EventFormView(std::shared_ptr<Login> login)
    : login_(login)
{    
    model_ = std::make_shared<EventFormModel>();
    setTemplateText(tr("event-form-template"));
    addFunction("id", &WTemplate::Functions::id);

    /*
     * Event Date Widget
     */
    auto eventDateWidget = std::make_unique<Wt::WDateEdit>();
    eventDateWidget->setReadOnly(true);
    eventDateWidget->setPlaceholderText("nu este setata nici o data");
    eventDateWidget->setFormat(Emlab::DATE_FORMAT);
    eventDate_ = eventDateWidget.get();
    setFormWidget(model_->EventDateField, std::move(eventDateWidget));

    /*
     * Event Starting Time Widget
     */
    auto eventStartWidget = std::make_unique<Wt::WTimeEdit>();
    eventStartWidget->setMinuteStep(15);
    eventStartWidget->setTime(Wt::WTime(12, 0, 0));
    eventStartWidget->setFormat(Emlab::TIME_FORMAT);
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
    eventLocationWidget->setPlaceholderText("nu este setata nici o locatie");
    eventLocation_ = eventLocationWidget.get();
    setFormWidget(model_->EventLocationField, std::move(eventLocationWidget));

    /*
     *  Observations Textarea Widget
     */
    auto observationsWidget = std::make_unique<Wt::WTextArea>();
    eventObservations_ = observationsWidget.get();
    setFormWidget(model_->EventObservationsField, std::move(observationsWidget));
    eventObservations_->setHeight(Wt::WLength::Auto);
    eventObservations_->focussed().connect(this, [=]() {
        eventObservations_->setHeight(Wt::WLength(250));
    });

    updateView(model_.get());
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
        
            break;
        case EventDataModule::EventField::time:
            model_->setReadOnly(model_->EventStartField, false);
            updateViewField(model_.get(), model_->EventStartField);
            eventStart_->setFocus();
          
            break;
        case EventDataModule::EventField::duration:
            model_->setReadOnly(model_->EventDurationField, false);
            updateViewField(model_.get(), model_->EventDurationField);
            eventDuration_->setFocus();
       
            break;
        case EventDataModule::EventField::location:
            model_->setReadOnly(model_->EventLocationField, false);
            updateViewField(model_.get(), model_->EventLocationField);
            eventLocation_->setFocus();
      
            break;
        case EventDataModule::EventField::observations:
            model_->setReadOnly(model_->EventObservationsField, false);
            updateViewField(model_.get(), model_->EventObservationsField);
            eventObservations_->setFocus();

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
                auto stringField = model_->getDateTime();
                eventsDataInterface->modifyEventStringField(login_->userToken(), eventId, EventDataModule::EventField::dateTime, stringField);
       
                // signal emmited to change navItem date
                dateChanged_.emit(Wt::WDate().fromString(model_->valueText(model_->EventDateField), Emlab::DATE_FORMAT), model_->id);
            }
            else if (field == EventDataModule::EventField::time)
            {
                std::cout << "\n\n case time \n\n";
                model_->setReadOnly(model_->EventStartField, true);
                auto stringField = model_->getDateTime();
                eventsDataInterface->modifyEventStringField(login_->userToken(), eventId, EventDataModule::EventField::dateTime, stringField);
               
            }
            else if (field == EventDataModule::EventField::duration)
            {
                std::cout << "\n\n case duration \n\n";
                model_->setReadOnly(model_->EventDurationField, true);
                auto doubleField = ::atof(model_->valueText(model_->EventDurationField).toUTF8().c_str());
                eventsDataInterface->modifyEventDoubleField(login_->userToken(), eventId, field, doubleField);
               
            }
            else if (field == EventDataModule::EventField::location)
            {
                std::cout << "\n\n case location \n\n";
                model_->setReadOnly(model_->EventLocationField, true);
                auto stringField = model_->valueText(model_->EventLocationField).toUTF8();
                eventsDataInterface->modifyEventStringField(login_->userToken(), eventId, field, stringField);
               
            }
            else if (field == EventDataModule::EventField::observations)
            {
                std::cout << "\n\n case observations \n\n";
                model_->setReadOnly(model_->EventObservationsField, true);
                auto stringField = model_->valueText(model_->EventObservationsField).toUTF8();
                eventsDataInterface->modifyEventStringField(login_->userToken(), eventId, field, stringField);
            
                eventObservations_->setHeight(Wt::WLength::Auto);
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


void EventFormView::setReadOnlyAll(bool readOnly)
{
    if (readOnly)
    {
        model_->setReadOnly(model_->EventDateField, true);
        model_->setReadOnly(model_->EventStartField, true);
        model_->setReadOnly(model_->EventDurationField, true);
        model_->setReadOnly(model_->EventLocationField, true);
        model_->setReadOnly(model_->EventObservationsField, true);
    updateView(model_.get());
    }
    else
    {
        model_->setReadOnly(model_->EventDateField, false);
        model_->setReadOnly(model_->EventStartField, false);
        model_->setReadOnly(model_->EventDurationField, false);
        model_->setReadOnly(model_->EventLocationField, false);
        model_->setReadOnly(model_->EventObservationsField, false);
    updateView(model_.get());
    }
}

void EventFormView::setData(EventDataModule::EventData eventData)
{
    Wt::WDateTime dateTime = Wt::WDateTime::fromString(eventData.dateTime, Emlab::DATE_TIME_FORMAT);

    std::string eventDateString = dateTime.date().toString(Emlab::DATE_FORMAT).toUTF8();
    std::string eventTimeString = dateTime.time().toString(Emlab::TIME_FORMAT).toUTF8();

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
        bindEmpty("date-info");
        bindEmpty("time-info");
        bindEmpty("duration-info");
        bindEmpty("location-info");
        bindEmpty("observations-info");
        return true;
    }
    else
    {
        if (!model_->validateField(model_->EventDateField))
        {
            setFocus(model_->EventDateField);
            updateViewField(model_.get(), model_->EventDateField);
            bindString("date-info", "Data obligatorie");
        }
        else if (!model_->validateField(model_->EventStartField))
        {
            setFocus(model_->EventStartField);
            updateViewField(model_.get(), model_->EventStartField);
            bindString("start-info", "Ora obligatorie");
        }
        else if (!model_->validateField(model_->EventDurationField))
        {
            setFocus(model_->EventDurationField);
            updateViewField(model_.get(), model_->EventDurationField);
            bindString("duration-info", "Durata obligatorie");
        }
        else if (!model_->validateField(model_->EventLocationField))
        {
            setFocus(model_->EventLocationField);
            updateViewField(model_.get(), model_->EventLocationField);
            bindString("location-info", "Locatie obligatorie");
        }
        else if (!model_->validateField(model_->EventObservationsField))
        {
            setFocus(model_->EventObservationsField);
            updateViewField(model_.get(), model_->EventObservationsField);
            bindString("observations-info", "Observatii obligatorii");
        }
        return false;
    }
}