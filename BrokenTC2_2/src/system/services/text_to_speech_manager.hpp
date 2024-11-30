#pragma once

#include <QObject>
#include <QQmlEngine>
#include <QTextToSpeech>

#include <DataStructures/structures.hpp>

namespace btc2 {

class TextToSpeechManager : public QObject {
  Q_OBJECT

 public:
  Q_INVOKABLE static QStringList GetAvailableEngines();
  static void Init();

 public:
  explicit TextToSpeechManager(std::shared_ptr<ApplicationSettings> settings);

  Q_INVOKABLE void SetTTSValues(double rate, double volume);

  Q_INVOKABLE void Speak(const QString& text);

  void Stop();

  bool GetEnabled();
  void SetEnabled(bool enabled);

 private slots:
  void UpdateTTSValuesFromSettings();

  void OnStateChanged();

 private:
  std::shared_ptr<ApplicationSettings> m_settings;

  QTextToSpeech* m_speech{};

  bool m_asked_stop{};
  bool m_ready{};
  bool m_enabled{};
};

}  // namespace btc2
