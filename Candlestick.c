#include "Candlestick.h"
#include <stdlib.h>
#include <stdio.h>

#define X_AXIS 0
#define Y_AXIS 1

enum{
    TIME_DIM = 0,
    OPEN_DIM,
    CLOSE_DIM,
    LOW_DIM,
    HIGH_DIM 
};


double candlestick_get_max(candle array[], int32_t size, int32_t dim)
{
    double max = ((double*)(&array[0]))[dim];

    for(int32_t i = 0; i < size; i++){
        if(((double*)(&array[i]))[dim] > max){
            max = ((double*)(&array[i]))[dim];
        }
    }
 
    return max;
}

double candlestick_get_min(candle array[], int32_t size, int32_t dim)
{
    double min = ((double*)(&array[0]))[dim];

    for(int32_t i=0; i< size; i++){
        if(min > ((double*)(&array[i]))[dim]){
            min = ((double*)(&array[i]))[dim];
        }
    }

    return min;
}

candlestick* candlestick_init(int32_t candles_count)
{
    candlestick* p = (candlestick*)malloc(sizeof(candlestick));
    p->candles = (candle*)malloc(candles_count * sizeof(candle));
    if(!p->candles){
        free(p);
        return NULL;
    }
    p->candles_count = candles_count;
    p->displayed_candles_count = candles_count;
    p->current_candle_idx = 0;
    p->resolution = 1;
    return p;
}

static void resample(candlestick * plt){
    // Implement resampling logic here
    if( plt->displayed_candles_count < plt->current_candle_idx){
        for(int32_t i = 0; i < plt->displayed_candles_count; i++){
            int32_t index = i * (plt->current_candle_idx / plt->displayed_candles_count);
            plt->displayed_candles[i] = plt->candles[index];
        }
    }else{
        for(int32_t i = 0; i < plt->current_candle_idx; i++){
            plt->displayed_candles[i] = plt->candles[i];
        }
        plt->displayed_candles_count = plt->current_candle_idx;
    }
}

void candlestick_update(void* self){
        candlestick* plt = (candlestick*)self;
        if(plt != NULL){
            // zoom logic (future feature)
            resample(plt);
            plt->y_max_displayed_candle = candlestick_get_max(plt->displayed_candles, plt->displayed_candles_count,HIGH_DIM);
            plt->y_min_displayed_candle = candlestick_get_min(plt->displayed_candles, plt->displayed_candles_count,LOW_DIM);
            plt->x_max_displayed_candle = candlestick_get_max(plt->displayed_candles, plt->displayed_candles_count,TIME_DIM);
            plt->x_min_displayed_candle = candlestick_get_min(plt->displayed_candles, plt->displayed_candles_count,TIME_DIM);
            
        }
}

void candlestick_set_nb_candles_to_display(candlestick* p, int32_t nb_candles_to_display){
    if(p != NULL && nb_candles_to_display <= p->candles_count){
        p->displayed_candles_count = nb_candles_to_display;
    }
}

candlestick* candlestick_add_candle(candlestick* p, double t, double op,double cl, double lo, double hi){
    if(p && p->candles && p->candles_count > 0){
        if(p->current_candle_idx < p->candles_count){
            p->candles[p->current_candle_idx].t = t;
            p->candles[p->current_candle_idx].open = op;
            p->candles[p->current_candle_idx].close = cl;
            p->candles[p->current_candle_idx].low = lo;
            p->candles[p->current_candle_idx].high = hi;
            p->current_candle_idx++;
            return p;
        }
    }else{
        candlestick * p =candlestick_init(1);
        return(candlestick_add_candle(p,t,op,cl,lo,hi));
    }
}