#include "appointmenteditview.h"
#include "ui_appointmenteditview.h"
#include "idatabase.h"
#include <QSqlTableModel>
#include <QMessageBox>

AppointmentEditView::AppointmentEditView(QWidget *parent, int index)
    : QWidget(parent)
    , ui(new Ui::AppointmentEditView)
    , currentIndex(index)
    , dataMapper(nullptr)
{
    ui->setupUi(this);

    // 检查主模型是否已初始化
    if (!IDatabase::getInstance().appointmentTabModel) {
        QMessageBox::critical(this, "错误", "预约模型未初始化，无法编辑预约信息");
        return;
    }

    // 加载患者下拉框（检查患者模型）
    if (IDatabase::getInstance().patientTabModel) {
        QStringList patients = IDatabase::getInstance().getAllPatientNames();
        ui->cbPatient->addItems(patients);
    } else {
        qDebug() << "警告：患者模型未初始化";
    }

    // 加载医生下拉框（检查医生模型）
    if (IDatabase::getInstance().doctorTabModel) {
        QStringList doctors = IDatabase::getInstance().getAllDoctorNames();
        ui->cbDoctor->addItems(doctors);
    } else {
        qDebug() << "警告：医生模型未初始化";
    }

    // 检查index是否有效
    QSqlTableModel *tabModel = IDatabase::getInstance().appointmentTabModel;
    if (index < 0 || index >= tabModel->rowCount()) {
        qDebug() << "警告：无效的索引" << index << "，模型行数：" << tabModel->rowCount();
        QMessageBox::warning(this, "警告", "无效的预约记录索引");
        return;
    }

    // 初始化数据映射器
    dataMapper = new QDataWidgetMapper(this);
    dataMapper->setModel(tabModel);
    dataMapper->setSubmitPolicy(QDataWidgetMapper::AutoSubmit);

    // 建立控件与数据库表字段的映射关系
    dataMapper->addMapping(ui->dbEditID, tabModel->fieldIndex("ID"));
    dataMapper->addMapping(ui->dbDateTimeEdit, tabModel->fieldIndex("APPOINTMENT_TIME"));
    dataMapper->addMapping(ui->dbComboStatus, tabModel->fieldIndex("STATUS"));
    dataMapper->addMapping(ui->dbEditAuditComment, tabModel->fieldIndex("AUDIT_COMMENT"));
    dataMapper->addMapping(ui->dbEditCreatedTime, tabModel->fieldIndex("CREATEDTIMESTAMP"));

    // 设置状态可选值
    ui->dbComboStatus->addItems(QStringList() << "待审核" << "已通过" << "已拒绝");

    // 通过代码设置只读属性
    ui->dbEditID->setReadOnly(true);
    ui->dbEditCreatedTime->setReadOnly(true);

    // 设置当前索引
    dataMapper->setCurrentIndex(index);

    // 显式连接按钮点击信号
    connect(ui->btSave, &QPushButton::clicked, this, &AppointmentEditView::on_btSave_clicked);
    connect(ui->btCancel, &QPushButton::clicked, this, &AppointmentEditView::on_btCancel_clicked);
    connect(ui->cbPatient, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &AppointmentEditView::on_cbPatient_currentIndexChanged);
    connect(ui->cbDoctor, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &AppointmentEditView::on_cbDoctor_currentIndexChanged);

    qDebug() << "AppointmentEditView 初始化成功，索引：" << index;
}

AppointmentEditView::~AppointmentEditView()
{
    delete ui;
}

