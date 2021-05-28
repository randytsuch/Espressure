//#include <WiFi.h>
//#include <WiFiClient.h>
//#include <WebServer.h>
//#include <ESPmDNS.h>
//#include <Update.h>
#include "FS.h"
#include "SD.h"
#include "SPI.h"
#include "SPIFFS.h"
//#include <Wire.h>    //for I2C  
#include <LiquidCrystal.h>  //For LCD 
//#include "Credentials.h" //wifi info 
#include "user.h"

const char* host = "esp32 expobar";
//WebServer server(80);

// these "contributed" libraries must be installed in your sketchbook's arduino/libraries folder
//#include <TypeK.h>
//#include <cADC.h>

#define BANNER_BRBN "Java 32 01"
#define BAUD 115200
// LCD output strings
char smin[3],ssec[3],st1[6],st2[6],smilli[6];

// ---------------------------------- LCD interface definition
#ifdef I2C_LCD
#define BACKLIGHT lcd.backlight();
cLCD lcd; // I2C LCD interface
#else // equivalent to standard LiquidCrystal interface
#define BACKLIGHT_PIN 10
#define RS_PIN 13
#define ENABLE_PIN 12
#define D4_PIN 14
#define D5_PIN 27
#define D6_PIN 33
#define D7_PIN 32
LiquidCrystal lcd( RS_PIN, ENABLE_PIN, D4_PIN, D5_PIN, D6_PIN, D7_PIN ); // standard 4-bit parallel interface
#endif

#define NOBUTTON 0       // set to this value if no valid switch push detected
#define BUTTON_1 1       // set to this value if Button 1 was pushed
#define BUTTON_2 2       // set to this value if Button 2 was pushed
#define BUTTON_3 3       // set to this value if Button 3 was pushed
#define BUTTON_4 4       // set to this value if Button 4 was pushed

//define digital inputs to read push buttons
#define PUSH01_PIN 3
#define PUSH02_PIN 5
#define PUSH03_PIN 6
#define PUSH04_PIN 11


//boolean ota = true;  //set to true if starting up in Over The Air update mode
//boolean ota = false;  //set to true if starting up in Over The Air update mode
boolean config_info = false;  //set to true for configuration information update mode
//int otaEnPin = 36;  //gpio pin to read at startup to decide if OTA mode


#define FORMAT_SPIFFS_IF_FAILED true

/*
// --------------------------------------------------------------
// variables and constants for reading temperature
#define BT_FILTER 10 // filtering level (percent) for displayed BT
#define ET_FILTER 10 // filtering level (percent) for displayed ET

// needed for usesr without calibration values stored in EEPROM
#define CAL_GAIN 1.00 // substitute known gain adjustment from calibration
#define UV_OFFSET 0 // subsitute known value for uV offset in ADC
#define AMB_OFFSET 0.0 // substitute known value for amb temp offset (Celsius)

// ambient sensor should be stable, so quick variations are probably noise -- filter heavily
#define AMB_FILTER 70 // 70% filtering on ambient sensor readings

// other compile directives
#define MIN_DELAY 270   // ms between ADC samples (tested OK at 270)
#define NCHAN 2   // number of TC input channels
#define TC_TYPE TypeK  // thermocouple type / library
#define DP 1  // decimal places for output on serial port
#define D_MULT 0.001 // multiplier to convert temperatures from int to float

// class objects
cADC adc( A_ADC ); // MCP3424
ambSensor amb( A_AMB ); // MCP9800
filterRC fT[NCHAN]; // filter for displayed/logged ET, BT
filterRC fRise[NCHAN]; // heavily filtered for calculating RoR
filterRC fRoR[NCHAN]; // post-filtering on RoR values
int32_t temps[NCHAN]; //  stored temperatures are divided by D_MULT
int32_t ftemps[NCHAN]; // heavily filtered temps
int32_t ftimes[NCHAN]; // filtered sample timestamps
int32_t flast[NCHAN]; // for calculating derivative
int32_t lasttimes[NCHAN]; // for calculating derivative
//-----------------------------------------------------------------
*/

