#include "zipfile.h"

#include <unzip.h>
#include <zip.h>

#include <QBuffer>
#include <QDataStream>
#include <QDate>
#include <QDebug>
#include <QFile>
#include <QFileInfo>

#define setErrorString(x) qDebug() << x

#define CHECK_ZIP_BOOL(x) { if (_mode != Create && _mode != Append) return false; }
#define CHECK_UNZIP_BOOL(x) { if (_mode != Open) return false; }

tm_zip_s GenerateZipTimeInfo()
{
    auto d = QDate::currentDate();
    auto t = QTime::currentTime();

    tm_zip_s tmp_zip_time_info;
    tmp_zip_time_info.tm_year = d.year();
    tmp_zip_time_info.tm_mon  = d.month();
    tmp_zip_time_info.tm_mday = d.day();
    tmp_zip_time_info.tm_hour = t.hour();
    tmp_zip_time_info.tm_min  = t.minute();
    tmp_zip_time_info.tm_sec  = t.second();

    return tmp_zip_time_info;
}
zip_fileinfo GenerateZipInfo(tm_zip_s tmp_data)
{
    zip_fileinfo tmp_fileinfo;
    tmp_fileinfo.tmz_date = tmp_data;
    tmp_fileinfo.dosDate  = 0;
    tmp_fileinfo.internal_fa = 0;
    tmp_fileinfo.external_fa = 0;

    return tmp_fileinfo;
}

ZipFile::ZipFile(const QString &filePath) : _zipFileName(filePath)
{

}

bool ZipFile::open(OpenType mode)
{
    switch (mode) {
    case Create:
        if (QFile::exists(_zipFileName) && !QFile::remove(_zipFileName))
            return false;
        _zipFile = zipOpen(_zipFileName.toStdString().c_str(), false);
        break;
    case Append:
        if (QFile::exists(_zipFileName))
            _zipFile = zipOpen(_zipFileName.toStdString().c_str(), true);
        else
            _zipFile = zipOpen(_zipFileName.toStdString().c_str(), false);
        break;
    case Open:
        _zipFile = unzOpen(_zipFileName.toStdString().c_str());
        break;
    case NotOpenned:
        return false;
    }
    _mode = mode;
    return _zipFile == 0;
}
bool ZipFile::close()
{
    switch (_mode) {
    case Create:
    case Append:
        return zipClose(_zipFile, nullptr) == Z_OK;
    case Open:
        return unzClose(_zipFile) == UNZ_OK;
    case NotOpenned:
        return false;
    }
    return false; //UNREACHABLE
}

bool ZipFile::addEntry(const QString &filePath)
{
    QFileInfo fi{filePath};
    if (!fi.exists())
        return false;
    return addEntry(filePath, fi.fileName());
}
bool ZipFile::addEntry(const QString &filePath, const QString &entryName)
{
    QFile f{filePath};
    return addEntry(f, entryName);
    return true;
}

bool ZipFile::addEntry(const QByteArray &data, const QString &entryName)
{
    CHECK_ZIP_BOOL();

    zip_fileinfo tmp_file_info = GenerateZipInfo(GenerateZipTimeInfo());

    zipOpenNewFileInZip3(_zipFile,
                         entryName.toStdString().c_str(),
                         &tmp_file_info,
                         nullptr,
                         0,
                         nullptr,
                         0,
                         "",
                         Z_DEFLATED,
                         _compressionLevel,
                         0,
                         15,
                         8,
                         Z_DEFAULT_STRATEGY,
                         nullptr,
                         crc32(0, (uchar *) (data.data()), data.size()));

    zipWriteInFileInZip(_zipFile, data.data(), data.size());
    return true;
}

bool ZipFile::addEntry(QIODevice &in, const QString &entryName)
{
    if (!in.isOpen() && !in.open(QIODevice::ReadOnly))
        return false;

    auto data = in.readAll();
    return addEntry(data, entryName);
}

