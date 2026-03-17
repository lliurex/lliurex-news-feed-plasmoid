#ifndef LLIUREX_NEWS_FEED_PLUGIN_H
#define LLIUREX_NEWS_FEED_PLUGIN_H

#include <QQmlEngineExtensionPlugin>

class  LliurexNewsFeedPlugin : public QQmlEngineExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID QQmlEngineExtensionInterface_iid)

public:
    using QQmlEngineExtensionPlugin::QQmlEngineExtensionPlugin;
};

#endif // LLIUREX_NEWS_FEED_PLUGIN_H

