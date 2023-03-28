#include "include/ServiceForm.h"

#include <Ice/Ice.h>
#include <stdexcept>
#include <Wt/WPopupMenu.h>
#include <Wt/WStandardItem.h>

// Validators
#include <Wt/WIntValidator.h>    // for Cost
#include <Wt/WLengthValidator.h> // for Name
#include <Wt/WDoubleValidator.h> // for Duration
#include <Wt/WTimeValidator.h>   // for Start hour

// Text Validator
std::shared_ptr<Wt::WValidator> ServiceFormModel::createTextValidator(bool mandatory, int min, int max)
{
    auto v = std::make_shared<Wt::WLengthValidator>();
    v->setMandatory(mandatory);
    v->setMinimumLength(min);
    v->setMaximumLength(max);
    return v;
}

// Double Validator
std::shared_ptr<Wt::WValidator> ServiceFormModel::createDoubleValidator(double min, double max)
{
    auto v = std::make_shared<Wt::WDoubleValidator>();
    v->setMandatory(true);
    v->setTop(max);
    v->setBottom(min);
    return v;
}

// Int Validator
std::shared_ptr<Wt::WValidator> ServiceFormModel::createIntValidator(int min, int max)
{
    auto v = std::make_shared<Wt::WIntValidator>();
    v->setTop(max);
    v->setBottom(min);
    return v;
}

// mandatory Validator
std::shared_ptr<Wt::WValidator> ServiceFormModel::createMandatoryValidator()
{
    auto v = std::make_shared<Wt::WLengthValidator>();
    v->setMandatory(true);
    return v;
}

// Service fields
const Wt::WFormModel::Field ServiceFormModel::ServiceProviderIdentityField = "provider-identity";
const Wt::WFormModel::Field ServiceFormModel::ServiceProviderServiceField = "provider-service";
const Wt::WFormModel::Field ServiceFormModel::ServiceStartField = "service-start";
const Wt::WFormModel::Field ServiceFormModel::ServiceDurationField = "service-duration";
const Wt::WFormModel::Field ServiceFormModel::ServiceCostField = "service-cost";
const Wt::WFormModel::Field ServiceFormModel::ServiceDescriptionField = "service-description";
const Wt::WFormModel::Field ServiceFormModel::ServiceObservationsField = "service-observations";

// S E R V I C E ________ M O D E L
ServiceFormModel::ServiceFormModel(std::shared_ptr<EventFormModel> eventModel, std::shared_ptr<Login> login)
    : WFormModel(),
      eventModel_(eventModel),
      login_(login)
{
    auto userName = login_->user().name;
    ServiceMap serviceMap;
    // {{userName, {"Kids Entertainer", "Magician", "Ballons Workshop", "Ballon modeling artist"}}};

    auto userServices = login_->user().servicesInfoSq;

    std::vector<std::string> services;
    for (auto service : userServices)
    {
        services.push_back(service.title);
    }
    serviceMap.insert({userName, services});

    initializeModels(serviceMap);

    addField(ServiceProviderIdentityField);
    addField(ServiceProviderServiceField);
    addField(ServiceStartField);
    addField(ServiceDurationField);
    addField(ServiceCostField);
    addField(ServiceDescriptionField);
    addField(ServiceObservationsField);

    setValidator(ServiceProviderIdentityField, createMandatoryValidator());
    setValidator(ServiceProviderServiceField, createMandatoryValidator());
    setValidator(ServiceDurationField, createDoubleValidator(0.5, 10.0));
    setValidator(ServiceCostField, createIntValidator(0, 100000));
    setValidator(ServiceDescriptionField, createTextValidator(false, 0, 50));
    setValidator(ServiceObservationsField, createTextValidator(false, 0, 300));

    setValue(ServiceStartField, "12:00 PM");
    setValue(ServiceDurationField, "1");
}

