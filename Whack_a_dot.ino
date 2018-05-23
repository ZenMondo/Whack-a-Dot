
/*  Whack-a-Dot
 *  By L. Christopher Bird <zenmondo@gmail.com>
 *  
 *  A demonstration program for the 
 *  Iduino 320x240 Colour TFT-LCD Display Shield with Touch for Arduino
 *  
 *  The screen I have uses a ST7781 LCD driver, Other versions may use an ILI9341
 *  
 *  Uses the following libraries:
 *  
 *  https://github.com/yamsam/TFTLCD_ST7781
 *  https://github.com/adafruit/Adafruit_TouchScreen
 *  
 *  Based on an example by William Tavares 
 *  https://github.com/williamtavares
 *  
 * MIT license, all text above must be included in any redistribution 
 *  
 */


#include "TFTLCD.h"
#include <TouchScreen.h>

#define LCD_CS A3    
#define LCD_CD A2    
#define LCD_WR A1   
#define LCD_RD A0    
// you can also just connect RESET to the arduino RESET pin
#define LCD_RESET A4
//Duemilanove/Diecimila/UNO/etc ('168 and '328 chips) microcontoller:
// Color definitions
#define  BLACK           0x0000
#define BLUE            0x001F
#define RED             0xF800
#define GREEN           0x07E0
#define CYAN            0x07FF
#define MAGENTA         0xF81F
#define YELLOW          0xFFE0 
#define WHITE           0xFFFF

#define YP A3  // must be an analog pin, use "An" notation!
#define XM A2  // must be an analog pin, use "An" notation!
#define YM 9   // can be a digital pin
#define XP 8   // can be a digital pin

// calibration mins and max for raw data when touching edges of screen
// YOU CAN USE THIS SKETCH TO DETERMINE THE RAW X AND Y OF THE EDGES TO GET YOUR HIGHS AND LOWS FOR X AND Y
#define TS_MINX 210
#define TS_MINY 210
#define TS_MAXX 915
#define TS_MAXY 910


#define MINPRESSURE 1
#define MAXPRESSURE 1000






TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);
// For better pressure precision, we need to know the resistance
// between X+ and X- Use any multimeter to read it
// For the one we're using, its 300 ohms across the X plate
// Pins A2-A6
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 364);


//Container variables for touch coordinates
int X, Y, Z;

int dotX;
int dotY;

int first_touch;

int score = 0;

void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);
  Serial.println("Whack-a-Dot Begin!");
  
  first_touch = true;
  tft.reset();
  tft.initDisplay();
  tft.setRotation(0);
  titlescreen();
  

}

void loop() {
  // put your main code here, to run repeatedly:

  
  retrieveTouch();
 

  if (Z > MINPRESSURE && Z < MAXPRESSURE) {

      Serial.println("touchX: " + (String)X + " touchY: " + (String) Y);
 
      if(first_touch){
        first_touch = false;
        randomSeed(X^Y^Z); //where the touchscreen is touched is our entropy source
        drawDot();
      }
      
      if(X >= dotX && Y <= dotY && X <= dotX+50 && dotY <= Y+50){
      
        drawDot();
        score++;
        tft.setCursor(0, 300);
        tft.setTextColor(GREEN);
        tft.setTextSize(3);
        tft.println("Score: " + (String) score);
          
    
       }
  }
}

void titlescreen(){
  tft.fillScreen(BLACK);
  tft.setCursor(0, 20);
  tft.setTextColor(GREEN);
  tft.setTextSize(3);
  tft.println("Whack-a-Dot!");
  tft.setTextSize(2);
  tft.println("by");
  tft.println("L. Christopher Bird");
  tft.setTextSize(1);
  tft.println("Tap Screen to play");
}

/* retrieveTouch() By William Tavares
 *  slight modification by L. Christopher Bird
 */
void retrieveTouch()
{
  digitalWrite(13, HIGH);
  TSPoint p = ts.getPoint();
  digitalWrite(13, LOW);

  //If sharing pins, you'll need to fix the directions of the touchscreen pins
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);




  //Scale from 0->1023 to tft.width
    X = map(p.x, TS_MAXX, TS_MINX, 0, tft.width());
    //Y = map(p.y, TS_MAXY, TS_MINY, 0, tft.height());

  // on my tft the numbers are reversed so this is used instead of the above
  //X = tft.width() - map(p.x, TS_MAXX, TS_MINX, 0, tft.width());
  Y = tft.height() - map(p.y, TS_MAXY, TS_MINY, 0, tft.height());
  Z = p.z;
}

void drawDot(){
    tft.fillScreen(BLACK);
    
    dotX = random(100,200);
    dotY = random(100,250);

    Serial.println("dotx: " + (String)dotX + " dotY: " + (String)dotY);

    int color;
    int x = random(1,7);

    switch(x){
      case 1:
        color = BLUE;
        break;
      case 2:
        color = RED;
        break;
      case 3:
        color = GREEN;
        break;
      case 4:
        color = CYAN;
        break;
      case 5:
        color = MAGENTA;
        break;
      case 6:
        color = YELLOW;
        break;
      case 7:
        color = WHITE;
        break;
        
    }

    
    tft.fillCircle(dotX, dotY, 40, color);
  
}

