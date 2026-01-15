#ifndef APPOINTMENTEDITVIEW_H
#define APPOINTMENTEDITVIEW_H

#include <QWidget>
#include <QPushButton>
#include <QComboBox>
#include <QDataWidgetMapper>

namespace Ui {
class AppointmentEditView;
}

class AppointmentEditView : public QWidget
{
    Q_OBJECT

public:
    explicit AppointmentEditView(QWidget *parent = nullptr, int index = 0);
    ~AppointmentEditView();

private slots:
    void on_btSave_clicked();
    void on_btCancel_clicked();
    void on_cbPatient_currentIndexChanged(int index);
    void on_cbDoctor_currentIndexChanged(int index);

private:
    Ui::AppointmentEditView *ui;
    QDataWidgetMapper *dataMapper;
    int currentIndex;  // ← 新增的成员变量
    void loadPatientNames();
    void loadDoctorNames();

signals:
    void goPreviousView();
};

#endif // APPOINTMENTEDITVIEW_H
