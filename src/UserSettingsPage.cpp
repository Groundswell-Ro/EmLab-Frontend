#include "include/UserSettingsPage.h"
#include "../../comunication/comm/AuthInterface.h"
#include <stdexcept>
#include <Ice/Ice.h>

#include <Wt/WMenu.h>
#include <Wt/WMenuItem.h>
#include <Wt/WStackedWidget.h>
#include <Wt/WLink.h>
#include <Wt/WCssDecorationStyle.h>
#include <Wt/WText.h>
#include <Wt/WPushButton.h>
#include <Wt/WAnimation.h>
#include <Wt/WLineEdit.h>
#include <Wt/WDialog.h>
#include <Wt/WBreak.h>
#include <Wt/WDialog.h>
#include <Wt/WTextArea.h>
#include <Wt/WRegExpValidator.h>
#include <Wt/WLengthValidator.h>
#include <Wt/WMessageBox.h>

UserSettingsPage::UserSettingsPage(std::shared_ptr<Login> login)
:    login_(login)
{
    setStyleClass("mx-auto w-full max-w-7xl overflow-y-auto");

    auto tmp = addWidget(std::make_unique<Wt::WTemplate>(tr("profile-content-widget")));
    tmp->setCondition("if-no-profile", true);
    auto create_profile_btn = tmp->bindWidget("create-profile-btn", std::make_unique<Wt::WPushButton>("create provider profile"));
    auto stack = tmp->bindWidget("stack", std::make_unique<Wt::WStackedWidget>());
    auto menu = tmp->bindWidget("menu", std::make_unique<Wt::WMenu>(stack));
    stack->setStyleClass("flex-grow mt-3");

    
    auto settings_general_tmp = createSettingsGeneralWidget("settings-widget");
    auto change_email_tmp = createChangeEmailWidget("settings-widget");
    // auto change_username_tmp = createChangeUsernameWidget("settings-widget");
    auto change_name_tmp = createChangeNameWidget("settings-widget");
    auto change_phone_tmp = createChangePhoneWidget("settings-widget");
    auto change_password_tmp = createChangePasswordWidget("settings-widget");


    auto menu_item_general = menu->addItem("General", std::move(settings_general_tmp));
    auto menu_item_email = menu->addItem("Change Email", std::move(change_email_tmp));
    // auto menu_item_username = menu->addItem("Change Username", std::move(change_username_tmp));
    auto menu_item_name = menu->addItem("Change Name", std::move(change_name_tmp));
    auto menu_item_phone = menu->addItem("Change Phone", std::move(change_phone_tmp));
    auto menu_item_password = menu->addItem("Change Password", std::move(change_password_tmp));

    Wt::WString menu_item_styles = "bg-body-hover-border simple-menu-item text-bold [&>a]:px-8 [&>a]:py-2";
    menu_item_general->setStyleClass(menu_item_styles);
    menu_item_email->setStyleClass(menu_item_styles);
    // menu_item_username->setStyleClass(menu_item_styles);
    menu_item_name->setStyleClass(menu_item_styles);
    menu_item_phone->setStyleClass(menu_item_styles);
    menu_item_password->setStyleClass(menu_item_styles);

    create_profile_btn->clicked().connect(this, &UserSettingsPage::createProfileDialog);

    menu->select(menu_item_email);
    // createProfileDialog();

}

std::unique_ptr<Wt::WTemplate> UserSettingsPage::createSettingsGeneralWidget(std::string tempName)
{
    auto returnWidget = std::make_unique<Wt::WTemplate>(tr(tempName));
    returnWidget->bindString("title", "general");

    auto tmp = returnWidget->bindWidget("data-form-widget", std::make_unique<Wt::WTemplate>(tr("settings-general-data-widget")));
    

    return returnWidget;
}