EventDataModule::ServiceData ServiceFormModel::getData()
{
    EventDataModule::ServiceData serviceData;
    std::cout << "\n\n";
    std::cout << "----" << Wt::asString(value(ServiceProviderIdentityField)).toUTF8();
    std::cout << "\n\n";
    std::cout << "----" << Wt::asString(value(ServiceProviderServiceField)).toUTF8();
    std::cout << "\n\n";

    serviceData.id = id;
    serviceData.eventId = eventModel_->id;
    serviceData.providerIdentity = Wt::asString(value(ServiceProviderIdentityField)).toUTF8();
    serviceData.providerService = Wt::asString(value(ServiceProviderServiceField)).toUTF8();
    serviceData.dateTime = getDateTime();
    serviceData.duration = std::atof(valueText(ServiceDurationField).toUTF8().c_str());
    serviceData.cost = std::stoi(valueText(ServiceCostField).toUTF8());
    serviceData.description = valueText(ServiceDescriptionField).toUTF8();
    serviceData.observations = valueText(ServiceObservationsField).toUTF8();

    return serviceData;
}

// // from here, handles the provider and the services
// ProviderMap ServiceFormModel::getProviders()
// {
//     ProviderMap ProviderMap = {
//         {"1", {"Myself"}},
//         // {"2", {"Another"}}
//     };
//     return ProviderMap;
// }

// ServiceMap ServiceFormModel::getServices()
// {
//     ServiceMap serviceMap = {
//         {"1", {"Kids Entertainer", "Magician", "Ballons Workshop", "Ballon modeling artist"}},
//         {"2", {"A - Kids Entertainer", "A - Magician", "A - Ballons Workshop", "A - Ballon modeling artist"}}};

//     //
//     return serviceMap;
// }

void ServiceFormModel::initializeModels(ServiceMap servMap)
{
    // provider code should be user phone number
    auto userName = login_->user().name;
    providers = {{userName, {"Myself"}}};
    services = servMap;
    // Create a provider model.
    providerModel_ = std::make_shared<Wt::WStandardItemModel>(providers.size(), 1);

    // For each provider, update the model based on the key (corresponding
    // to the provider code):
    // - set the provider name for the display role,
    // - set the service names for the user role.
    int row = 0;
    for (ProviderMap::const_iterator i = providers.begin();
         i != providers.end(); ++i)
    {
        providerModel_->setData(row, 0, i->second, Wt::ItemDataRole::Display);
        providerModel_->setData(row++, 0, i->first, Wt::ItemDataRole::User);
    }

    // Create a city model.
    serviceModel_ = std::make_shared<Wt::WStandardItemModel>();
    // updateProviderServicesModel(providers.begin()->first);
}

void ServiceFormModel::updateProviderServicesModel(const std::string &providerCode)
{
    serviceModel_->clear();

    ServiceMap::const_iterator i = services.find(providerCode);

    if (i != services.end())
    {
        const std::vector<std::string> &services = i->second;

        // The initial text shown in the city combo box should be an empty
        // string.
        serviceModel_->appendRow(std::make_unique<Wt::WStandardItem>());

        for (unsigned j = 0; j < services.size(); ++j)
            serviceModel_->appendRow(std::make_unique<Wt::WStandardItem>(services[j]));
    }
    else
    {
        serviceModel_->appendRow(
            std::make_unique<Wt::WStandardItem>("(Choose Service)"));
    }
}

std::string ServiceFormModel::providerCode(int row)
{
    return Wt::asString(providerModel_->data(row, 0, Wt::ItemDataRole::User)).toUTF8();
}

int ServiceFormModel::providerModelRow(const std::string &code)
{
    for (int i = 0; i < providerModel_->rowCount(); ++i)
        if (providerCode(i) == code)
            return i;

    return -1;
}
// to here, handles the provider and the services

std::string ServiceFormModel::getDateTime()
{
    Wt::WDate date;
    Wt::WTime time;
    Wt::WDateTime dateTime;

    date = Wt::WDate().fromString(eventModel_->valueText(EventFormModel::EventDateField), dateFormat_);
    time = Wt::WTime().fromString(valueText(ServiceStartField), timeFormat_);
    dateTime = Wt::WDateTime(date, time);

    return dateTime.toString("dd/MM/yyyy HH:mm AP").toUTF8();
}

