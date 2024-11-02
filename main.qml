import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3

ApplicationWindow {
    visible: true
    width: 640
    height: 480
    title: "Dynamic Form Example"

    property var selectedCommands: jsonReader.selectedCommands  // ارتباط Q_PROPERTY با QML

    Column {
        width: parent.width
        spacing: 10

        Row {
            spacing: 10
            Button {
                text: "Load JSON Template"
                onClicked: jsonReader.loadJson("path/to/your/jsonfile.json")  // مسیر فایل JSON
            }
            Button {
                text: "Add Command"
                onClicked: commandSelectionDialog.open()  // باز کردن دیالوگ انتخاب دستورات
            }
        }

        // نمایش هر کامند به صورت فرم جداگانه
        Repeater {
            model: selectedCommands
            Column {
                width: parent.width
                spacing: 5

                Text {
                    text: modelData["name"] + " (" + modelData["commandId"] + ")"
                    font.bold: true
                    font.pointSize: 16
                }

                // نمایش هر پارامتر به صورت ورودی فرم
                Repeater {
                    model: modelData["parameters"];

                    id:selectedCommandsId
                    property int seIndex: index
                    Loader {
                        active: true
                        sourceComponent: Qt.createComponent(modelData["type"] + "Component.qml")

                        onLoaded: {
                            if (item) {
                                item.modelData = modelData;
                                item.commandUuid = selectedCommands[selectedCommandsId.seIndex]["uuid"]; // UUID کامند سطح بالاتر
                            }
                        }

//                        Component.onCompleted: {
//                            // بررسی و تنظیم دوباره commandUuid برای اطمینان از مقداردهی صحیح
//                            if (item && !item.commandUuid) {
//                                item.commandUuid = selectedCommands[index]["uuid"];
//                            }
//                        }
                    }
                }
            }
        }
    }

    Dialog {
        id: commandSelectionDialog
        title: "Select Commands"
        modal: true
        width: 400
        height: 300

        Column {
            spacing: 5
            width: parent.width

            Repeater {
                model: jsonReader.commands

                Row {
                    spacing: 10

                    Button {
                        text: modelData["name"]
                        onClicked: {
                            jsonReader.addCommand(modelData)  // افزودن کامند به لیست سمت C++
                        }
                    }
                }
            }
        }

        footer: Button {
            text: "Done"
            onClicked: commandSelectionDialog.close()
        }
    }
    Connections {
        target: jsonReader

        onParameterUpdated: function(commandUuid, parameterName, newValue) {
            console.log("Parameter", parameterName, "updated for command with UUID", commandUuid, "to value", newValue)
        }
    }

}
