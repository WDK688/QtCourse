#include "departmentview.h"
#include "ui_departmentview.h"
#include "idatabase.h"

DepartmentView::DepartmentView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::DepartmentView)
{
    ui->setupUi(this);
    this->setWindowTitle("科室管理");

    // 设置表格视图属性
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setEditTriggers(QAbstractItemView::DoubleClicked);
    ui->tableView->setAlternatingRowColors(true);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);

    // 初始化数据库模型并绑定到表格
    IDatabase &iDatabase = IDatabase::getInstance();
    if (iDatabase.initDepartmentModel()) {
        ui->tableView->setModel(iDatabase.departmentTabModel);
        ui->tableView->setSelectionModel(iDatabase.theDepartmentSelection);

        // 设置列宽
        ui->tableView->setColumnWidth(0, 80);   // dept_id
        ui->tableView->setColumnWidth(1, 150);  // dept_name
        ui->tableView->setColumnWidth(2, 200);  // dept_desc
    }
}

DepartmentView::~DepartmentView()
{
    delete ui;

}

void DepartmentView::on_btSearch_clicked()
{

}


void DepartmentView::on_btAdd_clicked()
{

}


void DepartmentView::on_btDelete_clicked()
{

}


void DepartmentView::on_btEdit_clicked()
{

}