// S E R V I C E ________ V I E W
ServiceFormView::ServiceFormView(std::shared_ptr<Login> login, std::shared_ptr<EventFormModel> eventModel)
    : Wt::WTemplateFormView(),
      login_(login),
      eventModel_(eventModel)
{
    setMinimumSize(Wt::WLength(280, Wt::LengthUnit::Pixel), Wt::WLength::Auto);
    setMaximumSize(Wt::WLength(350, Wt::LengthUnit::Pixel), Wt::WLength::Auto);

    model_ = std::make_shared<ServiceFormModel>(eventModel, login_);
    if (eventModel_->id == 0)
    {
    }
    setTemplateText(tr("service-form-template"));
    addFunction("id", &Wt::WTemplate::Functions::id);

    /*
     * Service Provider Identity Widget
     */
    auto serviceProviderIdentity = std::make_unique<Wt::WComboBox>();
    serviceProviderIdentity->setPlaceholderText("this is needed for input desighn animation");
    serviceProviderIdentity->setModel(model_->providerModel());
    providerIdentity_ = serviceProviderIdentity.get();
    serviceProviderIdentity->activated().connect([=]
                                                 {
            std::string code = model_->providerCode(providerIdentity_->currentIndex());
            model_->updateProviderServicesModel(code); });

    setFormWidget(
        model_->ServiceProviderIdentityField, std::move(serviceProviderIdentity),
        [=] { // updateViewValue()
            std::string code =
                Wt::asString(model_->value(ServiceFormModel::ServiceProviderIdentityField)).toUTF8();
            int row = model_->providerModelRow(code);
            providerIdentity_->setCurrentIndex(row);
        },

        [=] { // updateModelValue()
            std::string code = model_->providerCode(providerIdentity_->currentIndex());
            model_->setValue(ServiceFormModel::ServiceProviderIdentityField, code);
        });

    /*
     * Service Provider Identity Widget
     */

    auto serviceProviderService = std::make_unique<Wt::WComboBox>();
    serviceProviderService->setPlaceholderText("this is needed for input desighn animation");
    serviceProviderService->setModel(model_->serviceModel());
    providerService_ = serviceProviderService.get();
    serviceProviderService->focussed().connect([=]
                                               {
            std::string code = model_->providerCode(providerIdentity_->currentIndex());
            model_->updateProviderServicesModel(code); });

    setFormWidget(model_->ServiceProviderServiceField, std::move(serviceProviderService));

    /*
     * Service Starting Time Widget
     */
    auto serviceStartWidget = std::make_unique<Wt::WTimeEdit>();
    serviceStartWidget->setMinuteStep(15);
    serviceStartWidget->setTime(Wt::WTime(12, 0, 0));
    serviceStartWidget->setFormat(model_->timeFormat_);
    // serviceStartWidget->setReadOnly(true);
    serviceStart_ = serviceStartWidget.get();
    setFormWidget(model_->ServiceStartField, std::move(serviceStartWidget));

    /*
     * Service Duration Widget
     */
    auto serviceDurationWidget = std::make_unique<Wt::WDoubleSpinBox>();
    serviceDurationWidget->setDecimals(1);
    serviceDurationWidget->setSingleStep(0.5);
    serviceDuration_ = serviceDurationWidget.get();
    setFormWidget(model_->ServiceDurationField, std::move(serviceDurationWidget));

    /*
     * Service Cost Widget
     */

    auto serviceCostWidget = std::make_unique<Wt::WSpinBox>();
    serviceCostWidget->setSingleStep(25);
    serviceCostWidget->setMaximum(100000);
    serviceCost_ = serviceCostWidget.get();
    setFormWidget(model_->ServiceCostField, std::move(serviceCostWidget));

    /*
     *  Description Textarea Widget
     */
    auto serviceDescriptionWidget = std::make_unique<Wt::WTextArea>();
    serviceDescription_ = serviceDescriptionWidget.get();
    setFormWidget(model_->ServiceDescriptionField, std::move(serviceDescriptionWidget));

    /*
     *  Observations Textarea Widget
     */
    auto observationsWidget = std::make_unique<Wt::WTextArea>();
    serviceObservations_ = observationsWidget.get();
    setFormWidget(model_->ServiceObservationsField, std::move(observationsWidget));

    bindString("submit-info", Wt::WString(""));

    changeProviderIdentityBtn_ = bindWidget("service-provider-identity-btn-change", std::make_unique<Wt::WPushButton>("x"));
    changeProviderServiceBtn_ = bindWidget("service-provider-service-btn-change", std::make_unique<Wt::WPushButton>("x"));
    changeServiceStartBtn_ = bindWidget("service-start-btn-change", std::make_unique<Wt::WPushButton>("x"));
    changeServiceDurationBtn_ = bindWidget("service-duration-btn-change", std::make_unique<Wt::WPushButton>("x"));
    changeServiceCostBtn_ = bindWidget("service-cost-btn-change", std::make_unique<Wt::WPushButton>("x"));
    changeServiceDescriptionBtn_ = bindWidget("service-description-btn-change", std::make_unique<Wt::WPushButton>("x"));
    changeServiceObservationsBtn_ = bindWidget("service-observations-btn-change", std::make_unique<Wt::WPushButton>("x"));

    confirmProviderIdentityBtn_ = bindWidget("service-provider-identity-btn-confirm", std::make_unique<Wt::WPushButton>("✔"));
    confirmProviderServiceBtn_ = bindWidget("service-provider-service-btn-confirm", std::make_unique<Wt::WPushButton>("✔"));
    confirmServiceStartBtn_ = bindWidget("service-start-btn-confirm", std::make_unique<Wt::WPushButton>("✔"));
    confirmServiceDurationBtn_ = bindWidget("service-duration-btn-confirm", std::make_unique<Wt::WPushButton>("✔"));
    confirmServiceCostBtn_ = bindWidget("service-cost-btn-confirm", std::make_unique<Wt::WPushButton>("✔"));
    confirmServiceDescriptionBtn_ = bindWidget("service-description-btn-confirm", std::make_unique<Wt::WPushButton>("✔"));
    confirmServiceObservationsBtn_ = bindWidget("service-observations-btn-confirm", std::make_unique<Wt::WPushButton>("✔"));

    hideServiceChangeBtns(true);
    hideServiceConfirmBtns(true);

    updateView(model_.get());
}

