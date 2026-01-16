#ifndef PRESCRIPTIONEDITVIEW_H
#define PRESCRIPTIONEDITVIEW_H

#include <QWidget>
#include <QDataWidgetMapper>

namespace Ui {
class PrescriptionEditView;
}

class PrescriptionEditView : public QWidget
{
    Q_OBJECT

public:
    explicit PrescriptionEditView(QWidget *parent = nullptr, int rowNo = -1);
    ~PrescriptionEditView();

private slots:
    void on_btSave_clicked();
    void on_btCancel_clicked();

signals:
    void goPreviousView();

private:
    Ui::PrescriptionEditView *ui;
    QDataWidgetMapper *dataMapper;
    int currentIndex;
};

#endif // PRESCRIPTIONEDITVIEW_H
