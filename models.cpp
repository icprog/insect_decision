#include "models.h"

void um_set_defaults(params_um_t *p) {
    p->h = 0.2;
    p->d = 10;
    p->y1_0 = 0.0;
    p->y2_0 = 0.0;
    p->I1 = 0.4;
    p->I2 = 0.4;
    p->w1 = 0.5;
    p->w2 = p->w1;
    p->l1 = 0.2;
    p->l2 = p->l1;
    p->seed = 3;
    p->std_dev = 0.1;
    p->cn1 = nullptr;
    p->cn2 = nullptr;
}

// TODO: allow adjustment of population size
void pratt_set_defaults(params_pratt_t *p) {
    p->h = 0.2;
    p->d = 10;
    p->population = 1000;
    p->y1_0 = 0.0;
    p->y2_0 = 0.0;
    p->q1 = 0.4;
    p->q2 = 0.4;
    p->r1 = 0.5;
    p->r2 = p->r1;
    p->r1_prime = 0.4;
    p->r2_prime = p->r1_prime;
    p->l1 = 0.2;
    p->l2 = p->l1;
    p->seed = 3;
    p->std_dev = 0.1;
    p->cn_q1 = nullptr;
    p->cn_q2 = nullptr;
    p->cn_r1 = nullptr;
    p->cn_r2 = nullptr;
    p->cn_l1 = nullptr;
    p->cn_l2 = nullptr;
}

void um_set_noise(std::default_random_engine *g, double mean, double std_dev, int length, double *cn1, double *cn2) {
    std::normal_distribution<double> distribution(mean,std_dev);
    for (int i=0; i<length; i++) {
        cn1[i] = distribution(*g);
    }
    for (int i=0; i<length; i++) {
        cn2[i] = distribution(*g);
    }
}

// TODO: R1' needs a noise distribution
// TODO: r1_prime and r2_prime need to be set by users
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
                     double *cn_l2) {
    std::normal_distribution<double> distribution(mean,std_dev);
    for (int i=0; i<length; i++) {
        cn_q1[i] = distribution(*g);
    }
    for (int i=0; i<length; i++) {
        cn_q2[i] = distribution(*g);
    }
    for (int i=0; i<length; i++) {
        cn_r1[i] = distribution(*g);
    }
    for (int i=0; i<length; i++) {
        cn_r2[i] = distribution(*g);
    }
    for (int i=0; i<length; i++) {
        cn_r1_prime[i] = distribution(*g);
    }
    for (int i=0; i<length; i++) {
        cn_r2_prime[i] = distribution(*g);
    }
    for (int i=0; i<length; i++) {
        cn_l1[i] = distribution(*g);
    }
    for (int i=0; i<length; i++) {
        cn_l2[i] = distribution(*g);
    }
}

void usher_mcclelland_rk4(params_um_t *params, double * results_y1, double * results_y2) {
    int length = (int)(params->d/params->h);

    /* set initial conditions */
    results_y1[0] = params->y1_0;
    results_y2[0] = params->y2_0;

    int i;
    for (i=0;i<length-1;i++) {
        double y1_k1 = params->I1 + params->cn1[i] - (params->l1 * results_y1[i]) - (params->w2 * results_y2[i]);
        double y2_k1 = params->I2 + params->cn2[i] - (params->l2 * results_y2[i]) - (params->w1 * results_y1[i]);
        double y1_k2 = params->I1 + params->cn1[i] - (params->l1 * (results_y1[i] + (y1_k1 * params->h/2))) - (params->w2 * (results_y2[i] + (y2_k1 * params->h/2)));
        double y2_k2 = params->I2 + params->cn2[i] - (params->l2 * (results_y2[i] + (y2_k1 * params->h/2))) - (params->w1 * (results_y1[i] + (y1_k1 * params->h/2)));
        double y1_k3 = params->I1 + params->cn1[i] - (params->l1 * (results_y1[i] + (y1_k2 * params->h/2))) - (params->w2 * (results_y2[i] + (y2_k2 * params->h/2)));
        double y2_k3 = params->I2 + params->cn2[i] - (params->l2 * (results_y2[i] + (y2_k2 * params->h/2))) - (params->w1 * (results_y1[i] + (y1_k2 * params->h/2)));
        double y1_k4 = params->I1 + params->cn1[i] - (params->l1 * (results_y1[i] + (y1_k3 * params->h))) - (params->w2 * (results_y2[i] + (y2_k3 * params->h)));
        double y2_k4 = params->I2 + params->cn2[i] - (params->l2 * (results_y2[i] + (y2_k3 * params->h))) - (params->w1 * (results_y1[i] + (y1_k3 * params->h)));
        results_y1[i+1] = results_y1[i] + (((y1_k1 + 2*y1_k2 + 2*y1_k3 + y1_k4)/6)*params->h);
        results_y2[i+1] = results_y2[i] + (((y2_k1 + 2*y2_k2 + 2*y2_k3 + y2_k4)/6)*params->h);
    }
}

/* input:
 *  r_prime: recruitment rate
 *  s: current population at source
 *  cn: recruitement rate noise
 */
double pratt_r_prime(double r_prime, double s, double cn) {
    if (s <= 0.0) return 0;
    else return (r_prime + cn);
}

double pratt_s(double n, double y1, double y2) {
    if (n - y1 - y2 <= 0) return 0;
    else return (n - y1 - y2);
}

// EULER'S
void pratt_rk4(params_pratt_t *params, double * results_y1, double * results_y2) {
    int length = (int)(params->d/params->h);

    /* set initial conditions */
    results_y1[0] = params->y1_0;
    results_y2[0] = params->y2_0;

    int i;
    for (i=0;i<length-1;i++) {
        double current_population = pratt_s(params->population, results_y1[i], results_y2[i]);
        double y1_k1 = current_population*(params->q1 + params->cn_q1[i])
                + results_y1[i] * pratt_r_prime(params->r1_prime, current_population, params->cn_r1_prime[i])
                + results_y2[i] * (params->r2 + params->cn_r2[i])
                - results_y1[i] * (params->r1 + params->cn_r1[i])
                - results_y1[i] * (params->l1 + params->cn_l1[i]);
        double y2_k1 = current_population*(params->q1 + params->cn_q1[i])
                + results_y2[i] * pratt_r_prime(params->r2_prime, current_population, params->cn_r2_prime[i])
                + results_y1[i] * (params->r1 + params->cn_r1[i])
                - results_y2[i] * (params->r2 + params->cn_r2[i])
                - results_y2[i] * (params->l2 + params->cn_l2[i]);
        results_y1[i+1] = results_y1[i] + (y1_k1 * params->h);
        results_y2[i+1] = results_y2[i] + (y2_k1 * params->h);

//        results_y1[i+1] = results_y1[i] + (((y1_k1 + 2*y1_k2 + 2*y1_k3 + y1_k4)/6)*params->h);
//        results_y2[i+1] = results_y2[i] + (((y2_k1 + 2*y2_k2 + 2*y2_k3 + y2_k4)/6)*params->h);
    }
}
