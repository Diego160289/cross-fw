#include "CommonUtils.h"

#include <vector>

#define MD5_INIT_STATE_0 0x67452301
#define MD5_INIT_STATE_1 0xefcdab89
#define MD5_INIT_STATE_2 0x98badcfe
#define MD5_INIT_STATE_3 0x10325476

#define MD5_S11  7
#define MD5_S12 12
#define MD5_S13 17
#define MD5_S14 22
#define MD5_S21  5
#define MD5_S22  9
#define MD5_S23 14
#define MD5_S24 20
#define MD5_S31  4
#define MD5_S32 11
#define MD5_S33 16
#define MD5_S34 23
#define MD5_S41  6
#define MD5_S42 10
#define MD5_S43 15
#define MD5_S44 21

#define MD5_T01  0xd76aa478
#define MD5_T02  0xe8c7b756
#define MD5_T03  0x242070db
#define MD5_T04  0xc1bdceee
#define MD5_T05  0xf57c0faf
#define MD5_T06  0x4787c62a
#define MD5_T07  0xa8304613
#define MD5_T08  0xfd469501
#define MD5_T09  0x698098d8
#define MD5_T10  0x8b44f7af
#define MD5_T11  0xffff5bb1
#define MD5_T12  0x895cd7be
#define MD5_T13  0x6b901122
#define MD5_T14  0xfd987193
#define MD5_T15  0xa679438e
#define MD5_T16  0x49b40821

#define MD5_T17  0xf61e2562
#define MD5_T18  0xc040b340
#define MD5_T19  0x265e5a51
#define MD5_T20  0xe9b6c7aa
#define MD5_T21  0xd62f105d
#define MD5_T22  0x02441453
#define MD5_T23  0xd8a1e681
#define MD5_T24  0xe7d3fbc8
#define MD5_T25  0x21e1cde6
#define MD5_T26  0xc33707d6
#define MD5_T27  0xf4d50d87
#define MD5_T28  0x455a14ed
#define MD5_T29  0xa9e3e905
#define MD5_T30  0xfcefa3f8
#define MD5_T31  0x676f02d9
#define MD5_T32  0x8d2a4c8a

#define MD5_T33  0xfffa3942
#define MD5_T34  0x8771f681
#define MD5_T35  0x6d9d6122
#define MD5_T36  0xfde5380c
#define MD5_T37  0xa4beea44
#define MD5_T38  0x4bdecfa9
#define MD5_T39  0xf6bb4b60
#define MD5_T40  0xbebfbc70
#define MD5_T41  0x289b7ec6
#define MD5_T42  0xeaa127fa
#define MD5_T43  0xd4ef3085
#define MD5_T44  0x04881d05
#define MD5_T45  0xd9d4d039
#define MD5_T46  0xe6db99e5
#define MD5_T47  0x1fa27cf8
#define MD5_T48  0xc4ac5665

#define MD5_T49  0xf4292244
#define MD5_T50  0x432aff97
#define MD5_T51  0xab9423a7
#define MD5_T52  0xfc93a039
#define MD5_T53  0x655b59c3
#define MD5_T54  0x8f0ccc92
#define MD5_T55  0xffeff47d
#define MD5_T56  0x85845dd1
#define MD5_T57  0x6fa87e4f
#define MD5_T58  0xfe2ce6e0
#define MD5_T59  0xa3014314
#define MD5_T60  0x4e0811a1
#define MD5_T61  0xf7537e82
#define MD5_T62  0xbd3af235
#define MD5_T63  0x2ad7d2bb
#define MD5_T64  0xeb86d391


static unsigned char PADDING[64] = {
  0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};


