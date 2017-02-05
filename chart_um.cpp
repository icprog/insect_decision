#include "chart_um.h"

/* for debugging */
#include <iostream>

ChartUM::ChartUM(params_um_t *p, QWidget *parent)
    : QWidget(parent), params(p)
{
    /* make the window sort of large */
    setFixedSize(1000,600);

    /* populate noise arrays using seed */
    // if we wanted a seed from time: unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator(params->seed);
    int length = ceil(params->d/params->h);
    params->cn1 = (double *)malloc(length * sizeof(*(params->cn1)));
    params->cn2 = (double *)malloc(length * sizeof(*(params->cn2)));
    um_set_noise(&generator, 0.0, params->std_dev, length, params->cn1, params->cn2);

    /* get results of approximation */
    results_y1 = (double *)malloc(length * sizeof(*results_y1));
    results_y2 = (double *)malloc(length * sizeof(*results_y2));

    /* note: function call sets initial conditions */
    usher_mcclelland_rk4(params, results_y1, results_y2);
    //usher_mcclelland_eulers(params, results_y1, results_y2);

    /* create series to chart */
    double t;
    QtCharts::QLineSeries *series1 = new QtCharts::QLineSeries();
    QtCharts::QLineSeries *series2 = new QtCharts::QLineSeries();
    QtCharts::QLineSeries *series_diff = new QtCharts::QLineSeries();
    t = 0.;
    for(int i=0;i<length;i++) {
        series_diff->append(t,results_y1[i]-results_y2[i]);
        series1->append(t,results_y1[i]);
        series2->append(t,results_y2[i]);
        t += params->h;
    }

    /* set series options */
    series1->setName("y1 activation");
    series2->setName("y2 activation");
    series_diff->setName("y1-y2");

    QPen diff_pen(QRgb(0xff9933));
    diff_pen.setWidth(2);
    diff_pen.setStyle(Qt::DashLine);
    series_diff->setPen(diff_pen);

    /* create chart */
    QtCharts::QChart *chart = new QtCharts::QChart();
    chart->legend()->setVisible(true);
    //chart->setTitle("Usher-McClelland Model");

    /* this will animate the series as it is displayed */
    chart->setAnimationOptions(QtCharts::QChart::SeriesAnimations);
    chart->setAnimationDuration(3000);  // duration in milliseconds

    /* create axes */
    /* time axis */
    QtCharts::QValueAxis *axisX = new QtCharts::QValueAxis;
    axisX->setTitleText("Time");
    axisX->setGridLineVisible(false);

    /* activation axis */
    QtCharts::QValueAxis *axisY = new QtCharts::QValueAxis;
    axisY->setRange(-1.0,1.0);
    axisY->setTitleText("Activation");
    axisY->setGridLineVisible(false);

    /* region axis */
    QtCharts::QCategoryAxis *axisY2 = new QtCharts::QCategoryAxis;
    axisY2->append("y2", 2);
    axisY2->append("y1", 4);
    axisY2->setRange(0,4);
    axisY2->setLinePenColor(series_diff->pen().color());
    //axisY3->setGridLinePen((series->pen()));

    /* add axes to chart */
    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);
    chart->addAxis(axisY2, Qt::AlignRight);

    /* add data */
    chart->addSeries(series1);
    chart->addSeries(series2);
    chart->addSeries(series_diff);

    /* attach axes */
    series1->attachAxis(axisX);
    series1->attachAxis(axisY);
    series2->attachAxis(axisY);
    series2->attachAxis(axisX);
//    series_diff->attachAxis(axisY2);
    series_diff->attachAxis(axisY);
    series_diff->attachAxis(axisX);

    /* chart view goes on top */
    chart_view = new QtCharts::QChartView(chart);
    chart_view->setRenderHint(QPainter::Antialiasing);

    /* button box comes next */
    button_box = new QGroupBox;
    QHBoxLayout *button_layout = new QHBoxLayout;
    m_button_close = new QPushButton("close", this);
    m_button_print = new QPushButton("print", this);
    button_layout->addWidget(m_button_print);
    button_layout->addWidget(m_button_close);
    button_box->setLayout(button_layout);

    /* assemble window: main layout */
    QVBoxLayout *main_layout = new QVBoxLayout;
    main_layout->addWidget(chart_view);
    main_layout->addWidget(button_box);

    setLayout(main_layout);
    setWindowTitle(tr("Usher-McClelland Model"));
    show();

    /* wire the signals */
    connect(m_button_print, SIGNAL (clicked()), this, SLOT(slot_print()));
    connect(m_button_close, SIGNAL (clicked()), this, SLOT(slot_close()));
}

void ChartUM::slot_close() {
    free(params->cn1);
    free(params->cn2);
    free(results_y1);
    free(results_y2);
    this->close();
}

void ChartUM::slot_print() {
    /* create a printer */
    QPrinter *printer = new QPrinter(QPrinter::HighResolution);
    //printer->setOrientation(QPrinter::Landscape);
    QPrintDialog *dialog = new QPrintDialog(printer);
    dialog->setWindowTitle("Print Chart");
    if (dialog->exec() != QDialog::Accepted)
        return;

    /* make a painter to paint onto the pages of the printer */
    QPainter *painter = new QPainter();
    painter->begin(printer);
    painter->setRenderHint(QPainter::Antialiasing);
    chart_view->render(painter, printer->pageRect());
    painter->end();
    delete(dialog);
    delete(painter);
    delete(printer);
}
