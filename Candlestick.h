#ifndef PLOT_H
#define PLOT_H

#include <stdint.h>

#define MAX_DISPLAYABLE_POINTS 16384

typedef struct candle{
    double t;
    double open;
    double close;
    double low;
    double high;
}candle;


typedef struct candlestick{
    candle* candles;
    int32_t current_candle_idx;
    int32_t candles_count;
    int32_t resolution;
    candle  displayed_candles[MAX_DISPLAYABLE_POINTS];
    int32_t displayed_candles_count;
    double y_min_displayed_candle;
    double y_max_displayed_candle; 
    double x_min_displayed_candle;
    double x_max_displayed_candle; 

}candlestick;

candlestick* candlestick_init(int32_t candles_count);
candlestick* candlestick_add_candle(candlestick* p, double t, double op, double cl, double lo, double hi);

void candlestick_set_nb_candles_to_display(candlestick* p, int32_t nb_candles_to_display);
void candlestick_update(void* self);

#endif // PLOT_H