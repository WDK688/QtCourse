#include "prescriptioneditview.h"
#include "ui_prescriptioneditview.h"
#include "idatabase.h"
#include <QSqlTableModel>
#include <QSqlQueryModel>
#include <QMessageBox>

PrescriptionEditView::PrescriptionEditView(QWidget *parent, int rowNo)
    : QWidget(parent)
    , ui(new Ui::PrescriptionEditView)
    , currentIndex(rowNo)
{
    ui->setupUi(this);

    IDatabase &iDatabase = IDatabase::getInstance();

    // 初始化患者下拉框 - 显示姓名，存储ID
    QSqlTableModel *patientModel = iDatabase.patientTabModel;
    ui->comboPatient->setModel(patientModel);
    ui->comboPatient->setModelColumn(patientModel->fieldIndex("NAME"));

    // 初始化医生下拉框 - 显示姓名，存储ID
    QSqlTableModel *doctorModel = iDatabase.doctorTabModel;
    ui->comboDoctor->setModel(doctorModel);
    ui->comboDoctor->setModelColumn(doctorModel->fieldIndex("NAME"));

    // 初始化药品下拉框 - 显示名称，存储ID
    QSqlTableModel *medicineModel = iDatabase.medicineTabModel;
    ui->comboMedicine->setModel(medicineModel);
    ui->comboMedicine->setModelColumn(medicineModel->fieldIndex("NAME"));

    // 初始化数据映射器
    dataMapper = new QDataWidgetMapper(this);
    QSqlTableModel *tabModel = iDatabase.prescriptionTabModel;
    dataMapper->setModel(tabModel);
    dataMapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);

    // 建立控件与数据库字段的映射关系
    dataMapper->addMapping(ui->comboPatient, tabModel->fieldIndex("PATIENT_ID"));
    dataMapper->addMapping(ui->comboDoctor, tabModel->fieldIndex("DOCTOR_ID"));
    dataMapper->addMapping(ui->comboMedicine, tabModel->fieldIndex("MEDICINE_ID"));
    dataMapper->addMapping(ui->sbQuantity, tabModel->fieldIndex("QUANTITY"));
    dataMapper->addMapping(ui->leDosage, tabModel->fieldIndex("DOSAGE"));

    // 设置当前编辑的行
    dataMapper->setCurrentIndex(rowNo);

    // 连接信号槽
    connect(ui->btSave, &QPushButton::clicked, this, &PrescriptionEditView::on_btSave_clicked);
    connect(ui->btCancel, &QPushButton::clicked, this, &PrescriptionEditView::on_btCancel_clicked);
}

PrescriptionEditView::~PrescriptionEditView()
{
    delete ui;
}

void PrescriptionEditView::on_btSave_clicked()
{
    // 先让 DataMapper 提交所有挂起的更改到模型
    if (dataMapper->submit()) {
        // 然后将模型的更改提交到数据库
        IDatabase::getInstance().submitPrescriptionEdit();
        emit goPreviousView();
    } else {
        // 如果 DataMapper 提交失败，显示错误信息
       // qDebug() << "DataMapper submit failed: " << dataMapper->model()->lastError().text();
        QMessageBox::warning(this, "提交失败", "数据提交失败，请检查输入是否正确");
    }
}

void PrescriptionEditView::on_btCancel_clicked()
{
    // 取消编辑，回滚更改
    IDatabase::getInstance().revertPrescriptionEdit();
    emit goPreviousView();
}
