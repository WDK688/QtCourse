#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <math.h>

#ifdef _MSC_VER
// 微软编译器无需额外操作
#else
#pragma link "m"  // 用于GCC等编译器链接数学库
#endif

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 初始化状态
    reset();

    // 数字按钮映射
    digitBTNS = {
                 {Qt::Key_0, ui->btnNum0},
                 {Qt::Key_1, ui->btnNum1},
                 {Qt::Key_2, ui->btnNum2},
                 {Qt::Key_3, ui->btnNum3},
                 {Qt::Key_4, ui->btnNum4},
                 {Qt::Key_5, ui->btnNum5},
                 {Qt::Key_6, ui->btnNum6},
                 {Qt::Key_7, ui->btnNum7},
                 {Qt::Key_8, ui->btnNum8},
                 {Qt::Key_9, ui->btnNum9},
                 };

    // 二元运算符映射 - 修改为÷和×
    binaryOpBTNS = {
        {Qt::Key_Plus, ui->btnPlus},
        {Qt::Key_Minus, ui->btnMinus},
        {Qt::Key_Asterisk, ui->btnMultiple},  // * 对应 ×
        {Qt::Key_Slash, ui->btnDivide}        // / 对应 ÷
    };

    // 一元运算符映射
    unaryOpBTNS = {
        {Qt::Key_Percent, ui->btnPercentage},
        {Qt::Key_AsciiCircum, ui->btnSquare},
        {Qt::Key_S, ui->btnSqrt},
        {Qt::Key_I, ui->btnInverse},
        {Qt::Key_N, ui->btnSign}  // 添加正负号按键映射
    };

    // 连接信号槽
    foreach(auto btn, digitBTNS) {
        connect(btn, SIGNAL(clicked()), this, SLOT(btnNumClicked()));
    }

    foreach (QPushButton* btn, binaryOpBTNS.values()) {
        connect(btn, SIGNAL(clicked()), this, SLOT(btnBinaryOperatorClicked()));
    }

    foreach (QPushButton* btn, unaryOpBTNS.values()) {
        connect(btn, SIGNAL(clicked()), this, SLOT(btnUnaryOperatorClicked()));
    }

    // 连接其他按钮
    connect(ui->btnClear, SIGNAL(clicked()), this, SLOT(on_btnClear_clicked()));
    connect(ui->btnSign, SIGNAL(clicked()), this, SLOT(on_btnSign_clicked())); // 连接正负号按钮
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::reset()
{
    currentInput = "0";
    result = 0.0;
    pendingOperator.clear();
    waitingForOperand = true;
    ui->display->setText(currentInput);
}

void MainWindow::btnNumClicked()
{
    QPushButton *clickedButton = qobject_cast<QPushButton*>(sender());
    QString digit = clickedButton->text();

    if (currentInput == "0" && digit == "0")
        return;

    if (waitingForOperand) {
        currentInput.clear();
        waitingForOperand = false;
    }

    if (currentInput.length() < 16) { // 限制输入长度
        currentInput += digit;
    }

    ui->display->setText(currentInput);
}

void MainWindow::on_btnPeriod_clicked()
{
    if (waitingForOperand) {
        currentInput = "0";
        waitingForOperand = false;
    }

    if (!currentInput.contains('.')) {
        currentInput += '.';
        ui->display->setText(currentInput);
    }
}

void MainWindow::btnBinaryOperatorClicked()
{
    QPushButton *clickedButton = qobject_cast<QPushButton*>(sender());
    QString clickedOperator = clickedButton->text();
    double operand = currentInput.toDouble();

    // 将显示的符号转换为计算用的符号
    QString calcOperator = clickedOperator;
    if (clickedOperator == "×") calcOperator = "*";
    if (clickedOperator == "÷") calcOperator = "/";

    if (!pendingOperator.isEmpty()) {
        calculate(); // 如果有等待的运算符，先计算
    } else {
        result = operand;
    }

    pendingOperator = calcOperator;  // 存储计算用的符号
    waitingForOperand = true;

    ui->display->setText(QString::number(result, 'g', 15));
}

