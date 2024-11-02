#pragma once

#include <QObject>
#include <QScreen>

namespace btc2 {

class ScreenOverlaySelector : public QObject {
  Q_OBJECT

 signals:
  void selectedScreenChanged();

 public:
  static void Init();
  static QString ScreenName(const QScreen *screen);
  Q_INVOKABLE static const QStringList &GetAvailableScreens();
  static const std::map<QString, QScreen *> &GetAvailableScreensMap();

 public:
  explicit ScreenOverlaySelector(QObject *parent = nullptr);

  const QString &GetSelectedScreenName() const;
  void SetSelectedScreenName(const QString &screen);

  QScreen *GetSelectedScreen();

 private:
  QString m_selected_screen{};
};

}  // namespace btc2
