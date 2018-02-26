/***************************************************
  moon.ino based on the example sketch for the Adafruit 2.2" SPI display.
  This library works with the Adafruit 2.2" TFT Breakout w/SD card
  ----> http://www.adafruit.com/products/1480
****************************************************/

#include <Adafruit_GFX.h>    // Core graphics library
#include "Adafruit_ILI9340.h" // Hardware-specific library
#include <SPI.h>
#include <SD.h>

#include <ESP8266WiFi.h>

const char* ssid     = "nope";
const char* password = "noooope";

const char* host = "the_IP_of_the_machine_running_your_host_script";
const int hostPort = 1337;

#if defined(__SAM3X8E__)
    #undef __FlashStringHelper::F(string_literal)
    #define F(string_literal) string_literal
#endif

// TFT display and SD card will share the hardware SPI interface.
// Hardware SPI pins are specific to the Arduino board type and
// cannot be remapped to alternate pins.  For Arduino Uno,
// Duemilanove, etc., pin 11 = MOSI, pin 12 = MISO, pin 13 = SCK.
#define TFT_RST D8
#define TFT_DC D0
#define TFT_CS D3
#define SD_CS D1

#define TFT_BACKLIGHT D2

#define LDR_PIN A0
#define filter_alpha 15

Adafruit_ILI9340 tft = Adafruit_ILI9340(TFT_CS, TFT_DC, TFT_RST);

int ldr_val = 512;

int phase=10;
int lastPhase=10;
unsigned long lastGet = 0;
unsigned long loopDelay = 1000*60*60*5; //every 5 hours


//declarations:
void getIt();
void bmpDraw(char *filename, uint16_t x, uint16_t y);
uint16_t read16(File & f);
uint32_t read32(File & f);



void setup(void) {

  WiFi.persistent(false); //avoid weirdness


  Serial.begin(115200);

  pinMode(TFT_BACKLIGHT,OUTPUT);
  analogWrite(TFT_BACKLIGHT,1023);

  Serial.print("Initializing SD card...");
  if (!SD.begin(SD_CS)) {
    Serial.println("failed!");
    return;
  }
  Serial.println("OK!");

  tft.begin();
  tft.fillScreen(ILI9340_BLACK);

  Serial.println();


  bmpDraw("15.bmp", 0, 60);

  lastGet=millis();
  Serial.println("first round!");

  delay(10000); //wait ten seconds to let the server boot up (it is usually booted along with moon.)

  getIt();

}

void loop() {

	yield();

  ldr_val = (((long)ldr_val*filter_alpha)+analogRead(LDR_PIN))/(filter_alpha+1); //low pass
  //Serial.print("ldr_val: ");
  //Serial.println(ldr_val);
  analogWrite(TFT_BACKLIGHT,ldr_val);

  if(millis()>lastGet+loopDelay) {
  	Serial.println("next round!");
  	lastGet=millis();
  	getIt();
  	}

  if(phase!=lastPhase){

  	lastPhase=phase;

  	tft.fillScreen(ILI9340_BLACK);

  	switch(phase) {
  		case 0:
  			bmpDraw("01.bmp", 0, 60);
  			break;
  		case 1:
  			bmpDraw("02.bmp", 0, 60);
  			break;
  		case 2:
  			bmpDraw("03.bmp", 0, 60);
  			break;
  		case 3:
  			bmpDraw("04.bmp", 0, 60);
  			break;
  		case 4:
  			bmpDraw("05.bmp", 0, 60);
  			break;
  		case 5:
  			bmpDraw("06.bmp", 0, 60);
  			break;
  		case 6:
  			bmpDraw("07.bmp", 0, 60);
  			break;
  		case 7:
  			bmpDraw("08.bmp", 0, 60);
  			break;
  		case 8:
  			bmpDraw("09.bmp", 0, 60);
  			break;
  		case 9:
  			bmpDraw("10.bmp", 0, 60);
  			break;
  		case 10:
  			bmpDraw("11.bmp", 0, 60);
  			break;
  		case 11:
  			bmpDraw("12.bmp", 0, 60);
  			break;
  		case 12:
  			bmpDraw("13.bmp", 0, 60);
  			break;
  		case 13:
  			bmpDraw("14.bmp", 0, 60);
  			break;
  		case 14:
  			bmpDraw("15.bmp", 0, 60);
  			break;
  		case 15:
  			bmpDraw("16.bmp", 0, 60);
  			break;
  		case 16:
  			bmpDraw("17.bmp", 0, 60);
  			break;
  		case 17:
  			bmpDraw("18.bmp", 0, 60);
  			break;
  		case 18:
  			bmpDraw("19.bmp", 0, 60);
  			break;
  		case 19:
  			bmpDraw("20.bmp", 0, 60);
  			break;
  		case 20:
  			bmpDraw("21.bmp", 0, 60);
  			break;
  		case 21:
  			bmpDraw("22.bmp", 0, 60);
  			break;
  		case 22:
  			bmpDraw("23.bmp", 0, 60);
  			break;
  		case 23:
  			bmpDraw("24.bmp", 0, 60);
  			break;
  		case 24:
  			bmpDraw("25.bmp", 0, 60);
  			break;
  		case 25:
  			bmpDraw("26.bmp", 0, 60);
  			break;
  		case 26:
  			bmpDraw("27.bmp", 0, 60);
  			break;
  		case 27:
  			bmpDraw("28.bmp", 0, 60);
  			break;
  		default:
  			break;
  		}
	}
}


