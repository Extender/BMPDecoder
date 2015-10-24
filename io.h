#ifndef IO_H
#define IO_H

#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#ifndef fs_t
#define fs_t uint32_t
#endif

#define extendBufferSize(bufferSize,pos) while(pos>=bufferSize) bufferSize*=2

class io
{
public:
    static uint8_t readUInt8(char *&data);
    static uint16_t readUInt16(char *&data);
    static uint32_t readUInt32(char *&data);
    static uint64_t readUInt64(char *&data);
    static fs_t readFsT(char *&data);
    static char *readFixedLengthData(char *&data,fs_t &length);
    static char *readZeroTerminatedData(char *&data);

    static uint8_t peekUInt8(char *data,fs_t pos);
    static uint16_t peekUInt16(char *data,fs_t pos);
    static uint32_t peekUInt32(char *data,fs_t pos);
    static uint64_t peekUInt64(char *data,fs_t pos);
    static fs_t peekFsT(char *data,fs_t pos);
    static char *peekFixedLengthData(char *data,fs_t pos,fs_t &length);
    static char *peekZeroTerminatedData(char *data,fs_t pos);

    static uint8_t posBasedReadUInt8(char *data,fs_t &pos);
    static uint16_t posBasedReadUInt16(char *data,fs_t &pos);
    static uint32_t posBasedReadUInt32(char *data,fs_t &pos);
    static uint64_t posBasedReadUInt64(char *data,fs_t &pos);
    static fs_t posBasedReadFsT(char *data,fs_t &pos);
    static char *posBasedReadFixedLengthData(char *data,fs_t &pos,fs_t &length);
    static char *posBasedReadZeroTerminatedData(char *data,fs_t &pos);

    static void writeUInt8(char *data, uint8_t i, fs_t &pos);
    static void writeUInt16(char *data, uint16_t i, fs_t &pos);
    static void writeUInt32(char *data, uint32_t i, fs_t &pos);
    static void writeUInt64(char *data, uint64_t i, fs_t &pos);
    static void writeFsT(char *data, fs_t i, fs_t &pos);
    static void writeFixedLengthData(char *data, fs_t length, char *in, fs_t &pos);
    static void writeZeroTerminatedData(char *data, char *in, fs_t &pos);
    static void writeRawData(char *data, char *in, fs_t length, fs_t &pos);

    static void writeUInt8ToBuffer(char *&data, uint8_t i, fs_t &pos, fs_t &bufferSize);
    static void writeUInt16ToBuffer(char *&data, uint16_t i, fs_t &pos, fs_t &bufferSize);
    static void writeUInt32ToBuffer(char *&data, uint32_t i, fs_t &pos, fs_t &bufferSize);
    static void writeUInt64ToBuffer(char *&data, uint64_t i, fs_t &pos, fs_t &bufferSize);
    static void writeFsTToBuffer(char *&data, fs_t i, fs_t &pos, fs_t &bufferSize);
    static void writeFixedLengthDataToBuffer(char *&data, fs_t length, char *in, fs_t &pos, fs_t &bufferSize);
    static void writeZeroTerminatedDataToBuffer(char *&data, char *in, fs_t &pos, fs_t &bufferSize);
    static void writeRawDataToBuffer(char *&data, char *in, fs_t length, fs_t &pos, fs_t &bufferSize);
    static void writeRawDataToLongBuffer(char *&data, char *in, uint64_t length, uint64_t &pos, uint64_t &bufferSize);
    static void writeRawCharToBuffer(char *&data, char in, fs_t &pos, fs_t &bufferSize);

    static void terminateBuffer(char *buffer, fs_t &pos, fs_t bufferSize);
    static bool bufferCheck(char *&buffer,fs_t pos,fs_t &bufferSize);
    static bool longBufferCheck(char *&buffer,uint64_t pos,uint64_t &bufferSize);
};

#endif // IO_H
