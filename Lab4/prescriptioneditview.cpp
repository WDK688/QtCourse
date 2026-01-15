#include "prescriptioneditview.h"
#include "ui_prescriptioneditview.h"
#include "idatabase.h"

PrescriptionEditView::PrescriptionEditView(QWidget *parent, int rowNo)
    : QWidget(parent)
    , ui(new Ui::PrescriptionEditView)
{
    ui->setupUi(this);
    currentRowNo = rowNo;

    // 初始化下拉框数据
    IDatabase &iDatabase = IDatabase::getInstance();

    // 填充患者下拉框
    QStringList patients = iDatabase.getAllPatientNames();
    ui->comboPatient->addItems(patients);

    // 填充医生下拉框
    QStringList doctors = iDatabase.getAllDoctorNames();
    ui->comboDoctor->addItems(doctors);

    // 填充药品下拉框
    QStringList medicines = iDatabase.getAllMedicineNames();
    ui->comboMedicine->addItems(medicines);

    // 如果是编辑模式，加载现有数据
    if (rowNo >= 0) {
        iDatabase.prescriptionTabModel->setFilter(QString("ID='%1'").arg(
            iDatabase.prescriptionTabModel->record(rowNo).value("ID").toString()));
        if (iDatabase.prescriptionTabModel->select()) {
            if (iDatabase.prescriptionTabModel->rowCount() > 0) {
                // 设置患者
                int patientIndex = ui->comboPatient->findText(
                    iDatabase.prescriptionTabModel->record(0).value("PATIENT_NAME").toString());
                if (patientIndex >= 0) ui->comboPatient->setCurrentIndex(patientIndex);

                // 设置医生
                int doctorIndex = ui->comboDoctor->findText(
                    iDatabase.prescriptionTabModel->record(0).value("DOCTOR_NAME").toString());
                if (doctorIndex >= 0) ui->comboDoctor->setCurrentIndex(doctorIndex);

                // 设置药品
                int medicineIndex = ui->comboMedicine->findText(
                    iDatabase.prescriptionTabModel->record(0).value("MEDICINE_NAME").toString());
                if (medicineIndex >= 0) ui->comboMedicine->setCurrentIndex(medicineIndex);

                // 设置其他字段
                ui->sbQuantity->setValue(iDatabase.prescriptionTabModel->record(0).value("QUANTITY").toInt());
                ui->leDosage->setText(iDatabase.prescriptionTabModel->record(0).value("DOSAGE").toString());
            }
        }
        iDatabase.prescriptionTabModel->setFilter("");
    }
}

PrescriptionEditView::~PrescriptionEditView()
{
    delete ui;
}

void PrescriptionEditView::on_btSave_clicked()
{
    IDatabase &iDatabase = IDatabase::getInstance();

    // 获取选择的ID
    int patientRow = ui->comboPatient->currentIndex();
    int doctorRow = ui->comboDoctor->currentIndex();
    int medicineRow = ui->comboMedicine->currentIndex();

    if (patientRow < 0 || doctorRow < 0 || medicineRow < 0) {
        return;
    }

    QString patientId = iDatabase.getPatientIdByName(ui->comboPatient->currentText());
    QString doctorId = iDatabase.getDoctorIdByName(ui->comboDoctor->currentText());
    QString medicineId = iDatabase.getMedicineIdByName(ui->comboMedicine->currentText());

    if (currentRowNo >= 0) {
        // 编辑模式
        iDatabase.prescriptionTabModel->setFilter(QString("ID='%1'").arg(
            iDatabase.prescriptionTabModel->record(currentRowNo).value("ID").toString()));
        if (iDatabase.prescriptionTabModel->select()) {
            if (iDatabase.prescriptionTabModel->rowCount() > 0) {
                iDatabase.prescriptionTabModel->setData(iDatabase.prescriptionTabModel->index(0, 1), patientId);
                iDatabase.prescriptionTabModel->setData(iDatabase.prescriptionTabModel->index(0, 2), doctorId);
                iDatabase.prescriptionTabModel->setData(iDatabase.prescriptionTabModel->index(0, 3), medicineId);
                iDatabase.prescriptionTabModel->setData(iDatabase.prescriptionTabModel->index(0, 4), ui->sbQuantity->value());
                iDatabase.prescriptionTabModel->setData(iDatabase.prescriptionTabModel->index(0, 5), ui->leDosage->text());
                iDatabase.submitPrescriptionEdit();
            }
        }
        iDatabase.prescriptionTabModel->setFilter("");
    } else {
        // 新增模式
        iDatabase.prescriptionTabModel->insertRow(iDatabase.prescriptionTabModel->rowCount(), QModelIndex());
        int row = iDatabase.prescriptionTabModel->rowCount() - 1;

        iDatabase.prescriptionTabModel->setData(iDatabase.prescriptionTabModel->index(row, 1), patientId);
        iDatabase.prescriptionTabModel->setData(iDatabase.prescriptionTabModel->index(row, 2), doctorId);
        iDatabase.prescriptionTabModel->setData(iDatabase.prescriptionTabModel->index(row, 3), medicineId);
        iDatabase.prescriptionTabModel->setData(iDatabase.prescriptionTabModel->index(row, 4), ui->sbQuantity->value());
        iDatabase.prescriptionTabModel->setData(iDatabase.prescriptionTabModel->index(row, 5), ui->leDosage->text());
        iDatabase.prescriptionTabModel->setData(iDatabase.prescriptionTabModel->index(row, 6),
                                                QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"));
        iDatabase.prescriptionTabModel->submitAll();
    }

    emit goPreviousView();
}

void PrescriptionEditView::on_btCancel_clicked()
{
    IDatabase::getInstance().revertPrescriptionEdit();
    emit goPreviousView();
}
