/*
 Created By: Matan Gizunterman, Almog Zeltsman
 Part of the code was with the help of given examples code, StackOverflow Issues, and Tutorials Point C Programming Guide
 */


#include "sim.h"

// main memory
int mem[MEM_SIZE];
//Registers
int reg_list[NUM_REGS];
// opcode names
char op_name[][25] = { "ADD", "SUB", "LSF", "RSF", "RSF", "AND", "OR", "XOR",
		"LHI", "LD", "ST", "10", "11", "12", "13", "14", "15", "JLT", "JLE", "JEQ", //filler elements for easy access 
		"JNE", "JIN", "21", "22", "23", "HLT"};

int inst, opcode, dst, src0, src1, instCnt = 0; //global variables - for easy modeling
short int imm;
unsigned short int PC = 0;

// files to be read\written
FILE *fp_memin, *fp_memout, *fp_trace;


//Main function
int main(int argc, char *argv[]) {
	//Variables declarations
	int i, last;

	//Checking if number of arguments is valid
	if (argc < 4) {
		printf(ERR_MSG_INVALID_ARGS_NUM);
		printf("%d\n", argc - 1);
		exit(-1);
	} else if (argc > 4) {
		printf(ERR_MSG_INVALID_ARGS_NUM);
		printf("%d\n", argc - 1);
		exit(-1);
	}
	//Memory map input file path
	char* meminPath = argv[1];

	//Memory Map output file path
	char* memoutPath = argv[2];

	//Trace output file path
	char* tracePath = argv[3];


	//Opening files and validating return value for success
	fp_memin = fopen(meminPath, "r");
	if (!fp_memin) 
	{
		printf(ERR_MSG_OPEN_FILE);
		puts("memin.txt");
		exit(-1);
	}


	fp_trace = fopen(tracePath, "w");
	if (!fp_trace) 
	{
		printf(ERR_MSG_OPEN_FILE);
		puts("trace.txt");
		exit(-1);
	}

	//Initial Memory to zero
	memset(mem, 0, MEM_SIZE * sizeof(int));

	//Initial Registers to zero
	memset(reg_list, 0, NUM_REGS * sizeof(char));

	//Read memin.txt content into memory map
	i = 0;
	while (!feof(fp_memin)) 
	{
		if (fscanf(fp_memin, "%08X\n", &mem[i]) != 1)
			break;
		i++;
	}
	//Closing memin after reading
	fclose(fp_memin);

	//Find last non-zero memory entry
	for (last = MEM_SIZE - 1; last >= 0 && mem[last] == 0; last--);

	//Decode Instruction
	while (PC <= last) 
	{
		//Fetch Stage
		inst = mem[PC];
		//print non zero contents
		if (inst != 0) 
		{
			//Decode Stage
			opcode = sbs(inst, 29, 25);
			dst = sbs(inst, 24, 22);
			src0 = sbs(inst, 21, 19);
			src1 = sbs(inst, 18, 16);
			imm = sbs(inst, 15, 0);
			reg_list[1] = imm; //load imm to $r1

			if (dst == 0) 
			{
				PC++;
			} 
			else 
			{
				//Instruction Counter Increment
				instCnt++;

				//Print trace.txt
				printTrace_new(PC, inst);

				//Execute Stage
				instExec();

				//Printing memout.txt
				printMemout(memoutPath);

			}
		}
		else
		{
			PC++;
		}
	}
	gracfullyExit();
	return 0;

}

