# Calculator

一款基于 Qt 5 / QML 开发的桌面计算器应用，采用简洁的深色主题 UI，灵感来源于 iOS 计算器。

![C++17](https://img.shields.io/badge/C%2B%2B-17-blue)
![Qt](https://img.shields.io/badge/Qt-5.15-green)
![CMake](https://img.shields.io/badge/CMake-3.16%2B-orange)

## 功能特性

- **基本算术** — 加法、减法、乘法、除法
- **括号** — 支持嵌套表达式，如 `(2 + 3) × 4`
- **百分比** — 快速百分比转换
- **正负号切换** — 在正数和负数之间切换
- **退格 / 清除** — AC (全部清除)、CE (清除当前输入)、以及逐字符退格
- **键盘支持** — 完整键盘输入 (数字、运算符、回车、退格键、Escape)
- **表达式显示** — 在结果上方显示完整表达式
- **浮点精度** — 通过四舍五入正确处理 `0.1 + 0.2` 的精度问题
- **除零处理** — 优雅的错误处理

## 架构设计

```
main.cpp                  # 应用入口，将 CalculatorEngine 注册为 QML 上下文属性
Calculator.qml            # UI 布局 — 深色主题，响应式 CalcButton 按钮网格
src/
  CalculatorEngine.h      # Q_OBJECT，包含 display/expression 属性和 public slots
  CalculatorEngine.cpp    # Tokenizer + Shunting-yard 算法实现表达式求值
```

计算器引擎采用 **tokenizer → Shunting-yard → RPN 栈求值** 流水线，支持运算符优先级和括号，无须使用 `eval()`。

## 环境要求

- **Qt 5.15+** (模块: `Quick`, `Widgets`)
- **CMake 3.16+**
- 支持 C++17 的编译器 (GCC 7+、Clang 5+、MSVC 2017+)

## 编译

```bash
mkdir -p build && cd build
cmake ..
make -j$(nproc)
```

## 运行

```bash
./build/calculator_app
```

## 键盘快捷键

| 按键 | 功能 |
|------|------|
| `0`–`9` | 输入数字 |
| `+` `-` `*` `/` | 运算符 |
| `.` | 小数点 |
| `Enter` / `=` | 计算结果 |
| `Backspace` | 删除最后一个字符 |
| `Escape` | 全部清除 (AC) |
| `Delete` | 清除当前输入 (CE) |
| `%` | 百分比 |
| `(` `)` | 括号 |

## 许可证

MIT
