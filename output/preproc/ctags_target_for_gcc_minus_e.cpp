# 1 "c:\\Users\\NAZRUL\\Documents\\GitHub\\AnabaaNazrul.github.io\\AES.cpp"
# 2 "c:\\Users\\NAZRUL\\Documents\\GitHub\\AnabaaNazrul.github.io\\AES.cpp" 2

/*

 ---------------------------------------------------------------------------

 Copyright (c) 1998-2008, Brian Gladman, Worcester, UK. All rights reserved.



 LICENSE TERMS



 The redistribution and use of this software (with or without changes)

 is allowed without the payment of fees or royalties provided that:



  1. source code distributions include the above copyright notice, this

     list of conditions and the following disclaimer;



  2. binary distributions include the above copyright notice, this list

     of conditions and the following disclaimer in their documentation;



  3. the name of the copyright holder is not used to endorse products

     built using this software without specific written permission.



 DISCLAIMER



 This software is provided 'as is' with no explicit or implied warranties

 in respect of its properties, including, but not limited to, correctness

 and/or fitness for purpose.

 ---------------------------------------------------------------------------

 Issue 09/09/2006



 This is an AES implementation that uses only 8-bit byte operations on the

 cipher state (there are options to use 32-bit types if available).



 The combination of mix columns and byte substitution used here is based on

 that developed by Karl Malbrain. His contribution is acknowledged.

 */
# 36 "c:\\Users\\NAZRUL\\Documents\\GitHub\\AnabaaNazrul.github.io\\AES.cpp"
/* This version derived by Mark Tillotson 2012-01-23, tidied up, slimmed down

   and tailored to 8-bit microcontroller abilities and Arduino datatypes.



   The s-box and inverse s-box were retained as tables (0.5kB PROGMEM) but all 

   the other transformations are coded to save table space.  Many efficiency 

   improvments to the routines mix_sub_columns() and inv_mix_sub_columns()

   (mainly common sub-expression elimination).



   Only the routines with precalculated subkey schedule are retained (together

   with set_key() - this does however mean each AES object takes 240 bytes of 

   RAM, alas)



   The CBC routines side-effect the iv argument (so that successive calls work

   together correctly).



   All the encryption and decryption routines work with plain == cipher for

   in-place encryption, note.



*/
# 57 "c:\\Users\\NAZRUL\\Documents\\GitHub\\AnabaaNazrul.github.io\\AES.cpp"
/* functions for finite field multiplication in the AES Galois field    */

/* code was modified by george spanos <spaniakos@gmail.com>

 * 16/12/14

 */
# 63 "c:\\Users\\NAZRUL\\Documents\\GitHub\\AnabaaNazrul.github.io\\AES.cpp"
// GF(2^8) stuff




static const byte s_fwd [0x100] 
# 68 "c:\\Users\\NAZRUL\\Documents\\GitHub\\AnabaaNazrul.github.io\\AES.cpp" 3
                               __attribute__((section( "\".irom.text." "AES.cpp" "." "68" "." "9" "\""))) 
# 68 "c:\\Users\\NAZRUL\\Documents\\GitHub\\AnabaaNazrul.github.io\\AES.cpp"
                                       =
{
  0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
  0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
  0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
  0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
  0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
  0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
  0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
  0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
  0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
  0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
  0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
  0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
  0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
  0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
  0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
  0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16,
} ;

static const byte s_inv [0x100] 
# 88 "c:\\Users\\NAZRUL\\Documents\\GitHub\\AnabaaNazrul.github.io\\AES.cpp" 3
                               __attribute__((section( "\".irom.text." "AES.cpp" "." "88" "." "10" "\""))) 
# 88 "c:\\Users\\NAZRUL\\Documents\\GitHub\\AnabaaNazrul.github.io\\AES.cpp"
                                       =
{
  0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,
  0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,
  0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
  0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,
  0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,
  0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
  0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,
  0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,
  0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
  0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,
  0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,
  0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
  0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,
  0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,
  0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
  0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d,
} ;

// times 2 in the GF(2^8)



