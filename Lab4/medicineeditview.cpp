#include "medicineeditview.h"
#include "ui_medicineeditview.h"
#include "idatabase.h"

MedicineEditView::MedicineEditView(QWidget *parent, int rowNo)
    : QWidget(parent)
    , ui(new Ui::MedicineEditView)
{
    ui->setupUi(this);
    currentRowNo = rowNo;

    IDatabase &iDatabase = IDatabase::getInstance();
    iDatabase.medicineTabModel->setFilter(QString("ID='%1'").arg(
        iDatabase.medicineTabModel->record(rowNo).value("ID").toString()));
    if (iDatabase.medicineTabModel->select()) {
        if (iDatabase.medicineTabModel->rowCount() > 0) {
            ui->leCode->setText(iDatabase.medicineTabModel->record(0).value("CODE").toString());
            ui->leName->setText(iDatabase.medicineTabModel->record(0).value("NAME").toString());
            ui->leSpec->setText(iDatabase.medicineTabModel->record(0).value("SPECIFICATION").toString());
            ui->leManufacturer->setText(iDatabase.medicineTabModel->record(0).value("MANUFACTURER").toString());
            ui->dbPrice->setValue(iDatabase.medicineTabModel->record(0).value("PRICE").toDouble());
            ui->sbQuantity->setValue(iDatabase.medicineTabModel->record(0).value("QUANTITY").toInt());
            ui->sbMinThreshold->setValue(iDatabase.medicineTabModel->record(0).value("MIN_THRESHOLD").toInt());
        }
    }
    iDatabase.medicineTabModel->setFilter("");
}

MedicineEditView::~MedicineEditView()
{
    delete ui;
}

void MedicineEditView::on_btSave_clicked()
{
    IDatabase &iDatabase = IDatabase::getInstance();
    iDatabase.medicineTabModel->setFilter(QString("ID='%1'").arg(
        iDatabase.medicineTabModel->record(currentRowNo).value("ID").toString()));
    if (iDatabase.medicineTabModel->select()) {
        if (iDatabase.medicineTabModel->rowCount() > 0) {
            iDatabase.medicineTabModel->setData(iDatabase.medicineTabModel->index(0, 1), ui->leCode->text());
            iDatabase.medicineTabModel->setData(iDatabase.medicineTabModel->index(0, 2), ui->leName->text());
            iDatabase.medicineTabModel->setData(iDatabase.medicineTabModel->index(0, 3), ui->leSpec->text());
            iDatabase.medicineTabModel->setData(iDatabase.medicineTabModel->index(0, 4), ui->leManufacturer->text());
            iDatabase.medicineTabModel->setData(iDatabase.medicineTabModel->index(0, 5), ui->dbPrice->value());
            iDatabase.medicineTabModel->setData(iDatabase.medicineTabModel->index(0, 6), ui->sbQuantity->value());
            iDatabase.medicineTabModel->setData(iDatabase.medicineTabModel->index(0, 7), ui->sbMinThreshold->value());
            iDatabase.submitMedicineEdit();
        }
    }
    emit goPreviousView();
}

void MedicineEditView::on_btCancel_clicked()
{
    IDatabase::getInstance().revertMedicineEdit();
    emit goPreviousView();
}
