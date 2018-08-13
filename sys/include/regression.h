/**
 * @degroup    middleware
 * @ingroup    net
 * @brief      Middleware Javascript
 *
 * middleware provide a high-level interface for writing Javascript applications
 * wich interracts with RIOT. It is also a wrapper arround JerryScript engine
 * functions
 *
 * @{
 *
 * @file
 * @brief    middleware definition
 *
 * @author    Anthony Durot <anthony.durot@inria.fr>
 */

#ifndef REGRESSION_H
#define REGRESSION_H
#ifdef __cplusplus
extern "C" {
#endif

#include "phydat.h"
#include "xtimer.h"

#define NUMBER_OF_POINTS 3

typedef struct {
    float a;
    float b;
    #ifdef SECOND_DEGRE
        float c;
    #endif
    xtimer_ticks32_t start_time;
} polynom_t;

typedef struct {
    float value;
    xtimer_ticks32_t timestamp;
} data_t;


int linreg(int n, const uint32_t x[], const float y[], polynom_t *poly);
float error(int n, const uint32_t x[], const float y[], polynom_t *poly);
int add_value(polynom_t **polynom, data_t *data);
float polynom_calculate(polynom_t *polynom, uint32_t timestamp_ms);

#endif
