#include "chart_direct_britton.h"

/* for debugging */
#include <iostream>

ChartDirectBritton::ChartDirectBritton(params_direct_britton_t *p, QWidget *parent)
    : QWidget(parent), params(p)
{
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

    direct_britton_set_noise(&generator,
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
    direct_britton_rk4(params, results_y1, results_y2);

    /* create series to chart */
    double t;
    QtCharts::QLineSeries *series1 = new QtCharts::QLineSeries();
    QtCharts::QLineSeries *series2 = new QtCharts::QLineSeries();
    QtCharts::QLineSeries *source_population = new QtCharts::QLineSeries();
    t = 0.;
    for(int i=0;i<length;i++) {
        series1->append(t,results_y1[i]);
        series2->append(t,results_y2[i]);
        source_population->append(t,params->population-results_y1[i]-results_y2[i]);
        t += params->h;
    }

    /* set series options */
    series1->setName("Nest A");
    series2->setName("Nest B");
    source_population->setName("Source Population");

    /* set color of source population line */
    QPen pop_pen(QRgb(0xff9933));
    pop_pen.setWidth(2);
    pop_pen.setStyle(Qt::DashLine);
    source_population->setPen(pop_pen);

    /* create chart */
    QtCharts::QChart *chart = new QtCharts::QChart();
    //chart->legend()->hide();
    chart->legend()->setVisible(true);
    chart->setTitle("Simplified Direct Britton Model");

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

    /* add axes to chart */
    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);

    /* add data */
    chart->addSeries(series1);
    chart->addSeries(series2);
    chart->addSeries(source_population);

    /* attach axes */
    series1->attachAxis(axisX);
    series1->attachAxis(axisY);
    series2->attachAxis(axisY);
    series2->attachAxis(axisX);
    source_population->attachAxis(axisX);
    source_population->attachAxis(axisY);

    /* chart view goes on top */
    QtCharts::QChartView *chartView = new QtCharts::QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    /* button box comes next */
    button_box = new QGroupBox;
    QHBoxLayout *button_layout = new QHBoxLayout;
    m_button_close = new QPushButton("close", this);
    button_layout->addWidget(m_button_close);
    button_box->setLayout(button_layout);

    /* assemble window: main layout */
    QVBoxLayout *main_layout = new QVBoxLayout;
    main_layout->addWidget(chartView);
    main_layout->addWidget(button_box);

    setLayout(main_layout);
    setWindowTitle(tr("Simplified Direct Britton Model"));
    show();

    /* wire the signals */
    connect(m_button_close, SIGNAL (clicked()), this, SLOT(slot_close()));
}

void ChartDirectBritton::slot_close() {
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

