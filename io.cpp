#include "io.h"

uint8_t io::readUInt8(char *&data)
{
    return (uint8_t)*(data++);
}

uint16_t io::readUInt16(char *&data)
{
    uint16_t out=(uint16_t)*(data++);
    out|=(uint16_t)((uint8_t)*(data++))<<8;
    return out;
}

uint32_t io::readUInt32(char *&data)
{
    uint32_t out=(uint8_t)*(data++);
    out|=(uint32_t)((uint8_t)*(data++))<<8;
    out|=(uint32_t)((uint8_t)*(data++))<<16;
    out|=(uint32_t)((uint8_t)*(data++))<<24;
    return out;
}

uint64_t io::readUInt64(char *&data)
{
    uint64_t out=(uint8_t)*(data++);
    out|=(uint64_t)((uint8_t)*(data++))<<8;
    out|=(uint64_t)((uint8_t)*(data++))<<16;
    out|=(uint64_t)((uint8_t)*(data++))<<24;
    out|=(uint64_t)((uint8_t)*(data++))<<32;
    out|=(uint64_t)((uint8_t)*(data++))<<40;
    out|=(uint64_t)((uint8_t)*(data++))<<48;
    out|=(uint64_t)((uint8_t)*(data++))<<56;
    return out;
}

fs_t io::readFsT(char *&data)
{
    return readUInt32(data);
}

char *io::readFixedLengthData(char *&data, fs_t &length)
{
    length=readFsT(data);
    char *out=(char*)calloc(length,1);
    for(fs_t i=0;i<length;i++)
        out[i]=*(data++);
    return out;
}

char *io::readZeroTerminatedData(char *&data)
{
    fs_t length=strlen(data);
    char *out=(char*)calloc(length+1,1);
    for(fs_t i=0;i<length;i++)
        out[i]=*(data++);
    data++; // Zero-terminator
    return out;
}

uint8_t io::peekUInt8(char *data, fs_t pos)
{
    return (uint8_t)data[pos++];
}

uint16_t io::peekUInt16(char *data, fs_t pos)
{
    uint16_t out=data[pos++];
    out|=(uint16_t)data[pos++]<<8;
    return out;
}

uint32_t io::peekUInt32(char *data, fs_t pos)
{
    uint32_t out=data[pos++];
    out|=(uint32_t)data[pos++]<<8;
    out|=(uint32_t)data[pos++]<<16;
    out|=(uint32_t)data[pos++]<<24;
    return out;
}

uint64_t io::peekUInt64(char *data, fs_t pos)
{
    uint64_t out=data[pos++];
    out|=(uint64_t)data[pos++]<<8;
    out|=(uint64_t)data[pos++]<<16;
    out|=(uint64_t)data[pos++]<<24;
    out|=(uint64_t)data[pos++]<<32;
    out|=(uint64_t)data[pos++]<<40;
    out|=(uint64_t)data[pos++]<<48;
    out|=(uint64_t)data[pos++]<<56;
    return out;
}

fs_t io::peekFsT(char *data, fs_t pos)
{
    return peekUInt32(data,pos);
}

char *io::peekFixedLengthData(char *data, fs_t pos, fs_t &length)
{
    length=posBasedReadFsT(data,pos);
    char *out=(char*)calloc(length,1);
    for(fs_t i=0;i<length;i++)
        out[i]=data[pos++];
    return out;
}

char *io::peekZeroTerminatedData(char *data, fs_t pos)
{
    fs_t length=strlen(data);
    char *out=(char*)calloc(length+1,1);
    for(fs_t i=0;i<length;i++)
        out[i]=data[pos++];
    pos++; // Zero-terminator
    return out;
}

uint8_t io::posBasedReadUInt8(char *data, fs_t &pos)
{
    return (uint8_t)data[pos++];
}

uint16_t io::posBasedReadUInt16(char *data, fs_t &pos)
{
    uint16_t out=data[pos++];
    out|=(uint16_t)data[pos++]<<8;
    return out;
}

uint32_t io::posBasedReadUInt32(char *data, fs_t &pos)
{
    uint32_t out=data[pos++];
    out|=(uint32_t)data[pos++]<<8;
    out|=(uint32_t)data[pos++]<<16;
    out|=(uint32_t)data[pos++]<<24;
    return out;
}

uint64_t io::posBasedReadUInt64(char *data, fs_t &pos)
{
    uint64_t out=data[pos++];
    out|=(uint64_t)data[pos++]<<8;
    out|=(uint64_t)data[pos++]<<16;
    out|=(uint64_t)data[pos++]<<24;
    out|=(uint64_t)data[pos++]<<32;
    out|=(uint64_t)data[pos++]<<40;
    out|=(uint64_t)data[pos++]<<48;
    out|=(uint64_t)data[pos++]<<56;
    return out;
}

fs_t io::posBasedReadFsT(char *data, fs_t &pos)
{
    return posBasedReadUInt32(data,pos);
}

char *io::posBasedReadFixedLengthData(char *data, fs_t &pos, fs_t &length)
{
    length=posBasedReadFsT(data,pos);
    char *out=(char*)calloc(length,1);
    for(fs_t i=0;i<length;i++)
        out[i]=data[pos++];
    return out;
}

char *io::posBasedReadZeroTerminatedData(char *data, fs_t &pos)
{
    fs_t length=strlen(data+pos);
    char *out=(char*)calloc(length+1,1);
    for(fs_t i=0;i<length;i++)
        out[i]=data[pos++];
    pos++; // Zero-terminator
    return out;
}

void io::writeUInt8(char *data, uint8_t i, fs_t &pos)
{
    data[pos++]=i;
}

