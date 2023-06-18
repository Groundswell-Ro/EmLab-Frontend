#pragma once
#include "LoginObj.h"
#include <Wt/WContainerWidget.h>
#include <Wt/WTable.h>
#include <Wt/WText.h>
#include <Wt/WTemplate.h>
#include <Wt/WApplication.h>
#include <Wt/WPushButton.h>

class TestPage : public Wt::WContainerWidget
{
public:
    TestPage(std::shared_ptr<Login> login);
    Wt::WTable* events_table_;

private:
    void createButtonsExampla();
    void createEventsTable();
    void populateEventsTable();
    std::shared_ptr<Login> login_;
};
