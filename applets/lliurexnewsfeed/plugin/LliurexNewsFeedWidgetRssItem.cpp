#include "LliurexNewsFeedWidgetRssItem.h"

#include <QDebug>

LliurexNewsFeedWidgetRssItem::LliurexNewsFeedWidgetRssItem()
    : m_title()
    , m_link()
    , m_pubDate()

{
}

QString LliurexNewsFeedWidgetRssItem::title() const
{
    return m_title;
}

void LliurexNewsFeedWidgetRssItem::setTitle(const QString &title)
{
    m_title = title;
}

QString LliurexNewsFeedWidgetRssItem::link() const
{
    return m_link;
}

void LliurexNewsFeedWidgetRssItem::setLink(const QString &link)
{
    m_link = link;
}

QString LliurexNewsFeedWidgetRssItem::pubDate() const
{
    return m_pubDate;
}

void LliurexNewsFeedWidgetRssItem::setPubDate(const QString &pubDate)
{
    m_pubDate = pubDate;
}
bool LliurexNewsFeedWidgetRssItem::operator==(const LliurexNewsFeedWidgetRssItem &other) const
{
    return m_title==other.m_title
        && m_link == other.m_link
        && m_pubDate == other.m_pubDate;
   
}
bool LliurexNewsFeedWidgetRssItem::operator!=(const LliurexNewsFeedWidgetRssItem &other) const
{
    return ! (*this == other);
}
