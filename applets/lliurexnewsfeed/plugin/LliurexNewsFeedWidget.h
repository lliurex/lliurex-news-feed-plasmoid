#ifndef PLASMA_LLIUREX_NEWS_FEED_WIDGET_H
#define PLASMA_LLIUREX_NEWS_FEED_WIDGET_H

#include <QObject>
#include <QPointer>
#include <KNotification>

#include "LliurexNewsFeedWidgetUtils.h"

class QTimer;
class KNotification;
class LliurexNewsFeedWidgetRssModel;


class LliurexNewsFeedWidget : public QObject
{
    Q_OBJECT


    Q_PROPERTY(TrayStatus status READ status NOTIFY statusChanged)
    Q_PROPERTY(QString toolTip READ toolTip NOTIFY toolTipChanged)
    Q_PROPERTY(QString subToolTip READ subToolTip NOTIFY subToolTipChanged)
    Q_PROPERTY(QString iconName READ iconName NOTIFY iconNameChanged)
    Q_PROPERTY(int currentStackIndex READ currentStackIndex NOTIFY currentStackIndexChanged)
    Q_PROPERTY(QString lastBlogUpdate READ lastBlogUpdate NOTIFY lastBlogUpdateChanged)
    Q_PROPERTY(LliurexNewsFeedWidgetRssModel* rssBlogModel READ rssBlogModel CONSTANT)

    Q_ENUMS(TrayStatus)

public:
    /**
     * System tray icon states.
     */
    enum TrayStatus {
        ActiveStatus=0,
        PassiveStatus,
        HiddenStatus
    };

    LliurexNewsFeedWidget(QObject *parent = nullptr);

    TrayStatus status() const;
    void changeTryIconState (int state);
    void setStatus(TrayStatus status);

    QString toolTip() const;
    void setToolTip(const QString &toolTip);

    QString subToolTip() const;
    void setSubToolTip(const QString &subToolTip);

    QString iconName() const;
    void setIconName(const QString &name);

    int currentStackIndex();
    void setCurrentStackIndex(int);

    QString lastBlogUpdate();
    void setLastBlogUpdate(const QString &lastBlogUpdate);

    LliurexNewsFeedWidgetRssModel *rssBlogModel() const;

public slots:

    void manageNavigation(int stackIndex);
    
signals:
   
    void toolTipChanged();
    void subToolTipChanged();
    void iconNameChanged();
    void statusChanged();
    void currentStackIndexChanged();
    void lastBlogUpdateChanged();

private:

    TrayStatus m_status = PassiveStatus;
    QString m_iconName = QStringLiteral("lliurex-news-feed");
    QString m_toolTip;
    QString m_subToolTip;
    int m_currentStackIndex=0;
    QString m_lastBlogUpdate;
    QString notificationTitle;
    QString notificationBody;
    QString notificationHead;
    QString notificationFoot;
    LliurexNewsFeedWidgetUtils *m_utils;
    QPointer<KNotification> m_notification;
    void plasmoidMode();
    void disableApplet();
    void closeAllNotifications();
    void sendNotification();
    LliurexNewsFeedWidgetRssModel *m_rssBlogModel = nullptr;

private slots:
    
    void processRssModel(QVariantList rssEntries);
};


#endif // PLASMA_LLIUREX_NEWS_FEED_WIDGET_H
