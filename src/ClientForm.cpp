#include "include/ClientForm.h"

#include <Ice/Ice.h>
#include <stdexcept>

#include <Wt/WLengthValidator.h> // for Name

// Text Validator
std::shared_ptr<Wt::WValidator> ClientFormModel::createTextValidator(bool mandatory, int min, int max)
{
    auto v = std::make_shared<Wt::WLengthValidator>();
    v->setMandatory(mandatory);
    v->setMinimumLength(min);
    v->setMaximumLength(max);
    return v;
}

// Client fields
const Wt::WFormModel::Field ClientFormModel::ClientNameField = "client-name";
const Wt::WFormModel::Field ClientFormModel::ClientPhoneNumberField = "client-phone";

// C L I E N T _____________ M O D E L
ClientFormModel::ClientFormModel()
    : WFormModel()
{
    addField(ClientNameField);
    addField(ClientPhoneNumberField);

    setValidator(ClientNameField, createTextValidator(true, 3, 30));
    setValidator(ClientPhoneNumberField, createTextValidator(true, 10, 10));
}

EventDataModule::ClientData ClientFormModel::getData()
{
    EventDataModule::ClientData clientData;
    clientData.id = id;
    auto clientName = valueText(ClientNameField).toUTF8();

    // make client name lower case
    std::transform(clientName.begin(), clientName.end(), clientName.begin(), ::tolower);

    clientData.name = clientName;
    clientData.phone = valueText(ClientPhoneNumberField).toUTF8();

    return clientData;
}

// C L I E N T ____________ V I E W
ClientFormView::ClientFormView(std::shared_ptr<Login> login)
    : login_(login)
{
    setMinimumSize(Wt::WLength(280, Wt::LengthUnit::Pixel), Wt::WLength::Auto);
    setMaximumSize(Wt::WLength(350, Wt::LengthUnit::Pixel), Wt::WLength::Auto);

    model_ = std::make_shared<ClientFormModel>();
    setTemplateText(tr("client-form-template"));
    addFunction("id", &WTemplate::Functions::id);
    createSugestionPopups();

    /*
     * Client Name Widget
     */
    auto clientNameWidget = std::make_unique<Wt::WLineEdit>();
    clientNameWidget->setPlaceholderText("this is needed for input desighn animation");
    clientName_ = clientNameWidget.get();
    setFormWidget(model_->ClientNameField, std::move(clientNameWidget));
    clientNameSugestion_->forEdit(clientName_);

    /*
     * Client Phone Widget
     */
    auto clientPhoneNumberWidget = std::make_unique<Wt::WLineEdit>();
    clientPhoneNumberWidget->setMaxLength(10);
    clientPhoneNumberWidget->setInputMask("9999999999");
    clientPhone_ = clientPhoneNumberWidget.get();
    clientPhoneNumberWidget->setPlaceholderText("this is needed for input desighn animation");
    setFormWidget(model_->ClientPhoneNumberField, std::move(clientPhoneNumberWidget));
    clientPhoneSugestion_->forEdit(clientPhone_);

    confirmBtn_ = bindWidget("confirm-client-btn", std::make_unique<Wt::WPushButton>("confirm"));
    clearBtn_ = bindWidget("clear-client-btn", std::make_unique<Wt::WPushButton>("change client"));
    bindEmpty("submit-info");

    clearBtn_->hide();

    // connect clientName_ and clientPhone_ changed signal to lineEditChanged
    clientName_->textInput().connect(this, [=]()
                                     { clientNameChanged(clientName_->valueText()); });
    clientPhone_->textInput().connect(this, [=]()
                                      { clientPhoneChanged(clientPhone_->valueText()); });
    confirmBtn_->clicked().connect(this, &ClientFormView::process);
    clearBtn_->clicked().connect(this, &ClientFormView::clearData);
    // react to buttons
    updateView(model_.get());
}

void ClientFormView::setData(EventDataModule::ClientData clientData)
{

    clearBtn_->show();
    confirmBtn_->hide();

    model_->id = clientData.id;
    model_->setValue(model_->ClientNameField, clientData.name);
    model_->setValue(model_->ClientPhoneNumberField, clientData.phone);

    model_->setReadOnly(model_->ClientNameField, true);
    model_->setReadOnly(model_->ClientPhoneNumberField, true);

    updateView(model_.get());
}

void ClientFormView::clearData()
{
    clearBtn_->hide();
    confirmBtn_->show();

    model_->id = 0;
    model_->setValue(model_->ClientNameField, "");
    model_->setValue(model_->ClientPhoneNumberField, "");

    model_->setReadOnly(model_->ClientNameField, false);
    model_->setReadOnly(model_->ClientPhoneNumberField, false);

    model_->validate();

    updateView(model_.get());
}

void ClientFormView::clientNameChanged(Wt::WString currentValue)
{
    if (currentValue.empty())
    {
        return;
    }
    if (seqClients_.size() != 0)
    {
        EventDataModule::SeqClients newSeqClients;
        // search for client with the same lastClientName_ in seqClients_
        for (auto &client : seqClients_)
        {
            // if lastClientName_ is in the begining of client.name then add it to newSeqClients
            if (client.name.find(currentValue.toUTF8()) == 0)
            {
                newSeqClients.push_back(client);
            }
        }
        if (newSeqClients.size() != 0)
        {
            seqClients_ = newSeqClients;
            resetSuggestions();
            return;
        }
    }

    // Ice communication for getting clients from server
    try
    {
        Ice::CommunicatorHolder ich = Ice::initialize();
        auto base = ich->stringToProxy(login_->eventConnString_);
        auto eventsDataInterface = Ice::checkedCast<EventDataModule::EventsDataInterfacePrx>(base);
        if (!eventsDataInterface)
        {
            throw std::runtime_error("Invalid proxy");
        }
        // get clients from server by name
        seqClients_ = eventsDataInterface->getClientsByName(login_->userToken(), currentValue.toUTF8());
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }

    resetSuggestions();
    lastClientName_ = currentValue;
}

