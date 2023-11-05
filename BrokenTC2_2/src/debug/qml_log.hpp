#pragma once

#include <QObject>
#include <QQmlEngine>
#include <iostream>

#include <debug/qstring_callback_sink.hpp>
#include <system/logs/btype.hpp>
#include <system/logs/logger.hpp>

namespace btc2 {

class QMLLogHelper : public QObject {
  Q_OBJECT

 signals:
  void textAdded(const QString& addedText);

 public:
  static void Init();

 public:
  QMLLogHelper(QObject* parent = nullptr);
  ~QMLLogHelper();

  Q_INVOKABLE void TestFunc(bool at_bottom) {
    std::cout << "AT_BOTTOM: " << at_bottom << "\n";
  }

 private:
  spdlog::sink_ptr m_sink{nullptr};
};

}  // namespace btc2