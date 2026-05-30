#include "MainWindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), model(new AppModel(this)), downloader(new Downloader(this))
{
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);

    appList = new QListWidget(this);
    descriptionLabel = new QLabel("Select an app to see details", this);
    descriptionLabel->setWordWrap(true);
    installButton = new QPushButton("Install", this);
    installButton->setEnabled(false);
    progressBar = new QProgressBar(this);
    progressBar->setVisible(false);

    layout->addWidget(new QLabel("Nikita Store (C++/Qt Port)"));
    layout->addWidget(appList);
    layout->addWidget(descriptionLabel);
    layout->addWidget(installButton);
    layout->addWidget(progressBar);

    setCentralWidget(centralWidget);
    setWindowTitle("Nikita Store");
    resize(400, 500);

    connect(appList, &QListWidget::itemClicked, this, &MainWindow::onAppSelected);
    connect(installButton, &QPushButton::clicked, this, &MainWindow::onInstallClicked);
    connect(model, &AppModel::dataLoaded, this, &MainWindow::onDataLoaded);
    connect(downloader, &Downloader::progress, this, &MainWindow::onDownloadProgress);
    connect(downloader, &Downloader::finished, this, &MainWindow::onDownloadFinished);

    model->loadData("https://raw.githubusercontent.com/Nikita534cool/ssssssssssssssssssss/refs/heads/main/sss.json");
}

MainWindow::~MainWindow() {}

void MainWindow::onDataLoaded(const QList<AppInfo> &apps)
{
    appList->clear();
    for (const auto &app : apps) {
        QListWidgetItem *item = new QListWidgetItem(app.name);
        item->setData(Qt::UserRole, QVariant::fromValue(app.downloadLink));
        item->setData(Qt::UserRole + 1, app.description);
        appList->addItem(item);
    }
}

void MainWindow::onAppSelected(QListWidgetItem *item)
{
    currentApp.name = item->text();
    currentApp.downloadLink = item->data(Qt::UserRole).toString();
    currentApp.description = item->data(Qt::UserRole + 1).toString();

    descriptionLabel->setText(currentApp.name + "\n\n" + currentApp.description);
    installButton->setEnabled(true);
}

void MainWindow::onInstallClicked()
{
    installButton->setEnabled(false);
    progressBar->setVisible(true);
    progressBar->setValue(0);
    downloader->download(currentApp.downloadLink);
}

void MainWindow::onDownloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    if (bytesTotal > 0) {
        progressBar->setMaximum(100);
        progressBar->setValue(static_cast<int>(bytesReceived * 100 / bytesTotal));
    }
}

void MainWindow::onDownloadFinished(const QString &filePath)
{
    progressBar->setVisible(false);
    installButton->setEnabled(true);
    QMessageBox::information(this, "Success", "Download finished: " + filePath + "\n(Extraction not implemented in this demo)");
}
