// // #include <Arduino_MKRIoTCarrier.h>
// // #include "Colors.h"

// // MKRIoTCarrier carrier;

// // void setup() {
// //   Serial.begin(9600);
// //   delay(1500); 


// //   carrier.withCase();
// //   carrier.begin();
// //   // pinMode(TFT_BACKLIGHT, OUTPUT);
// //   // digitalWrite(TFT_BACKLIGHT, HIGH);
// //   // carrier.display.init(240, 240);
// //   // carrier.display.setRotation(0);
// //   // carrier.display.fillScreen(ST77XX_BLACK);
// //   // carrier.display.setTextColor(ST77XX_WHITE);
// //   // carrier.display.setTextSize(2);
// //   // carrier.display.cp437(true);
// // }

// // void loop() {
// //   carrier.Buttons.update();
// //   String fullTemp = "XXX.XX";
// //   String normTemp = "XX.XX";
// //   String lowTemp = "X.XX";
// //   String testFront = ".XX";
// //   String testFront2 = "-.XX";
// //   String testBack = "X.X";
// //   String data18 = "XXXXXXXXXXXXXXXXXX";
// //   String data = "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX";
// //   String testString = "A long string with several words";
// //   String testString2 = "string string string string string string";
// //   String testString3 = "0 2 4 6 8 1 2 4 6 8 2 2 4 6 8 3";

// //   uint16_t width = carrier.display.width();
// //   uint16_t height = carrier.display.height();

  // uint16_t centerX = width / 2;
  // uint16_t centerY = height / 2;
  // uint16_t thirdY = height / 3;
  // uint16_t thirdRow = thirdY * 2;
  // uint16_t fourthRow = thirdRow + 16;

  // //center y and center y
  // // carrier.display.drawFastVLine(centerX, 0, 240, display_green);
  // // carrier.display.drawFastHLine(0, centerY, 240, display_green);

  // // //first, third, and fourth rows
  // // carrier.display.drawFastHLine(0, thirdY, 240, CYAN);
  // // carrier.display.drawFastHLine(0, thirdRow, 240, CYAN);
  // // carrier.display.drawFastHLine(0, fourthRow, 240, CYAN);

  // // //center horizontal padding 
  // // carrier.display.drawFastVLine(centerX - 12, 0, 240, YELLOW);
  // // carrier.display.drawFastVLine(centerX + 12, 0, 240, YELLOW);

// //   //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// //   //try just printing the full one 
// //   // carrier.display.setCursor(12, centerY - 8);
// //   // carrier.display.print(fullTemp);
// //   // carrier.display.write(0xF8);
// //   // carrier.display.print("F");

// //   // carrier.display.setCursor(centerX + 12, centerY - 8);
// //   // carrier.display.print(fullTemp);
// //   // carrier.display.write(0xF8);
// //   // carrier.display.print("F");

// //   //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// //   if (carrier.Buttons.onTouchDown(TOUCH0)) {
// //     // testValidate(fullTemp, false);
// //     // testValidate(normTemp, false);
// //     // testValidate(lowTemp, false);
// //     // testValidate(testFront, true);
// //     // testValidate(testFront2, true);
// //     // testValidate(testBack, true);
// //     // Serial.println("Finished Validation Testing.");


// //     // testGetStart(fullTemp, 12);
// //     // testGetStart(normTemp, 24);
// //     // testGetStart(lowTemp, 36);
// //     // testGetStart(testBack, 48);
// //     // Serial.println("Finished Offset Testing.");

// //     String result = getLine(testString3, 18);
// //     testString3.remove(0, result.length() + 1);
// //     Serial.println(result.length());
// //     Serial.println(result);
// //     Serial.println(testString3);
// //     Serial.println("Finished Word Testing.");
// //   }//btn0 press






// //   //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


// //   //third row
// //   // carrier.display.setCursor(12, thirdRow - 8);
// //   // carrier.display.println(data);

