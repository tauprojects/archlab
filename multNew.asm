		asm_cmd(LD	,3, 0, 1, 1000);			// 3 = Mem[1000] = x
		asm_cmd(LD	,4, 0, 1, 1001);			// 4 = Mem[1001] = y
		asm_cmd(ADD ,2,	0, 1, 1	);			// 2 = mask = 1
		asm_cmd(ADD ,5, 0, 1, 32	);			// 5 = i = 32
		asm_cmd(ADD ,6, 0, 0, 0	);			// 6 = 0 --> result
		asm_cmd(AND ,7, 2, 3, 0	);			// 5 = x & 2
		asm_cmd(JEQ ,0, 0, 7, 8	);			// if 5==0 goto shift
		asm_cmd(ADD ,6, 6, 4, 0	);			// else, do addition: result += y
		asm_cmd(LSF ,2, 2, 1, 1	);			// mask << 1
		asm_cmd(LSF ,4, 4, 1, 1	);			// y << 1
		asm_cmd(SUB ,5, 5, 1, 1	);			// i--
		asm_cmd(JLT ,0, 0, 5, 5	);			// if i > 0 goto loop
		asm_cmd(ST  ,0, 6, 1, 1002);			// Mem[1002] = result