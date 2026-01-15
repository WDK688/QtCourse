#include "doctorview.h"
#include "ui_doctorview.h"
#include "idatabase.h"
#include <QMessageBox>

DoctorView::DoctorView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::DoctorView)
{
    ui->setupUi(this);
    this->setWindowTitle("医生管理");
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
        ui->tableView->setColumnWidth(2, 100);  // dept_id
        ui->tableView->setColumnWidth(3, 100);  // title
    }

    // 连接信号槽
    connect(ui->btAdd, &QPushButton::clicked, this, &DoctorView::on_btAdd_clicked);
    connect(ui->btSearch, &QPushButton::clicked, this, &DoctorView::on_btSearch_clicked);
    connect(ui->btDelete, &QPushButton::clicked, this, &DoctorView::on_btDelete_clicked);
    connect(ui->btEdit, &QPushButton::clicked, this, &DoctorView::on_btEdit_clicked);
}

DoctorView::~DoctorView()
{
    delete ui;
}

void DoctorView::on_btAdd_clicked()
{
    int currow = IDatabase::getInstance().addNewDoctor();
    ui->tableView->selectRow(currow);
    emit goDoctorEditView(currow);
}

void DoctorView::on_btSearch_clicked()
{
    QString filter = QString("name like '%%1%'").arg(ui->tstSearch->text());
    IDatabase::getInstance().searchDoctor(filter);
}

void DoctorView::on_btDelete_clicked()
{
    QModelIndex curIndex = IDatabase::getInstance().theDoctorSelection->currentIndex();
    if (!curIndex.isValid()) {
        QMessageBox::warning(this, "警告", "请先选择要删除的医生！");
        return;
    }

    int ret = QMessageBox::question(this, "确认", "确定要删除选中的医生吗？");
    if (ret == QMessageBox::Yes) {
        if (IDatabase::getInstance().deleteCurrentDoctor()) {
            QMessageBox::information(this, "提示", "删除成功！");
        } else {
            QMessageBox::critical(this, "错误", "删除失败！");
        }
    }
}

void DoctorView::on_btEdit_clicked()
{
    QModelIndex curIndex = IDatabase::getInstance().theDoctorSelection->currentIndex();
    if (!curIndex.isValid()) {
        QMessageBox::warning(this, "警告", "请先选择要编辑的医生！");
        return;
    }

    emit goDoctorEditView(curIndex.row());
}
