#include "include/PhotoUploder.h"
#include <cstdlib>
#include <Wt/WCssDecorationStyle.h>
#include <Wt/WApplication.h>

PhotoUploder::PhotoUploder(bool singlePhoto)
{;
    singlePhoto_ = singlePhoto;

    setTemplateText(tr("photo.uploder.template"));
	file_uploder_ = bindWidget("uploader", std::make_unique<Wt::WFileUpload>());

	status_ = bindWidget("status", std::make_unique<Wt::WText>("Click bellow to change photo"));
	uploder_content_ = bindWidget("uploder-content", std::make_unique<Wt::WContainerWidget>());

    // file_uploder_->setMultiple(!singlePhoto);
    file_uploder_->changed().connect(this, &PhotoUploder::uploderChanged);
    file_uploder_->uploaded().connect(this, &PhotoUploder::uploderUploded);
    file_uploder_->fileTooLarge().connect(this, &PhotoUploder::uploderFileToLarge);


    if(!singlePhoto){
        setCondition("if-multiple", true);
        auto add_photo_btn = bindWidget("add-photo-btn", std::make_unique<Wt::WPushButton>("Add Photo"));
        add_photo_btn->addStyleClass("btn w-1/3 m-0 p-0 rounded-md absolute -top-6");
        status_->addStyleClass("");
        status_->setText("aaaa");
        file_uploder_->setDisplayWidget(add_photo_btn);
        uploder_content_->addStyleClass("border border-solid border-gray-300 rounded-md bg-center flex justify-start items-start flex-wrap overflow-visible");

        auto background_style = Wt::WCssDecorationStyle();
        background_style.setBackgroundImage(Wt::WLink("resources/images/profile-cover/photos.png"));
        uploder_content_->setDecorationStyle(background_style);
    }else {
        setSinglePhoto();
    }

}

PhotoUploder::~PhotoUploder()
{
    if(photos_.size() > 0){
        Emlab::deleteFolder(destination.toUTF8(), Wt::WApplication::instance()->sessionId() + "/");
    }
}
 
void PhotoUploder::uploderChanged()
{
    file_uploder_->upload();
}

void PhotoUploder::uploderUploded()
{
    status_->setText("File upload is succesfull.");
    
    Wt::WString source = file_uploder_->spoolFileName();
    Wt::WString clientFileName = file_uploder_->clientFileName().toUTF8();

    // validation
    if(!Emlab::validateImage(file_uploder_->spoolFileName())){
        status_->setText("Photo is not a Photo.");
        status_->toggleStyleClass("!text-red-300", true);
        if(singlePhoto_)
            setSinglePhoto();
        return;
    }
    
    // response
    status_->setText("Photo is valid.");
    status_->toggleStyleClass("!text-red-300", false);
    // transfer
    auto session_id = Wt::WApplication::instance()->sessionId() + "/";
    auto destination_for_session = destination.toUTF8() + session_id;
    auto treansfer_result = Emlab::bytesToImage(Emlab::imageToBytes(source.toUTF8()), destination_for_session + clientFileName.toUTF8());
    if(treansfer_result){
        if(singlePhoto_){
            setSinglePhoto(Wt::WString(destination_for_session + clientFileName).toUTF8());
            single_photo_changed_.emit();
        }
        else{
            std::pair<std::string, std::string> photo = std::make_pair(destination_for_session + clientFileName.toUTF8(), clientFileName.toUTF8());
            photos_.push_back(photo);
            setMultiplePhotosDisplay();
        }
    }else {
        std::cout <<"\n\n transfer result FAILED  <" << treansfer_result << ">\n\n";
    }

}

void PhotoUploder::uploderFileToLarge()
{
    status_->setText("File is too large.");
    status_->toggleStyleClass("!text-red-300", true);
}

Emlab::ImageData PhotoUploder::getImageData()
{
    return Emlab::imageToBytes(destination.toUTF8() + Wt::WApplication::instance()->sessionId() + file_uploder_->clientFileName().toUTF8());
}

// this is used only when the photo uploder is set to a single photo
void PhotoUploder::setSinglePhoto(Wt::WString photo_path)
{

    uploder_content_->clear();

    auto img_btn = uploder_content_->addWidget(std::make_unique<Wt::WPushButton>("Change"));
    auto img_background = Wt::WCssDecorationStyle();
    
    if(photo_path == ""){
        // std::cout << "\n\n setSinglePhoto = '' \n\n";
        img_background.setBackgroundImage(Wt::WLink(default_photo_path_.toUTF8()));
    }else{
        if(photos_.size() > 0){
            Emlab::deleteFile(photos_[0].first);
            photos_.clear();
        }
        std::pair<std::string, std::string> photo = std::make_pair(photo_path.toUTF8(), photo_path.toUTF8());
        photos_.push_back(photo);
        // std::cout << "\n\n setSinglePhoto = " << photo_path.toUTF8() << "\n\n";
        img_background.setBackgroundImage(Wt::WLink(photo_path.toUTF8()));
    } 

        
    img_btn->setDecorationStyle(img_background);
    img_btn->setStyleClass("h-32 w-32 bg-cover !border-none rounded-full object-cover block mx-auto cursor-pointer hover-increse-size !text-transparent hover:!text-white transition-all text-2xl font-bold ");
    file_uploder_->setDisplayWidget(img_btn);
}

void PhotoUploder::setMultiplePhotosDisplay()
{
    uploder_content_->clear();
    for(auto photo : photos_){
        // photos_.push_back(photoPath);
        auto btn_background = Wt::WCssDecorationStyle();
        auto img_btn = uploder_content_->addWidget(std::make_unique<Wt::WPushButton>("Delete"));
        btn_background.setBackgroundImage(Wt::WLink(photo.first));
        img_btn->setDecorationStyle(btn_background);
        status_->toggleStyleClass("!text-red-300", false);
        img_btn->setStyleClass("w-1/3 h-32 bg-cover !border-none rounded-md object-cover cursor-pointer !text-transparent hover:!text-red-800 transition-all text-2xl font-bold ");

        img_btn->clicked().connect([=](){
            uploder_content_->removeWidget(img_btn);
            status_->setText("Photo is deleted.");
            status_->toggleStyleClass("!text-red-300", true);
            photos_.erase(std::remove(photos_.begin(), photos_.end(), photo), photos_.end());
        });
    }

    std::cout << "\n\n";
    for(auto& photo : photos_){
        std::cout << "\n" << photo.first << "\n";
    }
    std::cout << "\n\n";
}