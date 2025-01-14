#ifndef LOWPASSFILTER_H
#define LOWPASSFILTER_H
#include "Filter.h"

enum class FilterType
{
    FIRST_ORDER,
    SECOND_ORDER
};

class LowPassFilter
{
public:
    // Use default filter of first order if no filter type is specified
    LowPassFilter(float cutoffFrequency, FilterType _filterType = FilterType::FIRST_ORDER)
    {
        filterType = _filterType;
        switch (filterType)
        {
        case FilterType::FIRST_ORDER:
            lpf = new FirstOrderLPF(cutoffFrequency);
            break;
        case FilterType::SECOND_ORDER:
            lpf = new SecondOrderLPF(cutoffFrequency);
            break;
        default:
            break;
        }
    }

    float Process(float input, float samplingFrequency)
    {
        return lpf->Process(input, samplingFrequency);
    }

    FilterType getFilterType() { return filterType; }

    ~LowPassFilter()
    {
        delete lpf;
    }

private:
    Filter *lpf;
    FilterType filterType;
};

#endif // LOWPASSFILTER_H