static byte s_box (byte x)
{
  //  return fwd_affine (pgm_read_byte (&inv [x])) ;
  return 
# 115 "c:\\Users\\NAZRUL\\Documents\\GitHub\\AnabaaNazrul.github.io\\AES.cpp" 3
        pgm_read_byte_inlined(
# 115 "c:\\Users\\NAZRUL\\Documents\\GitHub\\AnabaaNazrul.github.io\\AES.cpp"
        & s_fwd [x]
# 115 "c:\\Users\\NAZRUL\\Documents\\GitHub\\AnabaaNazrul.github.io\\AES.cpp" 3
        ) 
# 115 "c:\\Users\\NAZRUL\\Documents\\GitHub\\AnabaaNazrul.github.io\\AES.cpp"
                                    ;
}

// Inverse Sbox
static byte is_box (byte x)
{
  // return pgm_read_byte (&inv [inv_affine (x)]) ;
  return 
# 122 "c:\\Users\\NAZRUL\\Documents\\GitHub\\AnabaaNazrul.github.io\\AES.cpp" 3
        pgm_read_byte_inlined(
# 122 "c:\\Users\\NAZRUL\\Documents\\GitHub\\AnabaaNazrul.github.io\\AES.cpp"
        & s_inv [x]
# 122 "c:\\Users\\NAZRUL\\Documents\\GitHub\\AnabaaNazrul.github.io\\AES.cpp" 3
        ) 
# 122 "c:\\Users\\NAZRUL\\Documents\\GitHub\\AnabaaNazrul.github.io\\AES.cpp"
                                    ;
}


static void xor_block (byte * d, byte * s)
{
  for (byte i = 0 ; i < (4 * 4) ; i += 4)
    {
      *d++ ^= *s++ ; // some unrolling
      *d++ ^= *s++ ;
      *d++ ^= *s++ ;
      *d++ ^= *s++ ;
    }
}

static void copy_and_key (byte * d, byte * s, byte * k)
{
  for (byte i = 0 ; i < (4 * 4) ; i += 4)
    {
      *d++ = *s++ ^ *k++ ; // some unrolling
      *d++ = *s++ ^ *k++ ;
      *d++ = *s++ ^ *k++ ;
      *d++ = *s++ ^ *k++ ;
    }
}

// #define add_round_key(d, k) xor_block (d, k)

/* SUB ROW PHASE */

static void shift_sub_rows (byte st [(4 * 4)])
{
  st [0] = s_box (st [0]) ; st [4] = s_box (st [4]) ;
  st [8] = s_box (st [8]) ; st [12] = s_box (st [12]) ;

  byte tt = st [1] ;
  st [1] = s_box (st [5]) ; st [5] = s_box (st [9]) ;
  st [9] = s_box (st [13]) ; st [13] = s_box (tt) ;

  tt = st[2] ; st [2] = s_box (st [10]) ; st [10] = s_box (tt) ;
  tt = st[6] ; st [6] = s_box (st [14]) ; st [14] = s_box (tt) ;

  tt = st[15] ;
  st [15] = s_box (st [11]) ; st [11] = s_box (st [7]) ;
  st [7] = s_box (st [3]) ; st [3] = s_box (tt) ;
}

static void inv_shift_sub_rows (byte st[(4 * 4)])
{
  st [0] = is_box (st[0]) ; st [4] = is_box (st [4]);
  st [8] = is_box (st[8]) ; st [12] = is_box (st [12]);

  byte tt = st[13] ;
  st [13] = is_box (st [9]) ; st [9] = is_box (st [5]) ;
  st [5] = is_box (st [1]) ; st [1] = is_box (tt) ;

  tt = st [2] ; st [2] = is_box (st [10]) ; st [10] = is_box (tt) ;
  tt = st [6] ; st [6] = is_box (st [14]) ; st [14] = is_box (tt) ;

  tt = st [3] ;
  st [3] = is_box (st [7]) ; st [7] = is_box (st [11]) ;
  st [11] = is_box (st [15]) ; st [15] = is_box (tt) ;
}

/* SUB COLUMNS PHASE */

