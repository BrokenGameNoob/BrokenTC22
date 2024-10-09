#include <QObject>
#include <QQmlEngine>
#include <QCoreApplication>

namespace btc2{

class EasySetupInterface : public QObject {
  Q_OBJECT

 public:
  EasySetupInterface(QObject* parent = nullptr);
};

void InitEasySetupInterface();
Q_COREAPP_STARTUP_FUNCTION(InitEasySetupInterface);

}
