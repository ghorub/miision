#include "json_reader.h"
#include "QtCore"

JsonReader::JsonReader(QObject *parent) : QObject(parent) {}

bool JsonReader::loadJson(const QString &filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Failed to open file:" << filePath;
        return false;
    }

    QByteArray jsonData = file.readAll();
    file.close();

    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);
    if (!jsonDoc.isObject()) {
        qWarning() << "Invalid JSON format";
        return false;
    }

    QJsonObject rootObj = jsonDoc.object();
    QJsonArray commandsArray = rootObj.value("commands").toArray();

    m_commands.clear();
    for (const QJsonValue &value : commandsArray) {
        m_commands.append(value.toObject().toVariantMap());
    }

    emit commandsChanged();
    return true;
}

QVariantList JsonReader::commands() const {
    return m_commands;
}
void JsonReader::updateParameterValue(const QString &paramName, const QVariant &value) {
    // در اینجا می‌توانید کد لازم برای بروزرسانی مقدار پارامتر را بنویسید
    qDebug() << "Parameter updated:" << paramName << "Value:" << value;

    // ارسال سیگنال برای اطلاع از تغییرات
    emit parameterUpdated(paramName, value);
}
