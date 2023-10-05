#if (SUBCORE != 1)
#error "Core selection is wrong!!"
#endif

#include <Adafruit_GFX.h>
#include <SPI.h>
#include <Wire.h>                // This is unused but seemed required
#include <Adafruit_ILI9341.h>
#include <XPT2046_Touchscreen.h>

#include <GS2200Hal.h>
#include <GS2200AtCmd.h>
#include <TelitWiFi.h>
#include "config.h"

#include <algorithm>
#include <vector>

#include <MP.h>

// Calibration data for the raw touch data to the screen coordinates
#define TS_MINX 395
#define TS_MINY 370
#define TS_MAXX 3800
#define TS_MAXY 4000


// Start Wi-Fi related section /////////////////////////////////////////////
extern uint8_t ESCBuffer[];

char server_cid_l = 0;
char server_cid_m = 0;
char httpsrvr_ip[16];
int wifiFlag = 0;

TelitWiFi gs2200;
TWIFI_Params gsparams;

void wifiInit() {
  /* Initialize SPI access of GS2200 */
  Init_GS2200_SPI();

  /* Initialize AT Command Library Buffer */
  gsparams.mode = ATCMD_MODE_STATION;
  gsparams.psave = ATCMD_PSAVE_DEFAULT;
  if( gs2200.begin( gsparams ) ){
    ConsoleLog( "GS2200 Initilization Fails" );
    while(1);
  }

  /* GS2200 Association to AP */
  if( gs2200.connect( AP_SSID, PASSPHRASE ) ){
    ConsoleLog( "Association Fails" );
    while(1);
  }
}

String wifiLoc(String txt) {
  if(wifiFlag) {
    return "Location%36431346%139090938%36475066%139059127%";
  }else {
    wifiFlag = 1;
  }
  ATCMD_RESP_E resp;
  uint32_t start;
  String data;
  int isPost;
  uint32_t size = txt.length();
  String body;
  char size_string[10];
  
  if(size > 0) {
    isPost = 1;
    data = "id=2&location=" + txt; ///////////////////////////////////////////////////////////////////////////////////////////////////////////
    size = data.length();
    AtCmd_HTTPCONF( HTTP_HEADER_CONTENT_TYPE, "application/x-www-form-urlencoded" );
  }else {
    isPost = 0;
    data = "";
  }

  ConsoleLog( "Start HTTP Client");

  /* Set some HTTP Headers */
  AtCmd_HTTPCONF( HTTP_HEADER_HOST, HTTP_SRVR_IP );
  AtCmd_HTTPCONF( HTTP_HEADER_USER_AGENT, "Spresense" );

  /* Prepare for the next chunck of incoming data */
  WiFi_InitESCBuffer();

  ConsoleLog( "POST or GET Start" );

  do {
    resp = AtCmd_HTTPOPEN( &server_cid_l, HTTP_SRVR_IP, HTTP_PORT );
  } while (ATCMD_RESP_OK != resp);
  
  ConsoleLog( "Socket Opened" );

  /* Content-Length should be set BEFORE sending the data */
  sprintf( size_string, "%d", size );
  do {
    resp = AtCmd_HTTPCONF( HTTP_HEADER_CONTENT_LENGTH, size_string );
  } while (ATCMD_RESP_OK != resp);

  if(isPost) {
    do {
      resp = AtCmd_HTTPSEND( server_cid_l, HTTP_METHOD_POST, 10, "/AD33Tui6AKcV/location.php", data.c_str(), size );
    } while (ATCMD_RESP_OK != resp);
    Serial.println("HTTPSEND");
    /* Need to receive the HTTP response */
    while( 1 ){
      if( Get_GPIO37Status() ){
        resp = AtCmd_RecvResponse();
        
        if( ATCMD_RESP_BULK_DATA_RX == resp ){
          if( Check_CID( server_cid_l ) ){
            char *p;
            if( (p=strstr( (char *)(ESCBuffer+1), "200 OK\r\n" )) != NULL ){
              body = strstr(p, "Location");
            }
          }
        }
        WiFi_InitESCBuffer();
        break;
      }
    }
  }else {
    resp = AtCmd_HTTPSEND( server_cid_l, HTTP_METHOD_GET, 10, "/AD33Tui6AKcV/location.php", "", 0);
    if( ATCMD_RESP_BULK_DATA_RX == resp ){
      if( Check_CID( server_cid_l ) ){
          char *p;
          if( (p=strstr( (char *)(ESCBuffer+1), "200 OK\r\n" )) != NULL ){
            body = strstr(p, "Location");
          }
      }
      WiFi_InitESCBuffer();
    }
    else{
      ConsoleLog( "?? Unexpected HTTP Response ??" );
      ConsolePrintf( "Response Code : %d\r\n", resp );
    }
  }

  start = millis();
  while(1){
    if( Get_GPIO37Status() ){
      resp = AtCmd_RecvResponse();
      if( ATCMD_RESP_OK == resp ){
        // AT+HTTPSEND command is done
        break;
      }
    }
    if( msDelta(start)>20000 ) // Timeout
      break;
  }

  do {
    resp = AtCmd_HTTPCLOSE( server_cid_l );
  } while( ATCMD_RESP_OK != resp && ATCMD_RESP_INVALID_CID != resp );
  ConsoleLog( "Socket Closed" );
  wifiFlag = 0;
  return body;
}

