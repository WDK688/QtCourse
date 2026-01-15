#include "departmentview.h"
#include "ui_departmentview.h"

DepartmentView::DepartmentView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::DepartmentView)
{
    ui->setupUi(this);
}

DepartmentView::~DepartmentView()
{
    delete ui;
}

void DepartmentView::on_btSearch_clicked()
{

}


void DepartmentView::on_btAdd_clicked()
{

}


void DepartmentView::on_btDelete_clicked()
{

}


void DepartmentView::on_btEdit_clicked()
{

}

