#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


//��̫��֡���ݽṹ������������֡��
typedef struct _ETHERNET_FRAME_
{
    int frame_number; //֡���
    unsigned long long timestamp; //8bytes ֡ʱ���
    char timestampStr[80]; //֡ʱ��� Arrival Time: Nov  1, 2019 15:35:59.430461000 �й���׼ʱ��
    int frame_length; //4bytes ֡���ݴ�С��������ͷ������16�ֽڣ���λ���ֽڣ�һ��Ϊ 0x05EA = 1514 bytes
    int capture_length; //4bytes �����֡���ݴ�С
    int file_offset; //�ļ�ƫ��

public:
    int printInfo()
    {
        printf("\n-----ETHERNET_FRAME-----\n");
        printf("frame_number: %d\n", frame_number);
        printf("timestamp: 0x%x\n", timestamp);
        printf("timestampStr: %s\n", timestampStr);
        printf("frame_length: %d (0x%x) bytes\n", frame_length, frame_length);
        printf("capture_length: %d (0x%x) bytes\n", capture_length, capture_length);
        printf("file_offset: %d (0x%x)\n", file_offset, file_offset);
        return 0;
    }
}ETHERNET_FRAME;


//������·����̫��֡ͷ������̫��Э��汾II��
typedef struct _ETHERNET_II_HEADER_
{
    char destination_address[50]; //6bytes Ŀ��MAC������_��ţ�������ַ�� Address: HuaweiTe_70:5c:3c (08:4f:0a:70:5c:3c)
    char source_address[50]; //6bytes ԴMAC������_��ţ�������ַ�� Address: Hangzhou_68:5c:9c (48:7a:da:68:5c:9c)
    int type; //2bytes ֡�ڷ�װ���ϲ�Э�����ͣ�IP=0x0800��ARP=0806��RARP=0835 [TCP-IP����1:Э�� ͼ2-1 16ҳ]��Type: IPv4 (0x0800)

public:
    int printInfo()
    {
        printf("-----ETHERNET_II_HEADER-----\n");
        printf("destination_address: %s\n", destination_address);
        printf("source_address: %s\n", source_address);
        printf("type: 0x%04x %s\n", type, (type == 0x0800) ? "IPv4" : "unknown");
        return 0;
    }
}ETHERNET_II_HEADER;


//��������IP��ͷ�� [TCP-IP����1:Э�� ͼ3-1 24ҳ]
typedef struct _INTERNET_PROTOCOL_V4_HEADER_
{
    int version; //4bit �汾 Version: 4
    int ip_header_length; //4bit IP��ͷ������ Header Length: 20 bytes (5)
    int differentiated_services_field; //8bit ��ַ����ֶ� Differentiated Services Field: 0x00 (DSCP: CS0, ECN: Not-ECT)
    int total_length; //16bit IP�����ܳ��� Total Length: 52
    int identification; //16bit ��־�ֶ� Identification: 0x0000 (0)
    int flags_reserved_1bit; //1bit ��־�ֶ� 0 = Reserved bit: Not set
    int flags_do_not_fragment_set_1bit; //1bit ��־�ֶ� .1.. .... .... .... = Don't fragment: Set
    int flags_more_fragments_1bit; //1bit ��־�ֶ� ..0. .... .... .... = More fragments: Not set
    int flags_fragments_offset_13bits; //13bit ��־�ֶ� �ֶ�ƫ��������һ��IP���ֶκ���ʱ�����εı�ʶ��...0 0000 0000 0000 = Fragment offset: 0
    int time_to_live; //8bit ������TTL Time to live: 62
    int protocol; //8bit �˰��ڷ�װ���ϲ�Э�� Protocol: TCP (6); UDP(17) https://tools.ietf.org/html/rfc1700 Page7
    char protocol_str[10]; //8bit �˰��ڷ�װ���ϲ�Э�� Protocol: TCP (6)
    int header_checksum; //16bit ͷ�����ݵ�У��� Header checksum: 0x326e [validation disabled]
    int source_ip_addr; //32bit ԴIP��ַ Source: 172.31.25.211
    char source_ip_addr_str[20]; //32bit ԴIP��ַ Source: 172.31.25.211
    int destination_ip_addr; //32bit Ŀ��IP��ַ Destination: 43.123.24.233
    char destination_ip_addr_str[20]; //32bit Ŀ��IP��ַ Destination: 43.123.24.233

public:
    int printInfo()
    {
        printf("-----INTERNET_PROTOCOL_V4_HEADER-----\n");
        printf("version: %d\n", version);
        printf("ip_header_length: %d bytes\n", ip_header_length);
        printf("differentiated_services_field: %d\n", differentiated_services_field);
        printf("total_length: %d bytes\n", total_length);
        printf("identification: %d (0x%x)\n", identification, identification);
        printf("flags_reserved_1bit: %d\n", flags_reserved_1bit);
        printf("flags_do_not_fragment_set_1bit: %d\n", flags_do_not_fragment_set_1bit);
        printf("flags_more_fragments_1bit: %d\n", flags_more_fragments_1bit);
        printf("flags_fragments_offset_13bits: %d\n", flags_fragments_offset_13bits);
        printf("time_to_live: %d\n", time_to_live);
        printf("protocol: %d\n", protocol);
        printf("protocol_str: %s\n", protocol_str);
        printf("header_checksum: 0x%x\n", header_checksum);
        printf("source_ip_addr: 0x%x\n", source_ip_addr);
        printf("source_ip_addr_str: %s\n", source_ip_addr_str);
        printf("destination_ip_addr: 0x%x\n", destination_ip_addr);
        printf("destination_ip_addr_str: %s\n", destination_ip_addr_str);
        return 0;
    }
}INTERNET_PROTOCOL_V4_HEADER;