// those widgets are created into the settings page
std::unique_ptr<Wt::WTemplate> UserSettingsPage::createChangeEmailWidget(std::string tempName)
{
    auto widget_tmp = std::make_unique<Wt::WTemplate>(tr(tempName));
    widget_tmp->bindString("title", "change email");
    
    auto tmp = widget_tmp->bindWidget("data-form-widget", std::make_unique<Wt::WTemplate>(tr("data-form-widget")));
    tmp->bindString("title", "email");
    tmp->bindString("current-data", login_->user().email);
    tmp->bindEmpty("submit-info");
    tmp->bindWidget("validation-state", std::make_unique<Wt::WTemplate>("<div class=\"text-red-400 font-semibold\">Not Validated</div>"));
    
    auto email_temp = tmp->bindWidget("input", std::make_unique<Wt::WTemplate>(tr("input-template-normal")));
    email_temp->setCondition("if-start-svg", true);
    email_temp->bindWidget("start-svg", std::make_unique<Wt::WTemplate>(tr("email-svg")));
    email_temp->bindString("label", "New Email");
    auto email_input = email_temp->bindWidget("input", std::make_unique<Wt::WLineEdit>());
    email_input->setPlaceholderText("New Email");
    auto email_validator = Wt::WRegExpValidator("(?!(^[.-].*|[^@]*[.-]@|.*\\.{2,}.*)|^.{254}.)([a-zA-Z0-9!#$%&'*+\\/=?^_`{|}~.-]+@)(?!-.*|.*-\\.)([a-zA-Z0-9-]{1,63}\\.)+[a-zA-Z]{2,15}");
    email_validator.setMandatory(true);
    email_validator.setInvalidNoMatchText("Email is not valid address, \"example.address@mail.com\"");
    email_validator.setInvalidBlankText("Email is empty");
    auto submit_btn = tmp->bindWidget("submit-btn", std::make_unique<Wt::WPushButton>("submit"));

    submit_btn->clicked().connect(this, [=](){
        // validate
        auto validation_result = email_validator.validate(email_input->text());
        if(validation_result.state() == Wt::ValidationState::InvalidEmpty){
            tmp->bindString("submit-info", validation_result.message());
            return;
        }else if (validation_result.state() == Wt::ValidationState::Invalid){
            tmp->bindString("submit-info", validation_result.message());
            return;
        }else if (email_input->text() == login_->user().email){
            tmp->bindString("submit-info", "Email is the same as the current one!");
            return;
        }
        // send data
        auto messageBox = addChild(std::make_unique<Wt::WMessageBox>(
                  "Change Email / Identity",
                  "<div>Are your sure you want to try to change your Email ?</div>"
                  "<div>This is also the way you login !!!</div>",
                  Wt::Icon::Information,
                  Wt::StandardButton::Yes | Wt::StandardButton::No));

        messageBox->setModal(false);
        messageBox->buttonClicked().connect([=] {
            if(messageBox->buttonResult() == Wt::StandardButton::Yes){
                auto emailChangeResponse = changeEmail(email_input->text().toUTF8());
                if(emailChangeResponse == Emlab::ChangeUniqueDataResponse::Changed){
                    tmp->bindString("submit-info", "Email changed successfully!");
                    tmp->bindString("current-data", email_input->text());
                }else if(emailChangeResponse == Emlab::ChangeUniqueDataResponse::NotChanged){
                    tmp->bindString("submit-info", "Email not changed!, make a photo and show the developer!:D");
                }else if(emailChangeResponse == Emlab::ChangeUniqueDataResponse::AllreadyExists){
                    tmp->bindString("submit-info", "Email allready exists!");
                }
            }
            removeChild(messageBox);
        });
        messageBox->setOffsets(20, Wt::Side::Top | Wt::Side::Left | Wt::Side::Right);
        messageBox->setHidden(false, Wt::WAnimation(Wt::AnimationEffect::SlideInFromTop, Wt::TimingFunction::Linear, 300));
        // response
        // if(emailChangeResponse == Emlab::ChangeUniqueDataResponse::Changed){
        //     tmp->bindString("submit-info", "Email changed successfully!");
        //     tmp->bindString("current-data", email_input->text());
        // }else if(emailChangeResponse == Emlab::ChangeUniqueDataResponse::NotChanged){
        //     tmp->bindString("submit-info", "Email not changed!");
        // }else if(emailChangeResponse == Emlab::ChangeUniqueDataResponse::AllreadyExists){
        //     tmp->bindString("submit-info", "Email allready exists!");
        // }
    });
    return widget_tmp;
}

