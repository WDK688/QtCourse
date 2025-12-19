#include "welcomeview.h"
#include "ui_welcomeview.h"
#include<QDebug>
WelcomeView::WelcomeView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::WelcomeView)
{
    qDebug()<<"create WelcomeView";
    ui->setupUi(this);
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