/*
https://tools.ietf.org/html/rfc2460#section-3

IPv6 Header Format

+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|Version| Traffic Class |           Flow Label                  |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|         Payload Length        |  Next Header  |   Hop Limit   |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                                                               |
+                                                               +
|                                                               |
+                         Source Address                        +
|                                                               |
+                                                               +
|                                                               |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                                                               |
+                                                               +
|                                                               |
+                      Destination Address                      +
|                                                               |
+                                                               +
|                                                               |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
typedef struct _INTERNET_PROTOCOL_V6_HEADER_
{
    int version; //4-bit Internet Protocol version number = 6
    int traffic_class; //8-bit traffic class field.
    int flow_label; //20-bit flow label.
    int payload_length; //16-bit unsigned integer. Length of the IPv6 payload, i.e., the rest of the packet following this IPv6 header, in octets.
    int next_header; //8-bit selector. Identifies the type of header immediately following the IPv6 header. Protocol: TCP (6); UDP(17) https://tools.ietf.org/html/rfc1700 Page7
    int hop_limit; //8-bit unsigned integer.  Decremented by 1 by each node that forwards the packet. The packet is discarded if Hop Limit is decremented to zero.
    char source_address[16]; //128-bit address of the originator of the packet.See [ADDRARCH]. Source: fe80::35df:9e1:c898:7eb8
    char destination_address[16]; //128-bit address of the intended recipient of the packet (possibly not the ultimate recipient, if a Routing header is present). Destination: ff02::1:2

public:
    int printInfo()
    {
        printf("-----INTERNET_PROTOCOL_V6_HEADER-----\n");
        printf("version: %d\n", version);
        printf("traffic_class: %d\n", traffic_class);
        printf("flow_label: %d\n", flow_label);
        printf("payload_length: %d bytes\n", payload_length);
        printf("next_header: %d (0x%x)\n", next_header, next_header);
        printf("hop_limit: %d\n", hop_limit);

        printf("source_address: ");
        for(int i = 0; i < 16; i += 2)
        {
            if(i != 14)
            {
                printf("%02x%02x::\n", source_address[i], source_address[i + 1]);
            }else if(i != 14)
            {
                printf("%02x%02x\n", source_address[i], source_address[i + 1]);
            }
        }
        printf("\n");
        
        printf("destination_address: ");
        for(int i = 0; i < 16; i += 2)
        {
            if(i != 14)
            {
                printf("%02x%02x::\n", destination_address[i], destination_address[i + 1]);
            }else if(i != 14)
            {
                printf("%02x%02x\n", destination_address[i], destination_address[i + 1]);
            }
        }
        printf("\n");
        return 0;
    }
}INTERNET_PROTOCOL_V6_HEADER;


typedef struct _INTERNET_PROTOCOL_HEADER_
{
    int ip_version; // 0, 4 or 6
    INTERNET_PROTOCOL_V4_HEADER ipv4;
    INTERNET_PROTOCOL_V6_HEADER ipv6;

public:
    int printInfo()
    {
        printf("-----INTERNET_PROTOCOL_HEADER-----\n");
        if(ip_version == 4)
        {
            ipv4.printInfo();
        }
        else if(ip_version == 6)
        {
            ipv6.printInfo();
        }
        else
        {
            printf("%s(%d): %s: Error: ip_version=%d; not 4 or 6\n", __FILE__, __LINE__, __FUNCTION__, ip_version);
            return -1;
        }
        return 0;
    };
}INTERNET_PROTOCOL_HEADER;


/*
TCPѡ��:

Kind(1byte)    Option-Total-Length(bytes)       Meaning
-----------    --------------------------       -------
0              1                                End of option list
1              1                                No-Operation
2              4=1+1+2                          Maximum Segment Size (MSS)
3              3=1+1+1                          TCP Window Scale Option (WSopt)
4              2=1+1                            SACK permitted(Selective Acknowledgment, ѡ����ȷ��)
5              2=1+1                            SACK packet
6              -                                Undefine
7              -                                Undefine
8              10=1+1+8                         TCP Timestamps Option (TSopt)

���磺kind=1����ʾ�޲�����NOP��Ҫ���������4�ֽڶ���

���磺kind=8����ʾ��TCPʱ���ѡ���ѡ���ܹ�ռ��10���ֽڣ�kind�ֶ�ռ��1���ֽڣ�
option_length�ֶ�ռ��1���ֽڣ���������4���ֽڱ�ʾ������ʱ���ֵ��ʣ�µ����4��
�ֽڱ�ʾʱ�������Ӧ��ֵ
*/
typedef struct _TCP_OPTIONS_
{
    int kind; //8bits TCPѡ�������
    int length; //8bits TCPѡ����ܳ��ȣ���λ��byte���������ֶγ��ȣ�
    char data[32]; //ֻ�е�length����2ʱ�����ֶβ���Ч

public:
    int printInfo()
    {
        printf("-----TCP_OPTIONS-----\n");
        printf("kind: %d\n", kind);
        printf("length: %d bytes\n", length);

        printf("data: ");
        if (kind == 2)
        {
            int mss = ((data[0] & 0xFF) << 8) | (data[1] & 0xFF);
            printf("Maximum Segment Size (MSS): %d bytes", mss);
        }
        else if (kind == 3)
        {
            printf("TCP Window Scale Option (WSopt): %d", data[0]);
        }
        else if (kind == 8)
        {
            printf("TCP Timestamps Option (TSopt): %d", data[0]);
        }
        printf("\n");

        return 0;
    }
}TCP_OPTIONS;


