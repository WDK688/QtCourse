#include "prescriptionview.h"
#include "ui_prescriptionview.h"

prescriptionview::prescriptionview(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::prescriptionview)
{
    ui->setupUi(this);
}

prescriptionview::~prescriptionview()
{
    delete ui;
}