String wifiMes(String txt) {
  if(wifiFlag) {
    return "Message,active,love,polite%Nice!,5,2,1%Good!!,2,1,4%Yeah!!!,2,3,3%HogeHoge!,5,2,1%KORINw,2,1,3%WAKARETA,3,4,1%SAYONARA,4,5,3%Single!!,2,4,5%YAHOOO!,1,2,3%HITORI!!!,5,3,4%YAHHOI!,3,2,3%";
  }else {
    wifiFlag = 1;
  }
  ATCMD_RESP_E resp;
  uint32_t start;
  String data;
  int isPost;
  uint32_t size = txt.length();
  String body;
  char size_string[10];
  
  if(size > 0) {
    isPost = 1;
    data = "message=" + txt;
    size = data.length();
    AtCmd_HTTPCONF( HTTP_HEADER_CONTENT_TYPE, "application/x-www-form-urlencoded" );
  }else {
    isPost = 0;
    data = "";
  }

  ConsoleLog( "Start HTTP Client");

  /* Set some HTTP Headers */
  AtCmd_HTTPCONF( HTTP_HEADER_HOST, HTTP_SRVR_IP );
  AtCmd_HTTPCONF( HTTP_HEADER_USER_AGENT, "Spresense" );

  /* Prepare for the next chunck of incoming data */
  WiFi_InitESCBuffer();

  ConsoleLog( "POST or GET Start" );

  do {
    resp = AtCmd_HTTPOPEN( &server_cid_m, HTTP_SRVR_IP, HTTP_PORT );
  } while (ATCMD_RESP_OK != resp);
  
  ConsoleLog( "Socket Opened" );

  /* Content-Length should be set BEFORE sending the data */
  sprintf( size_string, "%d", size );
  do {
    resp = AtCmd_HTTPCONF( HTTP_HEADER_CONTENT_LENGTH, size_string );
  } while (ATCMD_RESP_OK != resp);

  if(isPost) {
    do {
      resp = AtCmd_HTTPSEND( server_cid_m, HTTP_METHOD_POST, 10, "/AD33Tui6AKcV/message.php", data.c_str(), size );
    } while (ATCMD_RESP_OK != resp);
    /* Need to receive the HTTP response */
    while( 1 ){
      if( Get_GPIO37Status() ){
        resp = AtCmd_RecvResponse();
        
        if( ATCMD_RESP_BULK_DATA_RX == resp ){
          if( Check_CID( server_cid_m ) ){
            char *p;
            if( (p=strstr( (char *)(ESCBuffer+1), "200 OK\r\n" )) != NULL ){
              body = strstr(p, "Message");
            }
          }
        }
        WiFi_InitESCBuffer();
        break;
      }
    }
  }else {
    resp = AtCmd_HTTPSEND( server_cid_m, HTTP_METHOD_GET, 10, "/AD33Tui6AKcV/message.php", "", 0);
    if( ATCMD_RESP_BULK_DATA_RX == resp ){
      if( Check_CID( server_cid_m ) ){
          char *p;
          if( (p=strstr( (char *)(ESCBuffer+1), "200 OK\r\n" )) != NULL ){
            body = strstr(p, "Message");
          }
      }
      WiFi_InitESCBuffer();
    }
    else{
      ConsoleLog( "?? Unexpected HTTP Response ??" );
      ConsolePrintf( "Response Code : %d\r\n", resp );
    }
  }

  start = millis();
  while(1){
    if( Get_GPIO37Status() ){
      resp = AtCmd_RecvResponse();
      if( ATCMD_RESP_OK == resp ){
        // AT+HTTPSEND command is done
        break;
      }
    }
    if( msDelta(start)>20000 ) // Timeout
      break;
  }

  do {
    resp = AtCmd_HTTPCLOSE( server_cid_m );
  } while( ATCMD_RESP_OK != resp && ATCMD_RESP_INVALID_CID != resp );
  ConsoleLog( "Socket Closed" );

  wifiFlag = 0;
  return body;
}