//�����TCP���ݶ�ͷ�� [TCP-IP����1:Э�� ͼ17-2 172ҳ] Transmission Control Protocol
typedef struct _TRANSMISSION_CONTROL_PROTOCOL_HEADER_
{
    int source_port; //16bit Դ�˿ں� Source Port: 554
    int destination_port; //16bit Ŀ�Ķ˿ں� Destination Port: 55014
    unsigned int sequence_number; //32bit ���к� Sequence number: 0    (relative sequence number)
    unsigned int next_sequence_number; //32bit ��һ�����������к� = sequence_number + tcp_payload_length
    unsigned int acknowledgment_number; //32bit ȷ�����к� Acknowledgment number: 1    (relative ack number)
    int tcp_header_length; //4bit ����ͷ��ռ32���ص���Ŀ��û���κ�ѡ���ֶε�TCPͷ������Ϊ20�ֽڣ�5x32=160���أ�����������60�ֽڵ�TCPͷ���� 1000 .... = Header Length: 32 bytes (8)
    int flags_reserved_3bit; //3bit �����ֶ� 000. .... .... = Reserved: Not set
    int flags_nonce_1bit; //1bit �����ֶ� ....0 .... .... = Nonce: Not set
    int flags_congestion_window_reduced_1bit; //1bit ӵ�����ڼ��� ...... 0... .... = Congestion Window Reduced (CWR): Not set
    int flags_ecn_echo_1bit; //1bit ��ʽӵ��֪ͨExplicit Congestion Notification .... .0.. .... = ECN-Echo: Not set
    int flags_urgent_1bit; //1bit ����ָ��URG�� urgent pointer����Ч .... ..0. .... = Urgent: Not set
    int flags_acknowledgment_1bit; //1bit ȷ�������ЧACK .... ...1 .... = Acknowledgment: Set
    int flags_push_1bit; //1bit ���շ�Ӧ�þ��콫������Ķν���Ӧ�ò�PSH .... .... 0... = Push: Not set
    int flags_reset_1bit; //1bit �ؽ�����RST .... .... .0.. = Reset: Not set
    int flags_syn_1bit; //1bit ͬ�������������һ������SYN .... .... ..1. = Syn: Set
    int flags_fin_1bit; //1bit ������ɷ�������FIN .... .... ...0 = Fin: Not set
    int window_size; //16bit �������ƵĴ��ڴ�С Window size value: 29200
    int checksum; //16bit TCP���ݶε�У��� Checksum: 0x7f9e [unverified]
    int urgent_pointer; //16bit ����ָ�� Urgent pointer: 0
    int tcp_options_size; //TCP��ѡ�����Ŀ����Χ[1,40]������0ʱtcp_options[40]�ֶ���Ч
    TCP_OPTIONS tcp_options[40]; //TCP��ѡ��
    unsigned char *tcp_payload; //TCP��Ч�غ�
    int tcp_payload_length; //ע�⣺һ��TCP������Ч�غɿ��ܱ�RTP��Ϊ�ķֳ��������֣�ԭ���ǣ�����RTP���Ĵ�С������TCP������غ���������1460bytes������ʱ�򳬳��Ĳ���ֻ�з�����һ��TCP��

public:
    int printInfo()
    {
        printf("-----TRANSMISSION_CONTROL_PROTOCOL_HEADER-----\n");
        printf("source_port: %d\n", source_port);
        printf("destination_port: %d\n", destination_port);
        printf("sequence_number: %u (0x%x)\n", sequence_number, sequence_number);
        printf("next_sequence_number(Calculate): %u (0x%x)\n", next_sequence_number, next_sequence_number);
        printf("acknowledgment_number: %u (0x%x)\n", acknowledgment_number, acknowledgment_number);
        printf("tcp_header_length: %d bytes\n", tcp_header_length);
        printf("flags_reserved_3bit: 0x%x\n", flags_reserved_3bit);
        printf("flags_nonce_1bit: %d\n", flags_nonce_1bit);
        printf("flags_congestion_window_reduced_1bit: %d\n", flags_congestion_window_reduced_1bit);
        printf("flags_ecn_echo_1bit: %d\n", flags_ecn_echo_1bit);
        printf("flags_urgent_1bit %d\n", flags_urgent_1bit);
        printf("flags_acknowledgment_1bit: %d\n", flags_acknowledgment_1bit);
        printf("flags_push_1bit: %d\n", flags_push_1bit);
        printf("flags_reset_1bit: %d\n", flags_reset_1bit);
        printf("flags_syn_1bit: %d\n", flags_syn_1bit);
        printf("flags_fin_1bit: %d\n", flags_fin_1bit);
        printf("window_size: %d bytes\n", window_size);
        printf("checksum: 0x%x\n", checksum);
        printf("urgent_pointer: %d\n", urgent_pointer);
        printf("tcp_options_size: %d\n", tcp_options_size);

        for (int i = 0; i < tcp_options_size; ++i)
        {
            printf("tcp_options: %d/%d\n", i, tcp_options_size);
            tcp_options[i].printInfo();
        }
        
        printf("tcp_payload_length: %d bytes\n", tcp_payload_length);
        if (tcp_payload_length >= 6)
        {
            printf("payload: 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x\n", 
                tcp_payload[0], tcp_payload[1], tcp_payload[2], tcp_payload[3], tcp_payload[4], tcp_payload[5]);
        }

        return 0;
    }
}TRANSMISSION_CONTROL_PROTOCOL_HEADER;


