#include "LliurexNewsFeedWidgetUtils.h"
#include "LliurexNewsFeedWidgetRssModel.h"

#include <QFile>
#include <QDebug>
#include <QList>
#include <KLocalizedString>
#include <QDebug>
#include <QDir>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QXmlStreamReader>
#include <QDate>
#include <QVector>

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

    QString newsFeedPath="/home/"+user+"/.config/lliurex-news-feed";
    QDir newsFeedDir;
    if (!newsFeedDir.exists(newsFeedPath)){
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

QString LliurexNewsFeedWidgetUtils::getLastRssUpdate(QString rssPath){

    QString lastRssUpdate;
    
    QFile lastRssUpdateFile("/home/"+user+rssPath);
    if (lastRssUpdateFile.exists()){
        if (lastRssUpdateFile.open(QIODevice::ReadOnly)){
            QTextStream content(&lastRssUpdateFile);
            while (!content.atEnd()){
                QString tmpLine=content.readLine().remove('\n');
                if (!tmpLine.isEmpty()){
                    lastRssUpdate=tmpLine;
                }
            }
            lastRssUpdateFile.close();
        }
    }
    return lastRssUpdate;
}

void LliurexNewsFeedWidgetUtils::fetchRss(const QUrl &url) {
    
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QNetworkRequest request(url);
    request.setAttribute(QNetworkRequest::FollowRedirectsAttribute,true);
    request.setHeader(QNetworkRequest::UserAgentHeader, "Mozilla/5.0");

    QNetworkReply *reply = manager->get(request);

    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        QVariantList result;
        QVector <LliurexNewsFeedWidgetRssItem> rssModel;
        QString newUpdateRssDate;
        if (reply->error() == QNetworkReply::NoError) {
            qDebug()<<"[LLIUREX-NEWS-FEED]: status"<<reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
            result = parseRss(reply);
            if (result.count()>0){
                qDebug()<<"[LLIUREX-NEWS-FEED]: get "<<result.count()<<" entries from rss";
                QVariantMap lastItem=result.first().toMap();
                QString newUpdateRssDate=lastItem["pubDate"].toString();
                rssModel=setDataForModel(result);
                emit rssProcessed(rssModel,newUpdateRssDate);

            }else{
                emit rssProcessed(rssModel,newUpdateRssDate);
            }
        }else{
            emit rssProcessed(rssModel,newUpdateRssDate);
        }
        reply->deleteLater();
    });
}

QVariantList LliurexNewsFeedWidgetUtils::parseRss(QIODevice *device) {
    
    QVariantList rssItems;
    QXmlStreamReader reader(device); 

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
    return rssItems;
   
}

QVector <LliurexNewsFeedWidgetRssItem> LliurexNewsFeedWidgetUtils::setDataForModel(QVariantList rssItems)
{
    QVector<LliurexNewsFeedWidgetRssItem> items;
    if (rssItems.count()>0){
        for (const QVariant &v: rssItems){
            QVariantMap rssEntry=v.toMap();
            LliurexNewsFeedWidgetRssItem item;
            QString parsedDate=parseDate(rssEntry["pubDate"].toString(),false);
            item.setTitle(parsedDate+" - "+rssEntry["title"].toString());
            item.setLink(rssEntry["link"].toString());
            item.setPubDate(rssEntry["pubDate"].toString());
            items.append(item);
        }
    }

    return items;
}

QString LliurexNewsFeedWidgetUtils::parseDate(QString dateToParse,bool isoFormat)
{
    QString parsedDate;
    QDateTime tmpDateTime=QDateTime::fromString(dateToParse,Qt::RFC2822Date);
    if (tmpDateTime.isValid()){
        QDate tmpDate=tmpDateTime.date();
        if (isoFormat){
            parsedDate=tmpDate.toString("yyyy-MM-dd");
        }else{
            parsedDate=tmpDate.toString("dd/MM/yyyy");
        }
    }else{
        parsedDate=dateToParse;
    }

    return parsedDate; 
}

void LliurexNewsFeedWidgetUtils::updateLastRssPath(QString rssPath, QString newDate)
{

    QFile lastRssUpdateFile("/home/"+user+rssPath);
    QString newsFeedPath="/home/"+user+"/.config/lliurex-news-feed";
    QDir newsFeedDir;
    if (!newsFeedDir.exists(newsFeedPath)){
        newsFeedDir.mkdir(newsFeedPath);
    }else{
        if (lastRssUpdateFile.open(QIODevice::WriteOnly)){
            QTextStream data(&lastRssUpdateFile);
                data<<newDate;
                lastRssUpdateFile.close();
        }
    }
}


