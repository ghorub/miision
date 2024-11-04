import QtQuick 2.12
import QtQuick.Controls 2.12

TextField {
    property var modelData
    property string commandUuid

    placeholderText: modelData["name"]
    text: modelData["defaultValue"]

    onTextChanged: {
        let minValue = modelData["minValue"];
        let maxValue = modelData["maxValue"];
        let inputValue = parseInt(text, 10);

        if (!isNaN(inputValue) && (minValue === undefined || inputValue >= minValue) && (maxValue === undefined || inputValue <= maxValue)) {
            jsonReader.updateParameterValue(commandUuid, modelData["name"], inputValue);
        } else {
            console.log("Value out of range");
        }
    }
}
