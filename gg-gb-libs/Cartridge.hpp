#ifndef Cartridge_HPP
#define Cartridge_HPP
#include <cstdint>
#include <iostream>
#include <fstream>
#include "MMU.hpp"
#include "MBC.hpp"
#include <array>
class Cartridge{
	/*
	enjoy reading all of this :)

	Todo: Implement MBC here or in MMU


	Internal Information Area: [0100 - 014F]
		0100-0103: Begin Code execution point
		0104-0133: Scrolling Nintendo graphic
		0134-0142 Title of the game in U.case ASCII, if less than 16 characters the rest of bytes is filled with 00s
		0143: $80 gbc, other = other
		0144: Ascii hex digit, high nibble of licensee code(bruh they ascii'd this)
		0145: Ascii hex digit, low nibble of licensee code. Both of these as 0x00 if $014B is not 33
		0146: Gameboy/SGB Indicator: 00 if gb, 03 if sgb
		0147: Cartridge Types
		0148: ROM Size
		0149: RAM Size
		014A: Destination code: 0 - japan, 1- non japan
		014B: Licensee code
		014C: Mask ROM Version number
		014D: Complement Check
		014E-014F: Checksum , add all bytes of cartridge except for two checksum bytes and take lower two bytes of the result (gb ignores this?? BRUH I SPENT TIME WRITING THIS)
	
	Cartridge Types:
		0: Rom Only 
		1: Rom + mbc1 
		2: rom + mbc1 + ram
		3: rom + mbc1 + ram + batt
		5: rom + mbc2
		6: rom + ram
		8: rom + mbc2 + batt
		B: rom + mmm01
		C: rom + mmm01 + sram
		D: rom + mm01 + sram + bat
		F: rom + mbc2 + timer + batt
		10: rom + mbc3 + timer + ram + batt
		11: rom + mbc3
		12: rom + mbc3 + ram
		13: rom + mbc3 + ram + batt
		19: rom + mbc5
		1A: rom + mbc5 + ram
		1B: rom + mbc5 + ram + batt
		1C: rom + mbc5 + rumble
		1D: rom + mbc5 + rumble + sram
		1E: rom + mbc5 + rumble + sram + batt
		1F: pocket camera(bruh)
		FD: Bandai Tama5(bruh)
		FE: Hudson HuC-3(bruh)
		FF: Hudson Huc-1(bruh)
	
	Meaning: 
		ROM: occupies 0x0000-0x7FFF
		MBC1: 
			  two different max mem modes {16Mbit ROM/8Kb RAM , 4MBit ROM/32Kb RAM} defaults to [1] on startup, 
			  writing to the lsb of a byte in 0x6000-0x7FFF selects the mode (0 -> 16/8, 1-> 4/32)
			  writing to the first 5 bits of a byte in 0x2000-0x3FFF will select an appropriate ROM bank at 0x4000-0x7FFF (0 and 1 point to rom bank 1)
			  4/32 mode: Writing to 0x4000-5FFF will select an appropriate RAM bank at 0xA000-BFFF
			  			 Before rw to RAM enable by writing XXXX1010 to 0000-01FFF area. To disable write XXXX0000 to the area.
			  16/8 mode: Writing to the last 2 bits of a value in 0x4000-5FFF will set the two most significant ROM address lines.
		MBC2: 
			  works like MBC 1 with few exceptions:
			  works with ROM sizes up  to 2Mbit
			  writing a value XXXXBBBB into 0x2100-21FF area will select an appopriate rom bank to 0x4000-0x7FFF
			  RAM switching not provided, the 512x4 ram is in the controller itself rather than having eRAM like mbc1, requires battery to save data. 
			  Suggested address range for mbc2 ram enable is 0000-00FF
		MBC3:
			  Similar to MB1 one but it access all 16mbits of ROM without requiring writes to 0x4000-0x5FFF area.
			  writing value to XBBBBBBB to 0x2000 - 0x3FFF area will select an appropriate ROM bank at 4000-7FFF
			  has built in battery-backed real time clock
			  some mbc3 carts do notttttttttttttttttttttttttttt supprt it.
		MBC5:
			 Similar to MBC3 but can access up to 64mBits of ROM and 1mBit of RAM. 
			 lower 8 bits of 9 bit rom select is written to 0x2000-0x2FFF while the remaning upper bit is written to least significant bit of 0x3000-0x3FFF area
			 writing a value XXXXBBBB into 0x4000-0x5FFF selects appropriate RAM bank at 0xA000-0xBFFF if cart contains ram, sizes are 64kbit,256kbit,1mbit
			 allows rom bank 0 to appear in 4000-7FFF by writing 000 to rom bank select. (why would u need this)
		Rumble Carts:
			 stupid,only used with mbc5 if you have a rumble cart your ram is limited to 256 kbit rather than 1mbit
		Huc1:
			 what kinda bootleg is this, its just mbc1 with infrared led i/o, no im not implementing this.
		Bung Carts: 
			 light em up man,
			 its an mbc5 but writing 00 to the bank select reg selects rom bank 1 instead of rom bank 0, writing to 3000-3FFF does NOTHING!!?? just 1 byte rom select i guess
			 writing values to 0x6000-0x7FFF with lsb = 0 puts it in MBC1 16/8 mode but writes to 0x3000-0x3FFF does nothing, if lsb = 1, puts it in mbc5. default s=1
		Cart Location 0xA000 and 0xA100:
			special write only regs if c0 is written to 0x0000-0x1FFF area. hardware control regs used to set up cart hardware for different games for bung cart.

		ROM Size: 
			0- 32 kb   / 2   banks
			1- 64 kb   / 4   banks
			2- 128 kb  / 8   banks
			3- 256 kb  / 16  banks
			4- 512 kb  / 32  banks
			5- 1Mb     / 64  banks
			6- 2Mb     / 128 Banks
			52-1.1Mb   / 72  banks
			53-1.2 Mb  / 80  banks
			54-1.5 Mb  / 96  banks 
		RAM Size: 
			0- None
			1- 2   kb / 1 bank
			2- 8   kb / 1 bank
			3- 32  kb / 4 banks
			4- 128 kb / 16 banks

	

	*/

// loads cart and is used by MBC to give stuff to mmu
private:
	uint8_t Cart_Type;
	uint8_t rom_Size;
	uint8_t ram_Size;
	uint8_t *cart_rom;
	char* path;
	char* title;
	MBC *BankControl;
	MMU *mmu;
public:

	Cartridge(MMU* mem);
	bool load(char* path);
	MBC *InitMBC(Cartridge* icart);
	inline uint8_t getROM(){return cart_rom;}
	inline uint8_t getROM_Size(){return rom_size;}
	inline uint8_t getRAM_Size(){return ram_Size;}
	//inline void getCart_Type(){return Cart_Type;}

	// void swapROMBank(uint8_t bank);
	// void swapRAMBank(uint8_t bank);





}




#endif