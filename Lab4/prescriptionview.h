#ifndef PRESCRIPTIONVIEW_H
#define PRESCRIPTIONVIEW_H

#include <QWidget>

namespace Ui {
class PrescriptionView;
}

class PrescriptionView : public QWidget
{
    Q_OBJECT

public:
    explicit PrescriptionView(QWidget *parent = nullptr);
    ~PrescriptionView();

private slots:
    void on_btAdd_clicked();
    void on_btSearch_clicked();
    void on_btDelete_clicked();
    void on_btEdit_clicked();

signals:
    void goPrescriptionEditView(int idx);

private:
    Ui::PrescriptionView *ui;
};

#endif // PRESCRIPTIONVIEW_H
