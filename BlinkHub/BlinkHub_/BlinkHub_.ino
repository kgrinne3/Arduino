// #include <BufferedPrint.h>
// #include <FreeStack.h>
// #include <MinimumSerial.h>
// #include <RingBuf.h>
// #include <SdFat.h>
// #include <SdFatConfig.h>
// #include <sdios.h>
// #include <SD.h>
// #include <Adafruit_ImageReader.h>
// #include <Adafruit_SPIFlash.h>


#include "Arduino.h"
#ifndef ACTION
	#include "arduino_secrets.h"
	#define ACTION FALSE
#endif
#include <SPI.h>
#include <WiFiNINA.h>
#include <ArduinoHttpClient.h>
#include <Arduino_MKRIoTCarrier.h>
#include <ArduinoJson.h>
#include <StreamUtils.h>
#include <RTCZero.h>
#include "State.h"
#include "Hub_State.h"
#include "Icon.h"
#include "Image.h"

//#define USE_SD_CARD

// typedef struct Icon Icon;

MKRIoTCarrier carrier;

// SdFat sdCard;         // SD card filesystem
// Adafruit_ImageReader reader(sdCard); // Image-reader object, pass in SD filesys

char serverIP[] = "rest-prod.immedia-semi.com";
WiFiSSLClient sslClient;
WiFiSSLClient weather; 
String target = "api.openweathermap.org";
HttpClient httpClient = HttpClient(sslClient, serverIP, 443);
HttpClient weatherClient = HttpClient(weather, target, 443);

Hub_State *hubState; 
//Icon *icon;
Image currIcon;
const char* type = "bmp";

String authToken = String();
bool loggedIn = false;
bool homeScreen = false;


String lat = "42.3433518";
String lon = "-88.0412192";

int status = WL_IDLE_STATUS;     // the WiFi radio's status

int textDelay = 5000; //duration to display message on screen before clearing

long commandIds[5];
long commandOne; 
long commandTwo;
long commandThree;
long commandFour;
long commandFive;


void setup() {
  // Initialize serial and wait for port to open:
  Serial.begin(9600);
  // This delay gives the chance to wait for a Serial Monitor without blocking if none is found
  delay(1500); 

  carrierSetup();
  carrier.display.cp437(true);
  initScreen();
  clearScreen();
  screenOff();

  hubState = createHubState(hubState);
  updateLights(hubState);

  httpClient.setHttpResponseTimeout(5000);

  connectToWifi();

  // you're connected now, so print out the data:
  if (Serial) {
    Serial.println("You're connected to the network.");
    printWifiStatus();
  }



  delay(500); 
  do {
    authorizedLogin();
  } while (!loggedIn);

  do {
    getHomescreen();
  } while (!homeScreen);
} //SETUP

