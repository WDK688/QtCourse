#include "doctoreditview.h"
#include "ui_doctoreditview.h"
#include "idatabase.h"
#include <QSqlTableModel>
#include <QSqlQueryModel>
#include <QMessageBox>

DoctorEditView::DoctorEditView(QWidget *parent, int index)
    : QWidget(parent)
    , ui(new Ui::DoctorEditView)
    , currentIndex(index)
{
    ui->setupUi(this);

    // 初始化科室下拉框
    IDatabase &iDatabase = IDatabase::getInstance();
    QSqlTableModel *deptModel = iDatabase.departmentTabModel;
    ui->dbComboDept->setModel(deptModel);
    ui->dbComboDept->setModelColumn(deptModel->fieldIndex("name"));

    // 初始化数据映射器
    dataMapper = new QDataWidgetMapper(this);
    QSqlTableModel *tabModel = iDatabase.doctorTabModel;
    dataMapper->setModel(tabModel);
    dataMapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);

    // 建立控件与数据库字段的映射关系
    dataMapper->addMapping(ui->dbEditDoctorId, tabModel->fieldIndex("ID"));
    dataMapper->addMapping(ui->dbEditName, tabModel->fieldIndex("NAME"));
    dataMapper->addMapping(ui->dbComboDept, tabModel->fieldIndex("DEPARTMENT_ID"));
    dataMapper->addMapping(ui->dbEditTitle, tabModel->fieldIndex("EMPLOYEENO"));


    // 设置当前编辑的行
    dataMapper->setCurrentIndex(index);

    // 连接信号槽
    connect(ui->btSave, &QPushButton::clicked, this, &DoctorEditView::on_btSave_clicked);
    connect(ui->btCancel, &QPushButton::clicked, this, &DoctorEditView::on_btCancel_clicked);
}

DoctorEditView::~DoctorEditView()
{
    delete ui;
}

void DoctorEditView::on_btSave_clicked()
{
    // 先让 DataMapper 提交所有挂起的更改到模型
    if (dataMapper->submit()) {
        // 然后将模型的更改提交到数据库
        IDatabase::getInstance().submitDoctorEdit();
        emit goPreviousView();
    } else {
        //qDebug() << "DataMapper submit failed: " << dataMapper->model()->lastError().text();
    }
}

void DoctorEditView::on_btCancel_clicked()
{
    // 取消编辑，回滚更改
    IDatabase::getInstance().revertDoctorEdit();
    emit goPreviousView();
}
