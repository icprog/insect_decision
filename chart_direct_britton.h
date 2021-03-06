#ifndef CHART_DIRECT_BRITTON_H
#define CHART_DIRECT_BRITTON_H

#include <QApplication>
#include <QChart>
#include <QtCharts/QCategoryAxis>
#include <QChartView>
#include <QFlags>
#include <QGroupBox>
#include <QLineSeries>
#include <QPrinter>
#include <QPrintDialog>
#include <QPushButton>
#include <QValueAxis>
#include <QVBoxLayout>
#include <QWidget>
#include "models.h"

class ChartDirectBritton : public QWidget
{
    Q_OBJECT
public:
    explicit ChartDirectBritton(params_direct_britton_t *p, QWidget *parent = 0);

private:
    /* pointer to parameters */
    params_direct_britton_t *params;

    /* simulation results */
    double * results_y1;
    double * results_y2;

    /* window component: the chart */
    QtCharts::QChartView *chart_view;

    /* window component: button box */
    QGroupBox *button_box;
    QPushButton *m_button_print;
    QPushButton *m_button_close;

signals:

public slots:

private slots:
    void slot_close();
    void slot_print();
};

#endif // CHART_DIRECT_BRITTON_H
