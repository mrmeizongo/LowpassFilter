// Last update: 2021-06-29 by J. Meizongo
#ifndef FILTER_H
#define FILTER_H

#ifdef ARDUINO
#include <Arduino.h>
#else
#include <cmath>
#define M_PI 3.14159265358979323846f
#endif

#define SQRT2 1.4142135623730950488f

class Filter
{
public:
    virtual float Process(float, float) = 0;
    virtual ~Filter() = default;
};

class FirstOrderLPF : public Filter
{
public:
    FirstOrderLPF(float cutoffFrequency)
    {
        rc = 1.0f / (2.0f * M_PI * cutoffFrequency);
        prevOutput = 0.0f;
    }

    float Process(float input, float samplingFrequency)
    {
        // Calculate alpha based on the cuttoff frequency and sampling frequency
        prevOutput = prevOutput + ((samplingFrequency / (rc + samplingFrequency)) * (input - prevOutput));
        return prevOutput;
    }

private:
    float prevOutput; // Previous output value
    float rc;
};

class SecondOrderLPF : public Filter
{
public:
    SecondOrderLPF(float cutoffFrequency)
        : cutoffFrequency(cutoffFrequency), prevInput1(0.0f), prevInput2(0.0f), prevOutput1(0.0f), prevOutput2(0.0f) {}

    void CalculateCoEfficients(float samplingFrequency)
    {
        // Calculate normalized cutoff frequency
        float omega = 2.0f * M_PI * (cutoffFrequency * samplingFrequency);
        float sinOmega = sin(omega);
        float cosOmega = cos(omega);

        // Compute Butterworth coefficients
        float alpha = sinOmega / SQRT2;
        float scale = 1.0f / (1.0f + alpha);

        b0 = (1.0f - cosOmega) / (2.0f * scale);
        b1 = (1.0f - cosOmega) * scale;
        b2 = b0;
        a1 = -2.0f * cosOmega * scale;
        a2 = (1.0f - alpha) * scale;
    }

    float Process(float input, float samplingFrequency)
    {
        CalculateCoEfficients(samplingFrequency);
        float output = (b0 * input) + (b1 * prevInput1) + (b2 * prevInput2) - (a1 * prevOutput1) - (a2 * prevOutput2);

        // Update previous values
        prevInput2 = prevInput1;
        prevInput1 = input;
        prevOutput2 = prevOutput1;
        prevOutput1 = output;

        return output;
    }

private:
    float cutoffFrequency;
    float a1, a2, b0, b1, b2;                               // Filter coefficients
    float prevInput1, prevInput2, prevOutput1, prevOutput2; // Previous input and output values
};
#endif // FILTER_H