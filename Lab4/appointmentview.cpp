#include "appointmentview.h"
#include "ui_appointmentview.h"
#include "idatabase.h"
#include <QMessageBox>

AppointmentView::AppointmentView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AppointmentView)
{
    ui->setupUi(this);

    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->setAlternatingRowColors(true);

    IDatabase &iDatabase = IDatabase::getInstance();
    if (iDatabase.initAppointmentModel()) {
        ui->tableView->setModel(iDatabase.appointmentTabModel);
        ui->tableView->setSelectionModel(iDatabase.theAppointmentSelection);
    }

    // 设置状态过滤下拉框
    ui->cbStatusFilter->addItems(QStringList() << "全部" << "待审核" << "已通过" << "已拒绝");

    // 显式连接按钮点击信号（Qt 6需要显式连接）
    connect(ui->btAdd, &QPushButton::clicked, this, &AppointmentView::on_btAdd_clicked);
    connect(ui->btSearch, &QPushButton::clicked, this, &AppointmentView::on_btSearch_clicked);
    connect(ui->btDelete, &QPushButton::clicked, this, &AppointmentView::on_btDelete_clicked);
    connect(ui->btEdit, &QPushButton::clicked, this, &AppointmentView::on_btEdit_clicked);
    connect(ui->btApprove, &QPushButton::clicked, this, &AppointmentView::on_btApprove_clicked);
    connect(ui->btReject, &QPushButton::clicked, this, &AppointmentView::on_btReject_clicked);
}

AppointmentView::~AppointmentView()
{
    delete ui;
}

void AppointmentView::on_btAdd_clicked()
{
    // 安全检查：确保模型已初始化
    if (!IDatabase::getInstance().appointmentTabModel) {
        QMessageBox::warning(this, "错误", "预约模型未初始化，无法添加新预约");
        return;
    }

    int currow = IDatabase::getInstance().addNewAppointment();

    // 检查返回值是否有效
    if (currow < 0) {
        QMessageBox::warning(this, "错误", "无法创建新预约记录");
        return;
    }

    qDebug() << "新增预约，行号：" << currow;
    emit goAppointmentEditView(currow);
}

void AppointmentView::on_btSearch_clicked()
{
    QString filter;
    QString searchText = ui->tstSearch->text();
    QString statusFilter = ui->cbStatusFilter->currentText();

    if (statusFilter == "全部") {
        if (!searchText.isEmpty()) {
            filter = QString("PATIENT_ID LIKE '%%1%' OR DOCTOR_ID LIKE '%%2%'")
            .arg(searchText).arg(searchText);
        }
    } else {
        if (!searchText.isEmpty()) {
            filter = QString("(PATIENT_ID LIKE '%%1%' OR DOCTOR_ID LIKE '%%2%') AND STATUS = '%3'")
            .arg(searchText).arg(searchText).arg(statusFilter);
        } else {
            filter = QString("STATUS = '%1'").arg(statusFilter);
        }
    }

    IDatabase::getInstance().searchAppointment(filter);
}

void AppointmentView::on_btDelete_clicked()
{
    QModelIndex curIndex = IDatabase::getInstance().theAppointmentSelection->currentIndex();
    if (!curIndex.isValid()) {
        QMessageBox::warning(this, "警告", "请先选择要删除的预约记录");
        return;
    }

    // 获取当前预约的状态
    int row = curIndex.row();
    QString status = IDatabase::getInstance().appointmentTabModel->record(row).value("STATUS").toString();

    if (status == "已通过") {
        QMessageBox::warning(this, "警告", "已通过的预约不能删除");
        return;
    }

    if (QMessageBox::question(this, "确认", "确定要删除这条预约记录吗？") == QMessageBox::Yes) {
        if (IDatabase::getInstance().deleteCurrentAppointment()) {
            QMessageBox::information(this, "成功", "预约记录删除成功");
        } else {
            QMessageBox::critical(this, "错误", "删除预约记录失败");
        }
    }
}

void AppointmentView::on_btEdit_clicked()
{
    QModelIndex curIndex = IDatabase::getInstance().theAppointmentSelection->currentIndex();
    if (!curIndex.isValid()) {
        QMessageBox::warning(this, "警告", "请先选择要编辑的预约记录");
        return;
    }

    // 获取当前预约的状态
    int row = curIndex.row();
    QString status = IDatabase::getInstance().appointmentTabModel->record(row).value("STATUS").toString();

    if (status == "已通过") {
        QMessageBox::warning(this, "警告", "已通过的预约不能修改");
        return;
    }

    emit goAppointmentEditView(curIndex.row());
}

void AppointmentView::on_btApprove_clicked()
{
    QModelIndex curIndex = IDatabase::getInstance().theAppointmentSelection->currentIndex();
    if (!curIndex.isValid()) {
        QMessageBox::warning(this, "警告", "请先选择要审核的预约记录");
        return;
    }

    int row = curIndex.row();
    QString status = IDatabase::getInstance().appointmentTabModel->record(row).value("STATUS").toString();

    if (status != "待审核") {
        QMessageBox::warning(this, "警告", "只有待审核的预约才能进行审核操作");
        return;
    }

    if (QMessageBox::question(this, "确认", "确定通过这条预约吗？") == QMessageBox::Yes) {
        if (IDatabase::getInstance().approveAppointment(row)) {
            QMessageBox::information(this, "成功", "预约审核通过");
            IDatabase::getInstance().appointmentTabModel->select();
        } else {
            QMessageBox::critical(this, "错误", "审核操作失败");
        }
    }
}

void AppointmentView::on_btReject_clicked()
{
    QModelIndex curIndex = IDatabase::getInstance().theAppointmentSelection->currentIndex();
    if (!curIndex.isValid()) {
        QMessageBox::warning(this, "警告", "请先选择要审核的预约记录");
        return;
    }

    int row = curIndex.row();
    QString status = IDatabase::getInstance().appointmentTabModel->record(row).value("STATUS").toString();

    if (status != "待审核") {
        QMessageBox::warning(this, "警告", "只有待审核的预约才能进行审核操作");
        return;
    }

    if (QMessageBox::question(this, "确认", "确定拒绝这条预约吗？") == QMessageBox::Yes) {
        if (IDatabase::getInstance().rejectAppointment(row)) {
            QMessageBox::information(this, "成功", "预约已拒绝");
            IDatabase::getInstance().appointmentTabModel->select();
        } else {
            QMessageBox::critical(this, "错误", "审核操作失败");
        }
    }
}
