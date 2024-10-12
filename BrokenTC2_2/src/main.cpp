#include <QApplication>
#include <QDateTime>
#include <QDebug>
#include <QDirIterator>
#include <QFile>
#include <QGuiApplication>
#include <QLibraryInfo>
#include <QMessageBox>
#include <QQmlApplicationEngine>
#include <QTranslator>
#include <git_version.hpp>

#include <DataStructures/path_utils.hpp>
#include <DataStructures/structures.hpp>
#include <Logger/btype.hpp>
#include <Logger/logger_setup.hpp>
#include <SDL2/SDL.h>
#include <debug/qml_log.hpp>
#include <games/easy_setup_interface.hpp>
#include <system/controls_io/keystroke_sequencer.hpp>
#include <system/services/service_manager.hpp>
#include <utils/shared_constants.hpp>
#include <utils/style.hpp>


bool SetupFolders() {
  auto lambda_create_folder_if_not_exists = [](const QString& path) {
    QDir dir{path};
    if (dir.exists()) {
      return true;
    }
    const auto kSuccess{dir.mkpath(".")};
    if (!kSuccess) {
      SPDLOG_ERROR("Failed to create folder: <{}>", path);
    }
    return kSuccess;
  };

  bool success{true};
  success &= lambda_create_folder_if_not_exists(btc2::path::GetAppDataPath());
  success &= lambda_create_folder_if_not_exists(btc2::path::GetControllerProfilesPath());
  success &= lambda_create_folder_if_not_exists(btc2::path::GetGamesProfilesPath());
  return success;
}

int SDL_main(int argc, char* argv[]) {
  /* Systematic checks */
  // btc2::RunSoftControlsCompatibilityCountCheck();

  QGuiApplication app(argc, argv);

  /* -- Setting up logger -- */
  const auto kLogPath{btc2::GetLogFilePath(CMAKEMACRO_PROJECT_EXE)};
  btc2::SetupLoggerRotating(kLogPath, 2);

  /* -- Application setup -- */

  if (!SetupFolders()) {
    SPDLOG_ERROR("Failed to setup folders");
    QApplication err_app(argc, argv);
    QMessageBox::critical(nullptr,
                          QObject::tr("Critical error"),
                          QObject::tr("Failed to setup application data folders.\nExit application."));
    return -1;
  }

  btc2::QMLLogHelper::Init();
  btc2::Style::Init();
  btc2::Constants::Init();
  btc2::ServiceManager::Init();
  btc2::ControllerHandler::Init();
  btc2::KeyboardHandler::Init();
  btc2::GameProfilesHandler::Init();
  CREGISTER_QML_UNCREATABLE_TYPE(btc2, Game, "Enum class");
  qmlRegisterUncreatableType<Bidule>("btc2", 1, 0, "Bidule", "Enum class");

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
  std::thread key_sequencer_thread{&btc2::io::KeySequencerThread::Run, &btc2::io::KeySequencerThread::I()};

  const auto kRVal{app.exec()};
  btc2::StopLogger();
  btc2::io::KeySequencerThread::Stop();
  key_sequencer_thread.join();
  SPDLOG_INFO("Leaving app.");
  return kRVal;
}
