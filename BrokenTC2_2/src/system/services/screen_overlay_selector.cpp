#include "screen_overlay_selector.hpp"

#include <QApplication>

#include <utils/qt_utils.hpp>

namespace btc2 {

void ScreenOverlaySelector::Init() {
  CREGISTER_QML_UNCREATABLE_TYPE(btc2, ScreenOverlaySelector, "CppOwned");
}

QString ScreenOverlaySelector::ScreenName(const QScreen *screen) {
  static QRegularExpression reToRemove{"(\\.)|(/)|(\\\\)"};
  return screen->name().remove(reToRemove);
}

const QStringList &ScreenOverlaySelector::GetAvailableScreens() {
  static QStringList screens{};
  if (screens.empty()) {
    for (const auto &e : GetAvailableScreensMap()) {
      screens << e.first;
    }
  }
  return screens;
}

const std::map<QString, QScreen *> &ScreenOverlaySelector::GetAvailableScreensMap() {
  static std::map<QString, QScreen *> screens{};

  if (screens.empty()) {
    auto availableScreens{QApplication::screens()};
    int i{};
    std::map<QString, int32_t> screenCount{};
    qInfo() << "Found " << availableScreens.size() << " screens";
    for (const auto &e : availableScreens) {
      qInfo() << "\t-> " << ScreenName(e);
    }
    for (const auto &e : availableScreens) {
      const auto kTmpScreenName{ScreenName(e)};
      screenCount[kTmpScreenName]++;
      const auto kDisplayName{screenCount[kTmpScreenName] > 1
                                  ? QString{"%0_%1"}.arg(kTmpScreenName).arg(screenCount[kTmpScreenName])
                                  : std::move(kTmpScreenName)};
      screens[kDisplayName] = availableScreens[i];
      ++i;
    }
  }
  return screens;
}

ScreenOverlaySelector::ScreenOverlaySelector(QObject *parent) : QObject{parent} {}

const QString &ScreenOverlaySelector::GetSelectedScreenName() const {
  return m_selected_screen;
}

void ScreenOverlaySelector::SetSelectedScreenName(const QString &screen) {
  const auto kSelectedScreenBefore{m_selected_screen};
  if (GetAvailableScreensMap().contains(screen)) {
    m_selected_screen = screen;
    emit selectedScreenChanged();
  } else {
    m_selected_screen = GetAvailableScreens().first();
  }

  if (kSelectedScreenBefore != m_selected_screen) {
    emit selectedScreenChanged();
  }
}

QScreen *ScreenOverlaySelector::GetSelectedScreen() {
  if (GetAvailableScreensMap().contains(m_selected_screen)) {
    return GetAvailableScreensMap().at(m_selected_screen);
  }
  SetSelectedScreenName(GetAvailableScreens().first());
  return GetAvailableScreensMap().at(m_selected_screen);
}

}  // namespace btc2