// ********************  Global Variables
float timestamp = 0;
//boolean first;
uint32_t nextLoop;
float start_shot = 0;
float shot_time = 0;
boolean shot_flag = false;
boolean back_flag = true;
boolean button_pushed = false;
char ShotFilearray[SHOT_ROW_SIZE][SHOT_PARAM_SIZE];   //array [row][column]
// address of row i is *(ShotFilearray + i)
// value of row i, col j is  *( *(arr + i) + j)

char ReadRow[SHOT_PARAM_SIZE] = {0};
//Max pwr, min pwr, max flow, min flow, max pres, min pres, pulse pwr, pulse flow, max shot time, spare 10, spare 11, spare 12, spare 13, spare 14, 
int MaxPwr;
int MinPwr;
int MaxFlow;
int MinFlow;

//index,time pre, pwr pre, pres pre, flow pre, Rup time, shot time, shot pwr, shot flow, shot pres, Rdn time, spare12, spare13, spare14, spare15, prof nane
int ShotIndex;
int ShotTimePre;
int ShotPwrPre;
int ShotPresPre;
int ShotFlowPre;
int ShotRupTime;
int ShotTime;
int ShotPwr;
int ShotFlow;



//----------------------------------------------------------------

//void updateLCD( float t1, float t2);

// ------------------------------------------------------------------
/*
void logger()      //write temps to serial port and to LCD
{
  int i;
  float t1,t2, t_amb;
  float rx;

  // print timestamp from when samples were taken
  Serial.print( timestamp, DP );

  // print ambient
//  Serial.print(",");
  t_amb = amb.getAmbF();
  Serial.print( t_amb, DP );
   
  // print temperature, rate for each channel
  i = 0;
  if( NCHAN >= 1 ) {
    Serial.print(",");
    Serial.print( t1 = D_MULT*temps[i], DP );
    i++;
  };
  
  if( NCHAN >= 2 ) {
    Serial.print(",");
    Serial.print( t2 = D_MULT * temps[i], DP );
    i++;
  };
  
  Serial.print(",");
  Serial.println(millis());
// log the placeholder to serial port
//  Serial.println("");
  
  updateLCD( t1, t2 );  
};
*/
// --------------------------------------------
void updateLCD( float t1, float t2) {
  // form the timer output string in min:sec format
  int itod = round( timestamp );
  if( itod > 3599 ) itod = 3599;
  sprintf( smin, "%02u", itod / 60 );
  sprintf( ssec, "%02u", itod % 60 );
  //start of bottom row
  lcd.setCursor(0,1);
  lcd.print( smin );
  lcd.print( ":" );
  lcd.print( ssec );

  sprintf( smilli, "%06u", millis() );
  lcd.setCursor(4,0); //top row, 4 chars over
  lcd.print( smilli );

if (shot_flag == true) {
   shot_time = timestamp - start_shot;
   }
else
   { shot_time = 0 ;}   
  int ishot = round( shot_time );
  sprintf( ssec, "%02u", ishot % 60 );
  //start of top row
  lcd.setCursor(0,0);
  lcd.print( ssec );

 
  // channel 1 temperature
  int it01 = round( t1 );
  if( it01 > 999 ) 
    it01 = 999;
  else
    if( it01 < -999 ) it01 = -999;
  sprintf( st1, "%3d", it01 );
  lcd.setCursor( 11, 0 );  //right side of top row
  lcd.print("B:");//0
  lcd.print(st1);//1, 2,3 and 4 for a space

  // channel 2 temperature 
  int it02 = round( t2 );
  if( it02 > 999 ) it02 = 999;
  else if( it02 < -999 ) it02 = -999;
  sprintf( st2, "%3d", it02 );
  lcd.setCursor( 11, 1 );  //right side of bottom row
  lcd.print( "G:" ); //5
  lcd.print( st2 ); //6, 7, 8, and 9 for a space 
}
/*
// --------------------------------------------------------------------------
void get_samples() // this function talks to the amb sensor and ADC via I2C
{
  int32_t v;
  TC_TYPE tc;
  float tempC;
  
  for( int j = 0; j < NCHAN; j++ ) { // one-shot conversions on both chips
    adc.nextConversion( j ); // start ADC conversion on channel j
    amb.nextConversion(); // start ambient sensor conversion
    delay( MIN_DELAY ); // give the chips time to perform the conversions
    ftimes[j] = millis(); // record timestamp for RoR calculations
    amb.readSensor(); // retrieve value from ambient temp register
    v = adc.readuV(); // retrieve microvolt sample from MCP3424
    tempC = tc.Temp_C( 0.001 * v, amb.getAmbC() ); // convert to Celsius
    v = round( C_TO_F( tempC ) / D_MULT ); // store results as integers
    temps[j] = fT[j].doFilter( v ); // apply digital filtering for display/logging
//    ftemps[j] =fRise[j].doFilter( v ); // heavier filtering for RoR
  }
};
*/

