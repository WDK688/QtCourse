// #ifndef MEDICINEEDITVIEW_H
// #define MEDICINEEDITVIEW_H

// #include <QWidget>

// namespace Ui {
// class MedicineEditView;
// }

// class MedicineEditView : public QWidget
// {
//     Q_OBJECT

// public:
//     explicit MedicineEditView(QWidget *parent = nullptr, int rowNo = -1);
//     ~MedicineEditView();

// private slots:
//     void on_btSave_clicked();
//     void on_btCancel_clicked();

// signals:
//     void goPreviousView();

// private:
//     Ui::MedicineEditView *ui;
//     int currentRowNo;
// };

// #endif // MEDICINEEDITVIEW_H
#ifndef MEDICINEEDITVIEW_H
#define MEDICINEEDITVIEW_H

#include <QWidget>
#include <QDataWidgetMapper>  // 添加数据映射器头文件

namespace Ui {
class MedicineEditView;
}

class MedicineEditView : public QWidget
{
    Q_OBJECT

public:
    explicit MedicineEditView(QWidget *parent = nullptr, int rowNo = -1);
    ~MedicineEditView();

private slots:
    void on_btSave_clicked();
    void on_btCancel_clicked();

signals:
    void goPreviousView();

private:
    Ui::MedicineEditView *ui;
    QDataWidgetMapper *dataMapper;  // 数据映射器
    int currentRowNo;
};

#endif // MEDICINEEDITVIEW_H
