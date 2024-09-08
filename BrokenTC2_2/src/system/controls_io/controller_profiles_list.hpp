#pragma once

#include <QAbstractListModel>
#include <QObject>

#include <DataStructures/structures.hpp>

namespace btc2 {

class ControllerProfilesList : public QAbstractListModel {
  Q_OBJECT
 public:
  ControllerProfilesList(QObject* parent = nullptr);

  int rowCount(const QModelIndex& parent = QModelIndex()) const;
  int columnCount(const QModelIndex& parent = QModelIndex()) const;
  QVariant data(const QModelIndex& index, int role) const;
  void populate();

 private:
  std::vector<std::unique_ptr<ControllerProfile>> m_profiles{};
};

}  // namespace btc2