void ServiceFormView::setFieldsSignals()
{
    setReadOnlyAll(true);
    hideServiceChangeBtns(false);

    // signals for fields
    providerIdentity_->enterPressed().connect(this, [=]()
                                              { changeServiceData(EventDataModule::ServiceField::providerIdentity); });
    providerService_->enterPressed().connect(this, [=]()
                                             { changeServiceData(EventDataModule::ServiceField::providerService); });
    // serviceStart_->changed().connect(this, [=]()
    //                                  { changeServiceData(EventDataModule::ServiceField::dateTime); });
    // serviceDuration_->changed().connect(this, [=]()
    //                                     { changeServiceData(EventDataModule::ServiceField::duration); });
    // serviceCost_->changed().connect(this, [=]()
    //                                 { changeServiceData(EventDataModule::ServiceField::cost); });
    serviceDescription_->enterPressed().connect(this, [=]()
                                                { changeServiceData(EventDataModule::ServiceField::description); });
    serviceObservations_->enterPressed().connect(this, [=]()
                                                 { changeServiceData(EventDataModule::ServiceField::observations); });

    // signals for change data buttons
    changeProviderIdentityBtn_->clicked().connect(this, [=]()
                                                  { changeServiceData(EventDataModule::ServiceField::providerIdentity); });
    changeProviderServiceBtn_->clicked().connect(this, [=]()
                                                 { changeServiceData(EventDataModule::ServiceField::providerService); });
    changeServiceStartBtn_->clicked().connect(this, [=]()
                                              { changeServiceData(EventDataModule::ServiceField::dateTime); });
    changeServiceDurationBtn_->clicked().connect(this, [=]()
                                                 { changeServiceData(EventDataModule::ServiceField::duration); });
    changeServiceCostBtn_->clicked().connect(this, [=]()
                                             { changeServiceData(EventDataModule::ServiceField::cost); });
    changeServiceDescriptionBtn_->clicked().connect(this, [=]()
                                                    { changeServiceData(EventDataModule::ServiceField::description); });
    changeServiceObservationsBtn_->clicked().connect(this, [=]()
                                                     { changeServiceData(EventDataModule::ServiceField::observations); });

    // signals for confirm data buttons

    confirmProviderIdentityBtn_->clicked().connect(this, [=]()
                                                   { confirmServiceData(EventDataModule::ServiceField::providerIdentity); });
    confirmProviderServiceBtn_->clicked().connect(this, [=]()
                                                  { confirmServiceData(EventDataModule::ServiceField::providerService); });
    confirmServiceStartBtn_->clicked().connect(this, [=]()
                                               { confirmServiceData(EventDataModule::ServiceField::dateTime); });
    confirmServiceDurationBtn_->clicked().connect(this, [=]()
                                                  { confirmServiceData(EventDataModule::ServiceField::duration); });
    confirmServiceCostBtn_->clicked().connect(this, [=]()
                                              { confirmServiceData(EventDataModule::ServiceField::cost); });
    confirmServiceDescriptionBtn_->clicked().connect(this, [=]()
                                                     { confirmServiceData(EventDataModule::ServiceField::description); });
    confirmServiceObservationsBtn_->clicked().connect(this, [=]()
                                                      { confirmServiceData(EventDataModule::ServiceField::observations); });
}

