#include "models.h"

/*****************************************************************************
 *
 * Usher-McClelland Model
 *
 * Based on Marshall et. al. 2009, equation 4.1
 *
 *****************************************************************************/
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

/* Fill the noise arrays for the UM model */
void um_set_noise(std::default_random_engine *g, double mean, double std_dev, int length, double *cn1, double *cn2) {
    std::normal_distribution<double> distribution(mean,std_dev);
    for (int i=0; i<length; i++) {
        cn1[i] = distribution(*g);
        cn2[i] = distribution(*g);
    }
}

void usher_mcclelland_eulers(params_um_t *params, double *results_y1, double *results_y2) {
    int length = ceil(params->d/params->h);

    /* set initial conditions */
    results_y1[0] = params->y1_0;
    results_y2[0] = params->y2_0;

    int i;
    for (i=0;i<length-1;i++) {
        double y1_k1 = params->I1
                + params->cn1[i]
                - (params->l1 * results_y1[i])
                - (params->w2 * results_y2[i]);
        double y2_k1 = params->I2
                + params->cn2[i]
                - (params->l2 * results_y2[i])
                - (params->w1 * results_y1[i]);
        results_y1[i+1] = results_y1[i] + (y1_k1 * params->h);
        results_y2[i+1] = results_y2[i] + (y2_k1 * params->h);
    }
}

void usher_mcclelland_rk4(params_um_t *params, double * results_y1, double * results_y2) {

    int length = ceil(params->d/params->h);

    /* set initial conditions */
    results_y1[0] = params->y1_0;
    results_y2[0] = params->y2_0;

    int i;
    for (i=0;i<length-1;i++) {
        double y1_k1 = params->I1
                + params->cn1[i]
                - (params->l1 * results_y1[i])
                - (params->w2 * results_y2[i]);
        double y2_k1 = params->I2
                + params->cn2[i]
                - (params->l2 * results_y2[i])
                - (params->w1 * results_y1[i]);
        double y1_k2 = params->I1
                + params->cn1[i]
                - (params->l1 * (results_y1[i] + (y1_k1 * params->h/2)))
                - (params->w2 * (results_y2[i] + (y2_k1 * params->h/2)));
        double y2_k2 = params->I2
                + params->cn2[i]
                - (params->l2 * (results_y2[i] + (y2_k1 * params->h/2)))
                - (params->w1 * (results_y1[i] + (y1_k1 * params->h/2)));
        double y1_k3 = params->I1
                + params->cn1[i]
                - (params->l1 * (results_y1[i] + (y1_k2 * params->h/2)))
                - (params->w2 * (results_y2[i] + (y2_k2 * params->h/2)));
        double y2_k3 = params->I2
                + params->cn2[i]
                - (params->l2 * (results_y2[i] + (y2_k2 * params->h/2)))
                - (params->w1 * (results_y1[i] + (y1_k2 * params->h/2)));
        double y1_k4 = params->I1
                + params->cn1[i]
                - (params->l1 * (results_y1[i] + (y1_k3 * params->h)))
                - (params->w2 * (results_y2[i] + (y2_k3 * params->h)));
        double y2_k4 = params->I2
                + params->cn2[i]
                - (params->l2 * (results_y2[i] + (y2_k3 * params->h)))
                - (params->w1 * (results_y1[i] + (y1_k3 * params->h)));
        results_y1[i+1] = results_y1[i] + (((y1_k1 + 2*y1_k2 + 2*y1_k3 + y1_k4)/6)*params->h);
        results_y2[i+1] = results_y2[i] + (((y2_k1 + 2*y2_k2 + 2*y2_k3 + y2_k4)/6)*params->h);
    }
}

/*****************************************************************************
 *
 * Simplified Pratt Model
 *
 * Based on Marshall et. al. 2009, equations 6.1 and 6.2.
 *
 *****************************************************************************/

