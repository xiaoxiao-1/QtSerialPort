#include "plot.h"
#include "ui_plot.h"
#include "qcustomplot.h"
plot::plot(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::plot)
{
    ui->setupUi(this);
    start(ui->curveplot);
}

plot::~plot()
{
    delete ui;
}
void plot::start(QCustomPlot* customPlot){
    customPlot->addGraph(); //添加图层
    customPlot->graph(0)->setPen(QPen(QColor(40,110,255)));//设置画笔

    QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
    timeTicker->setTimeFormat("%h:%m:%s");
    customPlot->xAxis->setTicker((timeTicker));
    customPlot->yAxis->setRange(0,20);
    connect(customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->xAxis2, SLOT(setRange(QCPRange)));
    connect(customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->yAxis2, SLOT(setRange(QCPRange)));
    connect(&timer,&QTimer::timeout,this,&plot::realtimeDataSlot);
    timer.start(20);
}
void plot::realtimeDataSlot(){
    static QTime timestart=QTime::currentTime();
    double key=timestart.msecsTo(QTime::currentTime())/1000.0;
    static double lastPointKey=0;
    if(key-lastPointKey>0.002){
        ui->curveplot->graph(0)->addData(key,char_to_int(data));
        lastPointKey=key;
    }
    ui->curveplot->xAxis->setRange(key,8,Qt::AlignRight);
    ui->curveplot->replot();

}
void plot::set_data(char _data){
    data=_data;
}
int char_to_int(char _ch){
    return int(_ch-'0');
}
