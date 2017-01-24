#include "chart_pratt.h"

/* for debugging */
#include <iostream>

ChartPratt::ChartPratt(params_pratt_t *p, QWidget *parent)
    : QWidget(parent), params(p)
{
    /* make a close button */
    m_button_close = new QPushButton("close", this);

    /* make the window sort of large */
    setFixedSize(1000,600);

    /* populate noise arrays using seed */
    std::default_random_engine generator(params->seed);
    int length = (int)(params->d/params->h);

    params->cn_q1 = (double *)malloc(length * sizeof(*(params->cn_q1)));
    params->cn_q2 = (double *)malloc(length * sizeof(*(params->cn_q2)));
    params->cn_r1 = (double *)malloc(length * sizeof(*(params->cn_r1)));
    params->cn_r2 = (double *)malloc(length * sizeof(*(params->cn_r2)));
    params->cn_r1_prime = (double *)malloc(length * sizeof(*(params->cn_r1_prime)));
    params->cn_r2_prime = (double *)malloc(length * sizeof(*(params->cn_r2_prime)));
    params->cn_l1 = (double *)malloc(length * sizeof(*(params->cn_l1)));
    params->cn_l2 = (double *)malloc(length * sizeof(*(params->cn_l2)));

    pratt_set_noise(&generator,
                    0.0,
                    params->std_dev,
                    length,
                    params->cn_q1,
                    params->cn_q2,
                    params->cn_r1,
                    params->cn_r2,
                    params->cn_r1_prime,
                    params->cn_r2_prime,
                    params->cn_l1,
                    params->cn_l2);

    /* get results of approximation */
    results_y1 = (double *)malloc(length * sizeof(*results_y1));
    results_y2 = (double *)malloc(length * sizeof(*results_y2));

    // note: function call will set initial conditions
    pratt_rk4(params, results_y1, results_y2);

    /* create series to chart */
    double t;
    QtCharts::QLineSeries *series1 = new QtCharts::QLineSeries();
    QtCharts::QLineSeries *series2 = new QtCharts::QLineSeries();
    //QtCharts::QLineSeries *series_diff = new QtCharts::QLineSeries();
    t = 0.;
    for(int i=0;i<length;i++) {
        //series_diff->append(t,results_y1[i]-results_y2[i]);
        series1->append(t,results_y1[i]);
        series2->append(t,results_y2[i]);
        t += params->h;
    }

    /* set series options */
    series1->setName("Nest A");
    series2->setName("Nest B");
    //series_diff->setName("A-B");

    //QPen diff_pen(QRgb(0xff9933));
    //diff_pen.setWidth(2);
    //diff_pen.setStyle(Qt::DashLine);
    //series_diff->setPen(diff_pen);

    /* create chart */
    QtCharts::QChart *chart = new QtCharts::QChart();
    //chart->legend()->hide();
    chart->legend()->setVisible(true);
    chart->setTitle("Simplified Pratt (Direct Transfer) Model");

    /* this will animate the series as it is displayed */
    chart->setAnimationOptions(QtCharts::QChart::SeriesAnimations);
    chart->setAnimationDuration(3000);  // duration in milliseconds

    /* create axes */
    /* time axis */
    QtCharts::QValueAxis *axisX = new QtCharts::QValueAxis;
    axisX->setTitleText("Time");

    /* activation axis */
    QtCharts::QValueAxis *axisY = new QtCharts::QValueAxis;
    axisY->setRange(-1.0,params->population);
    axisY->setTitleText("Population");

    /* region axis */
    //QtCharts::QCategoryAxis *axisY2 = new QtCharts::QCategoryAxis;
    //axisY2->append("Option B Wins", 2);
    //axisY2->append("Option A Wins", 4);
    //axisY2->setRange(0,4);
    //axisY2->setLinePenColor(series_diff->pen().color());
    //axisY3->setGridLinePen((series->pen()));

    /* add axes to chart */
    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);
    //chart->addAxis(axisY2, Qt::AlignRight);

    /* add data */
    chart->addSeries(series1);
    chart->addSeries(series2);
    //chart->addSeries(series_diff);

    /* attach axes */
    series1->attachAxis(axisX);
    series1->attachAxis(axisY);
    series2->attachAxis(axisY);
    series2->attachAxis(axisX);
//    series_diff->attachAxis(axisY2);
    //series_diff->attachAxis(axisY);
    //series_diff->attachAxis(axisX);

    /* set up vertical layout */
    QVBoxLayout * vlayout;
    vlayout = new QVBoxLayout;

    QtCharts::QChartView *chartView = new QtCharts::QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    vlayout->addWidget(chartView);
    vlayout->addWidget(m_button_close);

    this->setLayout(vlayout);
    this->show();

    /* wire the signals */
    connect(m_button_close, SIGNAL (clicked()), this, SLOT(slot_close()));
}

void ChartPratt::slot_close() {
    free(params->cn_q1);
    free(params->cn_q2);
    free(params->cn_r1);
    free(params->cn_r2);
    free(params->cn_l1);
    free(params->cn_l2);
    free(params->cn_r1_prime);
    free(params->cn_r2_prime);
    free(results_y1);
    free(results_y2);
    this->close();
}

