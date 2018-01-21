// PLEASE DO NOT SUE ME MR. NYAN CAT MAN.

#define GRAYSCALE_AND_RAINBOW_ENABLED 1 // flag to disable or enable grayscale & rainbow at compiler time. Parts of screen will rapidly flicker black and white to simulate gray.

#include <Arduboy2.h>
#include <ArduboyTones.h>
#include "melody.h"
#include "kitty.h"

Arduboy2 arduboy;
ArduboyTones sound(arduboy.audio.enabled);

void setup()
{
  arduboy.begin();
  arduboy.audio.on();
  arduboy.setFrameRate(90);
}
// Ticker stuff leftover from the start. Still used, but really shouldn't be.
// I was in a rush ;)
#define frame(i) ((((i))/8)%12)

// convert a bit-pair to its full value
#define rainbowcolor(i,j) (0x7F*((rainbow[i] >> (j))&3))


// Sparkle starting positions.
// I resized the original nyan cat gif to 64 and used the positions of the sparkles.
// Then I doubled the number of sparkles and shifted them 64 pixels to the right
// because the arduboy width is 2x height.
int16_t sparklesX[] = { 4, 33, 43,  63, 31, 2,
                        64+4, 64+33, 64+43,  64+63, 64+31, 64+2};
char sparklesY[] = {1,  10, 20, 41, 51, 61, 1,  10, 20, 41, 51, 61};
// Starting frames for each sparkle.
char sparklesF[] = {1, 3, 1, 5, 5, 2, 1, 3, 1, 5, 5, 2};

// start the cat centered in the screen
char catX = 47;
char catY = 20;


void GoKittyGo (){  static uint16_t i = 0;
  // ANNOYING RAINBOW. As soon as I got the led rainbow to work
  // I disabled it until I recorded the video because it is grating.
#if GRAYSCALE_AND_RAINBOW_ENABLED
  arduboy.setRGBled(rainbowcolor(frame(i)/2,4),rainbowcolor(frame(i)/2,2),rainbowcolor(frame(i)/2,0));//2 0 0
#endif

  // clear the screen
  arduboy.clear();

 // move cat
 if (arduboy.pressed(LEFT_BUTTON))
    if (catX > 1)
      catX -= 1;
  if (arduboy.pressed(RIGHT_BUTTON))
    if (catX < 128-34)
      catX += 1;  

  if (arduboy.pressed(UP_BUTTON))
    if (catY > 1)
      catY -= 1;
  if (arduboy.pressed(DOWN_BUTTON))
    if (catY < 64-24)
      catY += 1;  

#if GRAYSCALE_AND_RAINBOW_ENABLED
  // draw "rainbow"
  for(char k=catX-6;k+16 > -16; k -= 16)
    arduboy.drawBitmap(k,catY + 1,rainbow_png_data + (16*3)*((frame(i)/5)%2) + (16*3*2*(i%2)),16,24,WHITE);
#endif  

  // draw sparkles
  for(char j=0;j<12;j++)
  {
    sparklesX[j] -= 1;/*-8*frame(i)));*/
    if (sparklesX[j] < -8)
      sparklesX[j] += 128;
    arduboy.drawBitmap(sparklesX[j],sparklesY[j]-3,sparkle + 8*((
      (frame(i)/2) + sparklesF[j]
      
    )%6),8,8,WHITE);
  }

  arduboy.drawBitmap(catX,catY,nyan_png_data + (34*3)*(frame(i)/2),34,24,WHITE);
  // draw cat
  arduboy.drawBitmap(catX,catY,nyan2_png_data + (34*3)*(frame(i)/2)
#if GRAYSCALE_AND_RAINBOW_ENABLED  
  + (34*3*6*(i%2))
#endif
  ,34,24,BLACK);
    
  // push to screen
  arduboy.display();

  // increase ticker
  i++;
  if (i == 192*10)
    i = 0;
}
void loop()
{
    if (!(arduboy.nextFrame()))
    return;
    sound.volumeMode(VOLUME_ALWAYS_HIGH);
    sound.tones(melody);
    while (sound.playing()) {
    GoKittyGo();
    }
}

