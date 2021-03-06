/*
   Copyright (c) 2016 RF Digital Corp. All Rights Reserved.

   The source code contained in this file and all intellectual property embodied in
   or covering the source code is the property of RF Digital Corp. or its licensors.
   Your right to use this source code and intellectual property is non-transferable,
   non-sub licensable, revocable, and subject to terms and conditions of the
   SIMBLEE SOFTWARE LICENSE AGREEMENT.
   http://www.simblee.com/licenses/SimbleeSoftwareLicenseAgreement.txt

   THE SOURCE CODE IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND.

   This heading must NOT be removed from this file.
*/
#include "Slot.h"
#include "Wine.h"
#include <SimbleeForMobile.h>
#include <vector>

#include "WineImages.h"

#define GE_RED rgb(158,11,15)
#define GE_YELLOW rgb(243,165,25)
#define GE_GRAY rgb(159,159,159)
#define GE_DARKGRAY rgb(123,123,123)
#define GE_LIGHTGRAY rgb(194,193,193)
#define GE_BEIGE rgb(216,208,200)

#define color CRGB::Green
#define color2 CRGB::Blue

#define LED_PIN 5
#define NUM_LEDS 25

#define numberOfWines 10
#define INSERT_WINE_TIMEOUT 15000

#define WineSpashScreen 1
#define ColumbiaCrest1 2
#define ColumbiaCrest2 3
#define Carmen1 4
#define Carmen2 5
#define CoppolaShiraz1 6
#define CoppolaShiraz2 7
#define LaTercera1 8
#define LaTercera2 9
#define Martini1 10
#define Martini2 11
#define CoppolaCab1 12
#define CoppolaCab2 13
#define Prieure1 14
#define Prieure2 15
#define PerezCruz1 16
#define PerezCruz2 17
#define Proximo1 18
#define Proximo2 19

CRGB leds[NUM_LEDS];

std::vector<Slot> Slots;

bool disconnected = false;
bool initialized = false;
bool updatePage = true;
bool addMode = false;

uint32_t nextButtonState[25];
uint32_t currentButtonState[25];
int count = 0;

//UI ELEMENTS
uint8_t previousPage, nextPage, pageValue,
        insertScreen1, insertScreen2, insertScreen3, insertScreen4, insertScreen5, insertScreen6, insertScreen7,
        addButton,
        removeScreen1, removeScreen2, removeScreen3, removeScreen4,
        removeButton, returnButton,
        VineyardUI[5], WineName1UI[5], WineName2UI[5], CountryUI[5], PriceUI[5], LocationUI[5], OverlayUI[5], WWDWTB[5], starsRect[5], starsImg[5],
        VineyardUISingle, WineName1UISingle, WineName2UISingle, CountryUISingle, PriceUISingle, LocationUISingle,
        addPopUpDetectedText, addPopUpRackText, addPopUpRackNumber, addPopUpSelectText,
        removePopUpDetectedText, removePopUpRackText, removePopUpRackNumber, removePopUpSelectText,
        screen2Title, screen2Background, screen2Segment,
        screen6Segment, screen6nextPage, screen6previousPage, screen6pageValue,
        HistoryVineyardUI[5], HistoryWineName1UI[5], HistoryWineName2UI[5], HistoryCountryUI[5], HistoryOverlayUI[5],
        consumeButton, giftButton, deleteButton, clickedOverlay2,
        HistoryVineyardUISingle, HistoryWineName1UISingle, HistoryWineName2UISingle, HistoryCountryUISingle, HistoryPriceUISingle, starsRectUISingle,
        rate[5];

int8_t winePage = 1,
       historyPage = 1,
       addDetected = -1,
       removeDetected = -1,
       switchDetected = -1,
       clickedOverlay = 0;

char *ChillerSlot[25] = {"A1", "A2", "A3", "A4", "A5",
                         "B1", "B2", "B3", "B4", "B5",
                         "C1", "C2", "C3", "C4", "C5",
                         "D1", "D2", "D3", "D4", "D5",
                         "E1", "E2", "E3", "E4", "E5"
                        };

//  Wine.h describes a basic "Wine" class. There are the classic getter
//    methods for various static values. There exists only one setter value
//    for "index," which would be the index to the Slots vector defined below.

Wine wine[25] = {Wine("Grand Estates", "Columbia Crest",  "2003", "Cabernet Sauvignon", "$11.24", -1, 0),
                 Wine("Grand Reserva", "Carmen", "2012", "Carbenet Sauvignon", "$14.50", -1, 0),
                 Wine("Rosso & Blonco", "Francis Ford Coppola", "2011", "Shiraz", "$12.00", -1, 0),
                 Wine("Reserva Rioja", "La Tercera", "2010", "Spanish Red", "$17.98", -1, 0),
                 Wine("Sonoma County", "Louis M. Martini", "2013", "Cabernet Sauvignon", "$14.99", -1, 0),
                 Wine("Diamond Collection Ivory", "Francis Ford Coppola", "2013", "Cabernet Sauvignon", "$14.98", -1, 0),
                 Wine("Prieure de Cenac", "Chateau Saint-Didier-Parnac", "2014", "Malbec", "$7.98", -1, 0),
                 Wine("Reserva", "Perez Cruz", "2013", "Cabernet Sauvignon", "$14.46", -1, 0),
                 Wine("Proximo Roja", "Marquest De Riscal", "2010", "Spanish Red", "$9.48", -1, 0),
                 Wine(),
                 Wine(),
                 Wine(),
                 Wine(),
                 Wine(),
                 Wine(),
                 Wine(),
                 Wine(),
                 Wine(),
                 Wine(),
                 Wine(),
                 Wine(),
                 Wine(),
                 Wine(),
                 Wine(),
                 Wine(),
                };

std::vector<Wine> History;

//SCREEN 1 BASE*********************************************************************************
void splashScreen()
{
  SimbleeForMobile.beginScreen(WHITE, PORTRAIT);
  SimbleeForMobile.drawRect(0, 0, 320, 570, GE_RED);
  SimbleeForMobile.drawText(20, 200, "LOADING", WHITE, 50);
  SimbleeForMobile.drawText(20, 250, "INTERFACE", WHITE, 50);
  SimbleeForMobile.imageSource(1, JPG, WineSplashScreen_jpg, WineSplashScreen_jpg_len);
  SimbleeForMobile.drawImage(1, 0, 0);
  SimbleeForMobile.endScreen();
}
//************************************************************************************************