std::unique_ptr<Wt::WTemplate> UserSettingsPage::createChangeNameWidget(std::string tempName)
{
    auto widget_tmp = std::make_unique<Wt::WTemplate>(tr(tempName));
    widget_tmp->bindString("title", "change username");
    
    auto tmp = widget_tmp->bindWidget("data-form-widget", std::make_unique<Wt::WTemplate>(tr("data-form-widget")));
    tmp->bindString("title", "name");
    tmp->bindString("current-data", login_->user().name);
    tmp->bindEmpty("submit-info");
    tmp->bindWidget("validation-state", std::make_unique<Wt::WTemplate>("<div class=\"text-red-400 font-semibold\">Not Validated</div>"));

    auto name_temp = tmp->bindWidget("input", std::make_unique<Wt::WTemplate>(tr("input-template-normal")));
    name_temp->setCondition("if-start-svg", true);
    name_temp->bindWidget("start-svg", std::make_unique<Wt::WTemplate>(tr("human-svg")));
    name_temp->bindString("label", "New Name");
    auto name_input = name_temp->bindWidget("input", std::make_unique<Wt::WLineEdit>());
    name_input->setPlaceholderText("New Username");
    auto name_validator = Wt::WRegExpValidator("^[a-zA-Z ]{3,50}$");
    name_validator.setInvalidNoMatchText("Username should be between 3 and 50 characters long and contain only letters");
    name_validator.setInvalidBlankText("Username is empty");

    auto submit_btn = tmp->bindWidget("submit-btn", std::make_unique<Wt::WPushButton>("submit"));
    submit_btn->clicked().connect(this, [=](){
        // validation
        auto validation_result = name_validator.validate(name_input->text());
        if(validation_result.state() == Wt::ValidationState::InvalidEmpty){
            tmp->bindString("submit-info", validation_result.message());
            return;
        }else if (validation_result.state() == Wt::ValidationState::Invalid){
            tmp->bindString("submit-info", validation_result.message());
            return;
        }
        // send data
        try {
            Ice::CommunicatorHolder ich = Ice::initialize();
            auto base = ich->stringToProxy(login_->AUTH_CONN_STRING);
            auto authInterface = Ice::checkedCast<Emlab::AuthInterfacePrx>(base);
            if (!authInterface)
            {
                throw std::runtime_error("Invalid proxy");
            }
            authInterface->setName(login_->userToken(), name_input->text().toUTF8());
            tmp->bindString("submit-info", "Name changed successfully!");
            tmp->bindString("current-data", name_input->text());
        } catch (const std::exception &e) {
            std::cerr << e.what() << std::endl;
            tmp->bindString("submit-info", "Name failed changing!");
        }
    });
return widget_tmp;
}

std::unique_ptr<Wt::WTemplate> UserSettingsPage::createChangePhoneWidget(std::string tempName)
{
auto widget_tmp = std::make_unique<Wt::WTemplate>(tr(tempName));
    widget_tmp->bindString("title", "change phone number");

    auto tmp = widget_tmp->bindWidget("data-form-widget", std::make_unique<Wt::WTemplate>(tr("data-form-widget")));
    tmp->bindString("title", "phone number");
    std::string phone = login_->user().phone;
    phone.insert(4, " ");
    phone.insert(8, " ");
    tmp->bindString("current-data", phone);
    tmp->bindEmpty("submit-info");
    tmp->bindWidget("validation-state", std::make_unique<Wt::WTemplate>("<div class=\"text-red-400 font-semibold\">Not Validated</div>"));
    std::cout << "\n\n\n\n current phone number :" << login_->user().phone << "\n\n\n\n";
    auto phone_temp = tmp->bindWidget("input", std::make_unique<Wt::WTemplate>(tr("input-template-normal")));
    phone_temp->setCondition("if-start-svg", true);
    phone_temp->bindWidget("start-svg", std::make_unique<Wt::WTemplate>(tr("phone-svg")));
    phone_temp->bindString("label", "New Phone Number");
    auto phone_input = phone_temp->bindWidget("input", std::make_unique<Wt::WLineEdit>());
    phone_input->setPlaceholderText("New Phone Number");
    phone_input->setInputMask("9999999999");
    auto phone_validator =  Wt::WRegExpValidator("^\\d{10}$");
    phone_validator.setMandatory(true);
    phone_validator.setInvalidNoMatchText("Phone number should be10 numbers long, 0000 000 000");
    phone_validator.setInvalidBlankText("Phone number is empty");

    auto submit_btn = tmp->bindWidget("submit-btn", std::make_unique<Wt::WPushButton>("submit"));
    submit_btn->clicked().connect(this, [=](){
        // validation
        auto validation_result = phone_validator.validate(phone_input->text());
        if(validation_result.state() == Wt::ValidationState::InvalidEmpty){
            tmp->bindString("submit-info", validation_result.message());
            return;
        }else if (validation_result.state() == Wt::ValidationState::Invalid){
            tmp->bindString("submit-info", validation_result.message());
            return;
        }
        // send data
        try {
            Ice::CommunicatorHolder ich = Ice::initialize();
            auto base = ich->stringToProxy(login_->AUTH_CONN_STRING);
            auto authInterface = Ice::checkedCast<Emlab::AuthInterfacePrx>(base);
            if (!authInterface)
            {
                throw std::runtime_error("Invalid proxy");
            }
            authInterface->changePhone(login_->userToken(), phone_input->text().toUTF8());
            tmp->bindString("submit-info", "Phone number changed successfully!");
            auto phone = phone_input->text().toUTF8();
            phone.insert(4, " ");
            phone.insert(8, " ");
            tmp->bindString("current-data", phone);
        } catch (const std::exception &e) {
            std::cerr << e.what() << std::endl;
            // tmp->bindString("submit-info", "Phone number failed changing!");
        }
    });
return widget_tmp;

}

