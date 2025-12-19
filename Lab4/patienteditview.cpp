#include "patienteditview.h"
#include "ui_patienteditview.h"
#include"idatabase.h"
#include<QSqlTableModel>

PatientEditView::PatientEditView(QWidget *parent,int index)
    : QWidget(parent)
    , ui(new Ui::PatientEditView)
{
    ui->setupUi(this);

    dataMapper = new QDataWidgetMapper();
    // 获取数据库中的患者表模型
    QSqlTableModel *tabModel = IDatabase::getInstance().patientTabModel;
    // 设置数据映射器的模型
    dataMapper->setModel(IDatabase::getInstance().patientTabModel);
    // 设置提交策略为自动提交
    dataMapper->setSubmitPolicy(QDataWidgetMapper::AutoSubmit);

    // 建立控件与数据库表字段的映射关系
    dataMapper->addMapping(ui->dbEditID, tabModel->fieldIndex("ID"));
    dataMapper->addMapping(ui->dbEditName, tabModel->fieldIndex("NAME"));
    dataMapper->addMapping(ui->dbEditIDCard, tabModel->fieldIndex("ID_CARD"));
    dataMapper->addMapping(ui->dbSpinHeight, tabModel->fieldIndex("HEIGHT"));
    dataMapper->addMapping(ui->dbSpinWeight, tabModel->fieldIndex("WEIGHT"));
    dataMapper->addMapping(ui->dbEditMobile, tabModel->fieldIndex("MOBILEPHONE"));
    dataMapper->addMapping(ui->dbDateEditDOB, tabModel->fieldIndex("DOB"));
    dataMapper->addMapping(ui->dbComboSex, tabModel->fieldIndex("SEX"));
    dataMapper->addMapping(ui->dbCreatedTimeStamp, tabModel->fieldIndex("CREATEDTIMESTAMP"));

    // 设置当前映射的索引（注意原代码是setcurrentIndex，Qt中首字母大写应为setCurrentIndex）
    dataMapper->setCurrentIndex(index);

}

PatientEditView::~PatientEditView()
{
    delete ui;
}

void PatientEditView::on_pushButton_clicked()
{
    IDatabase::getInstance().submitPatientEdit();
    emit goPreviousView();
}


void PatientEditView::on_pushButton_2_clicked()
{
    IDatabase::getInstance().revertPatientEdit();
    emit goPreviousView();
}

