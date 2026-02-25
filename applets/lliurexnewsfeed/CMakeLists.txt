
#######################################################################################
# Package
plasma_install_package(package org.kde.plasma.lliurexnewsfeed)


#######################################################################################
# Notes Library
add_definitions(-DTRANSLATION_DOMAIN="plasma_applet_org.kde.plasma.lliurexnewsfeed")

set(lliurexnewsfeedwidget_SRCS
    plugin/plugin.cpp
    plugin/LliurexNewsFeedWidget.cpp
    plugin/LliurexNewsFeedWidgetUtils.cpp
    plugin/LliurexNewsFeedWidgetRssModel.cpp
    plugin/LliurexNewsFeedWidgetRssItem.cpp
   
)

add_library(lliurexnewsfeedplugin SHARED ${lliurexnewsfeedwidget_SRCS})

target_link_libraries(lliurexnewsfeedplugin
                      Qt5::Quick
                      Qt5::Core
                      Qt5::Widgets
                      KF5::CoreAddons
                      KF5::I18n
                      KF5::Notifications
                      KF5::KIOWidgets
  
                      )

install(FILES plugin/qmldir DESTINATION ${QML_INSTALL_DIR}/org/kde/plasma/private/lliurexnewsfeed)
install(TARGETS lliurexnewsfeedplugin DESTINATION ${QML_INSTALL_DIR}/org/kde/plasma/private/lliurexnewsfeed)
install(FILES "icons/lliurex-news-feed.svg"
        DESTINATION ${DATAROOTDIR}/usr/share/icons/hicolor/scalable/apps/)
install(FILES "icons/lliurex-news-feed-updated.svg"
        DESTINATION ${DATAROOTDIR}/usr/share/icons/hicolor/scalable/apps/)
install(FILES "icons/lliurex-rss-new-post.svg"
        DESTINATION ${DATAROOTDIR}/usr/share/icons/hicolor/scalable/apps/)
install(FILES "knotifications5/lliurexnewsfeed.notifyrc"
       DESTINATION ${DATAROOTDIR}/usr/share/knotifications5/)