void ServiceFormView::hideServiceChangeBtns(bool hide)
{
    if (hide)
    {
        changeProviderIdentityBtn_->hide();
        changeProviderServiceBtn_->hide();
        changeServiceStartBtn_->hide();
        changeServiceDurationBtn_->hide();
        changeServiceCostBtn_->hide();
        changeServiceDescriptionBtn_->hide();
        changeServiceObservationsBtn_->hide();
    }
    else
    {
        changeProviderIdentityBtn_->show();
        changeProviderServiceBtn_->show();
        changeServiceStartBtn_->show();
        changeServiceDurationBtn_->show();
        changeServiceCostBtn_->show();
        changeServiceDescriptionBtn_->show();
        changeServiceObservationsBtn_->show();
    }
}

void ServiceFormView::hideServiceConfirmBtns(bool hide)
{
    if (hide)
    {
        confirmProviderIdentityBtn_->hide();
        confirmProviderServiceBtn_->hide();
        confirmServiceStartBtn_->hide();
        confirmServiceDurationBtn_->hide();
        confirmServiceCostBtn_->hide();
        confirmServiceDescriptionBtn_->hide();
        confirmServiceObservationsBtn_->hide();
    }
    else
    {
        confirmProviderIdentityBtn_->show();
        confirmProviderServiceBtn_->show();
        confirmServiceStartBtn_->show();
        confirmServiceDurationBtn_->show();
        confirmServiceCostBtn_->show();
        confirmServiceDescriptionBtn_->show();
        confirmServiceObservationsBtn_->show();
    }
}

void ServiceFormView::setReadOnlyAll(bool readOnly)
{
    if (readOnly)
    {
        model_->setReadOnly(model_->ServiceProviderIdentityField, true);
        model_->setReadOnly(model_->ServiceProviderServiceField, true);
        model_->setReadOnly(model_->ServiceStartField, true);
        model_->setReadOnly(model_->ServiceDurationField, true);
        model_->setReadOnly(model_->ServiceCostField, true);
        model_->setReadOnly(model_->ServiceDescriptionField, true);
        model_->setReadOnly(model_->ServiceObservationsField, true);
    }
    else
    {
        model_->setReadOnly(model_->ServiceProviderIdentityField, false);
        model_->setReadOnly(model_->ServiceProviderServiceField, false);
        model_->setReadOnly(model_->ServiceStartField, false);
        model_->setReadOnly(model_->ServiceDurationField, false);
        model_->setReadOnly(model_->ServiceCostField, false);
        model_->setReadOnly(model_->ServiceDescriptionField, false);
        model_->setReadOnly(model_->ServiceObservationsField, false);
    }
    updateView(model_.get());
}

