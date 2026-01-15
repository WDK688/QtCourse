#ifndef MEDICINEEDITVIEW_H
#define MEDICINEEDITVIEW_H

#include <QWidget>

namespace Ui {
class medicineeditview;
}

class medicineeditview : public QWidget
{
    Q_OBJECT

public:
    explicit medicineeditview(QWidget *parent = nullptr);
    ~medicineeditview();

private:
    Ui::medicineeditview *ui;
};

#endif // MEDICINEEDITVIEW_H
