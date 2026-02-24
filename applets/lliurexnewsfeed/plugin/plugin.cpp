#include "plugin.h"
#include "LliurexNewsFeedWidget.h"
#include "LliurexNewsFeedWidgetUtils.h"
#include "LliurexNewsFeedWidgetRssModel.h"

#include <QtQml>

void LliurexNewsFeedPlugin::registerTypes (const char *uri)
{
    Q_ASSERT(uri == QLatin1String("org.kde.plasma.private.lliurexnewsfeed"));
    qmlRegisterType<LliurexNewsFeedWidget>(uri, 1, 0, "LliurexNewsFeedWidget");
    qmlRegisterType<LliurexNewsFeedWidgetUtils>(uri, 1, 0, "LliurexNewsFeedWidgetUtils");
    qmlRegisterType<LliurexNewsFeedWidgetRssModel>(uri, 1, 0, "LliurexNewsFeedWidgetRssModel");

}
