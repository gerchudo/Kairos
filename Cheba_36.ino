//Hey guapo!
//Programmed in Barcelona, Spain


//LED COLUMN
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#include <EEPROM.h>


  ///////////  CHEBA 1802
  //LED COLUMN
  #define OUTPUTPIN   D7//
  #define NUM_LEDS    136
  //ENCODER
  #define ENC_PIN0    D0
  #define ENC_PIN1    D1
  #define ENC_PIN2    D2
  //Presence sensor
  #define ZEN_PIN     D3
  //Onboard led
  #define MOTHERLED_POWER    D5 //D8 NO SE PUEDE USAR CON LED PORQUE NO CARLA EL SOFT! o si?
  #define MOTHERLED_WIFI     D8
 
  //PCB CHEBA 1802    //5 arriba (rojo)    //6 verde centro    //8 azul?    //9 blanco


/*
//////////////////////////////////////////// CHEBA CUBO
  // CHEBA CUBO (pcb en hormigón)
  //LED COLUMN 
  #define OUTPUTPIN   D1
  #define NUM_LEDS    130
  //ENCODER
  #define ENC_PIN0    D6
  #define ENC_PIN1    D5
  #define ENC_PIN2    D2
  //Presence sensor
  #define ZEN_PIN     D4
  //Onboard led
  #define LED_PIN     D7
  #define MOTHERLED_POWER    D8
*/


boolean semaforo = false;
boolean flagStop = false;

//////////////////////////////////////////////////////////////////////////////// Wi-Fi
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

char ssid[32];
char password[64];
int jWifintentos = 0;

// AP mode password
const char WiFiAPPSK[] = "melange Cheba";
ESP8266WebServer server(80);

// WiFi Show IP colors
int stripColor [4] = {172, 92, 18, 0};
//Colores predefinidos
int pColorWhite[4] = {0, 0, 0, 180}; //Warm White
int pColorWifi[4] = {22, 20, 138, 0}; //WIFI Blue ORIGINAL 22 22 120

IPAddress ip;

boolean apMode = false;

const char* passphrase = "melange Cheba"; //////// Pass original //Scan net function
String st; //Scan net function
String content; //Wifi messages
int statusCode;
int ipCountLed = NUM_LEDS - 1;
//////////////////////////////////////////////////////////////////////////// FIN WIFI



// File System
#include <FS.h>



//Column
boolean power=true;
uint8_t currentPatternIndex = 0; // Index number of which pattern is current
int r; int g; int b;
int w1; int w2; int w3; int W4;
int brightnessStart = 30;
int brightness = brightnessStart;
int hideBackground=0;
byte neopix_gamma[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
  1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,  2,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,  5,  5,
  5,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9, 10, 10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
  17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25, 25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
  37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50, 51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
  69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89, 90, 92, 93, 95, 96, 98, 99, 101, 102, 104, 105, 107, 109, 110, 112, 114,
  115, 117, 119, 120, 122, 124, 126, 127, 129, 131, 133, 135, 137, 138, 140, 142, 144, 146, 148, 150, 152, 154, 156, 158, 160, 162, 164, 167, 169, 171, 173, 175,
  177, 180, 182, 184, 186, 189, 191, 193, 196, 198, 200, 203, 205, 208, 210, 213, 215, 218, 220, 223, 225, 228, 231, 233, 236, 239, 241, 244, 247, 249, 252, 255
};

Adafruit_NeoPixel column = Adafruit_NeoPixel(NUM_LEDS, OUTPUTPIN, NEO_RGBW + NEO_KHZ800);
//Adafruit_NeoPixel column = Adafruit_NeoPixel(NUM_LEDS, OUTPUTPIN, NEO_RGB + NEO_KHZ800);



unsigned long idleTime=0;






//Pointer
//When encMode==2 (pointer mode) it shine the pointer
int pointerCycle=0;
int pointerBright=0;
int pointerLimit=180;
int pointerMin=0;
int pointerMax=255;
boolean pointerGrowing=true;






//////////////////////////////////////////////////////////////////////////// ENCODER /
boolean encMotion;
int encMode=1;
int encButtonState;
int encButtonCount;
//volatile long brightness = 255; //Maximun bright until eeprom read
volatile int  encLastBrightness = 0;

volatile long encLineCenter = 0; volatile int encLastW_b = 0;
volatile long encLineLength = 0; volatile int encLastW_a = 0;
volatile long encPointer = 0; volatile int encLastPointer;

volatile long hue = 0; volatile int encLastR = 0;

volatile long encPush = 0; volatile int  encLastPush = 0;

volatile long encWRGB = 0; volatile int encLastWRGB = 0;

volatile long encZenTime = 0; volatile int encLastZenTime = 0;

int lastMSB = 0;
int lastLSB = 0;
boolean encIsPushing;
boolean encDrag = false;

int encClic = 0; //Para reconocer clic, doble clic
unsigned long encClicTime;
int encClicTimeOut=500;
int tocandoMargin=0;

///////////////////////////////////////// ZENTINELLA / 
int zenTime;
unsigned long zenLastMotion;
int zenDetected = 0;
boolean zenSleeping = false;
int zenPixelNumber = 0;

// dimLight function
boolean dimEffect = false;
int dimInitBright;
int dimEndBright;
float dimDurationUp = 320; //ms of the dimm effect
float dimDurationDown = 10000;
unsigned long dimEndTime;

boolean encZenConfigMode;

//////////////////////////////////////////////// Timer /
unsigned long previousMillisChange = 0; //Timmer 250ms
boolean ledPinStatus;

// EEPROM
boolean eepromScheduleUpdate = false;







