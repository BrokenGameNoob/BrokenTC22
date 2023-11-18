#include "qml_log.hpp"

#include <QStringView>
#include <string>
#include <string_view>

#include <Logger/btype.hpp>
#include <Logger/logger.hpp>
#include <Logger/qstring_callback_sink.hpp>
#include <utils/qt_utils.hpp>

namespace btc2 {

QMLLogHelper::QMLLogHelper(QObject* parent) : QObject{parent} {
  if constexpr (btype::HasDebInfo()) {
    SPDLOG_INFO("QMLLogHelper enabled and active");
    spdlog::custom_qstring_callback callback{[&](const QString& msg) { emit textAdded(msg); }};
    std::shared_ptr<spdlog::sinks::callback_qstring_sink_mt> sink{
        std::make_shared<spdlog::sinks::callback_qstring_sink_mt>(callback, true, true)};
    sink->set_pattern(kLoggerPattern);
    sink->set_level(MaxLogLevel());
    m_sink = sink;

    auto logger{spdlog::get(kLoggerName)};
    logger->sinks().emplace_back(m_sink);
  } else {
    SPDLOG_INFO("QMLLogHelper not active");
  }
}

QMLLogHelper::~QMLLogHelper() {
  if (m_sink) {
    auto& sinks{spdlog::get(kLoggerName)->sinks()};
    sinks.erase(std::remove(sinks.begin(), sinks.end(), m_sink), sinks.end());
  }
}

void QMLLogHelper::Init() {
  SPDLOG_INFO("Init");

  //  qmlRegisterType<btc2::QMLLogHelper>("btc2", 1, 0, "QMLLogHelper");
  CREGISTER_QML_TYPE(btc2, QMLLogHelper);
}

}  // namespace btc2
