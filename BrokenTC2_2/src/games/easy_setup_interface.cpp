#include "easy_setup_interface.hpp"

#include <Utils/qt_utils.hpp>

namespace btc2{

EasySetupInterface::EasySetupInterface(QObject *parent) : QObject{parent} {}

void InitEasySetupInterface(){
  CREGISTER_QML_TYPE(btc2,EasySetupInterface);
}

}
