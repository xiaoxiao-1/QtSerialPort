#ifndef PLOT_H
#define PLOT_H

#include <QWidget>
#include <qcustomplot.h>
#include <QTimer>
namespace Ui {
class plot;
}

class plot : public QWidget
{
    Q_OBJECT

public:
    explicit plot(QWidget *parent = nullptr);
    ~plot();
    void start(QCustomPlot* customPlot);
    void realtimeDataSlot();
    void set_data(char _data);
private:
    Ui::plot *ui;
    char data;
    QTimer timer;
};
static int char_to_int(char _ch);
#endif // PLOT_H
