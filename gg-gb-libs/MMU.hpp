#ifndef MMU_HPP
#define MMU_HPP
#include<cstdint>



class MMU{
private:
	/*
		MMU Memory Map: 
		0x0000 - 0x3FFF: ROM bank 0 from the cartridge | 16kB
		0x4000 - 0x7FFF: ROM bank n from the cartridge (this rom bank can be selected using the MBC in the cartridge) | 16kB
		0x8000 - 0x9FFF: VRAM | 8kB
		0xA000 - 0xBFFF: External Ram (From the cartridge) | 8kB
		0xC000 - 0xCFFF: 4kB WRAM Bank 0
		0xD000 - 0xDFFF: 4kB WRAM Bank 1
		0xE000 - 0xF0FF: Echo (Same as WRAM but typically unused) if you write to the actual RAM in range (0xC000 - 0xDE00) it writes to here as well
		0xFE00 - 0xFE9F: Sprite Attribute Table (OAM)
		0xFEA0 - 0xFEFF: Not usable
		0xFF00 - 0xFF7F: Screen I/0 Ports
		0xFF80 - 0xFFFE: High RAM
		0xFFFF: Interrupt Enable Register

		Reserved Mem Locations:
			0000: RST $00 calls this
			0008: RST $08 calls this
			0010: RST $10 calls this
			0018: RST $18 calls this
			0020: RST $20 calls this
			0028: RST $28 calls this
			0030: RST $30 calls this
			0038: RST $38 calls this
			0040: V_Blank Interrupt Start Address
			0050: LCDC Status Interrupt Start Address
			0058: Serial Transfer Completion Interrupt Start Address
			0060: High-to-low of P10-P13 Interrupt start address

	*/
	uint8_t Memory[65536];
	uint8_t MBC = 0;
	Cartridge &cartridge;
	CPU &cpu;

	void swapROMBank(uint8_t bank);
	void swapRAMBank(uint8_t bank);
public:
	MMU();
	//Read
	uint8_t read(uint16_t addr);
	uint16_t read16(uint16_t addr);
	uint8_t readh(uint8_t addr);
	uint16_t read16h(uint8_t addr);
	uint8_t &ref(uint16_t addr){return Memory[addr];}
	uint8_t &refh(uint16_t addr){return Memory[0xFF00 + addr];}
	//Write
	void write(uint16_t addr,uint8_t data);
	void writeh(uint8_t addr,uint8_t data);
	void write16(uint16_t addr,uint16_t data);
	void write16h(uint8_t addr,uint16_t data);
	//Utility
	inline uint8_t readbit(uint8_t num,uint8_t index){
		return (num >> index) & 0x1;
	}
	inline uint8_t writebit(uint8_t num,uint8_t index, bool val){
		if(val){
		return num | (0x1 << index);
		}else{
		return num & ~(0x1 << index);
		}
	}
	//todo: save()

}
#endif