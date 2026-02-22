#include "CalculatorEngine.h"
#include <cmath>
#include <QStack>

CalculatorEngine::CalculatorEngine(QObject *parent)
    : QObject(parent), m_display("0")
{
}

QString CalculatorEngine::display() const
{
    return m_display;
}

QString CalculatorEngine::expression() const
{
    return m_expression;
}

QString CalculatorEngine::formatNumber(double value) const
{
    if (std::abs(value) >= 1e10 || (std::abs(value) < 1e-6 && value != 0)) {
        return QString::number(value, 'g', 10);
    }

    // e.g. 0.1 + 0.2 = 0.30000000000000004 -> 0.3
    QString formatted = QString::number(value, 'f', 10);
    int dotPos = formatted.indexOf('.');
    if (dotPos != -1) {
        int lastNonZero = formatted.length() - 1;
        while (lastNonZero > dotPos && formatted[lastNonZero] == '0') {
            lastNonZero--;
        }
        if (lastNonZero == dotPos) {
            formatted = formatted.left(dotPos);
        } else {
            formatted = formatted.left(lastNonZero + 1);
        }
    }
    return formatted;
}

void CalculatorEngine::commitCurrentNumber()
{
    if (!m_currentNumber.isEmpty()) {
        m_tokens.append({Token::Number, m_currentNumber});
        m_currentNumber.clear();
    }
}

void CalculatorEngine::updateExpressionDisplay()
{
    QString expr;
    for (const auto &tok : m_tokens) {
        if (!expr.isEmpty() && tok.type != Token::RightParen
            && (expr.isEmpty() || expr.back() != '(')) {
            expr += " ";
        }
        if (tok.type == Token::LeftParen && !expr.isEmpty() && expr.back() != ' ') {
            expr += " ";
        }
        expr += tok.value;
    }
    if (!m_currentNumber.isEmpty()) {
        if (!expr.isEmpty()) expr += " ";
        expr += m_currentNumber;
    }
    m_expression = expr;
    emit expressionChanged();
}

void CalculatorEngine::inputDigit(const QString &digit)
{
    if (m_hasError) {
        clear();
    }

    if (m_justCalculated) {
        m_tokens.clear();
        m_justCalculated = false;
    }

    if (m_currentNumber == "0" && digit != "0") {
        m_currentNumber = digit;
    } else if (m_currentNumber == "0" && digit == "0") {
    } else {
        m_currentNumber += digit;
    }

    m_display = m_currentNumber;
    emit displayChanged();
    updateExpressionDisplay();
}

void CalculatorEngine::inputOperator(const QString &op)
{
    if (m_hasError) {
        clear();
        return;
    }

    m_justCalculated = false;

    if (!m_currentNumber.isEmpty()) {
        commitCurrentNumber();
    } else if (!m_tokens.isEmpty()) {
        auto &last = m_tokens.last();
        if (last.type == Token::Operator) {
            last.value = op;
            updateExpressionDisplay();
            return;
        }
    }

    if (m_tokens.isEmpty()) {
        return;
    }

    m_tokens.append({Token::Operator, op});
    updateExpressionDisplay();
}

void CalculatorEngine::inputDecimal()
{
    if (m_hasError) {
        clear();
    }

    if (m_justCalculated) {
        m_tokens.clear();
        m_justCalculated = false;
    }

    if (m_currentNumber.isEmpty()) {
        m_currentNumber = "0.";
    } else if (!m_currentNumber.contains('.')) {
        m_currentNumber += '.';
    }

    m_display = m_currentNumber;
    emit displayChanged();
    updateExpressionDisplay();
}

void CalculatorEngine::inputParen(const QString &paren)
{
    if (m_hasError) {
        clear();
    }

    if (m_justCalculated) {
        m_tokens.clear();
        m_justCalculated = false;
    }

    if (paren == "(") {
        if (!m_currentNumber.isEmpty()) {
            commitCurrentNumber();
            m_tokens.append({Token::Operator, "×"});
        } else if (!m_tokens.isEmpty()) {
            auto &last = m_tokens.last();
            if (last.type == Token::Number || last.type == Token::RightParen) {
                m_tokens.append({Token::Operator, "×"});
            }
        }
        m_tokens.append({Token::LeftParen, "("});
    } else {
        if (!m_currentNumber.isEmpty()) {
            commitCurrentNumber();
        }

        int openCount = 0;
        for (const auto &t : m_tokens) {
            if (t.type == Token::LeftParen) openCount++;
            else if (t.type == Token::RightParen) openCount--;
        }
        if (openCount <= 0) return;

        if (!m_tokens.isEmpty() && m_tokens.last().type == Token::Operator) {
            return;
        }

        m_tokens.append({Token::RightParen, ")"});
    }

    updateExpressionDisplay();
}

