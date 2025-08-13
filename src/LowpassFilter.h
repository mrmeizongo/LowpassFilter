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
    NONE = 0U,
    FIRST_ORDER,
    SECOND_ORDER
};

template <typename T>
class LowPassFilter
{
public:
    LowPassFilter(uint16_t _cutoffFrequency = CUTOFFFREQUENCY, FilterType _filterType = FilterType::FIRST_ORDER)
        : cutoffFrequency(_cutoffFrequency), filterType(_filterType)
    {
        switch (filterType)
        {
        case FilterType::NONE:
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
    }

    // Copy constructor
    LowPassFilter(const LowPassFilter &other)
        : cutoffFrequency(other.cutoffFrequency), filterType(other.filterType)
    {
        switch (filterType)
        {
        case FilterType::NONE:
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
        : cutoffFrequency(other.cutoffFrequency), filterType(other.filterType), lpf(other.lpf)
    {
        other.lpf = nullptr; // Prevent double deletion
        other.cutoffFrequency = 0;
        other.filterType = FilterType::NONE;
    }

    // Copy assignment operator.
    LowPassFilter &operator=(LowPassFilter &other)
    {
        if (this != &other)
        {
            delete lpf; // Clean up existing filter
            cutoffFrequency = other.cutoffFrequency;
            filterType = other.filterType;
            switch (filterType)
            {
            case FilterType::NONE:
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
        return *this;
    }

    // Move assignment operator.
    LowPassFilter &operator=(LowPassFilter &&other)
    {
        if (this != &other)
        {
            delete lpf; // Clean up existing filter
            cutoffFrequency = other.cutoffFrequency;
            filterType = other.filterType;
            lpf = other.lpf;     // Take ownership of the filter
            other.lpf = nullptr; // Prevent double deletion
            other.cutoffFrequency = 0;
            other.filterType = FilterType::NONE;
        }
        return *this;
    }

    T Process(T input, float samplingFrequency)
    {
        if (lpf != nullptr)
            return lpf->Process(input, samplingFrequency);

        return T{}; // Return default value if filter is not initialized
    }

    FilterType getFilterType() const { return filterType; }

private:
    Filter<T> *lpf = nullptr; // Pointer to the filter instance
    uint16_t cutoffFrequency;
    FilterType filterType;
};

#endif // LOWPASSFILTER_H