//**********************Webserver OTA pages **********************

/*
 * Login page
 

const char* loginIndex =
 "<form name='loginForm'>"
    "<table width='20%' bgcolor='A09F9F' align='center'>"
        "<tr>"
            "<td colspan=2>"
                "<center><font size=4><b>ESP32 Login Page</b></font></center>"
                "<br>"
            "</td>"
            "<br>"
            "<br>"
        "</tr>"
        "<tr>"
             "<td>Username:</td>"
             "<td><input type='text' size=25 name='userid'><br></td>"
        "</tr>"
        "<br>"
        "<br>"
        "<tr>"
            "<td>Password:</td>"
            "<td><input type='Password' size=25 name='pwd'><br></td>"
            "<br>"
            "<br>"
        "</tr>"
        "<tr>"
            "<td><input type='submit' onclick='check(this.form)' value='Login'></td>"
        "</tr>"
    "</table>"
"</form>"
"<script>"
    "function check(form)"
    "{"
    "if(form.userid.value=='admin' && form.pwd.value=='admin')"
    "{"
    "window.open('/serverIndex')"
    "}"
    "else"
    "{"
    " alert('Error Password or Username')/*displays error message"
    "}"
    "}"
"</script>";


// Server Index Page
 

const char* serverIndex =
"<script src='https://ajax.googleapis.com/ajax/libs/jquery/3.2.1/jquery.min.js'></script>"
"<form method='POST' action='#' enctype='multipart/form-data' id='upload_form'>"
   "<input type='file' name='update'>"
        "<input type='submit' value='Update'>"
    "</form>"
 "<div id='prg'>progress: 0%</div>"
 "<script>"
  "$('form').submit(function(e){"
  "e.preventDefault();"
  "var form = $('#upload_form')[0];"
  "var data = new FormData(form);"
  " $.ajax({"
  "url: '/update',"
  "type: 'POST',"
  "data: data,"
  "contentType: false,"
  "processData:false,"
  "xhr: function() {"
  "var xhr = new window.XMLHttpRequest();"
  "xhr.upload.addEventListener('progress', function(evt) {"
  "if (evt.lengthComputable) {"
  "var per = evt.loaded / evt.total;"
  "$('#prg').html('progress: ' + Math.round(per*100) + '%');"
  "}"
  "}, false);"
  "return xhr;"
  "},"
  "success:function(d, s) {"
  "console.log('success!')"
 "},"
 "error: function (a, b, c) {"
 "}"
 "});"
 "});"
 "</script>";
*/
  
