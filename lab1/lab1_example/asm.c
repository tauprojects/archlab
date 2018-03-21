/*
 * SP ASM: Simple Processor assembler
 *
 * usage: asm
 */
#include <stdio.h>
#include <stdlib.h>

#define ADD 0
#define SUB 1
#define LSF 2
#define RSF 3
#define AND 4
#define OR  5
#define XOR 6
#define LHI 7
#define LD 8
#define ST 9
#define JLT 16
#define JLE 17
#define JEQ 18
#define JNE 19
#define JIN 20
#define HLT 24

#define MEM_SIZE_BITS	(16)
#define MEM_SIZE	(1 << MEM_SIZE_BITS)
#define MEM_MASK	(MEM_SIZE - 1)
unsigned int mem[MEM_SIZE];

int pc = 0;

static void asm_cmd(int opcode, int dst, int src0, int src1, int immediate)
{
	int inst;

	inst = ((opcode & 0x1f) << 25) | ((dst & 7) << 22) | ((src0 & 7) << 19) | ((src1 & 7) << 16) | (immediate & 0xffff);
	mem[pc++] = inst;
}

static void assemble_program(char *program_name)
{
	FILE *fp;
	int addr, last_addr;

	for (addr = 0; addr < MEM_SIZE; addr++)
		mem[addr] = 0;

	pc = 0;

	/*
	 * Program starts here
	 */
asm_cmd(ADD	,4, 0, 1, 10		);
asm_cmd(JEQ ,0, 4, 0, 6          );
asm_cmd(ADD ,2, 4, 1, 1999		);
asm_cmd(ST	,0, 4, 2, 0			);
asm_cmd(SUB ,4, 4, 1, 1			);
asm_cmd(JIN ,0, 1, 0, 1       );
asm_cmd(ADD ,2, 0, 1, 2010           );
asm_cmd(ADD ,3, 0, 1, 1			);
asm_cmd(ADD ,7, 0, 1, 10             );
asm_cmd(JEQ ,0, 3, 7, 23		);
asm_cmd(ADD ,4, 0, 0, 0			);
asm_cmd(ADD ,7, 0, 1, 10			);
asm_cmd(JEQ ,0, 4, 7, 21	);
asm_cmd(SUB ,7, 2, 1, 10			);
asm_cmd(LD  ,7, 0, 7, 0			);
asm_cmd(ADD ,7, 7, 4, 0			);
asm_cmd(ADD ,7, 7, 1, 1			);
asm_cmd(ST  ,0, 7, 2, 0			);
asm_cmd(ADD ,4, 4, 1, 1			);
asm_cmd(ADD, 2, 2, 1, 1			);
asm_cmd(JIN	,0, 1, 0, 11        );
asm_cmd(ADD ,3, 3, 1, 1			);
asm_cmd(JIN	,0, 1, 0, 8		);
asm_cmd(HLT ,0, 0, 0, 0			);
	
	/* 
	 * Constants are planted into the memory somewhere after the program code:
	 */

	last_addr = 50;

	fp = fopen(program_name, "w");
	if (fp == NULL) {
		printf("couldn't open file %s\n", program_name);
		exit(1);
	}
	addr = 0;
	while (addr < last_addr) {
		fprintf(fp, "%08x\n", mem[addr]);
		addr++;
	}
}


int main(int argc, char *argv[])
{
	
	if (argc != 2){
		printf("usage: asm program_name\n");
		return -1;
	}else{
		assemble_program(argv[1]);
		printf("SP assembler generated machine code and saved it as %s\n", argv[1]);
		return 0;
	}
	
}