void CalculatorEngine::calculate()
{
    if (m_hasError) return;

    if (!m_currentNumber.isEmpty()) {
        commitCurrentNumber();
    }

    if (m_tokens.isEmpty()) return;

    while (!m_tokens.isEmpty() && m_tokens.last().type == Token::Operator) {
        m_tokens.removeLast();
    }

    int openCount = 0;
    for (const auto &t : m_tokens) {
        if (t.type == Token::LeftParen) openCount++;
        else if (t.type == Token::RightParen) openCount--;
    }
    while (openCount > 0) {
        m_tokens.append({Token::RightParen, ")"});
        openCount--;
    }

    bool ok = true;
    double result = evaluate(m_tokens, ok);

    if (!ok) {
        QString exprStr;
        for (const auto &t : m_tokens) {
            if (!exprStr.isEmpty() && t.type != Token::RightParen
                && (exprStr.isEmpty() || exprStr.back() != '(')) {
                exprStr += " ";
            }
            if (t.type == Token::LeftParen && !exprStr.isEmpty() && exprStr.back() != ' ') {
                exprStr += " ";
            }
            exprStr += t.value;
        }
        m_expression = exprStr + " =";
        emit expressionChanged();
        m_display = "Error";
        m_hasError = true;
        emit displayChanged();
        return;
    }

    double rounded = std::round(result * 1e10) / 1e10;
    QString resultStr = formatNumber(rounded);

    QString exprStr;
    for (const auto &t : m_tokens) {
        if (!exprStr.isEmpty() && t.type != Token::RightParen
            && (exprStr.isEmpty() || exprStr.back() != '(')) {
            exprStr += " ";
        }
        if (t.type == Token::LeftParen && !exprStr.isEmpty() && exprStr.back() != ' ') {
            exprStr += " ";
        }
        exprStr += t.value;
    }
    m_expression = exprStr + " =";
    emit expressionChanged();

    m_display = resultStr;
    emit displayChanged();

    m_tokens.clear();
    m_tokens.append({Token::Number, resultStr});
    m_currentNumber.clear();
    m_justCalculated = true;
}

void CalculatorEngine::clear()
{
    m_display = "0";
    m_expression.clear();
    m_tokens.clear();
    m_currentNumber.clear();
    m_hasError = false;
    m_justCalculated = false;
    emit displayChanged();
    emit expressionChanged();
}

void CalculatorEngine::clearEntry()
{
    if (m_hasError) {
        clear();
        return;
    }
    m_currentNumber.clear();
    m_display = "0";
    emit displayChanged();
    updateExpressionDisplay();
}

void CalculatorEngine::backspace()
{
    if (m_hasError) {
        clear();
        return;
    }

    if (!m_currentNumber.isEmpty()) {
        m_currentNumber.chop(1);
        m_display = m_currentNumber.isEmpty() ? "0" : m_currentNumber;
        emit displayChanged();
        updateExpressionDisplay();
    } else if (!m_tokens.isEmpty()) {
        auto last = m_tokens.last();
        m_tokens.removeLast();
        if (last.type == Token::Number) {
            m_currentNumber = last.value;
            m_currentNumber.chop(1);
            m_display = m_currentNumber.isEmpty() ? "0" : m_currentNumber;
            emit displayChanged();
        }
        updateExpressionDisplay();
    }
}

void CalculatorEngine::toggleSign()
{
    if (m_hasError) return;

    if (m_currentNumber.isEmpty() || m_currentNumber == "0") {
        return;
    }

    if (m_currentNumber.startsWith('-')) {
        m_currentNumber.remove(0, 1);
    } else {
        m_currentNumber.prepend('-');
    }
    m_display = m_currentNumber;
    emit displayChanged();
    updateExpressionDisplay();
}

void CalculatorEngine::percentage()
{
    if (m_hasError) return;

    if (m_currentNumber.isEmpty()) return;

    double value = m_currentNumber.toDouble() / 100.0;
    double rounded = std::round(value * 1e10) / 1e10;
    m_currentNumber = formatNumber(rounded);
    m_display = m_currentNumber;
    emit displayChanged();
    updateExpressionDisplay();
}

int CalculatorEngine::precedence(const QString &op) const
{
    if (op == "+" || op == "-") return 1;
    if (op == "×" || op == "*" || op == "÷" || op == "/") return 2;
    return 0;
}

bool CalculatorEngine::isLeftAssociative(const QString &) const
{
    return true;
}

double CalculatorEngine::applyOp(const QString &op, double a, double b, bool &ok) const
{
    if (op == "+") return a + b;
    if (op == "-") return a - b;
    if (op == "×" || op == "*") return a * b;
    if (op == "÷" || op == "/") {
        if (b == 0) { ok = false; return 0; }
        return a / b;
    }
    ok = false;
    return 0;
}

// Shunting-yard → RPN → stack evaluation
double CalculatorEngine::evaluate(const QVector<Token> &tokens, bool &ok) const
{
    QVector<Token> output;
    QStack<Token> opStack;

    for (const auto &tok : tokens) {
        switch (tok.type) {
        case Token::Number:
            output.append(tok);
            break;
        case Token::Operator:
            while (!opStack.isEmpty()
                   && opStack.top().type == Token::Operator
                   && ((isLeftAssociative(tok.value) && precedence(tok.value) <= precedence(opStack.top().value))
                       || (!isLeftAssociative(tok.value) && precedence(tok.value) < precedence(opStack.top().value)))) {
                output.append(opStack.pop());
            }
            opStack.push(tok);
            break;
        case Token::LeftParen:
            opStack.push(tok);
            break;
        case Token::RightParen:
            while (!opStack.isEmpty() && opStack.top().type != Token::LeftParen) {
                output.append(opStack.pop());
            }
            if (opStack.isEmpty()) { ok = false; return 0; }
            opStack.pop();
            break;
        }
    }

    while (!opStack.isEmpty()) {
        if (opStack.top().type == Token::LeftParen) { ok = false; return 0; }
        output.append(opStack.pop());
    }

    QStack<double> evalStack;
    for (const auto &tok : output) {
        if (tok.type == Token::Number) {
            evalStack.push(tok.value.toDouble());
        } else if (tok.type == Token::Operator) {
            if (evalStack.size() < 2) { ok = false; return 0; }
            double b = evalStack.pop();
            double a = evalStack.pop();
            evalStack.push(applyOp(tok.value, a, b, ok));
            if (!ok) return 0;
        }
    }

    if (evalStack.size() != 1) { ok = false; return 0; }
    return evalStack.top();
}
