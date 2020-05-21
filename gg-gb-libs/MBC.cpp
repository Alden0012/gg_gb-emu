#include "MBC.hpp"
MBC::MBC(Cartridge * icart){
	cart = icart;
		switch(cart->getROM_Size()){
		case 0:
			RomBanks = 2;
		case 1:
			Rombanks = 4;
		case 2:
			RomBanks = 8;
		case 3:
			RomBanks = 16;
		case 4: 
			RomBanks = 32;
		case 5:
			RomBanks = 64;
		case 6:
			RomBanks = 128;
		case 0x52:
			RomBanks = 72;
		case 0x53:
			RomBanks = 80;
		case 0x54:
			RomBanks = 96;

	}
	switch(cart->getRAM_Size()){
		case 0;
			RamBanks = 0;
		case 1:
			RamBanks = 1;
		case 2:
			RamBanks = 1;
		case 3:
			RamBanks = 4;
		case 4:
			RamBanks = 16;

	}
	uint8_t *TempRam[RamBanks * 8 * 1024];
	Eram = TempRam;
}
void MBC::initMemory(){// load initial banks 0 and 1 to memory
	CurrentRomBank = 1;
	uint8_t *CartROM = cart->getROM();
	for(uint16t i = 0; i < 0x8000;i++){
		Memory[i] = CartROM[i]
	}
}
//-------------------------------------Rom Only MBC
// were you expecting something ?
//-------------------------------------MBC1
void MBC1::Control(uint16_t addr,uint8_t data){
	if(addr >= 0x2000 && addr < 0x4000){ //    Switch banks
		uint8_t bank = data & 0x1F;
		if (bank !=0){CurrentRomBank = bank;}else{CurrentRomBank = 1;} 
		for(uint16_t i = 4000; i < 0x8000;i++ ){
			if (bank != 0){
				Memory[i] = CartROM[i+0x4000*bank];

			}
			else{
				Memory[i] = CartROM[i+0x4000];

			}
		}
	}
	else if(addr >= 0x0000 && addr < 0x2000){// Wram enable
		if(data == 0x00){
			immu->RAM_En = false;
		}
		else if(data == 0x0A){
			immu->RAM_En = true;
		}

	}
	else if(addr >= 0x6000 && addr < 0x8000){// mode select
		if((data& 0x1) == 0){
			mode = 0;
		}else{
			mode = 1;
		}
	else if(addr >= 0x4000 && address < 0x6000){// set RAM if mode = 1, or set upper bits of rom bank if rom = 0
		if(mode == 1){
			CurrentRamBank = data & 0x3
			for(uint16_t i = 0x0;i<0x2000;i++){
				Memory[0xA000+i] = Eram[i + (CurrentRamBank)*0x2000] //switching ram banks
			}
		}
		else{
			//this is stupid ayaya! ayaya!!!
			CurrentRomBank = (CurrentRomBank & 01F)|((data & 0x03)<<5) //set lower bits of current rom bank or sumn man
			if(CurrentRomBank == 0x0 || CurrentRomBank == 0x20 || CurrentRomBank == 0x40 || CurrentRomBank = 0x60){// Current rom bank cannot be special rom bank
				CurrentRomBank++;
			}
			CurrentRomBank &= (RomBanks-1) //restrict it back to range i guess
			
			for(uint16_t i = 4000; i < 0x8000;i++ ){
				Memory[i] = CartROM[i+0x4000*CurrentRomBank];
			}
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