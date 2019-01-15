
import QtQuick 2.1
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1
import QtQuick.Controls.Styles 1.1
import MovieMator.Controls 1.0

Item {
    id: background
    width: 300
    height: 250

    SystemPalette { id: activePalette }

    Component.onCompleted: {
        if (filter.isNew) {
            // Set default parameter values
            filter.set('0', 0)
            filter.set('1', 0)
            filter.set('2', 0)
            filter.set('wetness', 1.0)
            filter.savePreset(preset.parameters)
        }
        setControls()
    }

    function setControls() {
        sliderLow.value = filter.getDouble('0')
        sliderMid.value = filter.getDouble('1')
        sliderHigh.value = filter.getDouble('2')
    }

    Rectangle {
        id: topLine
        height: 1
        width: lowColumn.width * 3 + 20
        color: activePalette.text
        opacity: 0.5

        anchors {
            left: gridLayout.left
            leftMargin: gridLayout.leftMargin
            top: gridLayout.top
            topMargin: presetLayout.height + gridLayout.anchors.margins
        }

    }

    Label {
        text: '+12 dB'
        color: '#ffffff'
        font.pointSize: bassLabel.font.pointSize - 1
        anchors {
            right: topLine.left
            rightMargin: 8
            verticalCenter: topLine.verticalCenter
        }
    }

    Rectangle {
        height: 1
        width: lowColumn.width * 3 + gridLayout.anchors.margins * 2
        color: activePalette.text
        opacity: 0.5

        anchors {
            left: gridLayout.left
            leftMargin: gridLayout.leftMargin
            top: topLine.top
            topMargin: sliderLow.height * 0.25 + 1
        }

    }

    Rectangle {
        id: zeroLine
        height: 2
        width: lowColumn.width * 3 + gridLayout.anchors.margins * 2
        color: activePalette.text
        opacity: 0.5

        anchors {
            left: gridLayout.left
            leftMargin: gridLayout.leftMargin
            top: topLine.top
            topMargin: sliderLow.height * 0.5 - 4
        }

    }

    Label {
        text: '0 dB'
        color: '#ffffff'
        font.pointSize: bassLabel.font.pointSize - 1
        anchors {
            right: zeroLine.left
            rightMargin: 8
            verticalCenter: zeroLine.verticalCenter
        }
    }

    Rectangle {
        height: 1
        width: lowColumn.width * 3 + gridLayout.anchors.margins * 2
        color: activePalette.text
        opacity: 0.5

        anchors {
            left: gridLayout.left
            leftMargin: gridLayout.leftMargin
            top: topLine.top
            topMargin: sliderLow.height * 0.75 - 8
        }

    }

    Rectangle {
        id: bottomLine
        height: 1
        width: lowColumn.width * 3 + gridLayout.anchors.margins * 2
        color: activePalette.text
        opacity: 0.5

        anchors {
            left: gridLayout.left
            leftMargin: gridLayout.leftMargin
            bottom: gridLayout.bottom
            bottomMargin: bassLabel.height + gridLayout.anchors.margins
        }

    }

    Label {
        text: '-12 dB'
        color: '#ffffff'
        font.pointSize: bassLabel.font.pointSize - 1
        anchors {
            right: bottomLine.left
            rightMargin: 8
            verticalCenter: bottomLine.verticalCenter
        }
    }

    GridLayout {
        id: gridLayout
        anchors.fill: parent
        anchors.margins: 8
        columns: 5
        property double leftMargin: (width - lowColumn.width * 3 - columnSpacing * 2) / 2

        RowLayout {
            id: presetLayout
            spacing: 8
            Layout.columnSpan: parent.columns
            Label {
                text: qsTr('Preset')
                Layout.alignment: Qt.AlignRight
                color: '#ffffff'
            }
            Preset {
                id: preset
                parameters: ['0', '1', '2']
                onPresetSelected: setControls()
            }
        }

        Item {
            Layout.fillWidth: true
        }

        ColumnLayout {
            id: lowColumn
            Layout.minimumWidth: 80
            Layout.alignment: Qt.AlignHCenter

            Slider {
                id: sliderLow
                anchors.horizontalCenter: parent.horizontalCenter
                Layout.fillHeight: true
                orientation: Qt.Vertical
                minimumValue: -12
                maximumValue: 12
                value: filter.getDouble('0')
                onValueChanged: filter.anim_set("0", "0=-12;200=12;400=0")//filter.set('0', value)
                ToolTip { text: '%1 dB'.arg(Math.round(parent.value * 10) / 10) }
            }
            Label {
                id: bassLabel
                text: qsTr('Bass')
                anchors.horizontalCenter: parent.horizontalCenter
                color: '#ffffff'
            }
        }

        ColumnLayout {
            Layout.minimumWidth: 80
            Layout.alignment: Qt.AlignHCenter

            Slider {
                id: sliderMid
                anchors.horizontalCenter: parent.horizontalCenter
                Layout.fillHeight: true
                orientation: Qt.Vertical
                minimumValue: -12
                maximumValue: 12
                value: filter.getDouble('1')
                onValueChanged: filter.set('1', value)
                ToolTip { text: '%1 dB'.arg(Math.round(parent.value * 10) / 10) }
            }
            Label {
                text: qsTr('Middle')
                anchors.horizontalCenter: parent.horizontalCenter
                color: '#ffffff'
            }
        }

        ColumnLayout {
            Layout.minimumWidth: 80
            Layout.alignment: Qt.AlignHCenter

            Slider {
                id: sliderHigh
                anchors.horizontalCenter: parent.horizontalCenter
                Layout.fillHeight: true
                orientation: Qt.Vertical
                minimumValue: -12
                maximumValue: 12
                value: filter.getDouble('2')
                onValueChanged: filter.set('2', value)
                ToolTip { text: '%1 dB'.arg(Math.round(parent.value * 10) / 10) }
            }
            Label {
                text: qsTr('Treble')
                anchors.horizontalCenter: parent.horizontalCenter
                color: '#ffffff'
            }
        }

        Item {
            Layout.fillWidth: true
        }
    }
}
