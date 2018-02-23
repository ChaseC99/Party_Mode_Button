#include <sd_l0.h>
#include <sd_l1.h>
#include <sd_l2.h>
#include <SimpleSDAudio.h>
#include <SimpleSDAudioDefs.h>
#include <SimpleSDAudio.h>

#include <IRremote.h> //
#include <IRremoteInt.h> //

#define BUTTON 7

IRsend irsend; //

void setup()
{
  Serial.begin(9600);
  pinMode(BUTTON, INPUT); // input for the button
  SdPlay.setSDCSPin(4); // sd card cs pin
  
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

// VARIABLES FOR loop
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
        irsend.sendNEC(0xFF02FD, 32);
      }else if (pressCount > 10){
        SdPlay.setFile("music2.wav");
        irsend.sendNEC(0xFF02FD, 32);
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
