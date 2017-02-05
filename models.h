#ifndef MODELS_H
#define MODELS_H

#include <iostream> // debug
#include <random>

/* parameters for usher_mclelland model */
typedef struct params_um_s {
    double h; /* step size */
    int d;    /* duration */
    double y1_0;  /* y1 initial condition */
    double y2_0;  /* y2 initial condition */
    double I1;    /* input signal to y1 */
    double I2;    /* input signal to y2 */
    double w1;    /* weight from y1 to y2 */
    double w2;    /* weight from y2 to y1 */
    double l1;    /* leak (decay) term for y1 */
    double l2;    /* leak (decay) term for y2 */
    double std_dev;  /* standard deviation for noise */
    double * cn1; /* array of noise values with mean 0.0 and standard deviation c */
    double * cn2; /* ditto */
    int seed;
} params_um_t;

/* parameters for simplified Pratt model (direct switching) */
typedef struct params_pratt_s {
    double h;       /* step size */
    int d;          /* duration */
    double population; /* total population of ants */
    double y1_0;    /* y1 initial condition */
    double y2_0;    /* y2 initial condition */
    double q1;      /* quality of nest 1 (analogous to I1 from UM model)*/
    double q2;      /* quality of nest 2 (analogous to I2 from UM model)*/
    double r1;      /* direct recruitment from nest 1 to 2 (analogous to inhibitory weight from UM model)*/
    double r2;      /* direct recruitment from nest 2 to 1 (analogous to inhibitory weight from UM model)*/
    double r1_prime; /* rate of recruitment from nest 2 to nest 1 by already-converted ants */
    double r2_prime; /* rate of recruitment from nest 1 to nest 2 by already-converted ants */
    double l1;      /* leak (decay) term for nest 1 lost to source*/
    double l2;      /* leak (decay) term for nest 2 lost to source*/

    double std_dev; /* standard deviation for noise */
    int seed;       /* noise seed */

    /* noise arrays */
    double *cn_q1;  /* noise applied to quality of nest 1 */
    double *cn_q2;  /* noise applied to quality of nest 2 */
    double *cn_r1;  /* noise applied to recruitment leakage from nest 1 to nest 2 */
    double *cn_r2;  /* noise applied to recruitment leakage from nest 2 to nest 1 */
    double *cn_r1_prime; /* noise applied to recruitment leakage from S to nest 1 */
    double *cn_r2_prime; /* noise applied to recruitment leakage from S to nest 2 */
    double *cn_l1;  /* noise applied to leakage from nest 1 to source */
    double *cn_l2;  /* noise applied to leakage from nest 2 to source */

} params_pratt_t;

/* parameters for simplified indirect Britton model */
typedef struct params_indirect_britton_s {
    double h;       /* step size */
    int d;          /* duration */
    double population; /* total population of ants */
    double y1_0;    /* y1 initial condition */
    double y2_0;    /* y2 initial condition */
    double q1;      /* quality of nest 1 (analogous to I1 from UM model)*/
    double q2;      /* quality of nest 2 (analogous to I2 from UM model)*/
    double r1_prime; /* rate of recruitment from nest 2 to nest 1 by already-converted ants */
    double r2_prime; /* rate of recruitment from nest 1 to nest 2 by already-converted ants */
    double l1;      /* leak (decay) term for nest 1 lost to source*/
    double l2;      /* leak (decay) term for nest 2 lost to source*/

    double std_dev; /* standard deviation for noise */
    int seed;       /* noise seed */

    /* noise arrays */
    double *cn_q1;  /* noise applied to quality of nest 1 */
    double *cn_q2;  /* noise applied to quality of nest 2 */
    double *cn_r1_prime; /* noise applied to recruitment leakage from S to nest 1 */
    double *cn_r2_prime; /* noise applied to recruitment leakage from S to nest 2 */
    double *cn_l1;  /* noise applied to leakage from nest 1 to source */
    double *cn_l2;  /* noise applied to leakage from nest 2 to source */

} params_indirect_britton_t;

/* parameters for simplified Pratt model (direct switching) */
typedef struct params_direct_britton_s {
    double h;       /* step size */
    int d;          /* duration */
    double population; /* total population of ants */
    double y1_0;    /* y1 initial condition */
    double y2_0;    /* y2 initial condition */
    double q1;      /* quality of nest 1 (analogous to I1 from UM model)*/
    double q2;      /* quality of nest 2 (analogous to I2 from UM model)*/
    double r1;      /* direct recruitment from nest 1 to 2 (analogous to inhibitory weight from UM model)*/
    double r2;      /* direct recruitment from nest 2 to 1 (analogous to inhibitory weight from UM model)*/
    double r1_prime; /* rate of recruitment from nest 2 to nest 1 by already-converted ants */
    double r2_prime; /* rate of recruitment from nest 1 to nest 2 by already-converted ants */
    double l1;      /* leak (decay) term for nest 1 lost to source*/
    double l2;      /* leak (decay) term for nest 2 lost to source*/

    double std_dev; /* standard deviation for noise */
    int seed;       /* noise seed */

    /* noise arrays */
    double *cn_q1;  /* noise applied to quality of nest 1 */
    double *cn_q2;  /* noise applied to quality of nest 2 */
    double *cn_r1;  /* noise applied to recruitment leakage from nest 1 to nest 2 */
    double *cn_r2;  /* noise applied to recruitment leakage from nest 2 to nest 1 */
    double *cn_r1_prime; /* noise applied to recruitment leakage from S to nest 1 */
    double *cn_r2_prime; /* noise applied to recruitment leakage from S to nest 2 */
    double *cn_l1;  /* noise applied to leakage from nest 1 to source */
    double *cn_l2;  /* noise applied to leakage from nest 2 to source */
} params_direct_britton_t;

