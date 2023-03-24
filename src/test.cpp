// #include "include/test.h"
// #include <Wt/WText.h>
// #include <Wt/WTemplate.h>
// #include <Wt/WPushButton.h>
// #include <Wt/WImage.h>
// #include "../../comunication/EventDataModule.h"
// #include "../../comunication/AuthModule.h"
// #include <Ice/Ice.h>
// #include <stdexcept>
// #include <Wt/WProgressBar.h>
// // #include <Wt/WFile.h>
// #include <fstream>
// #include <vector>
// #include <iostream>

// using namespace std;

// Test::Test(std::shared_ptr<Login> login)
// 	: WContainerWidget(),
// 	  login_(login)
// {
// 	setStyleClass("test container here :D");
// 	addWidget(std::make_unique<Wt::WImage>());
// 	auto fileUpload_ = addWidget(std::make_unique<Wt::WFileUpload>());
// 	fileUpload_->setProgressBar(std::make_unique<Wt::WProgressBar>());
// 	fileUpload_->setFileTextSize(50);

// 	// set maximum file size to 20 MB
// 	fileUpload_->setFileTextSize(20 * 1024 * 1024);

// 	fileUpload_->setFilters("images/");
// 	fileUpload_->fileTooLarge().connect(this, [=]()
// 										{ std::cout << "\n\n file to large \n\n";
// 		Wt::WString fileLocation = fileUpload_->spoolFileName();
// 				std::cout << "\n\n"
// 				  << fileUpload_->spoolFileName() << "\n\n"; });

// 	// upload file
// 	fileUpload_->uploaded().connect(this, [=]()
// 									{
// 		Wt::WString fileLocation = fileUpload_->spoolFileName();

// 		std::cout << "\n\n File upload is finished.\n\n";
// 		std::cout << "\n\n"
// 				  << fileUpload_->spoolFileName() << "\n\n";

// 		std::cout << "\n\n --------";

// 		ifstream imageFile("/home/alex/Desktop/picTest.jpg", ios::binary);

// 		// Check if the file was successfully opened
// 		if (!imageFile.is_open())
// 		{
// 			std::cout << "Failed to open the image file." << endl;
// 			return;
// 		}

// 		// Get the file size
// 		imageFile.seekg(0, ios::end);
// 		int fileSize = imageFile.tellg();
// 		imageFile.seekg(0, ios::beg);

// 		// Create a vector to store the image data
// 		vector<unsigned char> imageData(fileSize);

// 		// Read the image data into the vector
// 		imageFile.read((char *)imageData.data(), fileSize);

// 		// Close the file
// 		imageFile.close();

// 		// Now you can write the image data to a new file
// 		ofstream outputFile("output.jpg", ios::binary);
// 		outputFile.write((char *)imageData.data(), fileSize);
// 		outputFile.close();

// 		std::cout << "Image stored successfully!" << "\n";
// 		std::cout << "\n\n --------";
// 		AuthModule::profileImageSq asd(imageData.begin(), imageData.end()); });

// 	// submit file
// 	auto uploadBtn = addWidget(std::make_unique<Wt::WPushButton>("upload"));
// 	uploadBtn->clicked().connect(this, [=]()
// 								 { fileUpload_->upload(); });
// }