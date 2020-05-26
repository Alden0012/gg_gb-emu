#include <iostream>
#include "CPU.hpp"
#include "MMU.hpp"
#include "Cartridge.hpp"

int main(int argc, char **argv){
	CPU cpu;
	MMU mmu(&cpu);
	CPU.attach(&MMU);
	//PERFORM BOOTROM STUFF MAYBE MAKE A FUNCTION FOR CPU WHICH SAVES CART 0x00-0xFF AND LOADS IN THE BOOT ROM INSTEAD THEN AFTER ITS DONE LOAD IN THE 
	//CARTRIDGE STUFF. PEPSI FOR PIZZA.
	Cartridge cart(&MMU);
	if(Cartridge.load(argv[1])){
		MMU.SetupCartridge(&cart)
	}
	else{
		std::cerr << "Failed to load rom" << std::endl;
	}
}