#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include <QPushButton>
#include <QProgressBar>
#include <QLabel>
#include <QVBoxLayout>
#include "AppModel.h"
#include "Downloader.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onAppSelected(QListWidgetItem *item);
    void onInstallClicked();
    void onDownloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void onDownloadFinished(const QString &filePath);
    void onDataLoaded(const QList<AppInfo> &apps);

private:
    QListWidget *appList;
    QLabel *descriptionLabel;
    QPushButton *installButton;
    QProgressBar *progressBar;

    AppModel *model;
    Downloader *downloader;
    AppInfo currentApp;
};

#endif // MAINWINDOW_H