//User Datagram Protocol (UDPЭ��)
//https://tools.ietf.org/html/rfc768
typedef struct _USER_DATAGRAM_PROTOCOL_HEADER_
{
    int source_port; //16-bit
    int destination_port; //16-bit
    int udp_header_and_data_length; //16-bit Length  is the length  in octets  of this user datagram  including  this header and the data.
    int checksum; //16-bit
    unsigned char *udp_payload; //UDP��Ч�غ�
    int udp_payload_length; 

public:
    int printInfo()
    {
        printf("-----USER_DATAGRAM_PROTOCOL_HEADER-----\n");
        printf("source_port: %d\n", source_port);
        printf("destination_port: %d\n", destination_port);
        printf("udp_header_and_data_length: %d(0x%x)\n", udp_header_and_data_length, udp_header_and_data_length);
        printf("checksum: 0x%x\n", checksum);
        printf("udp_payload_length: %d(0x%x)\n", udp_payload_length, udp_payload_length);
        if (udp_payload_length >= 6)
        {
            printf("payload: 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x\n", 
                udp_payload[0], udp_payload[1], udp_payload[2], udp_payload[3], udp_payload[4], udp_payload[5]);
        }

        return 0;
    };
}USER_DATAGRAM_PROTOCOL_HEADER;


