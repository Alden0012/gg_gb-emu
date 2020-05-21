#ifndef MBC_HPP
#define MBC_HPP
#include "Cartridge.hpp"
#include "MMU.hpp"

class MBC{
private:
	MMU* immu;
	Cartridge* cart;
	uint8_t *Eram; // Eram is the entire ram space of the cartridge, so MBC.control() will switch banks to and from Eram to the 8kb Eram Bank on memory.
	uint8_t CurrentRomBank;
	uint8_t CurrentRamBank;
	uint8_t RamBanks;
	uint8_t RomBanks;
	uint8_t *Memory;// MMU memory
/*	uint8_t C_romSize;
	uint8_t C_ramSize;*/

	// transfer of data between cart and mmu maybe
public:
	MBC();

	virtual void Control()=0; // when you write to ROM it does some funky stuff (ram/rom bank switching)
	inline void setMemoryref(MMU* mmu){Memory = mmu->returnMemref; immu = mmu;}
	virtual void setupEram();
	inline uint8_t getRamBanks{return RamBanks;}
	inline uint8_t getRomBanks{return RomBanks;}
	virtual void initMemory(Cartridge *icart);
	virtual void ROMbankSwitch(uint8_t bank);
	virtual int RAMbankSwitch(uint8_t bank); // only used for 32kb++ external ram.
}

class ROMonly_MBC : public MBC{
	inline void Control(uint16_t addr){} // do nothing 
}

class MBC1 : public MBC{
	/*	C_romSize = 4 * 1024 * 1024;
	C_ramSize = 32 * 1024;*/
	void Control(uint16_t addr); 
	bool mode = 1;
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
