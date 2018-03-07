		add $sp, $zero, $imm, 2048 			# $sp = 2048
		lw $a0, $zero, $imm, 1000			# $a0 = Mem[1000] = x
		lw $a1, $zero, $imm, 1001			# $a1 = Mem[1001] = y
		add $t0, $zero, $zero, 0			# $t0 = 0 = result
		add $t1, $a1, $zero, 0				# $t1 = temp_y
		ble $imm, $t1, $zero, loop_minus	# jump to loop_minus if temp_y<=0
loop_plus:
		beq $imm, $t1, $zero, check_sign	# jump to check_sign if temp_y=0
		add $t0, $t0, $a0, 0				# result = result + x
		sub $t1, $t1, $imm, 1				# temp_y = temp_y - 1
		beq $imm, $zero, $zero, loop_plus	# back to beginning of loop_plus
loop_minus:
		beq $imm, $t1, $zero, check_sign	# jump to check_sign if temp_y=0
		add $t0, $t0, $a0, 0				# result = result + x
		add $t1, $t1, $imm, 1				# temp_y = temp_y + 1
		beq $imm, $zero, $zero, loop_minus	# back to beginning of loop_plus
check_sign:
		bgt $imm, $zero, $a1, negate		# jump to negate if y<0 (we need to reverse sign of res iff y<0)
finish:
		sw $t0, $zero, $imm, 1026			# Mem[1002]	 = res
		halt $zero, $zero, $zero, 0			# exit mult.asm
negate:
		sub $t0, $zero, $t0, 0 				# res = -res
		beq $imm, $zero, $zero, finish			# jump to finish
		.word 1024 -2
		.word 1025 3
		
		

