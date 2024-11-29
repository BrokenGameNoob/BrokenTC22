#pragma once

#include <QObject>
#include <QQmlEngine>

#include <DataStructures/structures.hpp>

namespace btc2 {

class TextToSpeechManager : public QObject {
  Q_OBJECT

 public:
  Q_INVOKABLE static QStringList GetAvailableEngines();
  static void Init();

 public:
  explicit TextToSpeechManager(std::shared_ptr<ApplicationSettings> settings);

 private:
  std::shared_ptr<ApplicationSettings> m_settings;
};

}  // namespace btc2
