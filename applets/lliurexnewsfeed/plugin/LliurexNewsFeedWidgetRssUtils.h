#ifndef PLASMA_LLIUREX_NEWS_FEED_WIDGET_RSS_UTILS_H
#define PLASMA_LLIUREX_NEWS_FEED_WIDGET_RSS_UTILS_H

#include <QObject>
#include <QFile>

using namespace std; 


class LliurexNewsFeedWidgetRssUtils : public QObject
{
    Q_OBJECT


public:
   

   LliurexNewsFeedWidgetRssUtils(QObject *parent = nullptr);

   void fetchRss(const QUrl &url);

signals:
    void rssProcessed (QVariantList result);

private:    
     
    QVariantList parseRss(QIODevice *device);

 
};
#endif // PLASMA_LLIUREX_NEWS_FEED_WIDGET_RSS_UTILS_H
