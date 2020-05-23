#include "CPU.hpp"
void CPU::set_flag_zero(bool set){
	af &= 0xFF7F;
	af |= (set << 7);
}
void CPU::set_flag_subtract(bool set){
	af &= 0xFFBF;
	af |= (set << 6);
}
void CPU::set_flag_hcarry(bool set){
	af &= 0xFFCF;
	af |= (set << 5);
}
void CPU::set_flag_carry(bool set){
	af &= 0xFFEF;
	af |= (set << 4);
}
bool CPU::get_flag_carry(){
	return ((af >> 4) & 0x1);
}
void CPU::UbAdd(uint16_t & x,const uint8_t n,const bool c){
	uint8_t ub = (x >> 8);

	bool carry = c && get_flag_carry();
	if(n != 1){
		set_flag_carry((uint16_t)(ub + n + (int)carry)> 0xFF);
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
void CPU::UbSub(uint16_t & x,const uint8_t n,const bool c){
	uint8_t ub = (x >> 8);
	bool carry = c && get_flag_carry(); 
	if(n != 1){
		set_flag_carry(ub < (carry + n));
		set_flag_hcarry((n & 0x0F + (int)carry) > ub & 0x0F );
	}else{
		set_flag_hcarry((n & 0x0F + (int)carry) > ub & 0x0F );
	}
	ub -= n + (int)carry;
	x = (0x00FF & x) + (ub << 8);
	
	if (x == 0){
		set_flag_zero(1);

	}else{set_flag_zero(0);}
	set_flag_subtract(1);
}


void CPU::LbSub(uint16_t & x,const uint8_t n,const bool c){
	uint8_t lb = 0x00FF & x;
	bool carry = c && get_flag_carry();
	if(n != 1){
		set_flag_carry(lb < n + carry);
		set_flag_hcarry((lb & 0xf)<(n&0xf + carry));
	}else{
		set_flag_hcarry((lb & 0xf)<(n&0xf + carry));
	}
	lb -= n + (int)carry;
	lb &= 0x00FF;
	x = (0xFF00 & x) | lb;
	if (x == 0){
		set_flag_zero(1);

	}else{set_flag_zero(0);}
	set_flag_subtract(1);
}
void CPU::LbAdd(uint16_t &x,const uint8_t n,const bool c){
	uint8_t lb = 0x00FF & x;
	bool carry = c && get_flag_carry();
	if(n != 1){
		set_flag_carry((uint16_t)(lb + n + (int)carry)> 0xFF);
		set_flag_hcarry((lb & 0x0F + n & 0x0F + (int)carry) > 0x0F);
	}else{
		set_flag_hcarry((lb & 0x0F + n & 0x0F + (int)carry) > 0x0F);
	}
	lb += n + (int)carry;
	lb &= 0x00FF;
	x = (0xFF00 & x) | lb;
	if (x == 0){
		set_flag_zero(1);

	}else{set_flag_zero(0);}
	set_flag_subtract(0);
}
void CPU::AddtoHL(const uint16_t &reg){
	if(hl > (0xFFFF - reg)){ 
		set_flag_carry(1);
	}else{set_flag_carry(0);}
	if((hl & 0xfff) + (reg & 0xfff) > 0xfff){
		set_flag_hcarry(1);
	}else{set_flag_hcarry(0;)}
	set_flag_subtract(0);
	hl += reg;

}

