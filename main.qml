import QtQuick 2.12
import QtQuick.Controls 2.12

ApplicationWindow {
    visible: true
    width: 640
    height: 480
    title: "Dynamic Form Example"

    Column {
        width: parent.width
        spacing: 10

        // نمایش هر کامند به صورت فرم جداگانه
        Repeater {
            model: jsonReader.commands

            Column {
                width: parent.width
                spacing: 5

                Text {
                    text: modelData["name"]
                    font.bold: true
                    font.pointSize: 16
                }

                // نمایش هر پارامتر به صورت ورودی فرم
                Repeater {
                    model: modelData["parameters"]

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

                        // ارسال مدل به کامپوننت فرزند
                        onLoaded: {
                            if (item) {
                                item.modelData = modelData
                            }
                        }
                    }
                }
            }
        }
    }

    // تعریف هر نوع از کامپوننت‌ها با استفاده از modelData
    Component {
        id: textFieldComponent
        TextField {
            property var modelData // تعریف مدل داده برای دسترسی به مدل فرزند
            placeholderText: modelData["name"]
            text: modelData["defaultValue"]

            // ارسال مقدار جدید به C++ با استفاده از updateParameterValue
            onTextChanged: {
                jsonReader.updateParameterValue(modelData["name"], text)
            }
        }
    }

    Component {
        id: comboBoxComponent
        ComboBox {
            property var modelData // تعریف مدل داده برای دسترسی به مدل فرزند
            model: modelData["options"]
            currentIndex: modelData["options"].indexOf(modelData["defaultValue"]) // مقدار پیش‌فرض

            // ارسال مقدار جدید به C++ با استفاده از updateParameterValue
            onActivated: {  // استفاده از onActivated برای ارسال مقدار جدید بلافاصله
                jsonReader.updateParameterValue(modelData["name"], currentText)
            }
        }
    }

    Component {
        id: checkBoxComponent
        CheckBox {
            property var modelData // تعریف مدل داده برای دسترسی به مدل فرزند
            text: modelData["name"]
            checked: modelData["defaultValue"]

            // ارسال مقدار جدید به C++ با استفاده از updateParameterValue
            onCheckedChanged: {
                jsonReader.updateParameterValue(modelData["name"], checked)
            }
        }
    }
}
