#include "u2com.h"

RUDATA sRundata;

unsigned char RxBufU2[MAXRXU2];
unsigned char TxBufU2[200];
unsigned short RxLenU2 = 0;
unsigned short rsRxIndex = 0;
unsigned char rsPackFlag = 0;
unsigned short rsRxTime = 0;
unsigned short rsRxLen = 0;
unsigned char rsRxBuf[200];
static const unsigned char aucCRCHi[] = {
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40};

static const unsigned char aucCRCLo[] = {
    0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7,
    0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E,
    0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09, 0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9,
    0x1B, 0xDB, 0xDA, 0x1A, 0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC,
    0x14, 0xD4, 0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
    0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 0xF2, 0x32,
    0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4, 0x3C, 0xFC, 0xFD, 0x3D,
    0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A, 0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38,
    0x28, 0xE8, 0xE9, 0x29, 0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF,
    0x2D, 0xED, 0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
    0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 0x61, 0xA1,
    0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67, 0xA5, 0x65, 0x64, 0xA4,
    0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F, 0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB,
    0x69, 0xA9, 0xA8, 0x68, 0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA,
    0xBE, 0x7E, 0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
    0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 0x70, 0xB0,
    0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97,
    0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C, 0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E,
    0x5A, 0x9A, 0x9B, 0x5B, 0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89,
    0x4B, 0x8B, 0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
    0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 0x43, 0x83,
    0x41, 0x81, 0x80, 0x40};