typedef struct _TCP_FRAME_INFO_
{
    ETHERNET_FRAME ethernet_frame;
    ETHERNET_II_HEADER ethernet_ii_header;
    INTERNET_PROTOCOL_HEADER ip_header;
    TRANSMISSION_CONTROL_PROTOCOL_HEADER tcp_packet;
    USER_DATAGRAM_PROTOCOL_HEADER udp_packet;

public:
    int printInfo()
    {
        ethernet_frame.printInfo();
        ethernet_ii_header.printInfo();
        ip_header.printInfo();
        if((ip_header.ip_version == 4 && ip_header.ipv4.protocol == 6) //6=TCP
            || (ip_header.ip_version == 6 && ip_header.ipv6.next_header == 6)
            )
        {
            tcp_packet.printInfo();
        }
        else if((ip_header.ip_version == 4 && ip_header.ipv4.protocol == 17) //17=UDP
            || (ip_header.ip_version == 6 && ip_header.ipv6.next_header == 17)
            )
        {
            udp_packet.printInfo();
        }
        return 0;
    }
}TCP_FRAME_INFO;


//------------TCP/IPЭ��--------------
class CTcpIpProtocol
{
public:

public:
    CTcpIpProtocol();
    ~CTcpIpProtocol();

    int readOneEthernetFrame(unsigned char *buffer, int bufferSize, ETHERNET_FRAME &ethernetFrame, unsigned char *&newPos, unsigned char *bufferBase);
    int readOneEthernetIIHeader(unsigned char *buffer, int bufferSize, ETHERNET_II_HEADER &ethernetIIHeader, unsigned char *&newPos, unsigned char *bufferBase);
    int readOneInterNetProtocolHeader(unsigned char *buffer, int bufferSize, INTERNET_PROTOCOL_HEADER &internetProtocolHeader, unsigned char *&newPos, unsigned char *bufferBase);
    int readOneTransmissionControlProtocolHeader(unsigned char *buffer, int bufferSize, TRANSMISSION_CONTROL_PROTOCOL_HEADER &tcpHeader, unsigned char *&newPos, unsigned char *bufferBase);
    int readOneUserDatagramProtocolHeader(unsigned char *buffer, int bufferSize, USER_DATAGRAM_PROTOCOL_HEADER &udpHeader, unsigned char *&newPos, unsigned char *bufferBase);
};


