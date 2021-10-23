#ifndef VM_Common
#define VM_Common

#include <stdint.h>


uint64_t ExtendByte( uint8_t leByte, bool SignExtend )
{
    uint8_t SignBit = leByte & 0x80;

    uint64_t RawExtendedByte = (uint64_t) leByte;

    uint64_t BottomMask = 0xff;
    uint64_t TopMask = 0xffffffffffffff00;

    return (SignExtend && (SignBit != 0) ) ? ( TopMask | RawExtendedByte ) : ( BottomMask & RawExtendedByte );
}

uint64_t ExtendHWord( uint16_t leHWord, bool SignExtend )
{
    uint16_t SignBit = leHWord & 0x8000;

    uint64_t RawExtendedHWord = (uint64_t) leHWord;

    uint64_t BottomMask = 0xffff;
    uint64_t TopMask = 0xffffffffffff0000;

    return (SignExtend && (SignBit != 0) ) ? ( TopMask | RawExtendedHWord ) : ( BottomMask & RawExtendedHWord );
}

uint64_t ExtendWord( uint32_t leWord, bool SignExtend )
{
    uint32_t SignBit = leWord & 0x80000000;

    uint64_t RawExtendedWord = (uint64_t) leWord;

    uint64_t BottomMask = 0xffffffff;
    uint64_t TopMask = 0xffffffff00000000;

    return (SignExtend && (SignBit != 0) ) ? ( TopMask | RawExtendedWord ) : ( BottomMask & RawExtendedWord );
}











#endif
