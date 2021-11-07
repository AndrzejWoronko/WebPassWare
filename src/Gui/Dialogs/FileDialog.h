#ifndef CFILEDIALOG_H
#define CFILEDIALOG_H

#include "Global.h"
#include <QRegularExpression>
#include <QFileDialog>
#include <QFileInfo>

class CFileInfo : public QFileInfo
{
public:
    CFileInfo();

    static QDateTime lowerDate(QFileInfo fileInfo);
    static QString sizeToStringWithUnit(QFileInfo fileInfo);
};


class CFileDialog : public QFileDialog
{
  Q_OBJECT
public:
  explicit CFileDialog(QWidget *parent = NULL);

  static QString getOpenFileName(QWidget *parent, const QString &title, const QString &dir, const QString &filter = QString(""), Options options = Options(), bool absolutePath = false, const QString &name = QString());

  static QString getSaveFileName(QWidget *parent, const QString &title, const QString &dir, const QString &filter = QString(""), Options options = Options(), bool absolutePath = false, const QString &name = QString());

private:
  enum Operation
  {
    Operation_Save = 0,
    Operation_Open
  };
  static QString m_startDir;

  //filter to np tr("Obrazy (*.jpg *.png);; Wszystkie (*.*)")

  static QString getFileName(QWidget *parent, const QString &title, const QString &dir, Operation operation,  const QString &filter = QString(""), Options options = Options(), bool absolutePath = false, const QString &name = QString());

private slots:
  void onDirectoryEntered(const QString &dir);

};

#endif // CFILEDIALOG_H
