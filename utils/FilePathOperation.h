#pragma once
class QString;
namespace FilePathOperation {
	QString filePathToTxt(QString s); //将同名文件后缀改为txt
	QString getFileBaseName(QString path); //获取指定路径文件的文件名
	QString getFileSuffix(const QString& file_name); //获取文件后缀
	QString getFileParentPath(QString file_path); //文件父级文件夹路径
	QString getFileFolder(QString file_path); //获取文件目录名
}
