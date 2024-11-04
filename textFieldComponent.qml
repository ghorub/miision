import QtQuick 2.12
import QtQuick.Controls 2.12

TextField {
    property var modelData
    property string commandUuid

    placeholderText: modelData["name"]
    text: modelData["defaultValue"]

    // خصوصیت برای تعیین رنگ پس‌زمینه فیلد بر اساس اعتبار مقدار
    background: Rectangle {
        color: isValid ? "white" : "lightcoral"
        border.color: "gray"
        radius: 4
    }

    // خصوصیت کمکی برای بررسی اعتبار مقدار
    property bool isValid: {
        let minValue = modelData["minValue"] !== undefined ? modelData["minValue"] : Number.MIN_SAFE_INTEGER;
        let maxValue = modelData["maxValue"] !== undefined ? modelData["maxValue"] : Number.MAX_SAFE_INTEGER;
        let intValue = parseInt(text, 10);

        // بررسی اینکه مقدار ورودی عددی است و در محدوده معتبر قرار دارد
        return !isNaN(intValue) && intValue >= minValue && intValue <= maxValue;
    }

    // بررسی مقدار ورودی و ارسال به backend فقط در صورت معتبر بودن
    onTextChanged: {
        console.log(isValid)
        if (isValid) {
            jsonReader.updateParameterValue(commandUuid, modelData["name"], parseInt(text, 10));
        }
    }

    // جلوگیری از وارد کردن کاراکترهای غیرعددی توسط کاربر
    inputMethodHints: Qt.ImhFormattedNumbersOnly
}
