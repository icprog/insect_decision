#include "chart_um.h"
#include "chart_pratt.h"
#include "mainwindow.h"
#include "models.h"

/* for debugging */
#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    /* set default paramters */
    um_set_defaults(&params_um);
    pratt_set_defaults(&params_pratt);

    /* set default window size */
    setFixedWidth(500);

    /* create the boxes for the main window */
    create_menu_bar();
    create_combo_menu_box();
    create_action_box();
    create_model_box();

    /* main layout */
    QVBoxLayout * main_layout = new QVBoxLayout;

    /* add all of the widgets */
    main_layout->setMenuBar(menuBar);
    main_layout->addWidget(combo_menu_box);
    main_layout->addWidget(model_box);
    main_layout->addWidget(action_box);

    /* stick the layout in this widget (i.e., this window) and make it appear */
    setLayout(main_layout);
    setWindowTitle(tr("Insert something catchy"));
    show();
}

/* create the menu bar.
 * Note: on mac osx creating an exit action for a file menu apparently defaults to the
 * mac way of doing it (no File menu is created)
 */
void MainWindow::create_menu_bar() {
    menuBar = new QMenuBar;
    fileMenu = new QMenu(tr("&File"), this);
    exitAction = fileMenu->addAction(tr("E&xit"));

    menuBar->addMenu(fileMenu);
    connect(exitAction, SIGNAL (triggered(bool)), QApplication::instance(), SLOT (quit()));
}

void MainWindow::create_combo_menu_box()
{
    combo_menu_box = new QGroupBox(tr("Model:"));
    QVBoxLayout *layout = new QVBoxLayout;

    action_um = new QAction(this);
    action_um->setText("Usher-McClelland model");
    action_pratt = new QAction(this);
    action_pratt->setText("Simplified Pratt model");

    combo_menu = new QComboBox(this);
    combo_menu->addItem(action_um->text(), QVariant::fromValue(action_um));
    combo_menu->addItem(action_pratt->text(), QVariant::fromValue(action_pratt));

    layout->addWidget(combo_menu);
    combo_menu_box->setLayout(layout);

    /* wire the signals */
    connect(combo_menu, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_model_changed(int)));
}


void MainWindow::create_action_box()
{
    action_box = new QGroupBox;
    QHBoxLayout *layout = new QHBoxLayout;

    /* define the buttons */
    m_button_go = new QPushButton("go", this);
    m_button_quit = new QPushButton("quit",this);

    layout->addWidget(m_button_go);
    layout->addWidget(m_button_quit);

    action_box->setLayout(layout);

    /* wire the signals */
    connect(m_button_quit, SIGNAL (clicked()), QApplication::instance(), SLOT (quit()));
    connect(m_button_go, SIGNAL(clicked(bool)), this, SLOT(slot_go_um()));
}

void MainWindow::create_model_box()
{
    // proposal:
    // this will be a qstackedwidget instance
    // each model has its own widget, and each model accesses its own prameters
    // parameters are stored in main window structs
    // problem: how to change functionality of the go button?

    model_box = new QStackedWidget;

    create_model_box_um();
    create_model_box_pratt();

    model_box->addWidget(model_box_um);
    model_box->addWidget(model_box_pratt);
}

