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
			