// End Wi-Fi related section /////////////////////////////////////////////

/**
 * <PIN ASSIGN>
 *
 * T_IRQ     -> N/A
 * T_DO      -> PIN_D12
 * T_DIN     -> PIN_D11
 * T_CS      -> PIN_D08
 * T_CLK     -> PIN_D13
 * SDO(MISO) -> PIN_D12
 * LED       -> 3.3V
 * SCK       -> PIN_D13
 * SDI(MOSI) -> PIN_D11
 * DC        -> PIN_D04
 * RESET     -> PIN_D02
 * CS        -> PIN_D10
 * GND       -> GND
 * VCC       -> 3.3V
 */

// Setting for hardware SPI with touchpanel
#define TOUCH_CS PIN_D08
XPT2046_Touchscreen ts = XPT2046_Touchscreen(TOUCH_CS);

// Setting for hardware SPI with display
#define TFT_CS   PIN_D10
#define TFT_DC   PIN_D04
#define TFT_RST  PIN_D02
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);

String first_choice;
String second_choice;
String third_choice;

String selected_message;

// do not forget the last "%"
//String temp = "Message,active,love,polite%Nice!,5,2,1%Good!!,2,1,4%Yeah!!!,2,3,3%HogeHoge!,5,2,1%KORINw,2,1,3%WAKARETA,3,4,1%SAYONARA,4,5,3%Single!!,2,4,5%YAHOOO!,1,2,3%HITORI!!!,5,3,4%YAHHOI!,3,2,3%";
String temp;

// standard personality
std::vector<int> personality = {1, 2, 3};

std::vector<int> active_list = {1, 2, 3, 4, 5};

std::vector<int> love_list = {1, 2, 3, 4, 5};

std::vector<int> polite_list = {1, 2, 3, 4, 5};

std::vector<String> messageVector;

std::vector<String> positionVector;

std::vector<String> refVector;

int getSize(String tmp){
  int sizecount = 0;
  for (int i=0;i<1000;i++) {
    if (tmp[i] == false){
      return sizecount;
    }
    sizecount++;
  }
}

int getDelimNum(String tmpForFunc) {
    int count = 0;
    int for_size = getSize(tmpForFunc);
    for (int i=0; i<=for_size; i++) {
        if (tmpForFunc[i] == '%') {
            count++;
        }
    }
    return count;
}

void getRows(String tmpForFunc, std::vector<String> &messageVectorForFunc) {
  int count = 0;
  int for_size = getSize(tmpForFunc);
  String row;
  int i = 0;
  while (i<=for_size) {
    row += tmpForFunc[i];
    if (tmpForFunc[i+1] == '%' and count == 0) {
      i += 1;
      count++;
      row = "";
    }
    if (tmpForFunc[i+1] == '%') {
      messageVectorForFunc.push_back(row);
      i += 1;
      count++;
      row = "";
    }
    i++;
  }
}

// これがないと同じrowが入ってしまう可能性あり、
void shuffle(std::vector<String> &messageVectorForFunc) {
  random_shuffle(messageVectorForFunc.begin(), messageVectorForFunc.end());
}

// for first_choice
String getFirstMessage(String rowForFunc) {
  String first_message;
  int for_size = getSize(rowForFunc);
  for (int i=0; i<for_size; i++){
    first_message += rowForFunc[i];
    if (rowForFunc[i+1] == ','){
      return first_message;
    }
  }
  return "error: could not get message1.";
}

