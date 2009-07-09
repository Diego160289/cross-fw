#include "CommonUtils.h"

#include <string.h>

namespace
{

  inline char index2byte(char index)
  {
    if(index >= 0x41 && index <= 0x5A) return index-0x41;
    if(index >= 0x61 && index <= 0x7A) return index-0x61+26;
    if(index >= 0x30 && index <= 0x39) return index-0x30+52;
    if(index == 0x2B) return 62;
    if(index == 0x2F) return 63;
    return 0;
  }

  void base64_encode(char *out, const char *data, unsigned int len)
  {
    const char *base64_set = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    unsigned char index2,index3;
    unsigned int i=0;
    for(; i<len; (i+=3,data+=3,out+=4))
    {
      *out = base64_set[*data >> 2];
      index2 = (*data << 4) & 0x30;
      if(i+2<len) {
        index2 |= *(data+1) >> 4;
        index3 = ((*(data+1) << 4) & 0xFF) >> 2;
        if(i+3<len) {
          index3 |= *(data+2) >> 6;
          *(out + 3) = base64_set[*(data+2) & 0x3F];
        } else {
          *(out + 3) = '=';
        }
        *(out + 2) = base64_set[index3];
      } else {
        *(out + 2) = '=';
        *(out + 3) = '=';
      }
      *(out + 1) = base64_set[index2];
    }
    *out = 0;
  }

  unsigned int base64_decode(char *out, const char *data, unsigned int len)
  {
    char byte1,byte2,byte3,index2,index3;
    unsigned int count = 0;
    for(;len--;data+=4) {
      count+=3;
      byte1 = (index2byte(*data) << 2) & 0xFF;
      index2 = index2byte(*(data+1));
      byte1 |= index2 >> 4;
      if(*(data+2) != '=') {
        byte2 = (index2 & 0xF) << 4;
        index3 = index2byte(*(data+2));
        byte2 |= index3 >> 2;
        if(*(data+3) != '=') {
          byte3 = (index3 << 6) & 0xFF;
          byte3 |= index2byte(*(data+3));
        } else {count--;byte3=0;len=0;}
      } else {count-=2;byte2=byte3=0;len=0;}
      *out++ = byte1;
      *out++ = byte2;
      *out++ = byte3;
    }
    return count;
  }

  unsigned int calc_bufsize_base64_decode(const char *data, unsigned int len)
  {
    unsigned int count = len ? 1 : 0;
    for(;len--;data+=4) {
      count+=3;
      if(*(data+2) != '=') {
        if(*(data+3) != '=') {
        } else {count--;len=0;}
      } else {count-=2;len=0;}
    }
    return count;
  }

}

namespace Common
{
  std::string BinToBase64(const void *data, unsigned bytes)
  {
    unsigned OutLen = static_cast<unsigned>((bytes / 3.0) * 4 + 1) + 2;
    std::vector<char> OutBuf(OutLen, 0);
    base64_encode(&OutBuf[0], reinterpret_cast<const char *>(data), bytes);
    return &OutBuf[0];
  }

  CharVectorPtr Base64ToBin(const char *base64Str)
  {
    size_t Len = strlen(base64Str);
    CharVectorPtr Ret(new CharVector(calc_bufsize_base64_decode(base64Str, Len), 0));
    base64_decode(&Ret->front(), base64Str, Len);
    return Ret;
  }
}
