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
char op_name[][25] = { "ADD", "SUB", "LSF", "RSF", "AND", "OR", "XOR",
"LHI", "LD", "ST", "10", "11", "12", "13", "14", "15", "JLT", "JLE", "JEQ", //filler elements for easy access 
"JNE", "JIN", "21", "22", "23", "HLT" };

int inst, dst, src0, src1, instCnt = 0; //global variables - for easy modeling
short int imm;
OPCODE opcode;
unsigned short int PC = 0;

// files to be read\written
FILE *fp_memin, *fp_memout, *fp_trace;

#define isRegularInst()		(opcode <=9)

//Main function
int main(int argc, char *argv[]) {
	//Variables declarations
	int i, last;
	printf("\n");
	//Checking if number of arguments is valid
	if (argc != 2)
	{
		printf(ERR_MSG_INVALID_ARGS_NUM);
		printf("%d\n", argc - 1);
		exit(-1);
	}
	//Memory map input file path
	char* meminPath = argv[1];

	//Trace output file path
	char* tracePath = "trace.txt";

	//Memory Map output file path
	char* memoutPath = "sram_out.txt";

	//Opening files and validating return value for success
	fp_memin = fopen(meminPath, "r");
	if (!fp_memin)
	{
		printf("%s %s", ERR_MSG_OPEN_FILE, meminPath);
		exit(-1);
	}


	fp_trace = fopen(tracePath, "w");
	if (!fp_trace)
	{
		printf("%s %s", ERR_MSG_OPEN_FILE, tracePath);
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

			if (dst <= 1 && opcode < LD)	//don't write to R0 or R1
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
				if (instExec())
					break;

				if (isRegularInst())
				{
					PC++;
				}
			}
		}
		else
		{
			PC++;
		}
	}

	printMemout(memoutPath);

	gracfullyExit();
	return 0;

}

int instExec()
{
	int ret = 0;

	unsigned int temp;

	switch (opcode)
	{
	case ADD:		 //ADD
		reg_list[dst] = reg_list[src0] + reg_list[src1];
		break;

	case SUB:		//SUB
		reg_list[dst] = reg_list[src0] - reg_list[src1];
		break;

	case LSF:		//LSF
		reg_list[dst] = logicalLeftShift(reg_list[src0], reg_list[src1]);
		break;

	case RSF:		//RSF
		reg_list[dst] = arithmeticRightShift(reg_list[src0], reg_list[src1]);
		break;

	case AND:		//AND
		reg_list[dst] = reg_list[src0] & reg_list[src1];
		break;

	case OR:		//OR
		reg_list[dst] = reg_list[src0] | reg_list[src1];
		break;

	case XOR:		//XOR
		reg_list[dst] = reg_list[src0] ^ reg_list[src1];
		break;

	case LHI:	   //LHI
		temp = ((int)imm & 0x0000FFFF) << 16;
		reg_list[dst] = (reg_list[dst] & 0x0000FFFF) | temp;
		break;

	case LD:		//LD
		reg_list[dst] = mem[reg_list[src1] & 0x0000FFFF];	//take only relevant bits of address
		break;

	case ST:		//ST
		mem[reg_list[src1] & 0x0000FFFF] = reg_list[src0];	//take only relevant bits of address
		break;

	case JLT:		//JLT
		if (reg_list[src0] < reg_list[src1])
		{
			reg_list[7] = PC;
			PC = imm;
		}
		else
		{
			PC++;
		}
		break;

	case JLE:	//JLE
		if (reg_list[src0] <= reg_list[src1])
		{
			reg_list[7] = PC;
			PC = imm;
		}
		else
		{
			PC++;
		}
		break;

	case JEQ:	//JEQ
		if (reg_list[src0] == reg_list[src1])
		{
			reg_list[7] = PC;
			PC = imm;
		}
		else
		{
			PC++;
		}
		break;

	case JNE:	//JNE
		if (reg_list[src0] != reg_list[src1])
		{
			reg_list[7] = PC;
			PC = imm;
		}
		else
		{
			PC++;
		}
		break;

	case JIN:	//JIN
		reg_list[7] = PC;
		PC = reg_list[src0] & 0x0000FFFF;
		break;

	case HLT:	//HALT
		ret = 1;
		break;

	default:
		break;
	}

	return ret;
}


void printTrace_new()
{
	int i;

	fprintf(fp_trace, "--- instruction %d (%04x) @ PC %d (%04d) -----------------------------------------------------------\n",
		instCnt - 1, instCnt - 1, PC, PC);

	fprintf(fp_trace, "pc = %04d, inst = %08x, opcode = %d (%s), dst = %d, src0 = %d, src1 = %d, immediate = %08x\n",
		PC, mem[PC], opcode, op_name[opcode], dst, src0, src1, imm);

	for (i = 0; i < NUM_REGS; i++)
	{
		fprintf(fp_trace, "r[%d] = %08x ", i, reg_list[i]);

		if (i == 3)
		{
			fprintf(fp_trace, "\n");
		}
	}

	switch (opcode)
	{
	case ADD:
	case SUB:
	case LSF:
	case RSF:
	case AND:
	case OR:
	case XOR:
		fprintf(fp_trace, "\n\n>>>> EXEC: R[%d] = %d %s %d <<<<\n\n", dst, reg_list[src0], op_name[opcode], reg_list[src1]);
		break;

	case LHI:
		fprintf(fp_trace, "\n\n>>>> EXEC: R[%d] = 0x%08X %s 0x%04X <<<<\n\n", dst, reg_list[dst], op_name[opcode], imm & 0x0000FFFF);
		break;

	case LD:
		fprintf(fp_trace, "\n\n>>>> EXEC: R[%d] = MEM[%d] = 0x%08x (%d) <<<<\n\n", dst, reg_list[src1], mem[reg_list[src1]], mem[reg_list[src1]]);
		break;

	case ST:
		fprintf(fp_trace, "\n\n>>>> EXEC: MEM[%d] = R[%d] = 0x%08x (%d)<<<<\n\n", reg_list[src1], src0, reg_list[src0], reg_list[src0]);
		break;

	case JLT:
	case JLE:
	case JEQ:
	case JNE:
		fprintf(fp_trace, "\n\n>>>> EXEC: %s %d, %d, %d <<<<\n\n", op_name[opcode], reg_list[src0], reg_list[src1], imm);
		break;
	case JIN:
		fprintf(fp_trace, "\n\n>>>> EXEC: %s %d <<<<\n\n", op_name[opcode], reg_list[src0]);
		break;
	case HLT:
		fprintf(fp_trace, "\n\n>>>> EXEC: HALT at PC %04x <<<<\n\n", PC);
		break;
	}
}

void printMemout(char* memoutPath)
{
	int i;

	fp_memout = fopen(memoutPath, "wt");

	if (!fp_memout)
	{
		printf("%s %s", ERR_MSG_OPEN_FILE, memoutPath);
		exit(-1);
	}

	for (i = 0; i < MEM_SIZE; i++)
	{
		fprintf(fp_memout, "%08x\n", mem[i]);
	}
	fclose(fp_memout);
}


void gracfullyExit()
{
	// close opened files
	fclose(fp_trace);
	printf("sim finished at pc %d, %d instructions\n", PC, instCnt);
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
	return (unsigned)x << n;
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
