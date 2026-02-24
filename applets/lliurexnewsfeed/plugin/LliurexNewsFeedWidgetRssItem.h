#ifndef PLASMA_LLIUREX_NEWS_FEED_WIDGET_RSS_ITEM_H
#define PLASMA_LLIUREX_NEWS_FEED_WIDGET_RSS_ITEM_H

#include <QString>
#include <QMetaType>

/**
 * Class that holds all space info.
 */
class LliurexNewsFeedWidgetRssItem
{
public:
    LliurexNewsFeedWidgetRssItem();

    QString title() const;
    void setTitle(const QString &title);

    QString name() const;
    void setName(const QString &name);

    QString link() const;
    void setLink(const QString &link);

    QString pubDate() const;
    void setPubDate(const QString &pubDate);

    bool operator==(const LliurexNewsFeedWidgetRssItem &other) const;
    bool operator!=(const LliurexNewsFeedWidgetRssItem &other) const;

private:
    QString m_title;
    QString m_link;
    QString m_pubDate;
};

Q_DECLARE_METATYPE(LliurexNewsFeedWidgetRssItem)

#endif // PLASMA_LLIUREX_NEWS_FEED_RSS_WIDGET_ITEM_H