void pratt_set_defaults(params_pratt_t *p) {
    p->h = 0.05;                /* time step */
    p->d = 10;                  /* duration */
    p->population = 100;        /* population size */
    p->y1_0 = 0.0;              /* nest 1 initial size */
    p->y2_0 = 0.0;              /* nest 2 initial size */
    p->q1 = 0.4;                /* quality of nest 1 */
    p->q2 = 0.4;                /* quality of nest 2 */
    p->r1 = 0.3;                /* rate of directly switching from nest 1 to nest 2 */
    p->r2 = 0.3;                /* rate of directly switching from nest 2 to nest 1 */
    p->r1_prime = 0.1;          /* rate of recruitment from S to nest 1 */
    p->r2_prime = 0.1;          /* rate of recruitment from S to nest 2 */
    p->l1 = 0.2;                /* rate of leaking to S from 1 */
    p->l2 = 0.2;                /* rate of leaking to S from 2 */
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

/* Fill the noise arrays for the simplified pratt model */
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
        cn_q2[i] = distribution(*g);
        cn_r1[i] = distribution(*g);
        cn_r2[i] = distribution(*g);
        cn_r1_prime[i] = distribution(*g);
        cn_r2_prime[i] = distribution(*g);
        cn_l1[i] = distribution(*g);
        cn_l2[i] = distribution(*g);
    }
}

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

