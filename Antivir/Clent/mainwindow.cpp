#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QFileDialog"
#include <windows.h>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    QLocale::setDefault(QLocale(QString("rus_rus")));

    ui->setupUi(this);
    m_worker.moveToThread(&m_thread);
    connect(&m_thread, &QThread::started, &m_worker, &Worker::doWork);
    connect(&m_worker, &Worker::reconnect, this, &MainWindow::reconnect);
    connect(&m_worker, &Worker::toDo, this, &MainWindow::toTo);
    iHandle = oHandle = NULL;
    outerSender.reset(new Sender(oHandle));
    innerSender.reset(new Sender(iHandle));
    ui->timeEdit->setTime(QTime::currentTime());
    m_thread.start();
}

MainWindow::~MainWindow()
{

    m_worker.isWork = false;
    isStop = true;
    m_thread.wait();
    m_thread.quit();
    m_thread.exit();
    m_thread.terminate();
     delete ui;


}


void MainWindow::on_bChose_clicked()
{
    QString path;
    if(ui->rbFile->isChecked())
        path=QFileDialog::getOpenFileName(this,
                  tr("Выберите файл"), "",
                  tr("(*.*);"));
    else if (ui->rbDir->isChecked())
        path=QFileDialog::getExistingDirectory (this,
                  tr("Выберите папку"));
    ui->textEdit->setText(path);
}

void MainWindow::on_bScan_clicked()
{
    if (!outerSender)
        return;
    string arg = ui->textEdit->toPlainText().toStdString();
    if (!arg.empty())
        outerSender->sendTo(COMMANDLIST::START_SCAN,{arg});
}

void MainWindow::on_bStop_clicked()
{
    if (!outerSender)
        return;
    outerSender->sendTo(COMMANDLIST::STOP_SCAN);
}

void MainWindow::on_bReport_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_bBack_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_bToQuar_clicked()
{
    if (!outerSender)
        return;
    int64_t index = ui->lDetec->currentIndex().row();
    if (index != -1)
        outerSender->sendTo(COMMANDLIST::MOVE_TO_QUARANTINE,index);

}

void MainWindow::on_bReturnQuar_clicked()
{
    if (!outerSender)
        return;
    int64_t index = ui->lQuar->currentIndex().row();
    if (index != -1)
        outerSender->sendTo(COMMANDLIST::RETURN_FROM_QUARANTINE,index);
}

void MainWindow::on_bDelete_clicked()
{
    if (!outerSender)
        return;
    int64_t index = ui->lDetec->currentIndex().row();
    if (index != -1)
        outerSender->sendTo(COMMANDLIST::DELETE_FILE,index);
}

void MainWindow::on_pScanMonit_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}

