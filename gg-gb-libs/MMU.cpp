#include "MMU.hpp"
#include 
void swapROMBank(uint8_t bank){
	for(uint8_t i = 0;i< 0x4000;i++){
		Memory[i+0x4000] = cartridge[i+bank*0x4000];
	}
}
void swapRAMBank(uint8_t bank){
	
}




//Read
	uint8_t MMU::read(uint16_t addr){
		return Memory[addr];
	}
	uint16_t MMU::read16(uint16_t addr){
		return (Memory[addr+1] << 8 | Memory[addr]);
	}
	uint8_t MMU::readh(uint8_t addr){
		return (Memory[0xFF00+addr]);
	}
	uint16_t MMU::read16h(uint8_t addr){
		return (Memory[0xFF00+addr+1] << 8 | Memory[0xFF00+addr]);
	}

//Write (dont forget to implement echo write for 0xC000-0xDE00 and 0xE000-0xFE00)
	void MMU::write(uint16_t addr,uint8_t data);
	void MMU::writeh(uint8_t addr,uint8_t data);
	void MMU::write16(uint16_t addr,uint16_t data);
	void MMU::write16h(uint8_t addr,uint16_t data);