// //   // //fourth row
// //   // carrier.display.setCursor(24, fourthRow - 8);
// //   // carrier.display.print("X");
// //   // carrier.display.setCursor(204, fourthRow - 8);
// //   // carrier.display.print("X");



// // }


// // void testValidate(String testData, bool expectChange) {
// //   String modifiedString = "";
// //   modifiedString = validateData(testData);

// //   if (!expectChange && modifiedString.equalsIgnoreCase(testData)) {
// //     Serial.println("Pass.");
// //   } else if (expectChange && !modifiedString.equalsIgnoreCase(testData)) {
// //     Serial.println("Pass.");
// //   } else if (expectChange && modifiedString.equalsIgnoreCase(testData)){
// //     Serial.println("FAIL - Expected change in: ");
// //     Serial.println(testData);
// //   } else if (!expectChange && !modifiedString.equalsIgnoreCase(testData)) {
// //     Serial.println("FAIL - Unexpected change in: ");
// //     Serial.println(testData);
// //   }
// // }

// // void testGetStart(String testData, uint16_t expected) {
// //   uint16_t result = -1;
// //   result = getStart(testData);
// //   if (result == expected) {
// //     Serial.println("Pass.");
// //   } else {
// //     Serial.print("FAIL - Expected: ");
// //     Serial.print(expected);
// //     Serial.print(", Result: ");
// //     Serial.println(result);
// //   }
// // }

// // void testGetWord(String testData) {
// //   Serial.println("---");
// //   Serial.print("<");
// //   Serial.print(testData);
// //   Serial.println(">");

// //   String result = "";
// //   do {
// //     result = getWord(testData);
// //     testData.remove(0, result.length() + 1);
// //     Serial.print("<");
// //     Serial.print(result);
// //     Serial.print("> : ");
// //     Serial.print("<");
// //     Serial.print(testData);
// //     Serial.println(">");
// //   } while (testData.length() > 0);
// // }



// String validateData(String str) {
//   //ensure that the string version of the temp reading has all the digits 
//   //checking for the following:
//   //    there must be TWO DIGITS following the decimal
//   //    there must be AT LEAST ONE DIGIT preceding the decimal 
//   int numChars = str.length();
//   int pointIndex = str.indexOf(".");
//   int negative = str.indexOf("-");

//   //verify there is a decimal
//   if (pointIndex == -1) {
//     str.concat(".00");
//     numChars = str.length();
//     pointIndex = str.indexOf(".");
//   }

//   //verify there is at least one digit before the decimal
//   if (pointIndex == 0) {
//     //insert zero at index 0
//     String tStr = String(str);
//     str = "0";
//     str.concat(tStr);
//   } else if (pointIndex == 1 && negative == 0) {
//     //insert zero at index 1
//     String tStr = String(str);
//     str = "-0";
//     str.concat(tStr);
//   }

//   //verify there are two digits following the decimal
//   //    if this catches anything, it will be a decimal 
//   //    followed by a single digit, so just append a zero
//   if (pointIndex != (numChars - 3)) {
//     str.concat("0");
//     numChars = str.length();
//   }
//   return str;
// }

// uint16_t getStart(String data) {
//   //data = validateData(data);
//   int length = data.length();
//   uint16_t start = 0;

//   switch(length) {
//     case 6:
//       start = 12;
//       break;
//     case 5:
//       start = 24;
//       break;
//     case 4:
//       start = 36;
//       break;
//     case 3:
//       start = 48;
//       break;
//     default:
//       start = 48;
//       break;
//   }//switch

//   return start;
// }

// //count is number of characters allowed, will add one to account for the space
// String getLine(String data, int count) {
//   if (data.length() <= count) {
//     return data;
//   }
//   int spaceIndex = 0;
//   int lookAhead = data.indexOf(" ");
//   while (lookAhead < (count + 1)) {
//     spaceIndex = lookAhead;
//     lookAhead = data.indexOf(" ", spaceIndex + 1);
//   } 
//   return data.substring(0, spaceIndex);
// }

// String getWord(String str) {
//   int space = str.indexOf(" ");
//   return str.substring(0, space);
// }