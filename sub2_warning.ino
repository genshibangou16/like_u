#if (SUBCORE != 2)
#error "Core selection is wrong!!"
#endif

#include <vector>

#include <MP.h>

using namespace std;

void setup()
{
  MP.begin();
  Serial.begin(115200);
}

int8_t msgid1;
int8_t msgid2;
int8_t msgid3;
int8_t msgid4;
int32_t msgdata1;
int32_t msgdata2;
int32_t msgdata3;
int32_t msgdata4;

int32_t lat_data = 0;
int32_t lon_data = 0;
int32_t others_lat_data = 0;
int32_t others_lon_data = 0;

double DANGE_DIST = 0.000300; // about 3 meter


int is_dange(vector<double> MyPosi, vector<double> OthPosi)
{

  double r1 = MyPosi[0] - OthPosi[0];
  Serial.print("r1 = ");
  Serial.println(r1, 6);
  double x_dist = sq(r1);


  double r2 = MyPosi[1] - OthPosi[1];
  Serial.print("r2 = ");
  Serial.println(r2, 6);
  double y_dist = sq(r2);
  
  double r3 = x_dist - y_dist;
  Serial.print("r3 = ");
  Serial.println(r3, 6);
  double r4 = abs(r3);
  double distance =  sqrt(r4);
  Serial.print("distance = ");
  Serial.println(distance);

  if (distance < DANGE_DIST && distance != 0)
  {
    return 0; // dangerous
  }
  return 1; // safe
}

int isDanger = 0;


void loop()
{  
  
  /*　Multi Core Section */
  /* Layer1 */
  MP.RecvTimeout(MP_RECV_BLOCKING);
  MP.Recv(&msgid1, &msgdata1);
  MP.Recv(&msgid2, &msgdata2);
  MP.Recv(&msgid3, &msgdata3, 1);
  MP.Recv(&msgid4, &msgdata4, 1);
  MP.Send(20, 1);
//  MP.Send(21, 1, 1);
  
//  Serial.println("START 2222222222222222222222222222222222222");

  if(msgid1 == 2) {
    lat_data = msgdata1;
    lon_data = msgdata2;
  }else {
    lon_data = msgdata1;
    lat_data = msgdata2;
  }

  if(msgid3 == 12) {
    others_lat_data = msgdata3;
    others_lon_data = msgdata4;
  }else {
    others_lon_data = msgdata3;
    others_lat_data = msgdata4;
  }

//  if(msgid1 == 2) {
//    lat_data = msgdata1;
//    if(msgid2 == 3) {
//      lon_data = msgdata2;
//      if(msgid3 == 12) {
//        others_lat_data = msgdata3;
//        others_lon_data = msgdata4;
//      }else {
//        others_lon_data = msgdata3;
//        others_lat_data = msgdata4;
//      }
//    }else if (msgid2 == 12) {
//      others_lat_data = msgdata2;
//      if(msgid3 == 3) {
//        lon_data = msgdata3;
//        others_lon_data = msgdata4;
//      }else {
//        others_lon_data = msgdata3;
//        lon_data = msgdata4;
//      }
//    }else {
//      others_lon_data = msgdata2;
//      if(msgid3 == 3) {
//        lon_data = msgdata3;
//        others_lat_data = msgdata4;
//      }else {
//        others_lat_data = msgdata3;
//        lon_data = msgdata4;
//      }
//    }
//  }else if(msgid1 == 3) {
//    lon_data = msgdata1;
//    if(msgid2 == 2) {
//      lat_data = msgdata2;
//      if(msgid3 == 12) {
//        others_lat_data = msgdata3;
//        others_lon_data = msgdata4;
//      }else {
//        others_lon_data = msgdata3;
//        others_lat_data = msgdata4;
//      }
//    }else if (msgid2 == 12) {
//      others_lat_data = msgdata2;
//      if(msgid3 == 2) {
//        lat_data = msgdata3;
//        others_lon_data = msgdata4;
//      }else {
//        others_lon_data = msgdata3;
//        lat_data = msgdata4;
//      }
//    }else {
//      others_lon_data = msgdata2;
//      if(msgid3 == 2) {
//        lat_data = msgdata3;
//        others_lat_data = msgdata4;
//      }else {
//        others_lat_data = msgdata3;
//        lat_data = msgdata4;
//      }
//    }
//  }else if(msgid1 == 12) {
//    others_lat_data = msgdata1;
//    if(msgid2 == 2) {
//      lat_data = msgdata2;
//      if(msgid3 == 3) {
//        lon_data = msgdata3;
//        others_lon_data = msgdata4;
//      }else {
//        others_lon_data = msgdata3;
//        lon_data = msgdata4;
//      }
//    }else if (msgid2 == 12) {
//      others_lat_data = msgdata2;
//      if(msgid3 == 2) {
//        lat_data = msgdata3;
//        others_lon_data = msgdata4;
//      }else {
//        others_lon_data = msgdata3;
//        lat_data = msgdata4;
//      }
//    }else {
//      others_lon_data = msgdata2;
//      if(msgid3 == 2) {
//        lat_data = msgdata3;
//        others_lon_data = msgdata4;
//      }else {
//        others_lon_data = msgdata3;
//        lat_data = msgdata4;
//      }
//    }
//  }else {
//    others_lon_data = msgdata1;
//    if(msgid2 == 2) {
//      lat_data = msgdata2;
//      if(msgid3 == 3) {
//        lon_data = msgdata3;
//        others_lon_data = msgdata4;
//      }else {
//        others_lon_data = msgdata3;
//        lon_data = msgdata4;
//      }
//    }else if (msgid2 == 3) {
//      lon_data = msgdata2;
//      if(msgid3 == 2) {
//        lat_data = msgdata3;
//        others_lat_data = msgdata4;
//      }else {
//        others_lat_data = msgdata3;
//        lat_data = msgdata4;
//      }
//    }else {
//      others_lat_data = msgdata2;
//      if(msgid3 == 2) {
//        lat_data = msgdata3;
//        lon_data = msgdata4;
//      }else {
//        lon_data = msgdata3;
//        lat_data = msgdata4;
//      }
//    }
//  }

  vector<double> others_position = {};
  vector<double> my_position = {};

//  Serial.println("RECEIVED POS DATA IN WARNING");

  double new_lat_data = (double)lat_data * 0.000001;
  double new_lon_data = (double)lon_data * 0.000001;

//  Serial.println("MY POS IN WARNING");
//  Serial.print(lat_data);
//  Serial.print(" : ");
//  Serial.print(lon_data);
//  
//  Serial.println("");
//  
//  Serial.print(new_lat_data, 6);
//  Serial.print(" : ");
//  Serial.println(new_lon_data, 6);
//
//  Serial.println("END RECEIVED POS DATA IN WARNING");

  my_position.push_back(new_lat_data);
  my_position.push_back(new_lon_data);

  double new_others_lat_data = (double)others_lat_data * 0.000001;
  double new_others_lon_data = (double)others_lon_data * 0.000001;
  
  others_position.push_back(new_others_lat_data);
  others_position.push_back(new_others_lon_data);

  int result = is_dange(my_position, others_position);
  if (result == 0)
  {
    MP.Send(21, 1, 1);
  }
  else 
  {
    MP.Send(21, 2, 1);
  }

//  delay(1000);
//  Serial.println("OTHRES DATA IN WARNING");
//  Serial.print(new_others_lat_data, 6);
//  Serial.print(" : ");
//  Serial.println(new_others_lon_data, 6);
//
//  Serial.println("END 22222222222222222222222222");
}