//////////////////////////////////////////////////////////////////////////////// ENCODER INTERRUPTION
//////////////////////////////////////////////////////////////////////////////// INTERRUPCIONES POR ENCODER
void handleInterrupt() {
  encMotion = true;
  int MSB = digitalRead(ENC_PIN1); //MSB = most significant bit
  int LSB = digitalRead(ENC_PIN2); //LSB = least significant bit

  int encData = (MSB << 1) | LSB; //converting the 2 pin value to single number
  int sum;


   switch (encMode) {
    
    case 0:
            if (!encIsPushing)
            //////////////////////////////////////////////////////////////////////// CHANGES THE BRIGHTNESS
            { 
              sum  = (encLastBrightness << 2) | encData; //adding it to the previous encData value
              if (sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) brightness++;
              if (sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) brightness--;
              encLastBrightness = encData; //store this value for next time
              if (brightness > 255) brightness = 255;
              if (1 <= brightness<=255) power=true;
              if (brightness<=0) { brightness=0; power=false; }
            }
            else
            { 
            //encDrag=true;
            ////////////////////////////////////////////////////////////////////////// IF PUSHING, ADDS RGB
              int sum  = (encLastWRGB << 2) | encData; //adding it to the previous encData value
              if (sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) { encWRGB++; encPush++;}
              if (sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) { encWRGB--; encPush--;}
              encLastWRGB = encData; //store this value for next time
              if (encWRGB > 255) encWRGB = 255;
              if (encWRGB > brightness) brightness=encWRGB;
              if (encWRGB < 0) encWRGB = 0;
            }
            break;

    case 1: 
          if (!encIsPushing)
          //////////////////////////////////////////////////////////////////////////////White line position
          {

            sum  = (encLastW_b << 2) | encData; //adding it to the previous encData value
            if (sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) encLineCenter++;
            if (sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) encLineCenter--;
            encLastW_b = encData; //store this value for next time
            
            //Chequeos
            
            if (encLineCenter>NUM_LEDS/2) encLineCenter=NUM_LEDS/2; //El máximo de esta variable es la mitad del lenght pues renderiza para arriba y abajo
            if (encLineCenter > NUM_LEDS) encLineCenter = NUM_LEDS; //Tope variable del encoder encLineCenter

            //Comprueba si tocó
            int quedan=NUM_LEDS-encLineLength;
            if (encLineCenter>quedan) { tocandoMargin=1; encLineLength--; } //Toca arriba, baja el puntero
            if (encLineLength<encLineCenter) { tocandoMargin=2; encLineLength++; } //Toca abajo, sube el puntero
            
            if (encLineCenter<=0 ) {
              Serial.println ("Switching to encMode=2 (was on 1)");
                              encMode=2; //Al pasar de encMode 1 línea a encMode2= puntero
                              encPointer=encLineLength;
                            }
            
          }
          
          else
          
          { //Is pushing
            sum  = (encLastW_a << 2) | encData; //adding it to the previous encData value
            if (sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) {encLineLength++;encPush++;}
            if (sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) {encLineLength--;encPush--;}
            encLastW_a = encData; //store this value for next time
            if (encLineLength > NUM_LEDS-encLineCenter) encLineLength = NUM_LEDS-encLineCenter;
            if (encLineLength < encLineCenter) encLineLength = encLineCenter;
          }
        break;

    case 2: //Modo pointer
          sum  = (encLastPointer << 2) | encData; //adding it to the previous encData value
          if (sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) encPointer++;
          if (sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) encPointer--;
          encLastPointer = encData; //store this value for next time

          if (encPointer<0) encPointer=0;
          if (encPointer>NUM_LEDS) encPointer=NUM_LEDS;

          if (brightness<255)
            {
              Serial.print ("Subiendo brillo");
              brightness++;
              if (brightness>=255) brightness=255;
            }



          if (encIsPushing)
         
          {
            encLineCenter=3;
            encLineLength=encPointer;
            encMode=1;
            encDrag=true;
           // CAMBIARA EL TIPO DE PUNTERO POR INDEX
           
          }
            


                
        break;
    

    case 4:
          if (!encIsPushing)
          {
            sum  = (encLastR << 2) | encData; //adding it to the previous encData value
            if (sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) hue++;
            if (sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) hue--;
            encLastR = encData; //store this value for next time
            if (hue>511) hue = 511;//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
            if (hue<0) hue = 0;
            if (hideBackground!=0) { switchhideBackground(); Serial.print ("Switching hideBackground to 0 to show background");}
            if (encLineLength+encLineCenter>=NUM_LEDS)
              {
                encMode=1;
                Serial.println ("Skipping rainbow mode by being the whole column covered by the foreground.");
              }
          }
          else
          {
            sum  = (encLastPush << 2) | encData; //adding it to the previous encData value
            if (sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) encPush++;
            if (sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) encPush--;
            encLastPush = encData; //store this value for next time

            if (encPush > 4 || encPush < -4) {
                  encDrag = true;  //Serial.print ("YA NO APAGA");
                  //Serial.println ("encDrag=");Serial.println (encDrag);
                }
                

            
            //ZOOM
            //encDrag = true;
          }
          break;

      case 10: //Config mode
          if (!encIsPushing)
          {
            sum  = (encLastZenTime << 2) | encData; //adding it to the previous encData value
            if (sum == 0b1101) encZenTime++;
            if (sum == 0b1110) encZenTime--;
            encLastZenTime = encData; //store this value for next time
            if (encZenTime > 10) encZenTime = 10;
            if (encZenTime < 0) { encZenTime = 0; }
            encZenConfigMode = true;
          }
          else
          {
            
          //encDrag = true; //NOO Siempre que presiono, drageo
          }
        break;
/*
        //En todos los casos
        if (encIsPushing)
         
          {
            //Declara el Push/Drag    
                //Encoder push absolute position
                int sum  = (encLastPush << 2) | encData; //adding it to the previous encData value
                if (sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) {   encPush++;   }
                if (sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) {   encPush--;   }
                
                encLastPush = encData; //store this value for next time
            
            
                if (encPush > 4 || encPush < -4) {
                  encDrag = true;  //Serial.print ("YA NO APAGA");
                  Serial.println ("encDrag=");Serial.println (encDrag);
                }
    
          }
          
          
          */
          
         


            

        
    }

    


  flagStop = true;
  wasInteraction();
  eepromScheduleUpdate = true;

}


