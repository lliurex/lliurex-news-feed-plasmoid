#include "LliurexNewsFeedWidgetUtils.h"

#include <QFile>
#include <QDebug>
#include <QList>
#include <KLocalizedString>
#include <QDebug>
#include <QDir>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QXmlStreamReader>

#include <tuple>
#include <sys/types.h>
#include <iostream>


LliurexNewsFeedWidgetUtils::LliurexNewsFeedWidgetUtils(QObject *parent)
    : QObject(parent)
       
{
    user=qgetenv("USER");
    createEnvirontment();
}

void LliurexNewsFeedWidgetUtils::createEnvirontment(){

    qDebug()<<"CREANDO ENTORNO";
    QString newsFeedPath="/home/"+user+"/.config/lliurex-news-feed";
    qDebug()<<newsFeedPath;
    QDir newsFeedDir;
    if (!newsFeedDir.exists(newsFeedPath)){
        qDebug()<<"CREANDO CARPETA";
        newsFeedDir.mkdir(newsFeedPath);
    }

}
void LliurexNewsFeedWidgetUtils::cleanCache(){

    qDebug()<<"[LLIUREX-NEWS-FEED]: Clean cache";
    QFile CURRENT_VERSION_TOKEN;
    QDir cacheDir("/home/"+user+"/.cache/plasmashell/qmlcache");
    QString currentVersion="";
    bool clear=false;

    CURRENT_VERSION_TOKEN.setFileName("/home/"+user+"/.config/lliurex-news-feed-widget.conf");
    QString installedVersion=getInstalledVersion();

    if (!CURRENT_VERSION_TOKEN.exists()){
        if (CURRENT_VERSION_TOKEN.open(QIODevice::WriteOnly)){
            QTextStream data(&CURRENT_VERSION_TOKEN);
            data<<installedVersion;
            CURRENT_VERSION_TOKEN.close();
            clear=true;
        }
    }else{
        if (CURRENT_VERSION_TOKEN.open(QIODevice::ReadOnly)){
            QTextStream content(&CURRENT_VERSION_TOKEN);
            currentVersion=content.readLine();
            CURRENT_VERSION_TOKEN.close();
        }

        if (currentVersion!=installedVersion){
            if (CURRENT_VERSION_TOKEN.open(QIODevice::WriteOnly)){
                QTextStream data(&CURRENT_VERSION_TOKEN);
                data<<installedVersion;
                CURRENT_VERSION_TOKEN.close();
                clear=true;
            }
        }
    } 
    if (clear){
        if (cacheDir.exists()){
            cacheDir.removeRecursively();
        }
    }   

}

QString LliurexNewsFeedWidgetUtils::getInstalledVersion(){

    QFile INSTALLED_VERSION_TOKEN;
    QString installedVersion="";
    
    INSTALLED_VERSION_TOKEN.setFileName("/var/lib/lliurex-news-feed-plasmoid/version");

    if (INSTALLED_VERSION_TOKEN.exists()){
        if (INSTALLED_VERSION_TOKEN.open(QIODevice::ReadOnly)){
            QTextStream content(&INSTALLED_VERSION_TOKEN);
            installedVersion=content.readLine();
            INSTALLED_VERSION_TOKEN.close();
        }
    }
    return installedVersion;

}  

QString LliurexNewsFeedWidgetUtils::getLastBlogUpdate(){

    qDebug()<<"[LLIUREX-NEWS-FEED]: Check for last blog update";

    QFile lastBlogUpdateFile("/home/"+user+lastBlogUpdate);
    QString lastBlogUpdate="2025-01-01";
    if (lastBlogUpdateFile.exists()){
        if (lastBlogUpdateFile.open(QIODevice::ReadOnly)){
            QTextStream content(&lastBlogUpdateFile);
            while (!content.atEnd()){
                QString tmpLine=content.readLine().remove('\n');
                if (!tmpLine.isEmpty()){
                    lastBlogUpdate=tmpLine;
                }
            }
            
            lastBlogUpdateFile.close();
        }
    }
    return lastBlogUpdate;

}

void LliurexNewsFeedWidgetUtils::fetchRss(const QUrl &url) {
    qDebug()<<"[LLIUREX-NEWS-FEED]:Downloading RSS"<<url;
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QNetworkRequest request(url);
    request.setAttribute(QNetworkRequest::FollowRedirectsAttribute,true);
    request.setHeader(QNetworkRequest::UserAgentHeader, "Mozilla/5.0");

    QNetworkReply *reply = manager->get(request);

    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        QVariantList result;
        if (reply->error() == QNetworkReply::NoError) {
            qDebug()<<"[LLIUREX-NEWS-FEED]: status"<<reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
            QVariantList result = parseRss(reply);
            emit rssProcessed(result);
        }else{
            std::clog<<"[LLIUREX-NEWS-FEED]: Elements of list to return "<<result.count()<<std::endl;
            emit rssProcessed(result);
        }
        reply->deleteLater();
    });
}

QVariantList LliurexNewsFeedWidgetUtils::parseRss(QIODevice *device) {
    QVariantList rssItems;
    QXmlStreamReader reader(device); 

    qDebug()<<"[LLIUREX-NEWS-FEED]: Parsing rss";
    while (!reader.atEnd() && !reader.hasError()) {
        if (reader.readNextStartElement() && reader.name() == "item") {
            QVariantMap itemData;
            while (reader.readNextStartElement()) {
                if ((reader.name()=="title") || (reader.name()=="link") || (reader.name()=="pubDate")) {
                    itemData.insert(reader.name().toString(), reader.readElementText());
                }else{
                    reader.skipCurrentElement();
                }
            }
            rssItems.append(itemData);
        }
    }
    std::clog<<"[LLIUREX-NEWS-FEED]: Elements of list "<<rssItems.count()<<std::endl;
    return rssItems;
}