void pratt_rk4(params_pratt_t *params, double * results_y1, double * results_y2) {

    int length = ceil(params->d/params->h);

    /* set initial conditions */
    results_y1[0] = params->y1_0;
    results_y2[0] = params->y2_0;

    int i;
    for (i=0;i<length-1;i++) {
        double s = pratt_s(params->population, results_y1[i], results_y2[i]);
        double y1_k1 = (s*(params->q1 + params->cn_q1[i])
                + (results_y1[i] * pratt_r_prime(s, params->r1_prime, params->cn_r1_prime[i]))
                //+ (results_y1[i] * results_y2[i] * (params->r2 + params->cn_r2[i]))
                //- (results_y2[i] * results_y1[i] * (params->r1 + params->cn_r1[i]))
                + (results_y2[i] * (params->r2 + params->cn_r2[i]))
                - (results_y1[i] * (params->r1 + params->cn_r1[i]))
                - (results_y1[i] * (params->l1 + params->cn_l1[i])));
        double y2_k1 = (s*(params->q2 + params->cn_q2[i])
                + (results_y2[i] * pratt_r_prime(s, params->r2_prime, params->cn_r2_prime[i]))
                //+ (results_y2[i] * results_y1[i] * (params->r1 + params->cn_r1[i]))
                //- (results_y1[i] * results_y2[i] * (params->r2 + params->cn_r2[i]))
                + (results_y1[i] * (params->r1 + params->cn_r1[i]))
                - (results_y2[i] * (params->r2 + params->cn_r2[i]))
                - (results_y2[i] * (params->l2 + params->cn_l2[i])));
        double y1_k2 = (s*(params->q1 + params->cn_q1[i])
                + ((results_y1[i] + (y1_k1 * params->h/2)) * pratt_r_prime(s, params->r1_prime, params->cn_r1_prime[i]))
                //+ ((results_y1[i] + (y1_k1 * params->h/2)) * (results_y2[i] + (y2_k1 * params->h/2)) * (params->r2 + params->cn_r2[i]))
                //- ((results_y2[i] + (y2_k1 * params->h/2)) * (results_y1[i] + (y1_k1 * params->h/2)) * (params->r1 + params->cn_r1[i]))
                + ((results_y2[i] + (y2_k1 * params->h/2)) * (params->r2 + params->cn_r2[i]))
                - ((results_y1[i] + (y1_k1 * params->h/2)) * (params->r1 + params->cn_r1[i]))
                - ((results_y1[i] + (y1_k1 * params->h/2)) * (params->l1 + params->cn_l1[i])));
        double y2_k2 = (s*(params->q2 + params->cn_q2[i])
                + ((results_y2[i] + (y2_k1 * params->h/2)) * pratt_r_prime(s, params->r2_prime, params->cn_r2_prime[i]))
                //+ ((results_y2[i] + (y2_k1 * params->h/2)) * (results_y1[i] + (y1_k1 * params->h/2)) * (params->r1 + params->cn_r1[i]))
                //- ((results_y1[i] + (y1_k1 * params->h/2)) * (results_y2[i] + (y2_k1 * params->h/2)) * (params->r2 + params->cn_r2[i]))
                + ((results_y1[i] + (y1_k1 * params->h/2)) * (params->r1 + params->cn_r1[i]))
                - ((results_y2[i] + (y2_k1 * params->h/2)) * (params->r2 + params->cn_r2[i]))
                - ((results_y2[i] + (y2_k1 * params->h/2)) * (params->l2 + params->cn_l2[i])));
        double y1_k3 = (s*(params->q1 + params->cn_q1[i])
                + ((results_y1[i] + (y1_k2 * params->h/2)) * pratt_r_prime(s, params->r1_prime, params->cn_r1_prime[i]))
                //+ ((results_y1[i] + (y1_k2 * params->h/2)) * (results_y2[i] + (y2_k2 * params->h/2)) * (params->r2 + params->cn_r2[i]))
                //- ((results_y2[i] + (y2_k2 * params->h/2)) * (results_y1[i] + (y1_k2 * params->h/2)) * (params->r1 + params->cn_r1[i]))
                + ((results_y2[i] + (y2_k2 * params->h/2)) * (params->r2 + params->cn_r2[i]))
                - ((results_y1[i] + (y1_k2 * params->h/2)) * (params->r1 + params->cn_r1[i]))
                - ((results_y1[i] + (y1_k2 * params->h/2)) * (params->l1 + params->cn_l1[i])));
        double y2_k3 = (s*(params->q2 + params->cn_q2[i])
                + ((results_y2[i] + (y2_k2 * params->h/2)) * pratt_r_prime(s, params->r2_prime, params->cn_r2_prime[i]))
                //+ ((results_y2[i] + (y2_k2 * params->h/2)) * (results_y1[i] + (y1_k2 * params->h/2)) * (params->r1 + params->cn_r1[i]))
                //- ((results_y1[i] + (y1_k2 * params->h/2)) * (results_y2[i] + (y2_k2 * params->h/2)) * (params->r2 + params->cn_r2[i]))
                + ((results_y1[i] + (y1_k2 * params->h/2)) * (params->r1 + params->cn_r1[i]))
                - ((results_y2[i] + (y2_k2 * params->h/2)) * (params->r2 + params->cn_r2[i]))
                - ((results_y2[i] + (y2_k2 * params->h/2)) * (params->l2 + params->cn_l2[i])));
        double y1_k4 = (s*(params->q1 + params->cn_q1[i])
                + ((results_y1[i] + (y1_k3 * params->h)) * pratt_r_prime(s, params->r1_prime, params->cn_r1_prime[i]))
                //+ ((results_y1[i] + (y1_k3 * params->h)) * (results_y2[i] + (y2_k3 * params->h)) * (params->r2 + params->cn_r2[i]))
                //- ((results_y2[i] + (y2_k3 * params->h)) * (results_y1[i] + (y1_k3 * params->h)) * (params->r1 + params->cn_r1[i]))
                + ((results_y2[i] + (y2_k3 * params->h)) * (params->r2 + params->cn_r2[i]))
                - ((results_y1[i] + (y1_k3 * params->h)) * (params->r1 + params->cn_r1[i]))
                - ((results_y1[i] + (y1_k3 * params->h)) * (params->l1 + params->cn_l1[i])));
        double y2_k4 = (s*(params->q2 + params->cn_q2[i])
                + ((results_y2[i] + (y2_k3 * params->h)) * pratt_r_prime(s, params->r2_prime, params->cn_r2_prime[i]))
                //+ ((results_y2[i] + (y2_k3 * params->h)) * (results_y1[i] + (y1_k3 * params->h)) * (params->r1 + params->cn_r1[i]))
                //- ((results_y1[i] + (y1_k3 * params->h)) * (results_y2[i] + (y2_k3 * params->h)) * (params->r2 + params->cn_r2[i]))
                + ((results_y1[i] + (y1_k3 * params->h)) * (params->r1 + params->cn_r1[i]))
                - ((results_y2[i] + (y2_k3 * params->h)) * (params->r2 + params->cn_r2[i]))
                - ((results_y2[i] + (y2_k3 * params->h)) * (params->l2 + params->cn_l2[i])));
        /* RK4 method */
        results_y1[i+1] = results_y1[i] + (((y1_k1 + 2*y1_k2 + 2*y1_k3 + y1_k4)/6)*params->h);
        results_y2[i+1] = results_y2[i] + (((y2_k1 + 2*y2_k2 + 2*y2_k3 + y2_k4)/6)*params->h);
    }
}

