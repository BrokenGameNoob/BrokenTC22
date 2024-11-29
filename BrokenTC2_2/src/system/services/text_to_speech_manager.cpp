#include "text_to_speech_manager.hpp"

#include <QTextToSpeech>

#include <utils/qt_utils.hpp>

namespace btc2 {

QStringList TextToSpeechManager::GetAvailableEngines() {
  return QTextToSpeech::availableEngines();
}

void TextToSpeechManager::Init() {
  CREGISTER_QML_UNCREATABLE_TYPE(btc2, TextToSpeechManager, "CppOwned");
}

TextToSpeechManager::TextToSpeechManager(std::shared_ptr<ApplicationSettings> settings)
    : QObject{nullptr}, m_settings{std::move(settings)} {}

}  // namespace btc2
