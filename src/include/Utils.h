#pragma once
#include <Wt/WString.h>

namespace Emlab {
    const Wt::WString TIME_FORMAT  = "HH:mm AP";
    const Wt::WString DATE_FORMAT = "dd-MMM-yyyy";
    const Wt::WString DATE_TIME_FORMAT = TIME_FORMAT + " " + DATE_FORMAT;
}