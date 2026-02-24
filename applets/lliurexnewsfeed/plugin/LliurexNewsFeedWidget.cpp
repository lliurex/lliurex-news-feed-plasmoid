#include "LliurexNewsFeedWidget.h"
#include "LliurexNewsFeedWidgetUtils.h"
#include "LliurexNewsFeedWidgetRssModel.h"

#include <KLocalizedString>
#include <KFormat>
#include <KNotification>
#include <QDebug>
#include <QtCore/QStringList>

LliurexNewsFeedWidget::LliurexNewsFeedWidget(QObject *parent)
    : QObject(parent)
    , m_utils(new LliurexNewsFeedWidgetUtils(this))
    , m_rssBlogModel(new LliurexNewsFeedWidgetRssModel(this))


   
{
    m_utils->cleanCache();
    setLastBlogUpdate(m_utils->getLastBlogUpdate());
    notificationTitle=i18n("LliureX-News-Feed");
    notificationBody=i18n("Detected news");
    setSubToolTip(notificationBody);
    plasmoidMode();

}  

void LliurexNewsFeedWidget::plasmoidMode(){

    QString blogRss="https://portal.edu.gva.es/blogs/s1/lliurex/feed";
    m_utils->fetchRss(QUrl::fromUserInput(blogRss));
    connect(m_utils,&LliurexNewsFeedWidgetUtils::rssProcessed,this,&LliurexNewsFeedWidget::processRssModel);
    changeTryIconState(1);
    
}

void LliurexNewsFeedWidget::processRssModel(QVariantList rssEntries){

    qDebug()<<"[LLIUREX-NEWS-FEED]:TERMINADO";
    if (rssEntries.count()>0){
        m_rssBlogModel->clear();
        QVector<LliurexNewsFeedWidgetRssItem> items;
        for (const QVariant &v: rssEntries){
            QVariantMap rssEntry=v.toMap();
            LliurexNewsFeedWidgetRssItem item;
            item.setTitle(rssEntry["title"].toString());
            item.setLink(rssEntry["link"].toString());
            item.setPubDate(rssEntry["pubDate"].toString());
            items.append(item);
            
        }
        m_rssBlogModel->updateItems(items);
        changeTryIconState(0);
    }
    
}

void LliurexNewsFeedWidget::closeAllNotifications(){

    qDebug()<<"CLOSE NOTIFICATIONS";
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

QString LliurexNewsFeedWidget::lastBlogUpdate()
{

    return m_lastBlogUpdate;
}

void LliurexNewsFeedWidget::setLastBlogUpdate(const QString &lastBlogUpdate)
{

    if (m_lastBlogUpdate!=lastBlogUpdate){
        m_lastBlogUpdate=lastBlogUpdate;
        emit lastBlogUpdateChanged();
    }
}

LliurexNewsFeedWidgetRssModel *LliurexNewsFeedWidget::rssBlogModel() const
{
    return m_rssBlogModel;
}