//SCREEN 2 BASE*********************************************************************************
void inventoryScreen()
{
  SimbleeForMobile.beginScreen(WHITE, PORTRAIT);

  //BACKGROUND
  screen2Background = SimbleeForMobile.drawRect(0, 0, 320, 480, GE_RED);
  SimbleeForMobile.drawRect(0, 60, 280,  360, GE_LIGHTGRAY);
  SimbleeForMobile.drawRect(0, 61, 280, 70, WHITE);
  SimbleeForMobile.drawRect(0, 133, 280, 70, WHITE);
  SimbleeForMobile.drawRect(0, 205, 280, 70, WHITE);
  SimbleeForMobile.drawRect(0, 277, 280, 70, WHITE);
  SimbleeForMobile.drawRect(0, 349, 280, 70, WHITE);

  SimbleeForMobile.imageSource(2, PNG, WineBottleImage_png, WineBottleImage_png_len);
  SimbleeForMobile.drawImage(2, 25, 66);
  SimbleeForMobile.drawImage(2, 25, 138);
  SimbleeForMobile.drawImage(2, 25, 210);
  SimbleeForMobile.drawImage(2, 25, 282);
  SimbleeForMobile.drawImage(2, 25, 354);

  //TITLE
  screen2Title = SimbleeForMobile.drawText(90, 25, "Wine Inventory", WHITE, 25);

  int boxY[5] = {64, 136, 208, 280, 352};

  int overlayY[5] = {61, 133, 205, 277, 349};

  for (int i = 0; i < 5; i++) {
    //VINEYARD
    VineyardUI[i] = SimbleeForMobile.drawText(70, boxY[i], "", GE_LIGHTGRAY, 12);
    //WINE NAME 1
    WineName1UI[i] = SimbleeForMobile.drawText(70, boxY[i] + 12, "", BLACK, 14);
    //WINE NAME 2
    WineName2UI[i] = SimbleeForMobile.drawText(70, boxY[i] + 25, "", BLACK, 14);
    //COUNTRY
    CountryUI[i] = SimbleeForMobile.drawText(70, boxY[i] + 39, "", GE_LIGHTGRAY, 11);
    //PRICE
    PriceUI[i] = SimbleeForMobile.drawText(70, boxY[i] + 50 , "", GE_LIGHTGRAY, 10);
    //LOCATION TITLE
    SimbleeForMobile.drawText(220, boxY[i], "LOCATION", GE_LIGHTGRAY, 12);
    //LOCATION
    LocationUI[i] = SimbleeForMobile.drawText(232, boxY[i] + 15, "", BLACK, 30);
    //INVISBLE OVERLAY
    OverlayUI[i] = SimbleeForMobile.drawRect(0, overlayY[i], 280, 80, rgba(0, 0, 0, 255));
    SimbleeForMobile.setEvents(OverlayUI[i], EVENT_PRESS);
  }

  //PAGE SELECTION
  pageValue = SimbleeForMobile.drawText(295, 230, 1, WHITE, 20);
  SimbleeForMobile.drawText(297, 70, "^", WHITE, 20);
  previousPage = SimbleeForMobile.drawButton(280, 60, 40, "", WHITE, 2);
  SimbleeForMobile.setEvents(previousPage, EVENT_PRESS);
  SimbleeForMobile.drawText(297, 390, "v", WHITE, 12);
  nextPage = SimbleeForMobile.drawButton(280, 380, 40, "", WHITE, 2);
  SimbleeForMobile.setEvents(nextPage, EVENT_PRESS);

  char* segmentNames[] = {"Inventory", "History"};
  screen2Segment = SimbleeForMobile.drawSegment(80, 435, 160, segmentNames, countof(segmentNames), WHITE);

  //BLUR SCREEN
  insertScreen1 = SimbleeForMobile.drawRect(0, 0, 320, 570, rgba(155, 155, 155, 50));
  SimbleeForMobile.setVisible(insertScreen1, false);

  //POP BOX
  insertScreen2 = SimbleeForMobile.drawRect(20, 200, 280, 150, GE_RED);
  SimbleeForMobile.setVisible(insertScreen2, false);

  //TEXT
  insertScreen3 = SimbleeForMobile.drawText(60, 220, "Please insert wine into rack", WHITE, 18);
  SimbleeForMobile.setVisible(insertScreen3, false);
  insertScreen4 = SimbleeForMobile.drawText(200, 260, 1, WHITE, 22);
  SimbleeForMobile.setVisible(insertScreen4, false);
  insertScreen5 = SimbleeForMobile.drawText(110, 240, 1, WHITE, 22);
  SimbleeForMobile.setVisible(insertScreen5, false);
  insertScreen6 = SimbleeForMobile.drawText(60, 240, "Wine", WHITE, 22);
  SimbleeForMobile.setVisible(insertScreen6, false);
  insertScreen7 = SimbleeForMobile.drawText(60, 260, "Placed in rack #", WHITE, 22);
  SimbleeForMobile.setVisible(insertScreen7, false);

  addPopUpDetectedText = SimbleeForMobile.drawText(60, 220, "Bottle Detected", WHITE, 20);
  SimbleeForMobile.setVisible(addPopUpDetectedText, false);
  addPopUpRackText = SimbleeForMobile.drawText(60, 240, "In Rack#:", WHITE, 20);
  SimbleeForMobile.setVisible(addPopUpRackText, false);
  addPopUpRackNumber = SimbleeForMobile.drawText(140, 240, "A1", WHITE, 20);
  SimbleeForMobile.setVisible(addPopUpRackNumber, false);
  addPopUpSelectText = SimbleeForMobile.drawText(60, 260, "Please Select Wine", WHITE, 20);
  SimbleeForMobile.setVisible(addPopUpSelectText, false);

  removePopUpDetectedText = SimbleeForMobile.drawText(60, 220, "Bottle Removed", WHITE, 20);
  SimbleeForMobile.setVisible(removePopUpDetectedText, false);
  removePopUpRackText = SimbleeForMobile.drawText(60, 240, "In Rack#:", WHITE, 20);
  SimbleeForMobile.setVisible(removePopUpRackText, false);
  removePopUpRackNumber = SimbleeForMobile.drawText(140, 240, "A1", WHITE, 20);
  SimbleeForMobile.setVisible(removePopUpRackNumber, false);

  SimbleeForMobile.endScreen();

  SimbleeForMobile.updateValue(screen2Segment, 0);
}

//************************************************************************************************

