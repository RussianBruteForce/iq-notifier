/*
 *     This file is part of IQ Notifier.
 *
 * IQ Notifier is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * IQ Notifier is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with IQ Notifier.  If not, see <http://www.gnu.org/licenses/>.
 */

import QtQuick 2.0
import QtQuick.Window 2.1
import IQNotifier 1.0

IQPopup {
    id: root
    visible: IQNotifications.extraNotifications > 0
    x: IQNotifications.extraWindowPos.x
    y: IQNotifications.extraWindowPos.y
    width: IQNotifications.extraWindowSize.width
    height: IQNotifications.extraWindowSize.height

    property real elementsScale: 0.4
    property color bgColor: "#262d3a"

    IQFancyContainer {
        id: iQFancyContainer
        anchors.fill: parent
        Rectangle {
            id: bg
            color: bgColor
            anchors.fill: parent
        }
        Rectangle {
            id: circle
            scale: 0.6
            color: "#d74a37"
            height: parent.height
            width: height
            radius: width
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            Text {
                id: count
                color: "#fff"
                font.pointSize: parent.height/2.5
                text: IQNotifications.extraNotifications
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
            }
        }

        MouseArea {
            id: closeVisible
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.right: closeExtra.left
            height: root.height
            anchors.rightMargin: 0
            width: height
            hoverEnabled: true
            Image {
                scale: elementsScale
                anchors.fill: parent
                source: "img/closeVisible.png"
                opacity: parent.containsMouse ? 0.85 : 1
            }
            onClicked: IQNotifications.onDropVisible()
        }

        MouseArea {
            id: closeExtra
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.right: closeAll.left
            height: root.height
            anchors.rightMargin: 0
            width: height
            hoverEnabled: true
            Image {
                scale: elementsScale
                anchors.fill: parent
                source: "img/close.png"
                opacity: parent.containsMouse ? 0.85 : 1
            }
            onClicked: IQNotifications.onDropStacked()
        }

        MouseArea {
            id: closeAll
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.right: parent.right
            width: height
            hoverEnabled: true
            Rectangle {
                color: "#d74a37"
                anchors.fill: parent
                radius: height
                scale: 0.5
            }
            Image {
                scale: elementsScale
                anchors.fill: parent
                source: "img/close.png"
                opacity: parent.containsMouse ? 0.85 : 1
            }
            onClicked: IQNotifications.onDropAll()
        }
    }
}
