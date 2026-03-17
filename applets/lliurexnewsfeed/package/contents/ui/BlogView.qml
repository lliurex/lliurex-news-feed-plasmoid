import QtQuick
import QtQuick.Layouts
import QtQml.Models
import QtQuick.Controls
import org.kde.plasma.core as PlasmaCore
import org.kde.plasma.plasmoid 
import org.kde.plasma.components as PC3
import org.kde.plasma.extras as PlasmaExtras
import org.kde.kirigami as Kirigami


Rectangle{
    id:container
    color:"transparent"

    DelegateModel{

        id:filterModel
        property var visibleItems:[]

        model:lliurexNewsFeedWidget.blogRssModel
        
        delegate: Item {
            id:rssBlogItem
            width:rssBlogList.width-18
            height:Math.max(rssBlogEntry.height, Math.round(Kirigami.Units.gridUnit * 1.6)) + 2 * Kirigami.Units.smallSpacing
            MouseArea{
                id:itemArea
                anchors.fill:parent
                hoverEnabled:true
                propagateComposedEvents:false
                onEntered:rssBlogList.currentIndex=filterModel.visibleItems.indexOf(index)
            }
  
            PC3.Label{
                id:rssBlogEntry
                maximumLineCount: 3
                text: model.title
                width: parent.width-(linkBtn.width+newRss.width+50)
                anchors.verticalCenter:parent.verticalCenter
                wrapMode: Text.WordWrap
                padding: 10
            }

            Image{
                id:newRss
                source:"/usr/share/icons/hicolor/scalable/apps/lliurex-rss-new-post.svg"
                sourceSize.width:18
                sourceSize.height:18
                anchors.left:rssBlogEntry.right
                anchors.verticalCenter:parent.verticalCenter
                anchors.leftMargin:15
                visible:{
                    if (lliurexNewsFeedWidget.canFilterBlogRss){
                        if (!filterSwitchButton.checked){
                            model.isNew
                        }else{
                            false
                        }
                    }else{
                        false
                    }
                }
            }


            PC3.ToolButton{
                id:linkBtn
                width:35
                height:35
                display:AbstractButton.IconOnly
                icon.name:"folder-html.svg"
                anchors.leftMargin:15
                anchors.left:newRss.right
                anchors.verticalCenter:parent.verticalCenter
                visible:rssBlogItem.ListView.isCurrentItem?true:false
                PC3.ToolTip{
                    id:linkTT
                    text:i18n("Click to see the post")
                }
                onClicked:{
                    Qt.openUrlExternally(model.link)
                }
            }
        }
        groups:[
            DelegateModelGroup{
                id:allItems
                name:"all"
                includeByDefault:true
                onCountChanged:Qt.callLater(filterModel.updateFilter)

            },
            DelegateModelGroup{
                id:filteredItem
                name:"visible"
                includeByDefault:false
            }
        ]


        filterOnGroup:"visible"
        
        function updateFilter(){
            visibleItems=[]
            if (allItems.count>0){
                allItems.setGroups(0,allItems.count,["all"]);
                for (var index=0; index<allItems.count;index++){
                    let item=allItems.get(index).model;
                    let matchStatus=true

                    if (filterSwitchButton.checked){
                        if (item["isNew"]){
                            matchStatus=true
                        }else{
                            matchStatus=false
                        }
                    }else{
                       matchStatus=true
                    }

                    if (!matchStatus) continue;
                    allItems.setGroups(index,1,["all","visible"])
                    visibleItems.push(index)
                }
            }

        }
        Component.onCompleted:Qt.callLater(filterModel.updateFilter)
    }

    GridLayout{
        id:blogLayout
        rows: 2
        flow: GridLayout.TopToBottom
        rowSpacing:10
        width:parent.width
        RowLayout{
            id:headLatestBlog
            Layout.fillWidth:true
            PC3.Label{
                id:headBlogText
                text:i18n("LliureX blog")
                font.italic:true
                font.pointSize:11
                Layout.fillWidth:true
                Layout.leftMargin:15
                Layout.alignment:Qt.AlignHCenter|Qt.AlignVCenter
            }
            PC3.Label{
                id:switchText
                text:i18n("Show only news post:")
                font.pointSize:10
                width:headLatestBlog.width-(headBlogText.width+filterSwitchButton.width)
                Layout.alignment:Qt.AlignRight|Qt.AlignVCenter
            }

            PC3.Switch {
                id: filterSwitchButton
                checked:true
                Layout.alignment:Qt.AlignRight|Qt.AlignVCenter
                Layout.rightMargin:30
                PC3.ToolTip{
                    id:filterTT
                    text:{
                        if (filterSwitchButton.checked){
                            i18n("Clic to show other previous posts")
                        }else{
                            i18n("Clic to show only new posts")
                        }
                    }
                }
                indicator: Rectangle {
                    implicitWidth: 40
                    implicitHeight: 15
                    x: filterSwitchButton.width - width - filterSwitchButton.rightPadding
                    y: parent.height/2 - height/2 
                    radius: 7
                    color: filterSwitchButton.checked ? "#badcee" : "#dbdcde"
                    border.width:1
                    border.color: filterSwitchButton.checked ? "#3daee9" : "#a1a1a1"

                    Rectangle {
                        x: filterSwitchButton.checked ? parent.width - width : 0
                        width: 20
                        height: 20
                        y:parent.height/2-height/2
                        radius: 10
                        border.color: "#808080"
                   }
                }
                onToggled:filterModel.updateFilter();
           }
        }

        PC3.ScrollView {
            Layout.topMargin:10
            Layout.bottomMargin:5
            Layout.leftMargin:10
            Layout.rightMargin:10
            implicitWidth:parent.width-10
            implicitHeight:300
            ListView{
                id:rssBlogList
                anchors.fill: parent
                model: filterModel
                focus:true
                currentIndex:-1
                highlight: Rectangle { color: "#add8e6"; opacity:0.8;border.color:"#53a1c9" }
                highlightMoveDuration: 0
                highlightResizeDuration: 0
                PlasmaExtras.PlaceholderMessage {
                    id:emptyHint
                    anchors.centerIn:parent
                    width:parent.width*2-(Kirigami.Units.gridUnit * 4)
                    visible:{
                        if (rssBlogList.count>0){
                            return false
                        }else{
                            return true
                        }
                    }
                    text:i18n("The are no new posts on the LliureX blog")
                }
            }
        }

     }

}
