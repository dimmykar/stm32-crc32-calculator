/**
 * @file           : stm32-crc32.c
 * @author         : Dmitry Karasev    <karasevsdmitry@yandex.ru>
 * @brief          : STM32 CRC32 software calculator
 * @date           : 2022-04-16
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2022 Dmitry KARASEV
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
 * AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * Author:          Dmitry KARASEV <karasevsdmitry@yandex.ru>
 *
 ******************************************************************************
 */

/*============================================================================*
 *                                  Includes
 *============================================================================*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stddef.h>


/*============================================================================*
 *                          Private defines & types
 *============================================================================*/
#define APP_VERSION             "1.0.0"


/*============================================================================*
 *                                Private data
 *============================================================================*/
char filename[32];

unsigned long sw_crc32_by_byte_table[256] = {
    0x00000000, 0x04C11DB7, 0x09823B6E, 0x0D4326D9, // 0..3
    0x130476DC, 0x17C56B6B, 0x1A864DB2, 0x1E475005, // 4..7
    0x2608EDB8, 0x22C9F00F, 0x2F8AD6D6, 0x2B4BCB61, // 8..B
    0x350C9B64, 0x31CD86D3, 0x3C8EA00A, 0x384FBDBD, // C..F

    0x4C11DB70, 0x48D0C6C7, 0x4593E01E, 0x4152FDA9,
    0x5F15ADAC, 0x5BD4B01B, 0x569796C2, 0x52568B75,
    0x6A1936C8, 0x6ED82B7F, 0x639B0DA6, 0x675A1011,
    0x791D4014, 0x7DDC5DA3, 0x709F7B7A, 0x745E66CD,
    0x9823B6E0, 0x9CE2AB57, 0x91A18D8E, 0x95609039,
    0x8B27C03C, 0x8FE6DD8B, 0x82A5FB52, 0x8664E6E5,
    0xBE2B5B58, 0xBAEA46EF, 0xB7A96036, 0xB3687D81,
    0xAD2F2D84, 0xA9EE3033, 0xA4AD16EA, 0xA06C0B5D,
    0xD4326D90, 0xD0F37027, 0xDDB056FE, 0xD9714B49,
    0xC7361B4C, 0xC3F706FB, 0xCEB42022, 0xCA753D95,
    0xF23A8028, 0xF6FB9D9F, 0xFBB8BB46, 0xFF79A6F1,
    0xE13EF6F4, 0xE5FFEB43, 0xE8BCCD9A, 0xEC7DD02D,
    0x34867077, 0x30476DC0, 0x3D044B19, 0x39C556AE,
    0x278206AB, 0x23431B1C, 0x2E003DC5, 0x2AC12072,
    0x128E9DCF, 0x164F8078, 0x1B0CA6A1, 0x1FCDBB16,
    0x018AEB13, 0x054BF6A4, 0x0808D07D, 0x0CC9CDCA,
    0x7897AB07, 0x7C56B6B0, 0x71159069, 0x75D48DDE,
    0x6B93DDDB, 0x6F52C06C, 0x6211E6B5, 0x66D0FB02,
    0x5E9F46BF, 0x5A5E5B08, 0x571D7DD1, 0x53DC6066,
    0x4D9B3063, 0x495A2DD4, 0x44190B0D, 0x40D816BA,
    0xACA5C697, 0xA864DB20, 0xA527FDF9, 0xA1E6E04E,
    0xBFA1B04B, 0xBB60ADFC, 0xB6238B25, 0xB2E29692,
    0x8AAD2B2F, 0x8E6C3698, 0x832F1041, 0x87EE0DF6,
    0x99A95DF3, 0x9D684044, 0x902B669D, 0x94EA7B2A,
    0xE0B41DE7, 0xE4750050, 0xE9362689, 0xEDF73B3E,
    0xF3B06B3B, 0xF771768C, 0xFA325055, 0xFEF34DE2,
    0xC6BCF05F, 0xC27DEDE8, 0xCF3ECB31, 0xCBFFD686,
    0xD5B88683, 0xD1799B34, 0xDC3ABDED, 0xD8FBA05A,
    0x690CE0EE, 0x6DCDFD59, 0x608EDB80, 0x644FC637,
    0x7A089632, 0x7EC98B85, 0x738AAD5C, 0x774BB0EB,
    0x4F040D56, 0x4BC510E1, 0x46863638, 0x42472B8F,
    0x5C007B8A, 0x58C1663D, 0x558240E4, 0x51435D53,
    0x251D3B9E, 0x21DC2629, 0x2C9F00F0, 0x285E1D47,
    0x36194D42, 0x32D850F5, 0x3F9B762C, 0x3B5A6B9B,
    0x0315D626, 0x07D4CB91, 0x0A97ED48, 0x0E56F0FF,
    0x1011A0FA, 0x14D0BD4D, 0x19939B94, 0x1D528623,
    0xF12F560E, 0xF5EE4BB9, 0xF8AD6D60, 0xFC6C70D7,
    0xE22B20D2, 0xE6EA3D65, 0xEBA91BBC, 0xEF68060B,
    0xD727BBB6, 0xD3E6A601, 0xDEA580D8, 0xDA649D6F,
    0xC423CD6A, 0xC0E2D0DD, 0xCDA1F604, 0xC960EBB3,
    0xBD3E8D7E, 0xB9FF90C9, 0xB4BCB610, 0xB07DABA7,
    0xAE3AFBA2, 0xAAFBE615, 0xA7B8C0CC, 0xA379DD7B,
    0x9B3660C6, 0x9FF77D71, 0x92B45BA8, 0x9675461F,
    0x8832161A, 0x8CF30BAD, 0x81B02D74, 0x857130C3,
    0x5D8A9099, 0x594B8D2E, 0x5408ABF7, 0x50C9B640,
    0x4E8EE645, 0x4A4FFBF2, 0x470CDD2B, 0x43CDC09C,
    0x7B827D21, 0x7F436096, 0x7200464F, 0x76C15BF8,
    0x68860BFD, 0x6C47164A, 0x61043093, 0x65C52D24,
    0x119B4BE9, 0x155A565E, 0x18197087, 0x1CD86D30,
    0x029F3D35, 0x065E2082, 0x0B1D065B, 0x0FDC1BEC,
    0x3793A651, 0x3352BBE6, 0x3E119D3F, 0x3AD08088,
    0x2497D08D, 0x2056CD3A, 0x2D15EBE3, 0x29D4F654,
    0xC5A92679, 0xC1683BCE, 0xCC2B1D17, 0xC8EA00A0,
    0xD6AD50A5, 0xD26C4D12, 0xDF2F6BCB, 0xDBEE767C,
    0xE3A1CBC1, 0xE760D676, 0xEA23F0AF, 0xEEE2ED18,
    0xF0A5BD1D, 0xF464A0AA, 0xF9278673, 0xFDE69BC4,
    0x89B8FD09, 0x8D79E0BE, 0x803AC667, 0x84FBDBD0,
    0x9ABC8BD5, 0x9E7D9662, 0x933EB0BB, 0x97FFAD0C,
    0xAFB010B1, 0xAB710D06, 0xA6322BDF, 0xA2F33668,
    0xBCB4666D, 0xB8757BDA, 0xB5365D03, 0xB1F740B4
};

