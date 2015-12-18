#include "artworkrepository_tests.h"
#include "../xpiks-qt/Models/artworksrepository.h"

void ArtworkRepositoryTests::simpleAccountFileTest() {
    Models::ArtworksRepository repository;

#ifdef Q_OS_WIN
    QString filename = "C:/path/to/some/file";
    QString directory = "C:/path/to/some";
#else
    QString filename = "/path/to/some/file";
    QString directory = "/path/to/some";
#endif

    bool status = repository.accountFile(filename);

    QCOMPARE(status, true);
    QCOMPARE(repository.getArtworksSourcesCount(), 1);
    QCOMPARE(repository.getDirectory(0), directory);
    QCOMPARE(repository.getFilesCountForDirectory(directory), 1);
}

void ArtworkRepositoryTests::accountSameFileTest() {
    Models::ArtworksRepository repository;

#ifdef Q_OS_WIN
    QString filename = "C:/path/to/some/file";
    QString directory = "C:/path/to/some";
#else
    QString filename = "/path/to/some/file";
    QString directory = "/path/to/some";
#endif

    repository.accountFile(filename);
    bool status = repository.accountFile(filename);

    QCOMPARE(status, false);
    QCOMPARE(repository.getArtworksSourcesCount(), 1);
    QCOMPARE(repository.getDirectory(0), directory);
    QCOMPARE(repository.getFilesCountForDirectory(directory), 1);
}

void ArtworkRepositoryTests::addFilesFromOneDirectoryTest() {
    Models::ArtworksRepository repository;

#ifdef Q_OS_WIN
    QString filenameTemplate = "C:/path/to/some/file%1.jpg";
    QString directory = "C:/path/to/some";
#else
    QString filenameTemplate = "/path/to/some/file%1.jpg";
    QString directory = "/path/to/some";
    #endif

    bool anyWrong = false;
    int count = 5;
    while (count--) {
        QString filename = filenameTemplate.arg(5 - count - 1);
        if (!repository.accountFile(filename)) {
            anyWrong = true;
        }
    }

    QCOMPARE(anyWrong, false);
    QCOMPARE(repository.getArtworksSourcesCount(), 1);
    QCOMPARE(repository.getFilesCountForDirectory(directory), 5);
}

void ArtworkRepositoryTests::addAndRemoveSameFileTest() {
    Models::ArtworksRepository repository;

#ifdef Q_OS_WIN
    QString filename = "C:/path/to/some/file";
    QString directory = "C:/path/to/some";
#else
    QString filename = "/path/to/some/file";
    QString directory = "/path/to/some";
#endif

    bool status = repository.accountFile(filename);
    QCOMPARE(status, true);

    bool removeResult = repository.removeFile(filename, directory);
    repository.cleanupEmptyDirectories();

    QCOMPARE(repository.getArtworksSourcesCount(), 0);
    QCOMPARE(removeResult, true);
}

void ArtworkRepositoryTests::removeNotExistingFileTest() {
    Models::ArtworksRepository repository;

#ifdef Q_OS_WIN
    QString filename1 = "C:/path/to/some/file1";
    QString filename2 = "C:/path/to/some/file2";
    QString directory = "C:/path/to/some";
#else
    QString filename1 = "/path/to/some/file1";
    QString filename2 = "/path/to/some/file2";
    QString directory = "/path/to/some";
#endif

    bool status = repository.accountFile(filename1);
    QCOMPARE(status, true);
    QCOMPARE(repository.getArtworksSourcesCount(), 1);

    bool removeResult = repository.removeFile(filename2, directory);
    repository.cleanupEmptyDirectories();

    QCOMPARE(removeResult, false);
    QCOMPARE(repository.getArtworksSourcesCount(), 1);
}
