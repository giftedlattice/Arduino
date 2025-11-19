const int piezoPin = 9;   // Piezo connected directly to pin 9 + GND

// List of frequencies to play (Hz)
int freqs[] = {
  500, 1000, 2000, 3000, 4000, 
  5000, 6000, 8000, 10000, 12000, 15000
};

const int numFreqs = sizeof(freqs) / sizeof(freqs[0]);

void setup() {}

void loop() {
  for (int i = 0; i < numFreqs; i++) {
    
    tone(piezoPin, freqs[i]);  // Play this frequency
    delay(300);                // Tone duration (ms)
    
    noTone(piezoPin);          // Stop tone
    delay(150);                // Pause between tones
  }

  delay(1000);                 // Wait before repeating sequence
}
