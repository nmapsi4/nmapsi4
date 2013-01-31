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
import "style" 0.1

Rectangle {
    id: baseRect
    width: 500
    height: 500

    Properties { id: style }

    gradient: style.backgroundGradient

    Item {
        id: cLeft
        width: (baseRect.width / 3) - 5
        height: baseRect.height

        Rectangle {
            id: b1

            anchors {
                top: cLeft.top
                topMargin: 5
                left: cLeft.left
                leftMargin: 5
            }

            width: cLeft.width - 5
            height: (cLeft.height / 3) - 5
            smooth: true
            radius: 10
            border.width: 2
            border.color: style.borderColor
            opacity: style.opacityValue
            gradient: style.widgetGradient

            Text {
                id: hostScanLabel
                anchors.horizontalCenter: b1.horizontalCenter
                y: (b1.height / 2) - 42 //(32x32 + 10 margin)
                font.pointSize: 16
                text: qsTr("Scan an host")
            }

            Image {

                anchors {
                    top: hostScanLabel.bottom
                    topMargin: 10
                    horizontalCenter: b1.horizontalCenter
                }

                source: "qrc:/images/images/network_local.png"
            }

            MouseArea {
                anchors.fill: parent
                hoverEnabled: true
                onEntered: parent.border.color = style.onHoverColor
                onExited: parent.border.color = style.borderColor
                onClicked: mainObject.scanSection()
            }
        }

        Rectangle {
            id: b2

            anchors {
                top: b1.bottom
                topMargin: 5
                left: cLeft.left
                leftMargin: 5
            }

            width: cLeft.width - 5
            height: (cLeft.height / 3) - 5
            smooth: true
            radius: 10
            border.width: 2
            border.color: style.borderColor
            opacity: style.opacityValue

            gradient: style.widgetGradient

            Text {
                id: vulnerabilityLabel
                anchors.horizontalCenter: b2.horizontalCenter
                y: (b2.height / 2) - 42 //(32x32 + 10 margin)
                font.pointSize: 16
                text: qsTr("Search a vulnerability")
            }

            Image {

                anchors {
                    top: vulnerabilityLabel.bottom
                    topMargin: 10
                    horizontalCenter: b2.horizontalCenter
                }

                source: "qrc:/images/images/viewmag+.png"
            }

            MouseArea {
                anchors.fill: parent
                hoverEnabled: true
                onEntered: parent.border.color = style.onHoverColor
                onExited: parent.border.color = style.borderColor
                onClicked: mainObject.vulnerabilitySection()
            }
        }

        Rectangle {
            id: b3

            anchors {
                top: b2.bottom
                topMargin: 5
                left: cLeft.left
                leftMargin: 5
                bottomMargin: 5
            }

            width: cLeft.width - 5
            height: (cLeft.height / 3) - 10
            smooth: true
            radius: 10
            border.width: 2
            border.color: style.borderColor
            opacity: style.opacityValue

            gradient: style.widgetGradient

            Text {
                id: discoverNetLabel
                anchors.horizontalCenter: b3.horizontalCenter
                y: (b3.height / 2) - 42 //(32x32 + 10 margin)
                font.pointSize: 16
                text: qsTr("Discover a network")
            }

            Image {

                anchors {
                    top: discoverNetLabel.bottom
                    topMargin: 10
                    horizontalCenter: b3.horizontalCenter
                }

                source: "qrc:/images/images/document-preview-archive.png"
            }

            MouseArea {
                anchors.fill: parent
                hoverEnabled: true
                onEntered: parent.border.color = style.onHoverColor
                onExited: parent.border.color = style.borderColor
                onClicked: mainObject.discoverSection()
            }
        }
    }

    Item {
        id: cCenter

        anchors {
            left: cLeft.right
            verticalCenter: baseRect.verticalCenter
            leftMargin: 5
            rightMargin: 5
        }

        width: (baseRect.width / 3) - 5
        height: baseRect.height

        Rectangle {
            id: history

            anchors {
                top: cCenter.top
                topMargin: 5
                bottomMargin: 5
            }

            width: cCenter.width
            height: cCenter.height - 10
            smooth: true
            radius: 10
            border.width: 2
            border.color: style.borderColor
            opacity: style.opacityValue

            gradient: style.widgetGradient

            Image {
                id: nmapsi4Image

                anchors {
                    top: history.top
                    topMargin: 10
                    horizontalCenter: history.horizontalCenter
                }

                source: "qrc:/images/icons/128x128/nmapsi4.png"
            }

            Text {
                id: ipHistoryTitle

                anchors {
                    top: nmapsi4Image.bottom
                    topMargin: 10
                    horizontalCenter: history.horizontalCenter
                }

                height: 25
                font.pointSize: 22
                text: "Nmapsi4"
            }

            Text {
                id: nmapsi4Description

                anchors {
                    top: ipHistoryTitle.bottom
                    topMargin: 20
                    horizontalCenter: history.horizontalCenter
                }

                width: cCenter.width - 10
                text: description
                wrapMode: Text.WordWrap
            }

            Text {
                id: versionTitle

                anchors {
                    top: nmapsi4Description.bottom
                    topMargin: 20
                    horizontalCenter: history.horizontalCenter
                }

                height: 25
                font.pointSize: 18
                text: qsTr("Version")
            }

            Text {
                id: versionNumber

                anchors {
                    top: versionTitle.bottom
                    topMargin: 10
                    horizontalCenter: history.horizontalCenter
                }

                height: 25
                font.pointSize: 14
                text: version_number
            }
        }
    }

    Item {
        id: cRight

        anchors {
            left: cCenter.right
            verticalCenter: baseRect.verticalCenter
            leftMargin: 5
            rightMargin: 5
        }

        width: (baseRect.width / 3) - 5
        height: baseRect.height

        Rectangle {
            id: webreference

            anchors {
                top: cRight.top
                topMargin: 5
                bottomMargin: 5
            }

            width: cRight.width
            height: cRight.height - 10
            smooth: true
            radius: 10
            border.width: 2
            border.color: style.borderColor
            opacity: style.opacityValue

            gradient: style.widgetGradient

            Text {
                id: historyPrevLabel

                anchors {
                    top: webreference.top
                    topMargin: 20
                    horizontalCenter: webreference.horizontalCenter
                }

                font.pointSize: 16
                text: qsTr("Last scanned hosts")
            }

            ListView {
                id: ipHistoryText

                anchors {
                    top: historyPrevLabel.bottom
                    topMargin: 15
                    horizontalCenter: webreference.horizontalCenter
                }

                height: webreference.height - historyPrevLabel.height
                width: webreference.width - 10

                model: ipModel
                delegate: Rectangle {
                    id: rectModel
                    height: 25
                    width: ipHistoryText.width
                    radius: 10
                    color: style.transparentColor

                    Text {
                        id: hostLabel
                        anchors.centerIn: rectModel
                        font.pointSize: 12
                        text: modelData
                    }

                    MouseArea {
                        anchors.fill: parent
                        hoverEnabled: true
                        onEntered: {
                            parent.color = style.listItemColor
                            hostLabel.color = style.highlightText
                        }
                        onExited: {
                            parent.color = style.transparentColor
                            hostLabel.color = style.normalText
                        }
                        onClicked: mainObject.callScan(modelData)
                    }
                }
            }
        }
    }
}
