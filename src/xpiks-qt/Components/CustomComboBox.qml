/*
 * This file is a part of Xpiks - cross platform application for
 * keywording and uploading images for microstocks
 * Copyright (C) 2014-2017 Taras Kushnir <kushnirTV@gmail.com>
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

import QtQuick 2.2
import QtQuick.Controls 1.2
import "../Constants"
import "../StyledControls"
import xpiks 1.0
import QtGraphicalEffects 1.0

Item {
    id: comboBox
    property color highlightedItemColor: Colors.artworkActiveColor
    property bool showColorSign: true
    property bool hasLastItemAction: false
    property string lastActionText: ''
    property double headerHeight: comboBox.height
    property double itemHeight: comboBox.height
    property double maxCount: 5
    property alias model: dropDownItems.model
    property alias selectedIndex: dropDownItems.currentIndex
    property color comboboxBackgroundColor: Colors.defaultControlColor
    signal comboIndexChanged();
    signal lastItemActionInvoked();

    function closePopup() {
        comboBox.state = ""
    }

    RectangularGlow {
        anchors.left: header.left
        anchors.right: header.right
        anchors.top: header.top
        anchors.bottom: dropDown.bottom
        anchors.topMargin: glowRadius/2
        anchors.bottomMargin: -glowRadius
        visible: dropDown.visible
        height: dropDown.height
        glowRadius: 4
        spread: 0.0
        color: Colors.defaultDarkColor
        cornerRadius: glowRadius
    }

    Item {
        id: header
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        height: headerHeight

        Rectangle {
            id: colorSign
            anchors.left: parent.left
            width: showColorSign ? 6 : 0
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            color: highlightedItemColor
        }

        Rectangle {
            id: selectedItem
            anchors.left: colorSign.right
            anchors.right: arrowRect.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            color: comboboxBackgroundColor

            StyledText {
                id: selectedText
                text: dropDownItems.currentItem ? dropDownItems.currentItem.itemText : ""
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                anchors.leftMargin: 10
                width: parent.width - 10
                elide: Text.ElideRight
                verticalAlignment: TextInput.AlignVCenter
            }
        }

        Rectangle {
            id: arrowRect
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            width: 20
            color: enabled ? Colors.artworkBackground : Colors.panelSelectedColor

            TriangleElement {
                anchors.centerIn: parent
                anchors.verticalCenterOffset: isFlipped ? height*0.3 : 0
                color: (enabled && (headerMA.containsMouse || comboBox.state === "dropDown")) ? Colors.labelActiveForeground : Colors.labelInactiveForeground
                isFlipped: comboBox.state === ""
                width: parent.width * 0.6
                height: width * 0.5
            }
        }

        MouseArea {
            id: headerMA
            hoverEnabled: true
            anchors.fill: parent;
            onClicked: {
                comboBox.state = comboBox.state === "dropDown" ? "" : "dropDown"
                dropDown.forceActiveFocus()
            }
        }
    }

    Rectangle {
        id: dropDown
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: header.bottom
        color: Colors.popupBackgroundColor
        visible: false
        height: 0
        focus: true
        clip: true

        onActiveFocusChanged: {
            if (!activeFocus) {
                comboBox.state = ""
            }
        }

        ListView {
            id: dropDownItems
            property int itemsCount: count + (hasLastItemAction ? 1 : 0)
            anchors.fill: parent
            anchors.topMargin: 5
            anchors.bottomMargin: 5
            anchors.rightMargin: scrollBar.visible ? 14 : 0
            anchors.margins: 1
            boundsBehavior: Flickable.StopAtBounds

            delegate: Rectangle {
                id: currentDelegate
                color: itemMA.containsMouse ? highlightedItemColor : dropDown.color
                property var itemText: modelData
                property bool isCurrentItem: index == comboBox.selectedIndex
                property bool isLastItem: index === (dropDownItems.count - 1)
                anchors.left: parent.left
                anchors.right: parent.right
                height: itemHeight + 1

                StyledText {
                    text: i18.n + itemText
                    anchors.left: parent.left
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.leftMargin: 16
                    color: itemMA.containsMouse ? Colors.whiteColor : (isCurrentItem ? highlightedItemColor : Colors.labelActiveForeground)
                }

                Rectangle {
                    visible: !currentDelegate.isLastItem || hasLastItemAction
                    enabled: !currentDelegate.isLastItem || hasLastItemAction
                    height: 1
                    color: itemMA.containsMouse ? highlightedItemColor : Colors.inputBackgroundColor
                    anchors.bottom: parent.bottom
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.leftMargin: 6
                    anchors.rightMargin: 20
                    anchors.bottomMargin: -height/2
                }

                MouseArea {
                    id: itemMA
                    anchors.fill: parent
                    hoverEnabled: true
                    onClicked: {
                        if (index === comboBox.selectedIndex) {
                            comboBox.state = ""
                            return;
                        }

                        comboBox.selectedIndex = index
                        comboBox.state = ""
                        comboIndexChanged()
                    }
                }
            }

            footer: Rectangle {
                visible: hasLastItemAction
                enabled: hasLastItemAction
                color: lastItemMA.containsMouse ? highlightedItemColor : dropDown.color
                anchors.left: parent.left
                anchors.right: parent.right
                height: hasLastItemAction ? (itemHeight + 1) : 0

                StyledText {
                    text: lastActionText
                    anchors.left: parent.left
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.leftMargin: 16
                    color: lastItemMA.containsMouse ? Colors.whiteColor : Colors.labelActiveForeground
                }

                MouseArea {
                    id: lastItemMA
                    anchors.fill: parent
                    hoverEnabled: true
                    onClicked: {
                        comboBox.state = ""
                        lastItemActionInvoked()
                    }
                }
            }
        }

        CustomScrollbar {
            id: scrollBar
            visible: dropDownItems.itemsCount > maxCount
            anchors.rightMargin: -12
            flickable: dropDownItems
        }
    }

    states: State {
        name: "dropDown";
        PropertyChanges {
            target: dropDown;
            height: dropDownItems.itemsCount > maxCount ? (maxCount * (comboBox.itemHeight + 1) + 10) : ((comboBox.itemHeight + 1) * dropDownItems.itemsCount + 10)
            visible: true
        }
    }

    transitions: Transition {
        NumberAnimation {
            target: dropDown;
            properties: "height";
            easing.type: Easing.OutExpo; duration: 400
        }
    }
}