void ServiceFormView::changeServiceData(EventDataModule::ServiceField field)
{
    if (model_->id != 0)
    {
        switch (field)
        {
        case EventDataModule::ServiceField::providerIdentity:
            model_->setReadOnly(model_->ServiceProviderIdentityField, false);
            model_->setReadOnly(model_->ServiceProviderServiceField, false);
            updateViewField(model_.get(), model_->ServiceProviderIdentityField);
            updateViewField(model_.get(), model_->ServiceProviderServiceField);
            providerIdentity_->setFocus();
            changeProviderIdentityBtn_->hide();
            confirmProviderIdentityBtn_->show();
            changeProviderServiceBtn_->hide();
            confirmProviderServiceBtn_->show();
            break;
        case EventDataModule::ServiceField::providerService:
            model_->setReadOnly(model_->ServiceProviderServiceField, false);
            updateViewField(model_.get(), model_->ServiceProviderServiceField);
            providerService_->setFocus();
            changeProviderServiceBtn_->hide();
            confirmProviderServiceBtn_->show();
            break;
        case EventDataModule::ServiceField::dateTime:
            model_->setReadOnly(model_->ServiceStartField, false);
            updateViewField(model_.get(), model_->ServiceStartField);
            serviceStart_->setFocus();
            changeServiceStartBtn_->hide();
            confirmServiceStartBtn_->show();
            break;
        case EventDataModule::ServiceField::duration:
            model_->setReadOnly(model_->ServiceDurationField, false);
            updateViewField(model_.get(), model_->ServiceDurationField);
            serviceDuration_->setFocus();
            changeServiceDurationBtn_->hide();
            confirmServiceDurationBtn_->show();
            break;
        case EventDataModule::ServiceField::cost:
            model_->setReadOnly(model_->ServiceCostField, false);
            updateViewField(model_.get(), model_->ServiceCostField);
            serviceCost_->setFocus();
            changeServiceCostBtn_->hide();
            confirmServiceCostBtn_->show();
            break;
        case EventDataModule::ServiceField::description:
            model_->setReadOnly(model_->ServiceDescriptionField, false);
            updateViewField(model_.get(), model_->ServiceDescriptionField);
            serviceDescription_->setFocus();
            changeServiceDescriptionBtn_->hide();
            confirmServiceDescriptionBtn_->show();
            break;
        case EventDataModule::ServiceField::observations:
            model_->setReadOnly(model_->ServiceObservationsField, false);
            updateViewField(model_.get(), model_->ServiceObservationsField);
            serviceObservations_->setFocus();
            changeServiceObservationsBtn_->hide();
            confirmServiceObservationsBtn_->show();
            break;
        default:
            std::cout << "ServiceFormView::changeServiceData: unknown field" << std::endl;
            break;
        }
    }
}

void ServiceFormView::confirmServiceData(EventDataModule::ServiceField field)
{
    if (model_->id != 0 && validate())
    {
        bindEmpty("submit-info");
        // Ice comunication for update service field
        try
        {
            Ice::CommunicatorHolder ich = Ice::initialize();
            auto base = ich->stringToProxy(login_->eventConnString_);
            auto eventDataInterface = Ice::checkedCast<EventDataModule::EventsDataInterfacePrx>(base);
            updateModel(model_.get());
            if (!eventDataInterface)
            {
                throw std::runtime_error("Invalid proxy");
            }
            if (field == EventDataModule::ServiceField::providerIdentity)
            {
                model_->setReadOnly(model_->ServiceProviderIdentityField, true);
                model_->setReadOnly(model_->ServiceProviderServiceField, true);
                updateViewField(model_.get(), model_->ServiceProviderIdentityField);
                updateViewField(model_.get(), model_->ServiceProviderServiceField);
                auto identityStringField = Wt::asString(model_->value(model_->ServiceProviderIdentityField)).toUTF8();
                auto serviceStringField = Wt::asString(model_->value(model_->ServiceProviderServiceField)).toUTF8();
                eventDataInterface->modifyServiceStringField(login_->userToken(), model_->id, field, identityStringField);
                eventDataInterface->modifyServiceStringField(login_->userToken(), model_->id, field, serviceStringField);
                std::cout << "\n case providerIdentity \n";
                changeProviderIdentityBtn_->setHidden(false);
                confirmProviderIdentityBtn_->setHidden(true);
                changeProviderServiceBtn_->setHidden(false);
                confirmProviderServiceBtn_->setHidden(true);
            }
            else if (field == EventDataModule::ServiceField::providerService)
            {
                model_->setReadOnly(model_->ServiceProviderServiceField, true);
                model_->setReadOnly(model_->ServiceProviderIdentityField, true);
                updateViewField(model_.get(), model_->ServiceProviderServiceField);
                updateViewField(model_.get(), model_->ServiceProviderIdentityField);
                auto stringField = Wt::asString(model_->value(model_->ServiceProviderServiceField)).toUTF8();
                eventDataInterface->modifyServiceStringField(login_->userToken(), model_->id, field, stringField);
                std::cout << "\n case providerService \n";
                changeProviderServiceBtn_->setHidden(false);
                confirmProviderServiceBtn_->setHidden(true);
                changeProviderIdentityBtn_->setHidden(false);
                confirmProviderIdentityBtn_->setHidden(true);
            }
            else if (field == EventDataModule::ServiceField::dateTime)
            {
                model_->setReadOnly(model_->ServiceStartField, true);
                updateViewField(model_.get(), model_->ServiceStartField);
                auto stringField = model_->getDateTime();
                eventDataInterface->modifyServiceStringField(login_->userToken(), model_->id, field, stringField);
                std::cout << "\n case dateTime \n";
                changeServiceStartBtn_->setHidden(false);
                confirmServiceStartBtn_->setHidden(true);
            }
            else if (field == EventDataModule::ServiceField::duration)
            {
                model_->setReadOnly(model_->ServiceDurationField, true);
                updateViewField(model_.get(), model_->ServiceDurationField);
                auto doubleField = ::atof(model_->valueText(model_->ServiceDurationField).toUTF8().c_str());
                eventDataInterface->modifyServiceDoubleField(login_->userToken(), model_->id, field, doubleField);
                std::cout << "\n case duration \n";
                changeServiceDurationBtn_->setHidden(false);
                confirmServiceDurationBtn_->setHidden(true);
            }
            else if (field == EventDataModule::ServiceField::cost)
            {
                model_->setReadOnly(model_->ServiceCostField, true);
                updateViewField(model_.get(), model_->ServiceCostField);
                auto intField = ::atoi(model_->valueText(model_->ServiceCostField).toUTF8().c_str());
                eventDataInterface->modifyServiceIntField(login_->userToken(), model_->id, field, intField);
                std::cout << "\n case cost \n";
                changeServiceCostBtn_->setHidden(false);
                confirmServiceCostBtn_->setHidden(true);
            }
            else if (field == EventDataModule::ServiceField::description)
            {
                model_->setReadOnly(model_->ServiceDescriptionField, true);
                updateViewField(model_.get(), model_->ServiceDescriptionField);
                auto stringField = model_->valueText(model_->ServiceDescriptionField).toUTF8().c_str();
                eventDataInterface->modifyServiceStringField(login_->userToken(), model_->id, field, stringField);
                std::cout << "\n case description \n";
                changeServiceDescriptionBtn_->setHidden(false);
                confirmServiceDescriptionBtn_->setHidden(true);
            }
            else if (field == EventDataModule::ServiceField::observations)
            {
                model_->setReadOnly(model_->ServiceObservationsField, true);
                updateViewField(model_.get(), model_->ServiceObservationsField);
                auto stringField = model_->valueText(model_->ServiceObservationsField).toUTF8().c_str();
                eventDataInterface->modifyServiceStringField(login_->userToken(), model_->id, field, stringField);
                std::cout << "\n case observations \n";
                changeServiceObservationsBtn_->setHidden(false);
                confirmServiceObservationsBtn_->setHidden(true);
            }
        }
        catch (const std::exception &e)

        {
            std::cerr << e.what() << std::endl;
        }
    }
}