/* parameters for gaze model */
typedef struct params_gaze_s {
    double h;           /* step size */
    int d;              /* duration */
    double y1_0;        /* y1 initial condition (priming)*/
    double y2_0;        /* y2 initial condition (priming)*/
    double I1;          /* input to y1 */
    double I2;          /* input to y2 */
    double g;           /* maximum gaze input */
    double gaze_start;  /* start time for gaze */
    double gaze_end;    /* end time for gaze */
    double a;           /* y-intercept for linear gaze activation */
    double l1;          /* decay (leak) for y1 */
    double l2;          /* decay (leak) for y2 */
    double w1;          /* inhibition weight from y1 to y2 */
    double w2;          /* inhibition weight from y2 to y1 */
    double t1;          /* location of target location 1 */
    double t2;          /* location of target location 2 */
    double tg;          /* location of gaze */

    double n_std_dev;   /* standard deviation for noise */
    double g_std_dev;   /* standard deviation for gaze */
    int seed;           /* noise seed */

    /* noise arrays */
    double *n_I1;   /* input I1 noise */
    double *n_I2;   /* input I2 noise */
    double *n_w1;   /* weight w1 noise */
    double *n_w2;   /* weight w2 noise */
    double *n_g1;   /* noise on gaze input to y1 */
    double *n_g2;   /* noise on gaze input to y2 */
    double *n_l1;   /* noise on decay from y1 */
    double *n_l2;   /* noise on decay from y2 */
} params_gaze_t;

/* set default parameters */
void um_set_defaults(params_um_t *p);
void pratt_set_defaults(params_pratt_t *p);
void indirect_britton_set_defaults(params_indirect_britton_t *p);
void direct_britton_set_defaults(params_direct_britton_t *p);
void gaze_set_defaults(params_gaze_t *p);

/* given a seeded random engine, fill arrays with random noise */
void um_set_noise(std::default_random_engine *g, double mean, double std_dev, int length, double *cn1, double *cn2);

void pratt_set_noise(std::default_random_engine *g,
                     double mean,
                     double std_dev,
                     int length,
                     double *cn_q1,
                     double *cn_q2,
                     double *cn_r1,
                     double *cn_r2,
                     double *cn_r1_prime,
                     double *cn_r2_prime,
                     double *cn_l1,
                     double *cn_l2);

void indirect_britton_set_noise(std::default_random_engine *g,
                     double mean,
                     double std_dev,
                     int length,
                     double *cn_q1,
                     double *cn_q2,
                     double *cn_r1_prime,
                     double *cn_r2_prime,
                     double *cn_l1,
                     double *cn_l2);

void direct_britton_set_noise(std::default_random_engine *g,
                     double mean,
                     double std_dev,
                     int length,
                     double *cn_q1,
                     double *cn_q2,
                     double *cn_r1,
                     double *cn_r2,
                     double *cn_r1_prime,
                     double *cn_r2_prime,
                     double *cn_l1,
                     double *cn_l2);

void gaze_set_noise(std::default_random_engine *g,
                    double mean,
                    double n_std_dev,
                    int length,
                    double *n_I1,
                    double *n_I2,
                    double *n_w1,
                    double *n_w2,
                    double *n_g1,
                    double *n_g2,
                    double *n_l1,
                    double *n_l2);

/* the numerical approximations */
void usher_mcclelland_eulers(params_um_t *params, double *results_y1, double *results_y2);
void usher_mcclelland_rk4(params_um_t *params, double * results_y1, double * results_y2);

void pratt_rk4(params_pratt_t *params, double * results_y1, double * results_y2);
double pratt_r_prime(double r_prime, double s, double cn);
double pratt_s(double s, double y1, double y2);

void indirect_britton_rk4(params_indirect_britton_t *params, double * results_y1, double * results_y2);
double indirect_britton_s(double s, double y1, double y2);

void direct_britton_rk4(params_direct_britton_t *params, double * results_y1, double * results_y2);
double direct_britton_s(double s, double y1, double y2);

void gaze_rk4(std::default_random_engine *g, params_gaze_t * params, double *results_y1, double *results_y2);

#endif // MODELS_H
