

import QtQuick 2.1
import QtQuick.Controls 1.0
import QtQuick.Layouts 1.0
import MovieMator.Controls 1.0

Item {
    width: 300
    height: 250
    property string gainParameter: 'gain'


    function setKeyFrameValue(bKeyFrame)
    {
         var nFrame = keyFrame.getCurrentFrame();
         var sliderValue = fromDb(slider.value);

         if(bKeyFrame)
         {
             filter.setKeyFrameParaValue(nFrame, gainParameter, sliderValue.toString() )

             filter.combineAllKeyFramePara();
         }
         else
         {
             //Todo, delete the keyframe date of the currentframe
             filter.removeKeyFrameParaValue(nFrame);

             filter.set(gainParameter, sliderValue);


         }
     }
    Component.onCompleted: {
        if (filter.isNew) {
            // Set default parameter values
            filter.set(gainParameter, 1.0)
            var keyFrameCount = filter.getKeyFrameCountOnProject("anim-gain");
            var index=0;
            for(index=0; index<keyFrameCount;index++)
            {
                 var nFrame = filter.getKeyFrameOnProjectOnIndex(index, "anim-gain");
                 var keyValue = filter.getKeyValueOnProjectOnIndex(index, "anim-gain");
                 var sliderValue = toDb(keyValue);
                 filter.setKeyFrameParaValue(nFrame, gainParameter, sliderValue.toString() );
            }

            filter.combineAllKeyFramePara();

            slider.value = toDb(filter.getKeyValueOnProjectOnIndex(0, "anim-gain"));
        }
    }

    function toDb(value) {
        return 20 * Math.log(value) / Math.LN10
    }

    function fromDb(value) {
        return Math.pow(10, value / 20);
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 8

        KeyFrame{
             id: keyFrame
             Layout.columnSpan:8
               // 	currentPosition: filterDock.getCurrentPosition()
             onSetAsKeyFrame:
             {
                  setKeyFrameValue(bKeyFrame)
             }

             onLoadKeyFrame:
             {
                  var sliderValue = filter.getKeyFrameParaDoubleValue(keyFrameNum, gainParameter);
                  if(gammaKeyValue != -1.0)
                  {
                       slider.value = toDb(sliderValue)
                  }
             }
        }

        RowLayout {
            Label {
                text: qsTr('Gain')
                color: '#ffffff'
            }
            SliderSpinner {
                id: slider
                minimumValue: -50
                maximumValue: 24
                suffix: 'dB'
                decimals: 1
                spinnerWidth: 80
                value: toDb(filter.getDouble(gainParameter))
                onValueChanged: {
                    setKeyFrameValue(keyFrame.bKeyFrame)
                   // filter.anim_set(gainParameter, "0=-50.0;200=24.0;400=0.0")//filter.set(gainParameter, fromDb(value))
                }
            }
            UndoButton {
                onClicked: {
                    slider.value = 0
                    setKeyFrameValue(keyFrame.bKeyFrame)
                }
            }
        }
        Item {
            Layout.fillHeight: true;
        }
    }
}
