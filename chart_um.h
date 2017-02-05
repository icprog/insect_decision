#ifndef CHART_UM_H
#define CHART_UM_H

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

class ChartUM : public QWidget
{
    Q_OBJECT
public:
    explicit ChartUM(params_um_t *p, QWidget *parent = 0);

private:
    /* pointer to parameters */
    params_um_t *params;

    /* simulation results */
    double *results_y1;
    double *results_y2;

    /* window component: the chart */
    QtCharts::QChartView *chart_view;

    /* window component: button_box */
    QGroupBox *button_box;
    QPushButton *m_button_close;
    QPushButton *m_button_print;

signals:

public slots:

private slots:
    void slot_close();
    void slot_print();
};

#endif // CHART_UM_H
