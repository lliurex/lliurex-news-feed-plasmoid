#include "LliurexNewsFeedWidgetUtils.h"
#include "LliurexNewsFeedWidgetRssModel.h"
#include "LliurexNewsFeedWidgetRssUtils.h"

#include <QFile>
#include <QDebug>
#include <QList>
#include <KLocalizedString>
#include <QDir>
#include <QDate>
#include <QVector>
#include <QUrl>
#include <QtConcurrent>
#include <QMetaType>



LliurexNewsFeedWidgetUtils::LliurexNewsFeedWidgetUtils(QObject *parent)
    : QObject(parent)
    , m_blogRss(new LliurexNewsFeedWidgetRssUtils(this))

       
{
    user=qgetenv("USER");
    qRegisterMetaType<QVector<LliurexNewsFeedWidgetRssItem>>("QVector<LliurexNewsFeedWidgetRssItem>");
    connect(m_blogRss,&LliurexNewsFeedWidgetRssUtils::rssProcessed,this,&LliurexNewsFeedWidgetUtils::processBlogRssInfo);

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

void LliurexNewsFeedWidgetUtils::getBlogRssInfo(){

    QString lang=QLocale::system().name();
    QString blogRss="https://portal.edu.gva.es/blogs/s1/lliurex/es/feed";

    if (lang.startsWith("ca")){
       blogRss="https://portal.edu.gva.es/blogs/s1/lliurex/feed";
    }

    m_blogRss->fetchRss(QUrl::fromUserInput(blogRss));

}

void LliurexNewsFeedWidgetUtils::processBlogRssInfo(QVariantList blogRssEntries){

    QtConcurrent::run([this,blogRssEntries]() {
        QVector <LliurexNewsFeedWidgetRssItem> blogRssModel;
        bool areNews=false;
        bool firstRun=true;
        
        if (blogRssEntries.count()>0){
            lastBlogRssUpdate=getLastRssUpdate(lastBlogUpdatePath);
            QVariantMap lastItem=blogRssEntries.first().toMap();
            QString newUpdateBlogRssDate=lastItem["pubDate"].toString();
            blogRssModel=setDataForModel(blogRssEntries);
            if (!newUpdateBlogRssDate.isEmpty()){
                if (!lastBlogRssUpdate.isEmpty()){
                    firstRun=false;
                    QDate newUpdate=QDate::fromString(newUpdateBlogRssDate,Qt::RFC2822Date);
                    QDate previousDate=QDate::fromString(lastBlogRssUpdate,Qt::RFC2822Date);
                    if (newUpdate>previousDate){
                        areNews=true;
                    }  
                }
                updateLastRssPath(lastBlogUpdatePath,newUpdateBlogRssDate);
            }
        }

        emit blogRssProcessed(blogRssModel,areNews,firstRun);
    });
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

QVector <LliurexNewsFeedWidgetRssItem> LliurexNewsFeedWidgetUtils::setDataForModel(QVariantList rssItems)
{
    QVector<LliurexNewsFeedWidgetRssItem> items;
    if (rssItems.count()>0){
        for (const QVariant &v: rssItems){
            QVariantMap rssEntry=v.toMap();
            LliurexNewsFeedWidgetRssItem item;
            bool isNew=false;
            if (!lastBlogRssUpdate.isEmpty()){
                QDate itemDate=QDate::fromString(rssEntry["pubDate"].toString(),Qt::RFC2822Date);
                QDate previousDate=QDate::fromString(lastBlogRssUpdate,Qt::RFC2822Date);
                if (itemDate>previousDate){
                    isNew=true;
                }
            }else{
                isNew=true;
            }
            QString parsedDate=parseDate(rssEntry["pubDate"].toString(),false);
            item.setTitle(parsedDate+" - "+rssEntry["title"].toString().simplified());
            item.setLink(rssEntry["link"].toString());
            item.setIsNew(isNew);
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
    }
    if (lastRssUpdateFile.open(QIODevice::WriteOnly)){
        QTextStream data(&lastRssUpdateFile);
        data<<newDate;
        lastRssUpdateFile.close();
    }

}