void loop() {
  carrier.Buttons.update();
  
  //pinMode(A3, INPUT_PULLUP);
  // Serial.print("lower_bound:0, ");
  // //Serial.print(value);
  // for (int i = 1; i < 6; i++) {
  //   // Serial.print("pin");
  //   // Serial.print(i);
  //   Serial.print("btn");
  //   Serial.print(i - 1);
  //   Serial.print(":");
  //   Serial.print(analogRead(i));
  //   Serial.print(", ");
  // }
  // Serial.println("upper_bound:1200");
  // delay(10);

  // if (analogRead(A5) > 450) {
  //   Serial.println("Touching 0");
  // }
  // if (analogRead(A1) > 450) {
  //   Serial.println("Touching 1");
  // }
  // if (analogRead(A2) > 450) {
  //   Serial.println("Touching 2");
  // }
  // if (analogRead(A3) > 450) {
  //   Serial.println("Touching 3");
  // }
  // if (analogRead(A4) > 450) {
  //   Serial.println("Touching 4");
  // }
  // delay(10);



  // Your code here 

  if (status != WL_CONNECTED) {
    checkWifi();
  }

  //
  // Button Zero Press
  // GET STATUS
  //
  if (carrier.Buttons.onTouchDown(TOUCH0)) {

    //update status
    recognizeBtnPress();


    // carrier.Buttons.update();
    // getHomescreen();

    getWeather(hubState);


  }//btn0 press

 
  // //
  // // Button One Press 
  // // ARM
  // //
  // if (carrier.Buttons.onTouchDown(TOUCH1)) {
  //   recognizeBtnPress();
  //   carrier.Buttons.update();
  //   setArmed(hubState);
  //   getHomescreen();

   

  // }//if btn press





  // //
  // // Button Two Press 
  // // DISARM
  // //
  // if (carrier.Buttons.onTouchDown(TOUCH2)) {
  //   recognizeBtnPress();
  //   carrier.Buttons.update();
  //   setDisarmed(hubState);
  //   getHomescreen();
    

  // }//if btn2 press

  // if (analogRead(A3) < 550) {
  //   recognizeBtnPress();
  //   carrier.Buttons.update(); //do i need this still? probs not
  //   getWeather(hubState);
  // }

  //
  // Button Four Press 
  // SHOW MESSAGE
  //
  if (carrier.Buttons.onTouchDown(TOUCH4)) {
    recognizeBtnPress();
    carrier.Buttons.update();
    updateLights(hubState);
    // initScreen();
    // displayText();
    // if (Serial) {
    //   Serial.println("Message was displayed.");
    // }
    initScreen();
    uint16_t width = carrier.display.width();
    uint16_t height = carrier.display.height();

    uint16_t centerX = width / 2;
    uint16_t centerY = height / 2;
    uint16_t thirdY = height / 3;
    uint16_t thirdRow = thirdY * 2;
    uint16_t fourthRow = thirdRow + 16;

    //center y and center y
    carrier.display.drawFastVLine(centerX, 0, 240, display_green);
    carrier.display.drawFastHLine(0, centerY, 240, display_green);

    //first, third, and fourth rows
    carrier.display.drawFastHLine(0, thirdY, 240, CYAN);
    carrier.display.drawFastHLine(0, thirdRow, 240, CYAN);
    carrier.display.drawFastHLine(0, fourthRow, 240, CYAN);

    //center horizontal padding 
    carrier.display.drawFastVLine(centerX - 12, 0, 240, YELLOW);
    carrier.display.drawFastVLine(centerX + 12, 0, 240, YELLOW);
    


    delay(10000);
    clearScreen();
    screenOff();
    


  }//if btn4 press



}//loop


void carrierSetup() {
  carrier.withCase();
  carrier.begin();
  carrier.leds.begin();
  carrier.leds.setBrightness(25);
  carrier.Buttons.updateConfig(10);




}

void initScreen() {
  pinMode(TFT_BACKLIGHT, OUTPUT);
  digitalWrite(TFT_BACKLIGHT, HIGH);
  carrier.display.init(240, 240);
  carrier.display.setRotation(0);
  carrier.display.fillScreen(ST77XX_BLACK);
  carrier.display.setTextColor(ST77XX_WHITE);
  carrier.display.setTextSize(4);
  carrier.display.setCursor(105, 104);
}


void clearScreen() {
  carrier.display.fillScreen(ST77XX_BLACK);
  // delay(50);
  // pinMode(TFT_BACKLIGHT, OUTPUT);
  // digitalWrite(TFT_BACKLIGHT, LOW);
}

void screenOff() {
  pinMode(TFT_BACKLIGHT, OUTPUT);
  digitalWrite(TFT_BACKLIGHT, LOW);
}

//contains a delay!!
//delay(5000) to display text on screen for the duration 
void displayText() {
  int start = hubState->startPixel;
  carrier.display.setCursor(start+1, 110);
  carrier.display.print(hubState->msg);
  delay(textDelay);
  clearScreen();
  screenOff();
}

//contains a delay!! 
//delay(75) to allow white lights to show momentarily 
void recognizeBtnPress() {
  carrier.leds.fill(white, 0, 5); 
  carrier.leds.show();
  delay(75);
  carrier.leds.clear();
  carrier.leds.show();
}

void updateLights(Hub_State* hubState) {
  if (Serial) {
    Serial.print("Current State: ");
    Serial.println(hubState->msg);
  }
  carrier.leds.fill(hubState->color, 0, 5); 
  carrier.leds.show();
}

//contains a delay!!
//delay(5000) after attempting connection if not already connected
void connectToWifi() {
  while (status != WL_CONNECTED) {
    if (Serial) {
      Serial.print("Attempting to connect to SSID: ");
      Serial.println(SECRET_SSID);
    }
    status = WiFi.begin(SECRET_SSID, SECRET_OPTIONAL_PASS);
    delay(5000);
  }
  hubState = setState(hubState, WAITING);
  updateLights(hubState);
}

