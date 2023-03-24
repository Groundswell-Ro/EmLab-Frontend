#include "include/Theme.h"

#include <Wt/WLink.h>
#include <Wt/WLinkedCssStyleSheet.h>

#include <vector>

std::vector<Wt::WLinkedCssStyleSheet> Theme::styleSheets() const
{
  return {
    Wt::WLinkedCssStyleSheet(Wt::WLink("resources/css/theme.css"))
  };
}