void ServiceFormView::setData(EventDataModule::ServiceData serviceData)
{
    Wt::WDateTime dateTime = Wt::WDateTime::fromString(serviceData.dateTime, "dd/MM/yyyy HH:mm AP");
    Wt::WTime serviceTime = dateTime.time();
    std::string serviceTimeString = serviceTime.toString("HH:mm AP").toUTF8();

    model_->id = serviceData.id;

    const std::string &providerIdentity = serviceData.providerIdentity;
    const std::string &providerService = serviceData.providerService;

    providerService_->setCurrentIndex(providerService_->findText(serviceData.providerService, Wt::MatchFlag::StringExactly));

    model_->setValue(model_->ServiceStartField, serviceTimeString);
    model_->setValue(model_->ServiceDurationField, serviceData.duration);
    model_->setValue(model_->ServiceCostField, serviceData.cost);
    model_->setValue(model_->ServiceDescriptionField, serviceData.description);
    model_->setValue(model_->ServiceDurationField, serviceData.duration);
    model_->setValue(model_->ServiceObservationsField, serviceData.observations);

    updateView(model_.get());
    if (firstSetData_)
    {
        firstSetData_ = false;

        setReadOnlyAll(true);
        hideServiceChangeBtns(false);

        providerIdentity_->changed().emit();
        providerService_->changed().emit();
        serviceStart_->changed().emit();
        serviceDuration_->changed().emit();
        serviceCost_->changed().emit();
    }
}