/*****************************************************************************
 *
 * Simplified Indirect Britton Model
 *
 * Based on Marshall et. al. 2009, equation 6.3.
 *
 *****************************************************************************/

void indirect_britton_set_defaults(params_indirect_britton_t *p) {
    p->h = 0.05;                /* time step */
    p->d = 10;                  /* duration */
    p->population = 100;        /* population size */
    p->y1_0 = 0.0;              /* nest 1 initial size */
    p->y2_0 = 0.0;              /* nest 2 initial size */
    p->q1 = 0.4;                /* quality of nest 1 */
    p->q2 = 0.4;                /* quality of nest 2 */
    p->r1_prime = 0.1;          /* rate of recruitment from S to nest 1 */
    p->r2_prime = 0.1;          /* rate of recruitment from S to nest 2 */
    p->l1 = 0.2;                /* rate of leaking to S from 1 */
    p->l2 = 0.2;                /* rate of leaking to S from 2 */
    p->seed = 3;                /* random number seed */
    p->std_dev = 0.05;          /* random number std dev */
    p->cn_q1 = nullptr;         /* quality of nest 1 signal noise */
    p->cn_q2 = nullptr;         /* quality of nest 2 signal noise */
    p->cn_l1 = nullptr;         /* rate of leaking to S from 1 noise */
    p->cn_l2 = nullptr;         /* rate of leaking to S from 2 noise */
    p->cn_r1_prime = nullptr;   /* rate of recruitment from S to nest 1 noise */
    p->cn_r2_prime = nullptr;   /* rate of recruitment from S to nest 2 noise */
}

