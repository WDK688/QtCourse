#ifndef PRESCRIPTIONVIEW_H
#define PRESCRIPTIONVIEW_H

#include <QWidget>

namespace Ui {
class prescriptionview;
}

class prescriptionview : public QWidget
{
    Q_OBJECT

public:
    explicit prescriptionview(QWidget *parent = nullptr);
    ~prescriptionview();

private:
    Ui::prescriptionview *ui;
};

#endif // PRESCRIPTIONVIEW_H