//contains a delay!!
//delay(5000) while attempting connection if not already connected
void checkWifi() {
  if (Serial) {
    Serial.println("Checking WiFi Connection...");
  }
  while (status != WL_CONNECTED) {
    if (Serial) {
      Serial.print("Attempting to connect to SSID: ");
      Serial.println(SECRET_SSID);
    }

    hubState = setState(hubState, ERROR);
    setDetails(hubState, "WiFi Connection");
    updateLights(hubState);
    status = WiFi.begin(SECRET_SSID, SECRET_OPTIONAL_PASS);
    delay(5000);
  }

}

void printWifiStatus() {
  if (Serial) {
    Serial.print("SSID: ");
    Serial.println(WiFi.SSID());
    IPAddress ip = WiFi.localIP();
    Serial.print("IP Address: ");
    Serial.println(ip);
    long rssi = WiFi.RSSI();
    Serial.print("signal strength (RSSI):");
    Serial.print(rssi);
    Serial.println(" dBm");
  }
}


String updateAuth(const char* tkn) {
  authToken = String(tkn);
  return authToken;
}

//contains a delay!!
//delay(100) before reattempting login
void authorizedLogin() {
  hubState = setState(hubState, WAITING);
  updateLights(hubState);
  httpClient = HttpClient(sslClient, "rest-prod.immedia-semi.com", 443);
  httpClient.setHttpResponseTimeout(5000);
  
  if (Serial) {
    Serial.println("Making login request...");
  } 

  String data = "{\"email\":\"kgrinnell1@gmail.com\",\"password\":\"";
  data = String(data);
  data.concat(PASSWORD);
  data.concat("\",\"unique_id\":\"");
  data.concat(UNIQUE_ID);
  data.concat("\",\"reauth\":\"true\"}");

  httpClient.beginRequest();
  httpClient.post("/api/v5/account/login");
  httpClient.sendHeader("Content-Type", "application/json");
  httpClient.sendHeader("Content-Length", data.length());
  httpClient.beginBody();
  httpClient.print(data);
  httpClient.endRequest();


  int statusCode = httpClient.responseStatusCode();
  String response = httpClient.responseBody();

  if (Serial) {
    Serial.print("Status code: ");
    Serial.println(statusCode);
    Serial.print("Response: ");
    Serial.println(response);
    Serial.println("Parsing Response");
  }
  
  if (statusCode != 200) {
    String deets = String("Status Code: ");
    deets.concat(statusCode);
    hubState = setState(hubState, ERROR);
    hubState = setDetails(hubState, deets.c_str());
    updateLights(hubState);
    //return;
    httpClient.stop();
    return;
    // delay(100);
    // authorizedLogin();
  }
  
  JsonDocument doc; //no need to declare capacity of 1536
  
  DeserializationError error = deserializeJson(doc, response.c_str());
  if (error) {
    String deets = "Serialization Failed";
    hubState = setState(hubState, ERROR); 
    hubState = setDetails(hubState, deets.c_str());
    updateLights(hubState);
    if (Serial) {
      Serial.print("deserializeJson() failed: ");
      Serial.println(error.c_str());
    }
    httpClient.stop();
    return;
  }

  const char* tkn = doc["auth"]["token"];
  authToken = updateAuth(tkn);
  loggedIn = true;
  //doc.garbageCollect();
}