/* Fill the noise arrays for the simplified pratt model */
void indirect_britton_set_noise(std::default_random_engine *g,
                     double mean,
                     double std_dev,
                     int length,
                     double *cn_q1,
                     double *cn_q2,
                     double *cn_r1_prime,
                     double *cn_r2_prime,
                     double *cn_l1,
                     double *cn_l2) {

    std::normal_distribution<double> distribution(mean,std_dev);

    for (int i=0; i<length; i++) {
        cn_q1[i] = distribution(*g);
        cn_q2[i] = distribution(*g);
        cn_r1_prime[i] = distribution(*g);
        cn_r2_prime[i] = distribution(*g);
        cn_l1[i] = distribution(*g);
        cn_l2[i] = distribution(*g);
    }
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
double indirect_britton_s(double total_population, double y1, double y2) {
    if (total_population-y1-y2 <= 0) return 0.;
    else return (total_population-y1-y2);
}

void indirect_britton_rk4(params_indirect_britton_t *params, double * results_y1, double * results_y2) {

    int length = ceil(params->d/params->h);

    /* set initial conditions */
    results_y1[0] = params->y1_0;
    results_y2[0] = params->y2_0;

    int i;
    for (i=0;i<length-1;i++) {
        double s = indirect_britton_s(params->population, results_y1[i], results_y2[i]);
        double y1_k1 = (s*(params->q1 + params->cn_q1[i])
                + (results_y1[i] * s * (params->r1_prime + params->cn_r1_prime[i]))
                - (results_y1[i] * (params->l1 + params->cn_l1[i])));
        double y2_k1 = (s*(params->q2 + params->cn_q2[i])
                + (results_y2[i] * s * (params->r2_prime + params->cn_r2_prime[i]))
                - (results_y2[i] * (params->l2 + params->cn_l2[i])));
        double y1_k2 = (s*(params->q1 + params->cn_q1[i])
                + ((results_y1[i] + (y1_k1 * params->h/2)) * s * (params->r1_prime + params->cn_r1_prime[i]))
                - ((results_y1[i] + (y1_k1 * params->h/2)) * (params->l1 + params->cn_l1[i])));
        double y2_k2 = (s*(params->q2 + params->cn_q2[i])
                + ((results_y2[i] + (y2_k1 * params->h/2)) * s * (params->r2_prime + params->cn_r2_prime[i]))
                - ((results_y2[i] + (y2_k1 * params->h/2)) * (params->l2 + params->cn_l2[i])));
        double y1_k3 = (s*(params->q1 + params->cn_q1[i])
                + ((results_y1[i] + (y1_k2 * params->h/2)) * s * (params->r1_prime + params->cn_r1_prime[i]))
                - ((results_y1[i] + (y1_k2 * params->h/2)) * (params->l1 + params->cn_l1[i])));
        double y2_k3 = (s*(params->q2 + params->cn_q2[i])
                + ((results_y2[i] + (y2_k2 * params->h/2)) * s * (params->r2_prime + params->cn_r2_prime[i]))
                - ((results_y2[i] + (y2_k2 * params->h/2)) * (params->l2 + params->cn_l2[i])));
        double y1_k4 = (s*(params->q1 + params->cn_q1[i])
                + ((results_y1[i] + (y1_k3 * params->h)) * s * (params->r1_prime + params->cn_r1_prime[i]))
                - ((results_y1[i] + (y1_k3 * params->h)) * (params->l1 + params->cn_l1[i])));
        double y2_k4 = (s*(params->q2 + params->cn_q2[i])
                + ((results_y2[i] + (y2_k3 * params->h)) * s * (params->r2_prime + params->cn_r2_prime[i]))
                - ((results_y2[i] + (y2_k3 * params->h)) * (params->l2 + params->cn_l2[i])));
        /* Euler's method */
//        results_y1[i+1] = results_y1[i] + (y1_k1 * params->h);
//        results_y2[i+1] = results_y2[i] + (y2_k1 * params->h);
        /* RK4 method */
        results_y1[i+1] = results_y1[i] + (((y1_k1 + 2*y1_k2 + 2*y1_k3 + y1_k4)/6)*params->h);
        results_y2[i+1] = results_y2[i] + (((y2_k1 + 2*y2_k2 + 2*y2_k3 + y2_k4)/6)*params->h);
    }
}

/*****************************************************************************
 *
 * Simplified Direct Britton Model
 *
 * Based on Marshall et. al. 2009, equation 6.4.
 *
 *****************************************************************************/

void direct_britton_set_defaults(params_direct_britton_t *p) {
    p->h = 0.05;                /* time step */
    p->d = 10;                  /* duration */
    p->population = 100;        /* population size */
    p->y1_0 = 0.0;              /* nest 1 initial size */
    p->y2_0 = 0.0;              /* nest 2 initial size */
    p->q1 = 0.4;                /* quality of nest 1 */
    p->q2 = 0.4;                /* quality of nest 2 */
    p->r1 = 0.2;
    p->r2 = 0.2;
    p->r1_prime = 0.1;          /* rate of recruitment from S to nest 1 */
    p->r2_prime = 0.1;          /* rate of recruitment from S to nest 2 */
    p->l1 = 0.2;                /* rate of leaking to S from 1 */
    p->l2 = 0.2;                /* rate of leaking to S from 2 */
    p->seed = 3;                /* random number seed */
    p->std_dev = 0.05;          /* random number std dev */
    p->cn_q1 = nullptr;         /* quality of nest 1 signal noise */
    p->cn_q2 = nullptr;         /* quality of nest 2 signal noise */
    p->cn_r1 = nullptr;
    p->cn_r2 = nullptr;
    p->cn_l1 = nullptr;         /* rate of leaking to S from 1 noise */
    p->cn_l2 = nullptr;         /* rate of leaking to S from 2 noise */
    p->cn_r1_prime = nullptr;   /* rate of recruitment from S to nest 1 noise */
    p->cn_r2_prime = nullptr;   /* rate of recruitment from S to nest 2 noise */
}

/* Fill the noise arrays for the simplified pratt model */
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
                     double *cn_l2) {

    std::normal_distribution<double> distribution(mean,std_dev);

    for (int i=0; i<length; i++) {
        cn_q1[i] = distribution(*g);
        cn_q2[i] = distribution(*g);
        cn_r1[i] = distribution(*g);
        cn_r2[i] = distribution(*g);
        cn_r1_prime[i] = distribution(*g);
        cn_r2_prime[i] = distribution(*g);
        cn_l1[i] = distribution(*g);
        cn_l2[i] = distribution(*g);
    }
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
double direct_britton_s(double total_population, double y1, double y2) {
    if (total_population-y1-y2 <= 0) return 0.;
    else return (total_population-y1-y2);
}

void direct_britton_rk4(params_direct_britton_t *params, double * results_y1, double * results_y2) {

    int length = ceil(params->d/params->h);

    /* set initial conditions */
    results_y1[0] = params->y1_0;
    results_y2[0] = params->y2_0;

    int i;
    for (i=0;i<length-1;i++) {
        double s = indirect_britton_s(params->population, results_y1[i], results_y2[i]);
        double y1_k1 = (s*(params->q1 + params->cn_q1[i])
                + (results_y1[i] * s * (params->r1_prime + params->cn_r1_prime[i]))
                + (results_y1[i] * results_y2[i] * (params->r1 - params->r2 + params->cn_r1[i] - params->cn_r2[i]))
                - (results_y1[i] * (params->l1 + params->cn_l1[i])));
        double y2_k1 = (s*(params->q2 + params->cn_q2[i])
                + (results_y2[i] * s * (params->r2_prime + params->cn_r2_prime[i]))
                - (results_y1[i] * results_y2[i] * (params->r1 - params->r2 + params->cn_r1[i] - params->cn_r2[i]))
                - (results_y2[i] * (params->l2 + params->cn_l2[i])));
        double y1_k2 = (s*(params->q1 + params->cn_q1[i])
                + ((results_y1[i] + (y1_k1 * params->h/2)) * s * (params->r1_prime + params->cn_r1_prime[i]))
                + ((results_y1[i] + (y1_k1 * params->h/2)) * (results_y2[i] + (y2_k1 * params->h/2)) * (params->r1 - params->r2 + params->cn_r1[i] - params->cn_r2[i]))
                - ((results_y1[i] + (y1_k1 * params->h/2)) * (params->l1 + params->cn_l1[i])));
        double y2_k2 = (s*(params->q2 + params->cn_q2[i])
                + ((results_y2[i] + (y2_k1 * params->h/2)) * s * (params->r2_prime + params->cn_r2_prime[i]))
                - ((results_y1[i] + (y1_k1 * params->h/2)) * (results_y2[i] + (y2_k1 * params->h/2)) * (params->r1 - params->r2 + params->cn_r1[i] - params->cn_r2[i]))
                - ((results_y2[i] + (y2_k1 * params->h/2)) * (params->l2 + params->cn_l2[i])));
        double y1_k3 = (s*(params->q1 + params->cn_q1[i])
                + ((results_y1[i] + (y1_k2 * params->h/2)) * s * (params->r1_prime + params->cn_r1_prime[i]))
                + ((results_y1[i] + (y1_k2 * params->h/2)) * (results_y2[i] + (y2_k2 * params->h/2)) * (params->r1 - params->r2 + params->cn_r1[i] - params->cn_r2[i]))
                - ((results_y1[i] + (y1_k2 * params->h/2)) * (params->l1 + params->cn_l1[i])));
        double y2_k3 = (s*(params->q2 + params->cn_q2[i])
                + ((results_y2[i] + (y2_k2 * params->h/2)) * s * (params->r2_prime + params->cn_r2_prime[i]))
                - ((results_y1[i] + (y1_k2 * params->h/2)) * (results_y2[i] + (y2_k2 * params->h/2)) * (params->r1 - params->r2 + params->cn_r1[i] - params->cn_r2[i]))
                - ((results_y2[i] + (y2_k2 * params->h/2)) * (params->l2 + params->cn_l2[i])));
        double y1_k4 = (s*(params->q1 + params->cn_q1[i])
                + ((results_y1[i] + (y1_k3 * params->h)) * s * (params->r1_prime + params->cn_r1_prime[i]))
                + ((results_y1[i] + (y1_k3 * params->h)) * (results_y2[i] + (y2_k3 * params->h)) * (params->r1 - params->r2 + params->cn_r1[i] - params->cn_r2[i]))
                - ((results_y1[i] + (y1_k3 * params->h)) * (params->l1 + params->cn_l1[i])));
        double y2_k4 = (s*(params->q2 + params->cn_q2[i])
                + ((results_y2[i] + (y2_k3 * params->h)) * s * (params->r2_prime + params->cn_r2_prime[i]))
                - ((results_y1[i] + (y1_k3 * params->h)) * (results_y2[i] + (y2_k3 * params->h)) * (params->r1 - params->r2 + params->cn_r1[i] - params->cn_r2[i]))
                - ((results_y2[i] + (y2_k3 * params->h)) * (params->l2 + params->cn_l2[i])));
        /* Euler's method */
//        results_y1[i+1] = results_y1[i] + (y1_k1 * params->h);
//        results_y2[i+1] = results_y2[i] + (y2_k1 * params->h);
        /* RK4 method */
        results_y1[i+1] = results_y1[i] + (((y1_k1 + 2*y1_k2 + 2*y1_k3 + y1_k4)/6)*params->h);
        results_y2[i+1] = results_y2[i] + (((y2_k1 + 2*y2_k2 + 2*y2_k3 + y2_k4)/6)*params->h);
    }
}

/*****************************************************************************
 *
 * Gaze Model
 *
 * Currently calculates gaze input as a linear function of the distince
 * of the gaze from each target.
 *
 *****************************************************************************/
void gaze_set_defaults(params_gaze_t *p) {
    p->h = 0.2;
    p->d = 30;
    p->y1_0 = 0.0;
    p->y2_0 = 0.0;
    p->I1 = 0.05;
    p->I2 = 0.05;
    p->g = 0.0;
    p->gaze_start = 0.0;
    p->gaze_end = 0.0;
    p->l1 = 0.2;
    p->l2 = p->l1;
    p->w1 = 0.3;
    p->w2 = p->w1;
    p->t1 = 45.0;
    p->t2 = 135.0;
    p->tg = p->t1;
    p->a = 5;
    p->n_std_dev = 0.1;
    p->g_std_dev = 0.1;
    p->seed = 32;
}

/* Fill the noise arrays for the UM model */
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
                    double *n_l2)
{
    std::normal_distribution<double> distribution(mean,n_std_dev);
    for (int i=0; i<length; i++) {
        n_I1[i] = distribution(*g);
        n_I2[i] = distribution(*g);
        n_w1[i] = distribution(*g);
        n_w2[i] = distribution(*g);
        n_g1[i] = distribution(*g);
        n_g2[i] = distribution(*g);
        n_l1[i] = distribution(*g);
        n_l2[i] = distribution(*g);
    }
}

