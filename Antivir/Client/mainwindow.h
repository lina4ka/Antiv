#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include "reportwindow.h"
#include <memory>
#include <math.h>
#include "IO_.h"
#include "worker.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_bChose_clicked();

    void on_bScan_clicked();

    void on_bStop_clicked();

    void on_bReport_clicked();

    void on_bBack_clicked();

    void on_bToQuar_clicked();

    void on_bReturnQuar_clicked();

    void on_bDelete_clicked();

    void on_pScanMonit_clicked();

    void on_bBackSM_clicked();

    void on_bMonit_clicked();

    void on_bChose_2_clicked();

    void on_bMonitEnd_clicked();

    void on_bTime_clicked();

    void on_pushButton_clicked();

    void on_bStopServ_clicked();

    void on_bStartServ_clicked();
    void reconnect();
    void toTo (msg Message);


private:
    Ui::MainWindow *ui;
    QThread m_thread;
    Worker m_worker;
    unique_ptr<Sender> outerSender,innerSender;
    HANDLE oHandle,iHandle;
    void startService();
    bool isStop = false;

};
#endif // MAINWINDOW_H
