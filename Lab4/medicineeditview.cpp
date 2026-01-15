#include "medicineeditview.h"
#include "ui_medicineeditview.h"

medicineeditview::medicineeditview(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::medicineeditview)
{
    ui->setupUi(this);
}

medicineeditview::~medicineeditview()
{
    delete ui;
}
