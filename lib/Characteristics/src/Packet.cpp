#include "string.h"
#include "Packet.h"

#include "Arduino.h"

WritePacket Packet::writePacket;
WritePacket* Packet :: parseWrite(const char *binPacket) {

  writePacket.serviceId = binPacket[5]-1;
  writePacket.charCount = binPacket[6];
  writePacket.charsStruct = new CharStruct[writePacket.charCount];

  for (int i = 0; i < writePacket.charCount; i++) {
      writePacket.charsStruct[i].id = binPacket[7 + i*SIZE_OF_CHAR_STRUCT + 0]-1;
      writePacket.charsStruct[i].dataLen = binPacket[7 + i*SIZE_OF_CHAR_STRUCT + 1];
      writePacket.charsStruct[i].data = new char[writePacket.charsStruct[i].dataLen];

      for (int j = 0; j < writePacket.charsStruct[i].dataLen; j++) {
        writePacket.charsStruct[i].data[j] = binPacket[7 + i*SIZE_OF_CHAR_STRUCT + 1 + 1 + j];
      }
   }

   return &writePacket;
 }

ReadPacket Packet :: readPacket;
ReadPacket* Packet :: parseRead(const char *binPacket){

  readPacket.serviceId = binPacket[5]-1;
  readPacket.charCount = binPacket[6];
  readPacket.characteristicIds = &binPacket[7];

  return &readPacket;
}

ResponsePacket Packet :: responsePacket;
char* Packet :: stringifyResponse(ResponsePacket *responsePacket){
  Serial.println("one");
  int allCharDataLen = 0;
  for (int i = 0; i < responsePacket->charCount; i++) {
    allCharDataLen += responsePacket->charsStruct[i].dataLen;
  }
  char *binPacket = new char[7 + responsePacket->charCount*2 + allCharDataLen + 1];
Serial.print("allCharDataLen:");
Serial.println(allCharDataLen);

Serial.print("total char data:");
Serial.println(7 + responsePacket->charCount*2 + allCharDataLen + 1);

  binPacket[0] = 0x01;
  binPacket[1] = 0x04;
  binPacket[2] = 0x01;
  binPacket[3] = 0x01;
  binPacket[4] = 0x01;
  binPacket[5] = responsePacket->serviceId + 1;
  binPacket[6] = responsePacket->charCount;

  int index = 6;
  for (int i = 0; i < responsePacket->charCount; i++) {
    binPacket[++index] = responsePacket->charsStruct[i].id;
      Serial.print("charsStruct[i].id");
        Serial.println(responsePacket->charsStruct[i].id);
    binPacket[++index] = responsePacket->charsStruct[i].dataLen;
    Serial.print("charsStruct[i].dataLen");
      Serial.println(responsePacket->charsStruct[i].dataLen);
    for (int j = 0; j < responsePacket->charsStruct[i].dataLen; j++) {
      binPacket[++index] = responsePacket->charsStruct[i].data[j];
    }
  }

  binPacket[++index] = 0;

Serial.println("binPacket");
  for (int i = 0; i < index; i++) {
    Serial.println((int)binPacket[i]);
  }

  return binPacket;
}
