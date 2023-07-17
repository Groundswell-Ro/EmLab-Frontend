#include "include/TemplateEditorDialog.h"
#include <Wt/WTree.h>
#include <Wt/WApplication.h>
#include <Wt/WTemplate.h>
#include <Wt/WTreeNode.h>
#include <iostream>
#include <Wt/WCheckBox.h>
#include <Wt/WPushButton.h>


// some utilities
std::string getNextWord(const std::string& input, size_t& currentPosition) {
    std::stringstream ss(input.substr(currentPosition));
    std::string word;
    ss >> word;
    currentPosition += word.length(); // +1 to account for the space or newline character
    return word;
}

std::string cleanStringStartEnd(const std::string& input)
{
	std::string newString = input;
	auto found = newString.find("?");
	while(found != std::string::npos){
		newString.erase(found, 1);
		found = newString.find("?");
	}
	// remove newline from start
	while(newString[0] == '\n'){
		newString.erase(0, 1);
	}
	// remove newline from the end
	while(newString[newString.size() - 1] == '\n'){
		newString.erase(newString.size() - 1, 1);
	}
	// remove whitespace from start
	while(newString[0] == ' '){
		newString.erase(0, 1);
	}
	// remove whitespace from the end
	while(newString[newString.size() - 1] == ' '){
		newString.erase(newString.size() - 1, 1);
	}
	return newString;
}



TemplateEditorDialog::TemplateEditorDialog(std::string fileName, std::string templateName, Wt::WTemplate* parent_template)
{

    sourcePath_ += fileName + ".xml";
    templateName_ = templateName;
	setStyleClass("stylus basic-widget p-0");
	// setPositionScheme(Wt::PositionScheme::Absolute);
	rejectWhenEscapePressed();
	setModal(false);
	setResizable(true);

	content_temp_ = contents()->addWidget(std::make_unique<Wt::WTemplate>(tr("stylus-dialog-template")));

	titleBar()->clear();
	titlebar_temp_ = titleBar()->addWidget(std::make_unique<Wt::WTemplate>(tr("stylus-dialog-titlebar-template")));

    parseMessageAndDoc();
	createDialogTitleBar();
    createDialogContent();
	updateFile();
	updateView();
	
	if(outline_parent_on && parent_template != nullptr){
		
		auto outline_parent_checkbox = titlebar_temp_->bindWidget("outline-parent-checkbox", std::make_unique<Wt::WCheckBox>("Template"));
		outline_parent_checkbox->setChecked(outline_parent_on);
		titlebar_temp_->setCondition("parent", true);
		parent_template->addStyleClass("?"); 
		outline_parent_checkbox->changed().connect(this, [=](){

			outline_parent_on = !outline_parent_on;
			if(outline_parent_on){
				parent_template->addStyleClass("?");
			}else {
				parent_template->removeStyleClass("?");
			}
		});

	}

    finished().connect([=] {
        std::string selectedClasses = cleanStringStartEnd(selectedElement->Attribute("class"));
		selectedElement->SetAttribute("class", selectedClasses.c_str());
		updateFile();
		updateView();
		if(outline_parent_on && parent_template != nullptr) 
			parent_template->removeStyleClass("?");
    });
	setHidden(false, Wt::WAnimation(Wt::AnimationEffect::SlideInFromBottom, Wt::TimingFunction::Linear, 200));


}

TemplateEditorDialog::~TemplateEditorDialog()
{
	auto cleanStyles = cleanStringStartEnd(selectedElement->Attribute("class"));
	selectedElement->SetAttribute("class", cleanStyles.c_str());
	updateFile();
	updateView();
}

