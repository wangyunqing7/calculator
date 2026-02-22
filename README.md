# Calculator

A desktop calculator application built with Qt 5 / QML, featuring a clean dark-themed UI inspired by iOS Calculator.

![C++17](https://img.shields.io/badge/C%2B%2B-17-blue)
![Qt](https://img.shields.io/badge/Qt-5.15-green)
![CMake](https://img.shields.io/badge/CMake-3.16%2B-orange)

## Features

- **Basic Arithmetic** — Addition, subtraction, multiplication, division
- **Parentheses** — Supports nested expressions like `(2 + 3) × 4`
- **Percentage** — Quick percent conversion
- **Sign Toggle** — Switch between positive and negative
- **Backspace / Clear** — AC (all clear), CE (clear entry), and per-character backspace
- **Keyboard Support** — Full keyboard input (digits, operators, Enter, Escape, Backspace)
- **Expression Display** — Shows the full expression above the result
- **Floating Point Precision** — Handles `0.1 + 0.2` correctly via rounding
- **Division by Zero** — Graceful error handling

## Architecture

```
main.cpp                  # App entry, registers CalculatorEngine as QML context property
Calculator.qml            # UI layout — dark theme, responsive grid of CalcButton components
src/
  CalculatorEngine.h      # Q_OBJECT with display/expression properties and public slots
  CalculatorEngine.cpp    # Tokenizer + Shunting-yard algorithm for expression evaluation
```

The engine uses a **tokenizer → Shunting-yard → RPN stack evaluator** pipeline, supporting operator precedence and parentheses without `eval()`.

## Prerequisites

- **Qt 5.15+** (modules: `Quick`, `Widgets`)
- **CMake 3.16+**
- A C++17-compatible compiler (GCC 7+, Clang 5+, MSVC 2017+)

## Build

```bash
mkdir -p build && cd build
cmake ..
make -j$(nproc)
```

## Run

```bash
./build/calculator_app
```

## Keyboard Shortcuts

| Key | Action |
|-----|--------|
| `0`–`9` | Input digit |
| `+` `-` `*` `/` | Operators |
| `.` | Decimal point |
| `Enter` / `=` | Calculate |
| `Backspace` | Delete last character |
| `Escape` | Clear all (AC) |
| `Delete` | Clear entry (CE) |
| `%` | Percentage |
| `(` `)` | Parentheses |

## License

MIT
