#include <Arduino>
#include <LowpassFilter.h>

static constexpr float CUTOFFFREQUENCY = 20.0f;
static constexpr float SAMPLERATE = 1.0; // in milliseconds i.e. 1ms = 1kHz sample rate
static constexpr uint8_t SIGNALINPUTLENGTH = 10;

// Filter can be of two types i.e., FirstOrderLPF or SecondOrderLPF
FirstOrderLPF<float> lpf{CUTOFFFREQUENCY};
void setup()
{
    Serial.begin(9600);
    while (!Serial)
        ;

    float inputSignal[SIGNALINPUTLENGTH] = {10.0f, 20.0f, 30.0f, 40.0f, 50.0f, 60.0f, 70.0f, 80.0f, 90.0f, 100.0f};
    Serial.print("Filtered output: ");
    for (uint8_t i = 0; i < SIGNALINPUTLENGTH; i++)
    {
        float dt = SAMPLERATE * 0.001f; // Convert milliseconds to seconds
        float filteredOutput = lpf.Process(inputSignal[i], dt);
        Serial.print(filteredOutput);
        Serial.print(" ");
    }
    Serial.println();
}

void loop()
{
    Delay(10);
}