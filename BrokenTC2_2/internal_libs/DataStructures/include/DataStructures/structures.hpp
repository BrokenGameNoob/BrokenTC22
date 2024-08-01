#pragma once

#include <QCoreApplication>
#include <QQmlEngine>
#include <QString>
#include <QtQml>
#include <cstdint>

#include <Logger/logger.hpp>
#include <Utils/json_utils.hpp>

namespace btc2 {

constexpr auto kQmlModule{"btc2"};

#define DS_ELEM_ACCESSOR_DECL(type, name, editor_group, editor_type, editor_title) \
  bool Set##name(type val) {                                                       \
    m_##name = std::move(val);                                                     \
    emit dataChanged();                                                            \
    return true;                                                                   \
  }                                                                                \
  const type& name() const {                                                       \
    return m_##name;                                                               \
  }                                                                                \
  type& name() {                                                                   \
    return m_##name;                                                               \
  }

#define DS_STRUCT_ELEM_DECL(type, name, editor_group, editor_type, editor_title) type m_##name{};
#define DS_PROPERTY_ELEM_DECL(type, name, editor_group, editor_type, editor_title) \
  Q_PROPERTY(type name READ name WRITE Set##name NOTIFY dataChanged)
#define DS_TO_JSON_ELEM_DECL(type, name, editor_group, editor_type, editor_title) obj[#name] = val.name();
#define DS_FROM_JSON_ELEM_DECL(type, name, editor_group, editor_type, editor_title) \
  out->Set##name(obj[#name].toVariant().value<type>());
#define DS_DEBUG_STRING_ELEM_DECL(type, name, editor_group, editor_type, editor_title) \
  out += QString{"." #name "=%0,"}.arg(this->name());
#define DS_GET_PROPERTIES_FOR_GROUP_ELEM_DECL(type, name, editor_group, editor_type, editor_title) \
  if (group == editor_group && editor_type != DataEditor::kNone) {                                 \
    keys.emplace_back(#name);                                                                      \
  }
#define DS_SET_FROM_KEY_ELEM_DECL(type, name, editor_group, editor_type, editor_title) \
  if (key == #name) {                                                                  \
    return this->Set##name(val.value<type>());                                         \
  }
#define DS_GET_TITLE_FOR_KEY_ELEM_DECL(type, name, editor_group, editor_type, editor_title) \
  if (key == #name) {                                                                       \
    return editor_title;                                                                    \
  }

#define DS_DECLARE_STATIC_INIT_FUNC(ClassName)                      \
  static void Init() {                                              \
    qmlRegisterType<btc2::ClassName>(kQmlModule, 1, 0, #ClassName); \
  }

#define DS_DECLARE_STATIC_GET_PROPERTIES_FOR_GROUP_FUNC(ClassName, ELEMENTS_LIST) \
  Q_INVOKABLE static QStringList GetPropertiesKeysFor(const QString& group) {     \
    SPDLOG_DEBUG(#ClassName "::GetPropertiesKeysFor({})", group);                 \
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
    kNone,
    kDisplayOnly,
    kTextEdit,
  };
  Q_ENUM(EditorType)
};

/* ELEMENTS_LIST(type, Name, editor_group_name, DataEditor::EditorType, editor_title) */

#define Dummy_STRUCT_ELEMENTS_LIST(FUNC)                                               \
  FUNC(QString, Name, "global", DataEditor::kDisplayOnly, QObject::tr("Profile name")) \
  FUNC(int32_t, GearUp, "gear", DataEditor::kDisplayOnly, QObject::tr("Gear down"))    \
  FUNC(int32_t, GearDown, "gear", DataEditor::kDisplayOnly, QObject::tr("Gear up"))    \
  FUNC(int32_t, Clutch, "global", DataEditor::kNone, QObject::tr("Clutch key"))

DS_DECLARE_STRUCT(Dummy, Dummy_STRUCT_ELEMENTS_LIST);

}  // namespace btc2
