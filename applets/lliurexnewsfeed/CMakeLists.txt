
#######################################################################################
# Package
plasma_install_package(package org.kde.plasma.lliurexnewsfeed)


#######################################################################################
# Notes Library
set(PLUGIN_TARGET "lliurexnewsfeedplugin")
add_definitions(-DTRANSLATION_DOMAIN="plasma_applet_org.kde.plasma.lliurexnewsfeed")
ecm_add_qml_module(${PLUGIN_TARGET} URI org.kde.plasma.private.lliurexnewsfeed VERSION 2.0)

target_sources(${PLUGIN_TARGET} PRIVATE
    plugin/plugin.cpp
    plugin/LliurexNewsFeedWidget.cpp
    plugin/LliurexNewsFeedWidgetUtils.cpp
    plugin/LliurexNewsFeedWidgetRssUtils.cpp
    plugin/LliurexNewsFeedWidgetRssModel.cpp
    plugin/LliurexNewsFeedWidgetRssItem.cpp
    plugin/plugin.h
    plugin/LliurexNewsFeedWidget.h
    plugin/LliurexNewsFeedWidgetUtils.h
    plugin/LliurexNewsFeedWidgetRssUtils.h
    plugin/LliurexNewsFeedWidgetRssModel.h
    plugin/LliurexNewsFeedWidgetRssItem.h
   
)

target_link_libraries(${PLUGIN_TARGET} PRIVATE
                      Qt6::Quick
                      Qt6::Core
                      Qt6::Widgets
                      KF6::CoreAddons
                      KF6::I18n
                      KF6::Notifications
                      KF6::KIOWidgets
                      )

target_include_directories(${PLUGIN_TARGET} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/plugin)
ecm_finalize_qml_module(${PLUGIN_TARGET})

install(TARGETS ${PLUGIN_TARGET} DESTINATION ${KDE_INSTALL_TARGETS_DEFAULT_ARGS} LIBRARY DESTINATION ${KDE_INSTALL_QMLDIR}/org/kde/plasma/private/lliurexnewsfeed)
install(FILES "icons/lliurex-news-feed.svg"
        DESTINATION ${KDE_INSTALL_ICONDIR}/hicolor/scalable/apps/)
install(FILES "icons/lliurex-news-feed-updated.svg"
        DESTINATION ${KDE_INSTALL_ICONDIR}/hicolor/scalable/apps/)
install(FILES "icons/lliurex-rss-new-post.svg"
        DESTINATION ${KDE_INSTALL_ICONDIR}/hicolor/scalable/apps/)
install(FILES "knotifications6/lliurexnewsfeed.notifyrc"
        DESTINATION ${KDE_INSTALL_KNOTIFYRCDIR})
