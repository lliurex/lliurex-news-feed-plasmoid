import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15 as QQC2

import org.kde.plasma.plasmoid 2.0
import org.kde.plasma.core 2.1 as PlasmaCore
import org.kde.plasma.components 3.0 as PC3
import org.kde.plasma.extras 2.0 as PlasmaExtras

import org.kde.plasma.private.lliurexnewsfeed 2.0
// Item - the most basic plasmoid component, an empty container.
Item {

    id:lliurexNewsFeedApplet
    
    LliurexNewsFeedWidget{
        id:lliurexNewsFeedWidget

    }


    Plasmoid.status: {
        /* Warn! Enum types are accesed through ClassName not ObjectName */
        switch (lliurexNewsFeedWidget.status){
            case LliurexNewsFeedWidget.ActiveStatus:
                return PlasmaCore.Types.ActiveStatus
            case LliurexNewsFeedWidget.PassiveStatus:
                return PlasmaCore.Types.PassiveStatus
            case LliurexNewsFeedWidget.HiddenStatus:
                return PlasmaCore.Types.HiddenStatus
         
        }
        return  PlasmaCore.Types.ActiveStatus
        
    }

    Plasmoid.switchWidth: units.gridUnit * 5
    Plasmoid.switchHeight: units.gridUnit * 5

    Plasmoid.icon:lliurexNewsFeedWidget.iconName
    Plasmoid.toolTipMainText: lliurexNewsFeedWidget.toolTip
    Plasmoid.toolTipSubText: lliurexNewsFeedWidget.subToolTip

    Component.onCompleted: {
       plasmoid.removeAction("configure");
    }

   
    Plasmoid.preferredRepresentation: Plasmoid.fullRepresentation
    Plasmoid.fullRepresentation: Item {
        id:root
        Layout.fillWidth:true
        QQC2.StackView{
            id:stackLayout
            property int currentIndex:lliurexNewsFeedWidget.currentStackIndex
            width:parent.width
            height:parent.height
            initialItem:blogPanel
            onCurrentIndexChanged:{
                switch(currentIndex){
                    case 0:
                        stackLayout.replace(blogPanel)
                        break;
                }
            }

            Component{
                id:blogPanel
                BlogView{
                    id:blogView
                }
            }
        }

    }
} 
