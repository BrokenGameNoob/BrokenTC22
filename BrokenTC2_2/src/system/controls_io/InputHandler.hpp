#pragma once

#include <QObject>

class InputHandler : public QObject
{
  Q_OBJECT
 public:
  explicit InputHandler(QObject *parent = nullptr);

 signals:

};

