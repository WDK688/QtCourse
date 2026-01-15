#include "prescriptioneditview.h"
#include "ui_prescriptioneditview.h"

prescriptioneditview::prescriptioneditview(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::prescriptioneditview)
{
    ui->setupUi(this);
}

prescriptioneditview::~prescriptioneditview()
{
    delete ui;
}
