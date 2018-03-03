#include <Stepper.h>
#include <sd_l0.h>
#include <sd_l1.h>
#include <sd_l2.h>
#include <SimpleSDAudio.h>
#include <SimpleSDAudioDefs.h>
#include <SimpleSDAudio.h>
#include <IRremote.h> //
#include <IRremoteInt.h> //

#define BUTTON 7

IRsend irsend;  // Initialize ir sender
Stepper myStepper(64, 2, 5, 6, 8);  // Initialize servo

void setup()
{
  Serial.begin(9600);

  // Set up Button
  pinMode(BUTTON, INPUT);   // input for the button

  // Set up Music
  SdPlay.setSDCSPin(4);     // set sd card cs pin
  
  if (!SdPlay.init(SSDA_MODE_FULLRATE | SSDA_MODE_MONO | SSDA_MODE_AUTOWORKER))
  {
    //while(1); 
  }

  // music name file
  if(!SdPlay.setFile("music.wav "))
  {
    //while(1)
  }
  
}


// VARIABLES for ir codes
// Light Strip
  // 0xFF02FD     // Turn on/off light strip
  // 0xFFE01F     // Fade 7
  // 0xFF20DF     // Jump 3
  // 0xFFE817     // Quick
  // 0xFF708F     // Red

// Strobe Light
  // 0xFFA25D     // Turn on/off strobe light
  // 0xFF9867     // Seizure
  // 0xFF9067     // Music 3
  // 0xFF30CF     // Red

// Light Functions
void turnOnLightStrip(void)
{
  irsend.sendNEC(0xFF02FD, 32);
}

void turnOnStrobeLight(void)
{
  irsend.sendNEC(0xFFA25D, 32);
}


// VARIABLES for servo
// 64 steps/rev
int lightStripAngle = 0;
int strobeLightAngle = 10;


// VARIABLES for loop
bool buttonPressed = false;   // Represents whether the button was pressed or not
bool playing = false;         // Represents whether the music is playing or not
int pressCount = 0;           // Represents how long the button was pressed for


void loop(void)
{
  // Determine how long the button is being held for
  // Loop starts when the button is pressed and ends when it is released
  while (digitalRead(BUTTON) == HIGH )
  {
    buttonPressed = true;
    pressCount += 1;
    delay(100);
  }

  // Execute this code if the button was pressed
  // Makes sure pressCount is above 300, to prevent false starts
  if (buttonPressed && pressCount > 1)
  {
    // If the song is playing, stop it.
    // Otherwise, start up the music
    if (playing)
    {
      SdPlay.stop();    // Stop the music
      playing = false;  // Set playing to false
      
    } else {
      // Determine which song to play, based off of how long the button was held
      if (pressCount < 10){
        SdPlay.setFile("music.wav");
        turnOnLightStrip();
        myStepper.step(10);
        turnOnStrobeLight();
        
      }else if (pressCount > 10){
        SdPlay.setFile("music2.wav");
        turnOnLightStrip();
      }

      SdPlay.play();    // Start playing the music
      playing = true;   // Set playing to true
    }

    Serial.println(pressCount);   // Print how long the button was held for (debuging)
    
    pressCount = 0;         // Reset pressCount
    buttonPressed = false;  // Reset buttonPressed
  }

  //while(!SdPlay.isStopped()){ 
      //;
    //}
}