class CMD5Checksum
{
public:
  static std::string GetMD5(const void *pBuf, unsigned nLength);
private:
  CMD5Checksum();
  virtual ~CMD5Checksum() {};
  void Transform(unsigned char Block[64]);
  void Update(unsigned char* Input, unsigned long nInputLen);
  std::string Final();
  inline unsigned long RotateLeft(unsigned long x, int n);
  inline void FF( unsigned long& A, unsigned long B, unsigned long C, unsigned long D, unsigned long X, unsigned long S, unsigned long T);
  inline void GG( unsigned long& A, unsigned long B, unsigned long C, unsigned long D, unsigned long X, unsigned long S, unsigned long T);
  inline void HH( unsigned long& A, unsigned long B, unsigned long C, unsigned long D, unsigned long X, unsigned long S, unsigned long T);
  inline void II( unsigned long& A, unsigned long B, unsigned long C, unsigned long D, unsigned long X, unsigned long S, unsigned long T);
  void DWordToByte(unsigned char* Output, unsigned long* Input, unsigned int nLength);
  void ByteToDWord(unsigned long* Output, unsigned char* Input, unsigned int nLength);

private:
  unsigned char  m_lpszBuffer[64];
  unsigned long m_nCount[2];
  unsigned long m_lMD5[4];
};

std::string CMD5Checksum::GetMD5(const void* pBuf, unsigned int nLength)
{
  CMD5Checksum MD5Checksum;
  std::vector<unsigned char> Buf;
  std::copy(
      &reinterpret_cast<const unsigned char*>(pBuf)[0],
      &reinterpret_cast<const unsigned char*>(pBuf)[nLength],
      std::back_inserter(Buf));
  MD5Checksum.Update( &Buf.front(), nLength );
  return MD5Checksum.Final();
}

unsigned long CMD5Checksum::RotateLeft(unsigned long x, int n)
{
  return (x << n) | (x >> (32-n));
}

void CMD5Checksum::FF(unsigned long & A, unsigned long B, unsigned long C, unsigned long D, unsigned long X, unsigned long S, unsigned long T)
{
  unsigned long F = (B & C) | (~B & D);
  A += F + X + T;
  A = RotateLeft(A, S);
  A += B;
}


void CMD5Checksum::GG(unsigned long & A, unsigned long B, unsigned long C, unsigned long D, unsigned long X, unsigned long S, unsigned long T)
{
  unsigned long G = (B & D) | (C & ~D);
  A += G + X + T;
  A = RotateLeft(A, S);
  A += B;
}

void CMD5Checksum::HH(unsigned long & A, unsigned long B, unsigned long C, unsigned long D, unsigned long X, unsigned long S, unsigned long T)
{
  unsigned long H = (B ^ C ^ D);
  A += H + X + T;
  A = RotateLeft(A, S);
  A += B;
}

void CMD5Checksum::II(unsigned long & A, unsigned long B, unsigned long C, unsigned long D, unsigned long X, unsigned long S, unsigned long T)
{
  unsigned long I = (C ^ (B | ~D));
  A += I + X + T;
  A = RotateLeft(A, S);
  A += B;
}

void CMD5Checksum::ByteToDWord(unsigned long * Output, unsigned char* Input, unsigned int nLength)
{
  unsigned int i=0;
  unsigned int j=0;
  for ( ; j < nLength; i++, j += 4)
  {
    Output[i] = (unsigned long)Input[j]     |
          (unsigned long)Input[j+1] << 8  |
          (unsigned long)Input[j+2] << 16 |
          (unsigned long)Input[j+3] << 24;
  }
}

