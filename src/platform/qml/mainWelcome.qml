/*
Copyright 2012-2013  Francesco Cecconi <francesco.cecconi@gmail.com>

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of
the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

import QtQuick 1.1

Rectangle {
    id: baseRect
    width: 500
    height: 500
    gradient: Gradient {
        GradientStop { position: 0.140; color: "#39c244" }
        GradientStop { position: 1; color: "#24955b" }
    }

    SystemPalette { id: palette }

    property color onHoverColor: palette.highlight
    property color borderColor:  palette.light
    property color listItemColor: "#39c244"
    property color transparentColor: "#00FFFFFF"
    property color highlightText: palette.highlightedText
    property color normalText: palette.text
    property real opacityValue: 0.8


    Item {
        id: cLeft
        width: (baseRect.width / 3) - 5
        height: baseRect.height

        Rectangle {
            id: b1
            width: cLeft.width - 5
            height: (cLeft.height / 3) - 5
            anchors.top: cLeft.top
            anchors.topMargin: 5
            anchors.left: cLeft.left
            anchors.leftMargin: 5
            smooth: true
            radius: 10
            border.width: 2
            border.color: borderColor
            opacity: opacityValue

            gradient: Gradient {
                GradientStop { id: gradientStop; position: 0.0; color: palette.light }
                GradientStop { position: 1.0; color: palette.button }
            }

            Text {
                id: hostScanLabel
                anchors.horizontalCenter: b1.horizontalCenter
                y: (b1.height / 2) - 42 //(32x32 + 10 margin)
                font.family: palette.text
                font.pointSize: 16
                text: qsTr("Scan an host")
            }

            Image {
                anchors.top: hostScanLabel.bottom
                anchors.topMargin: 10
                anchors.horizontalCenter: b1.horizontalCenter
                source: "qrc:/images/images/network_local.png"
            }

            MouseArea {
                anchors.fill: parent
                hoverEnabled: true
                onEntered: parent.border.color = onHoverColor
                onExited: parent.border.color = borderColor
                onClicked: mainObject.scanSection()
            }
        }

        Rectangle {
            id: b2
            width: cLeft.width - 5
            height: (cLeft.height / 3) - 5
            anchors.top: b1.bottom
            anchors.topMargin: 5
            anchors.left: cLeft.left
            anchors.leftMargin: 5
            smooth: true
            radius: 10
            border.width: 2
            border.color: borderColor
            opacity: opacityValue

            gradient: Gradient {
                GradientStop { id: gradientStop2; position: 0.0; color: palette.light }
                GradientStop { position: 1.0; color: palette.button }
            }

            Text {
                id: vulnerabilityLabel
                anchors.horizontalCenter: b2.horizontalCenter
                y: (b2.height / 2) - 42 //(32x32 + 10 margin)
                font.family: palette.text
                font.pointSize: 16
                text: qsTr("Search a vulnerability")
            }

            Image {
                anchors.top: vulnerabilityLabel.bottom
                anchors.topMargin: 10
                anchors.horizontalCenter: b2.horizontalCenter
                source: "qrc:/images/images/viewmag+.png"
            }

            MouseArea {
                anchors.fill: parent
                hoverEnabled: true
                onEntered: parent.border.color = onHoverColor
                onExited: parent.border.color = borderColor
                onClicked: mainObject.vulnerabilitySection()
            }
        }

        Rectangle {
            id: b3
            width: cLeft.width - 5
            height: (cLeft.height / 3) - 10
            anchors.top: b2.bottom
            anchors.topMargin: 5
            anchors.left: cLeft.left
            anchors.leftMargin: 5
            anchors.bottomMargin: 5
            smooth: true
            radius: 10
            border.width: 2
            border.color: borderColor
            opacity: opacityValue

            gradient: Gradient {
                GradientStop { id: gradientStop3; position: 0.0; color: palette.light }
                GradientStop { position: 1.0; color: palette.button }
            }

            Text {
                id: discoverNetLabel
                anchors.horizontalCenter: b3.horizontalCenter
                y: (b3.height / 2) - 42 //(32x32 + 10 margin)
                font.family: palette.text
                font.pointSize: 16
                text: qsTr("Discover a network")
            }

            Image {
                anchors.top: discoverNetLabel.bottom
                anchors.topMargin: 10
                anchors.horizontalCenter: b3.horizontalCenter
                source: "qrc:/images/images/document-preview-archive.png"
            }

            MouseArea {
                anchors.fill: parent
                hoverEnabled: true
                onEntered: parent.border.color = onHoverColor
                onExited: parent.border.color = borderColor
                onClicked: mainObject.discoverSection()
            }
        }
    }

    Item {
        id: cCenter
        anchors.left: cLeft.right
        anchors.verticalCenter: baseRect.verticalCenter
        anchors.leftMargin: 5
        anchors.rightMargin: 5
        width: (baseRect.width / 3) - 5
        height: baseRect.height

        Rectangle {
            id: history
            width: cCenter.width
            height: cCenter.height - 10
            anchors.top: cCenter.top
            anchors.topMargin: 5
            anchors.bottomMargin: 5
            smooth: true
            radius: 10
            border.width: 2
            border.color: borderColor
            opacity: opacityValue

            gradient: Gradient {
                GradientStop { id: gradientStop4; position: 0.0; color: palette.light }
                GradientStop { position: 1.0; color: palette.button }
            }

            Image {
                id: nmapsi4Image
                anchors.top: history.top
                anchors.topMargin: 10
                anchors.horizontalCenter: history.horizontalCenter
                source: "qrc:/images/icons/128x128/nmapsi4.png"
            }

            Text {
                id: ipHistoryTitle
                anchors.top: nmapsi4Image.bottom
                anchors.topMargin: 10
                height: 25
                anchors.horizontalCenter: history.horizontalCenter
                font.family: palette.text
                font.pointSize: 22
                text: "Nmapsi4"
            }

            Text {
                id: nmapsi4Description
                width: cCenter.width - 10
                anchors.top: ipHistoryTitle.bottom
                anchors.topMargin: 20
                anchors.horizontalCenter: history.horizontalCenter
                font.family: palette.text
                text: description
                wrapMode: Text.WordWrap
            }

            Text {
                id: versionTitle
                anchors.top: nmapsi4Description.bottom
                anchors.topMargin: 20
                height: 25
                anchors.horizontalCenter: history.horizontalCenter
                font.family: palette.text
                font.pointSize: 18
                text: qsTr("Version")
            }

            Text {
                id: versionNumber
                anchors.top: versionTitle.bottom
                anchors.topMargin: 10
                height: 25
                anchors.horizontalCenter: history.horizontalCenter
                font.family: palette.text
                font.pointSize: 14
                text: version_number
            }
        }
    }

    Item {
        id: cRight
        anchors.left: cCenter.right
        anchors.verticalCenter: baseRect.verticalCenter
        anchors.leftMargin: 5
        anchors.rightMargin: 5
        width: (baseRect.width / 3) - 5
        height: baseRect.height

        Rectangle {
            id: webreference
            width: cRight.width
            height: cRight.height - 10
            anchors.top: cRight.top
            anchors.topMargin: 5
            anchors.bottomMargin: 5
            smooth: true
            radius: 10
            border.width: 2
            border.color: borderColor
            opacity: opacityValue

            gradient: Gradient {
                GradientStop { id: gradientStop5; position: 0.0; color: palette.light }
                GradientStop { position: 1.0; color: palette.button }
            }

            Text {
                id: historyPrevLabel
                anchors.top: webreference.top
                anchors.topMargin: 20
                anchors.horizontalCenter: webreference.horizontalCenter
                font.family: palette.text
                font.pointSize: 16
                text: qsTr("Last scanned hosts")
            }

            ListView {
                id: ipHistoryText
                height: webreference.height - historyPrevLabel.height
                width: webreference.width - 10
                anchors.top: historyPrevLabel.bottom
                anchors.topMargin: 15
                anchors.horizontalCenter: webreference.horizontalCenter

                model: ipModel
                delegate: Rectangle {
                    id: rectModel
                    height: 25
                    width: ipHistoryText.width
                    radius: 10
                    color: transparentColor

                    Text {
                        id: hostLabel
                        anchors.centerIn: rectModel
                        font.family: palette.text
                        font.pointSize: 12
                        text: modelData
                    }

                    MouseArea {
                        anchors.fill: parent
                        hoverEnabled: true
                        onEntered: {
                            parent.color = listItemColor
                            hostLabel.color = highlightText
                        }
                        onExited: {
                            parent.color = transparentColor
                            hostLabel.color = normalText
                        }
                        onClicked: mainObject.callScan(modelData)
                    }
                }
            }
        }
    }
}
