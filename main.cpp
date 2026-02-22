#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "src/CalculatorEngine.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    CalculatorEngine calculator;

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("calculator", &calculator);

    const QUrl url(QStringLiteral("qrc:/Calculator.qml"));

    engine.load(url);

    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
