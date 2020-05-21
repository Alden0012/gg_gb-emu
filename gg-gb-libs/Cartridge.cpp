#include "Cartridge.hpp"

Cartridge::Cartridge(MMU* mem){
	mmu = mem;
}

bool Cartridge::load(char* path,MMU* mmu){
	//LOAD rom into buffer (cart_rom)
	ifstream in_file(path, ios::in | ios::binary);
	if(!in_file.is_open()){
		fprintf(stderr,'failed to open rom\n');
		return false;
	}
	in_file.seekg(0, in_file.end);
	int len = in_file.tellg();
	in_file.seekg(0,in_file.beg);
	in_file.read((char *)cart_rom,length);
	in_file.close();
	//Get type and rom/ram size from buffer
	Cart_Type = cart_rom[0x147];
	rom_Size = cart_rom[0x148];
	ram_Size = cart_rom[0x149];
	return true;
}
MBC *Cartridge::InitMBC(Cartridge *cart){
	uint8_t type = Cart_Type;
	switch(type){
		case 0x0: 
			imbc = ROMonly_MBC(cart);
			imbc.setMemoryref(mmu);
			return &imbc;
		case 0x1:
			imbc = MBC1(cart);
			imbc.setMemoryref(mmu);
			return &imbc;

	}
}






// void Cartridge::swapROMBank(uint8_t bank, MMU *mmu);
// void Cartridge::swapRAMBank(uint8_t bank, MMU *mmu);