//SCREEN 5 BASE*********************************************************************************
void addScreen()
{
  SimbleeForMobile.beginScreen();

  //BACKGROUND
  SimbleeForMobile.drawRect(0, 0, 320, 480, GE_RED);
  SimbleeForMobile.drawRect(0, 60, 320,  180, GE_LIGHTGRAY);
  SimbleeForMobile.drawImage(3, 0, 60);
  SimbleeForMobile.drawImage(4, 0, 60);
  SimbleeForMobile.drawImage(5, 0, 60);
  SimbleeForMobile.drawImage(6, 0, 60);
  //SimbleeForMobile.drawImage(7,0,60);

  SimbleeForMobile.drawRect(0, 240, 320, 180, WHITE);

  VineyardUISingle = SimbleeForMobile.drawText(30, 245, "Cherry Blossom", GE_LIGHTGRAY, 16);
  WineName1UISingle = SimbleeForMobile.drawText(30, 262, "Central Otago", BLACK, 18);
  WineName2UISingle = SimbleeForMobile.drawText(30, 280, "2014", BLACK, 18);
  CountryUISingle = SimbleeForMobile.drawText(30, 300, "Pinot Noir", GE_LIGHTGRAY, 16);
  PriceUISingle = SimbleeForMobile.drawText(200, 300, "$20.00", GE_LIGHTGRAY, 16);
  //LOCATION TITLE
  SimbleeForMobile.drawText(250, 245, "LOCATION", GE_LIGHTGRAY, 12);
  //LOCATION
  LocationUISingle = SimbleeForMobile.drawText(262, 260, "", BLACK, 30);

  //INFO
  addButton = SimbleeForMobile.drawButton(40, 375, 240, "ADD TO CHILLER", BLACK, 2);
  SimbleeForMobile.setEvents(addButton, EVENT_PRESS);
  SimbleeForMobile.setVisible(addButton, false);
  removeButton = SimbleeForMobile.drawButton(40, 375, 240, "REMOVE FROM CHILLER", RED, 2);
  SimbleeForMobile.setEvents(removeButton, EVENT_PRESS);
  SimbleeForMobile.setVisible(removeButton, false);
  returnButton = SimbleeForMobile.drawButton(40, 435, 240, "RETURN TO INVENTORY", BLACK, 1);
  SimbleeForMobile.setEvents(returnButton, EVENT_PRESS);

  //SHOWWINESCREEN
  insertScreen1 = SimbleeForMobile.drawRect(0, 0, 320, 570, rgba(155, 155, 155, 50));
  SimbleeForMobile.setVisible(insertScreen1, false);
  insertScreen2 = SimbleeForMobile.drawRect(20, 200, 280, 150, GE_RED);
  SimbleeForMobile.setVisible(insertScreen2, false);
  insertScreen3 = SimbleeForMobile.drawText(60, 220, "Please insert wine into rack", WHITE, 18);
  SimbleeForMobile.setVisible(insertScreen3, false);
  insertScreen4 = SimbleeForMobile.drawText(200, 260, 1, WHITE, 22);
  SimbleeForMobile.setVisible(insertScreen4, false);
  insertScreen5 = SimbleeForMobile.drawText(60, 240, 1, WHITE, 22);
  SimbleeForMobile.setVisible(insertScreen5, false);
  insertScreen6 = SimbleeForMobile.drawText(60, 240, "", WHITE, 22);
  SimbleeForMobile.setVisible(insertScreen6, false);
  insertScreen7 = SimbleeForMobile.drawText(60, 260, "Placed in rack #", WHITE, 22);
  SimbleeForMobile.setVisible(insertScreen7, false);

  removeScreen1 = SimbleeForMobile.drawRect(0, 0, 320, 570, rgba(155, 155, 155, 50));
  SimbleeForMobile.setVisible(removeScreen1, false);
  removeScreen2 = SimbleeForMobile.drawRect(20, 200, 280, 150, GE_RED);
  SimbleeForMobile.setVisible(removeScreen2, false);
  removeScreen3 = SimbleeForMobile.drawText(60, 220, "Please remove wine from rack:", WHITE, 18);
  SimbleeForMobile.setVisible(removeScreen3, false);
  removeScreen4 = SimbleeForMobile.drawText(150, 260, 1, WHITE, 40);
  SimbleeForMobile.setVisible(removeScreen4, false);

  SimbleeForMobile.endScreen();
  int wineChosen = clickedOverlay + ((winePage - 1) * 5);
  if (wineChosen == 0) {
    SimbleeForMobile.imageSource(3, JPG, ColumbiaCrest2_jpg, ColumbiaCrest2_jpg_len);
  }
  else if (wineChosen == 1) {
    SimbleeForMobile.imageSource(4, JPG, Carmen1_jpg, Carmen1_jpg_len);
  }
  else if (wineChosen == 2) {
    SimbleeForMobile.imageSource(5, JPG, CoppolaShiraz1_jpg, CoppolaShiraz1_jpg_len);
  }
  else if (wineChosen == 3) {
    SimbleeForMobile.imageSource(6, JPG, LaTercera1_jpg, LaTercera1_jpg_len);
  }
  //  else if (wineChosen == 4) {
  //    //SimbleeForMobile.imageSource(7, JPG, Martini2_jpg, Martini2_jpg_len);
  //  }
  
    
}


//************************************************************************************************

