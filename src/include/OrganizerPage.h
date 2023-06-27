#pragma once
#include "../../utils/include/LoginObj.h"
#include <Wt/WContainerWidget.h>
#include <Wt/WTable.h>


class OrganizerPage : public Wt::WContainerWidget
{
public:
    OrganizerPage(std::shared_ptr<Login> login);
    Wt::WTable* events_table_;

private:
    void createButtonsExampla();
    void createEventsTable();
    void populateEventsTable();
    std::shared_ptr<Login> login_;
};