bool ServiceFormView::validate()
{

    updateModel(model_.get());
    if (model_->validate())
    {

        return true;
    }
    else
    {

        if (!model_->validateField(model_->ServiceProviderIdentityField))
        {
            setFocus(model_->ServiceProviderIdentityField);
            bindString("submit-info", "provider Identity is not valid");
        }
        else if (!model_->validateField(model_->ServiceProviderServiceField))
        {
            setFocus(model_->ServiceProviderServiceField);
            bindString("submit-info", "provider Service is not valid");
        }
        else if (!model_->validateField(model_->ServiceStartField))
        {
            setFocus(model_->ServiceStartField);
            bindString("submit-info", "service Start is not valid");
        }
        else if (!model_->validateField(model_->ServiceDurationField))
        {
            setFocus(model_->ServiceDurationField);
            bindString("submit-info", "service Duration is not valid");
        }
        else if (!model_->validateField(model_->ServiceCostField))
        {
            setFocus(model_->ServiceCostField);
            bindString("submit-info", "service Cost is not valid");
        }
        else if (!model_->validateField(model_->ServiceDescriptionField))
        {
            setFocus(model_->ServiceDescriptionField);
            bindString("submit-info", "service Description is not valid");
        }
        else if (!model_->validateField(model_->ServiceObservationsField))
        {
            setFocus(model_->ServiceObservationsField);
            bindString("submit-info", "service Observations is not valid");
        }
        return false;
    }
}

// S E R V I C E  ________ W I D G E T
ServiceFormWidget::ServiceFormWidget(std::shared_ptr<Login> login, std::shared_ptr<EventFormModel> eventModel)
    : WTemplate(tr("service-form-widget-template")),
      login_(login)
{
    setMinimumSize(Wt::WLength(412, Wt::LengthUnit::Pixel), Wt::WLength::Auto);
    setMaximumSize(Wt::WLength::Auto, Wt::WLength::Auto);

    setStyleClass("accordion-item");
    headerToggler_ = bindWidget("service-header-toggler", std::make_unique<Wt::WTemplate>(tr("service-header-toggler-template")));
    headerToggler_->setStyleClass("accordion-button collapsed");
    serviceForm_ = bindWidget("service-form", std::make_unique<ServiceFormView>(login_, eventModel));

    headerToggler_->setAttributeValue("data-bs-toggle", "collapse");
    headerToggler_->setAttributeValue("aria-expanded", "false");
    headerToggler_->setAttributeValue("data-bs-target", Wt::WString("#" + serviceForm_->id()).toUTF8());

    headerToggler_->bindEmpty("provider-identity");
    headerToggler_->bindEmpty("provider-service");
    headerToggler_->bindEmpty("service-start");
    headerToggler_->bindEmpty("service-duration");
    headerToggler_->bindEmpty("service-cost");
    bindEmpty("service-menu");

    serviceForm_->providerIdentity_->changed().connect([=]
                                                       { headerToggler_->bindString("provider-identity", serviceForm_->providerIdentity_->valueText()); });
    serviceForm_->providerService_->changed().connect([=]
                                                      { headerToggler_->bindString("provider-service", serviceForm_->providerService_->valueText()); });

    serviceForm_->serviceStart_->changed().connect([=]
                                                   { headerToggler_->bindString("service-start", serviceForm_->serviceStart_->valueText()); });

    serviceForm_->serviceDuration_->changed().connect([=]
                                                      { headerToggler_->bindString("service-duration", serviceForm_->serviceDuration_->valueText()); });

    serviceForm_->serviceCost_->changed().connect([=]
                                                  { headerToggler_->bindString("service-cost", serviceForm_->serviceCost_->valueText()); });
}

void ServiceFormWidget::setSettingsMenu()
{
    settingsBtn_ = bindWidget("service-menu", std::make_unique<Wt::WPushButton>("<i class=\"bi bi-three-dots-vertical\"></i>", Wt::TextFormat::UnsafeXHTML));

    auto popupMenu = std::make_unique<Wt::WPopupMenu>();
    popupMenu->addStyleClass("dropdown-menu-dark");
    popupMenu->addItem("Delete Service")->triggered().connect(this, [=]()
                                                              { deleteService_.emit(serviceForm_->model_->getData()); });
    settingsBtn_->setMenu(std::move(popupMenu));
    settingsBtn_->setStyleClass("btn btn-outline-dark btn-sm");
}

ServiceDialog::ServiceDialog(std::shared_ptr<Login> login, EventDataModule::ServiceData serviceData)
    : WDialog("Add Service"),
      login_(login)
{

    show();
}