void CMD5Checksum::Transform(unsigned char Block[64])
{
  unsigned long a = m_lMD5[0];
  unsigned long b = m_lMD5[1];
  unsigned long c = m_lMD5[2];
  unsigned long d = m_lMD5[3];
  unsigned long X[16];
  ByteToDWord( X, Block, 64 );
  FF (a, b, c, d, X[ 0], MD5_S11, MD5_T01);
  FF (d, a, b, c, X[ 1], MD5_S12, MD5_T02);
  FF (c, d, a, b, X[ 2], MD5_S13, MD5_T03);
  FF (b, c, d, a, X[ 3], MD5_S14, MD5_T04);
  FF (a, b, c, d, X[ 4], MD5_S11, MD5_T05);
  FF (d, a, b, c, X[ 5], MD5_S12, MD5_T06);
  FF (c, d, a, b, X[ 6], MD5_S13, MD5_T07);
  FF (b, c, d, a, X[ 7], MD5_S14, MD5_T08);
  FF (a, b, c, d, X[ 8], MD5_S11, MD5_T09);
  FF (d, a, b, c, X[ 9], MD5_S12, MD5_T10);
  FF (c, d, a, b, X[10], MD5_S13, MD5_T11);
  FF (b, c, d, a, X[11], MD5_S14, MD5_T12);
  FF (a, b, c, d, X[12], MD5_S11, MD5_T13);
  FF (d, a, b, c, X[13], MD5_S12, MD5_T14);
  FF (c, d, a, b, X[14], MD5_S13, MD5_T15);
  FF (b, c, d, a, X[15], MD5_S14, MD5_T16);
  GG (a, b, c, d, X[ 1], MD5_S21, MD5_T17);
  GG (d, a, b, c, X[ 6], MD5_S22, MD5_T18);
  GG (c, d, a, b, X[11], MD5_S23, MD5_T19);
  GG (b, c, d, a, X[ 0], MD5_S24, MD5_T20);
  GG (a, b, c, d, X[ 5], MD5_S21, MD5_T21);
  GG (d, a, b, c, X[10], MD5_S22, MD5_T22);
  GG (c, d, a, b, X[15], MD5_S23, MD5_T23);
  GG (b, c, d, a, X[ 4], MD5_S24, MD5_T24);
  GG (a, b, c, d, X[ 9], MD5_S21, MD5_T25);
  GG (d, a, b, c, X[14], MD5_S22, MD5_T26);
  GG (c, d, a, b, X[ 3], MD5_S23, MD5_T27);
  GG (b, c, d, a, X[ 8], MD5_S24, MD5_T28);
  GG (a, b, c, d, X[13], MD5_S21, MD5_T29);
  GG (d, a, b, c, X[ 2], MD5_S22, MD5_T30);
  GG (c, d, a, b, X[ 7], MD5_S23, MD5_T31);
  GG (b, c, d, a, X[12], MD5_S24, MD5_T32);
  HH (a, b, c, d, X[ 5], MD5_S31, MD5_T33);
  HH (d, a, b, c, X[ 8], MD5_S32, MD5_T34);
  HH (c, d, a, b, X[11], MD5_S33, MD5_T35);
  HH (b, c, d, a, X[14], MD5_S34, MD5_T36);
  HH (a, b, c, d, X[ 1], MD5_S31, MD5_T37);
  HH (d, a, b, c, X[ 4], MD5_S32, MD5_T38);
  HH (c, d, a, b, X[ 7], MD5_S33, MD5_T39);
  HH (b, c, d, a, X[10], MD5_S34, MD5_T40);
  HH (a, b, c, d, X[13], MD5_S31, MD5_T41);
  HH (d, a, b, c, X[ 0], MD5_S32, MD5_T42);
  HH (c, d, a, b, X[ 3], MD5_S33, MD5_T43);
  HH (b, c, d, a, X[ 6], MD5_S34, MD5_T44);
  HH (a, b, c, d, X[ 9], MD5_S31, MD5_T45);
  HH (d, a, b, c, X[12], MD5_S32, MD5_T46);
  HH (c, d, a, b, X[15], MD5_S33, MD5_T47);
  HH (b, c, d, a, X[ 2], MD5_S34, MD5_T48);
  II (a, b, c, d, X[ 0], MD5_S41, MD5_T49);
  II (d, a, b, c, X[ 7], MD5_S42, MD5_T50);
  II (c, d, a, b, X[14], MD5_S43, MD5_T51);
  II (b, c, d, a, X[ 5], MD5_S44, MD5_T52);
  II (a, b, c, d, X[12], MD5_S41, MD5_T53);
  II (d, a, b, c, X[ 3], MD5_S42, MD5_T54);
  II (c, d, a, b, X[10], MD5_S43, MD5_T55);
  II (b, c, d, a, X[ 1], MD5_S44, MD5_T56);
  II (a, b, c, d, X[ 8], MD5_S41, MD5_T57);
  II (d, a, b, c, X[15], MD5_S42, MD5_T58);
  II (c, d, a, b, X[ 6], MD5_S43, MD5_T59);
  II (b, c, d, a, X[13], MD5_S44, MD5_T60);
  II (a, b, c, d, X[ 4], MD5_S41, MD5_T61);
  II (d, a, b, c, X[11], MD5_S42, MD5_T62);
  II (c, d, a, b, X[ 2], MD5_S43, MD5_T63);
  II (b, c, d, a, X[ 9], MD5_S44, MD5_T64);
  m_lMD5[0] += a;
  m_lMD5[1] += b;
  m_lMD5[2] += c;
  m_lMD5[3] += d;
}

