#include "include/PhotoUploder.h"
#include <cstdlib>

PhotoUploder::PhotoUploder(std::string templateName)
 :   WTemplate(tr(templateName))
{

	fileUpload_ = bindWidget("uploader", std::make_unique<Wt::WFileUpload>());
	status_ = bindWidget("status", std::make_unique<Wt::WText>("Click bellow to add picture"));
	photo_ = bindWidget("photo", std::make_unique<Wt::WImage>(Wt::WLink("resources/images/blank-profile-picture.png")));

    fileUpload_->changed().connect(this, &PhotoUploder::uploderChanged);
    fileUpload_->uploaded().connect(this, &PhotoUploder::uploderUploded);
    fileUpload_->fileTooLarge().connect(this, &PhotoUploder::uploderFileToLarge);
}

PhotoUploder::~PhotoUploder()
{
    if(fileUpload_->multiple()){
        std::cout << "\n\n Multiple files have not been implemented yet\n\n";
    }else {
        Emlab::deleteFile(destination.toUTF8() + fileUpload_->clientFileName().toUTF8());
    }
}
 
void PhotoUploder::uploderChanged()
{
    fileUpload_->upload();
}

void PhotoUploder::uploderUploded()
{
    status_->setText("File upload is finished.");
    if(!Emlab::validateImage(fileUpload_->spoolFileName())){
        status_->setText("Photo is not a Photo.");
        setPhoto("resources/images/blank-profile-picture.png");
        status_->toggleStyleClass("!text-red-300", true);
        return;
    }

    status_->setText("Photo is valid.");
    status_->toggleStyleClass("!text-red-300", false);

    Wt::WString source = fileUpload_->spoolFileName();
    Wt::WString clientFileName = fileUpload_->clientFileName().toUTF8();
    

    // Emlab::moveFile(source.toUTF8(), destination.toUTF8() + clientFileName.toUTF8(), clientFileName.toUTF8());
    auto treansfer_result = Emlab::bytesToImage(Emlab::imageToBytes(source.toUTF8()), destination.toUTF8() + clientFileName.toUTF8());
    if(treansfer_result){
        setPhoto(Wt::WString(destination + clientFileName).toUTF8());
    }else {
        std::cout <<"\n\n result <" << treansfer_result << ">\n\n";
    }
}

void PhotoUploder::uploderFileToLarge()
{
    status_->setText("File is too large.");
    status_->toggleStyleClass("!text-red-300", true);
}

Emlab::ImageData PhotoUploder::getImageData()
{
    return Emlab::imageToBytes(destination.toUTF8() + fileUpload_->clientFileName().toUTF8());
}

void PhotoUploder::setPhoto(std::string photoPath)
{
    photo_->setImageLink(Wt::WLink(photoPath));
}
