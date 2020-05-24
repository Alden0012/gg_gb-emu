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
	// union{
	// 	struct{
	// 		Flags f;
	// 		uint8_t a;
	// 	};
	// uint16_t af; // low byte is flags , high byte is accumulator
	// };

	// union{
	// 	struct{
	// 		uint8_t c;
	// 		uint8_t b;
	// 	};
	// uint16_t bc; 
	// };
	
	// union{
	// 	struct{
	// 		uint8_t e;
	// 		uint8_t d;
	// 	};
	// uint16_t de; 
	// };
	
	// union{
	// 	struct{
	// 		uint8_t l;
	// 		uint8_t h;
	// 	};
	// uint16_t hl; 
	// };
	
	uint16_t af, bc, de, hl; 
	uint16_t sp;
	uint8_t &IF; // Interrupt Register
	uint8_t &IE; // Interrupt Enable Register
	uint16_t pc;
	bool interrupts_enabled;
	bool halted;
	MMU* mmu;
	//Todo: Add basic instructions
public:
	CPU();

	unsigned tick();// reads opcode,decodes using switch case, executes instruction, increments PC.
	unsigned ExecuteOpcode();
	unsigned ExecuteCB();
	unsigned CBExec(); // if the opcode was 0xCB, the next instruction byte is looked up from a seperate table dont forget to pc step +2 during tick.
	void interrupt_check();
	inline void attachMem(MMU* immu){
		mmu = immu;
		IF = mmu.refh(0x0f);
		IE = mmu.refh(0xff);
	}


	//helper for opcodes
	void UbAdd(uint16_t *reg x,uint8_t n,bool carry);
	void LbAdd(uint16_t *reg x,uint8_t n, bool carry);
	void UbSub(uint16_t *reg x,uint8_t n,bool carry);
	void LbSub(uint16_t *reg x,uint8_t n, bool carry);	

	void set_flag_zero(bool set);
	void set_flag_subtract(bool set);
	void set_flag_hcarry(bool set);
	void set_flag_carry(bool set);
	bool get_flag_carry();
	bool get_flag_zero();
	bool get_flag_subtract();
	bool get_flag_hcarry();
	void ANDa(const uint8_t byte);
	void ORa(const uint8_t byte);
	void XORa(const uint8_t byte);
	void CPa(const uint8_t n);
	void AddtoHL(const uint16_t &reg);
	void rst(const uint16_t addr);
	void push(uint16 &reg);
	void pop(uint16 &reg);
	void ret();
	void call();
	uint8_t RL(uint8_t n,bool C);
	uint8_t RR(uint8_t n,bool C);




	
};

#endif