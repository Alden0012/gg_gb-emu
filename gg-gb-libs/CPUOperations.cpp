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
bool CPU::get_flag_zero(){
	return ((af >> 7)& 0x1);
}
bool CPU::get_flag_subtract(){
	return((af>>6)&0x1);
}
bool CPU::get_flag_hcarry(){
	return((af>>5)&0x1);
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
	
	if (ub == 0){
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
	
	if (ub == 0){
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
	if (lb == 0){
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
	if (lb == 0){
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
void CPU::ANDa(const uint8_t byte){
	af &= (byte << 8);
	set_flag_zero(af >> 8 == 0);
	set_flag_hcarry(1);
	set_flag_carry(0);
	set_flag_subtract(0);
}
void CPU::ORa(const uint8_t byte){
	af |= (byte << 8);
	set_flag_zero(af >> 8 == 0);
	set_flag_hcarry(0);
	set_flag_carry(0);
	set_flag_subtract(0);
}
void CPU::XORa(const uint8_t byte){
	af ^= (byte << 8);
	set_flag_zero(af >> 8 == 0);
	set_flag_hcarry(0);
	set_flag_carry(0);
	set_flag_subtract(0);
}
void CPU::CPa(const uint8_t n){
	uint8_t ub = (af >> 8);
	bool carry = c && get_flag_carry(); 
	if(n != 1){
		set_flag_carry(ub < (carry + n));
		set_flag_hcarry((n & 0x0F + (int)carry) > ub & 0x0F );
	}else{
		set_flag_hcarry((n & 0x0F + (int)carry) > ub & 0x0F );
	}
	ub -= n + (int)carry;
		
	if (ub == 0){
		set_flag_zero(1);

	}else{set_flag_zero(0);}
	set_flag_subtract(1);
}
void CPU::rst(const uint16_t addr){
	sp--;
	mmu->write(sp,pc>>8);
	sp--;
	mmu->write(sp,pc & 0x00FF);
	pc = addr;
}
void CPU::push(const uint16 &reg){
	sp--;
	mmu->write(sp,reg>>8);
	sp--;
	mmu->write(sp,reg & 0x00FF);
}
void CPU::pop(uint16 &reg){
	uint8_t lowbyte = mmu->read(sp);
	sp++;
	uint8_t highbyte = mmu->read(sp);
	sp++;
	reg = (highbyte << 8) + lowbyte;
}
void CPU::ret(){
	pop(pc);
}
void CPU::call(){
	addr = (mmu->read(pc+2) << 8) + (mmu->read(pc+1));
	pc += 3;
	push(&pc);
	pc = addr;
}
uint8_t CPU::RL(uint8_t n,bool C){
	uint8_t byte;
	bool carrybit = get_flag_carry();
	if (C){
		byte = (n << 1) + n >> 7;
	}
	else{
		byte = (n << 1) + carrybit;
	}
	set_flag_zero(byte == 0);
	set_flag_carry(n >> 7);
	set_flag_subtract(0);
	set_flag_hcarry(0);
	return byte;
}
uint8_t CPU::RR(uint8_t n,bool C){
	uint8_t byte;
	bool carrybit = get_flag_carry;
	if(C){
		byte = (n >> 1) + ((n & 0x01) << 7);
	}
	else{
		byte = (n >> 1) + (carrybit << 7);
	}
	set_flag_zero(byte == 0);
	set_flag_carry(n & 0x1);
	set_flag_hcarry(0);
	set_flag_subtract(0);
	return byte;
}
void CPU::PlaceVal(uint16_t &n,uint8_t newb,bool bit){
	if(bit){
		n = (newb << 8) + (n & 0x0F);
	}else{
		n = newb + (n & 0xF0);
	}
	return;
}
void CPU::BIT(uint8_t bit,uint8_t reg){
	if(!((reg >> bit) & 0x1)){
		set_flag_zero(1);
	}
	set_flag_hcarry(1);

}
uint8_t CPU::RES(uint8_t bit,uint8_t reg){
	return ~((uint8_t)(0x1<<bit)) & reg;
}
uint8_t CPU::SET(uint8_t bit,uint8_t reg){
	return (0x1 << bit) | reg;
}