void getHomescreen() {
  hubState = setState(hubState, WAITING);
  updateLights(hubState);
  httpClient = HttpClient(sslClient, "rest-e004.immedia-semi.com", 443);
  httpClient.setHttpResponseTimeout(5000);
  if (Serial) {
    Serial.println("Making homescreen request...");
  } 

  httpClient.beginRequest();
  httpClient.get("/api/v3/accounts/221986/homescreen");
  httpClient.sendHeader("Host", "rest-e004.immedia-semi.com");
  httpClient.sendHeader("TOKEN-AUTH", authToken);
  httpClient.endRequest();


  int statusCode = httpClient.responseStatusCode();
  String response = httpClient.responseBody();

  if (Serial) {
    Serial.print("Status code: ");
    Serial.println(statusCode);
    Serial.print("Response: ");
    Serial.println(response.c_str());
    Serial.println("Parsing Response");
  }
  
  if (statusCode != 200) {
    String deets = String("Status Code: ");
    deets.concat(statusCode);
    hubState = setState(hubState, ERROR);
    hubState = setDetails(hubState, deets.c_str());
    updateLights(hubState);
    httpClient.stop();
    delay(100);
    //getHomescreen();
    homeScreen = false;
    return;
  }

  homeScreen = true;

  JsonDocument doc; //6144 

  
  DeserializationError error = deserializeJson(doc, response.c_str());

  bool armedStatus = doc["networks"][0]["armed"]; 

  if (error) {
    String deets = "Serialization Failed";

    if (Serial) {
      Serial.print("deserializeJson() failed: ");
      Serial.println(error.c_str());
      Serial.print("Armed Status from JSON: ");
      Serial.println(armedStatus);
    }


    if (armedStatus) {
      hubState = setState(hubState, ARMED);
    } else {
      hubState = setState(hubState, DISARMED);
    }
    updateLights(hubState);

    Serial.println(getPrintName(hubState->state));

    return;
  }


  if (armedStatus) {
    hubState = setState(hubState, ARMED);
  } else {
    hubState = setState(hubState, DISARMED);
  }
  updateLights(hubState);

  if (Serial) {
    Serial.println(getPrintName(hubState->state));
  }


}

void setArmed(Hub_State* hubState) {
  if (Serial) {
    Serial.println("Arm button press.");
  }

  if (hubState->state == ARMED) {
    return;
  } else {

    hubState = setState(hubState, WAITING);
    updateLights(hubState);
    httpClient = HttpClient(sslClient, "rest-e004.immedia-semi.com", 443);
    httpClient.setHttpResponseTimeout(5000);
    if (Serial) {
      Serial.println("Making ARM request...");
    } 

    // /api/v1/accounts/221986/networks/289274/state/arm
    httpClient.beginRequest();
    httpClient.post("/api/v1/accounts/221986/networks/289274/state/arm");
    httpClient.sendHeader("Host", "rest-e004.immedia-semi.com");
    httpClient.sendHeader("TOKEN-AUTH", authToken);
    httpClient.sendHeader("Content-Type", "application/json");
    httpClient.endRequest();

    if (Serial) {
      Serial.println("Request sent.");
    } 

    int statusCode = httpClient.responseStatusCode();
    String response = httpClient.responseBody();


    if (Serial) {
      Serial.print("Status code: ");
      Serial.println(statusCode);
      Serial.print("Response: ");
      Serial.println(response);
      Serial.println("Parsing Response");
    }
    
    if (statusCode != 200) {
      String deets = String("Status Code: ");
      deets.concat(statusCode);
      hubState = setState(hubState, ERROR);
      hubState = setDetails(hubState, deets.c_str());
      updateLights(hubState);
    }



    JsonDocument doc; //768

    DeserializationError error = deserializeJson(doc, response.c_str());

    if (error) {
      Serial.print("deserializeJson() failed: ");
      Serial.println(error.c_str());
      return;
    }


    // long commandIds[5];
    int i = 0;
    for (JsonObject command : doc["commands"].as<JsonArray>()) {
      //long command_id = command["id"]; // 800862014, 800862015, 800862016, 800862017, 800862018
      commandIds[i] = command["id"];
      switch (i) {
        case 0: 
          commandOne = command["id"];
          break;
        case 1: 
          commandTwo = command["id"];
          break;
        case 2: 
          commandThree = command["id"];
          break;
        case 3: 
          commandFour = command["id"];
          break;
        case 4: 
          commandFive = command["id"];
          break;
      }
      i++;
    }
    
    if (Serial) {

      Serial.println("Command IDs: ");
      Serial.println(commandOne);
      Serial.println(commandTwo);
      Serial.println(commandThree);
      Serial.println(commandFour);
      Serial.println(commandFive);
    }
  }
  

} //setArmed

