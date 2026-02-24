
#include "LliurexNewsFeedWidgetRssModel.h"

#include <QDebug>

LliurexNewsFeedWidgetRssModel::LliurexNewsFeedWidgetRssModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

namespace {
    /**
     * QML data roles.
     */
    enum {
        TitleRole = Qt::UserRole,
        LinkRole,
        PubDateRole
      };
}

QHash<int, QByteArray> LliurexNewsFeedWidgetRssModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[TitleRole] = "title";
    roles[LinkRole] = "link";
    roles[PubDateRole] = "pubDate";
 
    return roles;
}

QVariant LliurexNewsFeedWidgetRssModel::data(const QModelIndex &index, int role) const
{
    if (! index.isValid() || index.row() >= m_items.size()) {
        return QVariant();
    }

    const auto item = m_items[index.row()];

    switch (role) {
        case TitleRole: return item.title();
        case LinkRole: return item.link();
        case PubDateRole: return item.pubDate();
    }

    return QVariant();
}

int LliurexNewsFeedWidgetRssModel::rowCount(const QModelIndex &index) const
{
    if (! index.isValid()) {
        return m_items.size();
    }

    return 0;
}

bool LliurexNewsFeedWidgetRssModel::setData(const QModelIndex &index, const QVariant &variant, int role)
{
    Q_UNUSED(role)

    const int row = index.row();
    if (index.isValid() && row < m_items.size()) {
        const LliurexNewsFeedWidgetRssItem item = variant.value<LliurexNewsFeedWidgetRssItem>();

        // This assert makes sure that changing items modify the correct item:
        // therefore, the unique identifier 'filePath()' is used. If that
        // is not the case, the newly inserted row must have an empty filePath().
        Q_ASSERT(item.title() == m_items[row].title()
            || m_items[row].title().isEmpty());

        if (m_items[row] != item) {
            m_items[row] = item;
            emit dataChanged(index, index);
            return true;
        }
    }

    return false;
}

bool LliurexNewsFeedWidgetRssModel::insertRows(int row, int count, const QModelIndex &parent)
{
    // only top-level items are supported
    if (parent.isValid()) {
        return false;
    }

    beginInsertRows(QModelIndex(), row, row + count - 1);
    m_items.insert(row, count, LliurexNewsFeedWidgetRssItem());
    endInsertRows();

    return true;
}

bool LliurexNewsFeedWidgetRssModel::removeRows(int row, int count, const QModelIndex &parent)
{
    // only top-level items are valid
    if (parent.isValid() || (row + count) >= m_items.size()) {
        return false;
    }

    beginRemoveRows(QModelIndex(), row, row + count - 1);
    m_items.remove(row, count);
    endRemoveRows();

    return true;
}

void LliurexNewsFeedWidgetRssModel::clear()
{
    beginResetModel();
    m_items.clear();
    endResetModel();
}

namespace {
    QStringList rssTitle(const QVector<LliurexNewsFeedWidgetRssItem> &items)
    {
        QStringList list;
        for (auto & item : items) {
            list.append(item.title());
        }
        return list;
    }

    int indexOfRssTitle(const QString &title, const QVector<LliurexNewsFeedWidgetRssItem> &items)
    {
        for (int i = 0; i < items.size(); ++i) {
            if (title == items[i].title()) {
                return i;
            }
        }
        return -1;
    }
}

void LliurexNewsFeedWidgetRssModel::updateItems(const QVector<LliurexNewsFeedWidgetRssItem> &items)
{
    QStringList unusedRssTitles = rssTitle(m_items);
    // merge existing and new file paths
    for (auto & item : items) {
        // remove still used item from unused list
        unusedRssTitles.removeOne(item.title());
        // insert or modify m_items
        int row = indexOfRssTitle(item.title(), m_items);
        if (row < 0) {
            // new item: append on end
            row = m_items.size();
            insertRow(row);
        }
        setData(createIndex(row, 0), QVariant::fromValue(item));
    }

    // remove mount points, that do not exist anymore

    for (const auto & title : unusedRssTitles) {
        const int row = indexOfRssTitle(title, m_items);
        Q_ASSERT(row >= 0);
        removeRow(row);
    
    }
}
