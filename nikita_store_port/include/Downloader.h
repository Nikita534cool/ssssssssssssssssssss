#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QFile>

class Downloader : public QObject {
    Q_OBJECT
public:
    explicit Downloader(QObject *parent = nullptr);
    void download(const QString &url);

signals:
    void progress(qint64 bytesReceived, qint64 bytesTotal);
    void finished(const QString &filePath);
    void error(const QString &error);

private slots:
    void onDownloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void onReadyRead();
    void onReplyFinished();

private:
    QNetworkAccessManager *manager;
    QNetworkReply *currentReply;
    QFile *outputFile;
    QString currentFilePath;
};

#endif // DOWNLOADER_H
