#include "LliurexNewsFeedWidget.h"
#include "LliurexNewsFeedWidgetUtils.h"
#include "LliurexNewsFeedWidgetRssModel.h"

#include <KLocalizedString>
#include <KFormat>
#include <KNotification>
#include <QDebug>
#include <QtCore/QStringList>
#include <QLocale>

LliurexNewsFeedWidget::LliurexNewsFeedWidget(QObject *parent)
    : QObject(parent)
    , m_utils(new LliurexNewsFeedWidgetUtils(this))
    , m_blogRssModel(new LliurexNewsFeedWidgetRssModel(this))


   
{
    m_utils->cleanCache();
    notificationTitle=i18n("LliureX-News-Feed");
    notificationBody=i18n("The are no new posts on the LliureX blog");
    setSubToolTip(notificationBody);
    setIconName("lliurex-news-feed");
    changeTryIconState(2);
    initPlasmoid();

}

void LliurexNewsFeedWidget::initPlasmoid()
{
  
    connect(m_utils,&LliurexNewsFeedWidgetUtils::blogRssProcessed,this,&LliurexNewsFeedWidget::processBlogRssModel);
    m_utils->getBlogRssInfo();
}  

void LliurexNewsFeedWidget::processBlogRssModel(QVector <LliurexNewsFeedWidgetRssItem> rssEntries, bool areNews, bool firstRun){

    if (rssEntries.count()>0){
        m_blogRssModel->clear();
        m_blogRssModel->updateItems(rssEntries);
        if (!firstRun){
            setCanFilterBlogRss(areNews);
        }else{
            setCanFilterBlogRss(false);
        }
        
        changeTryIconState(0);
        if (areNews || firstRun){
            notificationBody=i18n("The are new post on the LliureX blog");
            setSubToolTip(notificationBody);
            setIconName("lliurex-news-feed-updated");
            m_notification=new KNotification(QStringLiteral("Blog"),KNotification::CloseOnTimeout,this);
            m_notification->setComponentName(QStringLiteral("lliurexnewsfeed"));
            m_notification->setTitle(notificationBody);
            m_notification->setText("");
            m_notification->setIconName("lliurex-news-feed-updated");
            m_notification->sendEvent();
        }

    }
    
}


LliurexNewsFeedWidget::TrayStatus LliurexNewsFeedWidget::status() const
{
    return m_status;
}

void LliurexNewsFeedWidget::changeTryIconState(int state){

    if (state==0){
    	setStatus(ActiveStatus);
        setToolTip(notificationTitle);
    }else if (state==1){
        setStatus(PassiveStatus);
    }else if (state==2){
        setStatus(HiddenStatus);
    }

}

void LliurexNewsFeedWidget::manageNavigation(int stackIndex)
{

    setCurrentStackIndex(stackIndex);
}

void LliurexNewsFeedWidget::setStatus(LliurexNewsFeedWidget::TrayStatus status)
{
    if (m_status != status) {
        m_status = status;
        emit statusChanged();
    }
}

QString LliurexNewsFeedWidget::iconName() const
{
    return m_iconName;
}

void LliurexNewsFeedWidget::setIconName(const QString &name)
{
    if (m_iconName != name) {
        m_iconName = name;
        emit iconNameChanged();
    }
}

QString LliurexNewsFeedWidget::toolTip() const
{
    return m_toolTip;
}

void LliurexNewsFeedWidget::setToolTip(const QString &toolTip)
{
    if (m_toolTip != toolTip) {
        m_toolTip = toolTip;
        emit toolTipChanged();
    }
}

QString LliurexNewsFeedWidget::subToolTip() const
{
    return m_subToolTip;
}

void LliurexNewsFeedWidget::setSubToolTip(const QString &subToolTip)
{
    if (m_subToolTip != subToolTip) {
        m_subToolTip = subToolTip;
        emit subToolTipChanged();
    }
}

int LliurexNewsFeedWidget::currentStackIndex()
{

    return m_currentStackIndex;
}

void LliurexNewsFeedWidget::setCurrentStackIndex(int currentStackIndex)
{

    if (m_currentStackIndex!=currentStackIndex){
        m_currentStackIndex=currentStackIndex;
        emit currentStackIndexChanged();
    }
}

bool LliurexNewsFeedWidget::canFilterBlogRss(){

    return m_canFilterBlogRss;
}

void LliurexNewsFeedWidget::setCanFilterBlogRss(bool canFilterBlogRss)
{
    if (m_canFilterBlogRss!=canFilterBlogRss){
        m_canFilterBlogRss=canFilterBlogRss;
        emit canFilterBlogRssChanged();
    }

}

LliurexNewsFeedWidgetRssModel *LliurexNewsFeedWidget::blogRssModel() const
{
    return m_blogRssModel;
}
