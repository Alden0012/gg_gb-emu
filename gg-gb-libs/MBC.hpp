#ifndef MBC_HPP
#define MBC_HPP
#include "Cartridge.hpp"
#include "MMU.hpp"
class Cartridge;
class MMU;
class MBC{
protected:
	MMU* immu;
	Cartridge* cart = nullptr;
	uint8_t *Eram = nullptr; // Eram is the entire ram space of the cartridge, so MBC.control() will switch banks to and from Eram to the 8kb Eram Bank on memory.
	uint8_t CurrentRomBank = 1;
	uint8_t CurrentRamBank = 0;
	uint8_t RamBanks = 0;
	uint8_t RomBanks = 0;
	uint8_t *Memory =  nullptr;// MMU memory
/*	uint8_t C_romSize;
	uint8_t C_ramSize;*/

	// transfer of data between cart and mmu maybe
public:
	MBC(Cartridge * icart);

	virtual void Control(uint16_t addr, uint8_t data)=0; // when you write to ROM it does some funky stuff (ram/rom bank switching)
	void setMemoryref(MMU* mmu);
	// virtual void setupEram();
	inline uint8_t getRamBanks(){return RamBanks;}
	inline uint8_t getRomBanks(){return RomBanks;}
	void initMemory();
	// virtual void ROMbankSwitch(uint8_t bank);
	// virtual int RAMbankSwitch(uint8_t bank); // only used for 32kb++ external ram.
};

class ROMonly_MBC : public MBC{
	public:
	ROMonly_MBC(Cartridge *icart) : MBC(icart){};
	inline void Control(uint16_t addr, uint8_t data){} // do nothing 


};

class MBC1 : public MBC{
	/*	C_romSize = 4 * 1024 * 1024;
	C_ramSize = 32 * 1024;*/
	public:
	MBC1(Cartridge *icart) : MBC(icart){};
	void Control(uint16_t addr, uint8_t data); 
	bool mode = 1;
};

class MBC2 : public MBC{
public:
	MBC2(Cartridge *icart) : MBC(icart){};
	void Control(uint16_t addr, uint8_t data);
};

class MBC3 : public MBC{
	void Control(uint16_t addr, uint8_t data);
};

class MBC5 : public MBC{
	void Control(uint16_t addr, uint8_t data);
};

#endif
