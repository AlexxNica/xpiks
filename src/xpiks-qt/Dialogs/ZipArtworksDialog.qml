/*
 * This file is a part of Xpiks - cross platform application for
 * keywording and uploading images for microstocks
 * Copyright (C) 2014-2015 Taras Kushnir <kushnirTV@gmail.com>
 *
 * Xpiks is distributed under the GNU General Public License, version 3.0
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.1
import QtQuick.Dialogs 1.2
import QtQuick.Controls.Styles 1.2
import "../Constants"
import "../Constants/Colors.js" as Colors;
import "../Common.js" as Common;
import "../Components"
import "../StyledControls"

Item {
    id: zipArtworksComponent
    property bool immediateProcessing: false
    property var callbackObject
    anchors.fill: parent

    function closePopup() {
        zipArtworksComponent.destroy()
    }

    PropertyAnimation { target: zipArtworksComponent; property: "opacity";
        duration: 400; from: 0; to: 1;
        easing.type: Easing.InOutQuad ; running: true }

    // This rectange is the a overlay to partially show the parent through it
    // and clicking outside of the 'dialog' popup will do 'nothing'
    Rectangle {
        anchors.fill: parent
        id: overlay
        color: "#000000"
        opacity: 0.6
        // add a mouse area so that clicks outside
        // the dialog window will not do anything
        MouseArea {
            anchors.fill: parent
        }
    }

    FocusScope {
        anchors.fill: parent

        MouseArea {
            anchors.fill: parent
            onWheel: wheel.accepted = true
            onClicked: mouse.accepted = true

            property real old_x : 0
            property real old_y : 0

            onPressed:{
                var tmp = mapToItem(zipArtworksComponent, mouse.x, mouse.y);
                old_x = tmp.x;
                old_y = tmp.y;

                var dialogPoint = mapToItem(dialogWindow, mouse.x, mouse.y);
                if (!Common.isInComponent(dialogPoint, dialogWindow)) {
                    closePopup()
                }
            }

            onPositionChanged: {
                var old_xy = Common.movePopupInsideComponent(zipArtworksComponent, dialogWindow, mouse, old_x, old_y);
                old_x = old_xy[0]; old_y = old_xy[1];
            }
        }

        // This rectangle is the actual popup
        Rectangle {
            id: dialogWindow
            width: 480
            height: 150
            color: Colors.selectedArtworkColor
            anchors.centerIn: parent
            Component.onCompleted: anchors.centerIn = undefined

            ColumnLayout {
                spacing: 10
                anchors.fill: parent
                anchors.margins: 20

                RowLayout {
                    Layout.fillWidth: true

                    StyledText {
                        text: qsTr("Zip zelected artworks with .eps files")
                    }

                    Item {
                        Layout.fillWidth: true
                    }

                    StyledText {
                        text: zipArchiver.itemsCount == 1 ? qsTr("1 artwork") : qsTr("%1 artworks").arg(zipArchiver.itemsCount)
                        color: Colors.defaultInputBackground
                    }
                }

                SimpleProgressBar {
                    id: progress
                    anchors.horizontalCenter: parent.horizontalCenter
                    width: parent.width
                    height: 20
                    color: zipArchiver.isError ? Colors.destructiveColor : Colors.artworkActiveColor
                    value: zipArchiver.percent
                }

                RowLayout {
                    height: 24

                    Item {
                        Layout.fillWidth: true
                    }

                    StyledButton {
                        id: importButton
                        width: 130
                        text: qsTr("Start Zipping")
                        enabled: !zipArchiver.inProgress
                        onClicked: {
                            text = qsTr("Zipping...")
                            zipArchiver.resetModel()
                            zipArchiver.archiveArtworks()
                        }

                        Connections {
                            target: zipArchiver
                            onFinishedProcessing: {
                                importButton.text = qsTr("Start Zipping")

                                if (immediateProcessing) {
                                    if (typeof callbackObject !== "undefined") {
                                        callbackObject.afterZipped()
                                    }

                                    closePopup()
                                }

                                //if (!zipArchiver.isError) {
                                //    closePopup()
                                //}
                            }
                        }
                    }

                    Item {
                        width: 10
                    }

                    StyledButton {
                        text: qsTr("Close")
                        width: 100
                        enabled: !zipArchiver.inProgress
                        onClicked: {
                            closePopup()
                        }
                    }
                }
            }
        }
    }

    Component.onCompleted: {
        if (immediateProcessing) {
            zipArchiver.resetModel()
            zipArchiver.archiveArtworks()
        }
    }
}
