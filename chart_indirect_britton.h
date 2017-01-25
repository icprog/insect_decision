#ifndef CHART_INDIRECT_BRITTON_H
#define CHART_INDIRECT_BRITTON_H

#include <QApplication>
#include <QChart>
#include <QtCharts/QCategoryAxis>
#include <QChartView>
#include <QFlags>
#include <QGroupBox>
#include <QLineSeries>
#include <QPushButton>
#include <QValueAxis>
#include <QVBoxLayout>
#include <QWidget>
#include "models.h"

class ChartIndirectBritton : public QWidget
{
    Q_OBJECT
public:
    explicit ChartIndirectBritton(params_indirect_britton_t *p, QWidget *parent = 0);

private:
    /* pointer to parameters */
    params_indirect_britton_t *params;

    /* simulation results */
    double * results_y1;
    double * results_y2;

    /* window component: button box */
    QGroupBox *button_box;
    QPushButton *m_button_close;

signals:

public slots:

private slots:
    void slot_close();
};

#endif // CHART_INDIRECT_BRITTON_H
