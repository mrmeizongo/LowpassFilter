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

    ~LowPassFilter()
    {
        delete lpf;
    }

    // Copy constructor
    LowPassFilter(const LowPassFilter &other)
        : filterType(other.filterType), lpf(other.lpf) {}

    // Move constructor
    LowPassFilter(LowPassFilter &&other)
    {
        filterType = other.filterType;
        lpf = other.lpf;
        other.lpf = nullptr; // Prevent double deletion
    }

    // Copy assignment operator.
    // std::swap is not available in Arduino, so we use a simple assignment
    // Since argument is passed by value, it will create a copy of the object using the copy constructor
    // Destructor of the copy object will clean up the old filter when it goes out of scope
    LowPassFilter &operator=(LowPassFilter other)
    {
        if (this != &other)
        {
            delete lpf; // Clean up existing filter
            filterType = other.filterType;
            lpf = other.lpf;
        }
        return *this;
    }

    T Process(T input, float samplingFrequency)
    {
        if (lpf != nullptr)
            return lpf->Process(input, samplingFrequency);

        return T{}; // Return default value if filter is not initialized
    }

    FilterType getFilterType() { return filterType; }

private:
    Filter<T> *lpf = nullptr; // Pointer to the filter instance
    FilterType filterType;
};

#endif // LOWPASSFILTER_H