void MainWindow::on_bBackSM_clicked()
{
     ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_bMonit_clicked()
{
    if (!outerSender)
        return;
    string arg = ui->textEdit_2->toPlainText().toStdString();
    if (!arg.empty())
        outerSender->sendTo(COMMANDLIST::START_MONITOR,{arg});
}

void MainWindow::on_bChose_2_clicked()
{
    QString path=QFileDialog::getExistingDirectory (this,
                                                    tr("Выберите папку"));
    ui->textEdit_2->setText(path);
}

void MainWindow::on_bMonitEnd_clicked()
{
    if (!outerSender)
        return;
    int64_t index = ui->lMonit->currentIndex().row();
    if (index != -1)
        outerSender->sendTo(COMMANDLIST::END_MONITOR,index);
}

void MainWindow::on_bTime_clicked()
{
    if (!outerSender)
        return;
    string argPath = ui->textEdit_2->toPlainText().toStdString();
    if (!argPath.empty())
    {
        uint64_t argTime = ui->timeEdit->time().hour()*60+ui->timeEdit->time().minute();
        outerSender->sendTo(COMMANDLIST::START_SCHEDULE,argTime,argPath);
    }

}

void MainWindow::on_pushButton_clicked()
{
    if (!outerSender)
        return;
    string index = ui->lTime->currentItem()->text().toStdString();
    if (!index.empty())
    {
        string time = index.substr(index.length()-5);
        string path = index.substr(0,index.length()-6);
        string a = time.substr(0,2);
         string b = time.substr(3);
        uint64_t _time = stoi(time.substr(0,2))*60+stoi(time.substr(3));
        outerSender->sendTo(COMMANDLIST::STOP_SCHEDULE,_time,path);
    }
}

void MainWindow::on_bStopServ_clicked()
{
    if (!outerSender)
        return;
     outerSender->sendTo(COMMANDLIST::OFF);
}

void MainWindow::on_bStartServ_clicked()
{
    system("C:\\Users\\fdn00\\Desktop\\ANTIV\\Service\\x64\\Debug\\ServerOn.exe");
}

void MainWindow::reconnect()
{
    CloseHandle(oHandle);
    oHandle = NULL;

        oHandle = CreateFile(
                        L"\\\\.\\pipe\\AntimalwarePipe", GENERIC_READ | GENERIC_WRITE,
                        0, NULL, OPEN_EXISTING, 0, NULL);

    outerSender->changeHandle(oHandle);
    if (oHandle != NULL && oHandle != INVALID_HANDLE_VALUE)
        outerSender->sendTo(COMMANDLIST::UPDATE_INFO);
}


void MainWindow::toTo(msg Message)
{
    switch (Message.command)
    {
    case COMMANDLIST::UNKNOWN:
        break;
    case COMMANDLIST::STOP_SCAN:
         ui->textBrowser->append(QString::fromStdString(Message.stringArray.at(0)));
        break;
    case COMMANDLIST::START_SCAN:
        if (Message.numericArray.size()==1)
            ui->progressBar->setMaximum(Message.numericArray.at(0));
        else
              ui->textBrowser->append("\r\nКоличество просканированных файлов = " + QString::fromStdString(to_string(Message.numericArray.at(0)))
                                      +"\r\nКоличество обнаруженных угроз = " + QString::fromStdString(to_string(Message.numericArray.at(1)))
                                      +"\r\nКоличество безопасных сайтов = " + QString::fromStdString(to_string(Message.numericArray.at(2))));
        break;
    case COMMANDLIST::GET_REPORT:
        for (size_t i = 0; i < Message.stringArray.size();++i)
        {
            ui->progressBar->setValue(ui->progressBar->value()+1);
            QString res = QString::fromStdString(Message.stringArray.at(i));
            if (Message.numericArray.at(i)==2)
                res+=" не является PE";
            else if (Message.numericArray.at(i)==1)
                res+=" обнаружен вирус";
            else
                res+=" безопасен";
            ui->textBrowser->append(res);
        }
        break;
    case COMMANDLIST::START_MONITOR:
        ui->lMonit->clear();
         for (auto const & el : Message.stringArray)
             ui->lMonit->addItem(QString::fromStdString(el));
        break;
    case COMMANDLIST::END_MONITOR:
        for (auto const & el : Message.numericArray)
            delete ui->lMonit->itemAt(0,el);
        break;
    case COMMANDLIST::START_SCHEDULE:
        for (size_t index = 0; index < Message.numericArray.size(); ++index)
        {
            div_t divresult = div(Message.numericArray.at(index),60);
            string result = Message.stringArray.at(index) +" "+
                    to_string(divresult.quot)+":";
            if (divresult.rem<10)
                result+="0"+to_string(divresult.rem);
            else
                result+=to_string(divresult.rem);
            ui->lTime->addItem(QString::fromStdString(result));
        }
        break;
    case COMMANDLIST::STOP_SCHEDULE:
        for (size_t index = 0; index < Message.numericArray.size(); ++index)
        {
            div_t divresult = div(Message.numericArray.at(index),60);
            string result = Message.stringArray.at(index) +" "+
                    to_string(divresult.quot)+":";
            if (divresult.rem<10)
                result+="0"+to_string(divresult.rem);
            else
                result+=to_string(divresult.rem);
            auto count =  ui->lTime->count();
            for ( int a = 0; a < count; ++a )
                if ( ui->lTime->item(a)->text() == QString::fromStdString(result) )
                {
                    delete ui->lTime->takeItem(a);
                    break;
                }
        }
        break;
    case COMMANDLIST::UPDATE_INFO:
    {
        ui->lDetec->clear();
        ui->lQuar->clear();
        bool isFirst = true;
        for (auto const & el : Message.stringArray)
            if (el == "$Quar")
                isFirst= false;
            else if (isFirst)
                ui->lDetec->addItem(QString::fromStdString(el));
            else
                ui->lQuar->addItem(QString::fromStdString(el));
        break;
    }
    default:
        break;
    }
}

void MainWindow::startService()
{

   /* SC_HANDLE schSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_CONNECT);

        if (NULL == schSCManager)
        {
            return;
        }

        SC_HANDLE schService = OpenServiceW(schSCManager,(LPCWSTR)L"AntimalwareService_Sirotkina",SERVICE_START |	SERVICE_QUERY_STATUS |	SERVICE_ENUMERATE_DEPENDENTS);

        if (schService == NULL)
        {
            CloseServiceHandle(schSCManager);
            return;
        }
        StartService(schService, NULL, NULL);*/
}