uint32_t crc32 = 0xFFFFFFFF;


/*============================================================================*
 *                             Private functions
 *============================================================================*/
uint32_t GetFileSize(char* filename);
uint32_t GetFileCRC32(char* filename);
uint32_t CalculateSwCRCByByte(uint32_t crc32, const uint8_t* pbuffer, uint32_t num_of_bytes);
void     PrintAppHeader(void);


/*============================================================================*/





int main(int argc, char** argv)
{
    PrintAppHeader();

    /* Check for number of passed arguments */
    if (argc != 2) {
        printf("Error: found %d input arguments. Needs exactly 1\n", argc - 1);
        puts("Before first using of this script see README.md");
        exit(EXIT_FAILURE);
    }

    /* Forming a file name from command line argument */
    sprintf(filename, "%s.bin", argv[1]);

    GetFileCRC32(filename);

    return 0;
}

uint32_t GetFileSize(char* filename)
{
    uint32_t size = 0;

    FILE* fp;
    if((fp = fopen(filename, "rb")) == NULL) {
        printf("Error occured while opening '%s'\n", filename);
        return 0;
    }

    fseek(fp, 0, SEEK_END);
    size = ftello(fp);

    fclose(fp);

    printf("File '%s' size is %d bytes\n", filename, size);

    return size;
}