// **************************setup starts here********************* 
void setup() {
  //this first part runs for any mode (OTA, Configuration update or normal)
  delay(100);

//setup lcd  
//  lcd.begin(16, 2);
//  BACKLIGHT;
//  lcd.setCursor( 0, 0 );
//  lcd.print( BANNER_BRBN ); // display version banner
//for temperature logic  
//  amb.init( AMB_FILTER );  // initialize ambient temp filtering
//start serial port
  Serial.begin(BAUD);
  Serial.println("Booting");
  delay(500);

 // Connect to WiFi network
 /*
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  //use mdns for host name resolution
  if (!MDNS.begin(host)) { //http://esp32.local
    Serial.println("Error setting up MDNS responder!");
    while (1) {
      delay(1000);
    }
  }
  Serial.println("mDNS responder started");
  */
    if(!SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED)){
        Serial.println("SPIFFS Mount Failed");
        return;
    }
    writeFile(SPIFFS, "/shot.txt",  "1,02,103,104,105,06,07,108,109,110,11,112,113,114,115,aprof67890123456\n");
    appendFile(SPIFFS, "/shot.txt", "11,12,213,214,215,16,17,218,219,220,21,222,223,224,225,bprof67890123456\n");
   // file.println("testing 1, 2, 3.");
    delay(500);
    listDir(SPIFFS, "/", 0);  
    readShotFile(SPIFFS, "/shot.txt"); 
/*
1        2         3         4        5         6          7          8         9         10        11        12       13       14       15   , name
index,time pre, pwr pre, pres pre, flow pre, Rup time, shot time, shot pwr, shot flow, shot pres, Rdn time, spare12, spare13, spare14, spare15, prof nane
int,  int secs,   int   ,int,      int,       int secs, int secs,    int .     ,int .     ,int    int secs,  int,    int,       int,     int,   string
3         6        10,     14,      18,         21,      24,         28,       32,        36,       39,      43,      47,       51,       55,    80 

 * 
 */

//******************  check for ota mode   *********************** 
/*    
if (ota == true) {  //see if first button is pushed on startup
//if (digitalRead (PUSH01_PIN) == 0) {  //see if first button is pushed on startup
  //if button was pushed, setup for OTA update mode
  ota = true;
  lcd.clear();
  lcd.setCursor( 0, 0 );
  lcd.print( "OTA Program Mode" ); // 
  server.on("/", HTTP_GET, []() {
    server.sendHeader("Connection", "close");
    server.send(200, "text/html", loginIndex);
  });
  server.on("/serverIndex", HTTP_GET, []() {
    server.sendHeader("Connection", "close");
    server.send(200, "text/html", serverIndex);
  });
  //handling uploading firmware file 
  server.on("/update", HTTP_POST, []() {
    server.sendHeader("Connection", "close");
    server.send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
    ESP.restart();
  }, []() {
    HTTPUpload& upload = server.upload();
    if (upload.status == UPLOAD_FILE_START) {
      Serial.printf("Update: %s\n", upload.filename.c_str());
      if (!Update.begin(UPDATE_SIZE_UNKNOWN)) { //start with max available size
        Update.printError(Serial);
      }
    } else if (upload.status == UPLOAD_FILE_WRITE) {
      // flashing firmware to ESP
      if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
        Update.printError(Serial);
      }
    } else if (upload.status == UPLOAD_FILE_END) {
      if (Update.end(true)) { //true to set the size to the current progress
        Serial.printf("Update Success: %u\nRebooting...\n", upload.totalSize);
      } else {
        Update.printError(Serial);
      }
    }
  });
  server.begin();

  } */
  //else 
  if (digitalRead (PUSH02_PIN) == 0) {  //see if 2nd button is pushed on startup
  //if 2nd button pushed at start, setup for configuration information update mode
  config_info = true;
  lcd.clear();
  lcd.setCursor( 0, 0 );
  lcd.print( "OTA Program Mode" ); //      
  }
  else {    //if no buttons pushed at startup, normal mode
/*    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    while (WiFi.waitForConnectResult() != WL_CONNECTED) {
      Serial.println("Connection Failed! Rebooting...");
      delay(5000);
      ESP.restart();
    }

    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  
    if (MDNS.begin("esp32_Expobar")) {
    Serial.println("MDNS responder started");
    }
    server.on("/inline", []() {
    server.send(200, "text/plain", "Expobar connected");
    });  
    
    server.begin();
    Serial.println("HTTP server started");
 */   
  }

}


//*************************************************************************
//****************************    LOOP   **********************************

void loop() {

//    server.handleClient();
    readShotFile(SPIFFS, "/shot.txt"); 
    parseData();
    receive_serial ();  //check for  serial input
    delay(1000);
  
}