void setup() {
  Serial.begin(115200);
  delay(10);

  EEPROM.begin(512);
  eepromLoadSettings();

  //LED COLUMN
  column.setBrightness(brightness);
  column.begin();
  //column.show(); // Initialize all pixels to 'off'
  //1803-22 quitted for don't disturb on reset in config mode


  ///////////////////////////////////////////////////ENCODER//
  pinMode(ENC_PIN0, INPUT_PULLUP); //BUTTON
  pinMode(ENC_PIN1, INPUT); //Encoder 1 w/10K resistor pullup
  pinMode(ENC_PIN2, INPUT); //2

  digitalWrite(ENC_PIN1, HIGH); //turn pullup resistor on
  digitalWrite(ENC_PIN2, HIGH); //turn pullup resistor on

  attachInterrupt(ENC_PIN1, handleInterrupt, CHANGE);
  attachInterrupt(ENC_PIN2, handleInterrupt, CHANGE);

  pinMode(ZEN_PIN, INPUT);
  //////////////////////////////////////////////////////////


//Led #1 
  pinMode(MOTHERLED_POWER, OUTPUT);


///////////////////////////////////////////////////////////////////////////////FILE LOAD////
  SPIFFS.begin();
  {
    Dir dir = SPIFFS.openDir("/");
    while (dir.next()) {
      String fileName = dir.fileName();
      size_t fileSize = dir.fileSize();
      Serial.printf("FS File: %s, size: %s\n", fileName.c_str(), String(fileSize).c_str());
    }
    Serial.printf("\n");
  }
  //////////////////////////////////////////////////////////////////////////////////////////


  if (brightness <= 0) {
    brightness = brightnessStart;
  }
  power=true;
  
  render();//Sets Column light
  column.setBrightness(brightness);
  column.show();
  

  ////////////////////////////////////////////////// WIFI////
  Serial.printf("Connecting to WiFi. ApMode=", apMode);
  Serial.print(apMode);
  Serial.println("");
  loadWifiSettings();
  
  if (apMode)
  { Serial.println ("apMode=true");
    WiFi.mode(WIFI_AP);
    uint8_t mac[WL_MAC_ADDR_LENGTH];
    WiFi.softAPmacAddress(mac);
    String macID = String(mac[WL_MAC_ADDR_LENGTH - 2], HEX) + String(mac[WL_MAC_ADDR_LENGTH - 1], HEX);
    macID.toUpperCase();
    String AP_NameString = "ENCHINCHE LAB " + macID;
    char AP_NameChar[AP_NameString.length() + 1];
    memset(AP_NameChar, 0, AP_NameString.length() + 1);
    for (int i = 0; i < AP_NameString.length(); i++) AP_NameChar[i] = AP_NameString.charAt(i);
    WiFi.softAP(AP_NameChar, WiFiAPPSK);
    Serial.printf("Connect to Wi-Fi access point: %s\n", AP_NameChar);
    Serial.println("Open http://192.168.4.1 in your browser connected to the same Wi-Fi");
  }
    else
  {
    Serial.println ("conectar \t apMode=false");
    //WiFi.mode(WIFI_STA);
    WiFi.mode(WIFI_AP_STA);

    Serial.print("Connecting to ssid: "); Serial.print(ssid);
    Serial.print(", password: "); Serial.print(password); Serial.println("");

    if (String(WiFi.SSID()) != String(ssid)) {
      WiFi.begin(ssid, password);
    }

    while (WiFi.status() != WL_CONNECTED and semaforo == false)
    {
      delay(500);
      Serial.print(".");
      jWifintentos++;

      if (jWifintentos == 20) {
        apMode = true;
        Serial.println ("No se encontró red");
        Serial.println ("conectar \t apMode=true");
        doApMode();
        semaforo = true;
      }
    }

    if (semaforo == false) //conectó pues no tuvo que salir del bucle
    { //Prints connection welcome
      Serial.print("WiFi Status "); Serial.println(WiFi.status()); //Serial.println("");
      Serial.print("Connected. Open http://"); Serial.print(WiFi.localIP()); Serial.println(" in your browser");
    }
    semaforo = false; //Reinicializa

  }

  ///////////////////////////////////////////////////////////// END OF WIFI

  //Starts Cheba in rainbow
  //whiteOverRainbow(1,20,31);

  rainbow(0); //Welcome


  render();//Sets Column light
  column.setBrightness(brightness);
  column.show();


  encButtonState = digitalRead(ENC_PIN0); //Config Mode
  if (encButtonState == LOW)//Pushing
  {
    zenConfig();
  }



  /////////////////////////////////////////////////////////////// SOCKET FUNCTIONS

  server.on("/all", HTTP_GET, []() {
    sendAll();
    Serial.print ("/all\t");
    wasInteraction();
  });

  server.on("/ip", HTTP_GET, []() {
    showIp();
    Serial.print ("/ip\t");
    wasInteraction();
  });

  server.on("/ip", HTTP_POST, []() {
    showIp();
    Serial.print ("/ip\t");
    wasInteraction();
  });

  server.on("/networks", HTTP_GET, []() {
    showNetworks();
    Serial.print ("/networks\t");
    Serial.println ("");
    wasInteraction();
  });

  server.on("/setting", HTTP_POST, []() {
    Serial.println("/setting");
    String qsid = server.arg("ssid");
    String qpass = server.arg("pass");
    if (qsid.length() > 0 && qpass.length() > 0)
    { 
      //Graba nueva pass en eeprom
      Serial.println("Clearing eeprom");
      for (int i = 16; i < 112; ++i) { EEPROM.write(i, 0); } //All to 0
      Serial.println(qsid);
      Serial.print("");
      Serial.println(qpass);
      //Serial.println("");
      
      eepromClean(16, 112);
      Serial.println("Writing eeprom ssid:");
      for (int i = 0; i < qsid.length(); ++i)
      {
        EEPROM.write(i + 16, qsid[i]); //Saves value in eeprom
        Serial.print("Wrote: "); Serial.println(qsid[i]);
      }
      Serial.println("Writing eeprom pass:");
      for (int i = 0; i < qpass.length(); ++i)
      {
        EEPROM.write(48 + i, qpass[i]); //Saves value in eeprom
        Serial.print("Wrote: "); Serial.println(qpass[i]);
      }
      EEPROM.commit();
      content = "{\"Success\":\"saved to eeprom. Now trying to connect...\"}";
      conectar();
      statusCode = 200;
    }
    else {
      content = "{\"Error\":\"404 not found\"}";
      statusCode = 404;
      Serial.println("Sending 404");
    }
    server.send(statusCode, "application/json", content);
  });



/*
  server.on("/power", HTTP_GET, []() {
    wasInteraction();
    sendPower();
    Serial.print ("/power\t");
    //pause=false;
  });*/

  server.on("/power", HTTP_POST, []()
  {
    String value = server.arg("value");
    Serial.print ("HTTP_GET /power/"); Serial.print (value); Serial.print ("\t");
    
    //Serial.print ("Power ");
    
    Serial.println ("");
    setPower(value.toInt());
    sendPower();
    //pause=false;
  });


/*
      server.on("/brightness", HTTP_GET, []() {
      wasInteraction();
      Serial.print ("/brightness\t");
      sendBrightness();
      render();
      //pause=false;
    });
*/

    server.on("/brightness", HTTP_POST, []() {
      wasInteraction();
      String value = server.arg("value");
      //setBrightness(value.toInt());

      brightness=(value.toInt());
      if (brightness==0) power=0; else { power=1; column.setBrightness(brightness);}
      sendBrightness();

      render();
    });


    server.on("/hue", HTTP_POST, []() {
      wasInteraction();
      String value = server.arg("value");
      hue=(value.toInt());



      sendHue();

      render();
    });

    

  /*

    server.on("/solidColor", HTTP_GET, []() {
      wasInteraction();
      Serial.print ("/solidcolor\t");
      //sendSolidColor();
      //pause=false;
    });

    server.on("/solidColor", HTTP_POST, []() {
      String r = server.arg("r");
      String g = server.arg("g");
      String b = server.arg("b");
      wasInteraction();
      Serial.print("r"); Serial.print(r); Serial.print(",");
      Serial.print("g"); Serial.print(g); Serial.print(",");
      Serial.print("b"); Serial.print(b); Serial.print("\t");
      Serial.println("");
      setSolidColor(r.toInt(), g.toInt(), b.toInt());
      sendSolidColor();
      pause=false;
    });

    server.on("/pattern", HTTP_GET, []() {
      wasInteraction();
      sendPattern();
      Serial.print ("/pattern\t");
      pause=false;
    });

    server.on("/pattern", HTTP_POST, []() {
      wasInteraction();
      String value = server.arg("value");
      setPattern(value.toInt());
      sendPattern();
      pause=false;
    });

    server.on("/patternUp", HTTP_POST, []() {
      wasInteraction();
      adjustPattern(true);
      Serial.print ("/patternUp\t");
      sendPattern();
      pause=false;
    });

    server.on("/patternDown", HTTP_POST, []() {
      wasInteraction();
      adjustPattern(false);
      Serial.print ("/patternDown\t");
      sendPattern();
      pause=false;
    });





    /////////////////////////////////////////////////////////////////////////////////////////////////SLEEP
    server.on("/sleep", HTTP_GET, []() {
      wasInteraction();
      Serial.print ("/sleep\t");
    });

    server.on("/sleep", HTTP_POST, []() {
      String value = server.arg("value");
      //Serial.print ("HOLAHOLA\t");
      //Serial.print (value); Serial.print ("\t");
      //setPower(value.toInt());
      //sendPower();
      setSleep(value.toInt());
      wasInteraction();
    });





    server.on("/+", HTTP_GET, []() {
      wasInteraction();
      Serial.println ("Subiendo brillo");
      adjustBrightness(1);
      sendBrightness();
    });

    server.on("/-", HTTP_GET, []() {
      wasInteraction();
      Serial.println ("Bajando brillo");
      adjustBrightness(0);
      sendBrightness();
    });
  */





    

    
  server.on("/test", HTTP_GET, []() {

    Serial.println ("Prueba G");

    //rainbow();

    //loadWifiSettings();

    //int aux=currentPatternIndex+1;
    //setPattern (aux);

    server.send(200, "text/json", "test ok");
  });
  /*
    server.on("/brightnessUp", HTTP_POST, []() {
      wasInteraction();
      adjustBrightness(true);
      Serial.print ("/brightnessUp\t");
      sendBrightness();
    });

    server.on("/brightnessDown", HTTP_POST, []() {
      wasInteraction();
      adjustBrightness(false);
      Serial.print ("/brightnessDown\t");
      sendBrightness();
    });

    server.on("/palette", HTTP_GET, []() {
      wasInteraction();
      Serial.print ("/palette\t");
      //sendPalette();
    });
    /*
    server.on("/palette", HTTP_POST, []() {
      wasInteraction();
      String value = server.arg("value");
      setPalette(value.toInt());
      sendPalette();
    });*/

  server.on("/index.htm", HTTP_GET, []() {
    wasInteraction();
    Serial.print ("HOLAAAA");
  });

  server.serveStatic("/index.htm", SPIFFS, "/index.htm");
  server.serveStatic("/", SPIFFS, "/index.htm");
  server.serveStatic("/fonts", SPIFFS, "/fonts", "max-age=86400");
  server.serveStatic("/js", SPIFFS, "/js");
  server.serveStatic("/css", SPIFFS, "/css", "max-age=86400");
  server.serveStatic("/images", SPIFFS, "/images", "max-age=86400");
  server.serveStatic("/setup", SPIFFS, "/setup.htm");
  server.serveStatic("/setup.htm", SPIFFS, "/setup.htm");

  server.begin();
  Serial.println("HTTP server started");

  //autoPlayTimeout = millis() + (autoPlayDurationSeconds * 1000);


  if (apMode)
  {
    //pause=true;
    ipPrint(0);
  }

}//Setup




