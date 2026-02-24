import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQml.Models 2.15
import QtQuick.Controls 2.15
import org.kde.plasma.core 2.1 as PlasmaCore
import org.kde.plasma.plasmoid 2.0
import org.kde.plasma.components 2.0 as Components
import org.kde.plasma.components 3.0 as PC3
import QtQuick.XmlListModel 2.15
import org.kde.plasma.private.lliurexnewsfeed 1.0

Rectangle{
	color:"transparent"
    /*
    XmlListModel {
        id: rssBlogModel
        source: "https://portal.edu.gva.es/blogs/s1/lliurex/feed" // URL del fil
        query: "/rss/channel/item" // Ruta XML als elements de la llista

        XmlRole { name: "title"; query: "title/string()" }
        XmlRole { name: "link"; query: "link/string()" }
        XmlRole { name: "pubDate";query: "pubDate/string()"}
        onStatusChanged:{
            if (status==XmlListModel.Loading){
                console.log("Cargando")
            }else if (status==XmlListModel.Ready){
                console.log("LISTO")
                console.log(Qt.locale().name)
                if (rssBlogModel.count>0){
                    var lastItemDate=get(0).pubDate
                    var lastUpdateDate=new Date(lastItemDate)
                    console.log(lastUpdateDate.toLocaleDateString(Qt.locale(),"yyyy-MM-dd"))
                    
                 }
            }else if (status==XmlListModel.Error){
                console.log("ERROR")
            }
        }
    }
    */
    DelegateModel{
        id:filterModel
        model:lliurexNewsFeedWidget.rssBlogModel
        delegate: Item {
            id:rssBlogItem
            width:parent.width-18
            height:60
            MouseArea{
                id:itemArea
                anchors.fill:parent
                hoverEnabled:true
                propagateComposedEvents:false
                onEntered:rssBlogList.currentIndex=index
            }
            property var rssDate:new Date(model.pubDate)
            Text{
                id:rssBlogEntry
                text: {
                    var itemDate=rssDate.toLocaleDateString(Qt.locale(),"dd/MM/yyyy")
                
                    return itemDate+" - "+model.title
                }
                width: parent.width-(linkBtn.width+45)
                anchors.verticalCenter:parent.verticalCenter
                wrapMode: Text.WordWrap
                padding: 10
            }

            PC3.ToolButton{
                id:linkBtn
                width:35
                height:35
                display:AbstractButton.IconOnly
                icon.name:"folder-html.svg"
                anchors.leftMargin:20
                anchors.left:rssBlogEntry.right
                anchors.verticalCenter:parent.verticalCenter
                visible:rssBlogItem.ListView.isCurrentItem?true:false
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

            let refDate=new Date("2025-01-01");
            for (var i=0; i<items.count;i++){
                let item=items.get(i).model;
                let itemDate=new Date(item.pubDate);
                if (itemDate.getTime()>refDate.getTime()){
                    items.get(i).inVisible=true
                }else{
                    items.get(i).inVisible=true
                }
            }
        }
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
            Components.Label{
                id:headBlogText
                text:i18n("News on the LliureX blog")
                font.italic:true
                font.pointSize:11
                Layout.fillWidth:true
                Layout.leftMargin:15
                Layout.alignment:Qt.AlignHCenter
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
            }
        }
     }

}
