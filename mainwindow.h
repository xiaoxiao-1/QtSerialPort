#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPortInfo>
#include <QSerialPort>
#include <QTimer>
#include "plot.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow;
               class plot;}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    plot* curve_plot;
    QSerialPort serial;
    QTimer* timer;
private slots:
    void serial_scan();
    void open_serial();
    void serial_read();
    void show_plot_widget();
};
#endif // MAINWINDOW_H
