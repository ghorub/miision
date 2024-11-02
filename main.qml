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
                    Loader {
                        active: true
                        sourceComponent: {
                            var paramType = modelData["type"];
                            if (paramType === "textField") {
                                textFieldComponent
                            } else if (paramType === "comboBox") {
                                comboBoxComponent
                            } else if (paramType === "checkBox") {
                                checkBoxComponent
                            } else {
                                null
                            }
                        }

                        onLoaded: {
                            if (item) {
                                item.modelData = modelData;
                                item.commandUuid = uuid;
                                item.commandUuid = selectedCommands[index]["uuid"]; // UUID کامند سطح بالاتر
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

    Component {
        id: textFieldComponent
        TextField {
            property var modelData
            property string commandUuid // تعریف commandUuid به‌عنوان خاصیت

            placeholderText: modelData["name"]
            text: modelData["defaultValue"]

            onTextChanged: {
                jsonReader.updateParameterValue(commandUuid, modelData["name"], text)
            }
        }
    }

    Component {
        id: comboBoxComponent
        ComboBox {
            property var modelData
            property string commandUuid

            model: modelData["options"]
            currentIndex: modelData["options"].indexOf(modelData["defaultValue"])

            onActivated: {
                jsonReader.updateParameterValue(commandUuid, modelData["name"], currentText)
            }
        }
    }

    Component {
        id: checkBoxComponent
        CheckBox {
            property var modelData
            property string commandUuid

            text: modelData["name"]
            checked: modelData["defaultValue"]

            onCheckedChanged: {
                jsonReader.updateParameterValue(commandUuid, modelData["name"], checked)
            }
        }
    }
}
