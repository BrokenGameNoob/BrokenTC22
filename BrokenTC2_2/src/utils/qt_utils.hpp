#pragma once

#define kQmlModuleName "btc2"

/*! @brief Usage example: CREGISTER_QML_TYPE(btc2, QMLLogHelper);*/
#define CREGISTER_QML_TYPE(nspce, class_name) qmlRegisterType<nspce::class_name>(kQmlModuleName, 1, 0, #class_name)
#define CREGISTER_QML_UNCREATABLE_TYPE(nspce, class_name, reason) \
  qmlRegisterUncreatableType<nspce::class_name>(kQmlModuleName, 1, 0, #class_name, reason)

/*! @brief Usage example: CREGISTER_QML_SINGLETON_I(btc2, ServiceManager);
 *  @note This macro must be use in a class function (typically a static "Init" function called from main) and requires
 * a static function "I()" providing the singleton instance as a reference
 * Example:
 * @code cpp
 *  static ServiceManager& I() {
      static auto instance{ServiceManager()};
      return instance;
    }
 * @endcode
 */
#define CREGISTER_QML_SINGLETON_I(nspce, class_name)                  \
  CREGISTER_QML_UNCREATABLE_TYPE(nspce, class_name, "Cpp singleton"); \
  qmlRegisterSingletonInstance<nspce::class_name>(kQmlModuleName, 1, 0, #class_name, &nspce::class_name::I());

/* --- QObject macro utils --- */

/*! @brief Usage example: QML_LITERAL_CONSTANT(QString, kAccent, "#F7E733");
 * @note To be used in a QObject class
 */
#define QML_LITERAL_CONSTANT(property_type, variable_name, value) \
 public:                                                          \
  static constexpr auto variable_name{value};                     \
                                                                  \
 private:                                                         \
  Q_PROPERTY(property_type variable_name MEMBER variable_name CONSTANT FINAL)
