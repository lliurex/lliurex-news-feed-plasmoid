#include "LliurexNewsFeedWidgetRssUtils.h"

#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QXmlStreamReader>


LliurexNewsFeedWidgetRssUtils::LliurexNewsFeedWidgetRssUtils(QObject *parent)
    : QObject(parent)
       
{

}

void LliurexNewsFeedWidgetRssUtils::fetchRss(const QUrl &url) {
    
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QNetworkRequest request(url);
    request.setAttribute(QNetworkRequest::FollowRedirectsAttribute,true);
    request.setHeader(QNetworkRequest::UserAgentHeader, "Mozilla/5.0");

    QNetworkReply *reply = manager->get(request);

    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        QVariantList result;

        if (reply->error() == QNetworkReply::NoError) {
            qDebug()<<"[LLIUREX-NEWS-FEED]: status"<<reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
            result = parseRss(reply);
            if (result.count()>0){
                qDebug()<<"[LLIUREX-NEWS-FEED]: get "<<result.count()<<" entries from rss";
                emit rssProcessed(result);

            }
        }else{
            emit rssProcessed(result);
        }
        reply->deleteLater();
    });
}

QVariantList LliurexNewsFeedWidgetRssUtils::parseRss(QIODevice *device) {
    
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