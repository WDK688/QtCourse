#include "departmenteditview.h"
#include "ui_departmenteditview.h"
#include "idatabase.h"
#include <QSqlTableModel>

DepartmentEditView::DepartmentEditView(QWidget *parent, int index)
    : QWidget(parent)
    , ui(new Ui::DepartmentEditView)
    , currentIndex(index)
{
    ui->setupUi(this);

    // 初始化数据映射器
    dataMapper = new QDataWidgetMapper(this);
    QSqlTableModel *tabModel = IDatabase::getInstance().departmentTabModel;
    dataMapper->setModel(tabModel);

    dataMapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
    // 建立控件与数据库字段的映射关系
    dataMapper->addMapping(ui->dbEditDeptId, tabModel->fieldIndex("ID"));
    dataMapper->addMapping(ui->dbEditDeptName, tabModel->fieldIndex("NAME"));


    // 设置当前编辑的行
    dataMapper->setCurrentIndex(index);

    // 连接信号槽
    connect(ui->btSave, &QPushButton::clicked, this, &DepartmentEditView::on_btSave_clicked);
    connect(ui->btCancel, &QPushButton::clicked, this, &DepartmentEditView::on_btCancel_clicked);
}

DepartmentEditView::~DepartmentEditView()
{
    delete ui;
}

void DepartmentEditView::on_btSave_clicked()
{
    // 先让 DataMapper 提交所有挂起的更改到模型
    if (dataMapper->submit()) {
        // 然后将模型的更改提交到数据库
        IDatabase::getInstance().submitDepartmentEdit();
        emit goPreviousView();
    } else {
        //qDebug() << "DataMapper submit failed: " << dataMapper->model()->lastError().text();
    }
}

void DepartmentEditView::on_btCancel_clicked()
{
    IDatabase::getInstance().revertDepartmentEdit();
    emit goPreviousView();
}
