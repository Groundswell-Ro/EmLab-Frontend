#include "include/Calendar.h"


Calendar::Calendar()
{
  clicked().connect(this, &Calendar::onCellClicked);
  setStyleClass("calendar");
	setSelectionMode(Wt::SelectionMode::Single);
	setHorizontalHeaderFormat(Wt::CalendarHeaderFormat::SingleLetterDayNames);
	select(Wt::WDate::currentDate());
}

void Calendar::onCellClicked(const Wt::WDate& date)
{
  std::cout << "Cell clicked: " << date.toString() << std::endl;

  if(date.month() != currentMonth()) {
    browseTo(date);
  }
  select(date);
  
}

Wt::WWidget *Calendar::renderCell(WWidget* widget, const Wt::WDate& date)
{
  Wt::WText* t = dynamic_cast<Wt::WText*>(widget);

  if (!t) {
    t = new Wt::WText();
    t->setInline(false);
    t->setTextFormat(Wt::TextFormat::Plain);
  }

  int day = date.day();
  std::string day_str = std::to_string(day);
  t->setText(Wt::WString::fromUTF8(day_str.c_str()));


  Wt::WString styleClass = "cal-cell";

  if (date.month() != currentMonth()) {
    styleClass += " cal-outside-day";
  }else {
    styleClass += " cal-inside-day";
  }

  if (date.dayOfWeek() == 6 || date.dayOfWeek() == 7) {
      styleClass += " cal-weekend-day";
  }
  if(isSelected(date))
    styleClass += " cal-selected-day";

  Wt::WDate currentDate = Wt::WDate::currentDate();
  if (date.day() == currentDate.day() && date.month() == currentDate.month() &&
      date.year() == currentDate.year()) {
    t->setToolTip(Wt::WString::tr("Wt.WCalendar.today"));
  } else
    t->setToolTip("");
  t->setStyleClass(styleClass.toUTF8());

  return t;
}
