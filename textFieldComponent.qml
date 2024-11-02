import QtQuick 2.12
import QtQuick.Controls 2.12

TextField {
    property var modelData
    property string commandUuid

    placeholderText: modelData["name"]
    text: modelData["defaultValue"]

    onTextChanged: {
        jsonReader.updateParameterValue(commandUuid, modelData["name"], text)
    }
}
