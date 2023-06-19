/*
void StalkerProtocol_123_FromTo_id(){
  int currPeriod = millis()/2000;
  uint8_t payload2_size;
  uint8_t payload3_size;
  char From_id_char[32] = "";
  char To_id_char[32] = "";
  char Buffer[32] = "";
  int From_id_int= 134217728;//rand() % 1000+1;
  int To_id_int = 200;
  char payload2[61] ;

  char DeviceType[] = "Anomaly";

  int AlphaRad_Offset_int = 50;
  char AlphaRad_Offset_char [32] = "";

  char TotMsg_char[32] = "";
  int TotMsg_int= 1;

  char CurrMsg_char[32] = "";
  int CurrMsg_int= 1;

  uint8_t payload3[61];
*/
  

  /*
Protocol:
0-From_id:
1-To_id:
2-CurrMsg:
3-TotalMsg:
4-Device_Type:
5-Name:
|Commnads:

*/

/*
  if (currPeriod != lastPeriod)
  {
    lastPeriod = currPeriod;

int MsgByteCount = 0;
Serial.println("START");
int DeviceType_int[20];

Serial.printf("sizeof(DeviceType): %d\n", sizeof(DeviceType));
Serial.printf("In Char: \n");
for (int i = 0; i < sizeof(DeviceType);i++){
  DeviceType_int[i] = DeviceType[i];
  Serial.printf("DeviceType[%d]: %c\n", i, DeviceType[i]);
}

Serial.printf("In Int: \n");
for (int i = 0; i < sizeof(DeviceType);i++){
  Serial.printf("DeviceType[%d]: %d\n", i, DeviceType[i]);
}



payload3[MsgByteCount] = (byte) (0 | 0x80);  MsgByteCount++;// "|0" MsgByteCount
IntegerDeconstruct_4ByteMax(From_id_int);
for(int i = 1; i<=IntegerDeconstruct_Length; i++){
  payload3[MsgByteCount-1+i]=IntegerDeconstruct_Output[i-1];
}
MsgByteCount = MsgByteCount + IntegerDeconstruct_Length;
//payload3[1] = IntegerDeconstruct_Output[0];    //From_ID data1
//payload3[2] = IntegerDeconstruct_Output[1];  //From_ID data2
payload3[MsgByteCount] = (byte) (1 | 0x80); MsgByteCount++;      // "|1"
payload3[MsgByteCount] = 1;                 MsgByteCount++;      //To_ID data1
payload3[MsgByteCount] = 56;                MsgByteCount++;      //To_ID data2
payload3[MsgByteCount] = 30;                MsgByteCount++;      //To_ID data3
payload3[MsgByteCount] = (byte) (2 | 0x80); MsgByteCount++;      // "|2"
payload3[MsgByteCount] = 100;               MsgByteCount++;      // Device Type data1
payload3[MsgByteCount] = 101;               MsgByteCount++;      // Device Type data2
payload3[MsgByteCount] = 102;               MsgByteCount++;      // Device Type data3
payload3[MsgByteCount] = 103;               MsgByteCount++;      // Device Type data4
payload3[MsgByteCount] = 104;               MsgByteCount++;      // Device Type data5
payload3[MsgByteCount] = 105;               MsgByteCount++;      // Device Type data6
payload3[MsgByteCount] = (byte) (3 | 0x80); MsgByteCount++;      // "|3"



for (int i = 0; i < sizeof(DeviceType)-1;i++){
  payload3[MsgByteCount] = DeviceType[i];
  MsgByteCount++;
  //Serial.printf("DeviceType[%d]: %c\n", i, DeviceType[i]);
}
MsgByteCount--;
//MsgByteCount = MsgByteCount + sizeof(DeviceType)-1;


    payload3_size = MsgByteCount+1;
    Serial.println("");
    //Serial.print("payload3: ");Serial.println(payload3);
    Serial.print("payload3_size: ");Serial.println(payload3_size);

    Serial.print("Sent: ");
    radio.send(GATEWAYID, payload3, payload3_size,false);
    for(int i = 0; i<payload3_size;i++){
      Serial.print(payload3[i]);Serial.print("|");
    }




    strcat(payload2,"|űéáőú");
    //int i = 1;
    //har car[0] = (char)i;
    //strcat(payload2, car);
    sprintf(From_id_char, "%d", From_id_int);
    strcat(payload2,From_id_char);

    strcat(payload2,"|");
    sprintf(Buffer, "%d", 1);
    strcat(payload2,Buffer);
    sprintf(To_id_char, "%d", To_id_int);
    strcat(payload2,To_id_char);

    strcat(payload2,"|");
    sprintf(Buffer, "%d", 2);
    strcat(payload2,Buffer);
    strcat(payload2,DeviceType);

    strcat(payload2,"|");
    sprintf(Buffer, "%d", 3);
    strcat(payload2,Buffer);
    sprintf(TotMsg_char, "%d", TotMsg_int);
    strcat(payload2,TotMsg_char);

    strcat(payload2,"|");
    sprintf(Buffer, "%d", 4);
    strcat(payload2,Buffer);
    sprintf(CurrMsg_char, "%d", CurrMsg_int);
    strcat(payload2,CurrMsg_char);

    strcat(payload2,"|");
    sprintf(Buffer, "%d", 5);
    strcat(payload2,Buffer);
    sprintf(AlphaRad_Offset_char, "%d", AlphaRad_Offset_int);
    strcat(payload2,AlphaRad_Offset_char);

    //END of MSG

    //Serial.print("payload3: ");
    for(int i = 0; i<255;i++){
      //Serial.print(payload3[i]);Serial.print("|");
      if (payload2[i] == '\0'){
        payload2_size = i;
        i = 255;
      }
    }
    //payload3_size = 13;
    Serial.println("");
    //Serial.print("payload3: ");Serial.println(payload3);
    Serial.print("payload3_size: ");Serial.println(payload2_size);

    Serial.print("Sent: ");
    radio.send(GATEWAYID, payload2, payload2_size,false);
    for(int i = 0; i<payload2_size;i++){
      Serial.print(payload2[i]);Serial.print("|");
    }
    Serial.println("");
  }
}
*/
/*
void StalkerProtocolTest(){
  int currPeriod = millis()/TRANSMITPERIOD;
  if (currPeriod != lastPeriod)
  {

    
    for(int i = 0; i < sizeof(SP_PlayloadTest);  i++ ){
      SP_PlayloadTest[i]='0';
    }
  Serial.print("SP_PlayloadTest-Beginning: "); Serial.println(SP_PlayloadTest);


    int sp_configbyte = 0; //1
    int sp_id = 12345;      //4
    char sp_name[32] = "almafa-123";    //32
    Serial.print("sp_name: ");Serial.println(sp_name);
    bool EndFlag = false;
    for(int i = 0; i < 20;  i++ ){
      if (i == 5){//sp_name[i] == '\0'
        EndFlag = true;
      }else if (EndFlag == true){
        sp_name[i] = 'x';
      }
    }
    Serial.print("sp_name: ");Serial.println(sp_name);
    char buffer[32];






    //for(int i = 0; i < 1;  i++ ){
      sprintf(buffer, "%01d", sp_configbyte);
      Serial.print("buffer: ");Serial.println(buffer);
      SP_PlayloadTest[0] = buffer[0];
    //}
    //for(int i = 1; i < 4;  i++ ){
      sprintf(buffer, "%04d", sp_id);
      Serial.print("buffer: ");Serial.println(buffer);

    for(int i = 1; i <= 4;  i++ ){
      SP_PlayloadTest[i] = buffer[i-1];
    }
    //}
    for(int i = 0; i < 32 ;  i++ ){//sizeof(sp_name)
      SP_PlayloadTest[i+5] = sp_name[i];
    }
  Serial.print("SP_PlayloadTest: ");Serial.println(SP_PlayloadTest);


    Serial.print("SP_PlayloadTest-Result: ");
    for(int i = 0; i < sizeof(SP_PlayloadTest);  i++ ){
      Serial.print((char)SP_PlayloadTest[i]);
    }
    Serial.println("");
  }

}*/