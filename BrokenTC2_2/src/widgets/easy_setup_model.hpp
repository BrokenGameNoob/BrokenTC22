#pragma once

#include <QObject>
#include <QQmlEngine>

namespace btc2 {

class EasySetupModel : public QObject {
  Q_OBJECT
  QML_ELEMENT
 public:
  explicit EasySetupModel(QObject *parent = nullptr);

 signals:
};

}  // namespace btc2