static void mix_sub_columns (byte dt[(4 * 4)], byte st[(4 * 4)])
{
  byte j = 5 ;
  byte k = 10 ;
  byte l = 15 ;
  for (byte i = 0 ; i < (4 * 4) ; i += 4)
    {
      byte a = st [i] ;
      byte b = st [j] ; j = (j+4) & 15 ;
      byte c = st [k] ; k = (k+4) & 15 ;
      byte d = st [l] ; l = (l+4) & 15 ;
      byte a1 = s_box (a), b1 = s_box (b), c1 = s_box (c), d1 = s_box (d) ;
      byte a2 = ((a1) & 0x80 ? (a1 << 1) ^ 0x011B : a1 << 1), b2 = ((b1) & 0x80 ? (b1 << 1) ^ 0x011B : b1 << 1), c2 = ((c1) & 0x80 ? (c1 << 1) ^ 0x011B : c1 << 1), d2 = ((d1) & 0x80 ? (d1 << 1) ^ 0x011B : d1 << 1) ;
      dt[i] = a2 ^ b2^b1 ^ c1 ^ d1 ;
      dt[i+1] = a1 ^ b2 ^ c2^c1 ^ d1 ;
      dt[i+2] = a1 ^ b1 ^ c2 ^ d2^d1 ;
      dt[i+3] = a2^a1 ^ b1 ^ c1 ^ d2 ;
    }
}

static void inv_mix_sub_columns (byte dt[(4 * 4)], byte st[(4 * 4)])
{
  for (byte i = 0 ; i < (4 * 4) ; i += 4)
    {
      byte a1 = st [i] ;
      byte b1 = st [i+1] ;
      byte c1 = st [i+2] ;
      byte d1 = st [i+3] ;
      byte a2 = ((a1) & 0x80 ? (a1 << 1) ^ 0x011B : a1 << 1), b2 = ((b1) & 0x80 ? (b1 << 1) ^ 0x011B : b1 << 1), c2 = ((c1) & 0x80 ? (c1 << 1) ^ 0x011B : c1 << 1), d2 = ((d1) & 0x80 ? (d1 << 1) ^ 0x011B : d1 << 1) ;
      byte a4 = ((a2) & 0x80 ? (a2 << 1) ^ 0x011B : a2 << 1), b4 = ((b2) & 0x80 ? (b2 << 1) ^ 0x011B : b2 << 1), c4 = ((c2) & 0x80 ? (c2 << 1) ^ 0x011B : c2 << 1), d4 = ((d2) & 0x80 ? (d2 << 1) ^ 0x011B : d2 << 1) ;
      byte a8 = ((a4) & 0x80 ? (a4 << 1) ^ 0x011B : a4 << 1), b8 = ((b4) & 0x80 ? (b4 << 1) ^ 0x011B : b4 << 1), c8 = ((c4) & 0x80 ? (c4 << 1) ^ 0x011B : c4 << 1), d8 = ((d4) & 0x80 ? (d4 << 1) ^ 0x011B : d4 << 1) ;
      byte a9 = a8 ^ a1,b9 = b8 ^ b1,c9 = c8 ^ c1,d9 = d8 ^ d1 ;
      byte ac = a8 ^ a4,bc = b8 ^ b4,cc = c8 ^ c4,dc = d8 ^ d4 ;

      dt[i] = is_box (ac^a2 ^ b9^b2 ^ cc^c1 ^ d9) ;
      dt[(i+5)&15] = is_box (a9 ^ bc^b2 ^ c9^c2 ^ dc^d1) ;
      dt[(i+10)&15] = is_box (ac^a1 ^ b9 ^ cc^c2 ^ d9^d2) ;
      dt[(i+15)&15] = is_box (a9^a2 ^ bc^b1 ^ c9 ^ dc^d2) ;
    }
}

/******************************************************************************/

AES::AES(){
  byte ar_iv[8] = { 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01 };
  memcpy(iv,ar_iv,8);
  memcpy(iv+8,ar_iv,8);
  arr_pad[0] = 0x01;
  arr_pad[1] = 0x02;
  arr_pad[2] = 0x03;
  arr_pad[3] = 0x04;
  arr_pad[4] = 0x05;
  arr_pad[5] = 0x06;
  arr_pad[6] = 0x07;
  arr_pad[7] = 0x08;
  arr_pad[8] = 0x09;
  arr_pad[9] = 0x0a;
  arr_pad[10] = 0x0b;
  arr_pad[11] = 0x0c;
  arr_pad[12] = 0x0d;
  arr_pad[13] = 0x0e;
  arr_pad[14] = 0x0f;
}

/******************************************************************************/