void TemplateEditorDialog::createDialogTitleBar()
{
	auto remove_element_btn = titlebar_temp_->bindWidget("remove", std::make_unique<Wt::WPushButton>("remove"));
	auto add_child_before_btn = titlebar_temp_->bindWidget("add-child-before", std::make_unique<Wt::WPushButton>("+ child first"));
	auto add_child_after_btn = titlebar_temp_->bindWidget("add-child-after", std::make_unique<Wt::WPushButton>("+ child last"));
	auto add_sibling_before_btn = titlebar_temp_->bindWidget("add-sibling-before", std::make_unique<Wt::WPushButton>("+ sibling bef"));
	auto add_sibling_after_btn = titlebar_temp_->bindWidget("add-sibling-after", std::make_unique<Wt::WPushButton>("+ sibling aft"));
	auto outline_selected_checkbox = titlebar_temp_->bindWidget("outline-selected-checkbox", std::make_unique<Wt::WCheckBox>("Selected"));
	outline_selected_checkbox->setChecked(outline_selected_on);

	remove_element_btn->doubleClicked().connect(this, &TemplateEditorDialog::removeElement);
	add_child_before_btn->clicked().connect(this, &TemplateEditorDialog::addChildElementFirst);
	add_child_after_btn->clicked().connect(this, &TemplateEditorDialog::addChildElementLast);
	add_sibling_before_btn->clicked().connect(this, &TemplateEditorDialog::addSiblingElementBefore);
	add_sibling_after_btn->clicked().connect(this, &TemplateEditorDialog::addSiblingElementAfter);

	outline_selected_checkbox->changed().connect(this, [=](){ 
		outline_selected_on = !outline_selected_on;
		toggleOutline(outline_selected_on);
		updateFile();
		updateView();
	});



}