// for second_choice
String getSecondMessage(String rowForFunc) {
  String second_message;
  int for_size = getSize(rowForFunc);
  for (int i=0; i<for_size; i++){
    second_message += rowForFunc[i];
    if (rowForFunc[i+1] == ','){
      return second_message;
    }
  }
  return "error: could not get message2.";
}

// for third_choice
String getThirdMessage(String rowForFunc) {
  String third_message;
  int for_size = getSize(rowForFunc);
  for (int i=0; i<for_size; i++){
    third_message += rowForFunc[i];
    if (rowForFunc[i+1] == ','){
      return third_message;
    }
  }
  return "error: could not get message3.";
}

int getFirstComma(String rowForFunc){
  int for_size = getSize(rowForFunc);
  for (int i=0; i<for_size; i++) {
    if (rowForFunc[i] == ','){
      int comma_index = i;
      return comma_index;
    }
  }
  return 1;
}

///////////////////////////////////////////////////////////

String first_choice_message;
int first_choice_value;
String second_choice_message;
int second_choice_value;
String third_choice_message;
int third_choice_value;

void getchoices(std::vector<String> &messageVectorForFunc, String tempForFunc)
{
  int end_active = 0;
  int end_love = 0;
  int end_polite = 0;

  String row;


  shuffle(messageVectorForFunc);
  
  int delim_num = getDelimNum(tempForFunc);
  // (delim_num is equal to row number)
  int row_num = 0;

//  Serial.println(messageVectorForFunc[row_num]);
  
  while (row_num<delim_num-1) {
    row = messageVectorForFunc[row_num];
//    Serial.println("row : ");
//    Serial.println(row);
    
    int first_comma_num = getFirstComma(row);

    int active_value = row[first_comma_num+1]-'0';
    int love_value = row[first_comma_num+3]-'0';
    int polite_value = row[first_comma_num+5]-'0';
//    Serial.println("active_value : ");
//    Serial.println(active_value);
    if ((active_value == personality[0]) && (end_active == 0)) {
      String first_choice_tmp = getFirstMessage(row);
      if ((first_choice_tmp != second_choice_message) && (first_choice_tmp != third_choice_message))
      {
        first_choice_message = first_choice_tmp;
        first_choice_value = active_value;
        end_active = 1;
      }
//      Serial.println("IN!!11111111111111");
    }
    else if ((love_value == personality[1]) && (end_love == 0)) {
      String second_choice_tmp = getFirstMessage(row);
      if ((second_choice_tmp != first_choice_message) && (second_choice_tmp != third_choice_message))
      {
        second_choice_message = second_choice_tmp;
        second_choice_value = love_value;
        end_love = 1;
      }
//      Serial.println("IN!!222222222222222222");
    }
    else if ((polite_value == personality[2]) && (end_polite == 0)) {
      String third_choice_tmp = getFirstMessage(row);
      if ((third_choice_tmp != first_choice_message) && (third_choice_tmp != second_choice_message))
      {
        third_choice_message = third_choice_tmp;
        third_choice_value = polite_value;
        end_polite = 1;
      }
    }
    
//    Serial.println("OUT!!");
    row_num++;
  }
//    Serial.println("#################################################");
}

// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

void choice(int cn, String txt) {
  tft.fillRect(5, 5 + cn * 80, 230, 70, ILI9341_BLACK);
  tft.drawRect(5, 5 + cn * 80, 230, 70, ILI9341_WHITE);

  tft.setCursor(10, 30 + cn * 80);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(3);
  tft.println(txt);

  tft.drawRect(239, 319, 1, 1, ILI9341_BLACK);
}

int getSelected() {
  // Retrieve a point
  TS_Point p = ts.getPoint();

  int tmp = p.x;
  p.x = p.y;
  p.y = tmp;
 
  // Scale from ~0->4000 to tft.width using the calibration #'s
  p.x = map(p.x, TS_MINX, TS_MAXX, 0, tft.width());
  p.y = map(p.y, TS_MINY, TS_MAXY, tft.height(), 0);

  int selected;

  if(p.y <= 80) {
    selected = 0;
  }else if(p.y <= 160) {
    selected = 1;
  }else if(p.y <= 240) {
    selected = 2;
  }else {
    selected = 3;
  }

  return selected;
}
// end of display
////////////////////////////////////////////////////////////