void gaze_rk4(std::default_random_engine *g, params_gaze_t *params, double *results_y1, double *results_y2) {
    int length = ceil(params->d/params->h);

    /* for generating gaze location */
    std::normal_distribution<double> distribution(0.0,params->g_std_dev);
    int gs = floor(params->gaze_start/params->h);
    int ge = floor(params->gaze_end/params->h);
    double g1 = 0.;
    double g2 = 0.;

    /* set initial conditions */
    results_y1[0] = params->y1_0;
    results_y2[0] = params->y2_0;

    /* outside of gaze interval, the gaze is directed at nothing */
    int i;
    for (i=0;i<length-1;i++) {
        if (i >= gs && i <= ge) {
            /* gaze is activated */
            double gaze_offset = distribution(*g);
            if(params->tg >= params->t1) {
                g1 = (params->g) * (((-1)/params->a * (params->tg - params->t1 + gaze_offset) + 1));
            } else {
                g1 = (params->g) * (((-1)/params->a * (params->t1 - params->tg - gaze_offset) + 1));
            }
            if(params->tg >= params->t2) {
                g2 = (params->g) * (((-1)/params->a * (params->tg - params->t2 + gaze_offset) + 1));
            } else {
                g2 = (params->g) * (((-1)/params->a * (params->t2 - params->tg - gaze_offset) + 1));
            }
            if (g1 < 0.0) g1 = 0.0;
            if (g2 < 0.0) g2 = 0.0;
        } else {
            /* gaze is not active */
            g1 = 0.;
            g2 = 0.;
        }

        double y1_k1 = (params->I1 + params->n_I1[i])
                + (g1 + params->n_g1[i])
                - (params->l1 * results_y1[i])
                - (params->w2 * results_y2[i]);
        double y2_k1 = (params->I2 + params->n_I2[i])
                + (g2 + params->n_g2[i])
                - (params->l2 * results_y2[i])
                - (params->w1 * results_y1[i]);
        double y1_k2 = (params->I1 + params->n_I1[i])
                + (g1 + params->n_g1[i])
                - (params->l1 * (results_y1[i] + (y1_k1 * params->h/2)))
                - (params->w2 * (results_y2[i] + (y2_k1 * params->h/2)));
        double y2_k2 = (params->I2 + params->n_I2[i])
                + (g2 + params->n_g2[i])
                - (params->l2 * (results_y2[i] + (y2_k1 * params->h/2)))
                - (params->w1 * (results_y1[i] + (y1_k1 * params->h/2)));
        double y1_k3 = (params->I1 + params->n_I1[i])
                + (g1 + params->n_g1[i])
                - (params->l1 * (results_y1[i] + (y1_k2 * params->h/2)))
                - (params->w2 * (results_y2[i] + (y2_k2 * params->h/2)));
        double y2_k3 = (params->I2 + params->n_I2[i])
                + (g2 + params->n_g2[i])
                - (params->l2 * (results_y2[i] + (y2_k2 * params->h/2)))
                - (params->w1 * (results_y1[i] + (y1_k2 * params->h/2)));
        double y1_k4 = (params->I1 + params->n_I1[i])
                + (g1 + params->n_g1[i])
                - (params->l1 * (results_y1[i] + (y1_k3 * params->h)))
                - (params->w2 * (results_y2[i] + (y2_k3 * params->h)));
        double y2_k4 = (params->I2 + params->n_I2[i])
                + (g2 + params->n_g2[i])
                - (params->l2 * (results_y2[i] + (y2_k3 * params->h)))
                - (params->w1 * (results_y1[i] + (y1_k3 * params->h)));
        results_y1[i+1] = results_y1[i] + (((y1_k1 + 2*y1_k2 + 2*y1_k3 + y1_k4)/6)*params->h);
        results_y2[i+1] = results_y2[i] + (((y2_k1 + 2*y2_k2 + 2*y2_k3 + y2_k4)/6)*params->h);
    }
}

