import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQml.Models 2.15
import QtQuick.Controls 2.15
import org.kde.plasma.core 2.1 as PlasmaCore
import org.kde.plasma.plasmoid 2.0
import org.kde.plasma.components 2.0 as Components
import org.kde.plasma.components 3.0 as PC3
import QtQuick.XmlListModel 2.15
import org.kde.kirigami 2.12 as Kirigami
import org.kde.plasma.private.lliurexnewsfeed 1.0

Rectangle{
    id:container
    color:"transparent"

    DelegateModel{
        id:filterModel
        model:lliurexNewsFeedWidget.blogRssModel
        delegate: Item {
            id:rssBlogItem
            width:rssBlogList.width-18
            height:60
            MouseArea{
                id:itemArea
                anchors.fill:parent
                hoverEnabled:true
                propagateComposedEvents:false
                onEntered:rssBlogList.currentIndex=index
            }
  
            Text{
                id:rssBlogEntry
                text: model.title
                width: parent.width-(linkBtn.width+newRss.width+50)
                anchors.verticalCenter:parent.verticalCenter
                wrapMode: Text.WordWrap
                padding: 10
            }

            Image{
                id:newRss
                source:"/usr/share/icons/hicolor/scalable/apps/lliurex-rss-new-post.svg"
                sourceSize.width:24
                sourceSize.height:24
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
                PC3.ToolTip.text:i18n("Click to see the article")
                onClicked:{
                    Qt.openUrlExternally(model.link)
                }
            }
        }
        groups:[
            DelegateModelGroup{
                id:filteredItem
                name:"visible"
                includeByDefault:false
             }
        ]

        filterOnGroup:"visible"

        function updateFilter(){

            for (var i=0; i<items.count;i++){
                if (filterSwitchButton.checked){
                    let item=items.get(i).model;
                    if (item.isNew){
                        items.get(i).inVisible=true
                    }else{
                        items.get(i).inVisible=false
                    }
                }else{
                   items.get(i).inVisible=true
                }

            }
        }
    }

    GridLayout{
        id:blogLayout
        rows: 2
        focus:true
        flow: GridLayout.TopToBottom
        rowSpacing:10
        width:parent.width
        RowLayout{
            id:headLatestBlog
            Layout.fillWidth:true
            Components.Label{
                id:headBlogText
                text:i18n("LliureX blog")
                font.italic:true
                font.pointSize:11
                Layout.fillWidth:true
                Layout.leftMargin:15
                Layout.alignment:Qt.AlignHCenter
            }
            Components.Label{
                id:switchText
                text:i18n("Show only news post:")
                font.pointSize:10
                width:headLatestBlog.width-(headBlogText.width+filterSwitchButton.width)
                Layout.rightMargin:10
                Layout.alignment:Qt.AlignRight
            }

            PC3.Switch {
                id: filterSwitchButton
                checked:true
                focus:true
                Layout.alignment:Qt.AlignRight
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
                Component.onCompleted:{
                    filterModel.updateFilter();
                }
                Kirigami.PlaceholderMessage{
                    id:emptyHint
                    anchors.centerIn:parent
                    width:parent.width-(units.largeSpacing*4)
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
