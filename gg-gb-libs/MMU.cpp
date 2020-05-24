#include "MMU.hpp"
#include <iostream>

MMU::MMU(CPU &cp){
	cpu = cp;
	//boot sequence
	Memory[0xFF05] = 0x00;
	Memory[0xFF06] = 0x00;
	Memory[0xFF07] = 0x00;
	Memory[0xFF0F] = 0xE1;
	Memory[0xFF10] = 0x80;
	Memory[0xFF11] = 0xBF;
	Memory[0xFF12] = 0xF3;
	Memory[0xFF14] = 0xBF;
	Memory[0xFF16] = 0x3F;
	Memory[0xFF17] = 0x00;
	Memory[0xFF19] = 0xBF;
	Memory[0xFF1A] = 0x7F;
	Memory[0xFF1B] = 0xFF;
	Memory[0xFF1C] = 0x9F;
	Memory[0xFF1E] = 0xBF;
	Memory[0xFF20] = 0xFF;
	Memory[0xFF21] = 0x00;
	Memory[0xFF22] = 0x00;
	Memory[0xFF23] = 0xBF;
	Memory[0xFF24] = 0x77;
	Memory[0xFF25] = 0xF3;
	Memory[0xFF26] = 0xF1;
	Memory[0xFF40] = 0x91;
	Memory[0xFF42] = 0x00;
	Memory[0xFF43] = 0x00;
	Memory[0xFF45] = 0x00;
	Memory[0xFF47] = 0xFC;
	Memory[0xFF48] = 0xFF;
	Memory[0xFF49] = 0xFF;
	Memory[0xFF4A] = 0x00;
	Memory[0xFF4B] = 0x00;
	Memory[0xFFFF] = 0x00;
}
void MMU::SetupCartridge(cartridge* icart){
	cart = icart;
	mbc = cart->InitMBC(cart);
}

//Read TODO: mem read special cases.
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
	void MMU::write(uint16_t addr,uint8_t data){
		if (addr >= 0x0000 && addr < 0x8000){
			mbc->control(addr,data);
		}
		/*else if (addr >= 0x8000 & addr < 0xA000){
			//VRAM
		}*/
		else if (addr >= 0xA000 & addr < 0xC000){
			//External Ram from cartridge
			if(Ram_En){
			Memory[addr] = data;
			}
		}/*
		else if (addr >= 0xC000 & addr < 0xD000){
			//Wram bank 0
		}
		else if (addr >= 0xD000 & addr < 0xE000){
			//Wram bank 1
		}
		*/
		else if(addr >= 0xC000 && addr < 0xDE00){
			//echo
			Memory[addr] = data;
			Memory[addr + 0x2000] = data;
		}
		else if(addr >= 0xE000 && addr < 0xFE00){
			//echo
			Memory[addr] = data;
			Memory[addr - 0x2000] = data;
		}
		else if(addr >= 0xFEA0 && addr < 0xFF00){
			std::cerr << "unusable ram" << std::endl;
		}
		else if(addr >= 0xFF00 && addr < 0xFF80){
			writeIO(addr,data);//make this;
		}
		else {
			Memory[addr] = data;
		}


	}
	void MMU::writeh(uint8_t addr,uint8_t data){
		Memory[addr + 0xFF] = data;
	}