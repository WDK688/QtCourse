#ifndef PRESCRIPTIONEDITVIEW_H
#define PRESCRIPTIONEDITVIEW_H

#include <QWidget>

namespace Ui {
class prescriptioneditview;
}

class prescriptioneditview : public QWidget
{
    Q_OBJECT

public:
    explicit prescriptioneditview(QWidget *parent = nullptr);
    ~prescriptioneditview();

private:
    Ui::prescriptioneditview *ui;
};

#endif // PRESCRIPTIONEDITVIEW_H
