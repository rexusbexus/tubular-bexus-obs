#ifndef COMMANDTRANSLATOR_H
#define COMMANDTRANSLATOR_H

#include <stdlib.h>
#include <vector>
#include <cstdint>

// #define separator 0x2C
// #define m 0x6D
// #define d 0x64
// #define h 0x68
// #define t 0x74
// #define r 0x72
// #define a 0x61
// #define s 0x73
// #define c 0x63

typedef uint8_t byte;
typedef bool boolean;




int checkComma(byte data[], int j, byte datasize);
boolean checkCommand(byte data[]);
std::vector<std::vector<byte>> scanBuffer(byte bufferD[], int row, int col, byte datasize);
void collectingCommand (byte data_tcp[], int row, int col, byte datasize);


#endif