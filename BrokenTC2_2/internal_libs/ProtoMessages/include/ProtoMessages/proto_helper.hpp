#pragma once

#include <QMetaEnum>
#include <QProtobufSerializer>

#include <Logger/logger.hpp>
#include <google/protobuf/descriptor.h>

namespace btc2 {

template <class EnumType>
concept QEnumType = QtPrivate::IsQEnumHelper<EnumType>::Value == true;

/*!
 * \brief ForEach
 * \param lambda: bool(EnumType val, const char* key, const QMetaEnum& e) (return true to exit and false to continue)
 */
template <QEnumType EnumType, typename Lambda_t>
void ForEach(Lambda_t lambda) {
  QMetaEnum e{QMetaEnum::fromType<EnumType>()};

  for (int k = 0; k < e.keyCount(); k++) {
    EnumType val = (EnumType)e.value(k);
    if (lambda(val, QMetaEnum::fromType<EnumType>().valueToKey(val), e)) {
      return;
    }
  }
}

template <QEnumType EnumType>
auto EnumValuesCount() {
  return QMetaEnum::fromType<EnumType>().keyCount();
}

}  // namespace btc2
