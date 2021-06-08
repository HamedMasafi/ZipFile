#ifndef TST_ZIPFILETEST_H
#define TST_ZIPFILETEST_H

#include <QObject>
#include <QStringList>

class ZipFileTest : public QObject
{
    Q_OBJECT
    QStringList _files;

public:
    ZipFileTest();
    ~ZipFileTest();

private slots:
    void test_zip();
    void test_unzip();
    void test_qbytearray_extract();
    void cleanupTestCase();
};

#endif // TST_ZIPFILETEST_H