void historyScreen() {
  SimbleeForMobile.beginScreen(WHITE, PORTRAIT);

  //BACKGROUND
  screen2Background = SimbleeForMobile.drawRect(0, 0, 320, 480, GE_RED);
  SimbleeForMobile.drawRect(0, 60, 280,  360, GE_LIGHTGRAY);
  SimbleeForMobile.drawRect(0, 61, 280, 70, WHITE);
  SimbleeForMobile.drawRect(0, 133, 280, 70, WHITE);
  SimbleeForMobile.drawRect(0, 205, 280, 70, WHITE);
  SimbleeForMobile.drawRect(0, 277, 280, 70, WHITE);
  SimbleeForMobile.drawRect(0, 349, 280, 70, WHITE);

  //IMAGES
  //SimbleeForMobile.imageSource(ColumbiaCrest2,JPG,ColumbiaCrest2_jpg,ColumbiaCrest2_jpg_len);
  //SimbleeForMobile.drawImage(ColumbiaCrest2,10,66);
  SimbleeForMobile.drawRect(10, 66, 50, 60, BLACK);
  //SimbleeForMobile.imageSource(Carmen2,JPG,Carmen2_jpg,Carmen2_jpg_len);
  //SimbleeForMobile.drawImage(Carmen2,10,138);
  SimbleeForMobile.drawRect(10, 138, 50, 60, BLACK);
  //SimbleeForMobile.imageSource(CoppolaShiraz2,JPG,CoppolaShiraz2_jpg,CoppolaShiraz2_jpg_len);
  //SimbleeForMobile.drawImage(CoppolaShiraz2,10,210);
  SimbleeForMobile.drawRect(10, 210, 50, 60, BLACK);
  //SimbleeForMobile.imageSource(LaTercera2,JPG,LaTercera2_jpg,LaTercera2_jpg_len);
  //SimbleeForMobile.drawImage(LaTercera2,10,282);
  SimbleeForMobile.drawRect(10, 282, 50, 60, BLACK);
  //SimbleeForMobile.imageSource(Martini1,JPG,Martini1_jpg,Martini1_jpg_len);
  //SimbleeForMobile.drawImage(Martini1,10,354);
  SimbleeForMobile.drawRect(10, 354, 50, 60, BLACK);

  SimbleeForMobile.imageSource(7, PNG, stars_png, stars_png_len);


  //TITLE
  screen2Title = SimbleeForMobile.drawText(120, 25, "History", WHITE, 25);

  int boxY[5] = {64, 136, 208, 280, 352};

  int overlayY[5] = {61, 133, 205, 277, 349};



  for (int i = 0; i < 5; i++) {
    //VINEYARD
    HistoryVineyardUI[i] = SimbleeForMobile.drawText(70, boxY[i], "", GE_LIGHTGRAY, 12);
    //WINE NAME 1
    HistoryWineName1UI[i] = SimbleeForMobile.drawText(70, boxY[i] + 12, "", BLACK, 14);
    //WINE NAME 2
    HistoryWineName2UI[i] = SimbleeForMobile.drawText(70, boxY[i] + 25, "", BLACK, 14);
    //COUNTRY
    HistoryCountryUI[i] = SimbleeForMobile.drawText(70, boxY[i] + 39, "", GE_LIGHTGRAY, 11);
    //WHATWEDIDWITHTHEBOTTLE
    WWDWTB[i] = SimbleeForMobile.drawText(70, boxY[i] + 50 , "", GE_LIGHTGRAY, 10);
    //INVISBLE OVERLAY



    starsRect[i] = SimbleeForMobile.drawRect(190, boxY[i], 1, 19, YELLOW);
    SimbleeForMobile.setVisible(starsRect[i], false);
    starsImg[i] = SimbleeForMobile.drawImage(7, 190, boxY[i]);
    SimbleeForMobile.setVisible(starsImg[i], false);
    HistoryOverlayUI[i] = SimbleeForMobile.drawRect(0, overlayY[i], 280, 80, rgba(0, 0, 0, 255));
    SimbleeForMobile.setEvents(HistoryOverlayUI[i], EVENT_PRESS);
  }

  //PAGE SELECTION
  screen6pageValue = SimbleeForMobile.drawText(295, 230, 1, WHITE, 20);
  SimbleeForMobile.drawText(297, 70, "^", WHITE, 20);
  screen6previousPage = SimbleeForMobile.drawButton(280, 60, 40, "", WHITE, 2);
  SimbleeForMobile.setEvents(screen6previousPage, EVENT_PRESS);
  SimbleeForMobile.drawText(297, 390, "v", WHITE, 12);
  screen6nextPage = SimbleeForMobile.drawButton(280, 380, 40, "", WHITE, 2);
  SimbleeForMobile.setEvents(screen6nextPage, EVENT_PRESS);

  char* segmentNames[] = {"Inventory", "History"};
  screen6Segment = SimbleeForMobile.drawSegment(80, 435, 160, segmentNames, countof(segmentNames), WHITE);

  //BLUR SCREEN
  insertScreen1 = SimbleeForMobile.drawRect(0, 0, 320, 570, rgba(155, 155, 155, 50));
  SimbleeForMobile.setVisible(insertScreen1, false);

  //POP BOX
  insertScreen2 = SimbleeForMobile.drawRect(20, 200, 280, 150, GE_RED);
  SimbleeForMobile.setVisible(insertScreen2, false);

  //TEXT
  insertScreen3 = SimbleeForMobile.drawText(60, 220, "Please insert wine into rack", WHITE, 18);
  SimbleeForMobile.setVisible(insertScreen3, false);
  insertScreen4 = SimbleeForMobile.drawText(200, 260, 1, WHITE, 22);
  SimbleeForMobile.setVisible(insertScreen4, false);
  insertScreen5 = SimbleeForMobile.drawText(110, 240, 1, WHITE, 22);
  SimbleeForMobile.setVisible(insertScreen5, false);
  insertScreen6 = SimbleeForMobile.drawText(60, 240, "Wine", WHITE, 22);
  SimbleeForMobile.setVisible(insertScreen6, false);
  insertScreen7 = SimbleeForMobile.drawText(60, 260, "Placed in rack #", WHITE, 22);
  SimbleeForMobile.setVisible(insertScreen7, false);

  addPopUpDetectedText = SimbleeForMobile.drawText(60, 220, "Bottle Detected", WHITE, 20);
  SimbleeForMobile.setVisible(addPopUpDetectedText, false);
  addPopUpRackText = SimbleeForMobile.drawText(60, 240, "In Rack#:", WHITE, 20);
  SimbleeForMobile.setVisible(addPopUpRackText, false);
  addPopUpRackNumber = SimbleeForMobile.drawText(140, 240, "A1", WHITE, 20);
  SimbleeForMobile.setVisible(addPopUpRackNumber, false);
  addPopUpSelectText = SimbleeForMobile.drawText(60, 260, "Please Select Wine", WHITE, 20);
  SimbleeForMobile.setVisible(addPopUpSelectText, false);

  removePopUpDetectedText = SimbleeForMobile.drawText(60, 220, "Bottle Removed", WHITE, 20);
  SimbleeForMobile.setVisible(removePopUpDetectedText, false);
  removePopUpRackText = SimbleeForMobile.drawText(60, 240, "In Rack#:", WHITE, 20);
  SimbleeForMobile.setVisible(removePopUpRackText, false);
  removePopUpRackNumber = SimbleeForMobile.drawText(140, 240, "A1", WHITE, 20);
  SimbleeForMobile.setVisible(removePopUpRackNumber, false);

  SimbleeForMobile.endScreen();

  SimbleeForMobile.updateValue(screen6Segment, 1);
}

