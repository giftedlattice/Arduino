const int piezoPin = 9;

// DOOM-inspired chiptune frequencies (Hz)
int riff[] = {
  880, 830, 740, 622,  // descending minor-metal feel
  0,
  988, 880, 830,        // punch riff (B5 → A5 → G#5)
  0,
  622, 588, 622, 740,   // tritone grind (D#5 → D5 → D#5 → F#5)
  0,
  830, 740, 622, 554,   // deeper descent
  0,
  988, 880, 830, 740,   // ending like a DOOM cadence
};

int duration[] = {
  120,120,150,180,
  80,
  120,120,150,
  80,
  100, 80, 100, 140,
  80,
  120,120,150,160,
  80,
  120,100,100,200
};

int numNotes = sizeof(riff)/sizeof(riff[0]);

void setup() {}

void loop() {
  for (int i = 0; i < numNotes; i++) {
    if (riff[i] == 0) {
      noTone(piezoPin);
    } else {
      tone(piezoPin, riff[i]);
    }

    delay(duration[i]);
    noTone(piezoPin);
    delay(25);  // tiny gap = punchy
  }

  delay(1500); // repeat like a theme
}
