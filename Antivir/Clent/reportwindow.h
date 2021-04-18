#ifndef REPORTWINDOW_H
#define REPORTWINDOW_H

#include <QWidget>

namespace Ui {
class ReportWindow;
}

class ReportWindow : public QWidget
{
    Q_OBJECT

public:
    explicit ReportWindow(QWidget *parent = nullptr);
    ~ReportWindow();

private:
    Ui::ReportWindow *ui;
};

#endif // REPORTWINDOW_H
