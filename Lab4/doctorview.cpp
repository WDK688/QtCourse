#include "doctorview.h"
#include "ui_doctorview.h"
#include "idatabase.h"

#include <QMessageBox>

DoctorView::DoctorView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::DoctorView)
{
    ui->setupUi(this);

    // 设置表格视图属性
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setEditTriggers(QAbstractItemView::DoubleClicked);
    ui->tableView->setAlternatingRowColors(true);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);

    // 初始化数据库模型
    IDatabase &iDatabase = IDatabase::getInstance();
    if (iDatabase.initDoctorModel()) {
        ui->tableView->setModel(iDatabase.doctorTabModel);
        ui->tableView->setSelectionModel(iDatabase.theDoctorSelection);

        // 设置表格列宽度
        ui->tableView->setColumnWidth(0, 80);   // doctor_id
        ui->tableView->setColumnWidth(1, 80);   // name
        ui->tableView->setColumnWidth(2, 60);   // gender
        ui->tableView->setColumnWidth(3, 100);  // dept_id
        ui->tableView->setColumnWidth(4, 100);  // title
        ui->tableView->setColumnWidth(5, 100);  // phone
        ui->tableView->setColumnWidth(6, 100);  // work_time
    }


}

DoctorView::~DoctorView()
{
    delete ui;
}

void DoctorView::on_btSearch_clicked()
{

}


void DoctorView::on_btAdd_clicked()
{

}


void DoctorView::on_btDelete_clicked()
{

}


void DoctorView::on_btEdit_clicked()
{

}

