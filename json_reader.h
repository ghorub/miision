#ifndef JSON_READER_H
#define JSON_READER_H

#include <QObject>
#include <QVariant>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

class JsonReader : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariantList commands READ commands NOTIFY commandsChanged)

public:
    explicit JsonReader(QObject *parent = nullptr);

    bool loadJson(const QString &filePath);
    QVariantList commands() const;

signals:
    void commandsChanged();
    void parameterUpdated(const QString &paramName, const QVariant &value);

public slots:
    void updateParameterValue(const QString &paramName, const QVariant &value);

private:
    QVariantList m_commands;
};


#endif // JSON_READER_H
