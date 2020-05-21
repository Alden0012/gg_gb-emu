#ifndef MBC_HPP
#define MBC_HPP
#include "Cartridge.hpp"
#include "MMU.hpp"
class MBC{
private:
	MMU* immu;
	Cartridge* cart; 
	uint8_t *Memory;// MMU memory
/*	uint8_t C_romSize;
	uint8_t C_ramSize;*/

	// transfer of data between cart and mmu maybe
public:
	MBC();
	/*inline void setmmu(MMU *mem){
		mmu = mem;
	}*/
	virtual void Control()=0; // when you write to ROM it does some funky stuff (ram/rom bank switching)
	inline void setMemoryref(MMU* mmu){Memory = mmu->returnMemref; immu = mmu;}
	virtual void initMemory(Cartridge *icart);
	virtual void ROMbankSwitch(uint8_t bank);
	virtual void RAMbankSwitch(uint8_t bank);
}
class ROMonly_MBC : public MBC{
	inline void Control(uint16_t addr){} // do nothing 
}
class MBC1 : public MBC{
	/*	C_romSize = 4 * 1024 * 1024;
	C_ramSize = 32 * 1024;*/
	void Control(uint16_t addr); 
}
class MBC2 : public MBC{
	void Control(uint16_t addr);
}
class MBC3 : public MBC{
	void Control(uint16_t addr);
}
class MBC5 : public MBC{
	void Control(uint16_t addr);
}
#endif