void MainWindow::create_model_box_um()
{
    model_box_um = new QGroupBox(tr("U-M model parameters:"));
    QGridLayout *layout = new QGridLayout;

    QLabel *label_h = new QLabel("Step size:");
    QLabel *label_d = new QLabel("Duration:");
    QLabel *label_y1_0 = new QLabel("y1 initial condition:");
    QLabel *label_y2_0 = new QLabel("y2 initial condition:");
    QLabel *label_I1 = new QLabel("y1 input strength:");
    QLabel *label_I2 = new QLabel("y2 input strength:");
    QLabel *label_w = new QLabel("inhibatory weight:");
    QLabel *label_l = new QLabel("Leakage:");
    QLabel *label_seed = new QLabel("Noise seed:");
    QLabel *label_std_dev = new QLabel("Noise std deviation:");

    /* spinboxes */
    QDoubleSpinBox * sb_h = new QDoubleSpinBox();
    sb_h->setSingleStep(0.01);
    sb_h->setValue(params_um.h);

    QSpinBox * sb_d = new QSpinBox();
    sb_d->setSingleStep(1);
    sb_d->setValue(params_um.d);

    QDoubleSpinBox * sb_y1_0 = new QDoubleSpinBox();
    sb_y1_0->setSingleStep(0.01);
    sb_y1_0->setValue(params_um.y1_0);

    QDoubleSpinBox * sb_y2_0 = new QDoubleSpinBox();
    sb_y2_0->setSingleStep(0.01);
    sb_y2_0->setValue(params_um.y2_0);

    QDoubleSpinBox * sb_I1 = new QDoubleSpinBox();
    sb_I1->setSingleStep(0.01);
    sb_I1->setValue(params_um.I1);

    QDoubleSpinBox * sb_I2 = new QDoubleSpinBox();
    sb_I2->setSingleStep(0.01);
    sb_I2->setValue(params_um.I2);

    QDoubleSpinBox * sb_w = new QDoubleSpinBox();
    sb_w->setSingleStep(0.01);
    sb_w->setValue(params_um.w1);

    QDoubleSpinBox * sb_l = new QDoubleSpinBox();
    sb_l->setSingleStep(0.01);
    sb_l->setValue(params_um.l1);

    QSpinBox * sb_seed = new QSpinBox();
    sb_seed->setSingleStep(1);
    sb_seed->setRange(0,10000);
    sb_seed->setValue(params_um.seed);

    QDoubleSpinBox * sb_std_dev = new QDoubleSpinBox();
    sb_std_dev->setSingleStep(0.01);
    sb_std_dev->setRange(0,100.00);
    sb_std_dev->setValue(params_um.std_dev);

    /* put the labels and spinboxes in the rows, cols */
    layout->addWidget(label_h, 0, 0);
    layout->addWidget(sb_h, 0, 1);
    layout->addWidget(label_d, 1, 0);
    layout->addWidget(sb_d, 1, 1);
    layout->addWidget(label_I1, 2, 0);
    layout->addWidget(sb_I1, 2, 1);
    layout->addWidget(label_I2, 3, 0);
    layout->addWidget(sb_I2, 3, 1);
    layout->addWidget(label_w, 4, 0);
    layout->addWidget(sb_w, 4, 1);
    layout->addWidget(label_seed, 0, 2);
    layout->addWidget(sb_seed, 0, 3);
    layout->addWidget(label_std_dev, 1, 2);
    layout->addWidget(sb_std_dev, 1, 3);
    layout->addWidget(label_y1_0, 2, 2);
    layout->addWidget(sb_y1_0, 2, 3);
    layout->addWidget(label_y2_0, 3, 2);
    layout->addWidget(sb_y2_0, 3, 3);
    layout->addWidget(label_l, 4, 2);
    layout->addWidget(sb_l, 4, 3);

    layout->setColumnStretch(1, 10);
    layout->setColumnStretch(3, 20);
    model_box_um->setLayout(layout);

    connect(sb_h, SIGNAL(valueChanged(double)), this, SLOT(slot_um_h_changed(double)));
    connect(sb_d, SIGNAL(valueChanged(int)), this, SLOT(slot_um_d_changed(int)));
    connect(sb_y1_0, SIGNAL(valueChanged(double)), this, SLOT(slot_um_y1_0_changed(double)));
    connect(sb_y2_0, SIGNAL(valueChanged(double)), this, SLOT(slot_um_y2_0_changed(double)));
    connect(sb_I1, SIGNAL(valueChanged(double)), this, SLOT(slot_um_I1_changed(double)));
    connect(sb_I2, SIGNAL(valueChanged(double)), this, SLOT(slot_um_I2_changed(double)));
    connect(sb_w, SIGNAL(valueChanged(double)), this, SLOT(slot_um_w_changed(double)));
    connect(sb_l, SIGNAL(valueChanged(double)), this, SLOT(slot_um_l_changed(double)));
    connect(sb_seed, SIGNAL(valueChanged(int)), this, SLOT(slot_um_seed_changed(int)));
    connect(sb_std_dev, SIGNAL(valueChanged(double)), this, SLOT(slot_um_std_dev_changed(double)));

}

/**************************************************************
 * Simplified Pratt Model GUI Box
 **************************************************************/
