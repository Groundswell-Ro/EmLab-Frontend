#pragma once
#include "../../../comunication/EventDataModule.h"
#include "LoginObj.h"
#include "EventForm.h"

#include <Wt/WFormModel.h>
#include <Wt/WTemplateFormView.h>
#include <Wt/WSuggestionPopup.h> // for client sugestion
#include <Wt/WLineEdit.h>
#include <Wt/WPushButton.h>

// C L I E N T _____________ M O D E L
class ClientFormModel : public Wt::WFormModel
{
public:
    ClientFormModel();

    // Associate each field with a unique string literal.
    static const Field ClientNameField;
    static const Field ClientPhoneNumberField;
    int id = 0;

    EventDataModule::ClientData getData();

private:
    std::shared_ptr<Wt::WValidator> createTextValidator(bool mandatory, int min, int max);
};

// C L I E N T ____________ V I E W
class ClientFormView : public Wt::WTemplateFormView
{
public:
    ClientFormView(std::shared_ptr<Login> login, std::shared_ptr<EventFormModel> eventModel);

    std::shared_ptr<Login> login_;
    std::shared_ptr<EventFormModel> eventModel_;

    Wt::WSuggestionPopup *clientNameSugestion_;
    Wt::WSuggestionPopup *clientPhoneSugestion_;

    Wt::WPushButton *clearBtn_;
    Wt::WPushButton *confirmBtn_;

    Wt::WLineEdit *clientName_;
    Wt::WLineEdit *clientPhone_;

    std::shared_ptr<ClientFormModel> model_;
    EventDataModule::SeqClients seqClients_;

    Wt::WString lastClientName_;
    Wt::WString lastClientPhone_;

    void setData(EventDataModule::ClientData clientData);
    void process();
    bool validate();

    // Wt::Signal<int> &clientSelected() { return clientSelected_; }
    Wt::Signal<Wt::WString, int> &clientChanged() { return clientChanged_; }
    bool firstSetData_ = true;

private:
    void clientPhoneChanged(Wt::WString currentValue);
    void clientNameChanged(Wt::WString currentValue);
    void clientSelected(int pos, Wt::WFormWidget *formWidget);

    // create sugestion popups
    void createSugestionPopups();
    void resetSuggestions();
    void clearData();
    void setSignals();
    // Wt::Signal<int> clientSelected_;
    Wt::Signal<Wt::WString, int> clientChanged_;
};