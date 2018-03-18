		LD	R3, R0, R1, 1000			# R3 = Mem[1000] = x
		LD	R4, R0, R1, 1001			# R4 = Mem[1001] = y
		ADD R2,	R0, R1, 1				# R2 = mask = 1
		ADD R5, R0, R1, 32				# R5 = i = 32
		ADD R6, R0, R0, 0				# R6 = 0 --> result
loop:   AND R7, R2, R3, 0				# R5 = x & R2
		JEQ R0, R0, R7, shift			# if R5==0 goto shift
		ADD R6, R6, R4, 0				# else, do addition: result += y
shift:  LSF R2, R2, R1, 1				# mask << 1
		LSF R4, R4, R1, 1				# y << 1
		SUB R5, R5, R1, 1				# i--
		JLT R0, R0, R5, loop			# if i > 0 goto loop
		ST  R0, R6, R1, 1002			# Mem[1002] = result
		HALT