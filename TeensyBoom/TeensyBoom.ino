#include <Synth-Clatter.h>
#include <Synth-Decay.h>
#include <Synth-DrumHeart.h>
#include <synth_simple_drum.h>
#include <TeensyBoomVoiceLibrary.h>

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include <usb_dev.h>

#include "panel-scanner.h"
#include "editor.h"
#include "pattern.h"
#include "player.h"
#include "voice.h"


// Appp construct singletons
PanelScanner theScanner;
Editor       theEditor;
Pattern      thePattern; // TBD: multidimensional...
Player       thePlayer;


void setup() 
{
  // put your setup code here, to run once:
  usb_init();

  delay(1500);

  dbgln("Setup");

  pinMode(15, INPUT); // Volume pot pin?

  // SD CARD & general SPI inits
  // Initialize the SD card
  SPI.setMOSI(7);
  SPI.setSCK(14);

  // This sets the chip selects for the panel
  // to OFF.  Gotta do it before SD init, else the panels contend!
  theScanner.initScanning();


  if (!(SD.begin(10)))
  {
    dbgln("Unable to access the SD card");
  }
  else
  {
    dbgln("SD card begin worked");
  }

  if (SD.exists("test.txt"))
  {
    dbgln("found test.txt file");
  }
  else
  {
    dbgln("Didn't find test file?");
  }

  theEditor.setMode(Editor::eMODE_PATT_SEL);

  // audio library init
  AudioMemory(20);

  voiceInit();

  delay(500);

  dbgln("Setup Complete");
}

void loop()
{
  uint32_t now = millis();
  static uint32_t then;

  // put your main code here, to run repeatedly:

  paramUpdate1();// kik,snr,hat
  paramUpdate2();// toms, shaker
  paramUpdate3();// master volume & tempo

  if (now > then)
  {
    thePlayer.tick();
  }

  if (now % 5 == 0)
  {
    theScanner.tick();
  }

  if (now % 5000 == 0)
  {
    //theScanner.dumpLEDs();

    dbgln("Diagnostics:");
    fprintf(stderr, " max: %f, buffs: %d\n", AudioProcessorUsageMax(), AudioMemoryUsageMax());
    AudioProcessorUsageMaxReset();
  }

  then = now;

	usbMIDI.read();
}
