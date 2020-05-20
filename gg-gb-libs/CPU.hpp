#ifndef CPU_HPP
#define CPU_HPP
#include <cstdint>
#include <array>
using namespace std;
/* 
Done: 

Made Regs,Flags
Declared Tick

To do:

Declare operation functions,
Define tick and use switch case with the operation functions to decode instructions and execute them


*/







struct Flags{
	uint8_t : 4,
	c : 1,
	h : 1,
	n : 1,
	z : 1
}


class CPU{

private:
	/*Registers:
	af (Accumulator and flags)
	bc (General)
	de (General)
	hl (General)
	sp (Stack Pointer)
	pc (Program Counter)

	*/
	union{
		struct{
			Flags f;
			uint8_t a;
		};
	uint16_t af; // low byte is flags , high byte is accumulator
	};

	union{
		struct{
			uint8_t c;
			uint8_t b;
		};
	uint16_t bc; 
	};
	
	union{
		struct{
			uint8_t e;
			uint8_t d;
		};
	uint16_t de; 
	};
	
	union{
		struct{
			uint8_t l;
			uint8_t h;
		};
	uint16_t hl; 
	};
	
	uint16_t sp;
	uint8_t &IF = MMU.refh(0x0f); // Interrupt Register
	uint8_t &IE = MMU.refh(0xff); // Interrupt Enable Register
	uint16_t pc;
	
	//Todo: Add basic instructions
public:
	void tick();// reads opcode,decodes using switch case, executes instruction, increments PC.





	
};

#endif