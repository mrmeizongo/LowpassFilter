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
#ifndef FILTER_H
#define FILTER_H
#include <Arduino.h>

#define CUTOFFFREQUENCY 10 // Default cutoff frequency for the filter

// Abstract class for filters. All filters must implement the Process function
template <typename T>
class Filter
{
public:
    virtual T Process(T, float) = 0;
    virtual ~Filter() = default;
};

// Computationally less expensive than second order filter but less effective, has slower roll-off and limited flexibility
// See https://en.wikipedia.org/wiki/Low-pass_filter#RC_filter
template <typename T>
class FirstOrderLPF : public Filter<T>
{
public:
    FirstOrderLPF(uint16_t cutoffFrequency = CUTOFFFREQUENCY)
    {
        rc = 1.0f / (2.0f * M_PI * cutoffFrequency);
        prevOutput = T{};
    }

    // Filter input signal to remove unwanted high frequency noise
    T Process(T input, float samplingFrequency) override
    {
        // Calculate alpha based on the cutoff and sampling frequencies
        prevOutput = static_cast<T>(prevOutput + ((samplingFrequency / (rc + samplingFrequency)) * (input - prevOutput)));
        return prevOutput;
    }

private:
    T prevOutput; // Previous output value
    float rc;
};

// More computationally expensive than first order filter but more effective, has faster roll-off and more flexibility
// See https://en.wikipedia.org/wiki/Butterworth_filter#Normalized_Butterworth_polynomials
template <typename T>
class SecondOrderLPF : public Filter<T>
{
public:
    SecondOrderLPF(uint16_t cutoffFrequency = CUTOFFFREQUENCY, T _prevInput1 = T{}, T _prevInput2 = T{})
        : cutoffFrequency(cutoffFrequency), prevInput1(_prevInput1), prevInput2(_prevInput2), prevOutput1(T{}), prevOutput2(T{}) {}

    void CalculateCoEfficients(float samplingFrequency)
    {
        // Calculate normalized cutoff frequency
        float omega = 1 / (2.0f * M_PI * cutoffFrequency * samplingFrequency);
        float sinOmega = sin(omega);
        float cosOmega = cos(omega);

        // Compute Butterworth coefficients
        float alpha = sinOmega / M_SQRT2;
        float scale = 1.0f / (1.0f + alpha);

        b0 = (1.0f - cosOmega) / (2.0f * scale);
        b1 = (1.0f - cosOmega) * scale;
        b2 = b0;
        a1 = -2.0f * cosOmega * scale;
        a2 = (1.0f - alpha) * scale;
    }

    // Filter input signal to remove unwanted high frequency noise
    T Process(T input, float samplingFrequency) override
    {
        CalculateCoEfficients(samplingFrequency);
        T output = static_cast<T>((b0 * input) + (b1 * prevInput1) + (b2 * prevInput2) - (a1 * prevOutput1) - (a2 * prevOutput2));

        // Update previous values
        prevInput2 = prevInput1;
        prevInput1 = input;
        prevOutput2 = prevOutput1;
        prevOutput1 = output;

        return output;
    }

private:
    uint16_t cutoffFrequency;
    float a1, a2, b0, b1, b2;                           // Filter coefficients
    T prevInput1, prevInput2, prevOutput1, prevOutput2; // Previous input and output values
};
#endif // FILTER_H