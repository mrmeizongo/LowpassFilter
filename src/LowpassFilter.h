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

template <typename T, template <typename> class FilterType>
class LowPassFilter
{
    Filter<T, FilterType> *lpf = nullptr; // Pointer to the filter instance
    uint16_t cutoffFrequency;

public:
    LowPassFilter(uint16_t _cutoffFrequency = CUTOFFFREQUENCY)
        : cutoffFrequency(_cutoffFrequency), lpf(new Filter<T, FilterType>(cutoffFrequency)) {}

    ~LowPassFilter()
    {
        delete lpf;
        cutoffFrequency = 0;
    }

    // Copy constructor
    LowPassFilter(const LowPassFilter &other)
        : cutoffFrequency(other.cutoffFrequency) lpf(new Filter<T, FilterType>(other.cutoffFrequency)) {}

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

    friend void swap(LowPassFilter &first, LowPassFilter &second) noexcept
    {
        Filter<T, FilterType> *temp = first.lpf;
        first.lpf = second.lpf;
        second.lpf = temp;

        uint16_t tempCutoff = first.cutoffFrequency;
        first.cutoffFrequency = second.cutoffFrequency;
        second.cutoffFrequency = tempCutoff;
    }
};

#endif // LOWPASSFILTER_H