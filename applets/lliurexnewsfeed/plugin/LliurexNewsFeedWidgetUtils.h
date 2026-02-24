#ifndef PLASMA_LLIUREX_NEWS_FEED_WIDGET_UTILS_H
#define PLASMA_LLIUREX_NEWS_FEED_WIDGET_UTILS_H

#include <QObject>
#include <QFile>
#include <QDir>

using namespace std;


class LliurexNewsFeedWidgetUtils : public QObject
{
    Q_OBJECT


public:
   

   LliurexNewsFeedWidgetUtils(QObject *parent = nullptr);

   void cleanCache();
   QString getLastBlogUpdate();
   void fetchRss(const QUrl &url);

signals:
    void rssProcessed (QVariantList result);

private:    
     
    QString user;
    QString lastBlogUpdate="/.config/lliurex-news-feed/lastBlogUpdate";

    QString getInstalledVersion();
    void createEnvirontment();
    QVariantList parseRss(QIODevice *device);

 
};
#endif // PLASMA_LLIUREX_NEWS_FEED_WIDGET_UTILS_H
