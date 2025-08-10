/*
MIT License

Copyright (c) 2025 Jamal Meizongo

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
 */
#ifndef LOWPASSFILTER_H
#define LOWPASSFILTER_H
#include "Filter.h"

enum class FilterType : uint8_t
{
    FIRST_ORDER = 0U,
    SECOND_ORDER
};

template <typename T>
class LowPassFilter
{
public:
    // Use default filter of first order if no filter type is specified
    LowPassFilter(uint16_t cutoffFrequency = CUTOFFFREQUENCY, FilterType _filterType = FilterType::FIRST_ORDER, T _prevInput1 = T{}, T _prevInput2 = T{})
    {
        filterType = _filterType;
        switch (filterType)
        {
        case FilterType::FIRST_ORDER:
            (void)_prevInput1;
            (void)_prevInput2;
            lpf = new FirstOrderLPF<T>(cutoffFrequency);
            break;
        case FilterType::SECOND_ORDER:
            lpf = new SecondOrderLPF<T>(cutoffFrequency, _prevInput1, _prevInput2);
            break;
        default:
            break;
        }
    }

    LowPassFilter(LowPassFilter &&other) noexcept
    {
        filterType = other.filterType;
        lpf = other.lpf;
        other.lpf = nullptr; // Prevent double deletion
    }

    LowPassFilter &operator=(LowPassFilter &&other) noexcept
    {
        if (this != &other)
        {
            delete lpf; // Clean up existing filter
            filterType = other.filterType;
            lpf = other.lpf;
            other.lpf = nullptr; // Prevent double deletion
        }
        return *this;
    }

    T Process(T input, float samplingFrequency)
    {
        if (lpf != nullptr)
            return lpf->Process<T>(input, samplingFrequency);
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