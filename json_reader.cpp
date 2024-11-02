#include "json_reader.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>

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
                    param["defaultValue"] = newValue;
                    parameters[j] = param;
                    command["parameters"] = parameters;
                    m_selectedCommands[i] = command;

                    emit parameterUpdated(commandUuid, parameterName, newValue);  // ارسال سیگنال جدید به جای selectedCommandsChanged
                    qDebug() << "Parameter" << parameterName << "updated for command with UUID" << commandUuid << "to value" << newValue;
                    return;
                }
            }
        }
    }
}