uint32_t GetFileCRC32(char* filename)
{
    uint32_t fw_size;
    uint32_t fw_size_in_dword;
    uint8_t  fw_size_of_tail;
    uint8_t  fw_buf[4];

    FILE* fp;
    if((fp = fopen(filename, "rb")) == NULL) {
        printf("Error occured while opening '%s'\n", filename);
        return 0;
    }

    fw_size  = GetFileSize(filename);
    fw_size_in_dword = fw_size >> 2;
    fw_size_of_tail =  fw_size & 3;

    puts("Calculating CRC32 checksum of the binary file ...");

    for (uint32_t i = 0; i < fw_size_in_dword; ++i) {
        size_t res = fread(fw_buf, sizeof(uint8_t), sizeof(fw_buf), fp);
        if (res != sizeof(fw_buf)) {
            puts("Error occured while reading binary file!");
            exit(EXIT_FAILURE);
        }

        crc32 = CalculateSwCRCByByte(crc32, fw_buf, sizeof(fw_buf));
    }

    if (fw_size_of_tail != 0) {
        size_t res = fread(fw_buf, sizeof(uint8_t), fw_size_of_tail, fp);
        if (res != fw_size_of_tail) {
            puts("Error occured while reading binary file!");
            exit(EXIT_FAILURE);
        }

        crc32 = CalculateSwCRCByByte(crc32, fw_buf, fw_size_of_tail);
    }

    fclose(fp);

    printf("Success! Calculated CRC32 of the binary file is equal 0x%08X\n\n", crc32);

    return 1;
}

/*
 * Function description:
 * Each write operation into the data register creates a combination of
 * the previous CRC value and the new one. Initial crc32 value must be 0xFFFFFFFF.
 * CRC computation is done on the whole 32-bit data word, and not byte per byte.
 *
 * Algorithm taken from the website
 *       https://www.cnblogs.com/shangdawei/archive/2013/04/28/3049789.html
 */
uint32_t CalculateSwCRCByByte(uint32_t crc32, const uint8_t* pbuffer, uint32_t num_of_bytes)
{
    uint32_t last_data;
    uint32_t num_of_dword = num_of_bytes >> 2;
    uint32_t num_of_tail_byte =  num_of_bytes & 3 ;

    while (num_of_dword--) {
        crc32 = crc32 ^ *((unsigned long*)pbuffer);
        pbuffer += 4;
        
        for (uint32_t i = 0; i < 4; ++i) {
            crc32 = (crc32 << 8) ^ sw_crc32_by_byte_table[crc32 >> 24];
        }
    }

    switch (num_of_tail_byte) {
        case 0:
            return crc32;
        case 1:
            last_data = pbuffer[0] << 24;
            break;
        case 2:
            last_data = *((uint16_t*)(&pbuffer[0]));
            last_data <<= 16;
            break;
        case 3:
            last_data = *((uint16_t*)(&pbuffer[0]));
            last_data <<= 8;
            last_data += pbuffer[2] << 24;
            break;
        default:
            break;
    }

    crc32 = CalculateSwCRCByByte(crc32, (uint8_t*)&last_data, 4);

    return crc32;
}

void PrintAppHeader(void)
{
    puts("");
    puts("------------------------------------------------------------------------------");
    printf("                     Software STM32 CRC32 Calculator v%s\n", APP_VERSION);
    puts("                       Copyright (c) 2022 Dmitry Karasev");
    puts("");
    puts("                       Algorithm taken from the website");
    puts("      https://www.cnblogs.com/shangdawei/archive/2013/04/28/3049789.html      ");
    puts("------------------------------------------------------------------------------");
    puts("\n");
}