void io::writeUInt16(char *data, uint16_t i, fs_t &pos)
{
    data[pos++]=(uint8_t)i;
    data[pos++]=(uint8_t)(i>>8);
}

void io::writeUInt32(char *data, uint32_t i, fs_t &pos)
{
    data[pos++]=(uint8_t)i;
    data[pos++]=(uint8_t)(i>>8);
    data[pos++]=(uint8_t)(i>>16);
    data[pos++]=(uint8_t)(i>>24);
}

void io::writeUInt64(char *data, uint64_t i, fs_t &pos)
{
    data[pos++]=(uint8_t)i;
    data[pos++]=(uint8_t)(i>>8);
    data[pos++]=(uint8_t)(i>>16);
    data[pos++]=(uint8_t)(i>>24);
    data[pos++]=(uint8_t)(i>>32);
    data[pos++]=(uint8_t)(i>>40);
    data[pos++]=(uint8_t)(i>>48);
    data[pos++]=(uint8_t)(i>>56);
}

void io::writeFsT(char *data, fs_t i, fs_t &pos)
{
    writeUInt32(data,i,pos);
}

void io::writeFixedLengthData(char *data, fs_t length, char *in, fs_t &pos)
{
    writeFsT(data,length,pos);
    for(fs_t i=0;i<length;i++)
        data[pos++]=(uint8_t)in[i];
}

void io::writeZeroTerminatedData(char *data, char *in, fs_t &pos)
{
    fs_t length=strlen(in);
    for(fs_t i=0;i<length;i++)
        data[pos++]=(uint8_t)in[i];
    data[pos++]=0;
}

void io::writeRawData(char *data, char *in, fs_t length, fs_t &pos)
{
    for(fs_t i=0;i<length;i++)
        data[pos++]=(uint8_t)in[i];
}

void io::writeUInt8ToBuffer(char *&data, uint8_t i, fs_t &pos, fs_t &bufferSize)
{
    fs_t newPos=pos+sizeof(uint8_t);
    bufferCheck(data,newPos,bufferSize);
    writeUInt8(data,i,pos);
}

void io::writeUInt16ToBuffer(char *&data, uint16_t i, fs_t &pos, fs_t &bufferSize)
{
    fs_t newPos=pos+sizeof(uint16_t);
    bufferCheck(data,newPos,bufferSize);
    writeUInt16(data,i,pos);
}

void io::writeUInt32ToBuffer(char *&data, uint32_t i, fs_t &pos, fs_t &bufferSize)
{
    fs_t newPos=pos+sizeof(uint32_t);
    bufferCheck(data,newPos,bufferSize);
    writeUInt32(data,i,pos);
}

void io::writeUInt64ToBuffer(char *&data, uint64_t i, fs_t &pos, fs_t &bufferSize)
{
    fs_t newPos=pos+sizeof(uint64_t);
    bufferCheck(data,newPos,bufferSize);
    writeUInt64(data,i,pos);
}

void io::writeFsTToBuffer(char *&data, fs_t i, fs_t &pos, fs_t &bufferSize)
{
    writeUInt32ToBuffer(data,i,pos,bufferSize);
}

void io::writeFixedLengthDataToBuffer(char *&data, fs_t length, char *in, fs_t &pos, fs_t &bufferSize)
{
    fs_t newPos=pos+length+sizeof(fs_t);
    bufferCheck(data,newPos,bufferSize);
    writeFixedLengthData(data,length,in,pos);
}

void io::writeZeroTerminatedDataToBuffer(char *&data, char *in, fs_t &pos, fs_t &bufferSize)
{
    fs_t newPos=pos+strlen(in)+1;
    bufferCheck(data,newPos,bufferSize);
    writeZeroTerminatedData(data,in,pos);
}

void io::writeRawDataToBuffer(char *&data, char *in, fs_t length, fs_t &pos, fs_t &bufferSize)
{
    fs_t newPos=pos+length;
    bufferCheck(data,newPos,bufferSize);
    writeRawData(data,in,length,pos);
}

void io::writeRawDataToLongBuffer(char *&data, char *in, uint64_t length, uint64_t &pos, uint64_t &bufferSize)
{
    fs_t newPos=pos+length;
    longBufferCheck(data,newPos,bufferSize);
    memcpy(data+pos,in,length);
    pos+=length;
}

void io::writeRawCharToBuffer(char *&data, char in, fs_t &pos, fs_t &bufferSize)
{
    if(pos+1==bufferSize) // Because we're only adding one character, == is permissible here.
    {
        extendBufferSize(bufferSize,pos+1);
        data=(char*)realloc(data,bufferSize);
    }
    data[pos++]=in;
}

void io::terminateBuffer(char *buffer, fs_t &pos, fs_t bufferSize)
{
    bufferCheck(buffer,pos,bufferSize);
    buffer[pos]=0; // Do not use pos++! Many functions use the parameter passed in "pos" as a string length indicator!
}

bool io::bufferCheck(char *&buffer, fs_t pos, fs_t &bufferSize)
{
    bool ret=true;
    while(pos>=bufferSize) // Needed to check if sth would fit into the buffer.
    {
        extendBufferSize(bufferSize,pos);
        buffer=(char*)realloc(buffer,bufferSize);
        ret=false;
    }
    return ret;
}

bool io::longBufferCheck(char *&buffer, uint64_t pos, uint64_t &bufferSize)
{
    bool ret=true;
    while(pos>=bufferSize) // Needed to check if sth would fit into the buffer.
    {
        extendBufferSize(bufferSize,pos);
        buffer=(char*)realloc(buffer,bufferSize);
        ret=false;
    }
    return ret;
}
