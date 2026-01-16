#ifndef APPOINTMENTEDITVIEW_H
#define APPOINTMENTEDITVIEW_H

#include <QWidget>
#include <QDataWidgetMapper>

namespace Ui {
class AppointmentEditView;
}

class AppointmentEditView : public QWidget
{
    Q_OBJECT

public:
    explicit AppointmentEditView(QWidget *parent = nullptr, int rowNo = -1);
    ~AppointmentEditView();

private slots:
    void on_btSave_clicked();
    void on_btCancel_clicked();

signals:
    void goPreviousView();

private:
    Ui::AppointmentEditView *ui;
    QDataWidgetMapper *dataMapper;
    int currentIndex;
};

#endif // APPOINTMENTEDITVIEW_H
