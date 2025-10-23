#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QStack>
#include <QKeyEvent>
#include <QMap>
#include <QPushButton>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void btnNumClicked();
    void btnBinaryOperatorClicked();
    void btnUnaryOperatorClicked();
    void on_btnPeriod_clicked();
    void on_btnDel_clicked();
    void on_btnClearAll_clicked();
    void on_btnEqual_clicked();
    void on_btnClear_clicked();
    void on_btnSign_clicked(); // 新增：正负号切换

private:
    Ui::MainWindow *ui;

    // 成员变量
    QString currentInput;      // 当前输入的数字
    double result;            // 计算结果
    QString pendingOperator;  // 等待的运算符
    bool waitingForOperand;   // 是否等待新操作数

    // 按钮映射
    QMap<int, QPushButton*> digitBTNS;
    QMap<int, QPushButton*> binaryOpBTNS;
    QMap<int, QPushButton*> unaryOpBTNS;

    // 方法
    void reset();
    void calculate();
    void abortOperation();
    bool calculate(double rightOperand, const QString &pendingOperator);

    QLabel *statusMsgLabel;
};
#endif // MAINWINDOW_H
