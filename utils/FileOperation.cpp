#include "FileOperation.h"
#include <QFileDevice>
#include <QFileInfo>
#include <QFileDialog>
#include <fstream>
#include <cstring>
#include <zlib.h>

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

bool FileZipOperation::compressFilesToZip(const std::vector<std::string>& fileNames, 
    const std::string& outputFileName)
{
    // 打开输出文件流
    std::ofstream outputFile(outputFileName, std::ios::binary);
    if (!outputFile.is_open()) {
        std::cerr << "Failed to open output file: " << outputFileName << std::endl;
        return false;
    }
    // 初始化 zlib 压缩流
    z_stream deflateStream;
    deflateStream.zalloc = Z_NULL;
    deflateStream.zfree = Z_NULL;
    deflateStream.opaque = Z_NULL;
    deflateStream.avail_in = 0;
    deflateStream.next_in = Z_NULL;
    if (deflateInit(&deflateStream, Z_DEFAULT_COMPRESSION) != Z_OK) {
        std::cerr << "Failed to initialize deflate stream!" << std::endl;
        return false;
    }

    // 逐个压缩文件并写入到输出文件
    for (const auto& fileName : fileNames) {
        // 打开输入文件流
        std::ifstream inputFile(fileName, std::ios::binary);
        if (!inputFile.is_open()) {
            std::cerr << "Failed to open input file: " << fileName << std::endl;
            return false;
        }

        // 压缩并写入到输出文件
        char inputBuffer[1024];
        do {
            inputFile.read(inputBuffer, sizeof(inputBuffer));
            deflateStream.avail_in = static_cast<uInt>(inputFile.gcount());
            deflateStream.next_in = reinterpret_cast<Bytef*>(inputBuffer);

            do {
                char outputBuffer[1024];
                deflateStream.avail_out = sizeof(outputBuffer);
                deflateStream.next_out = reinterpret_cast<Bytef*>(outputBuffer);
                if (deflate(&deflateStream, Z_FINISH) == Z_STREAM_ERROR) {
                    std::cerr << "Failed to deflate input data!" << std::endl;
                    return false;
                }

                outputFile.write(outputBuffer, sizeof(outputBuffer) - deflateStream.avail_out);
            } while (deflateStream.avail_out == 0);
        } while (!inputFile.eof());

        inputFile.close();
    }

    // 结束压缩
    do {
        char outputBuffer[1024];
        deflateStream.avail_out = sizeof(outputBuffer);
        deflateStream.next_out = reinterpret_cast<Bytef*>(outputBuffer);
        if (deflate(&deflateStream, Z_FINISH) == Z_STREAM_ERROR) {
            std::cerr << "Failed to deflate input data!" << std::endl;
            return false;
        }

        outputFile.write(outputBuffer, sizeof(outputBuffer) - deflateStream.avail_out);
    } while (deflateStream.avail_out == 0);

    // 结束 zlib 压缩流
    deflateEnd(&deflateStream);

    // 关闭输出文件流
    outputFile.close();

    return true;
}