void TemplateEditorDialog::createDialogContent()
{
	// stylus tree here
	auto tree = content_temp_->bindWidget("tree-view", std::make_unique<Wt::WTree>());
	tree->addStyleClass("max-w-[125px] min-w-[125px]");
	tree->setSelectionMode(Wt::SelectionMode::Single);
	
	auto rootPtr = std::make_unique<Wt::WTreeNode>("root");
	auto stylusElement = tempMessage->FirstChildElement();
	auto element = stylusElement->NextSiblingElement();
	selectedElement = element;
	while(element){
		if(element->Attribute("class") == NULL){
			element->SetAttribute("class", "");
		}
		if(element->GetText() == NULL){
			element->SetText("");
		}
		rootPtr->addChildNode(createNodeTree(element));


		element = element->NextSiblingElement();
	}

	Wt::WTreeNode *root = rootPtr.get(); 
	tree->setTreeRoot(std::move(rootPtr));
	root->setSelectable(false); 
	root->label()->setTextFormat(Wt::TextFormat::Plain);
	root->setLoadPolicy(Wt::ContentLoading::NextLevel);
	root->expand();	
	
	// tree->select(selectedNode, true);

	// text areas for default and dark classes and text content
	auto default_styles_temp = content_temp_->bindWidget("default-styles-display", std::make_unique<Wt::WTemplate>(tr("stylus-display-category-template")));
	textarea_default_styles_ = default_styles_temp->bindWidget("text-display", std::make_unique<Wt::WTextArea>(""));
	
	auto default_hover_styles_temp = content_temp_->bindWidget("default-hover-styles-display", std::make_unique<Wt::WTemplate>(tr("stylus-display-category-template")));
	textarea_default_hover_styles_ = default_hover_styles_temp->bindWidget("text-display", std::make_unique<Wt::WTextArea>(""));

	auto dark_styles_temp = content_temp_->bindWidget("dark-styles-display", std::make_unique<Wt::WTemplate>(tr("stylus-display-category-template")));
	textarea_dark_styles_ = dark_styles_temp->bindWidget("text-display", std::make_unique<Wt::WTextArea>(""));

	auto dark_hover_styles_temp = content_temp_->bindWidget("dark-hover-styles-display", std::make_unique<Wt::WTemplate>(tr("stylus-display-category-template")));
	textarea_dark_hover_styles_ = dark_hover_styles_temp->bindWidget("text-display", std::make_unique<Wt::WTextArea>(""));

	auto text_content_temp= content_temp_->bindWidget("text-content-display", std::make_unique<Wt::WTemplate>(tr("stylus-display-category-template")));
	textarea_content_text_ = text_content_temp->bindWidget("text-display", std::make_unique<Wt::WTextArea>(""));
	textarea_content_text_->setStyleClass("w-full");

	// title of display textarea
	default_styles_temp->bindString("title", "Default Classes");
	default_hover_styles_temp->bindString("title", "Hover Classes");
	dark_styles_temp->bindString("title", "Dark Classes");
	dark_hover_styles_temp->bindString("title", "Hover Classes");
	text_content_temp->bindString("title", "Text Content");



	// checkboxes to change displayed textareas
	auto default_styles_checkbox = titlebar_temp_->bindWidget("display-default-checkbox", std::make_unique<Wt::WCheckBox>("Default"));
	auto default_hover_styles_checkbox = titlebar_temp_->bindWidget("display-default-hover-checkbox", std::make_unique<Wt::WCheckBox>("Hover"));
	auto dark_styles_checkbox = titlebar_temp_->bindWidget("display-dark-checkbox", std::make_unique<Wt::WCheckBox>("Dark"));
	auto dark_styles_hover_checkbox = titlebar_temp_->bindWidget("display-dark-hover-checkbox", std::make_unique<Wt::WCheckBox>("Hover"));
	auto text_content_checkbox = titlebar_temp_->bindWidget("display-text-content-checkbox", std::make_unique<Wt::WCheckBox>("Text Content"));



	// textarea processing
	textarea_default_styles_->enterPressed().connect(this, [=](){
		saveStyles();
		textarea_default_styles_->setFocus();
	});

	textarea_default_hover_styles_->enterPressed().connect(this, [=](){
		saveStyles();
		textarea_default_hover_styles_->setFocus();
	});

	textarea_dark_styles_->enterPressed().connect(this, [=](){
		saveStyles();
		textarea_dark_styles_->setFocus();
	});

	textarea_dark_hover_styles_->enterPressed().connect(this, [=](){
		saveStyles();
		textarea_dark_hover_styles_->setFocus();
	});

	textarea_content_text_->enterPressed().connect(this, [=](){
		saveContent();
		textarea_content_text_->setFocus();
	});

	// checkbox processing
	default_styles_checkbox->changed().connect(this, [=](){
		on_display_default_ = default_styles_checkbox->isChecked();
		default_styles_temp->setHidden(!on_display_default_);
		std::cout << "\n\n styles_textarea_on: " << on_display_default_ << "\n\n";
	});

	default_hover_styles_checkbox->changed().connect(this, [=](){
		on_display_default_hover_ = default_hover_styles_checkbox->isChecked();
		default_hover_styles_temp->setHidden(!on_display_default_hover_);
	});

	dark_styles_checkbox->changed().connect(this, [=](){
		on_display_dark_ = dark_styles_checkbox->isChecked();
		dark_styles_temp->setHidden(!on_display_dark_);
	});

	dark_styles_hover_checkbox->changed().connect(this, [=](){
		on_display_dark_hover_ = dark_styles_hover_checkbox->isChecked();
		dark_hover_styles_temp->setHidden(!on_display_dark_hover_);
	});

	text_content_checkbox->changed().connect(this, [=](){
		on_display_content = text_content_checkbox->isChecked();
		text_content_temp->setHidden(!on_display_content);
	});
	
	// set the checkboxes to the correct state
	if(on_display_default_){
		default_styles_checkbox->setChecked(on_display_default_);
	}
	if(on_display_default_hover_){
		default_hover_styles_checkbox->setChecked(on_display_default_hover_);
	}
	if(on_display_dark_){
		dark_styles_checkbox->setChecked(on_display_dark_);
	}
	if(on_display_dark_hover_){
		dark_styles_hover_checkbox->setChecked(on_display_dark_hover_);
	}
	if(on_display_content){
		text_content_checkbox->setChecked(on_display_content);
	}
		default_styles_checkbox->changed().emit();
		dark_styles_checkbox->changed().emit();
		text_content_checkbox->changed().emit();

	textarea_default_styles_->setFocus();
	// nodeSelected(selectedElement);

}

std::unique_ptr<Wt::WTreeNode> TemplateEditorDialog::createNodeTree(tinyxml2::XMLElement* element)
{
	if(element->Attribute("class") == NULL){
			element->SetAttribute("class", "");
		}
	if(element->GetText() == NULL){
		element->SetText("");
	}
	auto node = std::make_unique<Wt::WTreeNode>(element->Name()); 
	node->label()->setTextFormat(Wt::TextFormat::Plain);
	node->setLoadPolicy(Wt::ContentLoading::NextLevel);
	node->selected().connect(this, [=](){
		nodeSelected(element);
	});
	// Check if the element has child elements
	tinyxml2::XMLElement* child = element->FirstChildElement();
		// child valid and node can be added
	while (child) {
		auto nodePtr = node->addChildNode(createNodeTree(child));
		nodePtr->selected().connect(this, [=](){
			nodeSelected(child);
		});
		child = child->NextSiblingElement();
	}

	return node;
}