void setDisarmed(Hub_State* hubState) {
  if (Serial) {
    Serial.println("Disarm button press.");
  }
  // checkWifi();
  // getHomescreen();

  if (hubState->state == DISARMED) {
    return;
  } else {

    hubState = setState(hubState, WAITING);
    updateLights(hubState);
    httpClient = HttpClient(sslClient, "rest-e004.immedia-semi.com", 443);
    httpClient.setHttpResponseTimeout(5000);
    if (Serial) {
      Serial.println("Making DISARM request...");
    } 

    // /api/v1/accounts/221986/networks/289274/state/arm
    httpClient.beginRequest();
    httpClient.post("/api/v1/accounts/221986/networks/289274/state/disarm");
    httpClient.sendHeader("Host", "rest-e004.immedia-semi.com");
    httpClient.sendHeader("TOKEN-AUTH", authToken);
    httpClient.sendHeader("Content-Type", "application/json");
    httpClient.endRequest();

    if (Serial) {
      Serial.println("Request sent.");
    } 

    int statusCode = httpClient.responseStatusCode();
    String response = httpClient.responseBody();


    if (Serial) {
      Serial.print("Status code: ");
      Serial.println(statusCode);
      Serial.print("Response: ");
      Serial.println(response);
      Serial.println("Parsing Response");
    }
    
    if (statusCode != 200) {
      String deets = String("Status Code: ");
      deets.concat(statusCode);
      hubState = setState(hubState, ERROR);
      hubState = setDetails(hubState, deets.c_str());
      updateLights(hubState);
    }


    JsonDocument doc; //768 

    DeserializationError error = deserializeJson(doc, response.c_str());

    if (error) {
      Serial.print("deserializeJson() failed: ");
      Serial.println(error.c_str());
      return;
    }


    // long commandIds[5];
    int i = 0;
    for (JsonObject command : doc["commands"].as<JsonArray>()) {
      //long command_id = command["id"]; // 800862014, 800862015, 800862016, 800862017, 800862018
      commandIds[i] = command["id"];
      switch (i) {
        case 0: 
          commandOne = command["id"];
          break;
        case 1: 
          commandTwo = command["id"];
          break;
        case 2: 
          commandThree = command["id"];
          break;
        case 3: 
          commandFour = command["id"];
          break;
        case 4: 
          commandFive = command["id"];
          break;
      }
      i++;
    }
    
    if (Serial) {

      Serial.println("Command IDs: ");
      Serial.println(commandOne);
      Serial.println(commandTwo);
      Serial.println(commandThree);
      Serial.println(commandFour);
      Serial.println(commandFive);
    }
  }


}// setDisarmed

void getWeather(Hub_State* hubState) {
  Hub_State* prevState = setState(prevState, hubState->state);

  hubState = setState(hubState, WAITING);
  updateLights(hubState);

  String target = "api.openweathermap.org";

  weatherClient = HttpClient(weather, target, 443);
  weatherClient.setHttpResponseTimeout(5000);
  if (Serial) {
    Serial.println("Fetching Weather...");
  } 

  String data = "/data/3.0/onecall?lat=";
  data = String(data);
  data.concat(lat);
  data.concat("&lon=");
  data.concat(lon);
  data.concat("&exclude=minutely,hourly,alerts&appid=");
  data.concat(APPKEY); 
  data.concat("&units=imperial");

  // if (Serial) {
  //   Serial.println(data);
  // }

  weatherClient.get(data);

  int statusCode = weatherClient.responseStatusCode();
  String response = weatherClient.responseBody();

  if (Serial) {
    Serial.print("Status code: ");
    Serial.println(statusCode);
    Serial.print("Response: ");
    Serial.println(response.c_str());
    Serial.println("Parsing Response");
  }
  
  if (statusCode != 200) {
    String deets = String("Status Code: ");
    deets.concat(statusCode);
    hubState = setState(hubState, ERROR);
    hubState = setDetails(hubState, deets.c_str());
    updateLights(hubState);
    //return;
    weatherClient.stop();
    // delay(100);
    // getWeather(hubState);
    return;
  }



  JsonDocument doc;

  DeserializationError error = deserializeJson(doc, response.c_str());

  if (error) {
    String deets = "Serialization Failed";

    if (Serial) {
      Serial.print("deserializeJson() failed: ");
      Serial.println(error.c_str());
    }


    if (prevState->state == ARMED) {
      hubState = setState(hubState, ARMED);
    } else {
      hubState = setState(hubState, DISARMED);
    }
    updateLights(hubState);

    Serial.println(getPrintName(hubState->state));

    return;
  } // if error


  if (Serial) {
    Serial.println("Created Doc...");
  }


  JsonObject current = doc["current"];
  float temp = current["temp"];
  float feelsLike = current["feels_like"];
  int humidity = current["humidity"];
  JsonObject currWeather = current["weather"][0];
  String weatherState = currWeather["main"];
  String description = currWeather["description"];
  String iconCode = currWeather["icon"];
  iconCode.remove(2, 1);

  JsonObject daily = doc["daily"][0];
  String summary = daily["summary"];
  summary.remove(0, 16);
  JsonObject tempObj = daily["temp"];
  float minTemp = tempObj["min"];
  float maxTemp = tempObj["max"];
  String dailyCode = doc["daily"][0]["weather"][0]["icon"];
  dailyCode.remove(2, 1);

  if (Serial) {
    Serial.print("Daily Summary: ");
    Serial.println(summary);
    Serial.print("Today's Min: ");
    Serial.println(minTemp);
    Serial.print("Today's Max: ");
    Serial.println(maxTemp);
    Serial.print("Code: ");
    Serial.println(dailyCode);
  }





  // showWeather(weatherState, temp, feelsLike, humidity, iconCode.toInt());
  // clearScreen();
  initScreen();
  showForecast(weatherState, minTemp, maxTemp, summary, dailyCode.toInt());
  clearScreen();
  screenOff();



  if (prevState->state == ARMED) {
    hubState = setState(hubState, ARMED);
  } else {
    hubState = setState(hubState, DISARMED);
  }
  updateLights(hubState);

  if (Serial) {
    Serial.println(getPrintName(hubState->state));
  }


}

