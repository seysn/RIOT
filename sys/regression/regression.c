#include <stdio.h>
#include <stdlib.h>

#include "regression.h"
#include "fix16.h"
#include "fmt.h"
#include "inttypes.h"

inline static float square(float x)
{
    return x*x;
}

int linreg(int n, const uint32_t x[], const float y[], polynom_t *poly)
{
    float sumx = 0.0;
    float sumx2 = 0.0;
    float sumxy = 0.0;
    float sumy = 0.0;
    float sumy2 = 0.0;

    for (int i = 0 ; i < n ; i++) {
        sumx += x[i];
        sumx2 += square(x[i]);
        sumxy += x[i] * y[i];
        sumy += y[i];
        sumy2 += square(y[i]);
    }
    float denom = (n * sumx2 - sumx * sumx);
    if (denom == 0) {
        poly->a = 0;
        poly->b = 0;
    }
    poly->a = (n * sumxy - sumx * sumy) / denom;
    poly->b = (sumy * sumx2 - sumx * sumxy) / denom;
    return 0;
}

float error(int n, const uint32_t x[], const float y[], polynom_t *poly)
{
    int i = 0;
    float res = 0.0;
    for(i = 0 ; i < n ; i++) {
        res += square(y[i] - (poly->a * x[i] + poly->b));
    }
    return fix16_to_float(fix16_sqrt(fix16_from_float(res/n)));
}

int add_value(polynom_t **polynom, data_t *data)
{
    if(*polynom == NULL) {
        /* First value to enter, we must allocate the polynom */
        /* TODO : Just do it ! Yes you can ! Don't let your dreams be dreams */
        *polynom = (polynom_t *)malloc(sizeof(polynom_t));
        (*polynom)->a = 0.1;
        (*polynom)->b = 0; /* TODO : change it to phydat_t */
        (*polynom)->start_time = data->timestamp;
        printf("Initializing polynom on %" PRIu32 "ms\n", xtimer_msec_from_ticks((*polynom)->start_time));
    }
    else {
        uint32_t x[NUMBER_OF_POINTS];
        float y[NUMBER_OF_POINTS];
        int i = 0;
        printf("Adding value %f\n", data->value);
        xtimer_ticks32_t time_delta = { .ticks32 = data->timestamp.ticks32 - (*polynom)->start_time.ticks32 };
        printf("The interval between the start of the polynom and new value is %" PRIu32 "\n",
               xtimer_msec_from_ticks(time_delta));

        uint32_t interval = xtimer_msec_from_ticks(time_delta) / NUMBER_OF_POINTS;
        linreg(NUMBER_OF_POINTS, x, y, *polynom);
        printf("Polynom is %fx + %f\n", (*polynom)->a, (*polynom)->b);
        for (i = 0 ; i < NUMBER_OF_POINTS ; i++) {
            x[i] = i * interval;
            y[i] = polynom_calculate(*polynom, x[i]);
            printf("Point %d, time = %" PRIu32 "ms is %f\n", i, x[i], y[i]);
        }
    }
    return 0;
}

/* TODO : Do it
int add_value_remote(polynom_t **polynom, data_t *data)
{

}

*/

float polynom_calculate(polynom_t *polynom, uint32_t timestamp_ms) {
    return polynom->a * timestamp_ms + polynom->b;
}
