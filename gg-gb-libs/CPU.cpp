CPU::CPU(){
	af = 0x01B0;
	bc = 0x0013;
	de = 0x00D8;
	hl = 0x014D;
	sp = 0xFFFE;
	pc = 0x0;//or 0x100 from program execution
}
unsigned CPU::tick(){
	
	interrupt_check();

	uint8_t opcode = mmu->read(pc);
	ExecuteOpcode(opcode);
}


unsigned CPU::ExecuteOpcode(uint8_t opcode){
		unsigned cycles = 0;
		switch(opcode){
		// Instruction Decoding!
		
		case 0x00: //NOP
			cycles += 4;
			pc += 1;
			break;
		case 0x01: //LD BC d16
			bc = mmu->read16(pc+1);
			cycles += 12;
			pc += 3;
			break;
		case 0x11: // LD DE d16
			de = mmu->read16(pc+1);
			cycles += 12;
			pc += 3;
			break;
		case 0x21: // LD HL d16
			hl = mmu->read16(pc+1);
			cycles += 12;
			pc += 3;
			break;
		case 0x31: // LD SP d16
			sp = mmu->read16(pc+1);
			cycles += 12;
			pc += 3;
			break;
		case 0x02:// LD (BC),A
			mmu->write(bc,(af >> 8));
			cycles += 8;
			pc += 1;
			break;
		case 0x12:// LD (DE),A
			mmu->write(de,(af >> 8));
			cycles += 8;
			pc += 1;
			break;
		case 0x22:// LD (HL++),A
			mmu->write(hl,(af >> 8));
			hl++;
			cycles += 8;
			pc += 1;
			break;
		case 0x32:// LD (HL--),A
			mmu->write(hl,(af >> 8));
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
			uint8_t iByte = mmu->read(hl);
			set_flag_hcarry((iByte & 0x0F + 1) > 0x0F);
			set_flag_carry((uint16_t)(iByte + 1) > 0xFF);
			set_flag_subtract(0);
			iByte++;
			set_flag_zero((bool)iByte);

			mmu->write(hl,iByte);
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
			uint8_t iByte = mmu->read(hl);
			set_flag_hcarry(1 > iByte & 0x0F);
			set_flag_carry(iByte < 1);
			set_flag_subtract(0);
			iByte++;
			set_flag_zero((bool)iByte);

			mmu->write(hl,iByte);
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
			addr = mmu->read16(pc+1);
			mmu->write(addr+1,sp >> 8);
			mmu->write(addr,sp & 0x00FF);
			cycles += 20;
			pc += 3;
			break;
		case 0x06://b = u8
			bc &= 0x00FF;
			bc |= ((mmu->read(pc+1)) << 8);
			cycles += 8;
			pc += 2;
			break;
		case 0x16://d = u8
			de &= 0x00FF;
			de |= ((mmu->read(pc+1)) << 8);
			cycles += 8;
			pc += 2;
			break;
		case 0x26://h = u8
			hl &= 0x00FF;
			hl |= ((mmu->read(pc+1)) << 8);
			cycles += 8;
			pc += 2;
			break;
		case 0x36://(hl) = u8
			mmu->write(hl,mmu->read(pc+1));
			cycles += 12;
			pc += 2;
			break;
		case 0x0E://c = u8
			bc = bc & 0xFF00 + mmu->read(pc+1);
			cycles += 8;
			pc += 2;
			break;
		case 0x1E://e = u8
			de = de & 0xFF00 + mmu->read(pc+1);
			cycles += 8;
			pc += 2;
			break;
		case 0x2E://l = u8
			hl = hl & 0xFF00 + mmu->read(pc+1);
			cycles += 8;
			pc += 2;
			break;
		case 0x3E://a = u8
			af = af & 0x00FF + (mmu->read(pc+1) << 8);
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
			bc = (mmu->read(hl) << 8) + (bc & 0x00FF);
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
			bc = (bc & 0xFF00) + mmu->read(hl);
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
			de = (mmu->read(hl) << 8) + (de & 0x00FF);
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
			de = (de & 0xFF00) + mmu->read(hl);
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
			hl = (mmu->read(hl) << 8) + (hl & 0x00FF);
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
			hl = (hl & 0xFF00) + mmu->read(hl);
			cycles += 8;
			pc += 1;
			break;
		case 0x6F:
			hl = (af >> 8) + (hl & 0xFF00);
			cycles += 4;
			pc += 1;
			break;
		case 0x70:
			mmu->write(hl,bc >> 8);
			cycles += 8;
			pc += 1;
			break;
		case 0x71:
			mmu->write(hl,bc & 0x00FF);
			cycles += 8;
			pc += 1;
			break;
		case 0x72:
			mmu->write(hl,de >> 8);
			cycles += 8;
			pc += 1;
			break;
		case 0x73:
			mmu->write(hl,de & 0x00FF);
			cycles += 8;
			pc += 1;
			break;
		case 0x74:
			mmu->write(hl,hl >> 8);
			cycles += 8;
			pc += 1;
			break;
		case 0x75:
			mmu->write(hl,hl & 0x00FF);
			cycles += 8;
			pc += 1;
			break;
		case 0x77:
			mmu->write(hl,af >> 8);
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
			af = (mmu->read(hl) << 8) + (af & 0x00FF);
			cycles += 8;
			pc += 1;
			break;
		case 0xE0:
			mmu->write((0xFF00+mmu->read(pc+1)),af >> 8);
			cycles += 12;
			pc += 2;
			break;
		case 0xF0:
			af = (mmu->read((0xFF00+mmu->read(pc+1))) << 8) + (af & 0x00FF);
			cycles += 12;
			pc += 2;
			break;
		case 0xE2:
			mmu->write((0xFF00+(bc&0x00FF)),af >> 8);
			cycles += 8;
			pc += 2;
			break;
		case 0xF2:
			af = (mmu->read((0xFF00+(bc&0x00FF))) << 8) + (af & 0x00FF);
			cycles += 8;
			pc += 2;
			break;
		case 0xEA:
			mmu->write((mmu->read(pc+2)+mmu->read(pc+1)),af >> 8);
			cycles += 16;
			pc += 3;
			break;
		case 0xFA:
			af = (mmu->read((mmu->read(pc+2)+mmu->read(pc+1))) << 8) + (af & 0x00FF);
			cycles += 16;
			pc += 3;
			break;
		// loads done bois!
		// ADD / SUB instructions
		case 0x80:
			UbAdd(&af,bc >> 8,0);
			cycles += 4;
			pc += 1;
			break;
		case 0x81:
			UbAdd(&af,bc & 0x00FF,0);
			cycles += 4;
			pc += 1;
			break;
		case 0x82:
			UbAdd(&af,de >> 8,0);
			cycles += 4;
			pc += 1;
			break;
		case 0x83:
			UbAdd(&af,de & 0x00FF,0);
			cycles += 4;
			pc += 1;
			break;
		case 0x84:
			UbAdd(&af,hl >> 8,0);
			cycles += 4;
			pc += 1;
			break;
		case 0x85:
			UbAdd(&af,hl & 0x00FF,0);
			cycles += 4;
			pc += 1;
			break;
		case 0x86:
			UbAdd(&af,mmu->read(hl),0);
			cycles += 8;
			pc += 1;
			break;
		case 0x87:
			UbAdd(&af,af >> 8,0);
			cycles += 4;
			pc += 1;
			break;
		case 0x88:
			UbAdd(&af,bc >> 8,1);
			cycles += 4;
			pc += 1;
			break;
		case 0x89:
			UbAdd(&af,bc & 0x00FF,1);
			cycles += 4;
			pc += 1;
			break;
		case 0x8A:
			UbAdd(&af,de >> 8,1);
			cycles += 4;
			pc += 1;
			break;
		case 0x8B:
			UbAdd(&af,de & 0x00FF,1);
			cycles += 4;
			pc += 1;
			break;
		case 0x8C:
			UbAdd(&af,hl >> 8,1);
			cycles += 4;
			pc += 1;
			break;
		case 0x8D:
			UbAdd(&af,hl & 0x00FF,1);
			cycles += 4;
			pc += 1;
			break;
		case 0x8E:
			UbAdd(&af,mmu->read(hl),1);
			cycles += 8;
			pc += 1;
			break;
		case 0x8F:
			UbAdd(&af,af >> 8,1);
			cycles += 4;
			pc += 1;
			break;
		case 0x90:
			UbSub(&af,bc >> 8,0);
			cycles += 4;
			pc += 1;
			break;
		case 0x91:
			UbSub(&af,bc & 0x00FF,0);
			cycles += 4;
			pc += 1;
			break;
		case 0x92:
			UbSub(&af,de >> 8,0);
			cycles += 4;
			pc += 1;
			break;
		case 0x93:
			UbSub(&af,de & 0x00FF,0);
			cycles += 4;
			pc += 1;
			break;
		case 0x94:
			UbSub(&af,hl >> 8,0);
			cycles += 4;
			pc += 1;
			break;
		case 0x95:
			UbSub(&af,hl & 0x00FF,0);
			cycles += 4;
			pc += 1;
			break;
		case 0x96:
			UbSub(&af,mmu->read(hl),0);
			cycles += 8;
			pc += 1;
			break;
		case 0x97:
			UbSub(&af,af >> 8,0);
			cycles += 4;
			pc += 1;
			break;
		case 0x98:
			UbSub(&af,bc >> 8,1);
			cycles += 4;
			pc += 1;
			break;
		case 0x99:
			UbSub(&af,bc & 0x00FF,1);
			cycles += 4;
			pc += 1;
			break;
		case 0x9A:
			UbSub(&af,de >> 8,1);
			cycles += 4;
			pc += 1;
			break;
		case 0x9B:
			UbSub(&af,de & 0x00FF,1);
			cycles += 4;
			pc += 1;
			break;
		case 0x9C:
			UbSub(&af,hl >> 8,1);
			cycles += 4;
			pc += 1;
			break;
		case 0x9D:
			UbSub(&af,hl & 0x00FF,1);
			cycles += 4;
			pc += 1;
			break;
		case 0x9E:
			UbSub(&af,mmu->read(hl),1);
			cycles += 8;
			pc += 1;
			break;
		case 0x9F:
			UbSub(&af,af >> 8,1);
			cycles += 4;
			pc += 1;
			break;
		case 0xA0:
			ANDa(bc >> 8);
			cycles += 4;
			pc += 1;
			break;
		case 0xA1:
			ANDa(bc & 0x00FF);
			cycles += 4;
			pc += 1;
			break;
		case 0xA2:
			ANDa(de >> 8);
			cycles += 4;
			pc += 1;
			break;
		case 0xA3:
			ANDa(de & 0x00FF);
			cycles += 4;
			pc += 1;
			break;
		case 0xA4:
			ANDa(hl >> 8);
			cycles += 4;
			pc += 1;
			break;
		case 0xA5:
			ANDa(hl & 0x00FF);
			cycles += 4;
			pc += 1;
			break;
		case 0xA6:
			ANDa(mmu->read(hl));
			cycles += 8;
			pc += 1;
			break;
		case 0xA7:
			ANDa(af >> 8);
			cycles += 4;
			pc += 1;
			break;
		case 0xA8:
			XORa(bc >> 8);
			cycles += 4;
			pc += 1;
			break;
		case 0xA9:
			XORa(bc & 0x00FF);
			cycles += 4;
			pc += 1;
			break;
		case 0xAA:
			XORa(de >> 8);
			cycles += 4;
			pc += 1;
			break;
		case 0xAB:
			XORa(de & 0x00FF);
			cycles += 4;
			pc += 1;
			break;
		case 0xAC:
			XORa(hl >> 8);
			cycles += 4;
			pc += 1;
			break;
		case 0xAD:
			XORa(hl & 0x00FF);
			cycles += 4;
			pc += 1;
			break;		
		case 0xAE:
			XORa(mmu->read(hl));
			cycles += 8;
			pc += 1;
			break;	
		case 0xAF:
			XORa(af >> 8);
			cycles += 4;
			pc += 1;
			break;
		case 0xB0:
			ORa(bc >> 8);
			cycles += 4;
			pc += 1;
			break;
		case 0xB1:
			ORa(bc & 0x00FF);
			cycles += 4;
			pc += 1;
			break;
		case 0xB2:
			ORa(de >> 8);
			cycles += 4;
			pc += 1;
			break;
		case 0xB3:
			ORa(de & 0x00FF);
			cycles += 4;
			pc += 1;
			break;
		case 0xB4:
			ORa(hl >> 8);
			cycles += 4;
			pc += 1;
			break;
		case 0xB5:
			ORa(hl & 0x00FF);
			cycles += 4;
			pc += 1;
			break;
		case 0xB6:
			ORa(mmu->read(hl));
			cycles += 8;
			pc += 1;
			break;
		case 0xB7:
			ORa(af >> 8);
			cycles += 4;
			pc += 1;
			break;
		case 0xB8:
			CPa(bc >> 8);
			cycles += 4;
			pc += 1;
			break;
		case 0xB9:
			CPa(bc & 0x00FF);
			cycles += 4;
			pc += 1;
			break;
		case 0xBA:
			CPa(de >> 8);
			cycles += 4;
			pc += 1;
			break;
		case 0xBB:
			CPa(de & 0x00FF);
			cycles += 4;
			pc += 1;
			break;
		case 0xB8:
			CPa(hl >> 8);
			cycles += 4;
			pc += 1;
			break;
		case 0xB9:
			CPa(hl & 0x00FF);
			cycles += 4;
			pc += 1;
			break;
		case 0xB8:
			CPa(mmu->read(hl));
			cycles += 8;
			pc += 1;
			break;
		case 0xB9:
			CPa(af >> 8);
			cycles += 4;
			pc += 1;
			break;
		case 0xCE:
			UbAdd(&af,mmu->read(pc+1),1);
			cycles += 8;
			pc += 2;
			break;
		case 0xDE:
			UbSub(&af,mmu->read(pc+1),1);
			cycles += 8;
			pc += 2;
			break;
		case 0xEE:
			XORa(mmu->read(pc+1));
			cycles += 8;
			pc += 2;
			break;
		case 0xFE:
			CPa(mmu->read(pc+1));
			cycles += 8;
			pc += 2;
			break;
		//jumps
		case 0x18:
			pc = mmu->read(pc+1);
			cycles += 12;
			break;
		case 0xC3:
			pc = mmu->read(pc+2) | mmu->read(pc+1);
			cycles += 16;
			break;
		case 0x28:
			if(get_flag_zero()){
				pc = mmu->read(pc+1);
				cycles += 12;
			}else{
				cycles += 8;
				pc += 2;
			}
			break;
		case 0x20:
			if(!get_flag_zero()){
				pc = mmu->read(pc+1);
				cycles += 12;
			}else{
				cycles += 8;
				pc += 2;
			}

			break;
		case 0x38:
			if(get_flag_carry()){
				pc = mmu->read(pc+1);
				cycles += 12;
			}else{
				cycles += 8;
				pc += 2;
			}
			break;
		case 0x30:
			if(!get_flag_carry()){
				pc = mmu->read(pc+1);
				cycles += 12;
			}else{
				cycles += 8;
				pc += 2;
			}
			break;
		case 0xCA:
			if(get_flag_zero())
				pc = mmu->read(pc+2) | mmu->read(pc+1);
				cycles += 16;
			else{
				cycles += 12;
				pc += 3;
			}
			break;
		case 0xDA:
			if(get_flag_carry())
				pc = mmu->read(pc+2) | mmu->read(pc+1);
				cycles += 16;
			else{
				cycles += 12;
				pc += 3;
			}
			break;
		case 0xC2:
			if(!get_flag_zero())
				pc = mmu->read(pc+2) | mmu->read(pc+1);
				cycles += 16;
			else{
				cycles += 12;
				pc += 3;
			}
			break;
		case 0xD2:
			if(!get_flag_carry())
				pc = mmu->read(pc+2) | mmu->read(pc+1);
				cycles += 16;
			else{
				cycles += 12;
			}
			pc += 3;
			break;
		//resets
		case 0xCF:
			rst(0x08);
			cycles += 16;
			pc += 1;
			break;
		case 0xDF:
			rst(0x18);
			cycles += 16;
			pc += 1;
			break;
		case 0xEF:
			rst(0x28);
			cycles += 16;
			pc += 1;
			break;
		case 0xFF:
			rst(0x38);
			cycles += 16;
			pc += 1;
			break;
		case 0xC7:
			rst(0x00);
			cycles += 16;
			pc += 1;
			break;		
		case 0xD7:
			rst(0x10);
			cycles += 16;
			pc += 1;
			break;
		case 0xE7:
			rst(0x20);
			cycles += 16;
			pc += 1;
			break;
		case 0xF7:
			rst(0x30);
			cycles += 16;
			pc += 1;
			break;
		//push pop
		case 0xF9:
			sp = hl;
			cycles += 8;
			pc += 1;
			break;
		case 0xC1:
			pop(&bc);
			cycles += 12;
			pc += 1;
			break;	
		case 0xD1:
			pop(&de);
			cycles += 12;
			pc += 1;
			break;
		case 0xE1:
			pop(&hl);
			cycles += 12;
			pc += 1;
			break;
		case 0xF1:
			pop(&af);
			cycles += 12;
			pc += 1;
			break;
		case 0xC5:
			push(&bc);
			cycles += 16;
			pc += 1;
			break;
		case 0xD5:
			push(&de);
			cycles += 16;
			pc += 1;
			break;
		case 0xE5:
			push(&hl);
			cycles += 16;
			pc += 1;
			break;
		case 0xF5:
			push(&af);
			cycles += 16;
			pc += 1;
			break;
		case 0xC6:
			UbAdd(&af,mmu->read(pc+1),0);
			cycles += 8;
			pc += 2;
			break;
		case 0xD6:
			UbSub(&af,mmu->read(pc+1),0);
			cycles += 8;
			pc += 2;
			break;
		case 0xE6:
			ANDa(mmu->read(pc+1));
			cycles += 8;
			pc += 2;
			break;
		case 0xF6:
			ORa(mmu->read(pc+1));
			cycles += 8;
			pc += 2;
			break;
		case 0xE8:
			set_flag_hcarry((sp&0xF + (mmu->read(pc+1))&0xF)&0x10);
			set_flag_carry((sp&0xFF + (mmu->read(pc+1))&0xFF)&0x100);
			sp += mmu->read(pc+1);
			set_flag_zero(0);
			set_flag_subtract(0);
			pc += 2;
			break;
		case  0xE9:
			pc = hl;
			cycles += 4;
			break;
		//ret
		case 0xC0:
			if(!get_flag_zero()){
				ret();
				cycles += 20;
			}else{
				cycles += 8;
				pc += 1;
			}
			break;
		case 0xD0:
			if(!get_flag_carry()){
				ret();
				cycles += 20;
			}else{
				cycles += 8;
				pc += 1;
			}
			break;
		case 0xC8:
			if(get_flag_zero()){
				ret();
				cycles += 20;
			}else{
				cycles += 8;
				pc += 1;
			}
			break;
		case 0xD8:
			if(get_flag_carry()){
				ret();
				cycles += 20;
			}else{
				cycles += 8;
				pc += 1;
			}

			break;	
		case 0xC9:
			ret();
			cycles += 16;
			pc += 1;
			break;
		case 0xD9:
			ret();
			cycles += 16;
			interrupts_enabled = true;
			break;
		case 0xFB:
			interrupts_enabled = true;
			cycles +=4;
			pc += 1;
			break;
		case 0xF3:
			interrupts_enabled = false;
			cycles += 4;
			pc += 1;
			break;
		case 0xCD:
			call();
			cycles += 24;
			break;
		case 0xCC:
			if(get_flag_zero()){
				call();
				cycles += 24;
			}else{
				cycles += 12;
				pc += 3;
			}
			break;
		case 0xDC:
			if(get_flag_carry()){
				call();
				cycles += 24;
			}else{
				cycles += 12;
				pc += 3;
			}
			break;
		case 0xC4:
			if(!get_flag_zero()){
				call();
				cycles += 24;
			}else{
				cycles += 12;
				pc += 3;
			}
			break;
		case 0xD4:
			if(!get_flag_carry()){
				call();
				cycles += 24;
			}else{
				cycles += 12;
				pc += 3;
			}
			break;
		//rotate a
		case 0x07:
			af = RL((af>> 8),1) + (af & 0xFF);
			set_flag_zero(0);
			cycles += 4;
			pc += 1;
			break;
		case 0x17:
			af = RL((af>> 8),0) + (af & 0xFF);
			set_flag_zero(0);
			cycles += 4;
			pc += 1;
			break;
		case 0x0F:
			af = RR((af>> 8),1) + (af & 0xFF);
			set_flag_zero(0);
			cycles += 4;
			pc += 1;
			break;
		case 0x1F:
			af = RR((af>> 8),0) + (af & 0xFF);
			set_flag_zero(0);
			cycles += 4;
			pc += 1;
			break;
		//carry flag stuff
		case 0x37:
			set_flag_carry(1);
			set_flag_subtract(0);
			set_flag_hcarry(0);
			cycles += 4;
			pc += 1;
			break;
		case 0x3F:
			set_flag_carry(!get_flag_carry());
			set_flag_subtract(0);
			set_flag_hcarry(0);
			cycles += 4;
			pc += 1;
			break;
		case 0x2F:
			af = (~(af >> 8) << 8) + (af & 0xFF);
			set_flag_subtract(1);
			set_flag_hcarry(1);
			cycles += 4;
			pc += 1;
			break;
		case 0xCB:
			ExecuteCB(opcode);
		case 0x10:
			//halt cpu? what
			pc += 1;
			break;
		case 0x76:
			halted = true;
			pc += 1;
			break;
		case 0x27://DAA: i understand 13% of whats happening here.
			{
			uint8_t result = af >> 8;
			uint16_t correction = get_flag_carry() ? 0x60 : 0x00;
			if(get_flag_hcarry() || (!get_flag_subtract() && ((result & 0x0F) > 9))){
				correction |= 0x06;
			}
			if(get_flag_carry() || (!get_flag_subtract()&&(result > 0x99))){
				correction |= 0x60;
			}
			if(get_flag_subtract()){
				result = result - correction
			}else{
				result = result + correction
			}
			af = ((result << 8) & 0xFF00) + (af & 0xFF);
			if(((correction << 2) & 0x100) != 0){
				set_flag_carry(true);
			}
			set_flag_hcarry(false);
			set_flag_zero(result == 0);
			}



	
	}
}
unsigned ExecuteCB(uint8_t opcode){}