CMD5Checksum::CMD5Checksum()
{
  memset( m_lpszBuffer, 0, 64 );
  m_nCount[0] = m_nCount[1] = 0;
  m_lMD5[0] = MD5_INIT_STATE_0;
  m_lMD5[1] = MD5_INIT_STATE_1;
  m_lMD5[2] = MD5_INIT_STATE_2;
  m_lMD5[3] = MD5_INIT_STATE_3;
}

void CMD5Checksum::DWordToByte(unsigned char* Output, unsigned long* Input, unsigned int nLength )
{
  unsigned int i = 0;
  unsigned int j = 0;
  for ( ; j < nLength; i++, j += 4)
  {
    Output[j] =   (unsigned char)(Input[i] & 0xff);
    Output[j+1] = (unsigned char)((Input[i] >> 8) & 0xff);
    Output[j+2] = (unsigned char)((Input[i] >> 16) & 0xff);
    Output[j+3] = (unsigned char)((Input[i] >> 24) & 0xff);
  }
}

std::string CMD5Checksum::Final()
{
  unsigned char Bits[8];
  DWordToByte( Bits, m_nCount, 8 );
  unsigned int nIndex = (unsigned int)((m_nCount[0] >> 3) & 0x3f);
  unsigned int nPadLen = (nIndex < 56) ? (56 - nIndex) : (120 - nIndex);
  Update( PADDING, nPadLen );
  Update( Bits, 8 );
  const int nMD5Size = 16;
  unsigned char lpszMD5[ nMD5Size ];
  DWordToByte( lpszMD5, m_lMD5, nMD5Size );
  std::string strMD5;
  for ( int i=0; i < nMD5Size; i++)
  {
    char Buf[64];
    if (lpszMD5[i] == 0) {
      ::sprintf(Buf, "00");
    }
    else if (lpszMD5[i] <= 15)  {
      ::sprintf(Buf, "0%x", lpszMD5[i]);
    }
    else {
      ::sprintf(Buf, "%x", lpszMD5[i]);
    }
    strMD5 += Buf;
  }
  return strMD5;
}

void CMD5Checksum::Update(unsigned char * Input,  unsigned long nInputLen )
{
  unsigned int nIndex = (unsigned int)((m_nCount[0] >> 3) & 0x3F);
  if ( ( m_nCount[0] += nInputLen << 3 )  <  ( nInputLen << 3) )
  {
    m_nCount[1]++;
  }
  m_nCount[1] += (nInputLen >> 29);
  unsigned int i=0;
  unsigned int nPartLen = 64 - nIndex;
  if (nInputLen >= nPartLen)
  {
    memcpy( &m_lpszBuffer[nIndex], Input, nPartLen );
    Transform( m_lpszBuffer );
    for (i = nPartLen; i + 63 < nInputLen; i += 64)
    {
      Transform( &Input[i] );
    }
    nIndex = 0;
  }
  else
  {
    i = 0;
  }
  memcpy( &m_lpszBuffer[nIndex], &Input[i], nInputLen-i);
}

namespace Common
{
  std::string GetChecksumMD5(const void* data, unsigned length)
  {
    return CMD5Checksum::GetMD5(data, length);
  }
}