void showWeather(String weatherState, float temp, float feelsLike, float humidity, int iconCode) {
  initScreen();
  carrier.display.setTextSize(2);

  uint16_t width = carrier.display.width();
  uint16_t height = carrier.display.height();

  uint16_t centerX = width / 2;
  uint16_t centerY = height / 2;
  uint16_t thirdY = height / 3;
  uint16_t thirdRow = thirdY * 2;
  uint16_t fourthRow = thirdRow + 16;

  // carrier.display.drawFastVLine(centerX, 0, 240, display_green);

  // carrier.display.drawFastHLine(0, centerY, 240, display_green);
  // carrier.display.drawFastHLine(0, thirdY, 240, CYAN);
  // carrier.display.drawFastHLine(0, thirdRow, 240, CYAN);

  // carrier.display.drawFastVLine(centerX - 12, 0, 240, YELLOW);
  // carrier.display.drawFastVLine(centerX + 12, 0, 240, YELLOW);

  String tempStr = String(temp, 2);
  String feelsStr = String(feelsLike, 2);
  String humStr = String(humidity);


  //icon = getIcon(iconCode);
  currIcon = Image(iconCode);

  uint16_t topY = thirdY / 2;
  uint16_t iconX = centerX - (currIcon.getOffset());
  uint16_t iconY = topY - (currIcon.getVOffset());


  carrier.display.drawRect(iconX, iconY, currIcon.getWidth(), currIcon.getHeight(), 0xFFFF);


  carrier.display.drawBitmap(iconX, iconY, getData(currIcon.getFileName()), currIcon.getWidth(), currIcon.getHeight(), 0xFFFF);


  uint16_t topLine = thirdY - 8;
  uint16_t stateIndent; 
  uint16_t stateLen = weatherState.length();
  stateIndent = centerX - ((stateLen * 12) / 2);

  carrier.display.setCursor(stateIndent, topLine);
  carrier.display.println(weatherState);


  uint16_t midLine = centerY - 6;
  uint16_t midStart; 
  bool needZero = false;
  bool prob = false;

  // if (tempStr.length() == 7) {
  //   midStart = 0;
  // } else if (tempStr.length() == 6) {
  //   //offset 1
  //   midStart = 12;
  // } else if (tempStr.length() == 5) {
  //   //offset 2
  //   midStart = 24;
  // } else if (tempStr.length() == 4) {
  //   //offset 3
  //   midStart = 36;
  // } else if (tempStr.length() == 3) {
  //   //just the decimal, no leading zero
  //   //offset 3
  //   midStart = 36;
  //   needZero = true;
  // } else {
  //   //error - just display zeros i guess
  //   //offset 3
  //   midStart = 36;
  //   prob = true;
  // }
  tempStr = validateData(tempStr);
  midStart = getStart(tempStr);

  carrier.display.setCursor(midStart, midLine);
  carrier.display.print(tempStr);
  carrier.display.write(0xF8);
  carrier.display.print("F ");

  // if (needZero) {
  //   carrier.display.print("0");
  // }
  // if (!prob) {
  //   carrier.display.print(temp);
  //   carrier.display.write(0xF8);
  //   carrier.display.print("F ");
  // } else {
  //   carrier.display.print("0.00");
  //   carrier.display.write(0xF8);
  //   carrier.display.print("F ");
  // }
  
  if (humStr.length() == 3) {
    //print single blank char or move cursor over 12px
    carrier.display.print(" " + humStr);
  } else if (humStr.length() == 2 || humStr.length() == 1) {
    // print two blank char or move cursor over 24px
    carrier.display.print("  " + humStr);
  } else {
    //should not happen, just display 0 i guess
    carrier.display.print("  0");
  }
  carrier.display.println("% hum");


  int feelsTemp = round(feelsLike);
  feelsStr = String(feelsTemp);
  int len = (feelsStr.length() + 2) * 24; // +2 for deg f
  uint16_t offset = centerX - (len / 2);

  carrier.display.setCursor(offset, thirdRow);
  carrier.display.setTextSize(4);
  carrier.display.print(feelsTemp);
  carrier.display.write(0xF8);
  carrier.display.print("F");

  delay(10000);
  clearScreen();
}

