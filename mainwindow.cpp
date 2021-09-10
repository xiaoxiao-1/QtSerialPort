# include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ui_plot.h"
#include <QDebug>
#include <QMessageBox>
#include <QString>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , serial()
    , curve_plot(nullptr)
{

    ui->setupUi(this);
    QObject::connect(ui->pushButton,&QPushButton::clicked,this,&MainWindow::serial_scan);
    QObject::connect(ui->pushButton_2,&QPushButton::clicked,this,&MainWindow::open_serial);
    timer=new QTimer(this); //用于定时接收串口数据
    QObject::connect(this->timer,&QTimer::timeout,this,&MainWindow::serial_read);
    timer->start(10);

    QObject::connect(ui->action,&QAction::triggered,this,&MainWindow::show_plot_widget);

}
MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::serial_scan(){
    //搜索串口
    ui->comboBox->clear();
    foreach (const QSerialPortInfo& info, QSerialPortInfo::availablePorts()) {
        ui->comboBox->addItem(info.portName());
    }
}
void MainWindow::open_serial(){
    //这个类用来打开串口
    if(ui->pushButton_2->text()=="打开串口"){
        //现在应该从combobox中获取选择的串口数值 ， 然后打开串口
        serial.setPortName(QString("/dev/")+ui->comboBox->currentText());
        serial.setBaudRate(ui->comboBox_5->currentText().toInt());

        //设置停止位
        switch(ui->comboBox_6->currentText().toInt()){
            case 1:serial.setStopBits(QSerialPort::OneStop);
                   break;
        case 2:serial.setStopBits(QSerialPort::TwoStop);
            break;
        default:
            qDebug()<<"停止位设置错误!";
            break;
        }
        //设置数据位
        switch(ui->comboBox_3->currentText().toInt()){
            case 5:
                serial.setDataBits(QSerialPort::Data5);
                break;
            case 6:
                serial.setDataBits(QSerialPort::Data6);
                break;
            case 7:
                serial.setDataBits(QSerialPort::Data7);
                break;
            case 8:
                serial.setDataBits(QSerialPort::Data8);
                break;
            default:
                qDebug()<<"数据位设置错误！";
                break;
        }
        //设置校验方式
        if(ui->comboBox_7->currentText()=="奇校验"){
            serial.setParity(QSerialPort::OddParity);
        }else if(ui->comboBox_7->currentText()=="偶校验"){
            serial.setParity(QSerialPort::EvenParity);
        }else{
            serial.setParity(QSerialPort::NoParity);
        }
        //设置流控制模式
        serial.setFlowControl(QSerialPort::NoFlowControl);
        if(!serial.open(QIODevice::ReadWrite)){
            QMessageBox::warning(nullptr,"错误","串口打开失败，请查看debug记录");
            return ;
        }
        ui->comboBox->setEnabled(false);
        ui->comboBox_3->setEnabled(false);
        ui->comboBox_5->setEnabled(false);
        ui->comboBox_6->setEnabled(false);
        ui->comboBox_7->setEnabled(false);
        ui->pushButton->setEnabled(false);
        ui->pushButton_2->setText("关闭串口");
    }else if(ui->pushButton_2->text()=="关闭串口"){
        serial.close();
        if(serial.isOpen()){
            QMessageBox::warning(nullptr,"错误","串口关闭失败，请查看debug记录");
            return ;
        }
        ui->comboBox->setEnabled(true);
        ui->comboBox_3->setEnabled(true);
        ui->comboBox_5->setEnabled(true);
        ui->comboBox_6->setEnabled(true);
        ui->comboBox_7->setEnabled(true);
        ui->pushButton->setEnabled(true);
        ui->pushButton_2->setText("打开串口");

    }else{
        qDebug()<<"打开关闭串口的第三种状态";
    }
}
void MainWindow::serial_read(){
    //读取数据并且显示到读取数据的组件中
    char ch=' ';
    if(serial.isOpen()){
        //如果serial是打开的状态，
        serial.read(&ch,1);
        if(ch!=' '){
            ui->textEdit->append(QString(ch));
            if(curve_plot!=nullptr){
                qDebug()<<"波形图创建成功，可以开始显示";
                curve_plot->set_data(ch);
            }

        }

    }else{
        qDebug()<<"串口没有打开";
    }
}
void MainWindow::show_plot_widget(){
    this->curve_plot=new plot();
    curve_plot->setAttribute(Qt::WA_DeleteOnClose);
    curve_plot->show();
}
