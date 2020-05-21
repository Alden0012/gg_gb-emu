#include "MMU.hpp"
#include 

MMU::MMU(CPU &cp){
	cpu = cp;
}
MMU::SetupCartridge(cartridge* icart){
	cart = icart;
	mbc = cart->InitMBC(cart);
}
/*
void MMU::swapROMBank(uint8_t bank){
	for(uint8_t i = 0;i< 0x4000;i++){
		Memory[i+0x4000] = cartridge[i+bank*0x4000];
	}
}
void MMU::swapRAMBank(uint8_t bank){
	
}
*/




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
	void MMU::write(uint16_t addr,uint8_t data){
		if (addr >= 0x0000 & addr < 0x8000){
			MBC->control(addr,data);
		}
		/*else if (addr >= 0x8000 & addr < 0xA000){
			//VRAM
		}
		else if (addr >= 0xA000 & addr < 0xC000){
			//External Ram from cartridge
		}
		else if (addr >= 0xC000 & addr < 0xD000){
			//Wram bank 0
		}
		else if (addr >= 0xD000 & addr < 0xE000){
			//Wram bank 1
		}
		else if (addr >= 0xC000 & addr < 0xD000){
			//Wram bank 0
		}*/
		else if(addr >= 0xC000 & addr < 0xDE00){
			//echo
			Memory[addr] = data;
			Memory[addr + 0x2000] = data;
		}
		else if(addr >= 0xE000 & addr < 0xFE00){
			//echo
			Memory[addr] = data;
			Memory[addr - 0x2000] = data;
		}
		else{
			Memory[addr] = data;
		}


	}
	void MMU::writeh(uint8_t addr,uint8_t data){
		Memory[addr + 0xFF] = data;
	}