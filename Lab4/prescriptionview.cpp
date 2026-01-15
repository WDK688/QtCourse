#include "prescriptionview.h"
#include "ui_prescriptionview.h"
#include "idatabase.h"

PrescriptionView::PrescriptionView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PrescriptionView)
{
    ui->setupUi(this);

    // 设置表格样式
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->setAlternatingRowColors(true);

    // 初始化数据库模型（第二阶段启用）
    IDatabase &iDatabase = IDatabase::getInstance();
    if (iDatabase.initPrescriptionModel()) {
        ui->tableView->setModel(iDatabase.prescriptionTabModel);
        ui->tableView->setSelectionModel(iDatabase.thePrescriptionSelection);
    }
}

PrescriptionView::~PrescriptionView()
{
    delete ui;
}

void PrescriptionView::on_btAdd_clicked()
{
    int currow = IDatabase::getInstance().addNewPrescription();
    emit goPrescriptionEditView(currow);
}

void PrescriptionView::on_btSearch_clicked()
{
    QString filter = QString("PATIENT_NAME like '%%1%'").arg(ui->tstSearch->text());
    IDatabase::getInstance().searchPrescription(filter);
}

void PrescriptionView::on_btDelete_clicked()
{
    IDatabase::getInstance().deleteCurrentPrescription();
}

void PrescriptionView::on_btEdit_clicked()
{
    QModelIndex curIndex = IDatabase::getInstance().thePrescriptionSelection->currentIndex();
    emit goPrescriptionEditView(curIndex.row());
}