void TemplateEditorDialog::nodeSelected(tinyxml2::XMLElement* element)
{
	std::cout << "\n\n node selected \n\n";
	if(outline_selected_on){
		toggleOutline(false);
		selectedElement = element;
		toggleOutline(true);
		updateFile();
		updateView();
	}else {
		selectedElement = element;
	}
	setDisplayElement();
}

void TemplateEditorDialog::toggleOutline(bool on)
{
	std::string currentStyles = cleanStringStartEnd(selectedElement->Attribute("class"));

	if(on){
		currentStyles = "? " + currentStyles;
	}else {
		currentStyles = cleanStringStartEnd(currentStyles);
	}

	selectedElement->SetAttribute("class", currentStyles.c_str());

}

// get the correct message from the xml file and template id (message id)
void TemplateEditorDialog::parseMessageAndDoc()
{
	auto result = doc.LoadFile(sourcePath_.toUTF8().c_str());
	if(result != tinyxml2::XML_SUCCESS){
		std::cout << "\n\n error opening document of the message template \n\n";
	}
	tinyxml2::XMLElement* root = doc.RootElement();
	if(!root) {
		std::cout << "\n\n error getting root element of the message template \n\n";
	}
	tempMessage = root->FirstChildElement("message");
	while(tempMessage->Attribute("id") != templateName_.toUTF8()){
		tempMessage = tempMessage->NextSiblingElement("message");
	}
}


// handle adding and removing elements
void TemplateEditorDialog::addSiblingElementBefore()
{
	std::cout << "\n\n add sibling element got called \n\n";

	// create element <div class="text-center">header</div>
	auto newElement = doc.NewElement("div");
	newElement->SetAttribute("class", "");
	newElement->SetText("siblings before");
	// add it to the selected element
	selectedElement->Parent()->InsertAfterChild(selectedElement->PreviousSiblingElement(), newElement);

	
	updateFile();
	updateView();
    createDialogContent();
}

void TemplateEditorDialog::addSiblingElementAfter()
{
	std::cout << "\n\n add sibling element got called \n\n";

	// create element <div class="text-center">header</div>
	auto newElement = doc.NewElement("div");
	newElement->SetAttribute("class", "");
	newElement->SetText("siblings after");
	// add it to the selected element
	selectedElement->Parent()->InsertAfterChild(selectedElement, newElement);

	
	updateFile();
	updateView();
	createDialogContent();
}

void TemplateEditorDialog::addChildElementFirst()
{
	std::cout << "\n\n add sibling element got called \n\n";

	// create element <div class="text-center">header</div>
	auto newElement = doc.NewElement("div");
	newElement->SetAttribute("class", "");
	newElement->SetText("child before");
	// add it to the selected element
	selectedElement->InsertFirstChild(newElement);

	updateFile();
	updateView();
	createDialogContent();

}

void TemplateEditorDialog::addChildElementLast()
{

	auto newElement = doc.NewElement("div");
	newElement->SetAttribute("class", "");
	newElement->SetText("child after");
	// add it to the selected element
	selectedElement->InsertEndChild(newElement);

	updateFile();
	updateView();
    createDialogContent();
}

void TemplateEditorDialog::removeElement()
{
	std::cout << "\n\n remove element got called \n\n";
    doc.DeleteNode(selectedElement);
	updateFile();
	updateView();
    createDialogContent();
}

// saving display with below functions
void TemplateEditorDialog::saveContent()
{
	std::cout << "\n\n set element content got called \n\n";
	auto text = textarea_content_text_->text().toUTF8();
	std::cout << "\n\n text: " << text << "\n\n";
	selectedElement->SetText(text.c_str());
	updateFile();
	updateView();
}

