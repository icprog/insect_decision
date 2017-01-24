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
    p->h = 0.05;                /* time step */
    p->d = 10;                  /* duration */
    p->population = 100;        /* population size */
    p->y1_0 = 0.0;              /* nest 1 initial size */
    p->y2_0 = 0.0;              /* nest 2 initial size */
    p->q1 = 0.4;                /* quality of nest 1 */
    p->q2 = 0.4;                /* quality of nest 2 */
    p->r1 = 0.5;                /* rate of directly switching from nest 1 to nest 2 */
    p->r2 = p->r1;              /* rate of directly switching from nest 2 to nest 1 */
    p->r1_prime = 0.1;          /* rate of recruitment from S to nest 1 */
    p->r2_prime = p->r1_prime;  /* rate of recruitment from S to nest 2 */
    p->l1 = 0.2;                /* rate of leaking to S from 1 */
    p->l2 = p->l1;              /* rate of leaking to S from 2 */
    p->seed = 3;                /* random number seed */
    p->std_dev = 0.05;          /* random number std dev */
    p->cn_q1 = nullptr;         /* quality of nest 1 signal noise */
    p->cn_q2 = nullptr;         /* quality of nest 2 signal noise */
    p->cn_r1 = nullptr;         /* rate of directly switching from 1 to 2 noise */
    p->cn_r2 = nullptr;         /* rate of directly switching from 2 to 1 noise */
    p->cn_l1 = nullptr;         /* rate of leaking to S from 1 noise */
    p->cn_l2 = nullptr;         /* rate of leaking to S from 2 noise */
    p->cn_r1_prime = nullptr;   /* rate of recruitment from S to nest 1 noise */
    p->cn_r2_prime = nullptr;   /* rate of recruitment from S to nest 2 noise */
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

/*****************************************************************************
 * Usher-McClelland Model
 *
 * Based on Marshall et. al. 2009, equation 4.1
 *
 *****************************************************************************/

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

/*****************************************************************************
 * Simplified Pratt Model
 *
 * Based on Marshall et. al. 2009, equations 6.1 and 6.2.
 *
 * Note: In the Usher-McClelland model, the inhibition
 * strength is proportional to the activity of an ensemble,
 * e.g., as activation of option A increases, the inhibition
 * of option B increases. In contrast, in equation 6.2, the
 * inhibition (i.e., the direct transfer between nests)
 * does not have this property; instead, the number of ants leaving
 * nest B for nest A is simply a function of the number of ants in B.
 * That is, as the size of nest B grows, more ants leave it for A, and
 * vice versa. We found this makes it difficult for the model to converge,
 * because the only recriutment to a nest is done from the source nest, S.
 * So, following the lead given in figure 4 (p. 1068), we've made
 * inter-nest recruitment (i.e., inhibition) proportional to the size of
 * the nest -- the more ants a nest has, the more ants it can steal away from
 * both S and the other nest.
 *****************************************************************************/

/*
 * input:
 *  population
 *  recruitment_rate
 *  recruitement_noise
 *
 * output:
 *  0 if population is <= 0
 *  recruitment_rate + recruitment_noise, otherwise
 */
double pratt_r_prime(double population, double recruitment_rate, double recruitment_noise) {
    if (population <= 0.0) return 0;
    else return (recruitment_rate + recruitment_noise);
}

/*
 * input:
 *  total population size
 *  number in nest 1
 *  number in nest 2
 *
 * output:
 *  population left in source
 */
double pratt_s(double total_population, double y1, double y2) {
    if (total_population-y1-y2 <= 0) return 0.;
    else return (total_population-y1-y2);
}

// EULER'S
void pratt_rk4(params_pratt_t *params, double * results_y1, double * results_y2) {

    int length = (int)(params->d/params->h);

    /* set initial conditions */
    results_y1[0] = params->y1_0;
    results_y2[0] = params->y2_0;

    int i;
    for (i=0;i<length-1;i++) {
        double s = pratt_s(params->population, results_y1[i], results_y2[i]);
        double y1_k1 = (s*(params->q1 + params->cn_q1[i])
                + results_y1[i] * pratt_r_prime(s, params->r1_prime, params->cn_r1_prime[i])
                + results_y1[i] * results_y2[i] * (params->r2 + params->cn_r2[i])
                - results_y2[i] * results_y1[i] * (params->r1 + params->cn_r1[i])
                - results_y1[i] * (params->l1 + params->cn_l1[i]));
        double y2_k1 = (s*(params->q2 + params->cn_q2[i])
                + results_y2[i] * pratt_r_prime(s, params->r2_prime, params->cn_r2_prime[i])
                + results_y2[i] * results_y1[i] * (params->r1 + params->cn_r1[i])
                - results_y1[i] * results_y2[i] * (params->r2 + params->cn_r2[i])
                - results_y2[i] * (params->l2 + params->cn_l2[i]));
        results_y1[i+1] = results_y1[i] + (y1_k1 * params->h);
        results_y2[i+1] = results_y2[i] + (y2_k1 * params->h);

//        results_y1[i+1] = results_y1[i] + (((y1_k1 + 2*y1_k2 + 2*y1_k3 + y1_k4)/6)*params->h);
//        results_y2[i+1] = results_y2[i] + (((y2_k1 + 2*y2_k2 + 2*y2_k3 + y2_k4)/6)*params->h);
    }
}