void MainWindow::create_model_box_pratt()
{
    model_box_pratt = new QGroupBox(tr("Pratt model parameters:"));
    QGridLayout *layout = new QGridLayout;

    QLabel *label_h = new QLabel("Step size:");
    QLabel *label_d = new QLabel("Duration:");
    QLabel *label_population = new QLabel("Population:");
    QLabel *label_y1_0 = new QLabel("y1 initial condition:");
    QLabel *label_y2_0 = new QLabel("y2 initial condition:");
    QLabel *label_q1 = new QLabel("Nest 1 quality:");
    QLabel *label_q2 = new QLabel("Nest 2 quality:");
    QLabel *label_r1 = new QLabel("Direct trans rate:");
    QLabel *label_l1 = new QLabel("Leak rate:");
    QLabel *label_r1_prime = new QLabel("Recruitment rate:");
    QLabel *label_seed = new QLabel("Noise seed:");
    QLabel *label_std_dev = new QLabel("Noise std deviation:");

    /* spinboxes */
    QDoubleSpinBox * sb_h = new QDoubleSpinBox();
    sb_h->setSingleStep(0.01);
    sb_h->setDecimals(3);
    sb_h->setValue(params_pratt.h);

    QSpinBox * sb_d = new QSpinBox();
    sb_d->setSingleStep(1);
    sb_d->setRange(0,500);
    sb_d->setValue(params_pratt.d);

    QDoubleSpinBox * sb_population = new QDoubleSpinBox();
    sb_population->setSingleStep(1.0);
    sb_population->setDecimals(1);
    sb_population->setRange(1.0,1000000.0);
    sb_population->setValue(params_pratt.population);

    QDoubleSpinBox * sb_y1_0 = new QDoubleSpinBox();
    sb_y1_0->setSingleStep(0.01);
    sb_y1_0->setValue(params_pratt.y1_0);

    QDoubleSpinBox * sb_y2_0 = new QDoubleSpinBox();
    sb_y2_0->setSingleStep(0.01);
    sb_y2_0->setValue(params_pratt.y2_0);

    QDoubleSpinBox * sb_q1 = new QDoubleSpinBox();
    sb_q1->setSingleStep(0.01);
    sb_q1->setDecimals(3);
    sb_q1->setValue(params_pratt.q1);

    QDoubleSpinBox * sb_q2 = new QDoubleSpinBox();
    sb_q2->setSingleStep(0.01);
    sb_q2->setDecimals(3);
    sb_q2->setValue(params_pratt.q2);

    QDoubleSpinBox * sb_r1 = new QDoubleSpinBox();
    sb_r1->setSingleStep(0.01);
    sb_r1->setDecimals(3);
    sb_r1->setValue(params_pratt.r1);

    QDoubleSpinBox * sb_l1 = new QDoubleSpinBox();
    sb_l1->setSingleStep(0.01);
    sb_l1->setDecimals(3);
    sb_l1->setValue(params_pratt.l1);

    QDoubleSpinBox * sb_r1_prime = new QDoubleSpinBox();
    sb_r1_prime->setSingleStep(0.01);
    sb_r1_prime->setDecimals(3);
    sb_r1_prime->setValue(params_pratt.r1_prime);

    QSpinBox * sb_seed = new QSpinBox();
    sb_seed->setSingleStep(1);
    sb_seed->setRange(0,10000);
    sb_seed->setValue(params_pratt.seed);

    QDoubleSpinBox * sb_std_dev = new QDoubleSpinBox();
    sb_std_dev->setSingleStep(0.01);
    sb_std_dev->setDecimals(3);
    sb_std_dev->setRange(0,100.00);
    sb_std_dev->setValue(params_pratt.std_dev);

    /* put the labels and spinboxes in the rows, cols */
    layout->addWidget(label_h, 0, 0);
    layout->addWidget(sb_h, 0, 1);
    layout->addWidget(label_d, 1, 0);
    layout->addWidget(sb_d, 1, 1);
    layout->addWidget(label_population, 2, 0);
    layout->addWidget(sb_population, 2, 1);
    layout->addWidget(label_q1, 3, 0);
    layout->addWidget(sb_q1, 3, 1);
    layout->addWidget(label_q2, 4, 0);
    layout->addWidget(sb_q2, 4, 1);
    layout->addWidget(label_r1, 5, 0);
    layout->addWidget(sb_r1, 5, 1);
    layout->addWidget(label_r1_prime, 0, 2);
    layout->addWidget(sb_r1_prime, 0, 3);
    layout->addWidget(label_seed, 1, 2);
    layout->addWidget(sb_seed, 1, 3);
    layout->addWidget(label_std_dev, 2, 2);
    layout->addWidget(sb_std_dev, 2, 3);
    layout->addWidget(label_y1_0, 3, 2);
    layout->addWidget(sb_y1_0, 3, 3);
    layout->addWidget(label_y2_0, 4, 2);
    layout->addWidget(sb_y2_0, 4, 3);
    layout->addWidget(label_l1, 5, 2);
    layout->addWidget(sb_l1, 5, 3);

    layout->setColumnStretch(1, 10);
    layout->setColumnStretch(3, 20);
    model_box_pratt->setLayout(layout);

    connect(sb_h, SIGNAL(valueChanged(double)), this, SLOT(slot_sp_h_changed(double)));
    connect(sb_d, SIGNAL(valueChanged(int)), this, SLOT(slot_sp_d_changed(int)));
    connect(sb_population, SIGNAL(valueChanged(double)), this, SLOT(slot_sp_population_changed(double)));
    connect(sb_y1_0, SIGNAL(valueChanged(double)), this, SLOT(slot_sp_y1_0_changed(double)));
    connect(sb_y2_0, SIGNAL(valueChanged(double)), this, SLOT(slot_sp_y2_0_changed(double)));
    connect(sb_q1, SIGNAL(valueChanged(double)), this, SLOT(slot_sp_q1_changed(double)));
    connect(sb_q2, SIGNAL(valueChanged(double)), this, SLOT(slot_sp_q2_changed(double)));
    connect(sb_r1, SIGNAL(valueChanged(double)), this, SLOT(slot_sp_r1_changed(double)));
    connect(sb_l1, SIGNAL(valueChanged(double)), this, SLOT(slot_sp_l1_changed(double)));
    connect(sb_seed, SIGNAL(valueChanged(int)), this, SLOT(slot_sp_seed_changed(int)));
    connect(sb_std_dev, SIGNAL(valueChanged(double)), this, SLOT(slot_sp_std_dev_changed(double)));
}

