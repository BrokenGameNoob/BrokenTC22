#pragma once

#include <QCoreApplication>
#include <QQmlEngine>
#include <QString>
#include <QtQml>

#include <DataStructures/games.hpp>
#include <Logger/logger.hpp>
#include <Utils/json_utils.hpp>

namespace btc2 {

#define kBtc2QmlModule "btc2"

#define DS_DEFAULT \
  {}

namespace def {
constexpr auto kControllerButton{-1};
constexpr auto kDefaultKey{-1};
}  // namespace def

#define DS_ELEM_ACCESSOR_DECL(                                                                                 \
    type, name, default_value, editor_group, editor_type, editor_title, game_compatibility, display_condition) \
  bool Set##name(type val) {                                                                                   \
    if (m_##name == val) {                                                                                     \
      return false;                                                                                            \
    }                                                                                                          \
    m_##name = std::move(val);                                                                                 \
    emit dataChanged();                                                                                        \
    return true;                                                                                               \
  }                                                                                                            \
  const type& name() const {                                                                                   \
    return m_##name;                                                                                           \
  }                                                                                                            \
  type& name() {                                                                                               \
    return m_##name;                                                                                           \
  }

#define DS_STRUCT_ELEM_DECL(                                                                                   \
    type, name, default_value, editor_group, editor_type, editor_title, game_compatibility, display_condition) \
  type m_##name = default_value;
#define DS_PROPERTY_ELEM_DECL(                                                                                 \
    type, name, default_value, editor_group, editor_type, editor_title, game_compatibility, display_condition) \
  Q_PROPERTY(type name READ name WRITE Set##name NOTIFY dataChanged)
#define DS_RESET_ELEM_DECL(                                                                                    \
    type, name, default_value, editor_group, editor_type, editor_title, game_compatibility, display_condition) \
  success &= Set##name(default_value);
#define DS_RESET_FOR_GROUP_ELEM_DECL(                                                                          \
    type, name, default_value, editor_group, editor_type, editor_title, game_compatibility, display_condition) \
  if (GroupIsIn(group_title, editor_group)) success &= Set##name(default_value);
#define DS_TO_JSON_ELEM_DECL(                                                                                  \
    type, name, default_value, editor_group, editor_type, editor_title, game_compatibility, display_condition) \
  obj[#name] = val.name();
#define DS_FROM_JSON_ELEM_DECL(                                                                                \
    type, name, default_value, editor_group, editor_type, editor_title, game_compatibility, display_condition) \
  if (obj.find(#name) == obj.constEnd()) {                                                                     \
    out->Set##name(default_value);                                                                             \
  } else {                                                                                                     \
    out->Set##name(obj[#name].toVariant().value<type>());                                                      \
  }
#define DS_DEBUG_STRING_ELEM_DECL(                                                                             \
    type, name, default_value, editor_group, editor_type, editor_title, game_compatibility, display_condition) \
  out += QString{"." #name "=%0,"}.arg(this->name());
#define DS_GET_PROPERTIES_FOR_GROUP_ELEM_DECL(                                                                 \
    type, name, default_value, editor_group, editor_type, editor_title, game_compatibility, display_condition) \
  if (GroupIsIn(group, editor_group) && editor_type != DataEditor::NO_EDITOR) {                                \
    keys.emplace_back(#name);                                                                                  \
  }
#define DS_SET_FROM_KEY_ELEM_DECL(                                                                             \
    type, name, default_value, editor_group, editor_type, editor_title, game_compatibility, display_condition) \
  if (key == #name) {                                                                                          \
    return this->Set##name(val.value<type>());                                                                 \
  }
#define DS_GET_FROM_KEY_ELEM_DECL(                                                                             \
    type, name, default_value, editor_group, editor_type, editor_title, game_compatibility, display_condition) \
  if (key == #name) {                                                                                          \
    return QVariant::fromValue(this->name());                                                                  \
  }