void ClientFormView::clientPhoneChanged(Wt::WString currentValue)
{
    if (currentValue.empty())
    {
        return;
    }
    if (seqClients_.size() != 0)
    {
        EventDataModule::SeqClients newSeqClients;
        // search for client with the same lastClientPhone_ in seqClients_
        for (auto &client : seqClients_)
        {
            // if lastClientPhone_ is in the begining of client.phone then add it to newSeqClients
            if (client.phone.find(currentValue.toUTF8()) == 0)
            {
                std::cout << "client.phone: " << client.phone << std::endl;
                newSeqClients.push_back(client);
            }
        }
        if (newSeqClients.size() != 0)
        {
            seqClients_ = newSeqClients;
            resetSuggestions();
            return;
        }
    }

    // Ice communication for getting clients from server
    try
    {
        Ice::CommunicatorHolder ich = Ice::initialize();
        auto base = ich->stringToProxy(login_->eventConnString_);
        auto eventsDataInterface = Ice::checkedCast<EventDataModule::EventsDataInterfacePrx>(base);
        if (!eventsDataInterface)
        {
            throw std::runtime_error("Invalid proxy");
        }
        // get clients from server by phone
        seqClients_ = eventsDataInterface->getClientsByPhone(login_->userToken(), currentValue.toUTF8());
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }

    resetSuggestions();
    lastClientPhone_ = currentValue;
}

void ClientFormView::process()
{
    std::cout << "\n\n process \n\n ";
    // update model
    if (!validate())
    {
        return;
    }

    auto clientData = model_->getData();
    // Ice communication for registering client
    try
    {
        Ice::CommunicatorHolder ich = Ice::initialize();
        auto base = ich->stringToProxy(login_->eventConnString_);
        auto eventsDataInterface = Ice::checkedCast<EventDataModule::EventsDataInterfacePrx>(base);
        if (!eventsDataInterface)
        {
            throw std::runtime_error("Invalid proxy");
        }
        // Try to register Data
        clientData.id = eventsDataInterface->registerClient(login_->userToken(), clientData);
        clientChanged_.emit(Wt::WString(clientData.name), clientData.id);
        setData(clientData);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }

    // update view
    updateView(model_.get());
}

bool ClientFormView::validate()
{
    updateModel(model_.get());
    if (model_->validate())
    {
        return true;
        updateView(model_.get());
    }
    else if (!model_->validateField(model_->ClientNameField))
    {
        clientName_->setFocus();
        updateViewField(model_.get(), model_->ClientNameField);
        bindString("submit-info", Wt::WString("Client name is required"));
    }
    else if (!model_->validateField(model_->ClientPhoneNumberField))
    {
        clientPhone_->setFocus();
        updateViewField(model_.get(), model_->ClientPhoneNumberField);
        bindString("submit-info", Wt::WString("Client phone is required"));
    }
    else
    {
        bindString("submit-info", Wt::WString("something went wrong"));
    }
    return false;
}

void ClientFormView::clientSelected(int pos, Wt::WFormWidget *formWidget)
{
    auto clientData = seqClients_[pos];
    setData(clientData);
    clientChanged_.emit(Wt::WString(clientData.name), clientData.id);
    validate();
    updateView(model_.get());
}

void ClientFormView::createSugestionPopups()
{
    Wt::WSuggestionPopup::Options contactOptions;
    contactOptions.highlightBeginTag = "<span class=\"highlight\">";
    contactOptions.highlightEndTag = "</span>";
    // contactOptions.listSeparator = '\0';
    // contactOptions.whitespace = " \n";
    // contactOptions.wordSeparators = "-., \"@\n;";
    // contactOptions.appendReplacedText = ", ";

    clientNameSugestion_ = addChild(std::make_unique<Wt::WSuggestionPopup>(
        Wt::WSuggestionPopup::generateMatcherJS(contactOptions),
        Wt::WSuggestionPopup::generateReplacerJS(contactOptions)));

    clientPhoneSugestion_ = addChild(std::make_unique<Wt::WSuggestionPopup>(
        Wt::WSuggestionPopup::generateMatcherJS(contactOptions),
        Wt::WSuggestionPopup::generateReplacerJS(contactOptions)));

    clientNameSugestion_->activated().connect(this, &ClientFormView::clientSelected);
    clientPhoneSugestion_->activated().connect(this, &ClientFormView::clientSelected);
}

void ClientFormView::resetSuggestions()
{
    clientNameSugestion_->clearSuggestions();
    clientPhoneSugestion_->clearSuggestions();

    for (int i = 0; i < seqClients_.size(); ++i)
    {
        EventDataModule::ClientData clientData = seqClients_.at(i);
        Wt::WString clientName = clientData.name;
        Wt::WString clientPhone = clientData.phone;

        Wt::WString sugestionNameString{clientName + " | " + clientPhone};
        Wt::WString sugestionPhoneString{clientPhone + " | " + clientName};

        clientNameSugestion_->addSuggestion(sugestionNameString, clientName);
        clientPhoneSugestion_->addSuggestion(sugestionPhoneString, clientPhone);
    }
}
