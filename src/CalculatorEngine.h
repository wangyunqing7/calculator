#ifndef CALCULATORENGINE_H
#define CALCULATORENGINE_H

#include <QObject>
#include <QString>
#include <QVector>

struct Token {
    enum Type { Number, Operator, LeftParen, RightParen };
    Type type;
    QString value;
};

class CalculatorEngine : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString display READ display NOTIFY displayChanged)
    Q_PROPERTY(QString expression READ expression NOTIFY expressionChanged)

public:
    explicit CalculatorEngine(QObject *parent = nullptr);

    QString display() const;
    QString expression() const;

public slots:
    void inputDigit(const QString &digit);
    void inputOperator(const QString &op);
    void inputDecimal();
    void inputParen(const QString &paren);
    void calculate();
    void clear();
    void clearEntry();
    void backspace();
    void toggleSign();
    void percentage();

signals:
    void displayChanged();
    void expressionChanged();

private:
    QString formatNumber(double value) const;
    void commitCurrentNumber();
    void updateExpressionDisplay();
    double evaluate(const QVector<Token> &tokens, bool &ok) const;

    // Shunting-yard internals
    int precedence(const QString &op) const;
    bool isLeftAssociative(const QString &op) const;
    double applyOp(const QString &op, double a, double b, bool &ok) const;

    QString m_display;
    QString m_expression;

    QVector<Token> m_tokens;
    QString m_currentNumber;
    bool m_hasError = false;
    bool m_justCalculated = false;
};

#endif // CALCULATORENGINE_H