/*
  void handleInterruptPress() {

  Serial.println("changing mode");
  encMode++;

  }
*/

void loop() {


  //WIfi y toda la bola
  server.handleClient();

  /*
    colorWipe(column.Color(255, 255, 255, 255), 1); // White
    delay(10000);
    colorWipe(column.Color(0, 0, 0, 255), 1); // White
    delay(10000);
  */

  // Zentinella presense detection
  unsigned long currentMillis = millis();

  zenDetected = digitalRead(ZEN_PIN);
  if (zenDetected == HIGH) zenLastMotion = currentMillis;




  /////////////////////////////////////Timer c/250ms


  if (currentMillis - previousMillisChange >= 100) //TIMER
  {
    previousMillisChange = currentMillis;


    //  I Zentinella/
    int apa = zenLastMotion + zenTime;
    //    Serial.println ("");      Serial.print ("APA ");      Serial.print (apa);

    if (power && !encZenConfigMode)
    {
      if (currentMillis > apa) //Debe estar suspendida o suspenderse
      {
        if (zenSleeping == false) { //No estaba suspendida, mas ahora se suspende
          Serial.print ("Sleep, "); Serial.print ("currentMillis: "); Serial.print (currentMillis);
          Serial.print (", time to sleep  "); Serial.print (apa); Serial.println ("");

          //pause=false;
          //int espera; if (patternIndex!=9) espera= 2000; else espera=dimDurationDown; //Modifica el tiempo en el q se apaga, segun con solid o efecto
          dimLight(0, dimDurationDown);
        }
        zenSleeping = true;
      }
      else
      {
        if (zenSleeping)
        { //Was sleeping. Hi, Cheba wake up
          Serial.print ("Hi! Milis: "); Serial.print (currentMillis); Serial.print (", sleep will be@"); Serial.print (apa);
          Serial.print (" (if no more presence or interaction detected)\t "); Serial.println ("");
          dimLight(256, dimDurationUp);
          //despertandoEnPausa=true; //Activa la pausa para el final del ciclo del loop
        }
        zenSleeping = false;
      }
    }


    //  II LED Status/////////////////////////////////////////////////////////////////////////////////////////////////
    //if (ledPinStatus) {digitalWrite(MOTHERLED_POWER, LOW); ledPinStatus=false;} else{ digitalWrite(MOTHERLED_POWER, HIGH); ledPinStatus=true;}


    //Eeprom check
    if (eepromScheduleUpdate) eepromUpdate();
  }





  //Encoder clic
  encButtonState = digitalRead(ENC_PIN0);
  if (encButtonState==LOW) //Pushing
  {
    wasInteraction();
    encIsPushing = true;
//    if (encButtonCount == 0) encPush = 0;
    encButtonCount++;
    //Apaga luego de X ciclos presionado si no se movió el encoder
    if (encButtonCount==60000 && !encDrag)
    {
      if (brightness != 0) apaga();
    }

    if (power==false && encDrag==false) //Enciende
    {
      power = true;
      //encMode = 0;
      column.setBrightness(brightness);
      column.show();
      motherLedPower();
    }






  }
  else
  {

    if (encButtonCount > 1) //Acaba de soltarse el botón del encoder
    { //Cierra el ciclo
      encIsPushing=false;
      encPush=0;
      Serial.print ("Encoder pressed "); Serial.print (encButtonCount); Serial.print (" cycles");
      encZenConfigMode = false;
      if (encMode == 10) encMode = 0; //Si estaba en modo configuración, va a modo brillo
      encClic++;
      encClicTime = millis();

      if (encDrag) {encClic=0; Serial.print (" DRAG! cancela los clic");}

      encDrag=false;
   
      encMotion=true;
      encButtonCount=0;
      Serial.println ("");
    }
    
  }





  // dimEffect
  if (power == true && dimEffect == true) //hay q evitar q dimeffect este en true si no tiene q estarlo! puede interferir con otros efectos
  { //DIM EFFECT
    if (currentMillis < dimEndTime)
    { //Its dimmering
      int quedan = dimEndTime - currentMillis;
      float pasaron; float a; float b; float c;

      if ( dimInitBright > dimEndBright)
      {
        Serial.print ("BAJA ");
        pasaron = dimDurationDown - quedan;
        a = (pasaron / dimDurationDown);
        b = dimInitBright - dimEndBright; c = a * b;
        brightness = dimInitBright - int (c); //going down

        //Set in Column
        column.setBrightness(brightness);
        column.show();

      }
      else
      { Serial.print ("SUBE ");
        pasaron = dimDurationUp - quedan;
        a = (pasaron / dimDurationUp);

        b = dimEndBright - dimInitBright; c = a * b;
        brightness = dimInitBright + int (c); //going up
      }
      Serial.print ("dimInitBright="); Serial.print (dimInitBright);
      Serial.print ("\t dimEndBright=");  Serial.print (dimEndBright);
      Serial.print ("\ta="); Serial.print (a);
      Serial.print ("\tdif (b)=");  Serial.print (b);
      Serial.print ("\tfactor (c)=");  Serial.print (c);
      Serial.print ("\tbr=");  Serial.println (brightness);

      render();

      //Set in Column
      column.setBrightness(brightness);
      //column.show();


    }
    else
    { //closes the dimEffect

      if (dimEndBright == 0) { //Suspended
        zenSleeping = true;
        //Arregla porque el brillo queda seteado en columna en 1
        column.setBrightness(0);
        column.show();

        brightness = dimInitBright;
      }
      else
      {
        brightness = dimEndBright;
      }

      Serial.print ("End of dimLight");
      Serial.print ("dimEndBright"); Serial.print (dimEndBright);
      Serial.print ("brightness"); Serial.print (brightness);
      Serial.print ("power"); Serial.print (power);
      Serial.println ("");
      dimEffect = false;
    }
  } //dimEffect





  //Column print if encoded
  if (encMotion)
  {
    idleTime = millis();
   //int encPointerTamanio=2;
   
    motherLedPower();
    //If brightness is 0, puts the encoder in brightness mode to turn on when to up
    if ((power==0 || brightness==0) && encMode!=0) { encMode=0;  Serial.println("Switching to encMode=0"); }

    encPrint(); //prints in serial the info of encoder
    delay (20);

    //Pointer Mode Zentinella

        //Pointer
        
          if (encMode==2)
          {
            whitePointer();
          }
          
    if (!encZenConfigMode) render();    //shows the column
    ////////////////////////////////////////////////////////////

    

    //Configuration mode    
    if (encZenConfigMode) renderConfig();//Config mode
    

    //Impact in Column
    column.setBrightness(brightness);
    column.show();

    //Update Motherboard leds
    motherLedPower();

  }



                        
            /*   _<|>_                       
            / \                              
     __o__  \o/    o        __o__      __o__ 
    />  \    |    <|>      />  \      />  \  
  o/        / \   / \    o/           \o     
 <|         \o/   \o/   <|             v\    
  \\         |     |     \\             <\   
   _\o__</  / \   / \     _\o__</  _\o__*/   //Clics
// Encoder clic action configutation   
  if (encClic != 0)
  {
    if (millis() > encClicTime + encClicTimeOut)
    {
      Serial.println ("");  Serial.print ("Clic(s): ");  Serial.println (encClic);

      if (encClic==1 && !encDrag) { //1Clic
                          switch (encMode)
                          {
                            case 1: encMode=4; render(); break; //Modo line a rainbow
                            case 2: encMode=1; encLineCenter=encPointer; encLineLength=4; render(); Serial.print ("Modo pointer a Line"); break; 
                            case 4: encMode=1; break; //Modo rainbow a line
                            case 0: encMode=4; break; //Modo brillo a rainbow //////////////////////////////////////////////////////// antes era brillo a line (1)
                            
                            //default: encMode=1;//break;
                          }
                        }
      if (encClic==2 && !encDrag) { encMode=0; render();  } //Brightness
      if (encClic==3 && !encDrag) {
                                  if (encMode!=4)
                                    { 
                                      encMode=4;/*Hue*/
                                    } 
                                  else
                                    {
                                    switchhideBackground();
                                    encMode=1;
                                    } 
                                  }       
      if (encClic==4 && !encDrag) switchhideBackground();
      if (encClic==5 && !encDrag) showIp();
      if (encClic==6 && !encDrag) zenConfig();
      encClic=0;
      encDrag=false;
    } //Clic timeout
  }



  //PointerShine
  if (encMode==2)
  {
    if (pointerGrowing) pointerCycle++; else pointerCycle--;
    
    if (pointerCycle>pointerLimit) pointerGrowing=false; //Invierte el ciclo
    if (pointerCycle<=pointerMin){ pointerGrowing=true; }//Invierte el ciclo

    pointerBright=pointerMax*(float(pointerCycle)/float(pointerLimit));
    render();

    /*
    Serial.println ("POINTER> ");  
    Serial.print ("pointerBright"); Serial.print (pointerBright);
    Serial.print (" pointerMax"); Serial.print (pointerMax);
    Serial.print (" pointerLimit"); Serial.print (pointerLimit);
    Serial.print (" pointerCycle"); Serial.print (pointerCycle);
    */

    
  }






} //loop



