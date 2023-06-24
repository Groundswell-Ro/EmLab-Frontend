#pragma once
#include <Wt/WCalendar.h>
#include <Wt/WTemplate.h>
#include <Wt/WText.h>
#include <Wt/WApplication.h>
#include <Wt/WEnvironment.h>
#include <Wt/Utils.h>
#include <cstdlib>
#include <Wt/WComboBox.h>
#include <Wt/WInPlaceEdit.h>
#include <Wt/WLineEdit.h>
#include <iostream>

using namespace std;

class Calendar : public Wt::WCalendar
{
public:
    Calendar();

protected:
    virtual Wt::WWidget* renderCell(Wt::WWidget* widget, const Wt::WDate& date) override;
private:
    void onCellClicked(const Wt::WDate& date);
};


