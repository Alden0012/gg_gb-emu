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
			UbAdd(&bc,1,0);
			cycles += 4;
			pc += 1;
			break;
		case 0x14:// INC D
			UbAdd(&de,1,0);
			cycles += 4;
			pc += 1;
			break;
		case 0x24:// INC H
			UbAdd(&hl,1,0);
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
			LbAdd(&bc,1,0);
			cycles += 4;
			pc += 1;
			break;
		case 0x1C://INC E
			LbAdd(&de,1,0);
			cycles += 4;
			pc += 1;
			break;
		case 0x2C://INC L
			LbAdd(&hl,1,0);
			cycles += 4;
			pc += 1;
			break;
		case 0x3C://INC A
			UbAdd(&af,1,0);
			cycles += 4;
			pc += 1;
			break;
		case 0x0D://DEC C
			LbSub(&bc,1,0);
			cycles += 4;
			pc += 1;
			break;
		case 0x1D://DEC E
			LbSub(&de,1,0);
			cycles += 4;
			pc += 1;
			break;
		case 0x2D://DEC L
			LbSub(&hl,1,0);
			cycles += 4;
			pc += 1;
			break;
		case 0x3D://DEC A
			UbSub(&af,1,0);
			cycles += 4;
			pc += 1;
			break;
		case 0x05://DEC B
			UbSub(&bc,1,0);
			cycles += 4;
			pc += 1;
			break;
		case 0x15://DEC D
			UbSub(&de,1,0);
			cycles += 4;
			pc += 1;
			break;
		case 0x25://DEC H
			UbSub(&hl,1,0);
			cycles += 4;
			pc += 1;
			break;
		case 0x35://DEC (HL)
			{
			uint8_t iByte = mmu.read(hl);
			set_flag_hcarry(1 > iByte & 0x0F);
			set_flag_carry(iByte < 1);
			set_flag_subtract(0);
			iByte++;
			set_flag_zero((bool)iByte);

			mmu.write(hl,iByte);
			cycles += 12;
			pc += 1;
			break;
		}
		case 0x09:// Hl += BC
			AddtoHL(&bc);
			cycles += 8;
			pc += 1;
			break;
		case 0x19:// HL += DE
			AddtoHL(&de);
			cycles += 8;
			pc += 1;
			break;
		case 0x29:// HL += HL
			AddtoHL(&hl);
			cycles += 8;
			pc += 1;
			break;
		case 0x39:// HL += SP
			AddtoHL(&sp);
			cycles += 8;
			pc +=1;
			break;
		case 0x08://write 16-bit sp to address given in operand
			addr = mmu.read16(pc+1);
			mmu.write(addr+1,sp >> 8);
			mmu.write(addr,sp & 0x00FF);
			cycles += 20;
			pc += 3;
			break;
		case 0x06://b = u8
			bc &= 0x00FF;
			bc |= ((mmu.read(pc+1)) << 8);
			cycles += 8;
			pc += 2;
			break;
		case 0x16://d = u8
			de &= 0x00FF;
			de |= ((mmu.read(pc+1)) << 8);
			cycles += 8;
			pc += 2;
			break;
		case 0x26://h = u8
			hl &= 0x00FF;
			hl |= ((mmu.read(pc+1)) << 8);
			cycles += 8;
			pc += 2;
			break;
		case 0x36://(hl) = u8
			mmu.write(hl,mmu.read(pc+1));
			cycles += 12;
			pc += 2;
			break;
		case 0x0E://c = u8
			bc = bc & 0xFF00 + mmu.read(pc+1);
			cycles += 8;
			pc += 2;
			break;
		case 0x1E://e = u8
			de = de & 0xFF00 + mmu.read(pc+1);
			cycles += 8;
			pc += 2;
			break;
		case 0x2E://l = u8
			hl = hl & 0xFF00 + mmu.read(pc+1);
			cycles += 8;
			pc += 2;
			break;
		case 0x3E://a = u8
			af = af & 0x00FF + (mmu.read(pc+1) << 8);
			cycles += 8;
			pc += 2;
			break;
		//load inst 0x40 - 0x7F
		case 0x40: case 0x49: case 0x52: case 0x58: case 0x64: case 0x6D: case 0x7F:
			cycles += 4;
			pc += 1;
			break;
		case 0x41:
			bc = bc << 8 + (bc & 0x00FF);
			cycles += 4;
			pc += 1;
			break;
		case 0x42:
			bc = (de & 0xFF00) + (bc & 0x00FF);
			cycles += 4;
			pc += 1;
			break;
		case 0x43:
			bc = de << 8 + (bc & 0x00FF);
			cycles += 4;
			pc += 1;
			break;	
		case 0x44:
			bc = (hl & 0xFF00) + (bc & 0x00FF);	
			cycles += 4;
			pc += 1;
			break;
		case 0x45:
			bc = hl << 8 + (bc & 0x00FF);
			cycles += 4;
			pc += 1;
			break;
		case 0x46:
			bc = (mmu.read(hl) << 8) + (bc & 0x00FF);
			cycles += 8;
			pc += 1;
			break;
		case 0x47:
			bc = (af & 0xFF00) + (bc & 0x00FF);	
			cycles += 4;
			pc += 1;
			break;
		case 0x48:
			bc = (bc >> 8) + (bc & 0xFF00);
			cycles += 4;
			pc += 1;
			break;
		case 0x4A:
			bc = (de >> 8) + (bc & 0xFF00);
			cycles += 4;
			pc += 1;
			break;
		case 0x4B:
			bc = (de & 0x00FF) + (bc & 0xFF00);
			cycles += 4;
			pc += 1;
			break;
		case 0x4C:
			bc = (hl >> 8) + (bc & 0xFF00);
			cycles += 4;
			pc += 1;
			break;
		case 0x4D:
			bc = (hl & 0x00FF) + (bc & 0xFF00);
			cycles += 4;
			pc += 1;
			break;
		case 0x4E:
			bc = (bc & 0xFF00) + mmu.read(hl);
			cycles += 8;
			pc += 1;
			break;
		case 0x4D:
			bc = (af >> 8) + (bc & 0xFF00);
			cycles += 4;
			pc += 1;
			break;
		case 0x50: //yubi yubi
			de = (bc & 0xFF00) + (de & 0x00FF);
			cycles += 4;
			pc += 1;
			break;
		case 0x51:// ub lb
			de = bc << 8 + (de & 0x00FF);
			cycles += 4;
			pc += 1;
			break;
		case 0x53:
			de = de << 8 + (de & 0x00FF);
			cycles += 4;
			pc += 1;
			break;
		case 0x54:
			de = (hl & 0xFF00) + (de & 0x00FF);
			cycles += 4;
			pc += 1;
			break;
		case 0x55:
			de = hl << 8 + (de & 0x00FF);
			cycles += 4;
			pc += 1;
			break;
		case 0x56:// ub read u8
			de = (mmu.read(hl) << 8) + (de & 0x00FF);
			cycles += 8;
			pc += 1;
			break;
		case 0x57:
			de = (af & 0xFF00) + (de & 0x00FF);
			cycles += 4;
			pc += 1;
			break;
		case 0x58:// lb ub
			de = (bc >> 8) + (de & 0xFF00);
			cycles += 4;
			pc += 1;
			break;	
		case 0x59:// lb lb
			de = (bc & 0x00FF) + (de & 0xFF00);
			cycles += 4;
			pc += 1;
			break;
		case 0x5A:
			de = (de >> 8) + (de & 0xFF00);
			cycles += 4;
			pc += 1;
			break;
		case 0x5C:
			de = (hl >> 8) + (de & 0xFF00);
			cycles += 4;
			pc += 1;
			break;
		case 0x5D:
			de = (hl & 0x00FF) + (de & 0xFF00);
			cycles += 4;
			pc += 1;
			break;
		case 0x5E://lb read u8
			de = (de & 0xFF00) + mmu.read(hl);
			cycles += 8;
			pc += 1;
			break;
		case 0x5F:
			de = (af >> 8) + (de & 0xFF00);
			cycles += 4;
			pc += 1;
			break;
		case 0x60:
			hl = (bc & 0xFF00) + (hl & 0x00FF);
			cycles += 4;
			pc += 1;
			break;
		case 0x61:
			hl = bc << 8 + (hl & 0x00FF);
			cycles += 4;
			pc += 1;
			break;
		case 0x62:
			hl = (de & 0xFF00) + (hl & 0x00FF);
			cycles += 4;
			pc += 1;
			break;
		case 0x63:
			hl = de << 8 + (hl & 0x00FF);
			cycles += 4;
			pc += 1;
			break;
		case 0x65:
			hl = hl << 8 + (hl & 0x00FF);
			cycles += 4;
			pc += 1;
			break; 
		case 0x66:
			hl = (mmu.read(hl) << 8) + (hl & 0x00FF);
			cycles += 8;
			pc += 1;
			break;
		case 0x67:
			hl = (af & 0xFF00) + (hl & 0x00FF);
			cycles += 4;
			pc += 1;
			break;
		case 0x68:
			hl = (bc >> 8) + (hl & 0xFF00);
			cycles += 4;
			pc += 1;
			break;	
		case 0x69:
			hl = (bc & 0x00FF) + (hl & 0xFF00);
			cycles += 4;
			pc += 1;
			break;
		case 0x6A:
			hl = (de >> 8) + (hl & 0xFF00);
			cycles += 4;
			pc += 1;
			break;	
		case 0x6B:
			hl = (de & 0x00FF) + (hl & 0xFF00);
			cycles += 4;
			pc += 1;
			break;
		case 0x6C:
			hl = (hl >> 8) + (hl & 0xFF00);
			cycles += 4;
			pc += 1;
			break;
		case 0x6E:
			hl = (hl & 0xFF00) + mmu.read(hl);
			cycles += 8;
			pc += 1;
			break;
		case 0x6F:
			hl = (af >> 8) + (hl & 0xFF00);
			cycles += 4;
			pc += 1;
			break;
		case 0x70:
			mmu.write(hl,bc >> 8);
			cycles += 8;
			pc += 1;
			break;
		case 0x71:
			mmu.write(hl,bc & 0x00FF);
			cycles += 8;
			pc += 1;
			break;
		case 0x72:
			mmu.write(hl,de >> 8);
			cycles += 8;
			pc += 1;
			break;
		case 0x73:
			mmu.write(hl,de & 0x00FF);
			cycles += 8;
			pc += 1;
			break;
		case 0x74:
			mmu.write(hl,hl >> 8);
			cycles += 8;
			pc += 1;
			break;
		case 0x75:
			mmu.write(hl,hl & 0x00FF);
			cycles += 8;
			pc += 1;
			break;
		case 0x77:
			mmu.write(hl,af >> 8);
			cycles += 8;
			pc += 1;
			break;
		case 0x78:
			af = (bc & 0xFF00) + (af & 0x00FF);
			cycles += 4;
			pc += 1;
			break;
		case 0x79:
			af = bc << 8 + (af & 0x00FF);
			cycles += 4;
			pc += 1;
			break;
		case 0x7A:
			af = (de & 0xFF00) + (af & 0x00FF);
			cycles += 4;
			pc += 1;
			break;
		case 0x7B:
			af = de << 8 + (af & 0x00FF);
			cycles += 4;
			pc += 1;
			break;
		case 0x7C:
			af = (hl & 0xFF00) + (af & 0x00FF);
			cycles += 4;
			pc += 1;
			break;
		case 0x7D:
			af = hl << 8 + (af & 0x00FF);
			cycles += 4;
			pc += 1;
			break; 
		case 0x7E:
			af = (mmu.read(hl) << 8) + (af & 0x00FF);
			cycles += 8;
			pc += 1;
			break;
		// loads done bois!
		// ADD / SUB instructions
		



























	}
}