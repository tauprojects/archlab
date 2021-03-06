/*
 * sim.h
 *
 */

#ifndef MAIN_H_
#define MAIN_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Memory Size = 65535
#define MEM_SIZE (1 << 16)

//Number of internal registers
#define NUM_REGS  8

//Word size - 32 bit
#define BIT_SIZE (1 << 4)

#define LINE_SIZE (500)

//Error Messages Declarations
#define ERR_MSG_INVALID_ARGS_NUM "Invalid Number Of Arguments - Expected: 5 , Actual: "
#define INVALID_ARG "Invalid Argument"
#define ERR_MSG_OPEN_FILE "Error While Opening File: "


typedef enum _OPCODE 
{
	ADD = 0,
	SUB,
	LSF,
	RSF,
	AND,
	OR,
	XOR,
	LHI,
	LD,
	ST,
	JLT = 16,
	JLE,
	JEQ,
	JNE,
	JIN,
	HLT = 24
} OPCODE;

/////////////////////////
//Function Declarations//
/////////////////////////

/*
 * Print Trace method()
 * prints the register status into trace.txxt
 * @return - void
 */
void printTrace_new();

/*
 * Print Memout method()
 * prints the  memory state into memout.txt after each instruction execution
 * @return - void
 */
void printMemout(char* memoutPath);

/*
 * Instruction Execution method()
 * execute a current instruction - using switch-case
 * works with and modify  global variables
 * @return - 1 if halt, 0 otherwise
 */
int instExec();

/*
 * Shift Right Logical method
 * @x - input number to be shifted
 * @n - input shift number.
 * @return - x<<n.
 */
void gracfullyExit();

/*
 * Shift Right Arithmetic method - with sign extension
 * @x - input number to be shifted
 * @n - input shift number.
 * @return - x>>n.
 */
int arithmeticRightShift(int x, int n);

/*
 * Shift Left Logical method
 * @x - input number to be shifted
 * @n - input shift number.
 * @return - x<<n.
 */
int logicalLeftShift(int x, int n);

/*
 * Extract multiple bits method()
 * @x - input value
 * @lsb
 * @msb
 */
int sbs(int x, int msb, int lsb);

/*
 * Extract single bit method()
 * @x - input value
 * @bit - bit number.
 */
int sb(int x, int bit);

#endif /* MAIN_H_ */
