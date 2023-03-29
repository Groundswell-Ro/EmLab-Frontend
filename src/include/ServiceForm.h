#pragma once
#include "LoginObj.h"
#include "../../../comunication/EventDataModule.h"
#include "EventForm.h"

#include <Wt/WFormModel.h>
#include <Wt/WTemplateFormView.h>
#include <Wt/WStandardItemModel.h>

#include <Wt/WDialog.h>

#include <Wt/WLineEdit.h>
#include <Wt/WTimeEdit.h>
#include <Wt/WDateEdit.h>
#include <Wt/WPushButton.h>
#include <Wt/WDoubleSpinBox.h>
#include <Wt/WTextArea.h>
#include <Wt/WComboBox.h>

#include <Wt/WValidator.h>

typedef std::map<std::string, std::string> ProviderMap;
typedef std::map<std::string, std::vector<std::string>> ServiceMap;

// S E R V I C E ________ M O D E L
class ServiceFormModel : public Wt::WFormModel
{
public:
    ServiceFormModel(std::shared_ptr<EventFormModel> eventModel, std::shared_ptr<Login> login);

    std::shared_ptr<Login> login_;
    std::shared_ptr<EventFormModel> eventModel_;

    // Associate each field with a unique string literal.
    static const Field ServiceProviderIdentityField;
    static const Field ServiceProviderServiceField;
    static const Field ServiceStartField;
    static const Field ServiceDurationField;
    static const Field ServiceCostField;
    static const Field ServiceDescriptionField;
    static const Field ServiceObservationsField;
    int id = 0;

    // from here, handles the providers and the services
    void updateProviderServicesModel(const std::string &providerCode);
    std::string providerCode(int row);
    int providerModelRow(const std::string &code);

    std::shared_ptr<Wt::WAbstractItemModel> providerModel() { return providerModel_; }
    std::shared_ptr<Wt::WAbstractItemModel> serviceModel() { return serviceModel_; }

    void initializeModels();

    EventDataModule::ServiceData getData();

    std::string getDateTime();

    Wt::WString dateFormat_ = "dd/MM/yyyy";
    Wt::WString timeFormat_ = "HH:mm AP";

    ProviderMap providersMap;
    ServiceMap servicesMap;

private:
    std::shared_ptr<Wt::WValidator> createTextValidator(bool mandatory, int min, int max);
    std::shared_ptr<Wt::WValidator> createDoubleValidator(double min, double max);
    std::shared_ptr<Wt::WValidator> createIntValidator(int min, int max);
    std::shared_ptr<Wt::WValidator> createMandatoryValidator();

    std::shared_ptr<Wt::WStandardItemModel> providerModel_, serviceModel_;
};

// S E R V I C E ________ V I E W
class ServiceFormView : public Wt::WTemplateFormView
{
public:
    ServiceFormView(std::shared_ptr<Login> login, std::shared_ptr<EventFormModel> eventModel);

    std::shared_ptr<Login> login_;
    std::shared_ptr<ServiceFormModel> model_;
    std::shared_ptr<EventFormModel> eventModel_;

    Wt::WComboBox *providerIdentity_;
    Wt::WComboBox *providerService_;
    Wt::WTimeEdit *serviceStart_;
    Wt::WDoubleSpinBox *serviceDuration_;
    Wt::WSpinBox *serviceCost_;
    Wt::WTextArea *serviceDescription_;
    Wt::WTextArea *serviceObservations_;

    Wt::WPushButton *changeProviderIdentityBtn_;
    Wt::WPushButton *changeProviderServiceBtn_;
    Wt::WPushButton *changeServiceStartBtn_;
    Wt::WPushButton *changeServiceDurationBtn_;
    Wt::WPushButton *changeServiceCostBtn_;
    Wt::WPushButton *changeServiceDescriptionBtn_;
    Wt::WPushButton *changeServiceObservationsBtn_;

    Wt::WPushButton *confirmProviderIdentityBtn_;
    Wt::WPushButton *confirmProviderServiceBtn_;
    Wt::WPushButton *confirmServiceStartBtn_;
    Wt::WPushButton *confirmServiceDurationBtn_;
    Wt::WPushButton *confirmServiceCostBtn_;
    Wt::WPushButton *confirmServiceDescriptionBtn_;
    Wt::WPushButton *confirmServiceObservationsBtn_;

    void hideServiceChangeBtns(bool hide = true);
    void hideServiceConfirmBtns(bool hide = true);
    void setReadOnlyAll(bool readOnly = true);

    void changeServiceData(EventDataModule::ServiceField field);
    void confirmServiceData(EventDataModule::ServiceField field);

    void setFieldsSignals();

    void setData(EventDataModule::ServiceData serviceData);
    bool validate();

    bool firstSetData_ = true;

private:
};

// Service Form Widget
class ServiceFormWidget : public Wt::WTemplate
{
public:
    ServiceFormWidget(std::shared_ptr<Login> login, std::shared_ptr<EventFormModel> eventModel);
    std::shared_ptr<Login> login_;
    ServiceFormView *serviceForm_;

    Wt::WTemplate *headerToggler_;
    Wt::WPushButton *settingsBtn_;

    void setSettingsMenu();
    Wt::Signal<EventDataModule::ServiceData> &deleteService() { return deleteService_; }

private:
    Wt::Signal<EventDataModule::ServiceData> deleteService_;
};

class ServiceDialog : public Wt::WDialog
{
public:
    ServiceDialog(std::shared_ptr<Login> login, EventDataModule::ServiceData serviceData);
    std::shared_ptr<Login> login_;

private:
};