void AppointmentEditView::on_btSave_clicked()
{
    // 安全检查
    if (!dataMapper || !IDatabase::getInstance().appointmentTabModel) {
        QMessageBox::warning(this, "错误", "数据模型未初始化");
        return;
    }

    int row = dataMapper->currentIndex();
    if (row < 0 || row >= IDatabase::getInstance().appointmentTabModel->rowCount()) {
        QMessageBox::warning(this, "错误", "无效的行索引");
        return;
    }

    // 获取选中的患者和医生名称
    QString patientName = ui->cbPatient->currentText();
    QString doctorName = ui->cbDoctor->currentText();

    QString patientId = IDatabase::getInstance().getPatientIdByName(patientName);
    QString doctorId = IDatabase::getInstance().getDoctorIdByName(doctorName);
    QString appointmentTime = ui->dbDateTimeEdit->dateTime().toString("yyyy-MM-dd HH:mm:ss");

    // 检查时间冲突
    QString appointmentId = IDatabase::getInstance().appointmentTabModel->record(row).value("ID").toString();
    if (IDatabase::getInstance().checkTimeConflict(doctorId, appointmentTime, appointmentId)) {
        QMessageBox::warning(this, "警告", "该医生在所选时间已有预约，请选择其他时间！");
        return;
    }

    // 检查状态
    QString currentStatus = IDatabase::getInstance().appointmentTabModel->record(row).value("STATUS").toString();
    if (currentStatus == "已通过") {
        QMessageBox::warning(this, "警告", "已通过的预约不能修改");
        return;
    }

    // 手动保存PATIENT_ID和DOCTOR_ID
    QSqlRecord record = IDatabase::getInstance().appointmentTabModel->record(row);
    record.setValue("PATIENT_ID", patientId);
    record.setValue("DOCTOR_ID", doctorId);
    IDatabase::getInstance().appointmentTabModel->setRecord(row, record);

    // 保存数据
    if (IDatabase::getInstance().submitAppointmentEdit()) {
        QMessageBox::information(this, "成功", "预约信息保存成功");
        emit goPreviousView();
    } else {
        QMessageBox::critical(this, "错误", "保存预约信息失败");
    }
}

void AppointmentEditView::on_btCancel_clicked()
{
    IDatabase::getInstance().revertAppointmentEdit();
    emit goPreviousView();
}

void AppointmentEditView::on_cbPatient_currentIndexChanged(int index)
{
    Q_UNUSED(index);
    if (!dataMapper || !IDatabase::getInstance().appointmentTabModel) return;

    QString patientName = ui->cbPatient->currentText();
    QString patientId = IDatabase::getInstance().getPatientIdByName(patientName);
    if (!patientId.isEmpty()) {
        int row = dataMapper->currentIndex();
        if (row >= 0 && row < IDatabase::getInstance().appointmentTabModel->rowCount()) {
            QSqlRecord record = IDatabase::getInstance().appointmentTabModel->record(row);
            record.setValue("PATIENT_ID", patientId);
            IDatabase::getInstance().appointmentTabModel->setRecord(row, record);
        }
    }
}

void AppointmentEditView::on_cbDoctor_currentIndexChanged(int index)
{
    Q_UNUSED(index);
    if (!dataMapper || !IDatabase::getInstance().appointmentTabModel) return;

    QString doctorName = ui->cbDoctor->currentText();
    QString doctorId = IDatabase::getInstance().getDoctorIdByName(doctorName);
    QString appointmentTime = ui->dbDateTimeEdit->dateTime().toString("yyyy-MM-dd HH:mm:ss");

    if (!doctorId.isEmpty() && !appointmentTime.isEmpty()) {
        if (IDatabase::getInstance().checkTimeConflict(doctorId, appointmentTime)) {
            QMessageBox::warning(this, "警告", "该医生在所选时间已有预约，请选择其他时间！");
            ui->cbDoctor->setCurrentIndex(-1);
        } else {
            int row = dataMapper->currentIndex();
            if (row >= 0 && row < IDatabase::getInstance().appointmentTabModel->rowCount()) {
                QSqlRecord record = IDatabase::getInstance().appointmentTabModel->record(row);
                record.setValue("DOCTOR_ID", doctorId);
                IDatabase::getInstance().appointmentTabModel->setRecord(row, record);
            }
        }
    }
}
