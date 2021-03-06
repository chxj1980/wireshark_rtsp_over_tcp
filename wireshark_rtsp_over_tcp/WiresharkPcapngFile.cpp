#include "WiresharkPcapngFile.h"


#define RETURN_IF_FAILED(condition, ret)                                                      \
    do                                                                                        \
    {                                                                                         \
        if (condition)                                                                        \
        {                                                                                     \
            printf("%s(%d): %s: Error: ret=%d;\n", __FILE__, __LINE__, __FUNCTION__, ret);    \
            return ret;                                                                       \
        }                                                                                     \
    } while (0)


CWiresharkPcapngFile::CWiresharkPcapngFile()
{
    m_inputFilename = "";
    m_fp = NULL;
    m_fileSize = 0;
    m_buffer = NULL;
    m_bufferSize = 0;
    m_bufferPosNow = NULL;
}


CWiresharkPcapngFile::~CWiresharkPcapngFile()
{
    if (m_fp)
    {
        fclose(m_fp);
        m_fp = NULL;
    }

    if (m_buffer)
    {
        free(m_buffer);
        m_buffer = NULL;
    }
}


int CWiresharkPcapngFile::probeFileType(const char *inputFilename)
{
    RETURN_IF_FAILED(inputFilename == NULL, -1);

    printf("%s(%d): %s: inputFilename=%s;\n", __FILE__, __LINE__, __FUNCTION__, inputFilename);

    FILE * fp = fopen(inputFilename, "rb");
    RETURN_IF_FAILED(fp == NULL, -2);

    fseek(fp, 0, SEEK_END);
    long fileSize = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    unsigned char *buffer = (unsigned char *)malloc(sizeof(unsigned char) * fileSize); //暂时将整个文件读入内存，后续可以改成按frame大小读取
    RETURN_IF_FAILED(buffer == NULL, -3);

    size_t readSize = fread(buffer, fileSize, 1, fp);
    if (readSize != 1)
    {
        printf("%s(%d): Error: read_size=%d != 1\n", __FUNCTION__, __LINE__, readSize);
        fclose(fp);
        free(buffer);
    }

    fclose(fp);
    fp = NULL;

    if(fileSize < 24)
    {
        if (buffer)
        {
            free(buffer);
            buffer = NULL;
        }
        RETURN_IF_FAILED(fileSize < 24, -3);
    }

    //--------------------
    unsigned char *p = buffer;
    unsigned char *p1 = p;
    unsigned char *p2 = p;
    unsigned char *p3 = buffer + fileSize - 1;

    if (p1[0] == 0x0A && p1[1] == 0x0D && p1[2] == 0x0D && p1[3] == 0x0A)
    {
        m_fileHeader.block_type = (p1[3] << 24) | (p1[2] << 16) | (p1[1] << 8) | p1[0];
        p1 += 4;

        m_fileHeader.block_total_length = (p1[3] << 24) | (p1[2] << 16) | (p1[1] << 8) | p1[0];
        p1 += 4;

        m_fileHeader.magic = (p1[3] << 24) | (p1[2] << 16) | (p1[1] << 8) | p1[0];
        p1 += 4;

        m_fileHeader.version_major = (p1[1] << 8) | p1[0];
        p1 += 2;

        m_fileHeader.version_minor = (p1[1] << 8) | p1[0];
        p1 += 2;

        //-------------------------
        m_inputFilename = inputFilename;
        m_fileSize = fileSize;
        m_bufferSize = fileSize;
        m_buffer = buffer;
        m_bufferPosNow = buffer + m_fileHeader.block_total_length;

        printf("%s(%d): OK. This file is wireshark-pcapng file format.\n", __FUNCTION__, __LINE__, readSize);

        return 0;
    }

    //------------------
    if (buffer)
    {
        free(buffer);
        buffer = NULL;
    }

    return -1;
}

/*
http://xml2rfc.tools.ietf.org/cgi-bin/xml2rfc.cgi?url=https://raw.githubusercontent.com/pcapng/pcapng/master/draft-tuexen-opsawg-pcapng.xml&modeAsFormat=html/ascii&type=ascii#section_epb

   0                   1                   2                   3
   0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
   +---------------------------------------------------------------+
 0 |                    Block Type = 0x00000006                    |
   +---------------------------------------------------------------+
 4 |                      Block Total Length                       |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 8 |                         Interface ID                          |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
12 |                        Timestamp (High)                       |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
16 |                        Timestamp (Low)                        |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
20 |                    Captured Packet Length                     |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
24 |                    Original Packet Length                     |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
28 /                                                               /
   /                          Packet Data                          /
   /              variable length, padded to 32 bits               /
   /                                                               /
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   /                                                               /
   /                      Options (variable)                       /
   /                                                               /
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |                      Block Total Length                       |
   +---------------------------------------------------------------+
*/
int CWiresharkPcapngFile::getNextNetworkFrame(unsigned char *&framePos, int &frameSize)
{
    int ret = 0;
    
    RETURN_IF_FAILED(m_bufferPosNow == NULL, -1);

    //--------------------
    unsigned char *p = m_bufferPosNow;
    unsigned char *p1 = p;
    unsigned char *p2 = p;
    unsigned char *p3 = m_buffer + m_fileSize - 1;
    
    int block_type = 0;
    int block_total_length = 0;

    while (p1 < p3)
    {
        block_type = (p1[3] << 24) | (p1[2] << 16) | (p1[1] << 8) | p1[0];
        p1 += 4;

        block_total_length = (p1[3] << 24) | (p1[2] << 16) | (p1[1] << 8) | p1[0];
        p1 += 4;

        //------------------------
        if (block_type == 1) //Interface Description Block
        {
            p1 += (block_total_length - 8);
        }
        else if (block_type == 2) //Packet Block
        {
            p1 += (block_total_length - 8);
        }
        else if (block_type == 3) //Simple Packet Block
        {
            p1 += (block_total_length - 8);
        }
        else if (block_type == 4) //Name Resolution Block
        {
            p1 += (block_total_length - 8);
        }
        else if (block_type == 5) //Interface Statistics Block
        {
            p1 += (block_total_length - 8);
        }
        else if (block_type == 6) //Enhanced Packet Block
        {
            int interface_id = (p1[3] << 24) | (p1[2] << 16) | (p1[1] << 8) | p1[0];
            p1 += 4;

            framePos = p1;
            frameSize = block_total_length - 12;

            m_bufferPosNow += block_total_length;

            return 0;
        }
    }
    
    return -1;
}