void update_personality(int valueForFunc, String messageForFunc) {
  Serial.print("You chose: ");
  Serial.println(messageForFunc);

  if ( valueForFunc == 1 )
  {
    int selected_active_point = first_choice_value;
    active_list.push_back(selected_active_point);
    int random_active_value = random( 1 , active_list.size() );
    int new_active_value = active_list[ random_active_value ];
//    Serial.print( "new_active_value = " );
//    Serial.println( new_active_value );
    personality[0] = new_active_value;    
  }
  else if ( valueForFunc == 2 )
  {
    int selected_love_point = second_choice_value;
    love_list.push_back(selected_love_point);
    int random_love_value = random( 1 , love_list.size() );
    int new_love_value = love_list[ random_love_value ];
//    Serial.print("new_love_value = ");
//    Serial.println( new_love_value );
    personality[1] = new_love_value;
  }
  else if ( valueForFunc == 3 )
  {
    int selected_polite_point = third_choice_value;
    polite_list.push_back(selected_polite_point);

    int random_polite_value = random( 1 , polite_list.size() );
    int new_polite_value = polite_list[ random_polite_value ];
//    Serial.print("new_polite_value = ");
//    Serial.println( new_polite_value );
    personality[2] = new_polite_value;
  }
  else {
    Serial.println("EBIDORIA");
  }
}

void setup() {
  MP.begin();
  
  Serial.begin(115200);
  Serial.println("Start");

  tft.begin();
  
  if(!ts.begin()) {
    Serial.println("Couldn't start touchscreen controller");
    exit(1);
  }

  tft.fillScreen(ILI9341_BLACK);

  wifiInit();

  temp = wifiMes("");

  getRows(temp, messageVector);

  getchoices(messageVector, temp);

  choice(0, "Reload");
  choice(1, first_choice_message);
  choice(2, second_choice_message);
  choice(3, third_choice_message);
}

int8_t msgid1;
int8_t msgid2;
int8_t msgid3;
int8_t msgid4 = 1;
int8_t msgid5;
int8_t msgid6;
int32_t msgdata1;
int32_t msgdata2;
int32_t msgdata3;
int32_t msgdata4;
int32_t msgdata5 = 1;
int32_t msgdata6;

int32_t lat_data = 0;
int32_t lon_data = 0;

int isDanger = 0;