////////////////////////////////////////////////////////////////////////////////////////////////////////////////Functions //Funciones//////////

void encPrint() //Prints the info of encoder
{

   
   

   
  Serial.print("P:"); Serial.print(power);
  Serial.print(", eM:"); Serial.print(encMode);
  Serial.print(", brightness: "); Serial.print(brightness);
  Serial.print(", encLineLength:"); Serial.print(encLineLength); if (encPointer)("(P)");
  Serial.print(", encLineCenter:"); Serial.print(encLineCenter);
  Serial.print(", encPointer:"); Serial.print(encPointer);
  Serial.print(", hue: "); Serial.print(hue);
  Serial.print(", encWRGB:"); Serial.print(encWRGB);
  
  Serial.print(", encIsPushing:"); Serial.print(encIsPushing);
  Serial.print(", encPush: "); Serial.print(encPush);
  Serial.print("\tZenT: "); Serial.println(encZenTime);

  if (tocandoMargin!=0)
   {
    if (tocandoMargin==1) Serial.print ("\ttoca arriba"); 
    if (tocandoMargin==2) Serial.print ("\ttoca abajo");
    tocandoMargin=0;
   }
   
  encMotion = false;
}


// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for (uint16_t i = 0; i < column.numPixels(); i++) {
    column.setPixelColor(i, c);
    column.show();
    delay(wait);
  }
}



void pulseWhite(uint8_t wait) {
  for (int j = 0; j < 256 ; j++) {
    for (uint16_t i = 0; i < column.numPixels(); i++) {
      column.setPixelColor(i, column.Color(0, 0, 0, neopix_gamma[j] ) );
    }
    delay(wait);
    column.show();
  }

  for (int j = 255; j >= 0 ; j--) {
    for (uint16_t i = 0; i < column.numPixels(); i++) {
      column.setPixelColor(i, column.Color(0, 0, 0, neopix_gamma[j] ) );
    }
    delay(wait);
    column.show();
  }
}


void rainbow(uint8_t wait) {
  uint16_t i, j;
  if (flagStop == false)
  {
    for (j = 0; j < 256; j++) {
      for (i = 0; i < column.numPixels(); i++) {
        column.setPixelColor(i, Wheel((i + j) & 255));
      }
      column.show();
      delay(wait);
    }
  }//flagStop
}



void mkRainbow(uint8_t pos) {
  uint16_t i;
  for (i = 0; i < column.numPixels(); i++) {
    column.setPixelColor(i, Wheel((i + pos) & 255));
  }
  //column.show();
}


void whiteLight(int wLCentro, int wLLength) { //Centro, length //No impact
  uint16_t i;

  int inicio=wLCentro-wLLength;
  int fin=wLCentro+wLLength;

  if (inicio<0) inicio=0;
  if (fin>255)fin=255;
  
  for (i = inicio; i < fin; i++) { column.setPixelColor(i, encWRGB, encWRGB, encWRGB, brightness);  }
  //column.show();
}


void whitePointer()
  {
    uint16_t i=encPointer;
    //int secundario=brightness
    column.setPixelColor(i-1, 0, 0, 0, brightness); //led#1
    column.setPixelColor(i, pointerBright, pointerBright, pointerBright, brightness); //led#2
    column.setPixelColor(i+1, 0, 0, 0, brightness); //led#3
    //column.show();
  }




