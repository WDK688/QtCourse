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
    dataMapper->setSubmitPolicy(QDataWidgetMapper::AutoSubmit);

    // 建立控件与数据库字段的映射关系
    dataMapper->addMapping(ui->dbEditDeptId, tabModel->fieldIndex("id"));
    dataMapper->addMapping(ui->dbEditDeptName, tabModel->fieldIndex("name"));


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
    IDatabase::getInstance().submitDepartmentEdit();
    emit goPreviousView();
}

void DepartmentEditView::on_btCancel_clicked()
{
    IDatabase::getInstance().revertDepartmentEdit();
    emit goPreviousView();
}
