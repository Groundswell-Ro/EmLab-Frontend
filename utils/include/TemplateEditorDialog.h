#pragma once
#include <Wt/WTextArea.h>
#include <Wt/WLineEdit.h>
#include <Wt/WString.h>
#include <Wt/WTreeNode.h>
#include <Wt/WTemplate.h>
#include "tinyxml2.h"
#include <Wt/WDialog.h>

class TemplateEditorDialog : public Wt::WDialog
{
public:
    TemplateEditorDialog(std::string fileName, std::string templateName, Wt::WTemplate* parent_template = nullptr);
    ~TemplateEditorDialog();
private:
    void parseMessageAndDoc();
    Wt::WTextArea* textarea_default_styles_;
    Wt::WTextArea* textarea_default_hover_styles_;
    Wt::WTextArea* textarea_dark_styles_;
    Wt::WTextArea* textarea_dark_hover_styles_;
    Wt::WTextArea* textarea_content_text_;

    tinyxml2::XMLDocument doc;
    tinyxml2::XMLElement* tempMessage;

    tinyxml2::XMLElement* selectedElement;
    
    Wt::WString sourcePath_ = "resources/xml/";
    Wt::WString templateName_ = "";


    Wt::WTreeNode* selectedNode;

    Wt::WTemplate* content_temp_;
    Wt::WTemplate* titlebar_temp_;

    void createDialogTitleBar();
    void createDialogContent();
    
    std::unique_ptr<Wt::WTreeNode> createNodeTree(tinyxml2::XMLElement* element);

    void saveStyles();
    void saveContent();

    void nodeSelected(tinyxml2::XMLElement* element);
    void setDisplayElement();
    
    void toggleOutline(bool on = true);
    bool outline_selected_on = true;
    bool outline_parent_on = true;
    
    void addSiblingElementAfter();
    void addSiblingElementBefore();
    void addChildElementFirst();
    void addChildElementLast();
    void removeElement();

    void updateFile();
    void updateView();
    
    bool on_display_default_ = true;
    bool on_display_default_hover_ = true;
    bool on_display_dark_ = true;
    bool on_display_dark_hover_ = true;
    bool on_display_content = true;
};