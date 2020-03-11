#ifndef MICROBENCHMARK_H
#define MICROBENCHMARK_H

typedef enum {
    SQRT_LAT_HI,
    SQRT_GAP_HI,
    SQRT_LAT_LO,
    SQRT_GAP_LO,
    SIGMOID1_LAT_HI,
    SIGMOID1_GAP_HI,
    SIGMOID1_LAT_LO,
    SIGMOID1_GAP_LO,
    SIGMOID2_LAT_HI,
    SIGMOID2_GAP_HI,
    SIGMOID2_LAT_LO,
    SIGMOID2_GAP_LO,
    
    /* Limits */
    START_TEST = SQRT_LAT_HI,
    END_TEST = SIGMOID2_GAP_LO,
} microbenchmark_mode_t;


void    initialize_microbenchmark_data(microbenchmark_mode_t mode);
double  microbenchmark_get_add_latency ();
double  microbenchmark_get_add_gap     ();
double  microbenchmark_get_div_latency ();
double  microbenchmark_get_div_gap     ();
double  microbenchmark_get_sqrt_latency();
double  microbenchmark_get_sqrt_gap    ();
double  microbenchmark_get_sigmoid1_latency();
double  microbenchmark_get_sigmoid1_gap    ();
double  microbenchmark_get_sigmoid2_latency();
double  microbenchmark_get_sigmoid2_gap    ();


#endif /* MICROBENCHMARK_H */

