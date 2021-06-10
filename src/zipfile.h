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

    void setCompressionLevel(int level);;
    int compressionLevel() const;

    //iterators
    class iterator;
    class entry
    {
        ZipFile *_parent;
        QString _name;

    public:
        entry(ZipFile *parent) : _parent(parent) {}

        bool extract(const QString &filePath) { return _parent->extractEntry(filePath, _name); }
        bool extract(QIODevice &out) { return _parent->extractEntry(out, _name); }
        bool extract(QByteArray &data) { return _parent->extractEntry(data, _name); }

        const QString &name() const {
            return _name;
        };

        friend class iterator;
    };

    class iterator {
        entry _entry;
        ZipFile *_parent;
        bool _hasNext{true};

    public:
        iterator(ZipFile *parent) : _entry{parent}, _parent{parent} {
            _hasNext = _parent->gotoFirstEntry();
            _entry._name = _parent->_currentEntryName;
        }
        iterator() : _entry(nullptr), _hasNext(false) {}
        inline bool operator==(const iterator &o) const noexcept { return _hasNext == o._hasNext; }
        inline bool operator!=(const iterator &o) const noexcept { return _hasNext != o._hasNext; }
        inline iterator &operator++()
        {
            _hasNext = _parent->gotoNextEntry();
            _entry._name = _parent->_currentEntryName;

            return *this;
        }
        inline const entry &operator*() const { return _entry; }
    };
    friend class iterator;

    inline iterator begin() { return iterator(this); };
    inline iterator end() { return iterator(); }

private:
    bool gotoFirstEntry();
    bool gotoNextEntry();
    bool gotoEntry(const QString &name);
    void *_zipFile;
    QString _currentEntryName;
    QString _zipFileName;
    OpenType _mode{NotOpenned};
    int _compressionLevel{0};
};

#endif // ZIPFILE_H
