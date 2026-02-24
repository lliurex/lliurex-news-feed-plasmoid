#ifndef LLIUREX_NEWS_FEED_PLUGIN_H
#define LLIUREX_NEWS_FEED_PLUGIN_H

#include <QQmlEngine>
#include <QQmlExtensionPlugin>

class LliurexNewsFeedPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
/*    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface")*/
    Q_PLUGIN_METADATA(IID QQmlExtensionInterface_iid)

public:
    void registerTypes(const char *uri) Q_DECL_OVERRIDE;
};

#endif // LLIUREX_NEWS_FEED_PLUGIN_H
