#include "include/TestPage.h"
#include <Wt/WText.h>
#include <Wt/WTemplate.h>
#include <Wt/WApplication.h>

TestPage::TestPage()
    : WContainerWidget()
{
    addStyleClass("page-two");

    addWidget(std::make_unique<Wt::WTemplate>(tr("test")));

}