void whiteLight2points(int a, int b) { //A, B //No impact
  int encWRGBbalance=10;
  uint16_t i;
  if (a > b)
  {
    int auxPos = a;
    a = b;
    b = auxPos;
  }
  for (i = a; i < b; i++) {
    if (encWRGB>encWRGBbalance)encWRGB-encWRGBbalance;
    column.setPixelColor(i, encWRGB, encWRGB, encWRGB, brightness);
  }
  //column.show();
}



// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return column.Color(255 - WheelPos * 3, 0, WheelPos * 3, 0);
  }
  if (WheelPos < 170) {
    WheelPos -= 85;
    return column.Color(0, WheelPos * 3, 255 - WheelPos * 3, 0);
  }
  WheelPos -= 170;
  return column.Color(WheelPos * 3, 255 - WheelPos * 3, 0, 0);
}






void whiteOverRainbow(uint8_t wait, uint8_t whiteSpeed, uint8_t whiteLength ) {

  if (whiteLength >= column.numPixels()) whiteLength = column.numPixels() - 1;

  int head = whiteLength - 1;
  int tail = 0;

  int loops = 3;
  int loopNum = 0;

  static unsigned long lastTime = 0;


  while (true) {
    for (int j = 0; j < 256; j++) {
      for (uint16_t i = 0; i < column.numPixels(); i++) {
        if ((i >= tail && i <= head) || (tail > head && i >= tail) || (tail > head && i <= head) ) {
          column.setPixelColor(i, column.Color(0, 0, 0, 255 ) );
        }
        else {
          column.setPixelColor(i, Wheel(((i * 256 / column.numPixels()) + j) & 255));
        }

      }

      if (millis() - lastTime > whiteSpeed) {
        head++;
        tail++;
        if (head == column.numPixels()) {
          loopNum++;
        }
        lastTime = millis();
      }

      if (loopNum == loops) return;

      head %= column.numPixels();
      tail %= column.numPixels();
      column.show();
      delay(wait);
    }
  }
}






void eepromLoadSettings() // EEPROM BASICS 0 A 16, WIFI 16 a 112
{
  Serial.println ("eepromLoadSettings()");
  Serial.println ("_____________________________________________");
  // 0 Brightness
  brightness = EEPROM.read(0);
  Serial.print ("brightness: "); Serial.println (brightness);

  // 1 POINTER/CENTER 2 LENGHT
  encLineCenter     = EEPROM.read(1); Serial.print ("encLineCenter: "); Serial.println (encLineCenter);
  encLineLength     = EEPROM.read(2); Serial.print ("encLineLength: "); Serial.println (encLineLength);
  encPointer = EEPROM.read(3); Serial.print ("encPointer: "); Serial.println (encPointer);

  // 4 Zentinella: Time to sleep
  int tempZenTime = EEPROM.read(4); encZenTime = tempZenTime;
  Serial.print ("zenTime"); Serial.println (tempZenTime);
  switch (tempZenTime) {
    case 0: zenTime = 88888888;  break;
    case 1: zenTime = 5 * 1000;  break;
    case 2: zenTime = 10 * 1000; break;
    case 3: zenTime = 15 * 1000; break;
    case 4: zenTime = 30 * 1000; break;
    case 5: zenTime = 60 * 1000; break;
    case 6: zenTime = 2 * 60 * 1000; break;
    case 7: zenTime = 3 * 6 * 1000;  break;
    case 8: zenTime = 5 * 6 * 1000;  break;
    case 9: zenTime = 10 * 6 * 1000; break;
    case 10: zenTime = 15 * 6 * 1000; break;
  }

  // 5 Rainbow position
  hue = EEPROM.read(5);
  Serial.print ("hue"); Serial.println (hue);

  // 6 Rainbow position
  encWRGB = EEPROM.read(6);
  Serial.print ("encWRGB"); Serial.println (encWRGB);

  Serial.println ("_____________________________________________");

  // 7 renderBackground
  hideBackground = EEPROM.read(7);
  Serial.print ("hideBackground"); Serial.println (hideBackground);


}


void eepromUpdate()
{ //Guarda el brillo en la eeprom
  EEPROM.write(0, brightness);
  EEPROM.write(1, encLineCenter);
  EEPROM.write(2, encLineLength);
  EEPROM.write(3, encPointer);
  EEPROM.write(4, encZenTime);
  switch (encZenTime) {
    case 0: zenTime = 88888888;  break;
    case 1: zenTime = 5 * 1000;  break;
    case 2: zenTime = 10 * 1000; break;
    case 3: zenTime = 15 * 1000; break;
    case 4: zenTime = 30 * 1000; break;
    case 5: zenTime = 60 * 1000; break;
    case 6: zenTime = 2 * 60 * 1000; break;
    case 7: zenTime = 3 * 6 * 1000;  break;
    case 8: zenTime = 5 * 6 * 1000;  break;
    case 9: zenTime = 10 * 6 * 1000; break;
    case 10: zenTime = 15 * 6 * 1000; break;
  }
  EEPROM.write(5, hue);
  EEPROM.write(6, encWRGB);

  EEPROM.write(7, hideBackground);

  EEPROM.commit();
  eepromScheduleUpdate = false;
}








void dimLight(int endBr, int dimDuration) //si 256, brillo q estaba
{
  Serial.print ("dimLight(endBr="); Serial.print (endBr);  Serial.print (")\t");
  if (endBr == 256) {
    dimEndBright = brightness;  //Fija dimEndBright
  } else {
    dimEndBright = endBr;
  }
  int aux;
  int brilloGuardado = dimInitBright;
  if (zenSleeping == true) dimInitBright = 0; else dimInitBright = brightness;
  if (dimEffect == true)
  {
    //Da vuelta el ciclo
    aux = dimEndBright;
    dimEndBright = brilloGuardado;
    dimInitBright = aux;
  }

  power = true;
  dimEffect = true;
  dimEndTime = millis() + dimDuration;
}



void wasInteraction()
{
  //Serial.print ("wasInteraction\t");
  unsigned long currentMillis = millis();
  zenLastMotion = currentMillis;
}



void mkPixel(int color, int numero)
{
  zenPixelNumber++;
  for (int j = 0; j < numero ; j++) {
    if (color==0) column.setPixelColor(zenPixelNumber, 0, 0, 0, brightness);
             else column.setPixelColor(zenPixelNumber, 0, brightness, 0, 0);
    zenPixelNumber++;
  }
}


void zenConfig ()
{
  Serial.println ("Zentinella - Set the time to turn off");
  //encDrag = true; ///////////////////////////////////////////////////////////////////////////////////////////////// 
  pulseWhite(0);
  encMode = 10; //Zen Config Mode
  renderConfig();
  column.setBrightness(64);

}



void loadWifiSettings()// EEPROM BASICS 0 A 16, WIFI 16 a 112
{

  Serial.println("loadWifiSettings()");

  for (int i = 16; i < 48; ++i) {
    ssid[i - 16] = char(EEPROM.read(i));
  }

  Serial.println("Reading from EEPROM ");
  Serial.print("ssid\t "); Serial.println(ssid);
  //Serial.println("");

  for (int i = 48; i < 112; ++i) {
    password[i - 48] = char(EEPROM.read(i));
  }

  Serial.print("pass\t "); Serial.println(password);
  Serial.println("");

}





