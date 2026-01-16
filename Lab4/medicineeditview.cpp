// #include "medicineeditview.h"
// #include "ui_medicineeditview.h"
// #include "idatabase.h"

// MedicineEditView::MedicineEditView(QWidget *parent, int rowNo)
//     : QWidget(parent)
//     , ui(new Ui::MedicineEditView)
// {
//     ui->setupUi(this);
//     currentRowNo = rowNo;

//     IDatabase &iDatabase = IDatabase::getInstance();
//     iDatabase.medicineTabModel->setFilter(QString("ID='%1'").arg(
//         iDatabase.medicineTabModel->record(rowNo).value("ID").toString()));
//     if (iDatabase.medicineTabModel->select()) {
//         if (iDatabase.medicineTabModel->rowCount() > 0) {
//             ui->leCode->setText(iDatabase.medicineTabModel->record(0).value("CODE").toString());
//             ui->leName->setText(iDatabase.medicineTabModel->record(0).value("NAME").toString());
//             ui->leSpec->setText(iDatabase.medicineTabModel->record(0).value("SPECIFICATION").toString());
//             ui->leManufacturer->setText(iDatabase.medicineTabModel->record(0).value("MANUFACTURER").toString());
//             ui->dbPrice->setValue(iDatabase.medicineTabModel->record(0).value("PRICE").toDouble());
//             ui->sbQuantity->setValue(iDatabase.medicineTabModel->record(0).value("QUANTITY").toInt());
//             ui->sbMinThreshold->setValue(iDatabase.medicineTabModel->record(0).value("MIN_THRESHOLD").toInt());
//         }
//     }
//     iDatabase.medicineTabModel->setFilter("");
// }

// MedicineEditView::~MedicineEditView()
// {
//     delete ui;
// }

// void MedicineEditView::on_btSave_clicked()
// {
//     IDatabase &iDatabase = IDatabase::getInstance();
//     iDatabase.medicineTabModel->setFilter(QString("ID='%1'").arg(
//         iDatabase.medicineTabModel->record(currentRowNo).value("ID").toString()));
//     if (iDatabase.medicineTabModel->select()) {
//         if (iDatabase.medicineTabModel->rowCount() > 0) {
//             iDatabase.medicineTabModel->setData(iDatabase.medicineTabModel->index(0, 1), ui->leCode->text());
//             iDatabase.medicineTabModel->setData(iDatabase.medicineTabModel->index(0, 2), ui->leName->text());
//             iDatabase.medicineTabModel->setData(iDatabase.medicineTabModel->index(0, 3), ui->leSpec->text());
//             iDatabase.medicineTabModel->setData(iDatabase.medicineTabModel->index(0, 4), ui->leManufacturer->text());
//             iDatabase.medicineTabModel->setData(iDatabase.medicineTabModel->index(0, 5), ui->dbPrice->value());
//             iDatabase.medicineTabModel->setData(iDatabase.medicineTabModel->index(0, 6), ui->sbQuantity->value());
//             iDatabase.medicineTabModel->setData(iDatabase.medicineTabModel->index(0, 7), ui->sbMinThreshold->value());
//             iDatabase.submitMedicineEdit();
//         }
//     }
//     emit goPreviousView();
// }

// void MedicineEditView::on_btCancel_clicked()
// {
//     IDatabase::getInstance().revertMedicineEdit();
//     emit goPreviousView();
// }
#include "medicineeditview.h"
#include "ui_medicineeditview.h"
#include "idatabase.h"
#include <QSqlTableModel>

MedicineEditView::MedicineEditView(QWidget *parent, int rowNo)
    : QWidget(parent)
    , ui(new Ui::MedicineEditView)
{
    ui->setupUi(this);
    currentRowNo = rowNo;

    // 初始化数据映射器
    dataMapper = new QDataWidgetMapper(this);
    IDatabase &iDatabase = IDatabase::getInstance();
    QSqlTableModel *tabModel = iDatabase.medicineTabModel;
    dataMapper->setModel(tabModel);
    dataMapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);  // 手动提交

    // 建立控件与数据库字段的映射关系
    dataMapper->addMapping(ui->leCode, tabModel->fieldIndex("CODE"));
    dataMapper->addMapping(ui->leName, tabModel->fieldIndex("NAME"));
    dataMapper->addMapping(ui->leSpec, tabModel->fieldIndex("SPECIFICATION"));
    dataMapper->addMapping(ui->leManufacturer, tabModel->fieldIndex("MANUFACTURER"));
    dataMapper->addMapping(ui->dbPrice, tabModel->fieldIndex("PRICE"));
    dataMapper->addMapping(ui->sbQuantity, tabModel->fieldIndex("QUANTITY"));
    dataMapper->addMapping(ui->sbMinThreshold, tabModel->fieldIndex("MIN_THRESHOLD"));

    // 设置当前编辑的行
    dataMapper->setCurrentIndex(rowNo);

    // 连接信号槽
    connect(ui->btSave, &QPushButton::clicked, this, &MedicineEditView::on_btSave_clicked);
    connect(ui->btCancel, &QPushButton::clicked, this, &MedicineEditView::on_btCancel_clicked);
}

MedicineEditView::~MedicineEditView()
{
    delete ui;
}

void MedicineEditView::on_btSave_clicked()
{
    // 先让 DataMapper 提交所有挂起的更改到模型
    if (dataMapper->submit()) {
        // 然后将模型的更改提交到数据库
        IDatabase::getInstance().submitMedicineEdit();
        emit goPreviousView();
    } else {
        // 如果 DataMapper 提交失败，显示错误信息
        //qDebug() << "DataMapper submit failed: " << dataMapper->model()->lastError().text();
    }
}

void MedicineEditView::on_btCancel_clicked()
{
    IDatabase::getInstance().revertMedicineEdit();
    emit goPreviousView();
}
