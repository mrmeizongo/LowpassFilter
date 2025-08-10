#include <Arduino>
#include <LowpassFilter.h>

#define CUTOFFFREQUENCY 20.0f
#define SAMPLERATEHZ 50
#define SIGNALINPUTLENGTH 10

LowPassFilter<float> lpf{CUTOFFFREQUENCY, FilterType::SECOND_ORDER};
void setup()
{
    Serial.begin(9600);
    while (!Serial)
        ;

    float inputSignal[SIGNALINPUTLENGTH] = {10.0f, 20.0f, 30.0f, 40.0f, 50.0f, 60.0f, 70.0f, 80.0f, 90.0f, 100.0f};
    Serial.print("Filtered output: ");
    for (int i = 0; i < SIGNALINPUTLENGTH; i++)
    {
        float filteredOutput = lpf.Process(inputSignal[i], SAMPLERATEHZ * 0.001f);
        Serial.print(filteredOutput);
        Serial.print(" ");
    }
    Serial.println();
}

void loop()
{
    Delay(10);
}