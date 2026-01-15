#include "masterview.h"
#include "ui_masterview.h"
#include<QDebug>
#include"idatabase.h"
#include "departmenteditview.h"

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

    connect(loginView, SIGNAL(loginSuccess()),this,SLOT(goWelcomeView()));
}

void MasterView::goWelcomeView()
{
    qDebug() << "goWelcomeview";
    welcomeView = new WelcomeView(this);
    pushWidgetToStackView(welcomeView);

    connect(welcomeView, SIGNAL(goDoctorView()),this,SLOT(goDoctorView()));
    connect(welcomeView, SIGNAL(goDepartmentView()),this,SLOT(goDepartmentView()));
    connect(welcomeView, SIGNAL(goPatientView()),this,SLOT(goPatientView()));
}

void MasterView::goDoctorView()
{
    qDebug() << "goDoctorView";
    doctorView = new DoctorView(this);
    pushWidgetToStackView(doctorView);
    connect(doctorView, SIGNAL(goDoctorEditView(int)),this,SLOT(goDoctorEditView(int)));
}

void MasterView::goDepartmentView()
{
    qDebug() << "goDepartmentView";
    departmentView = new DepartmentView(this);
    pushWidgetToStackView(departmentView);
    connect(departmentView, SIGNAL(goDepartmentEditView(int)), this, SLOT(goDepartmentEditView(int)));
}

void MasterView::goPatientView()
{
    qDebug() << "goPatientView";
    patientView = new PatientView(this);
    pushWidgetToStackView(patientView);

    connect(patientView, SIGNAL(goPatientEditView(int)),this,SLOT(goPatientEditView(int)));
}

void MasterView::goPatientEditView(int rowNo)
{
    qDebug() << "goPatientEditView";
    patientEditView = new PatientEditView(this,rowNo);
    pushWidgetToStackView(patientEditView);

    connect(patientEditView, SIGNAL(goPreviousView()),this,SLOT(goPreviousView()));
}


void MasterView::goDoctorEditView(int rowNo)
{
    doctorEditView = new DoctorEditView(this, rowNo);
    pushWidgetToStackView(doctorEditView);

    connect(doctorEditView, SIGNAL(goPreviousView()),this,SLOT(goPreviousView()));
}
void MasterView::goDepartmentEditView(int rowNo)
{
    departmentEditView = new DepartmentEditView(this, rowNo);
    pushWidgetToStackView(departmentEditView);
    connect(departmentEditView, SIGNAL(goPreviousView()),this, SLOT(goPreviousView()));
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

