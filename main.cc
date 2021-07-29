#include <stdio.h>
#include <stdlib.h>

/*
  Author: Ernesto Jimenez V.
  Email: ernjivi@gmail.com

  Based on the work of Dave Poo
  http://www.obelisk.me.uk/6502/
*/


using Byte = unsigned char;
using Word = unsigned short;
using u32 = unsigned int;


struct Mem
{
  static constexpr u32 MAX_MEM = 1024 * 64;
  Byte Data[MAX_MEM];

  void initialize()
  {
    for(u32 i = 0; i < MAX_MEM; i++)
    {
      Data[i] = 0;
    }
  }

  Byte operator[](u32 address) const
  {
    return Data[address];
  }

  Byte& operator[](u32 address)
  {
    return Data[address];
  }
};

struct CPU
{

  Word PC;      // Program Counter
  Word SP;      // Stack Pointer
  
  Byte A, X, Y; // Registers

  // Status Flags
  Byte C : 1;
  Byte Z : 1;
  Byte I : 1;
  Byte D : 1;
  Byte B : 1;
  Byte V : 1;
  Byte N : 1;

  // Instruction codes
  static constexpr Byte
    INS_LDA_IM = 0xA9;

  void reset(Mem& memory)
  {
    PC = 0xFFFC;
    SP = 0x0100;
    D = Z = I = D = B = V = N = 0;
    A = X = Y = 0;
    memory.initialize();
  }

  Byte fetchByte(u32& cycles, Mem& memory)
  {
    Byte data = memory[PC];
    PC++;
    cycles--;
    return data;
  }

  void execute(u32 cycles, Mem& memory)
  {
    while(cycles > 0)
    {
      Byte ins = fetchByte(cycles, memory);
      switch(ins)
      {
        case INS_LDA_IM:
        {
          Byte value = fetchByte(cycles, memory);
          A = value;
          Z = (A == 0);
          N = (A & 0x80) > 0;
        } break;
        default:
        {
          printf("Instruction not handled %d", ins);
        } break;
      }
    }
  }
};



int main()
{
  Mem mem;
  CPU cpu;
  cpu.reset(mem);
  mem[0xFFFC] = CPU::INS_LDA_IM;
  mem[0xFFFD] = 0x42;
  cpu.execute(2, mem);
  return 0;
}