std::unique_ptr<Wt::WTemplate> UserSettingsPage::createChangePasswordWidget(std::string tempName)
{
    auto widget_tmp = std::make_unique<Wt::WTemplate>(tr(tempName));
    widget_tmp->bindString("title", "change password");
    
    auto tmp = widget_tmp->bindWidget("data-form-widget", std::make_unique<Wt::WTemplate>(tr("data-change-password-widget")));
    tmp->bindString("title", "password");
    tmp->bindEmpty("submit-info");
    tmp->bindEmpty("validation-state");
    tmp->bindEmpty("current-data");
    tmp->bindWidget("password-requirments", std::make_unique<Wt::WTemplate>(tr("password-requirments")));

    auto password_temp = tmp->bindWidget("password-input", std::make_unique<Wt::WTemplate>(tr("input-template-normal")));
    password_temp->setCondition("if-start-svg", true);
    password_temp->setCondition("if-end-svg", true);
    password_temp->bindWidget("start-svg", std::make_unique<Wt::WTemplate>(tr("key-svg")));
    password_temp->bindString("label", "New Password");
    auto password_input = password_temp->bindWidget("input", std::make_unique<Wt::WLineEdit>());
    password_input->setPlaceholderText("New Password");
    password_input->setEchoMode(Wt::EchoMode::Password);
    auto password_eye = password_temp->bindWidget("end-svg", std::make_unique<Wt::WTemplate>(tr("eye-svg")));

    auto new_password_temp = tmp->bindWidget("new_password-input", std::make_unique<Wt::WTemplate>(tr("input-template-normal")));
    new_password_temp->setCondition("if-start-svg", true);
    new_password_temp->setCondition("if-end-svg", true);
    new_password_temp->bindWidget("start-svg", std::make_unique<Wt::WTemplate>(tr("key-svg")));
    new_password_temp->bindString("label", "New Password");
    auto new_password_input = new_password_temp->bindWidget("input", std::make_unique<Wt::WLineEdit>());
    new_password_input->setPlaceholderText("New Password");
    new_password_input->setEchoMode(Wt::EchoMode::Password);
    auto new_password_eye = new_password_temp->bindWidget("end-svg", std::make_unique<Wt::WTemplate>(tr("eye-svg")));
 
    auto new_password_repeat_temp = tmp->bindWidget("new_password-repeat-input", std::make_unique<Wt::WTemplate>(tr("input-template-normal")));
    new_password_repeat_temp->setCondition("if-start-svg", true);
    new_password_repeat_temp->setCondition("if-end-svg", true);
    new_password_repeat_temp->bindWidget("start-svg", std::make_unique<Wt::WTemplate>(tr("key-svg")));
    new_password_repeat_temp->bindString("label", "New Password");
    auto new_password_repeat_input = new_password_repeat_temp->bindWidget("input", std::make_unique<Wt::WLineEdit>());
    new_password_repeat_input->setPlaceholderText("New Password");
    new_password_repeat_input->setEchoMode(Wt::EchoMode::Password);
    auto new_password_repeat_eye = new_password_repeat_temp->bindWidget("end-svg", std::make_unique<Wt::WTemplate>(tr("eye-svg")));

    password_eye->clicked().connect(this, [=](){
        if(password_input->echoMode() == Wt::EchoMode::Password){
            password_input->setEchoMode(Wt::EchoMode::Normal);
            password_eye->bindWidget("svg", std::make_unique<Wt::WTemplate>(tr("eye-slash-svg")));
        }else{
            password_input->setEchoMode(Wt::EchoMode::Password);
            password_eye->bindWidget("svg", std::make_unique<Wt::WTemplate>(tr("eye-svg")));
        }
    });

    new_password_eye->clicked().connect(this, [=](){
        if(new_password_input->echoMode() == Wt::EchoMode::Password){
            new_password_input->setEchoMode(Wt::EchoMode::Normal);
            new_password_eye->bindWidget("svg", std::make_unique<Wt::WTemplate>(tr("eye-slash-svg")));
        }else{
            new_password_input->setEchoMode(Wt::EchoMode::Password);
            new_password_eye->bindWidget("svg", std::make_unique<Wt::WTemplate>(tr("eye-svg")));
        }
    });

    new_password_repeat_eye->clicked().connect(this, [=](){
        if(new_password_repeat_input->echoMode() == Wt::EchoMode::Password){
            new_password_repeat_input->setEchoMode(Wt::EchoMode::Normal);
            new_password_repeat_eye->bindWidget("svg", std::make_unique<Wt::WTemplate>(tr("eye-slash-svg")));
        }else{
            new_password_repeat_input->setEchoMode(Wt::EchoMode::Password);
            new_password_repeat_eye->bindWidget("svg", std::make_unique<Wt::WTemplate>(tr("eye-svg")));
        }
    });


    auto password_validator =  Wt::WRegExpValidator("^(?=.*[A-Za-z])(?=.*\\d)[A-Za-z\\d]{6,}$");
    password_validator.setMandatory(true);
    password_validator.setInvalidNoMatchText("Password must contain at least 6 characters, including letters and numbers");
    password_validator.setInvalidBlankText("field is empty");
    
    auto submit_btn = tmp->bindWidget("submit-btn", std::make_unique<Wt::WPushButton>("submit"));
    submit_btn->clicked().connect(this, [=](){
        // validation
        auto password_validation = password_validator.validate(password_input->text());
        auto new_password_validation = password_validator.validate(new_password_input->text());

        // reset error message display
        password_temp->setCondition("if-error-message", false);
        new_password_temp->setCondition("if-error-message", false);
        new_password_repeat_temp->setCondition("if-error-message", false);

        if(password_validation.state() == Wt::ValidationState::Invalid){
            password_temp->setCondition("if-error-message", true);
            password_temp->bindWidget("error", std::make_unique<Wt::WText>(password_validation.message()));
            return;
        }else if(password_validation.state() == Wt::ValidationState::InvalidEmpty){
            password_temp->setCondition("if-error-message", true);
            password_temp->bindWidget("error", std::make_unique<Wt::WText>(password_validation.message()));
            return;
        }else if(new_password_validation.state() == Wt::ValidationState::Invalid){
            new_password_temp->setCondition("if-error-message", true);
            new_password_temp->bindWidget("error", std::make_unique<Wt::WText>(new_password_validation.message()));
            return;
        } else if(new_password_validation.state() == Wt::ValidationState::InvalidEmpty){
            new_password_temp->setCondition("if-error-message", true);
            new_password_temp->bindWidget("error", std::make_unique<Wt::WText>(new_password_validation.message()));
            return;
        } else if(new_password_input->text() != new_password_repeat_input->text()){
            new_password_repeat_temp->setCondition("if-error-message", true);
            new_password_repeat_temp->bindWidget("error", std::make_unique<Wt::WText>("Passwords do not match"));
            return;
        }if(password_input->text() == new_password_input->text()){
            new_password_temp->setCondition("if-error-message", true);
            tmp->bindWidget("submit-info", std::make_unique<Wt::WText>("<p class='text-red-400'>New password must be different from old password</p>"));
            return;
        }
        std::cout << "\n\n validation passed \n\n";
        
        // send data
        try {
            Ice::CommunicatorHolder ich = Ice::initialize();
            auto base = ich->stringToProxy(login_->AUTH_CONN_STRING);
            auto authInterface = Ice::checkedCast<Emlab::AuthInterfacePrx>(base);
            if (!authInterface)
            {
                throw std::runtime_error("Invalid proxy");
            }
            auto old_password = password_input->text().toUTF8();
            auto new_password = new_password_input->text().toUTF8();
            Emlab::ChangePasswordResponse response = authInterface->changePassword(login_->userToken(), old_password, new_password);
            // respond
            switch(response){
                case Emlab::ChangePasswordResponse::PasswordChanged:
                    tmp->bindString("submit-info", "<p class='text-green-400'>Password changed successfully</p>");
                break;
                case Emlab::ChangePasswordResponse::PasswordNotChanged:
                    tmp->bindString("submit-info", "<p class='text-red-400'>Password failed changing! make a photo and tell the developer to check this out</p>");
                break;
                case Emlab::ChangePasswordResponse::OldPasswordIncorrect:
                    tmp->bindString("submit-info", "<p class='text-red-400'>Old password is incorrect</p>");
                break;
            }

        } catch (const std::exception &e) {
            std::cerr << e.what() << std::endl;
            tmp->bindString("submit-info", "<p class='text-red-400'>Password failed changing! make a photo and tell the developer to check this out</p>");
        }
    });
    return widget_tmp;
}

