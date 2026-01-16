#include "appointmenteditview.h"
#include "ui_appointmenteditview.h"
#include "idatabase.h"
#include <QSqlTableModel>
#include <QSqlQueryModel>
#include <QMessageBox>

AppointmentEditView::AppointmentEditView(QWidget *parent, int rowNo)
    : QWidget(parent)
    , ui(new Ui::AppointmentEditView)
    , currentIndex(rowNo)
{
    ui->setupUi(this);

    IDatabase &iDatabase = IDatabase::getInstance();

    // 初始化患者下拉框 - 显示姓名，存储ID
    QSqlTableModel *patientModel = iDatabase.patientTabModel;
    ui->cbPatient->setModel(patientModel);
    ui->cbPatient->setModelColumn(patientModel->fieldIndex("NAME"));

    // 初始化医生下拉框 - 显示姓名，存储ID
    QSqlTableModel *doctorModel = iDatabase.doctorTabModel;
    ui->cbDoctor->setModel(doctorModel);
    ui->cbDoctor->setModelColumn(doctorModel->fieldIndex("NAME"));

    // 设置状态可选值
    ui->dbComboStatus->addItems(QStringList() << "待审核" << "已通过" << "已拒绝");

    // 初始化数据映射器
    dataMapper = new QDataWidgetMapper(this);
    QSqlTableModel *tabModel = iDatabase.appointmentTabModel;
    dataMapper->setModel(tabModel);
    dataMapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);

    // 建立控件与数据库字段的映射关系
    dataMapper->addMapping(ui->dbEditID, tabModel->fieldIndex("ID"));
    dataMapper->addMapping(ui->cbPatient, tabModel->fieldIndex("PATIENT_ID"));
    dataMapper->addMapping(ui->cbDoctor, tabModel->fieldIndex("DOCTOR_ID"));
    dataMapper->addMapping(ui->dbDateTimeEdit, tabModel->fieldIndex("APPOINTMENT_TIME"));
    dataMapper->addMapping(ui->dbComboStatus, tabModel->fieldIndex("STATUS"));
    dataMapper->addMapping(ui->dbEditAuditComment, tabModel->fieldIndex("AUDIT_COMMENT"));
    dataMapper->addMapping(ui->dbEditCreatedTime, tabModel->fieldIndex("CREATEDTIMESTAMP"));

    // 设置只读字段
    ui->dbEditID->setReadOnly(true);
    ui->dbEditCreatedTime->setReadOnly(true);

    // 设置当前编辑的行
    dataMapper->setCurrentIndex(rowNo);

    // 连接信号槽
    connect(ui->btSave, &QPushButton::clicked, this, &AppointmentEditView::on_btSave_clicked);
    connect(ui->btCancel, &QPushButton::clicked, this, &AppointmentEditView::on_btCancel_clicked);
}

AppointmentEditView::~AppointmentEditView()
{
    delete ui;
}

void AppointmentEditView::on_btSave_clicked()
{
    // 先让 DataMapper 提交所有挂起的更改到模型
    if (dataMapper->submit()) {
        // 然后将模型的更改提交到数据库
        if (IDatabase::getInstance().submitAppointmentEdit()) {
            emit goPreviousView();
        } else {
            qDebug() << "数据库提交失败：" << IDatabase::getInstance().appointmentTabModel->lastError().text();
            QMessageBox::warning(this, "提交失败", "数据提交到数据库失败");
        }
    } else {
        // 如果 DataMapper 提交失败，显示错误信息
        //qDebug() << "DataMapper submit failed: " << dataMapper->model()->lastError().text();
        QMessageBox::warning(this, "提交失败", "数据提交失败，请检查输入是否正确");
    }
}

void AppointmentEditView::on_btCancel_clicked()
{
    // 取消编辑，回滚更改
    IDatabase::getInstance().revertAppointmentEdit();
    emit goPreviousView();
}