void doApMode()
{
  loadWifiSettings();
  Serial.println("doApmode()");
  WiFi.mode(WIFI_AP);

  // Do a little work to get a unique-ish name. Append the
  // last two bytes of the MAC (HEX'd) to "Thing-":
  uint8_t mac[WL_MAC_ADDR_LENGTH];
  WiFi.softAPmacAddress(mac);
  String macID = String(mac[WL_MAC_ADDR_LENGTH - 2], HEX) +
                 String(mac[WL_MAC_ADDR_LENGTH - 1], HEX);
  macID.toUpperCase();
  String AP_NameString = "ENCHINCHE LAB " + macID;

  char AP_NameChar[AP_NameString.length() + 1];
  memset(AP_NameChar, 0, AP_NameString.length() + 1);

  for (int i = 0; i < AP_NameString.length(); i++)
    AP_NameChar[i] = AP_NameString.charAt(i);

  WiFi.softAP(AP_NameChar, WiFiAPPSK);

  //Serial.printf("Connect to Wi-Fi access point: %s\n", AP_NameChar);
  Serial.println("Open http://192.168.4.1 in your browser");
  Serial.println("ssid: ENCHINCHE LAB");
  Serial.println("key: melange Cheba");
  Serial.println("");
}


void scanNet(void) {
  //WiFi.mode(WIFI_STA); //////////// VA?
  //WiFi.disconnect();
  //delay(100);

  int n = WiFi.scanNetworks();
  Serial.println("scan done");
  if (n == 0)
    Serial.println("no networks found");
  else
  {
    Serial.print(n);
    Serial.println(" networks found");
    for (int i = 0; i < n; ++i)
    {
      // Print SSID and RSSI for each network found
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " " : "*");
      delay(10);
    }
  }
  Serial.println("");
  st = "<ol>";
  for (int i = 0; i < n; ++i)
  {
    // Print SSID and RSSI for each network found
    st += "<li>";
    st += WiFi.SSID(i);
    st += " (";
    st += WiFi.RSSI(i);
    st += ")";
    st += (WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " " : "*";
    st += "</li>";
  }
  st += "</ol>";
  delay(100);
  WiFi.softAP(ssid, passphrase, 6);
  Serial.println("softap");
  //launchWeb(1);
  Serial.println("over");
}




void eepromClean(int desde, int hasta)
{
  Serial.print ("eepromClean(");
  Serial.print (desde); Serial.print (",");
  Serial.print (hasta); Serial.println (")");;
  for (int i = desde; i < hasta; ++i) {
    EEPROM.write(i, 0);
  }
}



////////////////////////////////////////////////////////////////////////////////////////////////////IPPRINT

void ipPrint(int tempi)
{
  Serial.println ("ipPrint ");
  allStripColor (pColorWhite[0], pColorWhite[1], pColorWhite[2], pColorWhite[3]); //Base azul

  ip = WiFi.localIP(); Serial.println(ip); Serial.print ("ACAAAAA");

  IPAddress broadCast = WiFi.localIP();
  /////////////////////////////////////////////////////////// x . . .
  int c = broadCast[0] / 100;
  int d = (broadCast[0] - (c * 100)) / 10;
  int u = broadCast[0] - ((c * 100) + (d * 10));
  if (apMode) {
    c = 1;
    d = 9;
    u = 2;
  }
  printIpTern(c, d, u); //Imprime en el serial
  ipPrintTern(c, d, u); //Imprime en la tira
  ipSpace(6);
  /////////////////////////////////////////////////////////// . x . .
  c = broadCast[1] / 100;
  d = (broadCast[1] - (c * 100)) / 10;
  u = broadCast[1] - ((c * 100) + (d * 10));
  if (apMode) {
    c = 1;
    d = 6;
    u = 8;
  }
  printIpTern(c, d, u);
  ipPrintTern(c, d, u);
  ipSpace(6);
  /////////////////////////////////////////////////////////// . . x .
  c = broadCast[2] / 100;
  d = (broadCast[2] - (c * 100)) / 10;
  u = broadCast[2] - ((c * 100) + (d * 10));
  if (apMode) {
    c = 0;
    d = 0;
    u = 4;
  }
  printIpTern(c, d, u);
  ipPrintTern(c, d, u);
  ipSpace(6);
  /////////////////////////////////////////////////////////// . . . x
  c = broadCast[3] / 100;
  d = (broadCast[3] - (c * 100)) / 10;
  u = broadCast[3] - ((c * 100) + (d * 10));
  if (apMode) {
    c = 0;
    d = 0;
    u = 1;
  }
  printIpTern(c, d, u);
  ipPrintTern(c, d, u);
  ipSpace(5);
  /////////////////////////////////////////////////////////////////////strip.show();

  //Reset led's position
  ipCountLed = NUM_LEDS - 1;
  column.show();//FastLED.show();
  //fastLedPause(tempi); //Va a estar pausado en el bucle
  /*if (tempi>0)
    {
        quitPauseIn = tempi + millis();
        pause=true;
    }*/

}




void ipPrintTern(int c, int d, int u)
{
  boolean noHuboCentena = false;
  boolean noHuboDecenaNiCentena = false;

  //Imprime centena
  if (c == 0)
  {
    noHuboCentena = true;
  }
  else
  {
    for (int i = 0; i < c; i++)
    { /*Setea el entero*/
      ipPrintSerPixel(ipCountLed, 1); ipCountLed--;
    }

    ipPrintSerPixel(ipCountLed, 0); ipCountLed--; //ipSpace
  }

  if (d == 0)
  {
    if (noHuboCentena == true)
    {
      noHuboDecenaNiCentena = true;
      //Como no empezó la terna, no va ipSpace
    }
    else
    {
      noHuboDecenaNiCentena = false;
      ipPrintSerPixel(ipCountLed, 2); //Pone un "cero" de color
      ipCountLed--; /*pongo0 sino no se va a entendder el 100*/

    }
  }
  else
  { //Imprime los enteros
    /*Pone ipSpace antes (si era 0 el anterior no lo ponía*/
    ipPrintSerPixel(ipCountLed, 0); ipCountLed--; //ipSpace
    for (int i = 0; i < d; i++)
    {
      ipPrintSerPixel(ipCountLed, 1);
      ipCountLed--;
    }
    /*Pone ipSpace*/
    ipPrintSerPixel(ipCountLed, 0); ipCountLed--; //ipSpace
  }
  if (u == 0)
  {
    ipPrintSerPixel(ipCountLed, 2);
    ipCountLed--;
  }
  else
  {
    /*Pone ipSpace antes (si era 0 el anterior no lo ponía*/
    ipPrintSerPixel(ipCountLed, 0); ipCountLed--;
    //Imprime los enteros
    for (int i = 0; i < u; i++)
    {
      ipPrintSerPixel(ipCountLed, 1);
      ipCountLed--;
    }
    ipPrintSerPixel(ipCountLed, 0); ipCountLed--; //ipSpace
    /*NO Pone ipSpaceipPrintSerPixel(ipCountLed,0); ipCountLed--;*/
  }
}



void ipSpace(int cantipSpaces)
{
  for (int i = 0; i < cantipSpaces; i++)
  {
    ipPrintSerPixel(ipCountLed, 0);
    ipCountLed--;
  }
}


