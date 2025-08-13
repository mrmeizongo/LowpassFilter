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
    Filter<T> *lpf = nullptr; // Pointer to the filter instance
    uint16_t cutoffFrequency;
    FilterType filterType;

public:
    LowPassFilter(uint16_t _cutoffFrequency = CUTOFFFREQUENCY, FilterType _filterType = FilterType::FIRST_ORDER)
        : cutoffFrequency(_cutoffFrequency), filterType(_filterType)
    {
        switch (filterType)
        {
        case FilterType::FIRST_ORDER:
            lpf = new FirstOrderLPF<T>(cutoffFrequency);
            break;
        case FilterType::SECOND_ORDER:
            lpf = new SecondOrderLPF<T>(cutoffFrequency);
            break;
        default:
            break;
        }
    }

    ~LowPassFilter()
    {
        delete lpf;
        cutoffFrequency = 0;
        filterType = FilterType::FIRST_ORDER; // Reset to default
    }

    // Copy constructor
    LowPassFilter(const LowPassFilter &other)
        : cutoffFrequency(other.cutoffFrequency), filterType(other.filterType)
    {
        switch (filterType)
        {
        case FilterType::FIRST_ORDER:
            lpf = new FirstOrderLPF<T>(cutoffFrequency);
            break;
        case FilterType::SECOND_ORDER:
            lpf = new SecondOrderLPF<T>(cutoffFrequency);
            break;
        default:
            break;
        }
    }

    // Move constructor
    LowPassFilter(LowPassFilter &&other)
        : LowPassFilter() // Delegate to the default constructor
    {
        swap(*this, other); // Use the swap function to handle resource management
    }

    // Assignment operator
    LowPassFilter &operator=(LowPassFilter other) // copy by value
    {
        swap(*this, other); // Use the swap function to handle self-assignment and cleanup
        return *this;
    }

    // Filter input signal to remove unwanted high frequency noise
    T Process(T input, float samplingFrequency)
    {
        return lpf->Process(input, samplingFrequency);
    }

    FilterType getFilterType() const { return filterType; }

    friend void swap(LowPassFilter &first, LowPassFilter &second) noexcept
    {
        Filter<T> *temp = first.lpf;
        first.lpf = second.lpf;
        second.lpf = temp;

        uint16_t tempCutoff = first.cutoffFrequency;
        first.cutoffFrequency = second.cutoffFrequency;
        second.cutoffFrequency = tempCutoff;

        FilterType tempType = first.filterType;
        first.filterType = second.filterType;
        second.filterType = tempType;
    }
};

#endif // LOWPASSFILTER_H