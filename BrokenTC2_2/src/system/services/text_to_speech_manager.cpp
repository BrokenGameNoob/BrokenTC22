#include "text_to_speech_manager.hpp"

#include <qtconcurrentrun.h>

#include <Utils/computations.hpp>
#include <utils/qt_utils.hpp>

namespace btc2 {

QStringList TextToSpeechManager::GetAvailableEngines() {
  return QTextToSpeech::availableEngines();
}

void TextToSpeechManager::Init() {
  CREGISTER_QML_UNCREATABLE_TYPE(btc2, TextToSpeechManager, "CppOwned");
}

TextToSpeechManager::TextToSpeechManager(std::shared_ptr<ApplicationSettings> settings)
    : QObject{nullptr}, m_settings{std::move(settings)}, m_speech{new QTextToSpeech{this}} {
  connect(m_settings.get(), &ApplicationSettings::dataChanged, this, [this]() { UpdateTTSValuesFromSettings(); });

  connect(m_speech, &QTextToSpeech::stateChanged, this, &TextToSpeechManager::OnStateChanged);
  UpdateTTSValuesFromSettings();
  m_ready = (m_speech->state() == QTextToSpeech::State::Ready);
}

void TextToSpeechManager::SetTTSValues(double rate, double volume) {
  m_speech->setRate(rate);
  m_speech->setVolume(volume);
}

void TextToSpeechManager::Speak(const QString& text) {
  if (!m_enabled) {
    return;
  }
  if (m_ready) {
    QThreadPool::globalInstance()->start([this, text]() { m_speech->say(text); });
  } else {
    Stop();
    SPDLOG_WARN("TTS required to say <{}> but was not ready", text);
  }
}

void TextToSpeechManager::Stop() {
  if (m_asked_stop) {
    return;
  }
  m_speech->stop(QTextToSpeech::BoundaryHint::Immediate);
  m_asked_stop = true;
  m_ready = false;
}

bool TextToSpeechManager::GetEnabled() {
  return m_enabled;
}

void TextToSpeechManager::SetEnabled(bool enabled) {
  m_enabled = enabled;
}

void TextToSpeechManager::UpdateTTSValuesFromSettings() {
  const auto kMappedRate{utils::Map(m_settings->TTSRate(), 0, 1.0, -1., 1.0)};
  SetTTSValues(kMappedRate, m_settings->TTSVolume());
}

void TextToSpeechManager::OnStateChanged() {
  if (m_speech->state() == QTextToSpeech::State::Ready || m_speech->state() == QTextToSpeech::State::Error) {
    m_asked_stop = false;
  }
  if (m_speech->state() == QTextToSpeech::State::Ready) {
    m_ready = true;
  }
}

}  // namespace btc2
