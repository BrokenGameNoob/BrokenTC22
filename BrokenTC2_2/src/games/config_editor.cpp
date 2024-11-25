#include "games/config_editor.hpp"

#include <QDebug>
#include <QDirIterator>
#include <QDomElement>
#include <QFileInfo>
#include <QRegularExpression>

#include <DataStructures/structures.hpp>
#include <Utils/xml_utils.hpp>

namespace btc2 {

QStringList GetBindingsFiles(const QString& configDirPath) {
  QDirIterator configDirIt{configDirPath, {"*.xml"}};

  static const QRegularExpression bindingFilenameRegex{R"(Bindings_(?=.*[0-9])(?=.*[a-zA-Z])([a-zA-Z0-9]+)\.xml)"};

  if (!bindingFilenameRegex.isValid()) {
    throw std::runtime_error{"Invalid regex for binding files in " + std::string{__PRETTY_FUNCTION__}};
  }

  QStringList out{};
  QDir configDir{configDirPath};
  while (configDirIt.hasNext()) {
    QString file{configDirIt.next()};
    QFileInfo fi{file};
    file = fi.fileName();
    if (bindingFilenameRegex.match(file).hasMatch()) {
      out << configDir.absoluteFilePath(file);
    }
  }

  return out;
}

QString GetBindingKeyboardFile(const QString& configDirPath) {
  constexpr auto kBindingFileName{"Bindings_Keyboard.xml"};
  const auto kFile{QDir{configDirPath}.filePath(kBindingFileName)};
  if (QFileInfo::exists(kFile)) {
    return kFile;
  }
  return "";
}

namespace xml {

bool EditXmlControllerConf(const QString& xmlPath) {
  auto xmlDoc{::xml::GetXmlDoc(xmlPath)};
  if (!xmlDoc) {
    qCritical() << "Cannot open file " << xmlPath;
    return false;
  }
  auto xmlRoot{::xml::GetXmlRoot(xmlDoc.value())};
  auto actionBindings{::xml::FindElement(xmlRoot, "ActionBinding", [&](const QDomElement& e) -> bool {
    return e.attribute("Name", "") == "DrivingBindings";
  })};
  if (actionBindings.size() != 1) {
    qCritical() << QString{"<ActionBinding Name=%0> not found"}.arg("DrivingBindings");
    return false;
  }

  auto actions{::xml::FindElement(actionBindings[0], "Action", [&](const QDomElement& e) -> bool {
    auto name{e.attribute("Name", "")};
    return name == "RoadGearUp" || name == "RoadGearDown";
  })};
  if (actions.size() != 2) {
    qCritical() << QString{"<Action Name=(RoadGearUp | RoadGearDown)> not found"}.arg("DrivingBindings");
    return false;
  }

  for (auto& a : actions) {
    auto channels{::xml::FindElement(a, "Channel", [&](const QDomElement& e) -> bool {
      auto device{e.attribute("Device", "")};
      return device == "PAD";
    })};
    for (auto& c : channels) {
      a.removeChild(c);
    }
  }

  auto success{::xml::SaveXml(xmlDoc.value(), xmlPath)};
  return success;
}

namespace values {
struct Mapping {
  const char* kAttributeName;
  const char* kGameKeyName;
  const UINT kVkKey;
  bool (GameProfileTheCrew::*SetValue)(int32_t key);
  constexpr Mapping(const char* attribute_name, const char* game_key_name, const UINT vkKey,
                    bool (GameProfileTheCrew::*SetValuePtr)(int32_t key))
      : kAttributeName{attribute_name}, kGameKeyName{game_key_name}, kVkKey{vkKey}, SetValue{SetValuePtr} {}
};

const std::array kWheelBindingMapping{
    Mapping{"Clutch", "NUM0", VK_NUMPAD0, &GameProfileTheCrew::SetClutch},
    Mapping{"GearReverse", "NUM8", VK_NUMPAD8, &GameProfileTheCrew::SetGearReverse},
    Mapping{"Gear1", "NUM1", VK_NUMPAD1, &GameProfileTheCrew::SetGear1},
    Mapping{"Gear2", "NUM2", VK_NUMPAD2, &GameProfileTheCrew::SetGear2},
    Mapping{"Gear3", "NUM3", VK_NUMPAD3, &GameProfileTheCrew::SetGear3},
    Mapping{"Gear4", "NUM4", VK_NUMPAD4, &GameProfileTheCrew::SetGear4},
    Mapping{"Gear5", "NUM5", VK_NUMPAD5, &GameProfileTheCrew::SetGear5},
    Mapping{"Gear6", "NUM6", VK_NUMPAD6, &GameProfileTheCrew::SetGear6},
    Mapping{"Gear7", "NUM7", VK_NUMPAD7, &GameProfileTheCrew::SetGear7},
};

const std::array kDrivingBindingMapping{
    Mapping{"RoadGearUp", "NUM_PLUS", VK_ADD, &GameProfileTheCrew::SetGearUp},
    Mapping{"RoadGearDown", "NUM_MINUS", VK_SUBTRACT, &GameProfileTheCrew::SetGearDown}};

}  // namespace values

void OnGearActionFound(QDomElement* action, const values::Mapping& mapping, GameProfileTheCrew* settings) {
  // <Channel Device="KEYBOARD" Name=""/>

  while (action->hasChildNodes()) {
    action->removeChild(action->firstChild());
  }

  QDomElement channel{action->ownerDocument().createElement("Channel")};
  channel.setAttribute("Device", "KEYBOARD");
  channel.setAttribute("Name", mapping.kGameKeyName);
  action->appendChild(channel);

  if (settings) {
    (settings->*(mapping.SetValue))(mapping.kVkKey);
  }
}

bool EditXmlKeyboardConf(const QString& xmlPath, GameProfileTheCrew* settings) {
  qInfo() << "editXmlKeyboardConf: <" << xmlPath << ">";
  auto xmlDoc{::xml::GetXmlDoc(xmlPath)};
  if (!xmlDoc) {
    qCritical() << "Cannot open file " << xmlPath;
    return false;
  }
  auto xmlRoot{::xml::GetXmlRoot(xmlDoc.value())};
  constexpr auto kWheelActionBindingName{"WheelBindings"};
  auto wheelBindingsList{::xml::FindElement(xmlRoot, "ActionBinding", [&](const QDomElement& e) -> bool {
    return e.attribute("Name", "") == kWheelActionBindingName;
  })};
  if (wheelBindingsList.size() != 1) {
    qCritical() << QString{"<ActionBinding Name=%0> not found"}.arg(kWheelActionBindingName);
    return false;
  }
  auto& wheelBinding{wheelBindingsList[0]};

  for (const auto& mapping : values::kWheelBindingMapping) {
    auto actions{::xml::FindElement(wheelBinding, "Action", [&](const QDomElement& e) -> bool {
      auto name{e.attribute("Name", "")};
      return name == mapping.kAttributeName;
    })};
    if (actions.size() != 1) {
      qCritical() << QString{"<Action Name=%0> not found"}.arg(mapping.kAttributeName);
      return false;
    }
    OnGearActionFound(&actions[0], mapping, settings);
  }

  //

  constexpr auto kDrivingActionBindingName{"WheelBindings"};
  auto drivingBindingsList{::xml::FindElement(xmlRoot, "ActionBinding", [&](const QDomElement& e) -> bool {
    return e.attribute("Name", "") == kDrivingActionBindingName;
  })};
  if (drivingBindingsList.size() != 1) {
    qCritical() << QString{"<ActionBinding Name=%0> not found"}.arg(kDrivingActionBindingName);
    return false;
  }
  auto& drivingBinding{drivingBindingsList[0]};

  for (const auto& mapping : values::kDrivingBindingMapping) {
    auto actions{::xml::FindElement(drivingBinding, "Action", [&](const QDomElement& e) -> bool {
      auto name{e.attribute("Name", "")};
      return name == mapping.kAttributeName;
    })};
    if (actions.size() != 1) {
      qCritical() << QString{"<Action Name=%0> not found"}.arg(mapping.kAttributeName);
      return false;
    }
    OnGearActionFound(&actions[0], mapping, settings);
  }

  auto success{::xml::SaveXml(xmlDoc.value(), xmlPath)};
  return success;
}

}  // namespace xml
}  // namespace btc2