void historydetailScreen() {
  SimbleeForMobile.beginScreen();

  //BACKGROUND
  SimbleeForMobile.drawRect(0, 0, 320, 480, GE_RED);
  SimbleeForMobile.drawImage(3, 0, 60);
  SimbleeForMobile.drawImage(4, 0, 60);
  SimbleeForMobile.drawImage(5, 0, 60);
  SimbleeForMobile.drawImage(6, 0, 60);
  //SimbleeForMobile.drawImage(7,0,60);
  SimbleeForMobile.imageSource(7, PNG, stars_png, stars_png_len);

  //SimbleeForMobile.drawRect(0, 60, 320,  180, GE_LIGHTGRAY);
  SimbleeForMobile.drawRect(0, 240, 320, 180, WHITE);


  HistoryVineyardUISingle = SimbleeForMobile.drawText(30, 245, "Cherry Blossom", GE_LIGHTGRAY, 16);
  HistoryWineName1UISingle = SimbleeForMobile.drawText(30, 262, "Central Otago", BLACK, 18);
  HistoryWineName2UISingle = SimbleeForMobile.drawText(30, 280, "2014", BLACK, 18);
  HistoryCountryUISingle = SimbleeForMobile.drawText(30, 300, "Pinot Noir", GE_LIGHTGRAY, 16);
  HistoryPriceUISingle = SimbleeForMobile.drawText(200, 300, "$20.00", GE_LIGHTGRAY, 16);
  starsRectUISingle = SimbleeForMobile.drawRect(205, 245, 1, 19, YELLOW);
  SimbleeForMobile.drawImage(7, 205, 245);

  for (int i = 0; i < 5; i++) {
    rate[i] = SimbleeForMobile.drawRect(205 + (i * 16), 245, 16, 19, rgba(0, 0, 0, 255));
    SimbleeForMobile.setEvents(rate[i], EVENT_PRESS);
  }

  //  //LOCATION TITLE
  //  SimbleeForMobile.drawText(250, 245, "LOCATION", GE_LIGHTGRAY, 12);
  //  //LOCATION
  //  LocationUISingle = SimbleeForMobile.drawText(262, 260, "", BLACK, 30);


  //INFO
  consumeButton = SimbleeForMobile.drawButton(30, 330, 120, "CONSUMED", GRAY, 2);
  SimbleeForMobile.setEvents(consumeButton, EVENT_PRESS);
  giftButton = SimbleeForMobile.drawButton(170, 330, 120, "GIFTED", GRAY, 2);
  SimbleeForMobile.setEvents(giftButton, EVENT_PRESS);
  deleteButton = SimbleeForMobile.drawButton(40, 375, 240, "DELETE FROM HISTORY", RED, 2);
  SimbleeForMobile.setEvents(deleteButton, EVENT_PRESS);
  returnButton = SimbleeForMobile.drawButton(40, 435, 240, "RETURN TO HISTORY", BLACK, 1);
  SimbleeForMobile.setEvents(returnButton, EVENT_PRESS);

  SimbleeForMobile.endScreen();
  int wineChosen = clickedOverlay + ((winePage - 1) * 5);
  if (wineChosen == 0) {
    SimbleeForMobile.imageSource(3, JPG, ColumbiaCrest2_jpg, ColumbiaCrest2_jpg_len);
  }
  else if (wineChosen == 1) {
    SimbleeForMobile.imageSource(4, JPG, Carmen1_jpg, Carmen1_jpg_len);
  }
  else if (wineChosen == 2) {
    SimbleeForMobile.imageSource(5, JPG, CoppolaShiraz1_jpg, CoppolaShiraz1_jpg_len);
  }
  else if (wineChosen == 3) {
    SimbleeForMobile.imageSource(6, JPG, LaTercera1_jpg, LaTercera1_jpg_len);
  }
  //  else if (wineChosen == 4) {
  //    //SimbleeForMobile.imageSource(7, JPG, Martini2_jpg, Martini2_jpg_len);
  //  }
  else
    SimbleeForMobile.drawRect(0, 60, 320,  180, GE_LIGHTGRAY);
}

void checkButtonState(Slot toButton, int cellID) {
  if (toButton.isPressed()) {
    if (initialized == false) {
      currentButtonState[cellID] = true;
    }
    else {
      nextButtonState[cellID] = true;
    }
  }
  else if (toButton.isReleased()) {
    if (initialized == false) {
      currentButtonState[cellID] = false;
    }
    else {
      nextButtonState[cellID] = false;
    }
  }
}

void checkAllButtons() {
  for (int i = 0; i < 25; i++) {
    checkButtonState(Slots[i], i);
  }
  initialized = true;
}

//  I've just used this function as an example to demonstrate the classes I've implemented.
//    Remember: onPressed() returns true for a pressed button.
//      Progressive presses will return false.
void checkAdd() {
  for (int i = 0; i < 25; i++) {
    if (currentButtonState[i] < nextButtonState[i]) {
      addDetected = i;
      currentButtonState[i] = nextButtonState[i];
      updatePage = true;
      addMode = 1;
      leds[addDetected] = color;
    }
  }
}

void checkRemove() {
  for (int i = 0; i < 25; i++) {
    if (currentButtonState[i] > nextButtonState[i]) {
      removeDetected = i;
      currentButtonState[i] = nextButtonState[i];
      updatePage = true;
    }
  }
}

void compareStates() {
  count = millis();
  while (switchDetected == -1 && millis() - count < INSERT_WINE_TIMEOUT) {
    checkAllButtons();
    for (int i = 0; i < 25; i++) {
      if (currentButtonState[i] < nextButtonState[i]) {
        switchDetected = i;
        currentButtonState[i] = nextButtonState[i];
      }
    }
  }
  if (switchDetected == -1) {
    SimbleeForMobile.setVisible(insertScreen1, false);
    SimbleeForMobile.setVisible(insertScreen2, false);
    SimbleeForMobile.setVisible(insertScreen3, false);
    FastLED.clear();
    FastLED.show();
  }
  else {
    leds[switchDetected] = color;
    FastLED.show();
  }
}

void showWineInsertScreen() {
  SimbleeForMobile.setVisible(insertScreen1, true);
  SimbleeForMobile.setVisible(insertScreen2, true);
  SimbleeForMobile.setVisible(insertScreen3, true);
  checkAllButtons();
  compareStates();
}

void removeBottle()
{
  SimbleeForMobile.setVisible(removeScreen1, true);
  SimbleeForMobile.setVisible(removeScreen2, true);
  SimbleeForMobile.setVisible(removeScreen3, true);
  SimbleeForMobile.updateText(removeScreen4, ChillerSlot[wine[clickedOverlay + ((winePage - 1) * 5)].getIndex()]);
  SimbleeForMobile.setVisible(removeScreen4, true);
  for (int i = 0; i < 25; i++) {
    if (ChillerSlot[i] == ChillerSlot[wine[clickedOverlay + ((winePage - 1) * 5)].getIndex()]) {
      leds[i] = color2;
      break;
    }
  }
  FastLED.show();
  delay(100); // give time for the led to turn on in correct spot
  while (nextButtonState[wine[clickedOverlay + ((winePage - 1) * 5)].getIndex()] > 0)
  {
    checkAllButtons();
  }
  if (wine[clickedOverlay + ((winePage - 1) * 5)].getIndex() != -1) {
    History.push_back(wine[clickedOverlay + ((winePage - 1) * 5)]);
  }
  wine[clickedOverlay + ((winePage - 1) * 5)].setIndex(-1);

  SimbleeForMobile.setVisible(removeScreen4, false);
  SimbleeForMobile.setVisible(removeScreen1, false);
  SimbleeForMobile.setVisible(removeScreen2, false);
  SimbleeForMobile.setVisible(removeScreen3, false);
  SimbleeForMobile.showScreen(2);
}
// Inializing vector of Slots for TESTBOX units.
//   Should not have problems with corresponding
//   switch and led positions.
void setup()
{
  //Serial.begin(9600);

  for (int i = 0; i < 25; i++) {
    if (i == 1) {
      Slot tempSlotEdgeOne = Slot(30, leds[1]);
      Slots.push_back(tempSlotEdgeOne);
    }
    else if (i > 4) {
      Slot tempSlotEdgeOne = Slot(i + 1, leds[i]);
      Slots.push_back(tempSlotEdgeOne);
    }
    else  {
      Slot tempSlotEdgeOne = Slot(i, leds[i]);
      Slots.push_back(tempSlotEdgeOne);
    }

  }
  FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, NUM_LEDS);
  FastLED.clear();
  FastLED.show();
  //checkAllButtons();
  SimbleeForMobile.deviceName = "Wine";
  SimbleeForMobile.advertisementData = "Chiller";
  SimbleeForMobile.domain = "FirstBuild4.simblee.com";
  SimbleeForMobile.begin();
}

