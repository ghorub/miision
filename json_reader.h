#ifndef JSONREADER_H
#define JSONREADER_H

#include <QObject>
#include <QVariantList>
#include <QUuid>

class JsonReader : public QObject {
    Q_OBJECT
    Q_PROPERTY(QVariantList commands READ getCommands NOTIFY commandsChanged)
    Q_PROPERTY(QVariantList selectedCommands READ getSelectedCommands NOTIFY selectedCommandsChanged)

public:
    explicit JsonReader(QObject *parent = nullptr);

    QVariantList getCommands() const;
    QVariantList getSelectedCommands() const;
    Q_INVOKABLE void loadJson(const QString &filePath);
    Q_INVOKABLE void addCommand(const QVariantMap &command);
    Q_INVOKABLE void updateParameterValue(const QString &commandUuid, const QString &parameterName, const QVariant &newValue);

    Q_INVOKABLE void saveFormDataAsBinary(const QString &filePath);
    Q_INVOKABLE void saveFormDataAsJson(const QString &filePath);
    Q_INVOKABLE void loadFormDataFromJson(const QString &filePath);

signals:
    void commandsChanged();
    void selectedCommandsChanged();
    void parameterUpdated(const QString &commandUuid, const QString &parameterName, const QVariant &newValue);  // سیگنال جدید

private:
    QVariantList m_commands;
    QVariantList m_selectedCommands;
};

#endif // JSONREADER_H
