/**********************************************************************************
****************** Butianyun QT Video Lesson V2 ***********************************
*********** BUTIANYUN， QT Programming Trainning Professional **********************
***********************************************************************************/


import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Window {
    id: root
    width: 800
    height: 600
    visible: true
    title: qsTr("补天云QT系列视频课程")

    Button {
        id: button_reset
        anchors.horizontalCenter: parent.horizontalCenter
        width: 100
        height: 40
        text: qsTr("reset")
        onClicked: {
            fsmodel.clearCache()
        }
    }

    property var columns: [500, 200, 100, 200]
    function get_column_width(column) {
        var total = 0
        for (var i = 1; i < columns.length; i++) {
            total += columns[i]
        }
        if (column === 0 && total + columns[0] < root.width) {
            return root.width - total
        }
        return columns[column]
    }

    Row {
        id: header
        anchors.top: button_reset.bottom
        width: parent.width
        height: 40
        spacing: 10
        Repeater {
            model: fsmodel.columnCount()
            Text {
                height: parent.height
                width: get_column_width(index)
                text: fsmodel.headerData(index, Qt.Horizontal)
            }
        }
    }
    TreeView {
        id: tree
        anchors.topMargin: header.height + button_reset.height
        anchors.fill: parent
        model:  fsmodel
        columnWidthProvider: get_column_width
        columnSpacing: 10
        delegate: TreeViewDelegate {}
    }


}

//        delegate:  ItemDelegate {
//           implicitWidth: treeView.width

//           required property TreeView treeView;
//           required property bool expanded
//           required property int hasChildren
//           required property int depth


//            Row {
//                anchors.fill: parent
//                anchors.leftMargin: depth * 15

//                Button {
//                    text: "+"
//                    visible: !expanded && hasChildren
//                    onClicked: {
//                        console.log("+", index)
//                        treeView.expand(index)
//                    }
//                }
//                Button {
//                    text: "-"
//                    visible: expanded && hasChildren
//                    onClicked: {
//                        console.log("-", index)
//                        treeView.collapse(index)

//                    }
//                }

//                Text {
//                    text: fileName
//                }

//            }
//        }
