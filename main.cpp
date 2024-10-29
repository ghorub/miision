#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "json_reader.h"
int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);


    JsonReader jsonReader;
    jsonReader.loadJson(":/commands.json");  // فرض بر اینکه JSON در مسیر ریسورس قرار دارد


    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("jsonReader", &jsonReader);


    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
