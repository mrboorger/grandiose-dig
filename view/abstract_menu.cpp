#include "abstract_menu.h"

void AbstractMenu::setVisible(bool visible) {
  for (const auto& child :
       findChildren<QWidget*>(QString(), Qt::FindDirectChildrenOnly)) {
    child->setVisible(visible);
  }
  QWidget::setVisible(visible);
}