void MainWindow::btnUnaryOperatorClicked()
{
    QPushButton *clickedButton = qobject_cast<QPushButton*>(sender());
    QString clickedOperator = clickedButton->text();
    double operand = currentInput.toDouble();
    double unaryResult = 0.0;

    if (clickedOperator == "%") {
        unaryResult = operand / 100.0;
    } else if (clickedOperator == "1/x") {
        if (operand == 0.0) {
            abortOperation();
            return;
        }
        unaryResult = 1.0 / operand;
    } else if (clickedOperator == "x^2") {
        unaryResult = operand * operand;
    } else if (clickedOperator == "√") {
        if (operand < 0.0) {
            abortOperation();
            return;
        }
        unaryResult = sqrt(operand);
    } else if (clickedOperator == "+/-") {
        // 正负号切换功能
        unaryResult = -operand;
    }

    currentInput = QString::number(unaryResult, 'g', 15);
    ui->display->setText(currentInput);
    waitingForOperand = true;
}

// 新增：正负号切换功能
void MainWindow::on_btnSign_clicked()
{
    if (currentInput == "0" || waitingForOperand) {
        return;
    }

    double value = currentInput.toDouble();
    value = -value;
    currentInput = QString::number(value, 'g', 15);
    ui->display->setText(currentInput);
}

void MainWindow::on_btnEqual_clicked()
{
    if (!pendingOperator.isEmpty()) {
        calculate();
        pendingOperator.clear();
    }
    waitingForOperand = true;
}

void MainWindow::calculate()
{
    double operand = currentInput.toDouble();

    if (pendingOperator == "+") {
        result += operand;
    } else if (pendingOperator == "-") {
        result -= operand;
    } else if (pendingOperator == "*") {  // 对应 ×
        result *= operand;
    } else if (pendingOperator == "/") {  // 对应 ÷
        if (operand == 0.0) {
            abortOperation();
            return;
        }
        result /= operand;
    }

    currentInput = QString::number(result, 'g', 15);
    ui->display->setText(currentInput);
}

bool MainWindow::calculate(double rightOperand, const QString &pendingOperator)
{
    if (pendingOperator == "+") {
        result += rightOperand;
    } else if (pendingOperator == "-") {
        result -= rightOperand;
    } else if (pendingOperator == "*") {
        result *= rightOperand;
    } else if (pendingOperator == "/") {
        if (rightOperand == 0.0)
            return false;
        result /= rightOperand;
    }
    return true;
}

void MainWindow::abortOperation()
{
    reset();
    ui->display->setText("负数无法开根");
}

void MainWindow::on_btnDel_clicked()
{
    if (waitingForOperand)
        return;

    if (currentInput.length() > 1) {
        currentInput.chop(1);
    } else {
        currentInput = "0";
        waitingForOperand = true;
    }

    ui->display->setText(currentInput);
}

void MainWindow::on_btnClearAll_clicked()
{
    reset();
}

void MainWindow::on_btnClear_clicked()
{
    currentInput = "0";
    waitingForOperand = true;
    ui->display->setText(currentInput);
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    foreach(auto btnKey, digitBTNS.keys()) {
        if(event->key() == btnKey) {
            digitBTNS[btnKey]->animateClick();
            return;
        }
    }

    foreach (auto btnKey, binaryOpBTNS.keys()) {
        if (event->key() == btnKey) {
            binaryOpBTNS[btnKey]->animateClick();
            return;
        }
    }

    foreach (auto btnKey, unaryOpBTNS.keys()) {
        if (event->key() == btnKey) {
            unaryOpBTNS[btnKey]->animateClick();
            return;
        }
    }

    if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return) {
        ui->btnEqual->animateClick();
    } else if (event->key() == Qt::Key_Escape) {
        ui->btnClearAll->animateClick();
    } else if (event->key() == Qt::Key_Backspace) {
        ui->btnDel->animateClick();
    } else if (event->key() == Qt::Key_Period || event->key() == Qt::Key_Comma) {
        ui->btnPeriod->animateClick();
    } else if (event->key() == Qt::Key_N) {
        ui->btnSign->animateClick();  // N键对应正负号切换
    }
}
