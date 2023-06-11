// #include <Wt/WCompositeWidget.h>
// #include <Wt/WTemplate.h>
// #include <Wt/WComboBox.h>
// #include <Wt/WInPlaceEdit.h>
// #include <Wt/WLineEdit.h>
// #include <Wt/WCalendar.h>
// #include <Wt/WText.h>
// #include <Wt/WApplication.h>
// #include <Wt/WEnvironment.h>
// #include <Wt/Utils.h>
// #include <Wt/WCalendar.h>
// #include <iostream>
// #include <Wt/WWebWidget.h>
// #include <Wt/WInteractWidget.h>

// using namespace std;

// class CalendarCopy : public Wt::WCompositeWidget
// {
// public:
//     CalendarCopy();
//     void setSelectionMode(Wt::SelectionMode mode);
//     void browseToPreviousYear();
//     void browseToPreviousMonth();
//     void browseToNextYear();
//     void browseToNextMonth();
//     void browseTo(const Wt::WDate& date);
//     int currentMonth() const { return currentMonth_; }
//     int currentYear() const { return currentYear_; }
//     void clearSelection();
//     void select(const Wt::WDate& date);
//     void select(const std::set<Wt::WDate>& dates);
//     void setHorizontalHeaderFormat(Wt::CalendarHeaderFormat format);
//     Wt::CalendarHeaderFormat horizontalHeaderFormat() {
//         return horizontalHeaderFormat_;
//     }
//     void setFirstDayOfWeek(int dayOfWeek);
//     const std::set<Wt::WDate>& selection() const { return selection_; }
//     Wt::Signal<>& selectionChanged() { return selectionChanged_; }
//     Wt::Signal<Wt::WDate>& activated() { return activated_; }
//     Wt::Signal<Wt::WDate>& clicked() { return clicked_; }
//     Wt::Signal<int, int>& currentPageChanged() { return currentPageChanged_; }
//     void setSingleClickSelect(bool single);
//     void setBottom(const Wt::WDate& bottom);
//     const Wt::WDate& bottom() const { return bottom_; }
//     void setTop(const Wt::WDate& top);
//     const Wt::WDate& top() const { return top_; }
//     virtual void load() override;

// protected:
//     virtual void render(Wt::WFlags<Wt::RenderFlag> renderFlags) override;
//     virtual WWidget* renderCell(Wt::WWidget* widget, const Wt::WDate& date);
//     bool isSelected(const Wt::WDate& date) const;
//     virtual void enableAjax() override;

// private:
//     Wt::SelectionMode selectionMode_;
//     bool              singleClickSelect_;
//     int               currentYear_;
//     int               currentMonth_;
//     Wt::CalendarHeaderFormat horizontalHeaderFormat_;
//     int                   firstDayOfWeek_;
//     std::set<Wt::WDate>   selection_;
//     bool                  needRenderMonth_;

//     Wt::Signal<>          selectionChanged_;
//     Wt::Signal<Wt::WDate> activated_;
//     Wt::Signal<Wt::WDate> clicked_;
//     Wt::Signal<int, int>  currentPageChanged_;

//     Wt::WDate             bottom_, top_;

//     struct Coordinate {
//         int i, j;

//         Coordinate() : i(0), j(0) { }
//         Coordinate(int x, int y) { i = x; j = y; }
//     };

//     Wt::WTemplate *impl_;
//     Wt::WComboBox *monthEdit_;
//     Wt::WInPlaceEdit *yearEdit_;

//     void create();
//     void renderMonth();

//     void emitCurrentPageChanged();

//     void monthChanged(int newMonth);
//     void yearChanged(Wt::WString newYear);
//     Wt::WDate dateForCell(int week, int dayOfWeek);

//     void selectInCurrentMonth(const Wt::WDate& d);

//     bool isInvalid(const Wt::WDate& d);
//     void cellClicked(Coordinate c);
//     void cellDblClicked(Coordinate c);
// };


