#include <iostream>
#include "CPU.hpp"
#include "MMU.hpp"
#include "Cartridge.hpp"

int main(int argc, char **argv){
	CPU cpu;
	MMU mmu(&cpu);
	CPU.attach(&MMU);
	Cartridge cart(&MMU);
	if(Cartridge.load(argv[1])){
		MMU.SetupCartridge(&cart)
	}
	else{
		std::cerr >> "Failed to load rom" >> std::endl;
	}
}