void loop()
{
  checkAllButtons();
  checkAdd();
  checkRemove();
  if (disconnected == true)
  {
    SimbleeForMobile.showScreen(2);
    disconnected = false;
  }
  if (SimbleeForMobile.updatable)
  {
    if (SimbleeForMobile.screen == 2) {
      if (updatePage == true)
      {

        SimbleeForMobile.updateValue(pageValue, winePage);
        if (addDetected >= 0) {
          FastLED.show();
          SimbleeForMobile.updateText(addPopUpRackNumber, ChillerSlot[addDetected]);
          SimbleeForMobile.setVisible(insertScreen1, true);
          SimbleeForMobile.setVisible(insertScreen2, true);
          SimbleeForMobile.setVisible(addPopUpDetectedText, true);
          SimbleeForMobile.setVisible(addPopUpRackText, true);
          SimbleeForMobile.setVisible(addPopUpRackNumber, true);
          SimbleeForMobile.setVisible(addPopUpSelectText, true);
          delay(3000);
          SimbleeForMobile.setVisible(insertScreen1, false);
          SimbleeForMobile.setVisible(insertScreen2, false);
          SimbleeForMobile.setVisible(addPopUpDetectedText, false);
          SimbleeForMobile.setVisible(addPopUpRackText, false);
          SimbleeForMobile.setVisible(addPopUpRackNumber, false);
          SimbleeForMobile.setVisible(addPopUpSelectText, false);
          SimbleeForMobile.updateColor(screen2Background, BLUE);
          SimbleeForMobile.updateText(screen2Title, "Select Wine");
          FastLED.clear();
          FastLED.show();
        }
        if (removeDetected >= 0) {
          int bottle;
          for (int i = 0; i < 25; i++) {
            if (wine[i].getIndex() == removeDetected) {
              bottle = i;
            }
          }
          if (wine[bottle].getIndex() != -1) {
            History.push_back(wine[bottle]);
          }

          leds[removeDetected] = color2;
          FastLED.show();
          delay(10);//give time for led to turn on
          SimbleeForMobile.updateText(removePopUpRackNumber, ChillerSlot[removeDetected]);
          SimbleeForMobile.setVisible(insertScreen1, true);
          SimbleeForMobile.setVisible(insertScreen2, true);
          SimbleeForMobile.setVisible(removePopUpDetectedText, true);
          SimbleeForMobile.setVisible(removePopUpRackText, true);
          SimbleeForMobile.setVisible(removePopUpRackNumber, true);
          wine[bottle].setIndex(-1);
          delay(3000);
          SimbleeForMobile.setVisible(insertScreen1, false);
          SimbleeForMobile.setVisible(insertScreen2, false);
          SimbleeForMobile.setVisible(removePopUpDetectedText, false);
          SimbleeForMobile.setVisible(removePopUpRackText, false);
          SimbleeForMobile.setVisible(removePopUpRackNumber, false);
          FastLED.clear();
          FastLED.show();
          removeDetected = -1;
        }
        for (int i = 0; i < 5; i++) {
          SimbleeForMobile.updateText(VineyardUI[i], wine[i + ((winePage - 1) * 5)].getVineyard());
          SimbleeForMobile.updateText(WineName1UI[i], wine[i + ((winePage - 1) * 5)].getName());
          SimbleeForMobile.updateText(WineName2UI[i], wine[i + ((winePage - 1) * 5)].getYear());
          SimbleeForMobile.updateText(CountryUI[i], wine[i + ((winePage - 1) * 5)].getwineType());
          SimbleeForMobile.updateText(PriceUI[i], wine[i + ((winePage - 1) * 5)].getPrice());
          if (wine[i + ((winePage - 1) * 5)].getIndex() == -1) {
            SimbleeForMobile.updateColor(LocationUI[i], BLACK);
            SimbleeForMobile.updateText(LocationUI[i], "");
          }
          else if (wine[i + ((winePage - 1) * 5)].getIndex() >= 0) {
            SimbleeForMobile.updateColor(LocationUI[i], BLUE);
            SimbleeForMobile.updateText(LocationUI[i], ChillerSlot[wine[i + ((winePage - 1) * 5)].getIndex()]);
          }
        }
        updatePage = false;
      }
    }
    //*******************************************************************************************************

    //UPDATES FOR SCREEN 5 (INFO SCREEN)***********************************************
    else if (SimbleeForMobile.screen == 5)
    {
      SimbleeForMobile.updateText(VineyardUISingle, wine[clickedOverlay + ((winePage - 1) * 5)].getVineyard());
      SimbleeForMobile.updateText(WineName1UISingle, wine[clickedOverlay + ((winePage - 1) * 5)].getName());
      SimbleeForMobile.updateText(WineName2UISingle, wine[clickedOverlay + ((winePage - 1) * 5)].getYear());
      SimbleeForMobile.updateText(CountryUISingle, wine[clickedOverlay + ((winePage - 1) * 5)].getwineType());
      SimbleeForMobile.updateText(PriceUISingle, wine[clickedOverlay + ((winePage - 1) * 5)].getPrice());
      if (wine[clickedOverlay + ((winePage - 1) * 5)].getIndex() == -1) {
        SimbleeForMobile.updateColor(LocationUISingle, BLACK);
        SimbleeForMobile.updateText(LocationUISingle, "");
        SimbleeForMobile.setVisible(removeButton, false);
        SimbleeForMobile.setVisible(addButton, true);
      }
      else if (wine[clickedOverlay + ((winePage - 1) * 5)].getIndex() >= 0) {
        SimbleeForMobile.updateColor(LocationUISingle, BLUE);
        SimbleeForMobile.updateText(LocationUISingle, ChillerSlot[wine[clickedOverlay + ((winePage - 1) * 5)].getIndex()]);
        SimbleeForMobile.setVisible(addButton, false);
        SimbleeForMobile.setVisible(removeButton, true);
      }
      if (switchDetected >= 0)
      {
        SimbleeForMobile.updateText(insertScreen5, wine[clickedOverlay + ((winePage - 1) * 5)].getName());
        SimbleeForMobile.updateText(insertScreen4, ChillerSlot[switchDetected]);
        wine[clickedOverlay + ((winePage - 1) * 5)].setIndex(switchDetected);
        SimbleeForMobile.setVisible(insertScreen4, true);
        SimbleeForMobile.setVisible(insertScreen5, true);
        SimbleeForMobile.setVisible(insertScreen6, true);
        SimbleeForMobile.setVisible(insertScreen7, true);
        delay(3000);
        SimbleeForMobile.setVisible(insertScreen1, false);
        SimbleeForMobile.setVisible(insertScreen2, false);
        SimbleeForMobile.setVisible(insertScreen3, false);
        SimbleeForMobile.setVisible(insertScreen4, false);
        SimbleeForMobile.setVisible(insertScreen5, false);
        SimbleeForMobile.setVisible(insertScreen6, false);
        SimbleeForMobile.setVisible(insertScreen7, false);
        initialized = false;
        FastLED.clear();
        FastLED.show();
        switchDetected = -1;
        SimbleeForMobile.showScreen(2);
      }
    }

    //HISTORY LIST SCREEN UPDATES
    else if (SimbleeForMobile.screen == 6)
    {
      if (updatePage == true)
      {
        SimbleeForMobile.updateValue(screen6pageValue, historyPage);
        Serial.println(historyPage);
        int tempHistorysize = History.size();
        Serial.println(History.size());
        tempHistorysize -= ((historyPage - 1) * 5);
        for (int i = 0; i < constrain(tempHistorysize, 0, 5); i++) {
          Serial.println(History[i + ((historyPage - 1) * 5)].getName());
          SimbleeForMobile.updateText(HistoryVineyardUI[i], History[i + ((historyPage - 1) * 5)].getVineyard());
          SimbleeForMobile.updateText(HistoryWineName1UI[i], History[i + ((historyPage - 1) * 5)].getName());
          SimbleeForMobile.updateText(HistoryWineName2UI[i], History[i + ((historyPage - 1) * 5)].getYear());
          SimbleeForMobile.updateText(HistoryCountryUI[i], History[i + ((historyPage - 1) * 5)].getwineType());
          
          if (History[i + ((historyPage - 1) * 5)].getRating() == 1) {
            SimbleeForMobile.updateW(starsRect[i], 16);
          }
          else if (History[i + ((historyPage - 1) * 5)].getRating() == 2) {
            SimbleeForMobile.updateW(starsRect[i], 32);
          }
          else if (History[i + ((historyPage - 1) * 5)].getRating() == 3) {
            SimbleeForMobile.updateW(starsRect[i], 48);
          }
          else if (History[i + ((historyPage - 1) * 5)].getRating() == 4) {
            SimbleeForMobile.updateW(starsRect[i], 64);
          }
          else if (History[i + ((historyPage - 1) * 5)].getRating() == 5) {
            SimbleeForMobile.updateW(starsRect[i], 80);
          }
          else
            SimbleeForMobile.updateW(starsRect[i],1);
          
          SimbleeForMobile.setVisible(starsRect[i],true);
          SimbleeForMobile.setVisible(starsImg[i], true);
          
          if (History[i + ((historyPage - 1) * 5)].getIndex() == -1) {
            SimbleeForMobile.updateText(WWDWTB[i], "");
          }
          else if (History[i + ((historyPage - 1) * 5)].getIndex() == -2) {
            SimbleeForMobile.updateText(WWDWTB[i], "GIFTED");
          }
          else if (History[i + ((historyPage - 1) * 5)].getIndex() == -3) {
            SimbleeForMobile.updateText(WWDWTB[i], "CONSUMED");
          }
          else
            SimbleeForMobile.updateText(WWDWTB[i],"");
          SimbleeForMobile.setVisible(HistoryOverlayUI[i], true);

          
        }
        for (int i = 4; i >= constrain(tempHistorysize, 0, 5); i--) {
          SimbleeForMobile.updateText(HistoryVineyardUI[i], "");
          SimbleeForMobile.updateText(HistoryWineName1UI[i], "");
          SimbleeForMobile.updateText(HistoryWineName2UI[i], "");
          SimbleeForMobile.updateText(HistoryCountryUI[i], "");
          SimbleeForMobile.setVisible(starsRect[i],false);
          SimbleeForMobile.setVisible(starsImg[i], false);
          SimbleeForMobile.updateText(WWDWTB[i], "");
          SimbleeForMobile.setVisible(HistoryOverlayUI[i], false);
        }
        updatePage = false;
      }
    }

    //HISTORY DETAIL SCREEN
    else if (SimbleeForMobile.screen = 7) {
      SimbleeForMobile.updateText(HistoryVineyardUISingle, History[clickedOverlay2 + ((historyPage - 1) * 5)].getVineyard());
      SimbleeForMobile.updateText(HistoryWineName1UISingle, History[clickedOverlay2 + ((historyPage - 1) * 5)].getName());
      SimbleeForMobile.updateText(HistoryWineName2UISingle, History[clickedOverlay2 + ((historyPage - 1) * 5)].getYear());
      SimbleeForMobile.updateText(HistoryCountryUISingle, History[clickedOverlay2 + ((historyPage - 1) * 5)].getwineType());
      SimbleeForMobile.updateText(HistoryPriceUISingle, History[clickedOverlay2 + ((historyPage - 1) * 5)].getPrice());

      if (History[clickedOverlay2 + ((historyPage - 1) * 5)].getIndex() == -1) {
        SimbleeForMobile.updateColor(consumeButton, GRAY);
        SimbleeForMobile.updateColor(giftButton, GRAY);
      }
      else if (History[clickedOverlay2 + ((historyPage - 1) * 5)].getIndex() == -2) {
        SimbleeForMobile.updateColor(consumeButton, GRAY);
        SimbleeForMobile.updateColor(giftButton, GREEN);
      }
      else if (History[clickedOverlay2 + ((historyPage - 1) * 5)].getIndex() == -3) {
        SimbleeForMobile.updateColor(consumeButton, GREEN);
        SimbleeForMobile.updateColor(giftButton, GRAY);
      }
      if (History[clickedOverlay2 + ((historyPage - 1) * 5)].getRating() == 1) {
        SimbleeForMobile.updateW(starsRectUISingle, 16);
      }
      else if (History[clickedOverlay2 + ((historyPage - 1) * 5)].getRating() == 2) {
        SimbleeForMobile.updateW(starsRectUISingle, 32);
      }
      else if (History[clickedOverlay2 + ((historyPage - 1) * 5)].getRating() == 3) {
        SimbleeForMobile.updateW(starsRectUISingle, 48);
      }
      else if (History[clickedOverlay2 + ((historyPage - 1) * 5)].getRating() == 4) {
        SimbleeForMobile.updateW(starsRectUISingle, 64);
      }
      else if (History[clickedOverlay2 + ((historyPage - 1) * 5)].getRating() == 5) {
        SimbleeForMobile.updateW(starsRectUISingle, 80);
      }
    }
  }
  SimbleeForMobile.process();
}

