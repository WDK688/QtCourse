#include "welcomeview.h"
#include "ui_welcomeview.h"
#include<QDebug>
WelcomeView::WelcomeView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::WelcomeView)
{
    qDebug()<<"create WelcomeView";
    ui->setupUi(this);

    // 显式连接按钮点击信号（Qt 6需要显式连接）
    connect(ui->btnDepartment, &QPushButton::clicked, this, &WelcomeView::on_btnDepartment_clicked);
    connect(ui->btnDoctor, &QPushButton::clicked, this, &WelcomeView::on_btnDoctor_clicked);
    connect(ui->btnPatient, &QPushButton::clicked, this, &WelcomeView::on_btnPatient_clicked);
    connect(ui->btnMedicine, &QPushButton::clicked, this, &WelcomeView::on_btnMedicine_clicked);
    connect(ui->btnPrescription, &QPushButton::clicked, this, &WelcomeView::on_btnPrescription_clicked);
    connect(ui->btnAppointment, &QPushButton::clicked, this, &WelcomeView::on_btnAppointment_clicked);
}

WelcomeView::~WelcomeView()
{
    qDebug()<<"destroy WelcomeView";
    delete ui;
}

void WelcomeView::on_btnDepartment_clicked()
{
    emit goDepartmentView();
}


void WelcomeView::on_btnDoctor_clicked()
{
    emit goDoctorView();
}


void WelcomeView::on_btnPatient_clicked()
{
    emit goPatientView();
}


void WelcomeView::on_btnMedicine_clicked()
{
    emit goMedicineView();
}

void WelcomeView::on_btnPrescription_clicked()
{
    emit goPrescriptionView();
}


void WelcomeView::on_btnAppointment_clicked()
{
    emit goAppointmentView();
}

