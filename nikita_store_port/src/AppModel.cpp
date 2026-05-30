#include "AppModel.h"
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

AppModel::AppModel(QObject *parent) : QObject(parent), manager(new QNetworkAccessManager(this)) {}

void AppModel::loadData(const QString &url) {
    QNetworkReply *reply = manager->get(QNetworkRequest(QUrl(url)));
    connect(reply, &QNetworkReply::finished, this, &AppModel::onReplyFinished);
}

void AppModel::onReplyFinished() {
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if (reply->error() == QNetworkReply::NoError) {
        QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
        QJsonObject root = doc.object();
        QJsonObject appsObj = root["apps"].toObject();

        QList<AppInfo> apps;
        for (auto it = appsObj.begin(); it != appsObj.end(); ++it) {
            QJsonObject app = it.value().toObject();
            AppInfo info;
            info.name = app["app-name"].toString();
            info.description = app["description"].toString();
            info.downloadLink = app["app-download-link"].toString();
            info.version = app["version"].toString();
            info.category = app["category"].toString();
            apps.append(info);
        }
        emit dataLoaded(apps);
    } else {
        emit errorOccurred(reply->errorString());
    }
    reply->deleteLater();
}
