#include "controller_profiles_list.hpp"

namespace btc2 {

ControllerProfilesList::ControllerProfilesList(QObject* parent) {
  //
}

int ControllerProfilesList::rowCount(const QModelIndex& parent) const {
  return m_profiles.size();
}
int ControllerProfilesList::columnCount(const QModelIndex& parent) const {
  //
}
QVariant ControllerProfilesList::data(const QModelIndex& index, int role) const {
  //
}
void ControllerProfilesList::populate() {
  //
}

}  // namespace btc2
