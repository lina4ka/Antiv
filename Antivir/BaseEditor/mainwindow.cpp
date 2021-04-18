#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QFileDialog"
#include <string>
#include <memory>
#include "QMessageBox"
#include "malwaredata.h"
#include "Basewriter.h"
#include <algorithm>
using namespace std;

unique_ptr<MalwareData> storage;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_addRow_clicked()
{
    ui->tableWidget->setRowCount(ui->tableWidget->rowCount()+1);
}

void MainWindow::on_pathButton_clicked()
{
    QString baseAddress = QFileDialog::getOpenFileName(this,
           tr("Выберите файл"), "",
           tr("БД (*.bin);"));
    string path = baseAddress.toStdString();
    ui->textEdit->setText(baseAddress);
    reprintTable();
}

void MainWindow::reprintTable()
{
    string path = ui->textEdit->toPlainText().toStdString();
    storage.reset(new MalwareData(path,"Sirotkina"));
    if (!storage->isCorrect())
    {
        QMessageBox::information(this,"Info","Wrong Data");
        return;
    }
    storage->readData();
    ui->tableWidget->setRowCount(0);
    ui->tableWidget->setRowCount(storage->getSize());
    vector<RecordInfo> vec = storage->getList();
    string temp(8,'\0');
    for (size_t i = 0; i < vec.size();++i)
    {
        QTableWidgetItem *pCell = ui->tableWidget->item(i, 0);
        if(!pCell)
        {
            pCell = new QTableWidgetItem;
            ui->tableWidget->setItem(i, 0, pCell);
        }
        pCell->setText(vec.at(i).malvareName.c_str());

        pCell = ui->tableWidget->item(i, 1);
        memcpy(temp.data(),&vec.at(i).prefix,sizeof(uint64_t));
        if(!pCell)
        {
            pCell = new QTableWidgetItem;
            ui->tableWidget->setItem(i, 1, pCell);
        }
        pCell->setText(temp.c_str());

        pCell = ui->tableWidget->item(i, 2);
        if(!pCell)
        {
            pCell = new QTableWidgetItem;
            ui->tableWidget->setItem(i, 2, pCell);
        }
        pCell->setText(to_string(vec.at(i).signatureSize).c_str());

        pCell = ui->tableWidget->item(i, 3);
        if(!pCell)
        {
            pCell = new QTableWidgetItem;
            ui->tableWidget->setItem(i, 3, pCell);
        }
        pCell->setText(to_string(vec.at(i).offsetBegin).c_str());

        pCell = ui->tableWidget->item(i, 4);
        if(!pCell)
        {
            pCell = new QTableWidgetItem;
            ui->tableWidget->setItem(i, 4, pCell);
        }
        pCell->setText(to_string(vec.at(i).offsetEnd).c_str());

        pCell = ui->tableWidget->item(i, 5);
        if(!pCell)
        {
            pCell = new QTableWidgetItem;
            ui->tableWidget->setItem(i, 5, pCell);
        }
        pCell->setText(vec.at(i).SHA.c_str());
    }
}


void MainWindow::on_saveButton_clicked()
{
    RecordFabric rf;
    vector<RecordInfo> vec;
    vector<string> existPref;
    for (size_t i = 0; i< ui->tableWidget->rowCount();++i)
    {
        if (storage->isExist(ui->tableWidget->item(i,1)->text().toStdString()).SHA.empty())
            vec.emplace_back(rf.create(ui->tableWidget->item(i,0)->text().toStdString(),
                                       ui->tableWidget->item(i,1)->text().toStdString(),
                                       ui->tableWidget->item(i,3)->text().toULongLong(),
                                       ui->tableWidget->item(i,4)->text().toULongLong()));
        else
            vec.emplace_back(storage->isExist(ui->tableWidget->item(i,1)->text().toStdString()));
    }
    BaseWriter bw("Sirotkina");
    bw.writeIn(ui->textEdit->toPlainText().toStdString(),vec);
    reprintTable();
}

void MainWindow::on_rButton_clicked()
{
    ui->tableWidget->removeRow(ui->tableWidget->currentRow());
}

void MainWindow::on_bCreate_clicked()
{
    BaseWriter bw("Sirotkina");
    string path = bw.createData("");
     ui->textEdit->setText(path.c_str());
     storage.reset(new MalwareData(path,"Sirotkina"));
}