void TemplateEditorDialog::saveStyles()
{
	auto defaultClasses = cleanStringStartEnd(textarea_default_styles_->text().toUTF8());
	auto defaultHoverClasses = cleanStringStartEnd(textarea_default_hover_styles_->text().toUTF8());
	auto darkClasses = cleanStringStartEnd(textarea_dark_styles_->text().toUTF8());
	auto darkHoverClasses = cleanStringStartEnd(textarea_dark_hover_styles_->text().toUTF8());
	
	textarea_default_styles_->setText("");
	textarea_default_hover_styles_->setText("");
	textarea_dark_styles_->setText("");
	textarea_dark_hover_styles_->setText("");

	auto hoverPrefix = "hover:";
	auto darkPrefix = "dark:";
	auto darkHoverPrefix = "dark:hover:";

	std::cout << "\n\n styles saved and they are :\n";
	std::cout << "\n defaultClasses: <" << defaultClasses << ">";
	std::cout << "\n defaultHoverClasses: <" << defaultHoverClasses << ">";
	std::cout << "\n darkClasses: <" << darkClasses << ">";

	std::string elementStyles = "";
	for(size_t i = 0; i < defaultClasses.size(); i++){
		elementStyles += getNextWord(defaultClasses, i) + " ";
	}
	for(size_t i = 0; i < defaultHoverClasses.size(); i++){
		elementStyles += hoverPrefix + getNextWord(defaultHoverClasses, i) + " ";
	}
	for(size_t i = 0; i < darkClasses.size(); i++){
		elementStyles += darkPrefix + getNextWord(darkClasses, i) + " ";
	}
	for(size_t i = 0; i < darkHoverClasses.size(); i++){
		elementStyles += darkHoverPrefix + getNextWord(darkHoverClasses, i) + " ";
	}

	std::cout << "\n\n\n-------\n" << elementStyles << "\n------\n\n\n";
	std::string currentStyles = cleanStringStartEnd(selectedElement->Attribute("class"));

	selectedElement->SetAttribute("class", elementStyles.c_str());
	updateFile();
	updateView();
}


// set element display from element xml
void TemplateEditorDialog::setDisplayElement()
{

	std::string classString = cleanStringStartEnd(selectedElement->Attribute("class"));
	std::string defaultClasses_string = "";
	std::string defaultHoverClasses_string = "";
	std::string darkClasses_string = "";
	std::string darkHoverClasses_string = "";

	std::string temp;
	for(auto c : classString){
		if(c == ' '){
			if(temp.substr(0, 6) == "dark:hover:"){
				std::cout << "\n temp dark hover: " << temp;
				darkHoverClasses_string += temp.substr(6) + "\n";
			}else if(temp.substr(0, 6) == "hover:"){
				std::cout << "\n temp hover: " << temp;
				defaultHoverClasses_string += temp.substr(6) + "\n";
			}else if(temp.substr(0, 5) == "dark:"){
				std::cout << "\n temp dark: " << temp;
				darkClasses_string += temp.substr(5) + "\n";
			}else{
				std::cout << "\n temp default: " << temp;
				defaultClasses_string += temp + "\n";
			}
			temp = "";
		}else{
			temp += c;
		}
	}
	if(temp.substr(0, 6) == "dark:hover:"){
		darkHoverClasses_string += temp.substr(6) + "\n";
	}else if(temp.substr(0, 6) == "hover:"){
		defaultHoverClasses_string += temp.substr(6) + "\n";
	}else if (temp.substr(0, 5) == "dark:"){
		darkClasses_string += temp.substr(5) + "\n";
	}else {
		defaultClasses_string += temp + "\n";
	}

	// remove dark: from the start of the dark classes
	for(auto c : darkClasses_string){
		if(c == ' '){
			darkClasses_string += temp.substr(5) + "\n";
			temp = "";
		}else{
			temp += c;
		}
	}

	// std::cout << "\n\n ---" << defaultClasses_string << "----\n\n";
	// std::cout << "\n\n ---" << darkClasses_string << "----\n\n";
	textarea_default_styles_->setText(defaultClasses_string);
	textarea_default_hover_styles_->setText(defaultHoverClasses_string);
	textarea_dark_styles_->setText(darkClasses_string);
	textarea_dark_hover_styles_->setText(darkHoverClasses_string);
	textarea_content_text_->setText(selectedElement->GetText());


}

// update file and view
void TemplateEditorDialog::updateFile()
{
	std::cout << "\n Update File \n";
	doc.SaveFile(sourcePath_.toUTF8().c_str());
}

void TemplateEditorDialog::updateView()
{
	std::cout << "\n Update File \n";
	auto app  = Wt::WApplication::instance();
	app->messageResourceBundle().keys("");
	app->refresh();
}


