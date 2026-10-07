#ifndef PLASMA_LLIUREX_NEWS_FEED_WIDGET_UTILS_H
#define PLASMA_LLIUREX_NEWS_FEED_WIDGET_UTILS_H

#include <QObject>
#include <QFile>
#include <QDir>
#include <QMutex>
#include <QVector>

#include "LliurexNewsFeedWidgetRssModel.h"
#include "LliurexNewsFeedWidgetRssUtils.h"


class LliurexNewsFeedWidgetUtils : public QObject
{
    Q_OBJECT


public:
   

    static LliurexNewsFeedWidgetUtils& instance(){
        static LliurexNewsFeedWidgetUtils _instance;
        return _instance;
    }

    LliurexNewsFeedWidgetUtils(const LliurexNewsFeedWidgetUtils&)=delete;
    LliurexNewsFeedWidgetUtils& operator=(const LliurexNewsFeedWidgetUtils&)=delete;


   void getBlogRssInfo();
 
signals:

   void blogRssProcessed (QVector <LliurexNewsFeedWidgetRssItem> rssEntries,bool anyNews,bool fisrtRun);


private:

    explicit LliurexNewsFeedWidgetUtils();
    ~LliurexNewsFeedWidgetUtils() override;

    bool isLoading=false;    
     
    QString user;
    QString defaultFilterDate="2099-12-31";
    QString lastBlogUpdatePath="/.config/lliurex-news-feed/lastBlogUpdate";
    QString lastBlogRssUpdate;
    QString newUpdateBlogRssDate;
    LliurexNewsFeedWidgetRssUtils *m_blogRss;

    QMutex m_mutex;
    
    QString getLastRssUpdate(QString rssUpdatePath);
    void updateLastRssPath(QString rssToUpdatePath, QString newDate);
    QString parseDate(QString dateToParse,bool isoFormat);
    QVector <LliurexNewsFeedWidgetRssItem> setDataForModel (QVariantList rssEntries);
 
private slots:

    void processBlogRssInfo(QVariantList blogRssEntries);

};
#endif // PLASMA_LLIUREX_NEWS_FEED_WIDGET_UTILS_H
