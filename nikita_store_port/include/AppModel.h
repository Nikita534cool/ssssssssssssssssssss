#ifndef APPMODEL_H
#define APPMODEL_H

#include <QString>
#include <QList>
#include <QObject>
#include <QNetworkAccessManager>

struct AppInfo {
    QString name;
    QString description;
    QString downloadLink;
    QString version;
    QString category;
};

class AppModel : public QObject {
    Q_OBJECT
public:
    explicit AppModel(QObject *parent = nullptr);
    void loadData(const QString &url);

signals:
    void dataLoaded(const QList<AppInfo> &apps);
    void errorOccurred(const QString &error);

private slots:
    void onReplyFinished();

private:
    QNetworkAccessManager *manager;
};

#endif // APPMODEL_H
