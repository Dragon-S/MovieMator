
import QtQuick 2.1
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.0
import MovieMator.Controls 1.0

Item {
    width: 300
    height: 250
    Component.onCompleted: {
        if (filter.isNew) {
            filter.set('resource', filter.path + 'ruttetraizer.html')
            // Set default parameter values
            filter.set('opacity', 1.0)
            filter.set('thickness', 3.0)
            filter.set('density', 10)
            filter.set('depth', 100)
            filter.set('scale', 3.0)
            filter.set('rotation_x', 0)
            filter.set('rotation_y', 0)
            filter.savePreset(preset.parameters)

            setControls();
        }
        filter.set('in', filter.producerIn)
        filter.set('out', filter.producerOut)
    }

    function setControls() {
        opacitySlider.value = filter.getDouble('opacity') * opacitySlider.maximumValue
        thicknessSlider.value = filter.getDouble('thickness')
        densitySlider.value = filter.getDouble('density')
        depthSlider.value = filter.getDouble('depth')
        scaleSlider.value = filter.getDouble('scale') / 3.0
        xRotationSlider.value = filter.getDouble('rotation_x') * xRotationSlider.maximumValue
        yRotationSlider.value = filter.getDouble('rotation_y') * yRotationSlider.maximumValue
    }

    GridLayout {
        columns: 3
        anchors.fill: parent
        anchors.margins: 8

        Label {
            text: qsTr('Preset')
            Layout.alignment: Qt.AlignRight
            color: '#ffffff'
        }
        Preset {
            id: preset
            parameters: ['opacity', 'thickness', 'density', 'depth', 'scale',
                'rotation_x', 'rotation_y']
            Layout.columnSpan: 2
            onPresetSelected: setControls()
        }

        Label {
            text: qsTr('Brightness')
            Layout.alignment: Qt.AlignRight
            color: '#ffffff'
        }
        SliderSpinner {
            id: opacitySlider
            minimumValue: 1
            maximumValue: 100
            decimals: 1
            suffix: ' %'
            value: filter.getDouble('opacity') * maximumValue
            onValueChanged: filter.set('opacity', value / maximumValue)
        }
        UndoButton {
            onClicked: opacitySlider.value = 100
        }

        Label {
            text: qsTr('Thickness')
            Layout.alignment: Qt.AlignRight
            color: '#ffffff'
        }
        SliderSpinner {
            id: thicknessSlider
            minimumValue: 1
            maximumValue: 10
            decimals: 1
            value: filter.getDouble('thickness')
            onValueChanged: filter.set('thickness', value)
        }
        UndoButton {
            onClicked: thicknessSlider.value = 3.0
        }

        Label {
            text: qsTr('Density')
            Layout.alignment: Qt.AlignRight
            color: '#ffffff'
        }
        SliderSpinner {
            id: densitySlider
            minimumValue: 1
            maximumValue: 100
            spinnerWidth: 110
            suffix: ' pixels'
            value: filter.getDouble('density')
            onValueChanged: filter.set('density', value)
        }
        UndoButton {
            onClicked: densitySlider.value = 10
        }

        Label {
            text: qsTr('Depth')
            Layout.alignment: Qt.AlignRight
            color: '#ffffff'
        }
        SliderSpinner {
            id: depthSlider
            minimumValue: 1
            maximumValue: 500
            spinnerWidth: 110
            suffix: ' pixels'
            value: filter.getDouble('depth')
            onValueChanged: filter.set('depth', value)
        }
        UndoButton {
            onClicked: depthSlider.value = 100
        }

        Label {
            text: qsTr('Scale')
            Layout.alignment: Qt.AlignRight
            color: '#ffffff'
        }
        SliderSpinner {
            id: scaleSlider
            minimumValue: 0.01
            maximumValue: 3.0
            decimals: 2
            value: filter.getDouble('scale') / 3.0
            onValueChanged: filter.set('scale', value * 3.0)
        }
        UndoButton {
            onClicked: scaleSlider.value = 1.0
        }

        Label {
            text: qsTr('X Axis Rotation')
            Layout.alignment: Qt.AlignRight
            color: '#ffffff'
        }
        SliderSpinner {
            id: xRotationSlider
            minimumValue: 0
            maximumValue: 360
            spinnerWidth: 110
            suffix: ' degree'
            value: filter.getDouble('rotation_x') * maximumValue
            onValueChanged: filter.set('rotation_x', value / maximumValue)
        }
        UndoButton {
            onClicked: xRotationSlider.value = 0
        }

        Label {
            text: qsTr('Y Axis Rotation')
            Layout.alignment: Qt.AlignRight
            color: '#ffffff'
        }
        SliderSpinner {
            id: yRotationSlider
            minimumValue: 0
            maximumValue: 360
            spinnerWidth: 110
            suffix: ' degree'
            value: filter.getDouble('rotation_y') * maximumValue
            onValueChanged: filter.set('rotation_y', value / maximumValue)
        }
        UndoButton {
            onClicked: yRotationSlider.value = 0
        }

        Item {
            Layout.fillHeight: true
        }
    }
}
