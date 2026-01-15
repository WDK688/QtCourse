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
};
#endif // MASTERVIEW_H
