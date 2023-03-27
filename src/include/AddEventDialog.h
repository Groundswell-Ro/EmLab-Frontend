#pragma once
#include "LoginObj.h"
#include "EventView.h"

#include <Wt/WDialog.h>
#include <Wt/WSignal.h>

class AddEventDialog : public Wt::WDialog
{
public:
    AddEventDialog(std::shared_ptr<Login> login);
    EventView *eventView_;

    void submitBtnClicked();

    std::shared_ptr<Login> login_;

private:
};