void UserSettingsPage::createProfileDialog()
{
    auto dialog = addChild(std::make_unique<Wt::WDialog>());
    dialog->setOffsets(20, Wt::Side::Top | Wt::Side::Left | Wt::Side::Right);
    dialog->rejectWhenEscapePressed();
    dialog->setClosable(true);
    // dialog->setTitleBarEnabled(false);

    auto header = dialog->titleBar();
    auto content = dialog->contents();

    header->clear();
    header->setStyleClass("text-center");
    auto title = header->addWidget(std::make_unique<Wt::WText>("Create your provider profile"));
    title->setStyleClass("text-2xl font-semibold my-3");
    dialog->setResizable(false);
    dialog->setMovable(false);

    auto content_temp = content->addWidget(std::make_unique<Wt::WTemplate>(tr("profile-dialog-content")));
    auto close_btn = header->addWidget(std::make_unique<Wt::WPushButton>("X"));
    close_btn->clicked().connect(dialog, &Wt::WDialog::reject);
    close_btn->setStyleClass("btn btn-danger flex justify-center items-center absolute top-0 right-0 rounded-full p-2 m-2 cursor-pointer");

    auto photo_uploder = content_temp->bindWidget("photo-uploder", std::make_unique<PhotoUploder>());
    // login_->getUserPhotoPath() + "/profile.jpg"
    photo_uploder->setSinglePhoto();
    photo_uploder->status_->setText("Is this photo ok as your profile photo?");

    auto username_input_temp = content_temp->bindWidget("username-input", std::make_unique<Wt::WTemplate>(tr("input-template-normal")));
    username_input_temp->setCondition("if-start-svg", true);
    username_input_temp->bindString("label", "Username");
    username_input_temp->bindWidget("start-svg", std::make_unique<Wt::WTemplate>(tr("human-svg")));
    auto username_input = username_input_temp->bindWidget("input", std::make_unique<Wt::WLineEdit>());
    username_input->setAutoComplete(false);


    auto about_self_temp = content_temp->bindWidget("about-self-input", std::make_unique<Wt::WTemplate>(tr("input-template-normal")));
    about_self_temp->bindString("label", "About self as a sevice\'s provider");
    auto about_self_input = about_self_temp->bindWidget("input", std::make_unique<Wt::WTextArea>());
    about_self_input->addStyleClass("resize-none resize-y");
    auto submit_btn = content_temp->bindWidget("submit-btn", std::make_unique<Wt::WPushButton>("Create profile"));


    auto service_title_input = content_temp->bindWidget("service-title-input", std::make_unique<Wt::WTemplate>(tr("input-template-normal")));
    service_title_input->bindString("label", "Service title");
    auto service_title_input_input = service_title_input->bindWidget("input", std::make_unique<Wt::WLineEdit>());
    service_title_input_input->setAutoComplete(false);

    auto service_description_input = content_temp->bindWidget("service-description-input", std::make_unique<Wt::WTemplate>(tr("input-template-normal")));
    service_description_input->bindString("label", "Service description");
    auto service_description_input_input = service_description_input->bindWidget("input", std::make_unique<Wt::WTextArea>());
    service_description_input_input->addStyleClass("resize-none resize-y");

    auto service_photo_uploder = content_temp->bindWidget("service-photo-uploder", std::make_unique<PhotoUploder>(login_->user().email + "/temporary/", false));

    submit_btn->setStyleClass("btn btn-primary");

    dialog->finished().connect([=]{
        if(dialog->result() == Wt::DialogCode::Accepted)
        {
            std::cout << "\n\n dialog accepted protocol here \n\n";
        }else 
        {
            std::cout << "\n\n dialog rejected protocol here \n\n";
        }
        removeWidget(dialog);
    });
    // dialog->show();
    dialog->setHidden(false, Wt::WAnimation(Wt::AnimationEffect::SlideInFromTop, Wt::TimingFunction::EaseInOut, 500));


    submit_btn->clicked().connect(this, [=](){

        // validation
        auto short_validator = Wt::WRegExpValidator("^[a-zA-Z ]{3,50}$");
        auto long_validator = Wt::WLengthValidator(10, 500);
        short_validator.setMandatory(true);
        long_validator.setMandatory(true);
        short_validator.setInvalidBlankText("This field is required");
        long_validator.setInvalidBlankText("This field is required");
        short_validator.setInvalidNoMatchText("Try using just letters and spaces");
        long_validator.setInvalidTooLongText("maximum of 500 characters");
        long_validator.setInvalidTooShortText("minimum of 10 characters");
        
        auto username_validation_result = short_validator.validate(username_input->text());
        auto about_self_validation_result = long_validator.validate(about_self_input->text());
        auto service_title_validation_result = short_validator.validate(service_title_input_input->text());
        auto service_description_validation_result = long_validator.validate(service_description_input_input->text());
    
        username_input_temp->setCondition("if-error-message", true);
        about_self_temp->setCondition("if-error-message", true);
        service_title_input->setCondition("if-error-message", true);
        service_description_input->setCondition("if-error-message", true);

        username_input_temp->bindEmpty("error");
        about_self_temp->bindEmpty("error");
        service_title_input->bindEmpty("error");
        service_description_input->bindEmpty("error");

        switch (username_validation_result.state()) {
            case Wt::ValidationState::InvalidEmpty:
                username_input_temp->bindWidget("error", std::make_unique<Wt::WText>(username_validation_result.message()));
                return;
            case Wt::ValidationState::Invalid:
                username_input_temp->bindWidget("error", std::make_unique<Wt::WText>(username_validation_result.message()));
                return;
            default:
                username_input_temp->setCondition("if-error-message", false);
        }

        switch (about_self_validation_result.state()) {
            case Wt::ValidationState::InvalidEmpty:
                about_self_temp->bindWidget("error", std::make_unique<Wt::WText>(about_self_validation_result.message()));
                return;
            case Wt::ValidationState::Invalid:
                about_self_temp->bindWidget("error", std::make_unique<Wt::WText>(about_self_validation_result.message()));
                return;
            default:
                about_self_temp->setCondition("if-error-message", false);
        }

        switch (service_title_validation_result.state()) {
            case Wt::ValidationState::InvalidEmpty:
                service_title_input->bindWidget("error", std::make_unique<Wt::WText>(service_title_validation_result.message()));
                return;
            case Wt::ValidationState::Invalid:
                service_title_input->bindWidget("error", std::make_unique<Wt::WText>(service_title_validation_result.message()));
                return;
            default:
                service_title_input->setCondition("if-error-message", false);
        }

        switch (service_description_validation_result.state()) {
            case Wt::ValidationState::InvalidEmpty:
                service_description_input->bindWidget("error", std::make_unique<Wt::WText>(service_description_validation_result.message()));
                return;
            case Wt::ValidationState::Invalid:
                service_description_input->bindWidget("error", std::make_unique<Wt::WText>(service_description_validation_result.message()));
                return;
            default:
                service_description_input->setCondition("if-error-message", false);
        }


        // gather data

        // send data

        // responde
    });


}


Emlab::ChangeUniqueDataResponse UserSettingsPage::changeEmail(std::string email)
{
    Emlab::ChangeUniqueDataResponse emailChangeResponse;
     try
        {
            Ice::CommunicatorHolder ich = Ice::initialize();
            auto base = ich->stringToProxy(login_->AUTH_CONN_STRING);
            auto authInterface = Ice::checkedCast<Emlab::AuthInterfacePrx>(base);
            if (!authInterface)
            {
                throw std::runtime_error("Invalid proxy");
            }
            emailChangeResponse = authInterface->changeEmail(login_->userToken(), email);
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << std::endl;
        }   
    return emailChangeResponse;
}