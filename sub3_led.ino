#if (SUBCORE != 3)
#error "Core selection is wrong!!"
#endif

#include <MP.h>

#define led1 PIN_D03
#define led2 PIN_D05
#define led3 PIN_D06

void setup()
{
  MP.begin();
  Serial.begin(115200);

  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);

  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
  digitalWrite(led3, LOW);
}

int8_t msgid1;
int8_t msgid2;
int8_t msgid3;
int8_t msgid4;
int32_t msgdata1;
int32_t msgdata2;
int32_t msgdata3;
int32_t msgdata4;

int32_t lat_data;
int32_t lon_data;
int32_t ref_lat;
int32_t ref_lon;

void lighting(double LatData, double LonData, double ref_lat, double ref_lon)
{
//  delay(1000);
//  Serial.print("REF DATA IN LED");
//  Serial.print(ref_lat);
//  Serial.print(" : ");
//  Serial.println(ref_lon);
  if ( LatData >= ref_lat && LonData >= ref_lon )
  {
          Serial.println("1");
      digitalWrite(led1, LOW);
      digitalWrite(led2, HIGH);
      digitalWrite(led3, LOW);
//      delay(1000);
  }
  else if ( LatData >= ref_lat && LonData < ref_lon )
  {
      Serial.println("2");
      digitalWrite(led1, HIGH);
      digitalWrite(led2, LOW);
      digitalWrite(led3, LOW);
//      delay(1000);
  }
  else if ( LatData < ref_lat && LonData >= ref_lon )
  {
      Serial.println("3");
      digitalWrite(led1, LOW);
      digitalWrite(led2, LOW);
      digitalWrite(led3, HIGH);
//      delay(1000);
  }
  else
  {
      Serial.println("123");
      digitalWrite(led1, HIGH);
      digitalWrite(led2, HIGH);
      digitalWrite(led3, HIGH);
//      delay(1000);
  }
}

void loop()
{
  /*　Multi Core Section */
  MP.RecvTimeout(MP_RECV_BLOCKING);
  MP.Recv(&msgid1, &msgdata1);
  MP.Recv(&msgid2, &msgdata2);
  MP.Recv(&msgid3, &msgdata3, 1);
  MP.Recv(&msgid4, &msgdata4, 1);
  MP.Send(30, 1);
  MP.Send(31, 1, 1);

  if(msgid1 == 4) {
    lat_data = msgdata1;
    lon_data = msgdata2;
  }else {
    lon_data = msgdata1;
    lat_data = msgdata2;
  }

  if(msgid3 == 10) {
    ref_lat = msgdata3;
    ref_lon = msgdata4;
  }else {
    ref_lon = msgdata3;
    ref_lat = msgdata4;
  }

//  if(msgid1 == 4) {
//    lat_data = msgdata1;
//    if(msgid2 == 5) {
//      lon_data = msgdata2;
//      if(msgid3 == 10) {
//        ref_lat = msgdata3;
//        ref_lon = msgdata4;
//      }else {
//        ref_lon = msgdata3;
//        ref_lat = msgdata4;
//      }
//    }else if (msgid2 == 10) {
//      ref_lat = msgdata2;
//      if(msgid3 == 5) {
//        lon_data = msgdata3;
//        ref_lon = msgdata4;
//      }else {
//        ref_lon = msgdata3;
//        lon_data = msgdata4;
//      }
//    }else {
//      ref_lon = msgdata2;
//      if(msgid3 == 5) {
//        lon_data = msgdata3;
//        ref_lat = msgdata4;
//      }else {
//        ref_lat = msgdata3;
//        lon_data = msgdata4;
//      }
//    }
//  }else if(msgid1 == 5) {
//    lon_data = msgdata1;
//    if(msgid2 == 4) {
//      lat_data = msgdata2;
//      if(msgid3 == 10) {
//        ref_lat = msgdata3;
//        ref_lon = msgdata4;
//      }else {
//        ref_lon = msgdata3;
//        ref_lat = msgdata4;
//      }
//    }else if (msgid2 == 10) {
//      ref_lat = msgdata2;
//      if(msgid3 == 4) {
//        lat_data = msgdata3;
//        ref_lon = msgdata4;
//      }else {
//        ref_lon = msgdata3;
//        lat_data = msgdata4;
//      }
//    }else {
//      ref_lon = msgdata2;
//      if(msgid3 == 4) {
//        lat_data = msgdata3;
//        ref_lat = msgdata4;
//      }else {
//        ref_lat = msgdata3;
//        lat_data = msgdata4;
//      }
//    }
//  }else if(msgid1 == 10) {
//    ref_lat = msgdata1;
//    if(msgid2 == 4) {
//      lat_data = msgdata2;
//      if(msgid3 == 5) {
//        lon_data = msgdata3;
//        ref_lon = msgdata4;
//      }else {
//        ref_lon = msgdata3;
//        lon_data = msgdata4;
//      }
//    }else if (msgid2 == 10) {
//      ref_lat = msgdata2;
//      if(msgid3 == 4) {
//        lat_data = msgdata3;
//        ref_lon = msgdata4;
//      }else {
//        ref_lon = msgdata3;
//        lat_data = msgdata4;
//      }
//    }else {
//      ref_lon = msgdata2;
//      if(msgid3 == 4) {
//        lat_data = msgdata3;
//        ref_lon = msgdata4;
//      }else {
//        ref_lon = msgdata3;
//        lat_data = msgdata4;
//      }
//    }
//  }else {
//    ref_lon = msgdata1;
//    if(msgid2 == 4) {
//      lat_data = msgdata2;
//      if(msgid3 == 5) {
//        lon_data = msgdata3;
//        ref_lon = msgdata4;
//      }else {
//        ref_lon = msgdata3;
//        lon_data = msgdata4;
//      }
//    }else if (msgid2 == 5) {
//      lon_data = msgdata2;
//      if(msgid3 == 4) {
//        lat_data = msgdata3;
//        ref_lat = msgdata4;
//      }else {
//        ref_lat = msgdata3;
//        lat_data = msgdata4;
//      }
//    }else {
//      ref_lat = msgdata2;
//      if(msgid3 == 4) {
//        lat_data = msgdata3;
//        lon_data = msgdata4;
//      }else {
//        lon_data = msgdata3;
//        lat_data = msgdata4;
//      }
//    }
//  }
  
  Serial.println("START 3333333333333333333333333333333");

//  double lat_data = (double)lat_data * 0.0000001;
//  double lon_data = (double)lon_data * 0.0000001;
//
//  double ref_lat = (double)ref_lat * 0.0000001;
//  double ref_lon = (double)ref_lon * 0.0000001;

  double new_lat_data = lat_data * 0.000001;
  double new_lon_data = lon_data * 0.000001;

  double new_ref_lat = ref_lat * 0.000001;
  double new_ref_lon = ref_lon * 0.000001;

//  delay(1000);

//  Serial.print("REF DATA IN LED IN LOOP");
//  Serial.print(new_ref_lat);
//  Serial.print(" : ");
//  Serial.println(new_ref_lon);
  
  lighting(new_lat_data, new_lon_data, new_ref_lat, new_ref_lon);
//  Serial.println("END 333333333333333333333333333333");
}
