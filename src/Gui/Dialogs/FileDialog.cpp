#include "FileDialog.h"

CFileInfo::CFileInfo()
{

}

QDateTime CFileInfo::lowerDate(QFileInfo fileInfo)
{
    if( fileInfo.birthTime().isValid() && (fileInfo.birthTime() < fileInfo.lastModified()))
        return fileInfo.birthTime();
    else
        return fileInfo.lastModified();
}

QString CFileInfo::sizeToStringWithUnit(QFileInfo fileInfo)
{
    int counter = 0;
    double size = fileInfo.size();
    while(size > 1000 && counter < 4)
      {
        size /= 1000;
        counter ++;
      }
    QString unit;
    if(counter == 0)
        unit = "B";
    else if(counter == 1)
        unit = "kB";
    else if(counter == 2)
        unit = "MB";
    else if(counter == 3)
        unit = "GB";

    return QString::number(size, ',', 1).replace(QRegularExpression("[.]"), ",") + " " + unit;
}

QString CFileDialog::m_startDir = "";

CFileDialog::CFileDialog(QWidget *parent) : QFileDialog(parent)
{
  connect(this, SIGNAL(directoryEntered(const QString &)), this, SLOT(onDirectoryEntered(const QString &)));
}

QString CFileDialog::getFileName(QWidget *parent, const QString &title, const QString &dir, CFileDialog::Operation operation, const QString &filter, Options options, bool absolutePath, const QString &name)
{
  CFileDialog dialog(parent);

  dialog.setWindowTitle(title);
  m_startDir = dir;
  m_startDir.replace("//", "/");
  dialog.setDirectory(dir);
  dialog.setNameFilters(filter.split(";;"));
  if (!name.isEmpty())
      dialog.selectFile(name);

  dialog.setOptions(options | QFileDialog::DontUseNativeDialog);
  if (operation == Operation_Open)
  {
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
  }
  else //OperationSave
  {
    dialog.selectFile(dir);
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
  }

  int ret = dialog.exec();
  if (ret == QFileDialog::Accepted && dialog.selectedFiles().count() > 0)
  {
    QFileInfo fileInfo (dialog.selectedFiles().first());
    if (absolutePath)
      return fileInfo.absoluteFilePath();

    if (!m_startDir.isEmpty())
      return fileInfo.absoluteFilePath().remove(0, fileInfo.absoluteFilePath().indexOf(dir));
    return fileInfo.fileName();
  }
  return QString();
}

void CFileDialog::onDirectoryEntered(const QString &dir)
{
  if (!dir.contains(m_startDir))
    setDirectory(m_startDir);
}

QString CFileDialog::getOpenFileName(QWidget *parent, const QString &caption, const QString &dir, const QString &filter, Options options, bool absolutePath, const QString &name)
{
  return getFileName(parent, caption, dir, Operation_Open, filter, options, absolutePath, name);
}

QString CFileDialog::getSaveFileName(QWidget *parent, const QString &caption, const QString &dir, const QString &filter, Options options, bool absolutePath, const QString &name)
{
  return getFileName(parent, caption, dir, Operation_Save, filter, options, absolutePath, name);
}
