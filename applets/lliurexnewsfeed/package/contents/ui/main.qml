import QtQuick
import QtQuick.Layouts
import QtQuick.Controls as QQC2

import org.kde.plasma.plasmoid
import org.kde.plasma.core as PlasmaCore
import org.kde.plasma.components as PC3
import org.kde.plasma.extras as PlasmaExtras
import org.kde.kirigami as Kirigami

import org.kde.plasma.private.lliurexnewsfeed 1.0
// Item - the most basic plasmoid component, an empty container.
PlasmoidItem {

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

    switchWidth: Kirigami.Units.gridUnit * 5
    switchHeight: Kirigami.Units.gridUnit * 5

    Plasmoid.icon:lliurexNewsFeedWidget.iconName
    toolTipMainText: lliurexNewsFeedWidget.toolTip
    toolTipSubText: lliurexNewsFeedWidget.subToolTip
  
    fullRepresentation: Item {
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
