import QtQuick 2.12
import QtQuick.Controls 2.12

CheckBox {
    property var modelData
    property string commandUuid

    text: modelData["name"]
    checked: modelData["defaultValue"]

    onCheckedChanged: {
        jsonReader.updateParameterValue(commandUuid, modelData["name"], checked)
    }
}