byte AES::set_key (byte key [], int keylen)
{
  byte hi ;
  switch (keylen)
    {
    case 16:
    case 128:
      keylen = 16; // 10 rounds
      round = 10 ;
      break;
    case 24:
    case 192:
      keylen = 24; // 12 rounds
      round = 12 ;
      break;
    case 32:
    case 256:
      keylen = 32; // 14 rounds
      round = 14 ;
      break;
    default:
      round = 0;
      return (-1);
    }
  hi = (round + 1) << 4 ;
  copy_n_bytes (key_sched, key, keylen) ;
  byte t[4] ;
  byte next = keylen ;
  for (byte cc = keylen, rc = 1 ; cc < hi ; cc += 4)
    {
      for (byte i = 0 ; i < 4 ; i++)
        t[i] = key_sched [cc-4+i] ;
      if (cc == next)
        {
          next += keylen ;
          byte ttt = t[0] ;
          t[0] = s_box (t[1]) ^ rc ;
          t[1] = s_box (t[2]) ;
          t[2] = s_box (t[3]) ;
          t[3] = s_box (ttt) ;
          rc = ((rc) & 0x80 ? (rc << 1) ^ 0x011B : rc << 1) ;
        }
      else if (keylen == 32 && (cc & 31) == 16)
        {
          for (byte i = 0 ; i < 4 ; i++)
            t[i] = s_box (t[i]) ;
        }
      byte tt = cc - keylen ;
      for (byte i = 0 ; i < 4 ; i++)
        key_sched [cc + i] = key_sched [tt + i] ^ t[i] ;
    }
  return (0) ;
}

/******************************************************************************/

void AES::clean ()
{
  for (byte i = 0 ; i < ((14 + 1) * (4 * 4)) ; i++)
    key_sched [i] = 0 ;
  round = 0 ;
}

/******************************************************************************/

void AES::copy_n_bytes (byte * d, byte * s, byte nn)
{
  while (nn >= 4)
    {
      *d++ = *s++ ; // some unrolling
      *d++ = *s++ ;
      *d++ = *s++ ;
      *d++ = *s++ ;
      nn -= 4 ;
    }
  while (nn--)
    *d++ = *s++ ;
}

/******************************************************************************/

byte AES::encrypt (byte plain [(4 * 4)], byte cipher [(4 * 4)])
{
  if (round)
    {
      byte s1 [(4 * 4)], r ;
      copy_and_key (s1, plain, (byte*) (key_sched)) ;

      for (r = 1 ; r < round ; r++)
        {
          byte s2 [(4 * 4)] ;
          mix_sub_columns (s2, s1) ;
          copy_and_key (s1, s2, (byte*) (key_sched + r * (4 * 4))) ;
        }
      shift_sub_rows (s1) ;
      copy_and_key (cipher, s1, (byte*) (key_sched + r * (4 * 4))) ;
    }
  else
    return (-1) ;
  return (0) ;
}

/******************************************************************************/

byte AES::cbc_encrypt (byte * plain, byte * cipher, int n_block, byte iv [(4 * 4)])
{
  while (n_block--)
    {
      xor_block (iv, plain) ;
      if (encrypt (iv, iv) != (0))
        return (-1) ;
      copy_n_bytes (cipher, iv, (4 * 4)) ;
      plain += (4 * 4) ;
      cipher += (4 * 4) ;
    }
  return (0) ;
}

/******************************************************************************/

byte AES::cbc_encrypt (byte * plain, byte * cipher, int n_block)
{
  while (n_block--)
    {
    xor_block (iv, plain) ;
      if (encrypt (iv, iv) != (0))
        return (-1) ;
      copy_n_bytes (cipher, iv, (4 * 4)) ;
      plain += (4 * 4) ;
      cipher += (4 * 4) ;
    }
  return (0) ;
}

/******************************************************************************/

byte AES::decrypt (byte plain [(4 * 4)], byte cipher [(4 * 4)])
{
  if (round)
    {
      byte s1 [(4 * 4)] ;
      copy_and_key (s1, plain, (byte*) (key_sched + round * (4 * 4))) ;
      inv_shift_sub_rows (s1) ;

      for (byte r = round ; --r ; )
       {
         byte s2 [(4 * 4)] ;
         copy_and_key (s2, s1, (byte*) (key_sched + r * (4 * 4))) ;
         inv_mix_sub_columns (s1, s2) ;
       }
      copy_and_key (cipher, s1, (byte*) (key_sched)) ;
    }
  else
    return (-1) ;
  return (0) ;
}

