#include "json_reader.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <QDataStream>
JsonReader::JsonReader(QObject *parent) : QObject(parent) {}

QVariantList JsonReader::getCommands() const {
    return m_commands;
}

QVariantList JsonReader::getSelectedCommands() const {
    return m_selectedCommands;
}

void JsonReader::loadJson(const QString &filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Failed to open JSON file:" << filePath;
        return;
    }

    QByteArray data = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (!doc.isObject()) {
        qWarning() << "Invalid JSON format in file:" << filePath;
        return;
    }

    QJsonObject rootObj = doc.object();
    QJsonArray commandsArray = rootObj["commands"].toArray();

    m_commands.clear();
    for (const QJsonValue &commandVal : commandsArray) {
        QJsonObject commandObj = commandVal.toObject();
        QVariantMap command;
        command["name"] = commandObj["name"].toString();
        command["commandId"] = commandObj["commandId"].toInt();
        command["category"] = commandObj["category"].toString();

        QVariantList parameters;
        for (const QJsonValue &paramVal : commandObj["parameters"].toArray()) {
            QJsonObject paramObj = paramVal.toObject();
            QVariantMap parameter;
            parameter["name"] = paramObj["name"].toString();
            parameter["type"] = paramObj["type"].toString();
            parameter["defaultValue"] = paramObj["defaultValue"].toVariant();
            if (paramObj.contains("minValue")) {
                parameter["minValue"] = paramObj["minValue"].toVariant();
            }
            if (paramObj.contains("maxValue")) {
                parameter["maxValue"] = paramObj["maxValue"].toVariant();
            }
            if (paramObj.contains("options")) {
                parameter["options"] = paramObj["options"].toVariant();
            }
            parameters.append(parameter);
        }

        command["parameters"] = parameters;
        m_commands.append(command);
    }

    emit commandsChanged();
}

void JsonReader::addCommand(const QVariantMap &command) {
    QVariantMap commandWithUuid = command;
    commandWithUuid["uuid"] = QUuid::createUuid().toString();  // اضافه کردن UUID به کامند
    //qDebug() <<"command"<<command;
    qDebug() <<"uuid"<<commandWithUuid["uuid"];
    //qDebug() <<"commandWithUuid"<<commandWithUuid;
    m_selectedCommands.append(commandWithUuid);
   // qDebug() <<"m_selectedCommands"<<m_selectedCommands;
    emit selectedCommandsChanged();
}

void JsonReader::updateParameterValue(const QString &commandUuid, const QString &parameterName, const QVariant &newValue) {
    for (int i = 0; i < m_selectedCommands.size(); ++i) {
        QVariantMap command = m_selectedCommands[i].toMap();

        if (command["uuid"].toString() == commandUuid) {
            QVariantList parameters = command["parameters"].toList();

            for (int j = 0; j < parameters.size(); ++j) {
                QVariantMap param = parameters[j].toMap();

                if (param["name"].toString() == parameterName) {
                    bool isInRange = true;

                    if (param.contains("minValue") && param.contains("maxValue")) {
                        int minValue = param["minValue"].toInt();
                        int maxValue = param["maxValue"].toInt();
                        int newVal = newValue.toInt();

                        if (newVal < minValue || newVal > maxValue) {
                            qWarning() << "Value" << newVal << "out of range for parameter" << parameterName;
                            isInRange = false;
                        }
                    }

                    if (isInRange) {
                        param["defaultValue"] = newValue;
                        parameters[j] = param;
                        command["parameters"] = parameters;
                        m_selectedCommands[i] = command;

                        emit parameterUpdated(commandUuid, parameterName, newValue);
                        qDebug() << "Parameter" << parameterName << "updated for command with UUID" << commandUuid << "to value" << newValue;
                    }
                    return;
                }
            }
        }
    }
}

void JsonReader::saveFormDataAsJson(const QString &filePath) {
    QJsonArray commandsArray;

    for (const auto &commandVariant : m_selectedCommands) {
        QVariantMap command = commandVariant.toMap();
        QJsonObject commandObj;
        commandObj["commandId"] = command["commandId"].toString();
        commandObj["name"] = command["name"].toString();

        QJsonArray paramsArray;
        QVariantList parameters = command["parameters"].toList();
        for (const auto &paramVariant : parameters) {
            QVariantMap param = paramVariant.toMap();
            QJsonObject paramObj;
            paramObj["name"] = param["name"].toString();
            paramObj["value"] = QJsonValue::fromVariant(param["defaultValue"]);
            paramsArray.append(paramObj);
        }

        commandObj["parameters"] = paramsArray;
        commandsArray.append(commandObj);
    }

    QJsonDocument doc(commandsArray);
    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly)) {
        file.write(doc.toJson());
        file.close();
    }
}
void JsonReader::saveFormDataAsBinary(const QString &filePath) {
    QVector<qint32> valuesArray;

    for (const auto &commandVariant : m_selectedCommands) {
        QVariantMap command = commandVariant.toMap();
        QVariantList parameters = command["parameters"].toList();
        for (const auto &paramVariant : parameters) {
            QVariantMap param = paramVariant.toMap();
            QVariant value = param["defaultValue"];
            if (value.canConvert<qint32>()) {
                valuesArray.append(value.toInt());
            }
        }
    }

    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly)) {
        QDataStream out(&file);
        for (qint32 val : valuesArray) {
            out << val;
        }
        file.close();
    }
}
void JsonReader::loadFormDataFromJson(const QString &filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        return;
    }

    QByteArray jsonData = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(jsonData);
    if (!doc.isArray()) {
        return;
    }

    m_selectedCommands.clear();
    QJsonArray commandsArray = doc.array();
    for (const QJsonValue &commandValue : commandsArray) {
        QVariantMap command = commandValue.toObject().toVariantMap();
        m_selectedCommands.append(command);
    }

    emit selectedCommandsChanged();
}
