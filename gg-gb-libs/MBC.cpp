#include "MBC.hpp"
MBC::MBC(Cartridge * icart){
	cart = icart;
}
void ROMonly_MBC::initMemory(){

	uint8_t *CartROM = cart->getROM();
	for(uint16_t i = 0; i < 0x8000;i++ ){
		Memory[i] = CartROM[i];
	}
	uint8_t romSize = cart->getROM_Size();
	switch (romSize)
	{
		case 0x1:// stub: setup ram  

	}
}
void MBC1::Control(uint16_t addr,uint8_t data){
	if(addr >= 0x2000 & addr < 0x4000){ //Switch banks
		uint8_t bank = data & 0x1F;
		for(uint16_t i = 4000; i < 0x8000;i++ ){
			if (bank != 0){
				Memory[i] = CartROM[i+0x4000*bank];
			}
			else{
				Memory[i] = CartROM[i+0x4000];
			}
		}
	else if(addr >= 0x0000 & addr < 0x2000){// Wram enable
		if(data == 0x00){
			immu->RAM_En = false;
		}
		else if(data == 0x0A){
			immu->RAM_En = true;
		}

	}
	}
	//todo: ram switching
}
void MBC2::Control(uint16_t addr,uint8_t data){
	if(addr >= 0x2000 & addr < 0x4000){
		uint8_t bank = data & 0x0F;
		for(uint16_t i = 4000; i < 0x8000;i++ ){
			if (bank != 0){
				Memory[i] = CartROM[i+0x4000*bank];
			}
			else{
				Memory[i] = CartROM[i+0x4000];
			}
		}
	}
}