/******************************************************************************/

byte AES::cbc_decrypt (byte * cipher, byte * plain, int n_block, byte iv [(4 * 4)])
{
  while (n_block--)
    {
      byte tmp [(4 * 4)] ;
      copy_n_bytes (tmp, cipher, (4 * 4)) ;
      if (decrypt (cipher, plain) != (0))
        return (-1) ;
      xor_block (plain, iv) ;
      copy_n_bytes (iv, tmp, (4 * 4)) ;
      plain += (4 * 4) ;
      cipher += (4 * 4);
    }
  return (0) ;
}

/******************************************************************************/

byte AES::cbc_decrypt (byte * cipher, byte * plain, int n_block)
{
  while (n_block--)
    {
      byte tmp [(4 * 4)] ;
      copy_n_bytes (tmp, cipher, (4 * 4)) ;
      if (decrypt (cipher, plain) != (0))
        return (-1) ;
      xor_block (plain, iv) ;
      copy_n_bytes (iv, tmp, (4 * 4)) ;
      plain += (4 * 4) ;
      cipher += (4 * 4);
    }
  return (0) ;
}

/*****************************************************************************/

void AES::set_IV(unsigned long long int IVCl){
  memcpy(iv,&IVCl,8);
  memcpy(iv+8,&IVCl,8);
  IVC = IVCl;
}

/******************************************************************************/

void AES::iv_inc(){
  IVC += 1;
  memcpy(iv,&IVC,8);
  memcpy(iv+8,&IVC,8);
}

/******************************************************************************/

int AES::get_size(){
  return size;
}

/******************************************************************************/

void AES::set_size(int sizel){
  size = sizel;
}


/******************************************************************************/

void AES::get_IV(byte *out){
  memcpy(out,&IVC,8);
  memcpy(out+8,&IVC,8);
}

/******************************************************************************/

void AES::calc_size_n_pad(int p_size){
  int s_of_p = p_size - 1;
  if ( s_of_p % (4 * 4) == 0){
      size = s_of_p;
  }else{
    size = s_of_p + ((4 * 4)-(s_of_p % (4 * 4)));
  }
  pad = size - s_of_p;
}

/******************************************************************************/

void AES::padPlaintext(void* in,byte* out)
{
  memcpy(out,in,size);
  for (int i = size-pad; i < size; i++){;
    out[i] = arr_pad[pad - 1];
  }
}

/******************************************************************************/

bool AES::CheckPad(byte* in,int lsize){
  if (in[lsize-1] <= 0x0f){
    int lpad = (int)in[lsize-1];
    for (int i = lsize - 1; i >= lsize-lpad; i--){
      if (arr_pad[lpad - 1] != in[i]){
        return false;
      }
    }
  }else{
    return true;
  }
return true;
}

/******************************************************************************/

void AES::printArray(byte output[],bool p_pad)
{
uint8_t i,j;
uint8_t loops = size/(4 * 4);
uint8_t outp = (4 * 4);
for (j = 0; j < loops; j += 1){
  if (p_pad && (j == (loops - 1)) ) { outp = (4 * 4) - pad; }
  for (i = 0; i < outp; i++)
  {
    printf_P(
# 532 "c:\\Users\\NAZRUL\\Documents\\GitHub\\AnabaaNazrul.github.io\\AES.cpp" 3
            (__extension__({static const char __pstr__[] __attribute__((__aligned__(4))) __attribute__((section( "\".irom0.pstr." "AES.cpp" "." "532" "." "11" "\", \"aSM\", @progbits, 1 #"))) = (
# 532 "c:\\Users\\NAZRUL\\Documents\\GitHub\\AnabaaNazrul.github.io\\AES.cpp"
            "%c"
# 532 "c:\\Users\\NAZRUL\\Documents\\GitHub\\AnabaaNazrul.github.io\\AES.cpp" 3
            ); &__pstr__[0];}))
# 532 "c:\\Users\\NAZRUL\\Documents\\GitHub\\AnabaaNazrul.github.io\\AES.cpp"
                      ,output[j*(4 * 4) + i]);
  }
}
  printf_P(
# 535 "c:\\Users\\NAZRUL\\Documents\\GitHub\\AnabaaNazrul.github.io\\AES.cpp" 3
          (__extension__({static const char __pstr__[] __attribute__((__aligned__(4))) __attribute__((section( "\".irom0.pstr." "AES.cpp" "." "535" "." "12" "\", \"aSM\", @progbits, 1 #"))) = (
# 535 "c:\\Users\\NAZRUL\\Documents\\GitHub\\AnabaaNazrul.github.io\\AES.cpp"
          "\n"
# 535 "c:\\Users\\NAZRUL\\Documents\\GitHub\\AnabaaNazrul.github.io\\AES.cpp" 3
          ); &__pstr__[0];}))
# 535 "c:\\Users\\NAZRUL\\Documents\\GitHub\\AnabaaNazrul.github.io\\AES.cpp"
                    );
}

