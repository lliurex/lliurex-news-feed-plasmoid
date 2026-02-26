#include "LliurexNewsFeedWidgetRssItem.h"

#include <QDebug>

LliurexNewsFeedWidgetRssItem::LliurexNewsFeedWidgetRssItem()
    : m_title()
    , m_link()
    , m_isNew()

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

bool LliurexNewsFeedWidgetRssItem::isNew() const
{
    return m_isNew;
}

void LliurexNewsFeedWidgetRssItem::setIsNew(const bool &isNew)
{
    m_isNew = isNew;
}
bool LliurexNewsFeedWidgetRssItem::operator==(const LliurexNewsFeedWidgetRssItem &other) const
{
    return m_title==other.m_title
        && m_link == other.m_link
        && m_isNew == other.m_isNew;
   
}
bool LliurexNewsFeedWidgetRssItem::operator!=(const LliurexNewsFeedWidgetRssItem &other) const
{
    return ! (*this == other);
}