void ui_event(event_t &event)
{
  Serial.print("event.id = ");
  Serial.println(event.id);
  Serial.print("event.value = ");
  Serial.println(event.value);
  //SCREEN 2 EVENTS
  if (SimbleeForMobile.screen == 2)
  {
    if (event.id == nextPage)
    {
      updatePage = true;
      winePage++;
      if (winePage > 5)
      {
        winePage = 1;
      }
    }
    else if (event.id == previousPage)
    {
      updatePage = true;
      winePage--;
      if (winePage == 0) {
        winePage = 5;
      }
    }
    else if (event.id == OverlayUI[0]) {
      clickedOverlay = 0;
      updatePage = true;
      if (!addMode)
        SimbleeForMobile.showScreen(5);
      else {
        SimbleeForMobile.updateColor(screen2Background, GE_RED);
        SimbleeForMobile.updateText(screen2Title, "Wine Inventory");
        wine[clickedOverlay + ((winePage - 1) * 5)].setIndex(addDetected);
        addMode = false;
        addDetected = -1;
        updatePage = true;
      }
    }
    else if (event.id == OverlayUI[1]) {
      clickedOverlay = 1;
      updatePage = true;
      if (!addMode)
        SimbleeForMobile.showScreen(5);
      else {
        SimbleeForMobile.updateColor(screen2Background, GE_RED);
        SimbleeForMobile.updateText(screen2Title, "Wine Inventory");
        wine[clickedOverlay + ((winePage - 1) * 5)].setIndex(addDetected);
        addMode = false;
        addDetected = -1;
        updatePage = true;
      }

    }
    else if (event.id == OverlayUI[2]) {
      clickedOverlay = 2;
      updatePage = true;
      if (!addMode)
        SimbleeForMobile.showScreen(5);
      else {
        SimbleeForMobile.updateColor(screen2Background, GE_RED);
        SimbleeForMobile.updateText(screen2Title, "Wine Inventory");
        wine[clickedOverlay + ((winePage - 1) * 5)].setIndex(addDetected);
        addMode = false;
        addDetected = -1;
        updatePage = true;
      }
    }
    else if (event.id == OverlayUI[3]) {
      clickedOverlay = 3;
      updatePage = true;
      if (!addMode)
        SimbleeForMobile.showScreen(5);
      else {
        SimbleeForMobile.updateColor(screen2Background, GE_RED);
        SimbleeForMobile.updateText(screen2Title, "Wine Inventory");
        wine[clickedOverlay + ((winePage - 1) * 5)].setIndex(addDetected);
        addMode = false;
        addDetected = -1;
        updatePage = true;
      }
    }
    else if (event.id == OverlayUI[4]) {
      clickedOverlay = 4;
      updatePage = true;
      if (!addMode)
        SimbleeForMobile.showScreen(5);
      else {
        SimbleeForMobile.updateColor(screen2Background, GE_RED);
        SimbleeForMobile.updateText(screen2Title, "Wine Inventory");
        wine[clickedOverlay + ((winePage - 1) * 5)].setIndex(addDetected);
        addMode = false;
        addDetected = -1;
        updatePage = true;
      }
    }
    else if (event.id == screen2Segment) {
      if (event.value == 0) {
        updatePage = true;
        SimbleeForMobile.showScreen(2);
      }
      else if (event.value == 1) {
        updatePage = true;
        SimbleeForMobile.showScreen(6);
      }
    }
  }
  //SCREEN 5 EVENTS
  else if (SimbleeForMobile.screen == 5)
  {
    if (event.id == addButton) {
      showWineInsertScreen();
      updatePage = true;
    }
    else if (event.id == removeButton) {
      removeBottle();
      updatePage = true;
    }
    else if (event.id == returnButton) {
      SimbleeForMobile.showScreen(2);
      updatePage = true;
    }
  }


  else if (SimbleeForMobile.screen == 6) {
    if (event.id == screen6nextPage)
    {
      updatePage = true;
      historyPage++;
      if (historyPage > 5)
      {
        historyPage = 1;
      }
    }
    else if (event.id == screen6previousPage)
    {
      updatePage = true;
      historyPage--;
      if (historyPage == 0) {
        historyPage = 5;
      }
    }
    else if (event.id == screen6Segment) {
      if (event.value == 0) {
        updatePage = true;
        SimbleeForMobile.showScreen(2);
      }
      else if (event.value == 1) {
        updatePage = true;
        SimbleeForMobile.showScreen(6);
      }
    }
    else if (event.id == HistoryOverlayUI[0]) {
      clickedOverlay2 = 0;
      updatePage = true;
      SimbleeForMobile.showScreen(7);
    }

    else if (event.id == HistoryOverlayUI[1]) {
      clickedOverlay2 = 1;
      updatePage = true;
      SimbleeForMobile.showScreen(7);
    }
    else if (event.id == HistoryOverlayUI[2]) {
      clickedOverlay2 = 2;
      updatePage = true;
      SimbleeForMobile.showScreen(7);
    }
    else if (event.id == HistoryOverlayUI[3]) {
      clickedOverlay2 = 3;
      updatePage = true;
      SimbleeForMobile.showScreen(7);
    }
    else if (event.id == HistoryOverlayUI[4]) {
      clickedOverlay2 = 4;
      updatePage = true;
      SimbleeForMobile.showScreen(7);
    }
  }
  else if (SimbleeForMobile.screen == 7) {
    if (event.id == returnButton) {
      updatePage = true;
      SimbleeForMobile.showScreen(6);
    }
    else if (event.id == consumeButton) {
      History[clickedOverlay2 + ((historyPage - 1) * 5)].setIndex(-3);
      updatePage = true;
    }
    else if (event.id == giftButton) {
      History[clickedOverlay2 + ((historyPage - 1) * 5)].setIndex(-2);
      updatePage = true;
    }
    else if (event.id == deleteButton) {
      History[clickedOverlay2 + ((historyPage - 1) * 5)].setIndex(-1);
      History.erase(History.begin() + (clickedOverlay2 + ((historyPage - 1) * 5)));
      updatePage = true;
      SimbleeForMobile.showScreen(6);
    }
    else if (event.id == rate[0]) {
      SimbleeForMobile.updateW(starsRectUISingle, 16);
      History[clickedOverlay2 + ((historyPage - 1) * 5)].setRating(1);
    }
    else if (event.id == rate[1]) {
      SimbleeForMobile.updateW(starsRectUISingle, 16 * 2);
      History[clickedOverlay2 + ((historyPage - 1) * 5)].setRating(2);
    }
    else if (event.id == rate[2]) {
      SimbleeForMobile.updateW(starsRectUISingle, 16 * 3);
      History[clickedOverlay2 + ((historyPage - 1) * 5)].setRating(3);
    }
    else if (event.id == rate[3]) {
      SimbleeForMobile.updateW(starsRectUISingle, 16 * 4);
      History[clickedOverlay2 + ((historyPage - 1) * 5)].setRating(4);
    }
    else if (event.id == rate[4]) {
      SimbleeForMobile.updateW(starsRectUISingle, 16 * 5);
      History[clickedOverlay2 + ((historyPage - 1) * 5)].setRating(5);
    }
    updatePage = true;

  }

}

void ui()
{
  switch (SimbleeForMobile.screen)
  {
    case 1:
      splashScreen();
      delay(3000);
      SimbleeForMobile.showScreen(2);
      break;

    case 2:
      inventoryScreen();
      break;

    case 3:
      break;

    case 4:
      break;

    case 5:
      addScreen();
      break;

    case 6:
      historyScreen();
      break;

    case 7:
      historydetailScreen();
      break;

    default:
      splashScreen();
      break;
  }
}

void SimbleeForMobile_onDisconnect() {
  disconnected = true;
  updatePage = true;
}
