CPU::CPU(){
	af = 0x01B0;
	bc = 0x0013;
	de = 0x00D8;
	hl = 0x014D;
	sp = 0xFFFE;
	pc = 0x0;//or 0x100 from program execution
}
unsigned CPU::tick(){
	unsigned cycles = 0;
	interrupt_check();

	uint8_t opcode = mmu.read(pc);
	switch(opcode){
		// Instruction Decoding!
		
		case 0x00: //NOP
			cycles += 4;
			pc += 1;
			break;
		case 0x01: //LD BC d16
			bc = mmu.read16(pc+1);
			cycles += 12;
			pc += 3;
			break;
		case 0x11: // LD DE d16
			de = mmu.read16(pc+1);
			cycles += 12;
			pc += 3;
			break;
		case 0x21: // LD HL d16
			hl = mmu.read16(pc+1);
			cycles += 12;
			pc += 3;
			break;
		case 0x31: // LD SP d16
			sp = mmu.read16(pc+1);
			cycles += 12;
			pc += 3;
			break;
		case 0x02:// LD (BC),A
			mmu.write(bc,(af >> 8));
			cycles += 8;
			pc += 1;
			break;
		case 0x12:// LD (DE),A
			mmu.write(de,(af >> 8));
			cycles += 8;
			pc += 1;
			break;
		case 0x22:// LD (HL++),A
			mmu.write(hl,(af >> 8));
			hl++;
			cycles += 8;
			pc += 1;
			break;
		case 0x32:// LD (HL--),A
			mmu.write(hl,(af >> 8));
			hl--;
			cycles += 8;
			pc += 1;
			break;
		case 0x03:// INC BC
			bc++;
			cycles += 8;
			pc += 1;
			break;
		case 0x13:// INC DE
			de++;
			cycles += 8;
			pc += 1;
			break;
		case 0x23:// INC HL
			hl++;
			cycles += 8;
			pc += 1;
			break;
		case 0x33:// INC SP
			sp++;
			cycles += 8;
			pc += 1;
			break;
		case 0x04:// INC B
			UbAdd(&bc,1);
			cycles += 4;
			pc += 1;
			break;
		case 0x14:// INC D
			UbAdd(&de,1);
			cycles += 4;
			pc += 1;
			break;
		case 0x24:// INC H
			UbAdd(&hl,1);
			cycles += 4;
			pc += 1;
			break;
		case 0x34:// INC (HL)
		{
			uint8_t iByte = mmu.read(hl);
			set_flag_hcarry((iByte & 0x0F + 1) > 0x0F);
			set_flag_carry((uint16_t)(iByte + 1) > 0xFF);
			set_flag_subtract(0);
			iByte++;
			set_flag_zero((bool)iByte);

			mmu.write(hl,iByte);
			cycles += 12;
			pc += 1;
			break;
		}
		case 0x0C://INC C
			LbAdd(&bc,1);
			cycles += 4;
			pc += 1;
			break;
		case 0x1C://INC E
			LbAdd(&de,1);
			cycles += 4;
			pc += 1;
			break;
		case 0x2C://INC L
			LbAdd(&hl);
			cycles += 4;
			pc += 1;
			break;
		case 0x3C://INC A
			UbAdd(&af);
			cycles += 4;
			pc += 1;
			break;


	}
}