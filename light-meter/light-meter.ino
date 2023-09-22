// Put the pin numbers of your LED's here in the order you want them to light up
#define LED_ORDERING {3,5,6,9,10,11}
// Analog input pin from your sensor
#define PIN_IN	A0
// Minimum PWM value you want to use (if you have issues at low values)
#define PWM_MIN	 0

void calcLED(
		int signalIn,
		int signalInMinBound,
		int signalInMaxBound,
		int pinOut[],
		int numPinOut) {
	// Find the "boundary" LED by dividing each LED into a range of values it covers. Since all pins are on/off except 1 which corresponds to the signal value, we identify the pin that will have a value then set the rest on/off
	int fullSignalRange = signalInMaxBound - signalInMinBound;
	int pinRange = fullSignalRange / numPinOut;
	int boundaryPin = (signalIn - signalInMinBound) / pinRange;

	// Set all previous pins to full on
	for (int pin = 0; pin < boundaryPin; ++pin) {
		pinOut[pin] = 255;
	}
	// Set boundary pin based on signal. map the value between [MIN,255]. First
	// slide minimum boundary to 0 to normalize it, then scale it up by
	// multiplying (oldRange/newRange), then un-normalize by adding new min
	// boundary
	double sigNorm = static_cast<double>(signalIn - signalInMinBound);
	double convert = static_cast<double>(255 - PWM_MIN) / static_cast<double>(signalInMaxBound - signalInMinBound);
	int sigNew = static_cast<int>(sigNorm * convert) + PWM_MIN;
	pinOut[boundaryPin] = sigNew;
	// Set all other pins to full off
	for (int pin = boundaryPin+1; pin < numPinOut; ++pin) {
		pinOut[pin] = 0;
	}
}

void setup() {
	int pinMapping[] = LED_ORDERING;
	int numPins = sizeof(pinMapping)/sizeof(*pinMapping);
	for (int pin = 0; pin < numPins; ++pin) {
		pinMode(pinMapping[pin], OUTPUT);
	}
}

void loop() {
	// Collect analog signal (10-bit so values between [0,1024])
	int signal = analogRead(PIN_IN);
	int pinMapping[] = LED_ORDERING;
	int numPins = sizeof(pinMapping)/sizeof(*pinMapping);
	int outputs[numPins];

	// Map to output
	calcLED(signal, 0, 1024, outputs, numPins);
	// Apply outputs to pins
	for (int pin = 0; pin < numPins; ++pin) {
		analogWrite(pinMapping[pin], outputs[pin]);
	}
}