void ipPrintSerPixel(int queLed, int estado) //0apagado 1 encendido 2 separador
//Enciende los pixeles según 3 estados para la IP. Ajuste de color de la IP -No impacta!
{
  if (estado == 0) {
    column.setPixelColor(queLed, 0, 0, 0, 0);
  }
  if (estado == 1) {
    column.setPixelColor(queLed, pColorWifi[0], pColorWifi[1], pColorWifi[2], 0);  //Entero
  }
  if (estado == 2) {
    column.setPixelColor(queLed, pColorWhite[0], pColorWhite[1], pColorWhite[2], 0);  //Cero
  }
}




void sendAll()
{
  //pause=false;
  //int gg = timeToSleep / 1000; //time to sleep

  String json = "{";

  json += "\"power\":"      + String(power) + ",";
  json += "\"sleep\":"      + String(zenTime) + ",";
  json += "\"brightness\":" + String(brightness) + ",";
  json += "\"hue\":"        + String(hue) + ",";


  
  /*
    json += "\"currentPattern\":{";
    json += "\"index\":" + String(currentPatternIndex);
    json += ",\"name\":\"" + patterns[currentPatternIndex].name + "\"}";

    json += ",\"currentPalette\":{";
    json += "\"index\":" + String(currentPaletteIndex);
    json += ",\"name\":\"" + paletteNames[currentPaletteIndex] + "\"}";

    json += ",\"solidColor\":{";
    json += "\"r\":" + String(solidColor.r);
    json += ",\"g\":" + String(solidColor.g);
    json += ",\"b\":" + String(solidColor.b);
    json += "}";

    json += ",\"patterns\":[";/*
    for (uint8_t i = 0; i < patternCount; i++)
    {
      json += "\"" + patterns[i].name + "\"";
      if (i < patternCount - 1)
        json += ",";
    }
    json += "]";

    json += ",\"palettes\":[";

    for (uint8_t i = 0; i < paletteCount; i++)
    {
      json += "\"" + paletteNames[i] + "\"";
      if (i < paletteCount - 1)
        json += ",";
    }*/

  json += "]";

  json += "}";

  server.send(200, "text/json", json);
  json = String();
}



void printIpTern(int c, int d, int u)
{
  //Shows the ip on the Serial
  Serial.print ("c"); Serial.print (c); Serial.print ("\t");
  Serial.print ("d"); Serial.print (d); Serial.print ("\t");
  Serial.print ("u"); Serial.print (u); Serial.println ("\t");
}


void allStripColor(int rr, int gg, int bb, int ww )
{
  for (int i = 0; i < NUM_LEDS + 1; i++) {
    //leds[i] = CRGB( rr, gg, bb);
    column.setPixelColor(i, rr, gg, bb, ww);
  } column.show(); //FastLED.show();
}


void sendBrightness()
{
  String json = String(brightness);
  server.send(200, "text/json", json);
  json = String();
}



void sendHue()
{
  String json = String(hue);
  server.send(200, "text/json", json);
  json = String();
}



void conectar(){
  Serial.println ("conectar \t apMode=false");
  WiFi.mode(WIFI_STA);
  Serial.print("Connecting to ssid: ");
  delay (900);//////////////////////////////////////////////////////////////////////////////////////////////////////
  Serial.print(ssid);
  Serial.print(", password: ");
  Serial.print(password);
  Serial.println("");

  if (String(WiFi.SSID()) != String(ssid)) {
    WiFi.begin(ssid, password);
  }

  while (WiFi.status() != WL_CONNECTED and semaforo == false) {
    delay(500);
    Serial.print(".");
    jWifintentos++;

    if (jWifintentos == 20) {
      apMode = true;
      Serial.println ("No se encontró red");
      doApMode();
      semaforo = true;
    }
  }
  semaforo = false; //Reinicializa

  Serial.print("WIFI STATUS"); Serial.print(WiFi.status()); Serial.println("");
  Serial.print("Connected! Open http://");
  Serial.print(WiFi.localIP());
  Serial.println(" in your browser");

}


void showNetworks() {
  server.send(200, "text/json", "ok");
  scanNet();
  Serial.print ("showNetworks()\t");
}

void showIp() {///////////////////////////
  Serial.print ("showIp\t");
  ipPrint(10000);
  Serial.println ("show_ip");
  server.send(200, "text/json", "ok");
}

void sendPower()//////////////////////////
{
  Serial.println ("sendPower()");
  String json = String(power);
  server.send(200, "text/json", json);
  json = String();
}

void setPower(uint8_t value)//////////////
{
  Serial.println ("setPower");
  power = value == 0 ? 0 : 1;
  //power = value == false ? false : true;
  render();
  motherLedPower(); //Onboard led power
  if (power == 1) {
    Serial.println ("Now is on");
  }
}




void motherLedPower() ////////////////////////////////////////////////////
{ //Serial.print ("motherLedPower(");
  if (power==true){ digitalWrite(MOTHERLED_POWER, LOW); /*Serial.print ("on");*/ }
    else 
      { digitalWrite (MOTHERLED_POWER, HIGH); /*Serial.print ("off");*/ }
  //Serial.println (")");
}


void render() /////////////////////////////////////////////////////////

{
  //Serial.println ("render");
  column.clear();
  if (power)
  {
    if (encMode==0 || encMode==1 || encMode==4) //modo brillo o line o rainbow
    {
      if (hideBackground==0) mkRainbow(hue); //Imprime Background en matriz
      //if (encMode==1)
      whiteLight(encLineLength, encLineCenter); // Imprime line
    }

    if (encMode==2) //Modo pointer
    {
      if (hideBackground==0) mkRainbow(hue); //Imprime Background en matriz
      whitePointer();
    }
    
    //Chequeo, si está en modo rainbow sin bacground, pasa a otro encMode
    if (hideBackground==1) //Sin background
      {
      if (encMode==4) {encMode=0; Serial.println("Switching to encMode=0 for not Background mode active");}
      }
  }
  
  column.show();
      
}



void switchhideBackground() //////////////////////////////////////

{
  hideBackground++;
  if (hideBackground>1) {
                          hideBackground=0;
                          if (encMode==4) {encMode=0; Serial.print ("switching to encMode=1 (was in 4) "); }
                        }
  Serial.print ("hideBackground: ");  Serial.println (hideBackground);
  render();
   
}  


void apaga()
{
  Serial.println("APAGA");
  encDrag=true;//////////////////////////////////////////////////////////////////////////////
  power=false;
        
  column.setBrightness(0);
  column.show();   
}

void renderConfig()
{
      //column.setBrightness(255);
      column.clear();  // Initialize all pixels to 'off'
      //mkRainbow(hue);
      //Serial.println ("encZenConfigMode > mkPixel");
      //Prints the 
      if (encZenTime >= 1) mkPixel (0, 5);  // 5"
      if (encZenTime >= 2) mkPixel (0, 10); // 10"
      if (encZenTime >= 3) mkPixel (0, 15); // 15"
      if (encZenTime >= 4) mkPixel (0, 30); // 30"
      if (encZenTime >= 5) mkPixel (1, 1);  // 1'
      if (encZenTime >= 6) mkPixel (1, 2);  // 2'
      if (encZenTime >= 7) mkPixel (1, 3);  // 3'
      if (encZenTime >= 8) mkPixel (1, 5);  // 5'
      if (encZenTime >= 9) mkPixel (1, 10); // 10'
      if (encZenTime >= 10)mkPixel (1, 15); // 15'
      column.show();

      zenPixelNumber = 0;
    }

void switchEncMode (int modo)
{

  if (encPointer)    {   encMode=2;   render();    }
}
        

    
    
