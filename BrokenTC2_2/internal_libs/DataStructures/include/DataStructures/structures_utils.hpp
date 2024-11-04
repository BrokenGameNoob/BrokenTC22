#pragma once

#include <DataStructures/structures.hpp>

namespace btc2 {

struct ConflictsResults {
  Q_GADGET
  Q_PROPERTY(QStringList leftConflicts MEMBER left_conflicts)
  Q_PROPERTY(QStringList rightConflicts MEMBER right_conflicts)

 public:
  QStringList left_conflicts{};
  QStringList right_conflicts{};

  Q_INVOKABLE bool HasConflicts() const {
    return !left_conflicts.isEmpty() || !right_conflicts.isEmpty();
  }

  static void Init() {
    // qmlRegisterType<btc2::ConflictsResults>(kBtc2QmlModule, 1, 0, "ConflictsResults");
    // qRegisterMetaType<btc2::ConflictsResults>("btc2::ConflictsResults");
  }
};

template <typename LeftType, typename RightType>
ConflictsResults FindKeyboardConflicts(const LeftType& left, const RightType& right) {
  const auto kLeftKeyNames{LeftType::GetKeysForEditorType(DataEditor::EditorType::KEYBOARD_KEY)};
  const auto kRightKeyNames{RightType::GetKeysForEditorType(DataEditor::EditorType::KEYBOARD_KEY)};

  QStringList left_conflicts{};
  QStringList right_conflicts{};

  for (const auto& keyboard_key : kLeftKeyNames) {
    for (const auto& game_key : kRightKeyNames) {
      if (left.GetFromKey(keyboard_key) == right.GetFromKey(game_key)) {
        left_conflicts.append(keyboard_key);
        right_conflicts.append(game_key);
      }
    }
  }
  if (!left_conflicts.isEmpty() || !right_conflicts.isEmpty()) {
    return ConflictsResults{left_conflicts, right_conflicts};
  }
  return {};
}

}  // namespace btc2
