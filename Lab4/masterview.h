// #ifndef MASTERVIEW_H
// #define MASTERVIEW_H

// #include <QWidget>
// #include"loginview.h"
// #include"doctorview.h"
// #include"departmentview.h"
// #include"patientview.h"
// #include"patienteditview.h"
// #include"welcomeview.h"
// #include "departmenteditview.h"
// #include "doctoreditview.h"
// #include "medicineview.h"
// #include "medicineeditview.h"
// #include "prescriptionview.h"
// #include "prescriptioneditview.h"
// #include "appointmentview.h"
// #include "appointmenteditview.h"
// QT_BEGIN_NAMESPACE
// namespace Ui {
// class MasterView;
// }
// QT_END_NAMESPACE

// class MasterView : public QWidget
// {
//     Q_OBJECT

// public:
//     MasterView(QWidget *parent = nullptr);
//     ~MasterView();

// public slots:
//     void goLoginView();
//     void goWelcomeView();
//     void goDoctorView();
//     void goDepartmentView();
//     void goPatientView();
//     void goPatientEditView(int rowNo);
//     void goPreviousView();
//     void goDepartmentEditView(int rowNo);
//     void goDoctorEditView(int rowNo);
//     void goMedicineEditView(int rowNo);
//     void goMedicineView();
//     void goPrescriptionView();
//     void goPrescriptionEditView(int rowNo);
//     void goAppointmentView();
//     void goAppointmentEditView(int rowNo);



// private slots:
//     void on_btBack_clicked();

//     void on_stackedWidget_currentChanged(int arg1);

//     void on_byLogout_clicked();

// private:
//     void pushWidgetToStackView(QWidget *widget);

//     Ui::MasterView *ui;

//     WelcomeView *welcomeView;
//     DoctorView *doctorView;
//     PatientView *patientView;
//     DepartmentView *departmentView;
//     LoginView *loginView;
//     PatientEditView *patientEditView;
//     DepartmentEditView *departmentEditView;
//     DoctorEditView *doctorEditView;
//     MedicineView *medicineView;
//     MedicineEditView *medicineEditView;
//     PrescriptionView *prescriptionView;
//     PrescriptionEditView *prescriptionEditView;
//     AppointmentView *appointmentView;
//     AppointmentEditView *appointmentEditView;
// };
// #endif // MASTERVIEW_H
#ifndef MASTERVIEW_H
#define MASTERVIEW_H

#include <QWidget>
#include"loginview.h"
#include"doctorview.h"
#include"departmentview.h"
#include"patientview.h"
#include"patienteditview.h"
#include"welcomeview.h"
#include "departmenteditview.h"
#include "doctoreditview.h"
#include "medicineview.h"
#include "medicineeditview.h"
#include "prescriptionview.h"
#include "prescriptioneditview.h"
#include "appointmentview.h"
#include "appointmenteditview.h"
#include "reportview.h"  // 添加报表视图头文件
QT_BEGIN_NAMESPACE
namespace Ui {
class MasterView;
}
QT_END_NAMESPACE

class MasterView : public QWidget
{
    Q_OBJECT

public:
    MasterView(QWidget *parent = nullptr);
    ~MasterView();

public slots:
    void goLoginView();
    void goWelcomeView();
    void goDoctorView();
    void goDepartmentView();
    void goPatientView();
    void goPatientEditView(int rowNo);
    void goPreviousView();
    void goDepartmentEditView(int rowNo);
    void goDoctorEditView(int rowNo);
    void goMedicineEditView(int rowNo);
    void goMedicineView();
    void goPrescriptionView();
    void goPrescriptionEditView(int rowNo);
    void goAppointmentView();
    void goAppointmentEditView(int rowNo);
    void goReportView();  // 添加跳转到报表视图的槽函数



private slots:
    void on_btBack_clicked();

    void on_stackedWidget_currentChanged(int arg1);

    void on_byLogout_clicked();

private:
    void pushWidgetToStackView(QWidget *widget);

    Ui::MasterView *ui;

    WelcomeView *welcomeView;
    DoctorView *doctorView;
    PatientView *patientView;
    DepartmentView *departmentView;
    LoginView *loginView;
    PatientEditView *patientEditView;
    DepartmentEditView *departmentEditView;
    DoctorEditView *doctorEditView;
    MedicineView *medicineView;
    MedicineEditView *medicineEditView;
    PrescriptionView *prescriptionView;
    PrescriptionEditView *prescriptionEditView;
    AppointmentView *appointmentView;
    AppointmentEditView *appointmentEditView;
    ReportView *reportView;  // 添加报表视图成员变量
};
#endif // MASTERVIEW_H