void getIt()
{
  int rawRX=15;
  int wifiTries=0;

  // Use WiFiClient class to create TCP connections
  WiFiClient client;


  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    wifiTries++;
    Serial.print(".");
    if(wifiTries==60) { //after 30 seconds of trying to connect.
    	Serial.println("WiFi connection failed.. will retry next round.");
    	WiFi.disconnect();
    	return;
    }
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  Serial.print("connecting to ");
  Serial.println(host);

  if (!client.connect(host, hostPort)) {
    Serial.println("connection failed");
    return;
  }

  delay(500);

  // Read all the lines of the reply from server and print them to Serial
  if(client.available()){
    rawRX=client.parseInt();
    Serial.print("Phase RX: ");
    Serial.println(rawRX);
    phase=rawRX;
  }

   Serial.println();
   Serial.println("disconnecting.");
   client.stop();

   WiFi.disconnect();

}




// This function opens a Windows Bitmap (BMP) file and
// displays it at the given coordinates.  It's sped up
// by reading many pixels worth of data at a time
// (rather than pixel by pixel).  Increasing the buffer
// size takes more of the Arduino's precious RAM but
// makes loading a little faster.  20 pixels seems a
// good balance.

#define BUFFPIXEL 20

void bmpDraw(char *filename, uint16_t x, uint16_t y) {

  File     bmpFile;
  int      bmpWidth, bmpHeight;   // W+H in pixels
  uint8_t  bmpDepth;              // Bit depth (currently must be 24)
  uint32_t bmpImageoffset;        // Start of image data in file
  uint32_t rowSize;               // Not always = bmpWidth; may have padding
  uint8_t  sdbuffer[3*BUFFPIXEL]; // pixel buffer (R+G+B per pixel)
  uint8_t  buffidx = sizeof(sdbuffer); // Current position in sdbuffer
  boolean  goodBmp = false;       // Set to true on valid header parse
  boolean  flip    = true;        // BMP is stored bottom-to-top
  int      w, h, row, col;
  uint8_t  r, g, b;
  uint32_t pos = 0, startTime = millis();

  if((x >= tft.width()) || (y >= tft.height())) return;

  Serial.println();
  Serial.print("Loading image '");
  Serial.print(filename);
  Serial.println('\'');

  // Open requested file on SD card
  if ((bmpFile = SD.open(filename)) == NULL) {
    Serial.print("File not found");
    return;
  }

  // Parse BMP header
  if(read16(bmpFile) == 0x4D42) { // BMP signature
    Serial.print("File size: "); Serial.println(read32(bmpFile));
    (void)read32(bmpFile); // Read & ignore creator bytes
    bmpImageoffset = read32(bmpFile); // Start of image data
    Serial.print("Image Offset: "); Serial.println(bmpImageoffset, DEC);
    // Read DIB header
    Serial.print("Header size: "); Serial.println(read32(bmpFile));
    bmpWidth  = read32(bmpFile);
    bmpHeight = read32(bmpFile);
    if(read16(bmpFile) == 1) { // # planes -- must be '1'
      bmpDepth = read16(bmpFile); // bits per pixel
      Serial.print("Bit Depth: "); Serial.println(bmpDepth);
      if((bmpDepth == 24) && (read32(bmpFile) == 0)) { // 0 = uncompressed

        goodBmp = true; // Supported BMP format -- proceed!
        Serial.print("Image size: ");
        Serial.print(bmpWidth);
        Serial.print('x');
        Serial.println(bmpHeight);

        // BMP rows are padded (if needed) to 4-byte boundary
        rowSize = (bmpWidth * 3 + 3) & ~3;

        // If bmpHeight is negative, image is in top-down order.
        // This is not canon but has been observed in the wild.
        if(bmpHeight < 0) {
          bmpHeight = -bmpHeight;
          flip      = false;
        }

        // Crop area to be loaded
        w = bmpWidth;
        h = bmpHeight;
        if((x+w-1) >= tft.width())  w = tft.width()  - x;
        if((y+h-1) >= tft.height()) h = tft.height() - y;

        // Set TFT address window to clipped image bounds
        tft.setAddrWindow(x, y, x+w-1, y+h-1);

        for (row=0; row<h; row++) { // For each scanline...

          // Seek to start of scan line.  It might seem labor-
          // intensive to be doing this on every line, but this
          // method covers a lot of gritty details like cropping
          // and scanline padding.  Also, the seek only takes
          // place if the file position actually needs to change
          // (avoids a lot of cluster math in SD library).
          if(flip) // Bitmap is stored bottom-to-top order (normal BMP)
            pos = bmpImageoffset + (bmpHeight - 1 - row) * rowSize;
          else     // Bitmap is stored top-to-bottom
            pos = bmpImageoffset + row * rowSize;
          if(bmpFile.position() != pos) { // Need seek?
            bmpFile.seek(pos);
            buffidx = sizeof(sdbuffer); // Force buffer reload
          }

          for (col=0; col<w; col++) { // For each pixel...
            // Time to read more pixel data?
            if (buffidx >= sizeof(sdbuffer)) { // Indeed
              bmpFile.read(sdbuffer, sizeof(sdbuffer));
              buffidx = 0; // Set index to beginning
            }

            // Convert pixel from BMP to TFT format, push to display
            b = sdbuffer[buffidx++];
            g = sdbuffer[buffidx++];
            r = sdbuffer[buffidx++];
            tft.pushColor(tft.Color565(r,g,b));
          } // end pixel
        } // end scanline
        Serial.print("Loaded in ");
        Serial.print(millis() - startTime);
        Serial.println(" ms");
      } // end goodBmp
    }
  }

  bmpFile.close();
  if(!goodBmp) Serial.println("BMP format not recognized.");
}

// These read 16- and 32-bit types from the SD card file.
// BMP data is stored little-endian, Arduino is little-endian too.
// May need to reverse subscript order if porting elsewhere.

uint16_t read16(File & f) {
  uint16_t result;
  ((uint8_t *)&result)[0] = f.read(); // LSB
  ((uint8_t *)&result)[1] = f.read(); // MSB
  return result;
}

uint32_t read32(File & f) {
  uint32_t result;
  ((uint8_t *)&result)[0] = f.read(); // LSB
  ((uint8_t *)&result)[1] = f.read();
  ((uint8_t *)&result)[2] = f.read();
  ((uint8_t *)&result)[3] = f.read(); // MSB
  return result;
}
