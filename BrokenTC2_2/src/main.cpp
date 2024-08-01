#include <QDateTime>
#include <QDebug>
#include <QDirIterator>
#include <QFile>
#include <QGuiApplication>
#include <QLibraryInfo>
#include <QQmlApplicationEngine>
#include <QTranslator>
#include <SDL.h>
#include <git_version.hpp>

#include <DataStructures/structures.hpp>
#include <Logger/btype.hpp>
#include <Logger/logger_setup.hpp>
#include <debug/qml_log.hpp>
#include <system/services/service_manager.hpp>
#include <utils/shared_constants.hpp>
#include <utils/style.hpp>

int SDL_main(int argc, char *argv[]) {
  /* Systematic checks */
  // btc2::RunSoftControlsCompatibilityCountCheck();

  QGuiApplication app(argc, argv);

  /* -- Setting up logger -- */
  const auto kLogPath{btc2::GetLogFilePath(CMAKEMACRO_PROJECT_EXE)};
  btc2::SetupLoggerRotating(kLogPath, 2);

  /* -- Application setup -- */

  btc2::QMLLogHelper::Init();
  btc2::Style::Init();
  btc2::Constants::Init();
  btc2::ServiceManager::Init();
  btc2::ControllerHandler::Init();

  /* -- Debug -- */
#ifdef PRINT_RESOURCES
  QDirIterator it(":/", QDirIterator::Subdirectories);
  while (it.hasNext()) {
    QString dir = it.next();
    qDebug() << dir;
  }
#endif

  /* -- Loading translations -- */
  using namespace Qt::Literals::StringLiterals;
  QTranslator translator;
  // look up e.g. :/i18n/myapp_de.qm
  if (translator.load(QLocale(), "BrokenTC2_2"_L1, "_"_L1, ":/i18n"_L1)) {
    QCoreApplication::installTranslator(&translator);
    //    qInfo() << "Installed translator (" << QLocale() << ")";
    SPDLOG_INFO("Installed translator ({})", QLocale().nativeLanguageName());
  } else {
    qWarning() << "Failed to install translator (" << QLocale() << ")";
  }

  /* -- Actual start -- */
  QQmlApplicationEngine engine;
  QObject::connect(
      &engine,
      &QQmlApplicationEngine::objectCreationFailed,
      &app,
      []() { QCoreApplication::exit(-1); },
      Qt::QueuedConnection);
  engine.loadFromModule("BrokenTC2_2", "Main");

  /* -- Displaying startup infos -- */
  SPDLOG_INFO("-------- Logging {} session into: \"{}\"", QDateTime::currentDateTime().toString(), kLogPath);
  SPDLOG_INFO("Program info:");
  SPDLOG_INFO("\tProgram version: {}", gitversion::GetVersionRepresentationString());
  SPDLOG_INFO("\tProgram build configuration: {}", btype::BuildTypeStr());

  /* Actual exec */
  const auto kRVal{app.exec()};
  btc2::StopLogger();
  return kRVal;
}
