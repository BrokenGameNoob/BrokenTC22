#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QTranslator>
#include <QLibraryInfo>

#include <QDebug>
#include <QFile>
#include <QDirIterator>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

#ifdef PRINT_RESOURCES
    QDirIterator it(":/", QDirIterator::Subdirectories);
    while (it.hasNext()) {
        QString dir = it.next();
        qDebug() << dir;
    }
#endif

    using namespace Qt::Literals::StringLiterals;
    QTranslator translator;
    // look up e.g. :/i18n/myapp_de.qm
    if (translator.load(QLocale(), "BrokenTC2_2"_L1, "_"_L1, ":/i18n"_L1)){
        QCoreApplication::installTranslator(&translator);
        qInfo() << "Installed translator (" << QLocale() << ")";
    } else {
        qWarning() << "Failed to install translator (" << QLocale() << ")";
    }

    QQmlApplicationEngine engine;
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed,
        &app, []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("BrokenTC2_2", "Main");

    return app.exec();
}
