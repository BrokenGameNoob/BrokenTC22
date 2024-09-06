#pragma once

#include <QCoreApplication>
#include <QQmlEngine>
#include <QString>
#include <QtQml>
#include <cstdint>

#include <DataStructures/games.hpp>
#include <Logger/logger.hpp>
#include <Utils/json_utils.hpp>

namespace btc2 {

#define kBtc2QmlModule "btc2"

#define DS_ELEM_ACCESSOR_DECL(type, name, editor_group, editor_type, editor_title, game_compatibility) \
  bool Set##name(type val) {                                                                           \
    if (m_##name == val) {                                                                             \
      return false;                                                                                    \
    }                                                                                                  \
    m_##name = std::move(val);                                                                         \
    emit dataChanged();                                                                                \
    return true;                                                                                       \
  }                                                                                                    \
  const type& name() const {                                                                           \
    return m_##name;                                                                                   \
  }                                                                                                    \
  type& name() {                                                                                       \
    return m_##name;                                                                                   \
  }

#define DS_STRUCT_ELEM_DECL(type, name, editor_group, editor_type, editor_title, game_compatibility) type m_##name{};
#define DS_PROPERTY_ELEM_DECL(type, name, editor_group, editor_type, editor_title, game_compatibility) \
  Q_PROPERTY(type name READ name WRITE Set##name NOTIFY dataChanged)
#define DS_TO_JSON_ELEM_DECL(type, name, editor_group, editor_type, editor_title, game_compatibility) \
  obj[#name] = val.name();
#define DS_FROM_JSON_ELEM_DECL(type, name, editor_group, editor_type, editor_title, game_compatibility) \
  out->Set##name(obj[#name].toVariant().value<type>());
#define DS_DEBUG_STRING_ELEM_DECL(type, name, editor_group, editor_type, editor_title, game_compatibility) \
  out += QString{"." #name "=%0,"}.arg(this->name());
#define DS_GET_PROPERTIES_FOR_GROUP_ELEM_DECL(type, name, editor_group, editor_type, editor_title, game_compatibility) \
  if (group == editor_group && editor_type != DataEditor::NO_EDITOR) {                                                 \
    keys.emplace_back(#name);                                                                                          \
  }
#define DS_SET_FROM_KEY_ELEM_DECL(type, name, editor_group, editor_type, editor_title, game_compatibility) \
  if (key == #name) {                                                                                      \
    return this->Set##name(val.value<type>());                                                             \
  }
#define DS_GET_TITLE_FOR_KEY_ELEM_DECL(type, name, editor_group, editor_type, editor_title, game_compatibility) \
  if (key == #name) {                                                                                           \
    return editor_title;                                                                                        \
  }
#define DS_GET_GAME_COMPATIBILITY_ELEM_DECL(type, name, editor_group, editor_type, editor_title, game_compatibility) \
  if (key == #name) {                                                                                                \
    return game_compatibility;                                                                                       \
  }
#define DS_GET_EDITOR_TYPE_ELEM_DECL(type, name, editor_group, editor_type, editor_title, game_compatibility) \
  if (key == #name) {                                                                                         \
    return editor_type;                                                                                       \
  }

#define DS_DECLARE_STATIC_INIT_FUNC(ClassName)                          \
  static void Init() {                                                  \
    qmlRegisterType<btc2::ClassName>(kBtc2QmlModule, 1, 0, #ClassName); \
  }

#define DS_DECLARE_STATIC_GET_PROPERTIES_FOR_GROUP_FUNC(ClassName, ELEMENTS_LIST) \
  Q_INVOKABLE static QStringList GetPropertiesKeysFor(const QString& group) {     \
    QStringList keys{};                                                           \
                                                                                  \
    ELEMENTS_LIST(DS_GET_PROPERTIES_FOR_GROUP_ELEM_DECL)                          \
                                                                                  \
    return keys;                                                                  \
  }

#define DS_DECLARE_STATIC_GET_TITLE_FOR_FUNC(ClassName, ELEMENTS_LIST) \
  Q_INVOKABLE static QString GetTitleFor(const QString& key) {         \
    ELEMENTS_LIST(DS_GET_TITLE_FOR_KEY_ELEM_DECL);                     \
    return "???";                                                      \
  }

#define DS_DECLARE_STATIC_GET_GAME_COMPATIBILITY_FUNC(ClassName, ELEMENTS_LIST) \
  Q_INVOKABLE static Games GetKeyGameCompatiblity(const QString& key) {         \
    ELEMENTS_LIST(DS_GET_GAME_COMPATIBILITY_ELEM_DECL);                         \
    return Game::kNone;                                                         \
  }

#define DS_DECLARE_STATIC_IS_GAME_COMPATIBLE_FUNC(ClassName, ELEMENTS_LIST)        \
  Q_INVOKABLE static bool IsKeyCompatibleWithGame(const QString& key, Game game) { \
    const bool kIsCompatible{(GetKeyGameCompatiblity(key) & game) != 0};           \
    return kIsCompatible;                                                          \
  }

#define DS_DECLARE_STATIC_GET_EDITOR_TYPE_FOR_KEY_FUNC(ClassName, ELEMENTS_LIST)   \
  Q_INVOKABLE static DataEditor::EditorType GetEditorTypeFor(const QString& key) { \
    ELEMENTS_LIST(DS_GET_EDITOR_TYPE_ELEM_DECL);                                   \
    return DataEditor::NO_EDITOR;                                                  \
  }

#define DS_DECLARE_MEMBER_DEBUG_STRING_FUNC(ClassName, ELEMENTS_LIST) \
  QString DebugString() const {                                       \
    QString out{#ClassName "{"};                                      \
    ELEMENTS_LIST(DS_DEBUG_STRING_ELEM_DECL);                         \
    out.resize(out.size() - 1);                                       \
    return out + "}";                                                 \
  }

#define DS_DECLARE_MEMBER_SET_FROM_KEY_FUNC(ClassName, ELEMENTS_LIST)    \
  Q_INVOKABLE bool SetFromKey(const QString& key, const QVariant& val) { \
    ELEMENTS_LIST(DS_SET_FROM_KEY_ELEM_DECL);                            \
    return false;                                                        \
  }

#define DS_DECLARE_OSTREAM_OPERATOR(ClassName)                              \
  inline std::ostream& operator<<(std::ostream& os, const ClassName& obj) { \
    os << obj.DebugString().toStdString();                                  \
    return os;                                                              \
  }

#define DS_DECLARE_TO_JSON_FUNC(ClassName, ELEMENTS_LIST) \
  inline QJsonObject ToJson(const ClassName& val) {       \
    QJsonObject obj;                                      \
    ELEMENTS_LIST(DS_TO_JSON_ELEM_DECL);                  \
    return obj;                                           \
  }

#define DS_DECLARE_SAVE_AS_FUNC(ClassName)                            \
  inline auto SaveAs(const ClassName& val, const QString& filename) { \
    return utils::Save(QJsonDocument(ToJson(val)), filename);         \
  }

#define DS_DECLARE_SET_FROM_JSON_FUNC(ClassName, ELEMENTS_LIST)     \
  inline void SetFromJson(const QJsonObject& obj, ClassName* out) { \
    ELEMENTS_LIST(DS_FROM_JSON_ELEM_DECL);                          \
  }

#define DS_DECLARE_FILL_FROM_FILE_FUNC(ClassName)                     \
  inline bool FillFromFile(const QString& filename, ClassName* out) { \
    if (auto doc{utils::Read(filename)}) {                            \
      SetFromJson(doc->object(), out);                                \
      return true;                                                    \
    }                                                                 \
    return false;                                                     \
  }

#define DS_DECLARE_STRUCT(ClassName, ELEMENTS_LIST)                            \
  class ClassName : public QObject {                                           \
    Q_OBJECT                                                                   \
   public:                                                                     \
    Q_SIGNAL void dataChanged();                                               \
                                                                               \
    ELEMENTS_LIST(DS_PROPERTY_ELEM_DECL)                                       \
   public:                                                                     \
    ClassName(QObject* parent = nullptr) : QObject(parent) {}                  \
    DS_DECLARE_STATIC_INIT_FUNC(ClassName);                                    \
    DS_DECLARE_STATIC_GET_PROPERTIES_FOR_GROUP_FUNC(ClassName, ELEMENTS_LIST); \
    DS_DECLARE_STATIC_GET_TITLE_FOR_FUNC(ClassName, ELEMENTS_LIST);            \
    DS_DECLARE_STATIC_GET_GAME_COMPATIBILITY_FUNC(ClassName, ELEMENTS_LIST);   \
    DS_DECLARE_STATIC_IS_GAME_COMPATIBLE_FUNC(ClassName, ELEMENTS_LIST);       \
    DS_DECLARE_STATIC_GET_EDITOR_TYPE_FOR_KEY_FUNC(ClassName, ELEMENTS_LIST);  \
                                                                               \
    DS_DECLARE_MEMBER_SET_FROM_KEY_FUNC(ClassName, ELEMENTS_LIST);             \
                                                                               \
    Dummy_STRUCT_ELEMENTS_LIST(DS_ELEM_ACCESSOR_DECL);                         \
                                                                               \
    DS_DECLARE_MEMBER_DEBUG_STRING_FUNC(ClassName, ELEMENTS_LIST);             \
                                                                               \
   private:                                                                    \
    ELEMENTS_LIST(DS_STRUCT_ELEM_DECL)                                         \
  };                                                                           \
  static void register##ClassName() {                                          \
    ClassName::Init();                                                         \
  }                                                                            \
  Q_COREAPP_STARTUP_FUNCTION(register##ClassName);                             \
  DS_DECLARE_OSTREAM_OPERATOR(ClassName);                                      \
  DS_DECLARE_TO_JSON_FUNC(ClassName, ELEMENTS_LIST);                           \
  DS_DECLARE_SAVE_AS_FUNC(ClassName);                                          \
  DS_DECLARE_SET_FROM_JSON_FUNC(ClassName, ELEMENTS_LIST);                     \
  DS_DECLARE_FILL_FROM_FILE_FUNC(ClassName);

/* -------------------------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------------------------- */

class DataEditor {
  Q_GADGET

 public:
  enum EditorType {
    NO_EDITOR,
    RAW_DISPLAY,
    CONTROLLER_KEY,
    SLIDER,
  };
  Q_ENUM(EditorType)

  static QString GetEditorTypeString(EditorType type) {
    const QMetaObject& metaObject = DataEditor::staticMetaObject;
    int enumIndex = metaObject.indexOfEnumerator("EditorType");
    QMetaEnum metaEnum = metaObject.enumerator(enumIndex);
    return QString(metaEnum.valueToKey(type));
  }

  static void Init() {
    qmlRegisterUncreatableType<btc2::DataEditor>(
        kBtc2QmlModule, 1, 0, "DataEditor", "Error DataEditor class uncreatable");
  }
};
static void RegisterDataEditor() {
  DataEditor::Init();
}
Q_COREAPP_STARTUP_FUNCTION(RegisterDataEditor);

/* ELEMENTS_LIST(type, Name, editor_group_name, DataEditor::EditorType, editor_title) */

#define Dummy_STRUCT_ELEMENTS_LIST(FUNC)                                                             \
  FUNC(QString, Name, "global", DataEditor::RAW_DISPLAY, QObject::tr("Profile name"), Game::kAll)    \
  FUNC(int32_t, GearUp, "gear", DataEditor::RAW_DISPLAY, QObject::tr("Gear down"), Game::kTheCrew2)  \
  FUNC(int32_t, GearDown, "gear", DataEditor::RAW_DISPLAY, QObject::tr("Gear up"), Game::kAll)       \
  FUNC(int32_t, Clutch, "global", DataEditor::CONTROLLER_KEY, QObject::tr("Clutch key"), Game::kAll) \
  FUNC(double, Test, "gear", DataEditor::SLIDER, QObject::tr("TEST"), Game::kAll)

DS_DECLARE_STRUCT(Dummy, Dummy_STRUCT_ELEMENTS_LIST);

}  // namespace btc2
