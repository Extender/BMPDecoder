#include "bmp.h"

uint32_t *bmp::decode(char *data, int32_t &width, int32_t &height)
{
    char *orig=data;
    if(io::readUInt16(data)!=0x4D42)
        return 0;
    fs_t size=io::readUInt32(data);
    data+=4;
    fs_t pixelDataPos=io::readUInt32(data);
    fs_t headerSize=io::readUInt32(data);
    width=io::readUInt32(data);
    height=io::readUInt32(data);
    bool upsideDown=height>0;
    if(!upsideDown)
        height*=-1;
    uint16_t colorPlaneCount=io::readUInt16(data);
    uint16_t bitsPerPixel=io::readUInt16(data);
    uint32_t compressionMethod=io::readUInt32(data);
    uint32_t imageSize=io::readUInt32(data);
    int32_t horResInPixelsPerMeter=io::readUInt32(data);
    int32_t verResInPixelsPerMeter=io::readUInt32(data);
    uint32_t paletteColors=io::readUInt32(data);
    data+=4;
    uint32_t rowSize=floor((double)(bitsPerPixel*width+31)/32)*4;

    // If bitsPerPixel==16: actual masks; shift values stored in *Shift16.

    uint32_t redShift=compressionMethod==BMP_COMPRESSION_BITFIELDS?(bitsPerPixel==16?io::readUInt32(data):(ceil(log2(io::readUInt32(data)))/8))-1:2; // Get bit offset out of this number
    uint32_t greenShift=compressionMethod==BMP_COMPRESSION_BITFIELDS?(bitsPerPixel==16?io::readUInt32(data):(ceil(log2(io::readUInt32(data)))/8))-1:1;
    uint32_t blueShift=compressionMethod==BMP_COMPRESSION_BITFIELDS?(bitsPerPixel==16?io::readUInt32(data):(ceil(log2(io::readUInt32(data)))/8))-1:0;

    uint32_t redShift16,greenShift16,blueShift16;
    uint8_t redMax16,greenMax16,blueMax16;

    if(bitsPerPixel==16)
    {
        redShift16=ceil(log2(redShift));
        greenShift16=ceil(log2(greenShift));
        blueShift16=ceil(log2(blueShift));
        if(redShift16>greenShift16)
        {
            if(redShift16>blueShift16)
            {
                if(greenShift16>blueShift16)
                {
                    // redShift16>greenShift16>blueShift16
                    redShift16=greenShift16;
                    greenShift16=blueShift16;
                    blueShift16=0;
                }
                else
                {
                    // redShift16>blueShift16>greenShift16
                    redShift16=blueShift16;
                    blueShift16=greenShift16;
                    greenShift16=0;
                }
            }
            else
            {
                // blueShift16>redShift16>greenShift16
                blueShift16=redShift16;
                redShift16=greenShift16;
                greenShift16=0;
            }
        }
        else if(blueShift16>greenShift16)
        {
            // blueShift16>greenShift16>redShift16
            blueShift16=greenShift16;
            greenShift16=redShift16;
            redShift16=0;
        }
        else
        {
            if(redShift16>blueShift16)
            {
                // greenShift16>redShift16>blueShift16
                greenShift16=redShift16;
                redShift16=blueShift16;
                blueShift16=0;
            }
            else
            {
                // greenShift16>blueShift16>redShift16
                greenShift16=blueShift16;
                blueShift16=redShift16;
                redShift16=0;
            }
        }
        redMax16=(uint32_t)redShift>>redShift16;
        greenMax16=(uint32_t)greenShift>>greenShift16;
        blueMax16=(uint32_t)blueShift>>blueShift16;
    }

    uint32_t *out=(uint32_t*)malloc(width*height*4);
    uint32_t *colorTable;
    if(bitsPerPixel<16)
    {
        if(paletteColors==0)
            paletteColors=pow(2,bitsPerPixel);
        // Color table mandatory if bitsPerPixel<16
        colorTable=(uint32_t*)malloc(4*paletteColors);
        memcpy(colorTable,data,4*paletteColors);
    }
    data=orig+pixelDataPos;
    if(compressionMethod==BMP_COMPRESSION_RGB||compressionMethod==BMP_COMPRESSION_BITFIELDS)
    {
        if(bitsPerPixel==1)
        {
            if(upsideDown)
            {
                for(uint32_t y=0;y<height;y++)
                {
                    for(uint32_t x=0;x<width;x++)
                    {
                        out[(height-y-1)*width+x]=0xFF000000|colorTable[((uint8_t)data[(uint32_t)(y*rowSize+(uint32_t)floor(x/8))]&(1<<(7-(x%8))))>0];
                    }
                }
            }
            else
            {
                for(uint32_t y=0;y<height;y++)
                {
                    for(uint32_t x=0;x<width;x++)
                    {
                        out[y*width+x]=0xFF000000|colorTable[((uint8_t)data[(uint32_t)(y*rowSize+(uint32_t)floor(x/8))]&(1<<(7-(x%8))))>0];
                    }
                }
            }
        }
        else if(bitsPerPixel==4)
        {
            if(upsideDown)
            {
                for(uint32_t y=0;y<height;y++)
                {
                    for(uint32_t x=0;x<width;x++)
                    {
                        out[(height-y-1)*width+x]=0xFF000000|colorTable[(((uint8_t)data[(uint32_t)(y*rowSize+(uint32_t)floor(x/2))]&(0xF<<(((x+1)%2)*4))))>>(((x+1)%2)*4)];
                    }
                }
            }
            else
            {
                for(uint32_t y=0;y<height;y++)
                {
                    for(uint32_t x=0;x<width;x++)
                    {
                        out[y*width+x]=0xFF000000|colorTable[(((uint8_t)data[(uint32_t)(y*rowSize+(uint32_t)floor(x/2))]&(0xF<<(((x+1)%2)*4))))>>(((x+1)%2)*4)];
                    }
                }
            }
        }
        else if(bitsPerPixel==8)
        {
            if(upsideDown)
            {
                for(uint32_t y=0;y<height;y++)
                {
                    for(uint32_t x=0;x<width;x++)
                    {
                        uint8_t pixel=(uint8_t)data[y*rowSize+x];
                        out[(height-y-1)*width+x]=0xFF000000|colorTable[pixel];
                    }
                }
            }
            else
            {
                for(uint32_t y=0;y<height;y++)
                {
                    for(uint32_t x=0;x<width;x++)
                    {
                        uint32_t pixel=(uint8_t)data[y*width+x];
                        out[y*width+x]=0xFF000000|colorTable[pixel];
                    }
                }
            }
        }
        else if(bitsPerPixel==16)
        {
            if(upsideDown)
            {
                for(uint32_t y=0;y<height;y++)
                {
                    for(uint32_t x=0;x<width;x++)
                    {
                        uint32_t offset=y*rowSize+2*x; // Byte at which this pixel starts
                        uint32_t pixel=(uint8_t)data[offset]|((uint32_t)((uint8_t)data[offset+1])<<8);
                        uint8_t r=((double)((pixel&redShift)>>redShift16)/redMax16)*255;
                        uint8_t g=((double)((pixel&greenShift)>>greenShift16)/greenMax16)*255;
                        uint8_t b=((double)((pixel&blueShift)>>blueShift16)/blueMax16)*255;
                        out[(height-y-1)*width+x]=0xFF000000|(r<<16)|(g<<8)|b;
                    }
                }
            }
            else
            {
                for(uint32_t y=0;y<height;y++)
                {
                    for(uint32_t x=0;x<width;x++)
                    {
                        uint32_t offset=y*rowSize+2*x; // Byte at which this pixel starts
                        uint32_t pixel=(uint8_t)data[offset]|((uint32_t)((uint8_t)data[offset+1])<<8);
                        uint8_t r=((double)((pixel&redShift)>>redShift16)/redMax16)*255;
                        uint8_t g=((double)((pixel&greenShift)>>greenShift16)/greenMax16)*255;
                        uint8_t b=((double)((pixel&blueShift)>>blueShift16)/blueMax16)*255;
                        out[y*width+x]=0xFF000000|(r<<16)|(g<<8)|b;
                    }
                }
            }
        }
        else if(bitsPerPixel==24)
        {
            if(upsideDown)
            {
                for(uint32_t y=0;y<height;y++)
                {
                    for(uint32_t x=0;x<width;x++)
                    {
                        uint32_t offset=y*rowSize+3*x; // Byte at which this pixel starts
                        uint32_t b=(uint8_t)data[offset+blueShift];
                        uint32_t g=(uint8_t)data[offset+greenShift];
                        uint32_t r=(uint8_t)data[offset+redShift];
                        out[(height-y-1)*width+x]=0xFF000000|(r<<16)|(g<<8)|b;
                    }
                }
            }
            else
            {
                for(uint32_t y=0;y<height;y++)
                {
                    for(uint32_t x=0;x<width;x++)
                    {
                        uint32_t offset=y*rowSize+3*x; // Byte at which this pixel starts
                        uint32_t b=(uint8_t)data[offset+blueShift];
                        uint32_t g=(uint8_t)data[offset+greenShift];
                        uint32_t r=(uint8_t)data[offset+redShift];
                        out[y*width+x]=0xFF000000|(r<<16)|(g<<8)|b;
                    }
                }
            }
        }
        else if(bitsPerPixel==32)
        {
            if(upsideDown)
            {
                for(uint32_t y=0;y<height;y++)
                {
                    for(uint32_t x=0;x<width;x++)
                    {
                        uint32_t offset=y*rowSize+4*x; // Byte at which this pixel starts
                        uint32_t b=(uint8_t)data[offset+blueShift];
                        uint32_t g=(uint8_t)data[offset+greenShift];
                        uint32_t r=(uint8_t)data[offset+redShift];
                        out[y*width+x]=0xFF000000|(r<<16)|(g<<8)|b;
                    }
                }
            }
            else
            {
                for(uint32_t y=0;y<height;y++)
                {
                    for(uint32_t x=0;x<width;x++)
                    {
                        uint32_t offset=y*rowSize+4*x; // Byte at which this pixel starts
                        uint32_t b=(uint8_t)data[offset+blueShift];
                        uint32_t g=(uint8_t)data[offset+greenShift];
                        uint32_t r=(uint8_t)data[offset+redShift];
                        out[(height-y-1)*width+x]=0xFF000000|(r<<16)|(g<<8)|b;
                    }
                }
            }
        }
        return out;
    }
    else if(compressionMethod==BMP_COMPRESSION_RLE8)
    {
        // Change BMP_COMPRESSION_RLE4, too!

        uint32_t y=upsideDown?height-1:0;
        uint32_t x=0;
        uint8_t nextByte;
        while(true)
        {
            nextByte=(uint8_t)*(data++);
            if(nextByte>0)
            {
                // Encoded mode
                // First byte: number of pixels
                // Second byte: indexed color
                uint32_t color=0xFF000000|colorTable[(uint8_t)*(data++)];
                for(uint8_t pos=0;pos<nextByte;pos++)
                    out[y*width+x++]=color;
            }
            else
            {
                uint8_t secondByte=(uint8_t)*(data++);
                if(secondByte>0x2)
                {
                    // Absolute mode
                    for(uint8_t pos=0;pos<secondByte;pos++)
                        out[y*width+(x++)]=0xFF000000|colorTable[(uint8_t)*(data++)];
                    data+=(data-orig)%2; // Run must be word-aligned.
                }
                else
                {
                    // Special code
                    if(secondByte==0)
                    {
                        // End of line
                        if(upsideDown)
                            y--;
                        else
                            y++;
                        x=0;
                    }
                    else if(secondByte==1)
                        break; // End of bitmap
                    else // secondByte==2
                    {
                        // Delta
                        x+=(uint8_t)*(data++);
                        y+=(uint8_t)*(data++);
                    }
                }
            }
        }
        return out;
    }
    else if(compressionMethod==BMP_COMPRESSION_RLE4)
    {
        // Change BMP_COMPRESSION_RLE8, too!

        uint32_t y=upsideDown?height-1:0;
        uint32_t x=0;
        uint8_t nextByte;
        while(true)
        {
            nextByte=(uint8_t)*(data++);
            if(nextByte>0)
            {
                // Encoded mode
                // First byte: number of pixels
                // Second byte: indexed colors (2)!
                uint32_t secondByte=(uint8_t)*(data++);
                uint32_t colorA=0xFF000000|colorTable[(secondByte&0xF0)>>4];
                uint32_t colorB=0xFF000000|colorTable[secondByte&0xF];
                for(uint8_t pos=0;pos<nextByte;pos++)
                    out[y*width+x++]=(pos%2)>0?colorB:colorA;
            }
            else
            {
                uint8_t secondByte=(uint8_t)*(data++);
                if(secondByte>0x2)
                {
                    // Absolute mode
                    uint8_t pixel;
                    bool second;
                    for(uint8_t pos=0;pos<secondByte;pos++)
                    {
                        if(!(second=(pos%2)>0))
                            pixel=(uint8_t)*(data++);
                        out[y*width+(x++)]=0xFF000000|colorTable[(second?(pixel&0xF):((pixel&0xF0)>>4))];
                    }
                    data+=(data-orig)%2; // Run must be word-aligned.
                }
                else
                {
                    // Special code
                    if(secondByte==0)
                    {
                        // End of line
                        if(upsideDown)
                            y--;
                        else
                            y++;
                        x=0;
                    }
                    else if(secondByte==1)
                        break; // End of bitmap
                    else // secondByte==2
                    {
                        // Delta
                        x+=(uint8_t)*(data++);
                        y+=(uint8_t)*(data++);
                    }
                }
            }
        }
        return out;
    }
    return 0;
}
