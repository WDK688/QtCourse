// #ifndef WELCOMEVIEW_H
// #define WELCOMEVIEW_H

// #include <QWidget>
// #include <QPushButton>

// namespace Ui {
// class WelcomeView;
// }

// class WelcomeView : public QWidget
// {
//     Q_OBJECT

// public:
//     explicit WelcomeView(QWidget *parent = nullptr);
//     ~WelcomeView();

// private slots:
//     void on_btnDepartment_clicked();

//     void on_btnDoctor_clicked();

//     void on_btnPatient_clicked();
//     void on_btnMedicine_clicked();
//     void on_btnPrescription_clicked();


//     void on_btnAppointment_clicked();

// signals:
//     void goDepartmentView();
//     void goDoctorView();
//     void goPatientView();
//     void goMedicineView();

//     void goPrescriptionView();
//     void goAppointmentView();
// private:
//     Ui::WelcomeView *ui;
// };

// #endif // WELCOMEVIEW_H
#ifndef WELCOMEVIEW_H
#define WELCOMEVIEW_H

#include <QWidget>
#include <QPushButton>

namespace Ui {
class WelcomeView;
}

class WelcomeView : public QWidget
{
    Q_OBJECT

public:
    explicit WelcomeView(QWidget *parent = nullptr);
    ~WelcomeView();

private slots:
    void on_btnDepartment_clicked();

    void on_btnDoctor_clicked();

    void on_btnPatient_clicked();
    void on_btnMedicine_clicked();
    void on_btnPrescription_clicked();


    void on_btnAppointment_clicked();
    void on_btnReport_clicked();  // 添加报表按钮点击处理

signals:
    void goDepartmentView();
    void goDoctorView();
    void goPatientView();
    void goMedicineView();

    void goPrescriptionView();
    void goAppointmentView();
    void goReportView();  // 添加跳转到报表视图的信号
private:
    Ui::WelcomeView *ui;
};

#endif // WELCOMEVIEW_H