/******************************************************************************/

void AES::printArray(byte output[],int sizel)
{
  for (int i = 0; i < sizel; i++)
  {
    printf_P(
# 544 "c:\\Users\\NAZRUL\\Documents\\GitHub\\AnabaaNazrul.github.io\\AES.cpp" 3
            (__extension__({static const char __pstr__[] __attribute__((__aligned__(4))) __attribute__((section( "\".irom0.pstr." "AES.cpp" "." "544" "." "13" "\", \"aSM\", @progbits, 1 #"))) = (
# 544 "c:\\Users\\NAZRUL\\Documents\\GitHub\\AnabaaNazrul.github.io\\AES.cpp"
            "%x"
# 544 "c:\\Users\\NAZRUL\\Documents\\GitHub\\AnabaaNazrul.github.io\\AES.cpp" 3
            ); &__pstr__[0];}))
# 544 "c:\\Users\\NAZRUL\\Documents\\GitHub\\AnabaaNazrul.github.io\\AES.cpp"
                      ,output[i]);
  }
  printf_P(
# 546 "c:\\Users\\NAZRUL\\Documents\\GitHub\\AnabaaNazrul.github.io\\AES.cpp" 3
          (__extension__({static const char __pstr__[] __attribute__((__aligned__(4))) __attribute__((section( "\".irom0.pstr." "AES.cpp" "." "546" "." "14" "\", \"aSM\", @progbits, 1 #"))) = (
# 546 "c:\\Users\\NAZRUL\\Documents\\GitHub\\AnabaaNazrul.github.io\\AES.cpp"
          "\n"
# 546 "c:\\Users\\NAZRUL\\Documents\\GitHub\\AnabaaNazrul.github.io\\AES.cpp" 3
          ); &__pstr__[0];}))
# 546 "c:\\Users\\NAZRUL\\Documents\\GitHub\\AnabaaNazrul.github.io\\AES.cpp"
                    );
}


/******************************************************************************/

void AES::do_aes_encrypt(byte *plain,int size_p,byte *cipher,byte *key, int bits, byte ivl [(4 * 4)]){
  calc_size_n_pad(size_p);
  byte plain_p[get_size()];
  padPlaintext(plain,plain_p);
  int blocks = get_size() / (4 * 4);
  set_key (key, bits) ;
  cbc_encrypt (plain_p, cipher, blocks, ivl);
}

/******************************************************************************/

void AES::do_aes_encrypt(byte *plain,int size_p,byte *cipher,byte *key, int bits){
  calc_size_n_pad(size_p);
  byte plain_p[get_size()];
  padPlaintext(plain,plain_p);
  int blocks = get_size() / (4 * 4);
  set_key (key, bits) ;
  cbc_encrypt (plain_p, cipher, blocks);
}

/******************************************************************************/

void AES::do_aes_decrypt(byte *cipher,int size_c,byte *plain,byte *key, int bits, byte ivl [(4 * 4)]){
  set_size(size_c);
  int blocks = size_c / (4 * 4);
  set_key (key, bits);
  cbc_decrypt (cipher,plain, blocks, ivl);
}

/******************************************************************************/