#define DS_GET_TITLE_FOR_KEY_ELEM_DECL(                                                                        \
    type, name, default_value, editor_group, editor_type, editor_title, game_compatibility, display_condition) \
  if (key == #name) {                                                                                          \
    return editor_title;                                                                                       \
  }
#define DS_GET_GAME_COMPATIBILITY_ELEM_DECL(                                                                   \
    type, name, default_value, editor_group, editor_type, editor_title, game_compatibility, display_condition) \
  if (key == #name) {                                                                                          \
    return game_compatibility;                                                                                 \
  }
#define DS_GET_EDITOR_TYPE_ELEM_DECL(                                                                          \
    type, name, default_value, editor_group, editor_type, editor_title, game_compatibility, display_condition) \
  if (key == #name) {                                                                                          \
    return editor_type;                                                                                        \
  }
#define DS_GET_KEYS_FOR_EDITOR_TYPE_ELEM_DECL(                                                                 \
    type, name, default_value, editor_group, editor_type, editor_title, game_compatibility, display_condition) \
  if (editor_type == e_type) {                                                                                 \
    keys.append(#name);                                                                                        \
  }

#define DS_DECLARE_STATIC_INIT_FUNC(ClassName)                          \
  static void Init() {                                                  \
    qmlRegisterType<btc2::ClassName>(kBtc2QmlModule, 1, 0, #ClassName); \
    qRegisterMetaType<btc2::ClassName>("btc2::" #ClassName);            \
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
    return Game::NONE;                                                          \
  }

#define DS_DECLARE_STATIC_IS_GAME_COMPATIBLE_FUNC(ClassName, ELEMENTS_LIST)         \
  Q_INVOKABLE static bool IsKeyCompatibleWithGame(const QString& key, Games game) { \
    const bool kIsCompatible{(GetKeyGameCompatiblity(key) & game) != 0};            \
    return kIsCompatible;                                                           \
  }

#define DS_DECLARE_STATIC_GET_EDITOR_TYPE_FOR_KEY_FUNC(ClassName, ELEMENTS_LIST)   \
  Q_INVOKABLE static DataEditor::EditorType GetEditorTypeFor(const QString& key) { \
    ELEMENTS_LIST(DS_GET_EDITOR_TYPE_ELEM_DECL);                                   \
    return DataEditor::NO_EDITOR;                                                  \
  }

#define DS_DECLARE_STATIC_GET_KEYS_FOR_EDITOR_TYPE_FUNC(ClassName, ELEMENTS_LIST)      \
  Q_INVOKABLE static QStringList GetKeysForEditorType(DataEditor::EditorType e_type) { \
    QStringList keys{};                                                                \
    ELEMENTS_LIST(DS_GET_KEYS_FOR_EDITOR_TYPE_ELEM_DECL);                              \
    return keys;                                                                       \
  }

#define DS_DECLARE_STATIC_MATCH_GROUP_FROM_LIST(ClassName, ELEMENTS_LIST)                       \
  Q_INVOKABLE static bool GroupIsIn(const QString& group_name, const QString& group_list_str) { \
    const auto kSplitted{group_list_str.split(",")};                                            \
    for (const auto& group : kSplitted) {                                                       \
      if (group.trimmed() == group_name) {                                                      \
        return true;                                                                            \
      }                                                                                         \
    }                                                                                           \
    return false;                                                                               \
  }

#define DS_DECLARE_MEMBER_RESET_VALUES_FUNC(ClassName, ELEMENTS_LIST) \
  Q_INVOKABLE bool Reset() {                                          \
    bool success{true};                                               \
    ELEMENTS_LIST(DS_RESET_ELEM_DECL);                                \
    return success;                                                   \
  }

#define DS_DECLARE_MEMBER_RESET_VALUES_FOR_GROUP_FUNC(ClassName, ELEMENTS_LIST) \
  Q_INVOKABLE bool Reset(const QString& group_title) {                          \
    bool success{true};                                                         \
    ELEMENTS_LIST(DS_RESET_FOR_GROUP_ELEM_DECL);                                \
    return success;                                                             \
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

#define DS_DECLARE_MEMBER_GET_FROM_KEY_FUNC(ClassName, ELEMENTS_LIST) \
  Q_INVOKABLE QVariant GetFromKey(const QString& key) const {         \
    ELEMENTS_LIST(DS_GET_FROM_KEY_ELEM_DECL);                         \
    return QVariant{};                                                \
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

#define DS_DECLARE_SAVE_AS_FUNC_PROTO(ClassName) bool SaveAs(const ClassName& val, const QString& filename);

#define DS_DECLARE_SAVE_AS_FUNC(ClassName)                            \
  inline bool SaveAs(const ClassName& val, const QString& filename) { \
    return utils::Save(ToJson(val), filename);                        \
  }

#define DS_DECLARE_SET_FROM_JSON_FUNC(ClassName, ELEMENTS_LIST)     \
  inline void SetFromJson(const QJsonObject& obj, ClassName* out) { \
    ELEMENTS_LIST(DS_FROM_JSON_ELEM_DECL);                          \
  }

#define DS_DECLARE_FILL_FROM_FILE_FUNC_PROTO(ClassName) bool FillFromFile(const QString& filename, ClassName* out);

#define DS_DECLARE_FILL_FROM_FILE_FUNC(ClassName)                     \
  inline bool FillFromFile(const QString& filename, ClassName* out) { \
    if (auto doc{utils::Read(filename)}) {                            \
      SetFromJson(doc->object(), out);                                \
      return true;                                                    \
    }                                                                 \
    return false;                                                     \
  }

#define DS_DECLARE_STRUCT(ClassName, ELEMENTS_LIST)                                                           \
  class ClassName;                                                                                            \
  DS_DECLARE_SAVE_AS_FUNC_PROTO(ClassName)                                                                    \
  DS_DECLARE_FILL_FROM_FILE_FUNC_PROTO(ClassName)                                                             \
  class ClassName : public QObject {                                                                          \
    Q_OBJECT                                                                                                  \
   public:                                                                                                    \
    Q_SIGNAL void dataChanged();                                                                              \
                                                                                                              \
    ELEMENTS_LIST(DS_PROPERTY_ELEM_DECL)                                                                      \
   public:                                                                                                    \
    ClassName(const QString& save_path, QObject* parent = nullptr) : QObject(parent), m_m_save_path{} {       \
      SetSavePath(save_path, true);                                                                           \
      connect(this, &ClassName::dataChanged, this, &ClassName::InternalOnChange);                             \
    }                                                                                                         \
    explicit ClassName(QObject* parent = nullptr) : ClassName("", parent) {}                                  \
    DS_DECLARE_STATIC_INIT_FUNC(ClassName);                                                                   \
    DS_DECLARE_STATIC_GET_PROPERTIES_FOR_GROUP_FUNC(ClassName, ELEMENTS_LIST);                                \
    DS_DECLARE_STATIC_GET_TITLE_FOR_FUNC(ClassName, ELEMENTS_LIST);                                           \
    DS_DECLARE_STATIC_GET_GAME_COMPATIBILITY_FUNC(ClassName, ELEMENTS_LIST);                                  \
    DS_DECLARE_STATIC_IS_GAME_COMPATIBLE_FUNC(ClassName, ELEMENTS_LIST);                                      \
    DS_DECLARE_STATIC_GET_EDITOR_TYPE_FOR_KEY_FUNC(ClassName, ELEMENTS_LIST);                                 \
    DS_DECLARE_STATIC_GET_KEYS_FOR_EDITOR_TYPE_FUNC(ClassName, ELEMENTS_LIST);                                \
    DS_DECLARE_STATIC_MATCH_GROUP_FROM_LIST(ClassName, ELEMENTS_LIST);                                        \
                                                                                                              \
    DS_DECLARE_MEMBER_SET_FROM_KEY_FUNC(ClassName, ELEMENTS_LIST);                                            \
    DS_DECLARE_MEMBER_GET_FROM_KEY_FUNC(ClassName, ELEMENTS_LIST);                                            \
                                                                                                              \
    ELEMENTS_LIST(DS_ELEM_ACCESSOR_DECL);                                                                     \
                                                                                                              \
    DS_DECLARE_MEMBER_DEBUG_STRING_FUNC(ClassName, ELEMENTS_LIST);                                            \
    DS_DECLARE_MEMBER_RESET_VALUES_FUNC(ClassName, ELEMENTS_LIST);                                            \
    DS_DECLARE_MEMBER_RESET_VALUES_FOR_GROUP_FUNC(ClassName, ELEMENTS_LIST);                                  \
                                                                                                              \
    bool ReloadFromFile(bool reset_values_if_file_not_found) {                                                \
      if (m_m_save_path.isEmpty() || !QFile::exists(m_m_save_path)) {                                         \
        if (reset_values_if_file_not_found) {                                                                 \
          return Reset();                                                                                     \
        }                                                                                                     \
        return true;                                                                                          \
      }                                                                                                       \
      m_m_inhibit_auto_save = true;                                                                           \
      const auto kSuccess{FillFromFile(m_m_save_path, this)};                                                 \
      m_m_inhibit_auto_save = false;                                                                          \
      if (!kSuccess) {                                                                                        \
        SPDLOG_ERROR("Failed to reload from file <{}>{{{}}}", #ClassName, DebugString());                     \
      }                                                                                                       \
      SPDLOG_DEBUG("Reloaded from file <{}>{{{}}}", #ClassName, DebugString());                               \
      return kSuccess;                                                                                        \
    }                                                                                                         \
    bool SetSavePath(const QString& path, bool reset_value_if_file_not_found) {                               \
      if (m_m_save_path == path) {                                                                            \
        return true;                                                                                          \
      }                                                                                                       \
      m_m_save_path = path;                                                                                   \
      return ReloadFromFile(reset_value_if_file_not_found);                                                   \
    }                                                                                                         \
                                                                                                              \
   private:                                                                                                   \
    ELEMENTS_LIST(DS_STRUCT_ELEM_DECL)                                                                        \
                                                                                                              \
    QString m_m_save_path{};                                                                                  \
    bool m_m_inhibit_auto_save{};                                                                             \
                                                                                                              \
   private:                                                                                                   \
    void InternalOnChange() {                                                                                 \
      if (m_m_save_path.isEmpty() || m_m_inhibit_auto_save) {                                                 \
        return;                                                                                               \
      }                                                                                                       \
      if (!SaveAs(*this, m_m_save_path)) {                                                                    \
        SPDLOG_ERROR("Failed to save on change <{}>{{{}}} to\n{}", #ClassName, DebugString(), m_m_save_path); \
      }                                                                                                       \
    }                                                                                                         \
  };                                                                                                          \
  static void register##ClassName() {                                                                         \
    ClassName::Init();                                                                                        \
  }                                                                                                           \
  Q_COREAPP_STARTUP_FUNCTION(register##ClassName);                                                            \
  DS_DECLARE_OSTREAM_OPERATOR(ClassName);                                                                     \
  DS_DECLARE_TO_JSON_FUNC(ClassName, ELEMENTS_LIST);                                                          \
  DS_DECLARE_SAVE_AS_FUNC(ClassName);                                                                         \
  DS_DECLARE_SET_FROM_JSON_FUNC(ClassName, ELEMENTS_LIST);                                                    \
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
    KEYBOARD_KEY,
    SLIDER,
    SWITCH,
    SEPARATOR,
    COLOR,
    SCREEN_SELECTOR,
    FILE_SELECTOR,
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

/* ELEMENTS_LIST(type, Name, default_value, editor_group_name, DataEditor::EditorType, editor_title, game_compatibility,
 * display_condition) */

}  // namespace btc2
