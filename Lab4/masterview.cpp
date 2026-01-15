#include "masterview.h"
#include "ui_masterview.h"
#include<QDebug>
#include"idatabase.h"
#include "departmenteditview.h"
#include "prescriptionview.h"

MasterView::MasterView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MasterView)
{
    ui->setupUi(this);

    this->setWindowFlag(Qt::FramelessWindowHint);

    goLoginView();

    IDatabase::getInstance();

}

MasterView::~MasterView()
{
    delete ui;
}

void MasterView::goLoginView()
{
    qDebug() << "goLoginview";
    loginView = new LoginView(this);
    pushWidgetToStackView(loginView);

    // 使用Qt 5/6新式连接语法
    connect(loginView, &LoginView::loginSuccess, this, &MasterView::goWelcomeView);
}

void MasterView::goWelcomeView()
{
    qDebug() << "goWelcomeview";
    welcomeView = new WelcomeView(this);
    pushWidgetToStackView(welcomeView);

    // 使用Qt 5/6新式连接语法
    connect(welcomeView, &WelcomeView::goDoctorView, this, &MasterView::goDoctorView);
    connect(welcomeView, &WelcomeView::goDepartmentView, this, &MasterView::goDepartmentView);
    connect(welcomeView, &WelcomeView::goPatientView, this, &MasterView::goPatientView);
    connect(welcomeView, &WelcomeView::goMedicineView, this, &MasterView::goMedicineView);
    connect(welcomeView, &WelcomeView::goPrescriptionView, this, &MasterView::goPrescriptionView);
    connect(welcomeView, &WelcomeView::goAppointmentView, this, &MasterView::goAppointmentView);
}

void MasterView::goDoctorView()
{
    qDebug() << "goDoctorView";
    doctorView = new DoctorView(this);
    pushWidgetToStackView(doctorView);
    connect(doctorView, &DoctorView::goDoctorEditView, this, &MasterView::goDoctorEditView);
}

void MasterView::goDepartmentView()
{
    qDebug() << "goDepartmentView";
    departmentView = new DepartmentView(this);
    pushWidgetToStackView(departmentView);
    connect(departmentView, &DepartmentView::goDepartmentEditView, this, &MasterView::goDepartmentEditView);
}

void MasterView::goPatientView()
{
    qDebug() << "goPatientView";
    patientView = new PatientView(this);
    pushWidgetToStackView(patientView);

    connect(patientView, &PatientView::goPatientEditView, this, &MasterView::goPatientEditView);
}

void MasterView::goPatientEditView(int rowNo)
{
    qDebug() << "goPatientEditView";
    patientEditView = new PatientEditView(this,rowNo);
    pushWidgetToStackView(patientEditView);

    connect(patientEditView, &PatientEditView::goPreviousView, this, &MasterView::goPreviousView);
}

void MasterView::goPrescriptionView()
{
    qDebug() << "goPrescriptionView";
    prescriptionView = new PrescriptionView(this);
    pushWidgetToStackView(prescriptionView);

    connect(prescriptionView, &PrescriptionView::goPrescriptionEditView, this, &MasterView::goPrescriptionEditView);
}
void MasterView::goPrescriptionEditView(int rowNo)
{
    prescriptionEditView = new PrescriptionEditView(this, rowNo);
    pushWidgetToStackView(prescriptionEditView);
    connect(prescriptionEditView, &PrescriptionEditView::goPreviousView, this, &MasterView::goPreviousView);
}

void MasterView::goAppointmentView()
{
    qDebug() << "goAppointmentView";
    appointmentView = new AppointmentView(this);
    pushWidgetToStackView(appointmentView);

    connect(appointmentView, &AppointmentView::goAppointmentEditView, this, &MasterView::goAppointmentEditView);
}

void MasterView::goAppointmentEditView(int rowNo)
{
    appointmentEditView = new AppointmentEditView(this, rowNo);
    pushWidgetToStackView(appointmentEditView);
    connect(appointmentEditView, &AppointmentEditView::goPreviousView, this, &MasterView::goPreviousView);
}
void MasterView::goDoctorEditView(int rowNo)
{
    doctorEditView = new DoctorEditView(this, rowNo);
    pushWidgetToStackView(doctorEditView);

    connect(doctorEditView, &DoctorEditView::goPreviousView, this, &MasterView::goPreviousView);
}
void MasterView::goDepartmentEditView(int rowNo)
{
    departmentEditView = new DepartmentEditView(this, rowNo);
    pushWidgetToStackView(departmentEditView);
    connect(departmentEditView, &DepartmentEditView::goPreviousView, this, &MasterView::goPreviousView);
}

void MasterView::goMedicineView()
{
    qDebug() << "goMedicineView";
    medicineView = new MedicineView(this);
    pushWidgetToStackView(medicineView);

    connect(medicineView, &MedicineView::goMedicineEditView, this, &MasterView::goMedicineEditView);
}
void MasterView::goMedicineEditView(int rowNo)
{
    medicineEditView = new MedicineEditView(this, rowNo);
    pushWidgetToStackView(medicineEditView);
    connect(medicineEditView, &MedicineEditView::goPreviousView, this, &MasterView::goPreviousView);
}

void MasterView::goPreviousView()
{
    int count = ui ->stackedWidget->count();
    if(count>1){
        ui->stackedWidget->setCurrentIndex(count-2);
        ui->labelTitle->setText(ui->stackedWidget->currentWidget()->windowTitle());

        QWidget *widget = ui->stackedWidget->widget(count-1);
        ui->stackedWidget->removeWidget(widget);
        delete widget;
    }
}

void MasterView::pushWidgetToStackView(QWidget *widget)
{
    ui->stackedWidget->addWidget(widget);
    int count = ui->stackedWidget->count();
    ui->stackedWidget->setCurrentIndex(count-1);
    ui->labelTitle->setText(widget->windowTitle());

}

void MasterView::on_btBack_clicked()
{
    goPreviousView();
}


void MasterView::on_stackedWidget_currentChanged(int arg)
{
    int count = ui->stackedWidget->count();
    if(count>1)
        ui->btBack->setEnabled(true);
    else
        ui->btBack->setEnabled(false);

    QString title = ui->stackedWidget->currentWidget()->windowTitle();

    if(title=="欢迎"){
        ui->byLogout->setEnabled(true);
        ui->btBack->setEnabled(false);
    }else
        ui->byLogout->setEnabled(false);
}


void MasterView::on_byLogout_clicked()
{
    goPreviousView();
}