void showForecast(String weatherState, float min, float max, String summary, int iconCode) {
  //initScreen();
  carrier.display.setTextSize(2);

  uint16_t width = carrier.display.width();
  uint16_t height = carrier.display.height();

  uint16_t centerX = width / 2;
  uint16_t centerY = height / 2;
  uint16_t thirdY = height / 3;
  uint16_t thirdRow = thirdY * 2;
  uint16_t fourthRow = thirdRow + 16;

  //center y and center y
  carrier.display.drawFastVLine(centerX, 0, 240, display_green);
  carrier.display.drawFastHLine(0, centerY, 240, display_green);

  //first, third, and fourth rows
  carrier.display.drawFastHLine(0, thirdY, 240, CYAN);
  carrier.display.drawFastHLine(0, thirdRow, 240, CYAN);
  carrier.display.drawFastHLine(0, fourthRow, 240, CYAN);

  //center horizontal padding 
  carrier.display.drawFastVLine(centerX - 12, 0, 240, YELLOW);
  carrier.display.drawFastVLine(centerX + 12, 0, 240, YELLOW);

  String minStr = String(min, 2);
  String maxStr = String(max, 2);


  currIcon = Image(iconCode);

  uint16_t topY = thirdY / 2;
  uint16_t iconX = centerX - (currIcon.getOffset());
  uint16_t iconY = topY - (currIcon.getVOffset());


  carrier.display.drawRect(iconX, iconY, currIcon.getWidth(), currIcon.getHeight(), 0xFFFF);


  carrier.display.drawBitmap(iconX, iconY, getData(currIcon.getFileName()), currIcon.getWidth(), currIcon.getHeight(), 0xFFFF);


  uint16_t topLine = thirdY - 8;
  uint16_t stateIndent; 
  uint16_t stateLen = weatherState.length();
  stateIndent = centerX - ((stateLen * 12) / 2);

  carrier.display.setCursor(stateIndent, topLine);
  carrier.display.println(weatherState);


  uint16_t midLine = centerY - 6;
  uint16_t midStart; 
  // bool needZero = false;
  // bool prob = false;

  // if (minStr.length() == 7) {
  //   midStart = 0;
  // } else if (minStr.length() == 6) {
  //   //offset 1
  //   midStart = 12;
  // } else if (minStr.length() == 5) {
  //   //offset 2
  //   midStart = 24;
  // } else if (minStr.length() == 4) {
  //   //offset 3
  //   midStart = 36;
  // } else if (minStr.length() == 3) {
  //   //just the decimal, no leading zero
  //   //offset 3
  //   midStart = 36;
  //   needZero = true;
  // } else {
  //   //error - just display zeros i guess
  //   //offset 3
  //   midStart = 36;
  //   prob = true;
  // }

  // carrier.display.setCursor(midStart, midLine);
  // if (needZero) {
  //   carrier.display.print("0");
  // }
  // if (!prob) {
  //   carrier.display.print(min);
  //   carrier.display.write(0xF8);
  //   carrier.display.print("F ");
  // } else {
  //   carrier.display.print("0.00");
  //   carrier.display.write(0xF8);
  //   carrier.display.print("F ");
  // }
  
  // if (maxStr.length() > 3) {
  //   //print single blank char or move cursor over 12px
  //   carrier.display.setCursor(centerX + 13, midLine);
  //   carrier.display.print(maxStr);
  // } else if (maxStr.length() <= 2) {
  //   // print two blank char or move cursor over 24px
  //   carrier.display.setCursor(centerX + 25, midLine);
  //   carrier.display.print("0" + maxStr);
  // } else {
  //   //should not happen, just display 0 i guess
  //   carrier.display.setCursor(centerX + 25, midLine);
  //   carrier.display.print("0");
  // }
  // carrier.display.write(0xF8);
  // carrier.display.print("F ");

  minStr = validateData(minStr);
  midStart = getStart(minStr);

  carrier.display.setCursor(midStart, midLine);
  carrier.display.print(minStr);
  carrier.display.write(0xF8);
  carrier.display.print("F ");

  maxStr = validateData(maxStr);
  midStart = getStart(maxStr);

  carrier.display.setCursor(centerX + midStart, midLine);
  carrier.display.print(maxStr);
  carrier.display.write(0xF8);
  carrier.display.print("F ");


  // int feelsTemp = round(feelsLike);
  // maxStr = String(feelsTemp);
  // int len = summary.length() * 24; // +2 for deg f
  // uint16_t offset = centerX - (len / 2);

  //carrier.display.setCursor(16, (thirdRow) - 8);
  String tempString = getLine(summary, 18);
  int len = tempString.length();
  int sumStart = centerX - ((len * 12) / 2);
  summary.remove(0, len + 1);
  carrier.display.setCursor(sumStart, thirdRow - 8);
  carrier.display.print(tempString);

  tempString = getLine(summary, 16);
  len = tempString.length();
  sumStart = centerX - ((len * 12) / 2);
  summary.remove(0, len + 1);
  carrier.display.setCursor(sumStart, fourthRow - 8);
  carrier.display.print(tempString);

  delay(10000);
  clearScreen();
}

