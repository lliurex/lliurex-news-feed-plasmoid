#ifndef PLASMA_LLIUREX_NEWS_FEED_WIDGET_UTILS_H
#define PLASMA_LLIUREX_NEWS_FEED_WIDGET_UTILS_H

#include <QObject>
#include <QFile>
#include <QDir>
#include <QVector>

#include "LliurexNewsFeedWidgetRssModel.h"


using namespace std;


class LliurexNewsFeedWidgetUtils : public QObject
{
    Q_OBJECT


public:
   

   LliurexNewsFeedWidgetUtils(QObject *parent = nullptr);

   void cleanCache();
   QString getLastRssUpdate(QString rssUpdatePath);
   void fetchRss(const QUrl &url);
   void updateLastRssPath(QString rssToUpdatePath, QString newDate);

signals:
    void rssProcessed (QVector <LliurexNewsFeedWidgetRssItem> result,QString newUpdateRssDate);

private:    
     
    QString user;
    QString getInstalledVersion();
    void createEnvirontment();
    QVariantList parseRss(QIODevice *device);
    QVector <LliurexNewsFeedWidgetRssItem> setDataForModel(QVariantList rssItems);
    QString parseDate(QString dateToParse,bool isoFormat);
 
};
#endif // PLASMA_LLIUREX_NEWS_FEED_WIDGET_UTILS_H