//---------------------
/*
IPЭ���protocol�ֶ�

0        �����ֶΣ�����IPv6(��Ծ�㵽��Ծ��ѡ��)
1        Internet������Ϣ (ICMP)
2        Internet����� (IGMP)
3        ���ص����� (GGP)
4        1P�е�IP(��װ)
5        ��
6        ������� (TCP)
7        CBT
8        �ⲿ����Э�� (EGP)
9        �κ�˽���ڲ�����(Cisco������IGRPʵ����ʹ��) (IGP)
10        BBNRCC����
11        ��������Э��
12        PUP
13        ARGUS
14        EMCON
15        ������Ϲ���
16        ����(Chaos)
17        �û����ݱ��� (UDP)
18        ����
19        DCN������ϵͳ
20        ��������
21        �����߲���
22        XEROXNSIDP
23        Trunk-1
24        Trunk-2
25        leaf-1
26        1eaf-2
27        �ɿ�������Э��
28        Internet�ɿ�����
29        1SO����Э������� (TP4)
30        ������ݴ���Э��
31        MFE�������Э��
32        MERIT�ڵ�֮��Э��
33        ���н���Э��
34        ����������Э��
35        ��֮�����·��Э��
36        XTP
37        ���ݱ��Ĵ���Э��
38        IDPR������Ϣ����Э��
39        TP+ +����Э��
40        IL����Э��
41        1Pv6
42        ��Դ����·��Э��
43        1Pv6��·�ɱ�ͷ
44        1Pv6��Ƭ��ͷ
45        ��֮��·��Э��
46        ����Э��
47        ͨ��·�ɷ�װ
48        ���ƶ�����·��Э��
49        BNA
50        1Pv6��װ��ȫ��Ч����
51        1Pv6��֤��ͷ
52        ���ɵ�����㰲ȫTUBA
53        �����ܵ�IP
54        NBMA��ַ����Э��
55        IP���ƶ���
56        ʹ��KryptonetԿ�׹���Ĵ���㰲ȫЭ��
57        SKIP
58        1Pv6��ICMP
59        1Pv6������һ����ͷ
60        IPv6������ѡ��
61        �κ������ڲ�Э��
62        CFTP
63        �κα�������
64        SATNET��BackroomEXPAK
65        Kryptolan
66        MITԶ���������Э��
67        Internet Pluribus������
68        �κηֲ�ʽ�ļ�ϵͳ
69        SATNET����
70        VISAЭ��
71        Internet�����Ĺ���
72        �����Э��Network Executive
73        �����Э��Heart Beat
74        Wang Span����
75        ����ƵЭ��
76        Backroom SATNET����
77        SUN ND PROTOCOL����ʱ
78        WIDEBAND����
79        WIDEBAND EXPAK
80        ISO InternetЭ��
81        VMTP
82        SECURE��VMTP(��ȫ��VMTP)
83        VINES
84        TTP
85        NSFNET��IGP
86        ��ͬ����Э��
87        TCF
88        EIGRP
89        OSPF IGP
90        Sprite RPCЭ��
9]        Locus��ַ����Э��
92        �ಥ����Э��
93        AX.25֡
94        IP�ڲ���IP��װЭ��
95        ���ƶ����绥������Э��
96        ����ͨѶ��ȫЭ��
97        IP�е���̫��װ
98        ��װ��ͷ
99        �κ�˽�м��ܷ���
100        GMTP
101        Ipsilon��������Э��
102        PNNI over IP
103        Э������ಥ
104        ARIS
105        SCPS
106        QNX
107        �����
108        IP��Ч����ѹ��Э��
109        Sitara����Э��
110        Compaq�Ե�Э��
111        IP�е�IPX
112        ����·��������Э��
113        PGM�ɿ�����Э��
114        �κ�0��ԾЭ��
115        �ڶ������Э��
116        D-II���ݽ���(DDX)
117        ����ʽ������Э��
118        �ճ̼ƻ�����Э��
119        SpectraLink����Э��
120        UTI
121        ����ϢЭ��
122        SM
123        ����͸����Э��
124        ISIS over IPv4
125        FIRE
126        Combat���ߴ���Э��
127        Combat�����û����ݱ���
128        SSCOPMCE
129        IPLT
130        ��ȫ������
131        IP�е�˽��IP��װ
132        �����ƴ���Э��
133��254   δ����
255        ����
*/