/**************************************************************
 * Slots for Usher-McClelland Model
 **************************************************************/

void MainWindow::slot_um_h_changed(double v) {
    params_um.h = v;
}

void MainWindow::slot_um_d_changed(int v) {
    params_um.d = v;
}

void MainWindow::slot_um_y1_0_changed(double v) {
    params_um.y1_0 = v;
}

void MainWindow::slot_um_y2_0_changed(double v) {
    params_um.y2_0 = v;
}

void MainWindow::slot_um_I1_changed(double v) {
    params_um.I1 = v;
}

void MainWindow::slot_um_I2_changed(double v) {
    params_um.I2 = v;
}

void MainWindow::slot_um_w_changed(double v) {
    params_um.w1 = v;
    params_um.w2 = v;
}

void MainWindow::slot_um_l_changed(double v) {
    params_um.l1 = v;
    params_um.l2 = v;
}

void MainWindow::slot_um_seed_changed(int v) {
    params_um.seed = v;
}

void MainWindow::slot_um_std_dev_changed(double v) {
    params_um.std_dev = v;
}

/**************************************************************
 * END OF Slots for Usher-McClelland Model
 **************************************************************/

/**************************************************************
 * Slots for Simplified Pratt Model
 **************************************************************/

void MainWindow::slot_sp_h_changed(double v) {
    params_pratt.h = v;
}

void MainWindow::slot_sp_population_changed(double v) {
    params_pratt.population = v;
}

void MainWindow::slot_sp_d_changed(int v) {
    params_pratt.d = v;
}

void MainWindow::slot_sp_y1_0_changed(double v) {
    params_pratt.y1_0 = v;
}

void MainWindow::slot_sp_y2_0_changed(double v) {
    params_pratt.y2_0 = v;
}

void MainWindow::slot_sp_q1_changed(double v) {
    params_pratt.q1 = v;
}

void MainWindow::slot_sp_q2_changed(double v) {
    params_pratt.q2 = v;
}

void MainWindow::slot_sp_r1_changed(double v) {
    params_pratt.r1 = v;
    params_pratt.r2 = v;
}

void MainWindow::slot_sp_l1_changed(double v) {
    params_pratt.l1 = v;
    params_pratt.l2 = v;
}

void MainWindow::slot_sp_r1_prime_changed(double v) {
    params_pratt.r1_prime = v;
    params_pratt.r2_prime = v;
}

void MainWindow::slot_sp_seed_changed(int v) {
    params_pratt.seed = v;
}

void MainWindow::slot_sp_std_dev_changed(double v) {
    params_pratt.std_dev = v;
}

/**************************************************************
 * END OF Slots for Simplified Pratt Model
 **************************************************************/

/**************************************************************
 * Other Slots
 **************************************************************/

void MainWindow::slot_go_um() {
    ChartUM *chart = new ChartUM(&params_um);
    chart->show();
}

void MainWindow::slot_go_pratt() {
    ChartPratt *chart = new ChartPratt(&params_pratt);
    chart->show();
}

void MainWindow::slot_model_changed(int index)
{
    switch (index) {
        case MODEL_UM:
            model_box->setCurrentWidget(model_box_um);
            disconnect(m_button_go, SIGNAL(clicked(bool)), 0, 0);
            connect(m_button_go, SIGNAL(clicked(bool)), this, SLOT(slot_go_um()));
        break;
        case MODEL_PRATT:
            model_box->setCurrentWidget(model_box_pratt);
            disconnect(m_button_go, SIGNAL(clicked(bool)), 0, 0);
            connect(m_button_go, SIGNAL(clicked(bool)), this, SLOT(slot_go_pratt()));
            break;
        default:
            std::cout << "Unknown model" << std::endl;
    }
}
