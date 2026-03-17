#include "plugin.h"
#include "LliurexNewsFeedWidget.h"


#include <QtQml>

void LliurexNewsFeedPlugin::registerTypes (const char *uri)
{
    Q_ASSERT(uri == QLatin1String("org.kde.plasma.private.lliurexnewsfeed"));
    qmlRegisterType<LliurexNewsFeedWidget>(uri, 2, 0, "LliurexNewsFeedWidget");

}
