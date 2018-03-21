			ADD	R4, R0, R1, 10				# R4 = res = 10
initloop:	JEQ R0, R4, R0, main
			ADD R2, R4, R1, 1999			# address = 2000 + res - 1
			ST	R0, R4, R2, 0				# mem[address] = res
			SUB R4, R4, R1, 1				# res--		
			JIN R0, R1, R0, initloop
main:		ADD R2, R0, R1, 2010
			ADD R3, R0, R1, 1				#R3 = row = 1
			ADD R4, R0, R0, 0				#R4 = col = 0
rowloop:	ADD R7, R0, R1, 10
			JEQ R0, R3, R7, finish			# if row == 10 finish
colloop:	ADD R7, R0, R1, 10				
			JEQ R0, R4, R7, finishloop		# if col == 10 
			SUB R7, R2, R1, 10				
			LD  R7, R0, R7, 0				# tmp = mem[address - 10]
			ADD R7, R7, R4, 0				# 
			ADD R7, R7, R1, 1				# tmp += col + 1
			ST  R0, R7, R2, 0				# mem[address] = tmp
			ADD R4, R4, R1, 1				# col++
			ADD R2, R2, R1, 1
			JIN	R0, R1, R0, colloop
finishloop:	ADD R3, R3, R1, 1				# row++
			JIN	R0, R1, R0, rowloop			
finish:		HLT R0, R0, R0, 0
			
