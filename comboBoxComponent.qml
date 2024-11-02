import QtQuick 2.12
import QtQuick.Controls 2.12

ComboBox {
    property var modelData
    property string commandUuid

    model: modelData["options"]
    currentIndex: modelData["options"].indexOf(modelData["defaultValue"])

    onActivated: {
        jsonReader.updateParameterValue(commandUuid, modelData["name"], currentText)
    }
}
