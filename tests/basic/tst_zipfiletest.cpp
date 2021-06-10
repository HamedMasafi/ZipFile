#include "tst_zipfiletest.h"
#include <zipfile.h>
#include <QDir>
#include <QFile>
#include <QtTest>
#include <QRandomGenerator>

// add necessary includes here

//#define PATH "/doc/dev/tmp/zip-test/test/"

ZipFileTest::ZipFileTest()
{
    {

    } {
        //        ZipFile z{PATH "test.zip"};
        //        z.open(ZipFile::Open);
        //        qDebug() << z.readEntries();

        //        qDebug() << "extract" << z.extractEntry(PATH "2.png", "1.png");
    }

    QDir d{PATH};
    _files = d.entryList(QDir::Files | QDir::NoDotAndDotDot);
}

ZipFileTest::~ZipFileTest() {}

void ZipFileTest::test_zip()
{
    QString zipFileName = QStringLiteral("%1/out.zip").arg(PATH);
    ZipFile z{zipFileName};
    QVERIFY(z.open(ZipFile::Create));
    for (auto &f : _files)
        QVERIFY2(z.addEntry(QStringLiteral("%1/%2").arg(PATH, f)),
                 QString("Unzip" + f).toLocal8Bit().data());
    QVERIFY(z.close());
    qDebug() << zipFileName;
    QVERIFY(QFile::exists(zipFileName));
}

void ZipFileTest::test_unzip()
{
    QString unzipDir = QStringLiteral("%1/out_test/").arg(PATH);
    QString zipFileName = QStringLiteral("%1/out.zip").arg(PATH);
    ZipFile z{zipFileName};
    QVERIFY(z.open(ZipFile::Open));

    QDir().mkpath(unzipDir);

    for (auto &f : _files) {
        QVERIFY(z.extractEntry(unzipDir + f, f));
        QVERIFY(QFile::exists(unzipDir + f));
    }
    QVERIFY(z.close());
}

void ZipFileTest::test_qbytearray_extract()
{
    QByteArray ba;
    for (int i = 0; i < 1000; i++)
        ba.append(QString::number(QRandomGenerator::global()->bounded(0, 10000)).toLocal8Bit());

    QString zipFileName = QStringLiteral("%1/bytearray.zip").arg(PATH);
    {
        ZipFile z{zipFileName};
        QVERIFY(z.open(ZipFile::Create));
        QVERIFY(z.addEntry(ba, "sample"));
        QVERIFY(z.close());
    }
    {
        QByteArray ba2;
        ZipFile z2{zipFileName};
        QVERIFY(z2.open(ZipFile::Open));
        QVERIFY(z2.extractEntry(ba2, "sample"));
        QVERIFY(z2.close());
        QCOMPARE(ba, ba2);
    }
}

void ZipFileTest::test_iterator()
{
    QString zipFileName = QStringLiteral("%1/out.zip").arg(PATH);
    ZipFile z{zipFileName};
    QVERIFY(z.open(ZipFile::Open));

    auto files2 = _files;

    for (auto &e: z) {
        QVERIFY(_files.contains(e.name()));
        files2.removeOne(e.name());
    }
    QCOMPARE(files2.size(), 0);
}

void ZipFileTest::cleanupTestCase() {

    QString unzipDir = QStringLiteral("%1/out_test/").arg(PATH);
    QString zipFileName = QStringLiteral("%1/out.zip").arg(PATH);
    QFile::remove(zipFileName);
    for (auto &f: _files)
        QFile::remove(unzipDir + f);
    QDir().rmdir(unzipDir);
}

QTEST_APPLESS_MAIN(ZipFileTest)

//#include "tst_zipfiletest.moc"