unsigned short CRC16x(unsigned char *pucFrame, unsigned short usLen)
{
    unsigned char ucCRCHi = 0xFF;
    unsigned char ucCRCLo = 0xFF;
    int iIndex;

    while (usLen--)
    {
        iIndex = ucCRCLo ^ *(pucFrame++);
        ucCRCLo = (unsigned char)(ucCRCHi ^ aucCRCHi[iIndex]);
        ucCRCHi = aucCRCLo[iIndex];
    }
    return (unsigned short)(ucCRCHi << 8 | ucCRCLo);
}
void LoadReadData(void)
{
    sRundata.Ta = Ta;
    sRundata.Tb = Tb;
    sRundata.Tc = Tc;
    sRundata.pTa = pTa;
    sRundata.pTb = pTb;
    sRundata.pTc = pTc;

    sRundata.FenH = FenH;
    sRundata.FenM = FenM;
    sRundata.Tcala1 = Tcala1;
    sRundata.Tcala2 = Tcala2;
    sRundata.Tcala3 = Tcala3;
    sRundata.Tset1 = Tset1;
    sRundata.Tset2 = Tset2;
    sRundata.Tset3 = Tset3;
    sRundata.Tset4 = Tset4;
    sRundata.cshuiCha = cshuiCha;
    sRundata.cwhuiCha = cwhuiCha;
    sRundata.tzhuiCha = tzhuiCha;
    sRundata.MnTest = MnTest;

    sRundata.addr = addr;
    sRundata.jojy = jojy;
    sRundata.btlv = btlv;

    sRundata.k1 = (signed int)k1 * 10;
    sRundata.k2 = (signed int)k2 * 10;
    sRundata.k3 = (signed int)k3 * 10;
    sRundata.b1 = (signed int)b1 * 10;
    sRundata.b2 = (signed int)b2 * 10;
    sRundata.b3 = (signed int)b3 * 10;

    sRundata.macFlag[0] = FengJiSW;          // 0     风机
    sRundata.macFlag[1] = MacFailed;         // 1     故障
    sRundata.macFlag[2] = OverTemp;          // 2     超温报警
    sRundata.macFlag[3] = JumpZha;           // 3     跳闸
    sRundata.macFlag[4] = DoorJin;           // 4     门禁
    sRundata.macFlag[5] = TempASta;          // 5     a温度故障0 1过低 2过高
    sRundata.macFlag[6] = TempBSta;          // 6     b温度故障0 1过低 2过高
    sRundata.macFlag[7] = TempCSta;          // 7     c温度故障0 1过低 2过高
    sRundata.macFlag[8] = PowrASta;          // 8     a没电故障0 1没电
    sRundata.macFlag[9] = PowrBSta;          // 9     b没电故障0 1没电
    sRundata.macFlag[10] = 0x00;             // 10
    sRundata.macFlag[11] = 0x00;             // 11
    sRundata.macFlag[12] = MODAutoManuONOFF; // 12    0自动     1手动开 2手动关
    sRundata.macFlag[13] = 0x00;             // 13
    sRundata.macFlag[14] = disBeep;          // 14    0不叫     1叫  beep
    sRundata.macFlag[15] = goBeep;           // 15    0不消音   1消音
    sRundata.macFlag[16] = MODLoopShow;      // 16    0巡回     1最大温度显示
}
void u2RxIt(UART_HandleTypeDef *huart)
{

    __HAL_UART_CLEAR_IDLEFLAG(huart);

    HAL_UART_DMAStop(huart);

    __HAL_UART_CLEAR_FLAG(huart, UART_CLEAR_OREF | UART_CLEAR_NEF | UART_CLEAR_PEF | UART_CLEAR_FEF);

    RxLenU2 = MAXRXU2 - huart->hdmarx->Instance->CNDTR; // NDTR;
    unsigned short i = 0;

    extern unsigned int addr;
    if (RxBufU2[0] == addr &&
        RxBufU2[1] == 0x03 &&
        RxBufU2[2] == 0x00 &&
        RxBufU2[3] == 0x00 &&
        RxBufU2[4] == 0x00 &&
        RxBufU2[5] == 0x05 && RxLenU2 == 8)
    {
        i = 0;
        TxBufU2[0] = addr;
        TxBufU2[1] = 0x03;
        TxBufU2[2] = 0x0A;
        unsigned short Tmp = 0;
        if (Ta > Tset3)
            Tmp = Tmp | 0x0001;
        else
            Tmp = Tmp & (~0x0001);
        if (Tb > Tset3)
            Tmp = Tmp | 0x0002;
        else
            Tmp = Tmp & (~0x0002);
        if (Tc > Tset3)
            Tmp = Tmp | 0x0004;
        else
            Tmp = Tmp & (~0x0004);
        if (FengJiSW != 0)
            Tmp = Tmp | 0x0008;
        else
            Tmp = Tmp & (~0x0008);
        if (MacFailed != 0)
            Tmp = Tmp | 0x0010;
        else
            Tmp = Tmp & (~0x0010);
        if (OverTemp != 0)
            Tmp = Tmp | 0x0020;
        else
            Tmp = Tmp & (~0x0020);
        TxBufU2[3] = Tmp >> 8;
        TxBufU2[4] = Tmp >> 0;
        signed short Txxx = 0;
        Txxx = Ta * 10 + 0x23;
        TxBufU2[5] = Txxx >> 8;
        TxBufU2[6] = Txxx >> 0;
        Txxx = Tb * 10 + 0x23;
        TxBufU2[7] = Txxx >> 8;
        TxBufU2[8] = Txxx >> 0;
        Txxx = Tc * 10 + 0x23;
        TxBufU2[9] = Txxx >> 8;
        TxBufU2[10] = Txxx >> 0;
        TxBufU2[11] = FenH >> 8;
        TxBufU2[12] = FenH >> 0;
        unsigned short crc1, crc2;
        crc1 = crc2 = CRC16x(TxBufU2, 13);
        TxBufU2[13] = ((crc1)&0xff);
        TxBufU2[14] = ((crc2 >> 8) & 0xff);

        TxEn;
        ADCPause;
        HAL_UART_Transmit(huart, TxBufU2, 15, 100);
        ADCContinue;
    }
    if (
        RxBufU2[0] == 0xFA &&
        RxBufU2[1] == 0x04 &&
        RxBufU2[2] == 0xD1 &&
        RxBufU2[5] == 0xFB && RxLenU2 == 6)
    {
        MODAutoManuONOFF = RxBufU2[3];
        ADCPause;
        TxEn;
        HAL_UART_Transmit(huart, RxBufU2, rsRxIndex, 100);
        ADCContinue;
    }

    if (
        RxBufU2[0] == 0xFA &&
        RxBufU2[1] == 0x04 &&
        RxBufU2[2] == 0xD2 &&
        RxBufU2[5] == 0xFB && RxLenU2 == 6)
    {
        goBeep = RxBufU2[3];
        ADCPause;
        TxEn;
        HAL_UART_Transmit(huart, RxBufU2, rsRxIndex, 100);
        ADCContinue;
    }

    if (
        RxBufU2[0] == 0xFA &&
        RxBufU2[1] == 0x04 &&
        RxBufU2[2] == 0xF1 &&
        RxBufU2[5] == 0xFB && RxLenU2 == 6)
    {
        i = 0;
        TxBufU2[i++] = 0xFA;
        TxBufU2[i++] = 0x04;
        TxBufU2[i++] = 0xF1;
        unsigned int sDataLen = sizeof(sRundata);
        LoadReadData();
        memcpy(&TxBufU2[i], (unsigned char *)&sRundata, sDataLen);
        i = i + sDataLen;
        TxBufU2[i++] = 0xFB;
        ADCPause;
        TxEn;
        HAL_UART_Transmit(huart, TxBufU2, i, 100);
        ADCContinue;
    }
    HAL_UART_Receive_DMA(huart, RxBufU2, MAXRXU2);
    RxEn;
}