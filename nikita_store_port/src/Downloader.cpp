#include "Downloader.h"
#include <QUrl>
#include <QNetworkRequest>
#include <QStandardPaths>
#include <QDir>

Downloader::Downloader(QObject *parent) : QObject(parent), manager(new QNetworkAccessManager(this)), currentReply(nullptr), outputFile(nullptr) {}

void Downloader::download(const QString &url) {
    if (currentReply) return;

    QUrl qurl(url);
    QString fileName = qurl.fileName();
    if (fileName.isEmpty()) fileName = "download.zip";

    QString downloadPath = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);
    if (downloadPath.isEmpty()) downloadPath = QDir::tempPath();
    currentFilePath = downloadPath + "/" + fileName;

    outputFile = new QFile(currentFilePath, this);
    if (!outputFile->open(QIODevice::WriteOnly)) {
        emit error("Failed to open file for writing: " + currentFilePath);
        delete outputFile;
        outputFile = nullptr;
        return;
    }

    currentReply = manager->get(QNetworkRequest(qurl));
    connect(currentReply, &QNetworkReply::downloadProgress, this, &Downloader::onDownloadProgress);
    connect(currentReply, &QNetworkReply::readyRead, this, &Downloader::onReadyRead);
    connect(currentReply, &QNetworkReply::finished, this, &Downloader::onReplyFinished);
}

void Downloader::onDownloadProgress(qint64 bytesReceived, qint64 bytesTotal) {
    emit progress(bytesReceived, bytesTotal);
}

void Downloader::onReadyRead() {
    if (outputFile) {
        outputFile->write(currentReply->readAll());
    }
}

void Downloader::onReplyFinished() {
    if (outputFile) {
        outputFile->close();
        delete outputFile;
        outputFile = nullptr;
    }

    if (currentReply->error() == QNetworkReply::NoError) {
        emit finished(currentFilePath);
    } else {
        QFile::remove(currentFilePath);
        emit error(currentReply->errorString());
    }
    currentReply->deleteLater();
    currentReply = nullptr;
}
