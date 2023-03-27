#include "include/AddUserServiceDialog.h"

AddUserServiceDialog::AddUserServiceDialog()
    : WDialog("Add user service")
{
    this->setWindowTitle("Add user service");
    this->setClosable(true);
    this->setResizable(true);
    this->setModal(true);
    this->setMinimumSize(400, 400);
    this->setMaximumSize(400, 400);
}