void loop()
{
// The order should be "Touch -> Update parsonality -> Rewrite -> Initialize"
// This is Touch section
////
  if(!ts.touched()) {
    MP.RecvTimeout(MP_RECV_POLLING);
    MP.Recv(&msgid3, &msgdata3);
    MP.Recv(&msgid4, &msgdata4, 2);
    MP.Recv(&msgid5, &msgdata5, 3);
    if(msgid3 == 6) {
      Serial.println("3: START EVERY 10 SEC PROC");
      MP.Send(15, 1);

      Serial.println("4: WAIT FOR DATA");

      tft.fillScreen(ILI9341_BLACK);
      
      MP.RecvTimeout(MP_RECV_BLOCKING);
      MP.Recv(&msgid1, &msgdata1);
      MP.Recv(&msgid2, &msgdata2);

      Serial.println("7: DATA RECAIVED");

      MP.Send(16, 1);

      Serial.println("8: RECAIVED SIGNAL SENT");

      msgid3 = 0;

      if(msgid1 == 0) {
        lat_data = msgdata1;
        lon_data = msgdata2;
      }else {
        lat_data = msgdata2;
        lon_data = msgdata1;
      }

//      Serial.print("Lat : ");
//      Serial.print(lat_data);
//      Serial.print(" : ");
//      Serial.print("Lon : ");
//      Serial.println(lon_data);
      
      String wifi_send_temp = "Location#";
      wifi_send_temp += String(lat_data);
      wifi_send_temp += "#";
      wifi_send_temp += String(lon_data);
      wifi_send_temp += "#";

    /* send others_lat(lon)_data to sub2(warning) */
      String othLoc_refLoc_temp = wifiLoc(wifi_send_temp);
      
      getRows(othLoc_refLoc_temp, positionVector);
      
//      Serial.print("positionVector");
//      Serial.println(positionVector[0]);
//      Serial.println(positionVector[1]);
//      Serial.println(positionVector[2]);
//      Serial.println(positionVector[3]);

      /* Layer3 */
      int ref_lat_data = positionVector[2].toInt();
      int ref_lon_data = positionVector[3].toInt();
      
      /* convert String to int */
      int others_lat_data = positionVector[0].toInt();
      int others_lon_data = positionVector[1].toInt();

//      Serial.print("OTHERS DATA IN SUGGEST");
//      Serial.print(others_lat_data);
//      Serial.print(" : ");
//      Serial.println(others_lon_data);
//
//      Serial.print("REF DATA IN SUGGEST");
//      Serial.print(ref_lat_data);
//      Serial.print(" : ");
//      Serial.println(ref_lon_data);

      if(msgid4) {
//        Serial.println("SEND TO SUB 2 FROM SUB 1");
        MP.Send(12, others_lat_data, 2);
        MP.Send(13, others_lon_data, 2);
        if(msgdata4 == 1) {
          // DANGER
          isDanger = 1;
        }else {
          isDanger = 0;
        }
//        Serial.print("IS DANGER");
//        Serial.println(isDanger);
        msgid4 = 0;
        msgdata4 = 0;
      }
      if(msgdata5) {
//        Serial.println("SEND TO SUB 3 FROM SUB 1");
        MP.Send(10, ref_lat_data, 3);
        MP.Send(11, ref_lon_data, 3);
        msgdata5 = 0;
      }
      /*
       * 注意：int整数になっているから、1000000で割ってdouble型で使う
      */
//      Serial.println("11111111111111111111111111111111111");
    
      temp = wifiMes("");

      messageVector = {};
    
      getRows(temp, messageVector);
    
      getchoices(messageVector, temp);
    
      choice(0, "Reload");
      choice(1, first_choice_message);
      choice(2, second_choice_message);
      choice(3, third_choice_message);

//      int usedMem, freeMem, largestFreeMem;
//
//      MP.GetMemoryInfo(usedMem, freeMem, largestFreeMem);
//
//      Serial.print("[MEMORY INFO] USED MEM: ");
//      Serial.print(usedMem);
//      Serial.print("; FREE MEM: ");
//      Serial.print(freeMem);
//      Serial.print("; LARGEST FREE MEM: ");
//      Serial.println(largestFreeMem);
//
//      Serial.println(lat_data);
//      Serial.println(lon_data);
//      Serial.println("END UNTOUCH IF");

      if(isDanger) {
         tft.fillScreen(ILI9341_RED);
         choice(0, "KEEP");
         choice(1, "DISTANCE");
      }
      positionVector = {};
      refVector = {};
    }
    delay(10);
    return;
  }

// End Touch section

////
// The order should be "Touch -> Update parsonality -> Rewrite -> Initialize"
// This is Upgrade parsonality section
////

  int selected_value = getSelected();

  if(selected_value == 1) {
    selected_message = first_choice_message;
  }else if(selected_value == 2) {
    selected_message = second_choice_message;
  }else if(selected_value == 3) {
    selected_message = third_choice_message;
  }else {
//  Reload
    
    tft.fillScreen(ILI9341_BLACK);
  
    temp = wifiMes("");

    messageVector = {};
  
    getRows(temp, messageVector);
  
    getchoices(messageVector, temp);
    
    choice(0, "Reload");
    choice(1, first_choice_message);
    choice(2, second_choice_message);
    choice(3, third_choice_message);
    
    return;
  }
  
  update_personality(selected_value, selected_message);

// End Update parsonality section

////
// The order should be "Touch -> Update parsonality -> Rewrite -> Initialize"
// This is Rewrite section
////

  tft.fillScreen(ILI9341_BLACK);

  temp = wifiMes(selected_message);

  messageVector = {};

  getRows(temp, messageVector);

  getchoices(messageVector, temp);

  choice(0, "Reload");
  choice(1, first_choice_message);
  choice(2, second_choice_message);
  choice(3, third_choice_message);
// End Reqrite section
}
