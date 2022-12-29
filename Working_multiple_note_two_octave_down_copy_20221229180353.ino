#include <MIDI.h>

MIDI_CREATE_DEFAULT_INSTANCE();

#define MAX_NOTES 120
bool loweredNoteOn[MAX_NOTES];

void setup() {
  // Initialize MIDI communications
  MIDI.begin(MIDI_CHANNEL_OMNI);

  for (int i = 0; i < MAX_NOTES;i++) 
  {
    loweredNoteOn[i] = false;
  } 
}

void loop() {
  // Check for incoming MIDI messages
  MIDI.read();
  

  // If a MIDI note on message is received
  if (MIDI.getType() == midi::NoteOn) {
    // Get the note number and velocity
    int note = MIDI.getData1();
    int velocity = MIDI.getData2();

    // Calculate the note two octaves lower
    int loweredNote = note - 24;

    // Send a note off message for the original note
    MIDI.sendNoteOff(note, 0, MIDI.getChannel());

    // Only send the lowered note if the original note is above a certain value
    if (note > 24 && loweredNoteOn[loweredNote] == false) {
      MIDI.sendNoteOn(loweredNote, velocity, MIDI.getChannel());
      loweredNoteOn[loweredNote] = true;
    }
  } else if (MIDI.getType() == midi::NoteOff) {
    // Get the note number
    int note = MIDI.getData1();

    // Calculate the note two octaves lower
    int loweredNote = note - 24;

    // Only send the lowered note off message if the original note is above a certain value
    if (note > 24 && loweredNoteOn[loweredNote] == true) {
      MIDI.sendNoteOff(loweredNote, 0, MIDI.getChannel());
      loweredNoteOn[loweredNote] = false;
    }
  }
}