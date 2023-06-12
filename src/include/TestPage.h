#pragma once
#include <Wt/WContainerWidget.h>
#include "LoginObj.h"
class TestPage : public Wt::WContainerWidget
{
public:
    TestPage(std::shared_ptr<Login> login);
    
private:
    void createButtonsExampla();
    std::shared_ptr<Login> login_;
};