QStringList ZipFile::readEntries()
{
    int n = unzGoToFirstFile(_zipFile);

    if (n != UNZ_OK) {
        qDebug() << "Unable to read first entry";
        return QStringList();
    }

    QStringList list;
    while (n == UNZ_OK) {
        unz_file_info64 fi;
        char filename[256];
        unzGetCurrentFileInfo64(_zipFile, &fi, filename, sizeof(filename), NULL, 0, NULL, 0);
        n = unzGoToNextFile(_zipFile);
        list.append(filename);
    }

    return list;
}

bool ZipFile::extractEntry(const QString &filePath, const QString &name)
{
    CHECK_UNZIP_BOOL();

    QFile f{filePath};

    auto b = extractEntry(f, name);
    f.close();
    return b;
}
bool ZipFile::extractEntry(QIODevice &out, const QString &name)
{
    CHECK_UNZIP_BOOL();

    if (!gotoEntry(name))
        return false;

    if (!out.open(QIODevice::WriteOnly))
        return false;

    int err;
    char buf[8192];
    bool result = true;

    //    if (!password.isEmpty())
    //        err = unzOpenCurrentFilePassword(_zipFile, password.toLocal8Bit());
    //    else
    err = unzOpenCurrentFile(_zipFile);
    if (err != UNZ_OK) {
        setErrorString("Error opening archive entry");
        return false;
    }

    do {
        err = unzReadCurrentFile(_zipFile, buf, 8192);
        if (!err)
            break;

        if (err < 0) {
            setErrorString("Error reading current archive entry");
            break;
        }
        if (err > 0) {
            qint64 written;

            do {
                written = out.write(buf, err);
                if (written < 0)
                    break;
                err -= written;
            } while (err > 0);

            if (written < 0)
                err = -1;
            else
                continue;
        }
        if (!err)
            setErrorString("Fuck");
    } while (err >= 0);

    if (err < 0)
        result = false;

    err = unzCloseCurrentFile(_zipFile);

    if (err != UNZ_OK)
        result = false;

    return result;
}

bool ZipFile::extractEntry(QByteArray &data, const QString &name)
{
    QBuffer s{&data};

    return extractEntry(s, name);
}

void ZipFile::setCompressionLevel(int level)
{
    if (level < 0 || level > 9) {
        qDebug() << "Error in setCompressionLevel: the level must be in range of (0-9)";
        return;
    }
    _compressionLevel = level;
}

int ZipFile::compressionLevel() const
{
    return _compressionLevel;
}

bool ZipFile::gotoFirstEntry()
{
    CHECK_UNZIP_BOOL();

    if( UNZ_OK == unzGoToFirstFile(_zipFile)) {
        unz_file_info64 fi;
        char filename[256];
        unzGetCurrentFileInfo64(_zipFile, &fi, filename, sizeof(filename), NULL, 0, NULL, 0);
        _currentEntryName = QString{filename};
        return true;
    }
    return false;
}

bool ZipFile::gotoNextEntry()
{
    CHECK_UNZIP_BOOL();

    if( UNZ_OK == unzGoToNextFile(_zipFile)) {
        unz_file_info64 fi;
        char filename[256];
        unzGetCurrentFileInfo64(_zipFile, &fi, filename, sizeof(filename), NULL, 0, NULL, 0);
        _currentEntryName = QString{filename};
        return true;
    }
    return false;
}

bool ZipFile::gotoEntry(const QString &name)
{
    if (name == _currentEntryName)
        return true;

    int n = unzGoToFirstFile(_zipFile);

    if (n != UNZ_OK)
        return false;

    while (n == UNZ_OK) {
        unz_file_info64 fi;
        char filename[256];
        unzGetCurrentFileInfo64(_zipFile, &fi, filename, sizeof(filename), NULL, 0, NULL, 0);

        QString tmp{filename};
        if (name == tmp) {
            _currentEntryName = name;
            return true;
        }

        n = unzGoToNextFile(_zipFile);
    }

    _currentEntryName.clear();
    return false;
}
