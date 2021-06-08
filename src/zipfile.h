#ifndef ZIPFILE_H
#define ZIPFILE_H

#include <QString>
#include "zipfile_global.h"

class QIODevice;
class ZIP_FILE_EXPORT ZipFile
{
public:
    enum OpenType
    {
        NotOpenned,
        Create,
        Append,
        Open,
    };
    ZipFile(const QString &filePath);

    bool open(OpenType mode);
    bool close();

    bool addEntry(const QString &filePath);
    bool addEntry(QIODevice &in, const QString &entryName);
    bool addEntry(const QString &filePath, const QString &entryName);
    bool addEntry(const QByteArray &data, const QString &entryName);

    QStringList readEntries();

    bool extractEntry(const QString &filePath, const QString &name);
    bool extractEntry(QIODevice &out, const QString &name);
    bool extractEntry(QByteArray &data, const QString &name);

private:
    bool gotoEntry(const QString &name);
    void *_zipFile;
    QString _zipFileName;
    OpenType _mode{NotOpenned};
};

#endif // ZIPFILE_H