void instExec() 
{
	unsigned int temp;
	switch (opcode) 
	{
	case 0:		 //ADD
		reg_list[dst] = reg_list[src0] + reg_list[src1];
		break;

	case 1:		//SUB
		reg_list[dst] = reg_list[src0] - reg_list[src1];
		break;

	case 2:		//LSF
		reg_list[dst] = logicalLeftShift(reg_list[src0], reg_list[src1]);
		break;

	case 3:		//RSF
		reg_list[dst] = arithmeticRightShift(reg_list[src0], reg_list[src1]);
		break;

	case 4:		//AND
		reg_list[dst] = reg_list[src0] & reg_list[src1];
		break;

	case 5:		//OR
		reg_list[dst] = reg_list[src0] | reg_list[src1];
		break;

	case 6:		//XOR
		reg_list[dst] = reg_list[src0] ^ reg_list[src1];
		break;
		
	case 7:	   //LHI
		temp = ((int)imm & 0x0000FFFF) << 16;
		reg_list[dst] = (reg_list[dst] & 0x0000FFFF) | temp;
		break;

	case 8:		//LD
		reg_list[dst] = mem[reg_list[src1]];
		break;

	case 9:		//ST
		mem[reg_list[src1]] = reg_list[src0];
		break;

	case 16:	//JLT
		if (reg_list[src0] < reg_list[src1]) 
		{
			reg_list[7] = PC;
			PC = imm; 
		}
		break;

	case 17:	//JLE
		if (reg_list[src0] <= reg_list[src1])
		{
			reg_list[7] = PC;
			PC = imm;
		}
		break;

	case 18:	//JEQ
		if (reg_list[src0] == reg_list[src1])
		{
			reg_list[7] = PC;
			PC = imm;
		}
		break;

	case 19:	//JEQ
		if (reg_list[src0] != reg_list[src1])
		{
			reg_list[7] = PC;
			PC = imm;
		}
		break;

	case 20:	//$JIN
		reg_list[7] = PC;
		PC = reg_list[dst] & 0x0000FFFF;
		break;

	case 24:	//HALT
		gracfullyExit();
		exit(0);
		break;

	default:
		break;
	}

	if (opcode <= 9)
		PC++;


}
//Trace Eample
//	--- instruction 0 (0000) @ PC 0 (0000) -----------------------------------------------------------
//	pc = 0000, inst = 0088000f, opcode = 0 (ADD), dst = 2, src0 = 1, src1 = 0, immediate = 0000000f
//	r[0] = 00000000 r[1] = 0000000f r[2] = 00000000 r[3] = 00000000 
//	r[4] = 00000000 r[5] = 00000000 r[6] = 00000000 r[7] = 00000000 
//	>>>> EXEC: R[2] = 15 ADD 0 <<<<
//int inst, op, rd, rs, rt, PC, instCnt = 0;

void printTrace_new() 
{

	fprintf(fp_trace, "--- instrcution %d (%04X) @ PC %d (%04X) -----------------------------------------------------------\n", 
		instCnt, instCnt, PC, PC);

	fprintf(fp_trace, "pc = %04X, inst = %08x, opcode = %d (%s), dst = %d, src0 = %d, src1 = %d, immediate = %08x\n", 
		PC, instCnt, opcode, op_name[opcode], dst, src0, src1, imm);
	int i;
	for (i = 0; i < NUM_REGS; i++) {
		fprintf(fp_trace, "r[%d] = %08x ", i, reg_list[i]);
		if(i==3)
			fprintf(fp_trace, "\n");
	}
	fprintf(fp_trace, "\n>>>> EXEC: R[%d] = %d %s %d \n",dst, src0, op_name[opcode], src1);
	
}

void printTrace() {
	fprintf(fp_trace, "%08X %08X ", PC, inst);
	int i;
	for (i = 0; i < NUM_REGS; i++) {
		fprintf(fp_trace, "%08x ", reg_list[i]);
	}
	fprintf(fp_trace, "\n");
}

void printMemout(char* memoutPath) {
	fp_memout = fopen(memoutPath, "wt");
	if (!fp_memout) {
		printf(ERR_MSG_OPEN_FILE);
		puts("memout.txt");
		exit(-1);
	}
	int i;
	for (i = 0; i < MEM_SIZE; i++) {
		fprintf(fp_memout, "%08X\n", mem[i]);
	}
	fclose(fp_memout);
}


void gracfullyExit() {
	// close opened files
	fclose(fp_trace);
	puts("Exited Gracefully");
}

int arithmeticRightShift(int x, int n) 
{
	if (x < 0 && n > 0)
		return x >> n | ~(~0U >> n);
	else
		return x >> n;
}
int logicalLeftShift(int x, int n) 
{
	return (unsigned) x << n;
}

// extract single bit
int sb(int x, int bit) 
{
	return (x >> bit) & 1;
}

// extract multiple bits
int sbs(int x, int msb, int lsb) 
{
	if (msb == 31 && lsb == 0)
		return x;

	return (x >> lsb) & ((1 << (msb - lsb + 1)) - 1);
}
