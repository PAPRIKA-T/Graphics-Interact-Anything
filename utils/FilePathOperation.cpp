#include "FilePathOperation.h"
#include <QFileDevice>
#include <QFileInfo>
#include <QFileDialog>

QString FilePathOperation::filePathToTxt(QString s)
{
    QFileInfo info = QFileInfo(s);
    QString file_name = info.fileName();
    QString parent_path = s.remove(file_name);
    file_name = FilePathOperation::getFileBaseName(s);
    QString full_path = parent_path + file_name + ".txt";
    return full_path;
}

QString FilePathOperation::getFileBaseName(QString path)
{
    QString file_base_name;
    QStringList list = path.split('.');
    list.removeLast();
    path = list.join(".");
    list = path.split('/');
    file_base_name = list.last();
    return file_base_name;
}

QString FilePathOperation::getFileSuffix(const QString& file_name)
{
    QFileInfo info = QFileInfo(file_name);
    QString file_full_name = info.fileName();
    QString file_base_name = getFileBaseName(file_name);
    QString file_suffix = file_full_name.remove(file_base_name);
    return file_suffix;
}

QString FilePathOperation::getFileParentPath(QString file_path)
{
    QFileInfo info = QFileInfo(file_path);
    QString file_name = info.fileName();
    QString parent_path = file_path.remove(file_name);
    return parent_path;
}

QString FilePathOperation::getFileFolder(QString file_path)
{
    QString folder;
    QStringList list = file_path.split('/');
    folder = list.at(list.size() - 2);
    return folder;
}