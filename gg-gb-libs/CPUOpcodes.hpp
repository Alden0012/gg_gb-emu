#ifndef CPUOpcodes_HPP
#define CPUOpcodes_HPP

// so i could either make a function pointer list and have a layout of function pointers or have a switch case mess hmm..
// WHY ARE THERE 256 OPCODES

void set_flag_zero(bool set){
	af &= 0xFF7F;
	af |= (set << 7);
}
void set_flag_subtract(bool set){
	af &= 0xFFBF;
	af |= (set << 6);
}
void set_flag_hcarry(bool set){
	af &= 0xFFCF;
	af |= (set << 5);
}
void set_flag_carry(bool set){
	af &= 0xFFEF;
	af |= (set << 4);
}
void CPU::UbAdd(uint16_t *reg x,uint8_t n,bool carry){
	uint8_t ub = (x >> 8);
	
	if(n != 1){
		set_flag_carry(ub > 0xFF);
		set_flag_hcarry((ub & 0x0F + n & 0x0F + (int)carry) > 0x0F);
	}else{
		set_flag_hcarry((ub & 0x0F + n & 0x0F + (int)carry)> 0x0F);
	}
	ub += n + (int)carry;
	x = (0x00FF & x) + (ub << 8);
	
	if (x == 0){
		set_flag_zero(1);

	}else{set_flag_zero(0);}
	set_flag_subtract(0);
}
void CPU::LbAdd(uint16_t *reg x,uint8_t n,bool carry){
	lb = 0x00FF & x;
	if(n != 1){
		set_flag_carry(lb > 0xFF);
		set_flag_hcarry((lb & 0x0F + n & 0x0F + (int)carry) > 0x0F);
	}else{
		set_flag_hcarry((lb & 0x0F + n & 0x0F + (int)carry) > 0x0F);
	}
	lb += n + (int)carry;
	lb &= 0x00FF;
	x = (0xFF00 & x) | lb;
}




#endif