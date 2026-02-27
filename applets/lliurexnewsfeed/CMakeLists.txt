
#######################################################################################
# Package
plasma_install_package(package org.kde.plasma.lliurexnewsfeed)


#######################################################################################
# Notes Library
add_definitions(-DTRANSLATION_DOMAIN="plasma_applet_org.kde.plasma.lliurexnewsfeed")
ecm_add_qml_module(lliurexnewsfeedplugin URI org.kde.plasma.private.lliurexnewsfeed)

target_sources(lliurexnewsfeedplugin PRIVATE
    plugin/plugin.cpp
    plugin/LliurexNewsFeedWidget.cpp
    plugin/LliurexNewsFeedWidgetUtils.cpp
    plugin/LliurexNewsFeedWidgetRssUtils.cpp
    plugin/LliurexNewsFeedWidgetRssModel.cpp
    plugin/LliurexNewsFeedWidgetRssItem.cpp
   
)

target_link_libraries(lliurexnewsfeedplugin PRIVATE
                      Qt6::Quick
                      Qt6::Core
                      Qt6::Widgets
                      KF6::CoreAddons
                      KF6::I18n
                      KF6::Notifications
                      KF6::KIOWidgets
                      )
ecm_finalize_qml_module(lliurexnewsfeedplugin)

install(FILES plugin/qmldir DESTINATION ${QML_INSTALL_DIR}/org/kde/plasma/private/lliurexnewsfeed)
install(TARGETS lliurexnewsfeedplugin DESTINATION ${QML_INSTALL_DIR}/org/kde/plasma/private/lliurexnewsfeed)
install(FILES "icons/lliurex-news-feed.svg"
        DESTINATION ${DATAROOTDIR}/usr/share/icons/hicolor/scalable/apps/)
install(FILES "icons/lliurex-news-feed-updated.svg"
        DESTINATION ${DATAROOTDIR}/usr/share/icons/hicolor/scalable/apps/)
install(FILES "icons/lliurex-rss-new-post.svg"
        DESTINATION ${DATAROOTDIR}/usr/share/icons/hicolor/scalable/apps/)
install(FILES "knotifications6/lliurexnewsfeed.notifyrc"
       DESTINATION ${DATAROOTDIR}/usr/share/knotifications6/)
