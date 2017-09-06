/*
 * This file is a part of Xpiks - cross platform application for
 * keywording and uploading images for microstocks
 * Copyright (C) 2014-2017 Taras Kushnir <kushnirTV@gmail.com>
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

import QtQuick 2.0

Item {
    id: host
    width: 25
    height: 25
    property color iconColor: editMA.pressed ? uiColors.linkClickedColor : (editMA.containsMouse ? uiColors.artworkActiveColor : uiColors.inputBackgroundColor)
    property color backgroundColor: uiColors.defaultControlColor
    property double lineWidth: width / 9
    signal actionInvoked()

    Rectangle {
        id: background
        border.width: host.lineWidth
        border.color: host.iconColor
        anchors.fill: parent
        anchors.rightMargin: host.width/5
        anchors.topMargin: host.width/5
        color: "transparent"
    }

    Item {
        id: pencil
        anchors.fill: parent
        anchors.bottomMargin: host.width/4
        anchors.leftMargin: host.width/4

        Rectangle {
            height: host.lineWidth*1.2
            width: 1
            anchors.left: parent.left
            anchors.bottom: parent.bottom
            color: host.iconColor
            anchors.leftMargin: 0.5
            anchors.bottomMargin: 0.5
        }

        Rectangle {
            height: 1
            width: host.lineWidth*1.2
            anchors.left: parent.left
            anchors.bottom: parent.bottom
            color: host.iconColor
            anchors.bottomMargin: 0.5
            anchors.leftMargin: 0.5
        }

        Rectangle {
            x: host.lineWidth*1.7
            y: parent.height - height/2 - host.lineWidth*1.7
            width: parent.width
            height: host.lineWidth*4
            rotation: -45
            transformOrigin: Item.Left
            color: host.backgroundColor
        }

        Rectangle {
            x: host.lineWidth*0.75
            y: parent.height - height/2 - host.lineWidth*0.75
            width: parent.width*0.8
            height: host.lineWidth*1.6
            rotation: -45
            transformOrigin: Item.Left
            color: host.iconColor

            Rectangle {
                width: host.lineWidth
                anchors.bottom: parent.bottom
                anchors.top: parent.top
                anchors.right: parent.right
                anchors.rightMargin: -5
                color: host.iconColor
            }
        }
    }

    MouseArea {
        id: editMA
        anchors.fill: parent
        hoverEnabled: true
        onClicked: actionInvoked()
    }
}