void AES::do_aes_decrypt(byte *cipher,int size_c,byte *plain,byte *key, int bits){
  set_size(size_c);
  int blocks = size_c / (4 * 4);
  set_key (key, bits);
  cbc_decrypt (cipher,plain, blocks);
}


/******************************************************************************/
# 1 "c:\\Users\\NAZRUL\\Documents\\GitHub\\AnabaaNazrul.github.io\\wificonnect.ino"
# 2 "c:\\Users\\NAZRUL\\Documents\\GitHub\\AnabaaNazrul.github.io\\wificonnect.ino" 2
# 3 "c:\\Users\\NAZRUL\\Documents\\GitHub\\AnabaaNazrul.github.io\\wificonnect.ino" 2
# 4 "c:\\Users\\NAZRUL\\Documents\\GitHub\\AnabaaNazrul.github.io\\wificonnect.ino" 2
# 5 "c:\\Users\\NAZRUL\\Documents\\GitHub\\AnabaaNazrul.github.io\\wificonnect.ino" 2
# 6 "c:\\Users\\NAZRUL\\Documents\\GitHub\\AnabaaNazrul.github.io\\wificonnect.ino" 2






DHT dht(D4, 11 /**< DHT TYPE 11 */);

const char* serverName = "http://192.168.43.23:5000/";


AES aes;

uint8_t getrnd() {
    uint8_t really_random = *(volatile uint8_t *)0x3FF20E44;
    return really_random;
}

// Generate a random initialization vector
void gen_iv(byte *iv) {
    for (int i = 0 ; i < (4 * 4) ; i++ ) {
        iv[i]= (byte) getrnd();
    }
}

void setup()
{

  Serial.begin(115200);

  dht.begin();

  WiFi.begin("OPPO A37f", "123456789");
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

}

String sensorUpdate(){
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);

  if (isnan(h) || isnan(t) || isnan(f)) {
    return ("Failed to read from DHT sensor!");
  }



  String result = "Humidity: " + String(h)+"%";
  String result2 ="   Temperature: " + String(t)+" Celsius ";

  String result_last = result + result2;

  return result_last;


}
void loop() {

  String display = sensorUpdate();

  char b64data[200];
    byte cipher[1000];
    byte iv [(4 * 4)];

    Serial.println("Encrypting....:");
    // Our AES key. Note that is the same that is used on the Node-Js side but as hex bytes.
    byte key[] = { 0x2B, 0x7E, 0x15, 0x16, 0x28, 0xAE, 0xD2, 0xA6, 0xAB, 0xF7, 0x15, 0x88, 0x09, 0xCF, 0x4F, 0x3C };

    // The unitialized Initialization vector
    byte my_iv[(4 * 4)] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

    // Our message to encrypt. Static for this example.
    String msg = sensorUpdate();

    aes.set_key( key , sizeof(key)); // Get the globally defined key
    gen_iv( my_iv ); // Generate a random IV

    // Print the IV
    base64_encode( b64data, (char *)my_iv, (4 * 4));
    Serial.println(" Your IV b64: " + String(b64data));
    String data_iv = ",\"iv\" :\""+ String(b64data)+"\"";


    Serial.println(" Message: " + msg );

     int b64len = base64_encode(b64data, (char *)msg.c_str(),msg.length());
    Serial.println (" Message in B64: " + String(b64data) );
    Serial.println (" The lenght is:  " + String(b64len) );

    // Encrypt! With AES128, our key and IV, CBC and pkcs7 padding    
    aes.do_aes_encrypt((byte *)b64data, b64len , cipher, key, 128, my_iv);

    Serial.println("Encryption done!");

    Serial.println("Cipher size: " + String(aes.get_size()));

    base64_encode(b64data, (char *)cipher, aes.get_size() );
    Serial.println ("Encrypted data in base64: " + String(b64data) );

    Serial.println("Done...");



  Serial.print(display);

  if(WiFi.status()== WL_CONNECTED){
      WiFiClient client;
      HTTPClient http;

      http.begin(client, serverName);

       http.addHeader("Content-Type", "application/json");
       int httpResponseCode = http.POST("{\"data\":\""+String(b64data)+"\""+data_iv+"}");

      Serial.print("  HTTP Response code: ");
      Serial.println(httpResponseCode);

      http.end();
    }
    else {
      Serial.println("WiFi Disconnected");
    }

  delay(5000);
}
