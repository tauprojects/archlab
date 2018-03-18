		LD	R3, R0, R1, 1000			# R3 = Mem[1000] = x
		LD	R4, R0, R1, 1001			# R4 = Mem[1001] = y
		ADD R2,	R0, R0, 0				# R2 = 0 = result
		ADD R5, R3, R0, 0				# R6 = temp_x
		ADD R6, R4, R0, 0				# R6 = temp_y
		JLT R0, R3, R0, rev_x			# jump to rev_x if x<0
check_y:
		JLT R0, R4, R0, rev_y			# jump to rev_y if y<0
		JIN R0, R1, R0, loop			#ADD TO ASSEMBLY
rev_x:
		SUB R5, R0, R5, 0 				# temp_x = -temp_x
		JIN R0, R1, R0, check_y			# check if y<0 anyway
rev_y:
		SUB R6, R0, R6, 0 				# temp_y = -temp_y
										# now both temp_x, temp_y are >=0
loop:
		JEQ R0, R6, R0, check_sign		# jump to check_sign if temp_y=0
		ADD R2, R2, R3, 0				# result = result + x
		SUB R6, R6, R1, 1				# temp_y = temp_y - 1
		JEQ R0, R0, R0, loop			# back to beginning of loop

check_sign:
		JLT R0, R3, R0, check_sign2		# jump to negate if x<0
		JLT R0, R4, R0, negate			# if we're here and we jump it means x>0 and y<0, if not then x>0 and y>0, finish
finish:									
		ST	R2, R0, R1, 1002			# Mem[1002] = res
		HALT		
check_sign2:
		JLT R0, R4, R0, finish			# if we're here it means x<0, check if y<0, if yes then finish		
negate:
		SUB R2, R0, R2, 0 				# res = -res
		JEQ R1, R0, R0, finish			# jump to finish