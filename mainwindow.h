#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QApplication>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QMenuBar>
#include <QPushButton>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QWidget>
#include "models.h"

class MainWindow : public QWidget
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);

private:
    void create_menu_bar();         /* dropdown menu */
    void create_combo_menu_box();   /* select which model to use */
    void create_action_box();       /* for graph and exit buttons */
    void create_model_box();
    void create_model_box_um();
    void create_model_box_pratt();

    /* window component: menu bar */
    QMenuBar *menuBar;  /* note: apparently on mac/osx the quit action always defaults to the name of the program */
    QMenu *fileMenu;
    QAction *exitAction;

    /* window component: combo menu box for selecting model */
    enum {MODEL_UM = 0, MODEL_PRATT};
    QGroupBox *combo_menu_box;
    QComboBox *combo_menu;
    QAction *action_um;
    QAction *action_pratt;

    /* window component: button box for actions such as graph or quit */
    QGroupBox *action_box;

    /* window component: stacked widget to hold the boxes for each model */
    QStackedWidget *model_box;
    /* the boxes */
    QGroupBox *model_box_um;    /* the um model box */
    QGroupBox *model_box_pratt; /* the simplified pratt model box */

    /* model parameters */
    params_um_t params_um;          /* usher-mclelland binary decision */
    params_pratt_t params_pratt;    /* simplified pratt model: social insect, direct switching */

    /* buttons */
    /* Q: should these be defined here or in the action box?
     * A: I think they should stay here so they are easily accessible by other methods */
    QPushButton * m_button_go;
    QPushButton * m_button_quit;

signals:

public slots:

private slots:
    /* um model changes to um spinboxes */
    void slot_um_h_changed(double v);
    void slot_um_d_changed(int v);
    void slot_um_y1_0_changed(double v);
    void slot_um_y2_0_changed(double v);
    void slot_um_I1_changed(double v);
    void slot_um_I2_changed(double v);
    void slot_um_w_changed(double v);
    void slot_um_l_changed(double v);
    void slot_um_seed_changed(int v);
    void slot_um_std_dev_changed(double v);

    /* simplified pratt model changes to spinboxes */
    void slot_sp_h_changed(double v);
    void slot_sp_d_changed(int v);
    void slot_sp_population_changed(double v);
    void slot_sp_y1_0_changed(double v);
    void slot_sp_y2_0_changed(double v);
    void slot_sp_q1_changed(double v);
    void slot_sp_q2_changed(double v);
    void slot_sp_r1_changed(double v);
    void slot_sp_l1_changed(double v);
    void slot_sp_r1_prime_changed(double v);
    void slot_sp_seed_changed(int v);
    void slot_sp_std_dev_changed(double v);

    /* slots for handling the charting of the different models */
    void slot_go_um();
    void slot_go_pratt();

    void slot_model_changed(int);
};

#endif // MAINWINDOW_H
