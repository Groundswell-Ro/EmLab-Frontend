#pragma once
#include "../../utils/include/LoginObj.h"
#include <Wt/WContainerWidget.h>
#include <Wt/WTable.h>


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