String validateData(String str) {
  //ensure that the string version of the temp reading has all the digits 
  //checking for the following:
  //    there must be TWO DIGITS following the decimal
  //    there must be AT LEAST ONE DIGIT preceding the decimal 
  int numChars = str.length();
  int pointIndex = str.indexOf(".");
  int negative = str.indexOf("-");

  //verify there is a decimal
  if (pointIndex == -1) {
    str.concat(".00");
    numChars = str.length();
    pointIndex = str.indexOf(".");
  }

  //verify there is at least one digit before the decimal
  if (pointIndex == 0) {
    //insert zero at index 0
    String tStr = String(str);
    str = "0";
    str.concat(tStr);
  } else if (pointIndex == 1 && negative == 0) {
    //insert zero at index 1
    String tStr = String(str);
    str = "-0";
    str.concat(tStr);
  }

  //verify there are two digits following the decimal
  //    if this catches anything, it will be a decimal 
  //    followed by a single digit, so just append a zero
  if (pointIndex != (numChars - 3)) {
    str.concat("0");
    numChars = str.length();
  }
  return str;
}

uint16_t getStart(String data) {
  //data = validateData(data);
  int length = data.length();
  uint16_t start = 0;

  switch(length) {
    case 6:
      start = 12;
      break;
    case 5:
      start = 24;
      break;
    case 4:
      start = 36;
      break;
    case 3:
      start = 48;
      break;
    default:
      start = 48;
      break;
  }//switch

  return start;
}

//count is number of characters allowed, will add one to account for the space
String getLine(String data, int count) {
  if (data.length() <= count) {
    return data;
  }
  int spaceIndex = 0;
  int lookAhead = data.indexOf(" ");
  while (lookAhead > 0 && lookAhead < (count + 1)) {
    spaceIndex = lookAhead;
    lookAhead = data.indexOf(" ", spaceIndex + 1);
  } 

  return data.substring(0, spaceIndex);
}