import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Window {
    id: root
    visible: true
    width: 320
    height: 500
    minimumWidth: 260
    minimumHeight: 400
    title: "Calculator"
    color: "#1c1c1e"

    Item {
        id: keyHandler
        anchors.fill: parent
        focus: true

        Keys.onPressed: {
            if (event.key >= Qt.Key_0 && event.key <= Qt.Key_9) {
                calculator.inputDigit(String(event.key - Qt.Key_0))
            } else if (event.key === Qt.Key_Plus) {
                calculator.inputOperator("+")
            } else if (event.key === Qt.Key_Minus) {
                calculator.inputOperator("-")
            } else if (event.key === Qt.Key_Asterisk) {
                calculator.inputOperator("×")
            } else if (event.key === Qt.Key_Slash) {
                calculator.inputOperator("÷")
            } else if (event.key === Qt.Key_Return || event.key === Qt.Key_Enter || event.key === Qt.Key_Equal) {
                calculator.calculate()
            } else if (event.key === Qt.Key_Period) {
                calculator.inputDecimal()
            } else if (event.key === Qt.Key_Backspace) {
                calculator.backspace()
            } else if (event.key === Qt.Key_Escape) {
                calculator.clear()
            } else if (event.key === Qt.Key_Delete) {
                calculator.clearEntry()
            } else if (event.key === Qt.Key_Percent) {
                calculator.percentage()
            } else if (event.key === Qt.Key_ParenLeft) {
                calculator.inputParen("(")
            } else if (event.key === Qt.Key_ParenRight) {
                calculator.inputParen(")")
            }
        }
    }

    Column {
        anchors.fill: parent
        anchors.margins: 10
        spacing: 8

        Item {
            width: parent.width
            height: parent.height - buttonGrid.height - 8

            Rectangle {
                anchors.fill: parent
                color: "#2c2c2e"
                radius: 10

                Column {
                    anchors.right: parent.right
                    anchors.left: parent.left
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.margins: 20
                    spacing: 4

                    Text {
                        anchors.right: parent.right
                        text: calculator.expression
                        font.pixelSize: 16
                        color: "#8e8e93"
                        horizontalAlignment: Text.AlignRight
                        elide: Text.ElideLeft
                        width: parent.width
                    }

                    Text {
                        anchors.right: parent.right
                        text: calculator.display
                        font.pixelSize: Math.min(48, root.width / 7)
                        font.bold: true
                        color: "white"
                        horizontalAlignment: Text.AlignRight
                        elide: Text.ElideLeft
                        width: parent.width
                    }
                }
            }
        }

        GridLayout {
            id: buttonGrid
            width: parent.width
            columns: 4
            property real btnWidth: (width - 30) / 4
            property real btnHeight: Math.max(45, Math.min(65, (root.height - 200) / 7))
            columnSpacing: 10
            rowSpacing: 8

            CalcButton { text: "AC"; buttonColor: "#a5a5a5"; onClicked: calculator.clear() }
            CalcButton { text: "CE"; buttonColor: "#a5a5a5"; onClicked: calculator.clearEntry() }
            CalcButton { text: "%"; buttonColor: "#a5a5a5"; onClicked: calculator.percentage() }
            CalcButton { text: "⌫"; buttonColor: "#a5a5a5"; onClicked: calculator.backspace() }

            CalcButton { text: "("; buttonColor: "#a5a5a5"; onClicked: calculator.inputParen("(") }
            CalcButton { text: ")"; buttonColor: "#a5a5a5"; onClicked: calculator.inputParen(")") }
            CalcButton { text: "±"; buttonColor: "#505050"; onClicked: calculator.toggleSign() }
            CalcButton { text: "÷"; buttonColor: "#ff9f0a"; onClicked: calculator.inputOperator("÷") }

            CalcButton { text: "7"; buttonColor: "#505050"; onClicked: calculator.inputDigit("7") }
            CalcButton { text: "8"; buttonColor: "#505050"; onClicked: calculator.inputDigit("8") }
            CalcButton { text: "9"; buttonColor: "#505050"; onClicked: calculator.inputDigit("9") }
            CalcButton { text: "×"; buttonColor: "#ff9f0a"; onClicked: calculator.inputOperator("×") }

            CalcButton { text: "4"; buttonColor: "#505050"; onClicked: calculator.inputDigit("4") }
            CalcButton { text: "5"; buttonColor: "#505050"; onClicked: calculator.inputDigit("5") }
            CalcButton { text: "6"; buttonColor: "#505050"; onClicked: calculator.inputDigit("6") }
            CalcButton { text: "-"; buttonColor: "#ff9f0a"; onClicked: calculator.inputOperator("-") }

            CalcButton { text: "1"; buttonColor: "#505050"; onClicked: calculator.inputDigit("1") }
            CalcButton { text: "2"; buttonColor: "#505050"; onClicked: calculator.inputDigit("2") }
            CalcButton { text: "3"; buttonColor: "#505050"; onClicked: calculator.inputDigit("3") }
            CalcButton { text: "+"; buttonColor: "#ff9f0a"; onClicked: calculator.inputOperator("+") }

            CalcButton { text: "0"; buttonColor: "#505050"; onClicked: calculator.inputDigit("0"); Layout.columnSpan: 2; Layout.fillWidth: true }
            CalcButton { text: "."; buttonColor: "#505050"; onClicked: calculator.inputDecimal() }
            CalcButton { text: "="; buttonColor: "#ff9f0a"; onClicked: calculator.calculate() }
        }
    }

    component CalcButton: Rectangle {
        property string text
        property color buttonColor: "#505050"
        signal clicked()

        Layout.preferredWidth: buttonGrid.btnWidth
        Layout.preferredHeight: buttonGrid.btnHeight
        Layout.fillWidth: true
        radius: 10
        color: mouseArea.pressed ? Qt.lighter(buttonColor, 1.3) : buttonColor

        Text {
            anchors.centerIn: parent
            text: parent.text
            font.pixelSize: Math.min(24, parent.height * 0.38)
            font.bold: true
            color: parent.buttonColor.toString() === "#a5a5a5" ? "black" : "white"
        }

        MouseArea {
            id: mouseArea
            anchors.fill: parent
            onClicked: parent.clicked()
        }
    }
}
