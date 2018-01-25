	NOP		/ 00
	LD	BC,$64	/ 01 40 00
	LD	(BC),A	/ 02
	INC	BC	/ 03
	INC	B	/ 04
	DEC	B	/ 05
	LD	B,$32	/ 06 20
	RLCA		/ 07
	EX	AF,AF'	/ 08
	ADD	HL,BC	/ 09
	LD	A,(BC)	/ 0A
	DEC	BC	/ 0B
	INC	C	/ 0C
	DEC	C	/ 0D
	LD	C,$64	/ 0E 40
	RRCA		/ 0F
L1:	DJNZ	$L1	/ 10 02
	LD	DE,$32	/ 11 20 00
	LD	(DE),A	/ 12
	INC	DE	/ 13
	INC	D	/ 14
	DEC	D	/ 15
	LD	D,$64	/ 16 40
	RLA		/ 17
L2:	JR	$L2	/ 18 02
	ADD	HL,DE	/ 19
	LD	A,(DE)	/ 1A
	DEC	DE	/ 1B
	INC	E	/ 1C
	DEC	E	/ 1D
	LD	E,$64	/ 1E 40
	RRA		/ 1F
L3:	JR	NZ,$L3	/ 20 02
	LD	HL,$32	/ 21 20 00
	LD	(32768),HL	/ 22 00 80
	INC	HL	/ 23
	INC	H	/ 24
	DEC	H	/ 25
	LD	H,$64	/ 26 40
	DAA		/ 27
L4:	JR	Z,$L4	/ 28 02
	ADD	HL,HL	/ 29
	LD	HL, (16384)	/ 2A 00 40
	DEC	HL	/ 2B
	INC	L	/ 2C
	DEC	L	/ 2D
	LD	L,$32	/ 2E 20
	CPL		/ 2F
L5:	JR	NC,$L5	/ 30 02
	LD	SP,$64	/ 31 40 00
	LD	(32768),A	/ 32 00 80
	INC	SP	/ 33
	INC	(HL)	/ 34
	DEC	(HL)	/ 35
	LD	(HL),$32	/ 36 20
	SCF		/ 37
L6:	JR	C,$L6	/ 38 02
	ADD	HL,SP	/ 39
	LD	A,(16384)	/ 3A 00 40
	DEC	SP	/ 3B
	INC	A	/ 3C
	DEC	A	/ 3D
	LD	A,$32	/ 3E 20
	CCF		/ 3F
	LD	B,B	/ 40
	LD	B,C	/ 41
	LD	B,D	/ 42
	LD	B,E	/ 43
	LD	B,H	/ 44
	LD	B,L	/ 45
	LD	B,(HL)	/ 46
	LD	B,A	/ 47
	LD	C,B	/ 48
	LD	C,C	/ 49
	LD	C,D	/ 4A
	LD	C,E	/ 4B
	LD	C,H	/ 4C
	LD	C,L	/ 4D
	LD	C,(HL)	/ 4E
	LD	C,A	/ 4F
	LD	D,B	/ 50
	LD	D,C	/ 51
	LD	D,D	/ 52
	LD	D,E	/ 53
	LD	D,H	/ 54
	LD	D,L	/ 55
	LD	D,(HL)	/ 56
	LD	D,A	/ 57
	LD	E,B	/ 58
	LD	E,C	/ 59
	LD	E,D	/ 5A
	LD	E,E	/ 5B
	LD	E,H	/ 5C
	LD	E,L	/ 5D
	LD	E,(HL)	/ 5E
	LD	E,A	/ 5F
	LD	H,B	/ 60
	LD	H,C	/ 61
	LD	H,D	/ 62
	LD	H,E	/ 63
	LD	H,H	/ 64
	LD	H,L	/ 65
	LD	H,(HL)	/ 66
	LD	H,A	/ 67
	LD	L,B	/ 68
	LD	L,C	/ 69
	LD	L,D	/ 6A
	LD	L,E	/ 6B
	LD	L,H	/ 6C
	LD	L,L	/ 6D
	LD	L,(HL)	/ 6E
	LD	L,A	/ 6F
	LD	(HL),B	/ 70
	LD	(HL),C	/ 71
	LD	(HL),D	/ 72
	LD	(HL),E	/ 73
	LD	(HL),H	/ 74
	LD	(HL),L	/ 75
	HALT		/ 76
	LD	(HL),A	/ 77
	LD	A,B	/ 78
	LD	A,C	/ 79
	LD	A,D	/ 7A
	LD	A,E	/ 7B
	LD	A,H	/ 7C
	LD	A,L	/ 7D
	LD	A,(HL)	/ 7E
	LD	A,A	/ 7F
	ADD	A,B	/ 80
	ADD	A,C	/ 81
	ADD	A,D	/ 82
	ADD	A,E	/ 83
	ADD	A,H	/ 84
	ADD	A,L	/ 85
	ADD	A,(HL)	/ 86
	ADD	A,A	/ 87
	ADC	A,B	/ 88
	ADC	A,C	/ 89
	ADC	A,D	/ 8A
	ADC	A,E	/ 8B
	ADC	A,H	/ 8C
	ADC	A,L	/ 8D
	ADC	A,(HL)	/ 8E
	ADC	A,A	/ 8F
	SUB	A,B	/ 90
	SUB	A,C	/ 91
	SUB	A,D	/ 92
	SUB	A,E	/ 93
	SUB	A,H	/ 94
	SUB	A,L	/ 95
	SUB	A,(HL)	/ 96
	SUB	A,A	/ 97
	SBC	A,B	/ 98
	SBC	A,C	/ 99
	SBC	A,D	/ 9A
	SBC	A,E	/ 9B
	SBC	A,H	/ 9C
	SBC	A,L	/ 9D
	SBC	A,(HL)	/ 9E
	SBC	A,A	/ 9F
	AND	A,B	/ A0
	AND	A,C	/ A1
	AND	A,D	/ A2
	AND	A,E	/ A3
	AND	A,H	/ A4
	AND	A,L	/ A5
	AND	A,(HL)	/ A6
	AND	A,A	/ A7
	XOR	A,B	/ A8
	XOR	A,C	/ A9
	XOR	A,D	/ AA
	XOR	A,E	/ AB
	XOR	A,H	/ AC
	XOR	A,L	/ AD
	XOR	A,(HL)	/ AE
	XOR	A,A	/ AF
	OR	A,B	/ B0
	OR	A,C	/ B1
	OR	A,D	/ B2
	OR	A,E	/ B3
	OR	A,H	/ B4
	OR	A,L	/ B5
	OR	A,(HL)	/ B6
	OR	A,A	/ B7
	CP	A,B	/ B8
	CP	A,C	/ B9
	CP	A,D	/ BA
	CP	A,E	/ BB
	CP	A,H	/ BC
	CP	A,L	/ BD
	CP	A,(HL)	/ BE
	CP	A,A	/ BF
	RET	NZ	/ C0
	POP	BC	/ C1
	JP	NZ,$16384	/ C2 00 40
	JP	$32768	/ C3 00 80
	CALL	NZ,$32768	/ C4 00 80
	PUSH	BC	/ C5
	ADD	A,$32	/ C6 20
	RST	$0	/ C7
	RET	Z	/ C8
	RET		/ C9
	JP	Z,$32768	/ CA 00 80
	RLC	B	/ CB 00
	RLC	C	/ CB 01
	RLC	D	/ CB 02
	RLC	E	/ CB 03
	RLC	H	/ CB 04
	RLC	L	/ CB 05
	RLC	(HL)	/ CB 06
	RLC	A	/ CB 07
	RRC	B	/ CB 08
	RRC	C	/ CB 09
	RRC	D	/ CB 0A
	RRC	E	/ CB 0B
	RRC	H	/ CB 0C
	RRC	L	/ CB 0D
	RRC	(HL)	/ CB 0E
	RRC	A	/ CB 0F
	RL	B	/ CB 10
	RL	C	/ CB 11
	RL	D	/ CB 12
	RL	E	/ CB 13
	RL	H	/ CB 14
	RL	L	/ CB 15
	RL	(HL)	/ CB 16
	RL	A	/ CB 17
	RR	B	/ CB 18
	RR	C	/ CB 19
	RR	D	/ CB 1A
	RR	E	/ CB 1B
	RR	H	/ CB 1C
	RR	L	/ CB 1D
	RR	(HL)	/ CB 1E
	RR	A	/ CB 1F
	SLA	B	/ CB 20
	SLA	C	/ CB 21
	SLA	D	/ CB 22
	SLA	E	/ CB 23
	SLA	H	/ CB 24
	SLA	L	/ CB 25
	SLA	(HL)	/ CB 26
	SLA	A	/ CB 27
	SRA	B	/ CB 28
	SRA	C	/ CB 29
	SRA	D	/ CB 2A
	SRA	E	/ CB 2B
	SRA	H	/ CB 2C
	SRA	L	/ CB 2D
	SRA	(HL)	/ CB 2E
	SRA	A	/ CB 2F
	SLL	B	/ CB 30
	SLL	C	/ CB 31
	SLL	D	/ CB 32
	SLL	E	/ CB 33
	SLL	H	/ CB 34
	SLL	L	/ CB 35
	SLL	(HL)	/ CB 36
	SLL	A	/ CB 37
	SRL	B	/ CB 38
	SRL	C	/ CB 39
	SRL	D	/ CB 3A
	SRL	E	/ CB 3B
	SRL	H	/ CB 3C
	SRL	L	/ CB 3D
	SRL	(HL)	/ CB 3E
	SRL	A	/ CB 3F
	BIT	$0,B	/ CB 40
	BIT	$0,C	/ CB 41
	BIT	$0,D	/ CB 42
	BIT	$0,E	/ CB 43
	BIT	$0,H	/ CB 44
	BIT	$0,L	/ CB 45
	BIT	$0,(HL)	/ CB 46
	BIT	$0,A	/ CB 47
	BIT	$1,B	/ CB 48
	BIT	$1,C	/ CB 49
	BIT	$1,D	/ CB 4A
	BIT	$1,E	/ CB 4B
	BIT	$1,H	/ CB 4C
	BIT	$1,L	/ CB 4D
	BIT	$1,(HL)	/ CB 4E
	BIT	$1,A	/ CB 4F
	BIT	$2,B	/ CB 50
	BIT	$2,C	/ CB 51
	BIT	$2,D	/ CB 52
	BIT	$2,E	/ CB 53
	BIT	$2,H	/ CB 54
	BIT	$2,L	/ CB 55
	BIT	$2,(HL)	/ CB 56
	BIT	$2,A	/ CB 57
	BIT	$3,B	/ CB 58
	BIT	$3,C	/ CB 59
	BIT	$3,D	/ CB 5A
	BIT	$3,E	/ CB 5B
	BIT	$3,H	/ CB 5C
	BIT	$3,L	/ CB 5D
	BIT	$3,(HL)	/ CB 5E
	BIT	$3,A	/ CB 5F
	BIT	$4,B	/ CB 60
	BIT	$4,C	/ CB 61
	BIT	$4,D	/ CB 62
	BIT	$4,E	/ CB 63
	BIT	$4,H	/ CB 64
	BIT	$4,L	/ CB 65
	BIT	$4,(HL)	/ CB 66
	BIT	$4,A	/ CB 67
	BIT	$5,B	/ CB 68
	BIT	$5,C	/ CB 69
	BIT	$5,D	/ CB 6A
	BIT	$5,E	/ CB 6B
	BIT	$5,H	/ CB 6C
	BIT	$5,L	/ CB 6D
	BIT	$5,(HL)	/ CB 6E
	BIT	$5,A	/ CB 6F
	BIT	$6,B	/ CB 70
	BIT	$6,C	/ CB 71
	BIT	$6,D	/ CB 72
	BIT	$6,E	/ CB 73
	BIT	$6,H	/ CB 74
	BIT	$6,L	/ CB 75
	BIT	$6,(HL)	/ CB 76
	BIT	$6,A	/ CB 77
	BIT	$7,B	/ CB 78
	BIT	$7,C	/ CB 79
	BIT	$7,D	/ CB 7A
	BIT	$7,E	/ CB 7B
	BIT	$7,H	/ CB 7C
	BIT	$7,L	/ CB 7D
	BIT	$7,(HL)	/ CB 7E
	BIT	$7,A	/ CB 7F
	RES	$0,B	/ CB 80
	RES	$0,C	/ CB 81
	RES	$0,D	/ CB 82
	RES	$0,E	/ CB 83
	RES	$0,H	/ CB 84
	RES	$0,L	/ CB 85
	RES	$0,(HL)	/ CB 86
	RES	$0,A	/ CB 87
	RES	$1,B	/ CB 88
	RES	$1,C	/ CB 89
	RES	$1,D	/ CB 8A
	RES	$1,E	/ CB 8B
	RES	$1,H	/ CB 8C
	RES	$1,L	/ CB 8D
	RES	$1,(HL)	/ CB 8E
	RES	$1,A	/ CB 8F
	RES	$2,B	/ CB 90
	RES	$2,C	/ CB 91
	RES	$2,D	/ CB 92
	RES	$2,E	/ CB 93
	RES	$2,H	/ CB 94
	RES	$2,L	/ CB 95
	RES	$2,(HL)	/ CB 96
	RES	$2,A	/ CB 97
	RES	$3,B	/ CB 98
	RES	$3,C	/ CB 99
	RES	$3,D	/ CB 9A
	RES	$3,E	/ CB 9B
	RES	$3,H	/ CB 9C
	RES	$3,L	/ CB 9D
	RES	$3,(HL)	/ CB 9E
	RES	$3,A	/ CB 9F
	RES	$4,B	/ CB A0
	RES	$4,C	/ CB A1
	RES	$4,D	/ CB A2
	RES	$4,E	/ CB A3
	RES	$4,H	/ CB A4
	RES	$4,L	/ CB A5
	RES	$4,(HL)	/ CB A6
	RES	$4,A	/ CB A7
	RES	$5,B	/ CB A8
	RES	$5,C	/ CB A9
	RES	$5,D	/ CB AA
	RES	$5,E	/ CB AB
	RES	$5,H	/ CB AC
	RES	$5,L	/ CB AD
	RES	$5,(HL)	/ CB AE
	RES	$5,A	/ CB AF
	RES	$6,B	/ CB B0
	RES	$6,C	/ CB B1
	RES	$6,D	/ CB B2
	RES	$6,E	/ CB B3
	RES	$6,H	/ CB B4
	RES	$6,L	/ CB B5
	RES	$6,(HL)	/ CB B6
	RES	$6,A	/ CB B7
	RES	$7,B	/ CB B8
	RES	$7,C	/ CB B9
	RES	$7,D	/ CB BA
	RES	$7,E	/ CB BB
	RES	$7,H	/ CB BC
	RES	$7,L	/ CB BD
	RES	$7,(HL)	/ CB BE
	RES	$7,A	/ CB BF
	SET	$0,B	/ CB C0
	SET	$0,C	/ CB C1
	SET	$0,D	/ CB C2
	SET	$0,E	/ CB C3
	SET	$0,H	/ CB C4
	SET	$0,L	/ CB C5
	SET	$0,(HL)	/ CB C6
	SET	$0,A	/ CB C7
	SET	$1,B	/ CB C8
	SET	$1,C	/ CB C9
	SET	$1,D	/ CB CA
	SET	$1,E	/ CB CB
	SET	$1,H	/ CB CC
	SET	$1,L	/ CB CD
	SET	$1,(HL)	/ CB CE
	SET	$1,A	/ CB CF
	SET	$2,B	/ CB D0
	SET	$2,C	/ CB D1
	SET	$2,D	/ CB D2
	SET	$2,E	/ CB D3
	SET	$2,H	/ CB D4
	SET	$2,L	/ CB D5
	SET	$2,(HL)	/ CB D6
	SET	$2,A	/ CB D7
	SET	$3,B	/ CB D8
	SET	$3,C	/ CB D9
	SET	$3,D	/ CB DA
	SET	$3,E	/ CB DB
	SET	$3,H	/ CB DC
	SET	$3,L	/ CB DD
	SET	$3,(HL)	/ CB DE
	SET	$3,A	/ CB DF
	SET	$4,B	/ CB E0
	SET	$4,C	/ CB E1
	SET	$4,D	/ CB E2
	SET	$4,E	/ CB E3
	SET	$4,H	/ CB E4
	SET	$4,L	/ CB E5
	SET	$4,(HL)	/ CB E6
	SET	$4,A	/ CB E7
	SET	$5,B	/ CB E8
	SET	$5,C	/ CB E9
	SET	$5,D	/ CB EA
	SET	$5,E	/ CB EB
	SET	$5,H	/ CB EC
	SET	$5,L	/ CB ED
	SET	$5,(HL)	/ CB EE
	SET	$5,A	/ CB EF
	SET	$6,B	/ CB F0
	SET	$6,C	/ CB F1
	SET	$6,D	/ CB F2
	SET	$6,E	/ CB F3
	SET	$6,H	/ CB F4
	SET	$6,L	/ CB F5
	SET	$6,(HL)	/ CB F6
	SET	$6,A	/ CB F7
	SET	$7,B	/ CB F8
	SET	$7,C	/ CB F9
	SET	$7,D	/ CB FA
	SET	$7,E	/ CB FB
	SET	$7,H	/ CB FC
	SET	$7,L	/ CB FD
	SET	$7,(HL)	/ CB FE
	SET	$7,A	/ CB FF
	CALL	Z,$16384	/ CC 00 40
	CALL	$16384	/ CD 00 40
	ADC	A,$64	/ CE 40
	RST	$8	/ CF
	RET	NC	/ D0
	POP	DE	/ D1
	JP	NC,$16384	/ D2 00 40
	OUT	(153),A	/ D3 99
	CALL	NC,$32768	/ D4 00 80
	PUSH	DE	/ D5
	SUB	A,$32	/ D6 20
	RST	$16	/ D7
	RET	C	/ D8
	EXX		/ D9
	JP	C,$16384	/ DA 00 40
	IN	A,(153)	/ DB 99
	CALL	C,$32768	/ DC 00 80
	ADD	IX,BC	/ DD 09
	ADD	IX,DE	/ DD 19
	LD	IX,$64	/ DD 21 40 00
	LD	(32768),IX	/ DD 22 00 80
	INC	IX	/ DD 23
	INC	IXH	/ DD 24
	DEC	IXH	/ DD 25
	LD	IXH,$32	/ DD 26 20
	ADD	IX,IX	/ DD 29
	LD	IX,(16384)	/ DD 2A 00 40
	DEC	IX	/ DD 2B
	INC	IXL	/ DD 2C
	DEC	IXL	/ DD 2D
	LD	IXL,$64	/ DD 2E 40
	INC	(IX + 32)	/ DD 34 20
	DEC	(IX + 16)	/ DD 35 10
	LD	(IX + 64),$128	/ DD 36 40 80
	ADD	IX,SP	/ DD 39
	LD	B,IXH	/ DD 44
	LD	B,IXL	/ DD 45
	LD	B,(IX + 32)	/ DD 46 20
	LD	C,IXH	/ DD 4C
	LD	C,IXL	/ DD 4D
	LD	C,(IX + 48)	/ DD 4E 30
	LD	D,IXH	/ DD 54
	LD	D,IXL	/ DD 55
	LD	D,(IX + 32)	/ DD 56 20
	LD	E,IXH	/ DD 5C
	LD	E,IXL	/ DD 5D
	LD	E,(IX + 64)	/ DD 5E 40
	LD	IXH,B	/ DD 60
	LD	IXH,C	/ DD 61
	LD	IXH,D	/ DD 62
	LD	IXH,E	/ DD 63
	LD	IXH,IXH	/ DD 64
	LD	IXH,IXL	/ DD 65
	LD	H,(IX + 16)	/ DD 66 10
	LD	IXH,A	/ DD 67
	LD	IXL,B	/ DD 68
	LD	IXL,C	/ DD 69
	LD	IXL,D	/ DD 6A
	LD	IXL,E	/ DD 6B
	LD	IXL,IXH	/ DD 6C
	LD	IXL,IXL	/ DD 6D
	LD	L,(IX + 48)	/ DD 6E 30
	LD	IXL,A	/ DD 6F
	LD	(IX + 64),B	/ DD 70 40
	LD	(IX + 16),C	/ DD 71 10
	LD	(IX + 32),D	/ DD 72 20
	LD	(IX + 48),E	/ DD 73 30
	LD	(IX + 16),H	/ DD 74 10
	LD	(IX + 32),L	/ DD 75 20
	LD	(IX + 48),A	/ DD 77 30
	LD	A,IXH	/ DD 7C
	LD	A,IXL	/ DD 7D
	LD	A,(IX + 16)	/ DD 7E 10
	ADD	A,IXH	/ DD 84
	ADD	A,IXL	/ DD 85
	ADD	A,(IX + 32)	/ DD 86 20
	ADC	A,IXH	/ DD 8C
	ADC	A,IXL	/ DD 8D
	ADC	A,(IX + 48)	/ DD 8E 30
	SUB	A,IXH	/ DD 94
	SUB	A,IXL	/ DD 95
	SUB	A,(IX + 16)	/ DD 96 10
	SBC	A,IXH	/ DD 9C
	SBC	A,IXL	/ DD 9D
	SBC	A,(IX + 32)	/ DD 9E 20
	AND	A,IXH	/ DD A4
	AND	A,IXL	/ DD A5
	AND	A,(IX + 48)	/ DD A6 30
	XOR	A,IXH	/ DD AC
	XOR	A,IXL	/ DD AD
	XOR	A,(IX + 16)	/ DD AE 10
	OR	A,IXH	/ DD B4
	OR	A,IXL	/ DD B5
	OR	A,(IX + 32)	/ DD B6 20
	CP	A,IXH	/ DD BC
	CP	A,IXL	/ DD BD
	CP	A,(IX + 48)	/ DD BE 30
	RLC	(IX + 16),B	/ DD CB 10 00
	RLC	(IX + 32),C	/ DD CB 20 01
	RLC	(IX + 48),D	/ DD CB 30 02
	RLC	(IX + 64),E	/ DD CB 40 03
	RLC	(IX + 16),H	/ DD CB 10 04
	RLC	(IX + 32),L	/ DD CB 20 05
	RLC	(IX + 64)	/ DD CB 40 06
	RLC	(IX + 16),A	/ DD CB 10 07
	RRC	(IX + 32),B 	/ DD CB 20 08
	RRC	(IX + 48),C	/ DD CB 30 09
	RRC	(IX + 64),D	/ DD CB 40 0A
	RRC	(IX + 16),E	/ DD CB 10 0B
	RRC	(IX + 32),H	/ DD CB 20 0C
	RRC	(IX + 48),L	/ DD CB 30 0D
	RRC	(IX + 16)	/ DD CB 10 0E
	RRC	(IX + 16),A	/ DD CB 10 0F
	RL	(IX + 32),B	/ DD CB 20 10
	RL	(IX + 48),C	/ DD CB 30 11
	RL	(IX + 64),D	/ DD CB 40 12
	RL	(IX + 16),E	/ DD CB 10 13
	RL	(IX + 32),H	/ DD CB 20 14
	RL	(IX + 48),L	/ DD CB 30 15
	RL	(IX + 32)	/ DD CB 20 16
	RL	(IX + 16),A	/ DD CB 10 17
	RR	(IX + 32),B	/ DD CB 20 18
	RR	(IX + 48),C	/ DD CB 30 19
	RR	(IX + 64),D	/ DD CB 40 1A
	RR	(IX + 16),E	/ DD CB 10 1B
	RR	(IX + 32),H	/ DD CB 20 1C
	RR	(IX + 48),L	/ DD CB 30 1D
	RR	(IX + 48)	/ DD CB 30 1E
	RR	(IX + 16),A	/ DD CB 10 1F
	SLA	(IX + 32),B	/ DD CB 20 20
	SLA	(IX + 48),C	/ DD CB 30 21
	SLA	(IX + 64),D	/ DD CB 40 22
	SLA	(IX + 16),E	/ DD CB 10 23
	SLA	(IX + 32),H	/ DD CB 20 24
	SLA	(IX + 48),L	/ DD CB 30 25
	SLA	(IX + 64)	/ DD CB 40 26
	SLA	(IX + 16),A	/ DD CB 10 27
	SRA	(IX + 32),B	/ DD CB 20 28
	SRA	(IX + 48),C	/ DD CB 30 29
	SRA	(IX + 64),D	/ DD CB 40 2A
	SRA	(IX + 16),E	/ DD CB 10 2B
	SRA	(IX + 32),H	/ DD CB 20 2C
	SRA	(IX + 48),L	/ DD CB 30 2D
	SRA	(IX + 16)	/ DD CB 10 2E
	SRA	(IX + 16),A	/ DD CB 10 2F
	SLL	(IX + 32),B	/ DD CB 20 30
	SLL	(IX + 48),C	/ DD CB 30 31
	SLL	(IX + 64),D	/ DD CB 40 32
	SLL	(IX + 16),E	/ DD CB 10 33
	SLL	(IX + 32),H	/ DD CB 20 34
	SLL	(IX + 48),L	/ DD CB 30 35
	SLL	(IX + 32)	/ DD CB 20 36
	SLL	(IX + 16),A	/ DD CB 10 37
	SRL	(IX + 32),B	/ DD CB 20 38
	SRL	(IX + 48),C	/ DD CB 30 39
	SRL	(IX + 64),D	/ DD CB 40 3A
	SRL	(IX + 16),E	/ DD CB 10 3B
	SRL	(IX + 32),H	/ DD CB 20 3C
	SRL	(IX + 48),L	/ DD CB 30 3D
	SRL	(IX + 48)	/ DD CB 30 3E
	SRL	(IX + 64),A	/ DD CB 40 3F
	BIT	$0,(IX + 16)	/ DD CB 10 46
	BIT	$1,(IX + 32)	/ DD CB 20 4E
	BIT	$2,(IX + 48)	/ DD CB 30 56
	BIT	$3,(IX + 64)	/ DD CB 40 5E
	BIT	$4,(IX + 16)	/ DD CB 10 66
	BIT	$5,(IX + 32)	/ DD CB 20 6E
	BIT	$6,(IX + 48)	/ DD CB 30 76
	BIT	$7,(IX + 64)	/ DD CB 40 7E
	RES	$0,(IX + 16),B	/ DD CB 10 80
	RES	$0,(IX + 32),C	/ DD CB 20 81
	RES	$0,(IX + 48),D	/ DD CB 30 82
	RES	$0,(IX + 64),E	/ DD CB 40 83
	RES	$0,(IX + 16),H	/ DD CB 10 84
	RES	$0,(IX + 32),L	/ DD CB 20 85
	RES	$0,(IX + 48)	/ DD CB 30 86
	RES	$0,(IX + 64),A	/ DD CB 40 87
	RES	$1,(IX + 16),B	/ DD CB 10 88
	RES	$1,(IX + 32),C	/ DD CB 20 89
	RES	$1,(IX + 48),D	/ DD CB 30 8A
	RES	$1,(IX + 64),E	/ DD CB 40 8B
	RES	$1,(IX + 16),H	/ DD CB 10 8C
	RES	$1,(IX + 32),L	/ DD CB 20 8D
	RES	$1,(IX + 48)	/ DD CB 30 8E
	RES	$1,(IX + 64),A	/ DD CB 40 8F
	RES	$2,(IX + 16),B	/ DD CB 10 90
	RES	$2,(IX + 32),C	/ DD CB 20 91
	RES	$2,(IX + 48),D	/ DD CB 30 92
	RES	$2,(IX + 64),E	/ DD CB 40 93
	RES	$2,(IX + 16),H	/ DD CB 10 94
	RES	$2,(IX + 32),L	/ DD CB 20 95
	RES	$2,(IX + 48)	/ DD CB 30 96
	RES	$2,(IX + 64),A	/ DD CB 40 97
	RES	$3,(IX + 16),B	/ DD CB 10 98
	RES	$3,(IX + 32),C	/ DD CB 20 99
	RES	$3,(IX + 48),D	/ DD CB 30 9A
	RES	$3,(IX + 64),E	/ DD CB 40 9B
	RES	$3,(IX + 16),H	/ DD CB 10 9C
	RES	$3,(IX + 32),L	/ DD CB 20 9D
	RES	$3,(IX + 48)	/ DD CB 30 9E
	RES	$3,(IX + 64),A	/ DD CB 40 9F
	RES	$4,(IX + 16),B	/ DD CB 10 A0
	RES	$4,(IX + 32),C	/ DD CB 20 A1
	RES	$4,(IX + 48),D	/ DD CB 30 A2
	RES	$4,(IX + 64),E	/ DD CB 40 A3
	RES	$4,(IX + 16),H	/ DD CB 10 A4
	RES	$4,(IX + 32),L	/ DD CB 20 A5
	RES	$4,(IX + 48)	/ DD CB 30 A6
	RES	$4,(IX + 64),A	/ DD CB 40 A7
	RES	$5,(IX + 16),B	/ DD CB 10 A8
	RES	$5,(IX + 32),C	/ DD CB 20 A9
	RES	$5,(IX + 48),D	/ DD CB 30 AA
	RES	$5,(IX + 64),E	/ DD CB 40 AB
	RES	$5,(IX + 16),H	/ DD CB 10 AC
	RES	$5,(IX + 32),L	/ DD CB 20 AD
	RES	$5,(IX + 48)	/ DD CB 30 AE
	RES	$5,(IX + 64),A	/ DD CB 40 AF
	RES	$6,(IX + 16),B	/ DD CB 10 B0
	RES	$6,(IX + 32),C	/ DD CB 20 B1
	RES	$6,(IX + 48),D	/ DD CB 30 B2
	RES	$6,(IX + 64),E	/ DD CB 40 B3
	RES	$6,(IX + 16),H	/ DD CB 10 B4
	RES	$6,(IX + 32),L	/ DD CB 20 B5
	RES	$6,(IX + 48)	/ DD CB 30 B6
	RES	$6,(IX + 64),A	/ DD CB 40 B7
	RES	$7,(IX + 16),B	/ DD CB 10 B8
	RES	$7,(IX + 32),C	/ DD CB 20 B9
	RES	$7,(IX + 48),D	/ DD CB 30 BA
	RES	$7,(IX + 64),E	/ DD CB 40 BB
	RES	$7,(IX + 16),H	/ DD CB 10 BC
	RES	$7,(IX + 32),L	/ DD CB 20 BD
	RES	$7,(IX + 48)	/ DD CB 30 BE
	RES	$7,(IX + 64),A	/ DD CB 40 BF
	SET	$0,(IX + 16),B	/ DD CB 10 C0
	SET	$0,(IX + 32),C	/ DD CB 20 C1
	SET	$0,(IX + 48),D	/ DD CB 30 C2
	SET	$0,(IX + 64),E	/ DD CB 40 C3
	SET	$0,(IX + 16),H	/ DD CB 10 C4
	SET	$0,(IX + 32),L	/ DD CB 20 C5
	SET	$0,(IX + 48)	/ DD CB 30 C6
	SET	$0,(IX + 64),A	/ DD CB 40 C7
	SET	$1,(IX + 16),B	/ DD CB 10 C8
	SET	$1,(IX + 32),C	/ DD CB 20 C9
	SET	$1,(IX + 48),D	/ DD CB 30 CA
	SET	$1,(IX + 64),E	/ DD CB 40 CB
	SET	$1,(IX + 16),H	/ DD CB 10 CC
	SET	$1,(IX + 32),L	/ DD CB 20 CD
	SET	$1,(IX + 48)	/ DD CB 30 CE
	SET	$1,(IX + 64),A	/ DD CB 40 CF
	SET	$2,(IX + 16),B	/ DD CB 10 D0
	SET	$2,(IX + 32),C	/ DD CB 20 D1
	SET	$2,(IX + 48),D	/ DD CB 30 D2
	SET	$2,(IX + 64),E	/ DD CB 40 D3
	SET	$2,(IX + 16),H	/ DD CB 10 D4
	SET	$2,(IX + 32),L	/ DD CB 20 D5
	SET	$2,(IX + 48)	/ DD CB 30 D6
	SET	$2,(IX + 64),A	/ DD CB 40 D7
	SET	$3,(IX + 16),B	/ DD CB 10 D8
	SET	$3,(IX + 32),C	/ DD CB 20 D9
	SET	$3,(IX + 48),D	/ DD CB 30 DA
	SET	$3,(IX + 64),E	/ DD CB 40 DB
	SET	$3,(IX + 16),H	/ DD CB 10 DC
	SET	$3,(IX + 32),L	/ DD CB 20 DD
	SET	$3,(IX + 48)	/ DD CB 30 DE
	SET	$3,(IX + 64),A	/ DD CB 40 DF
	SET	$4,(IX + 16),B	/ DD CB 10 E0
	SET	$4,(IX + 32),C	/ DD CB 20 E1
	SET	$4,(IX + 48),D	/ DD CB 30 E2
	SET	$4,(IX + 64),E	/ DD CB 40 E3
	SET	$4,(IX + 16),H	/ DD CB 10 E4
	SET	$4,(IX + 32),L	/ DD CB 20 E5
	SET	$4,(IX + 48)	/ DD CB 30 E6
	SET	$4,(IX + 64),A	/ DD CB 40 E7
	SET	$5,(IX + 16),B	/ DD CB 10 E8
	SET	$5,(IX + 32),C	/ DD CB 20 E9
	SET	$5,(IX + 48),D	/ DD CB 30 EA
	SET	$5,(IX + 64),E	/ DD CB 40 EB
	SET	$5,(IX + 16),H	/ DD CB 10 EC
	SET	$5,(IX + 32),L	/ DD CB 20 ED
	SET	$5,(IX + 48)	/ DD CB 30 EE
	SET	$5,(IX + 64),A	/ DD CB 40 EF
	SET	$6,(IX + 16),B	/ DD CB 10 F0
	SET	$6,(IX + 32),C	/ DD CB 20 F1
	SET	$6,(IX + 48),D	/ DD CB 30 F2
	SET	$6,(IX + 64),E	/ DD CB 40 F3
	SET	$6,(IX + 16),H	/ DD CB 10 F4
	SET	$6,(IX + 32),L	/ DD CB 20 F5
	SET	$6,(IX + 48)	/ DD CB 30 F6
	SET	$6,(IX + 64),A	/ DD CB 40 F7
	SET	$7,(IX + 16),B	/ DD CB 10 F8
	SET	$7,(IX + 32),C	/ DD CB 20 F9
	SET	$7,(IX + 48),D	/ DD CB 30 FA
	SET	$7,(IX + 64),E	/ DD CB 40 FB
	SET	$7,(IX + 16),H	/ DD CB 10 FC
	SET	$7,(IX + 32),L	/ DD CB 20 FD
	SET	$7,(IX + 48)	/ DD CB 30 FE
	SET	$7,(IX + 64),A	/ DD CB 40 FF
	POP	IX	/ DD E1
	EX	(SP),IX	/ DD E3
	PUSH	IX	/ DD E5
	JP	(IX)	/ DD E9
	LD	SP,IX	/ DD F9
	SBC	A,$64	/ DE 40
	RST	$24	/ DF
	RET	PO	/ E0
	POP	HL	/ E1
	JP	PO,$32768	/ E2 00 80
	EX	(SP),HL	/ E3
	CALL	PO,$16384	/ E4 00 40
	PUSH	HL	/ E5
	AND	A,$16	/ E6 10
	RST	$32	/ E7
	RET	PE	/ E8
	JP	(HL)	/ E9
	JP	PE,$16384	/ EA 00 40
	EX	DE,HL	/ EB
	CALL	PE,$16384	/ EC 00 40
	IN	B,(C)	/ ED 40
	OUT	(C),B	/ ED 41
	SBC	HL,BC	/ ED 42
	LD	(32768),BC	/ ED 43 00 80
	NEG		/ ED 44
	RETN		/ ED 45
	IM	$0	/ ED 46
	LD	I,A	/ ED 47
	IN	C,(C)	/ ED 48
	OUT	(C),C	/ ED 49
	ADC	HL,BC	/ ED 4A
	LD	BC,(16384)	/ ED 4B 00 40
	RETI		/ ED 4D
	LD	R,A	/ ED 4F
	IN	D,(C)	/ ED 50
	OUT	(C),D	/ ED 51
	SBC	HL,DE	/ ED 52
	LD	(32768),DE	/ ED 53 00 80
	IM	$1	/ ED 56
	LD	A,I	/ ED 57
	IN	E,(C)	/ ED 58
	OUT	(C),E	/ ED 59
	ADC	HL,DE	/ ED 5A
	LD	DE,(16384)	/ ED 5B 00 40
	IM	$2	/ ED 5E
	LD	A,R	/ ED 5F
	IN	H,(C)	/ ED 60
	OUT	(C),H	/ ED 61
	SBC	HL,HL	/ ED 62
	RRD		/ ED 67
	IN	L,(C)	/ ED 68
	OUT	(C),L	/ ED 69
	ADC	HL,HL	/ ED 6A
	RLD		/ ED 6F
	IN	F,(C)	/ ED 70
	OUT	(C),$0	/ ED 71
	SBC	HL,SP	/ ED 72
	LD	(16384),SP	/ ED 73 00 40
	IN	A,(C)	/ ED 78
	OUT	(C),A	/ ED 79
	ADC	HL,SP	/ ED 7A
	LD	SP,(32768)	/ ED 7B 00 80
	LDI		/ ED A0
	CPI		/ ED A1
	INI		/ ED A2
	OUTI		/ ED A3
	LDD		/ ED A8
	CPD		/ ED A9
	IND		/ ED AA
	OUTD		/ ED AB
	LDIR		/ ED B0
	CPIR		/ ED B1
	INIR		/ ED B2
	OTIR		/ ED B3
	LDDR		/ ED B8
	CPDR		/ ED B9
	INDR		/ ED BA
	OTDR		/ ED BB
	XOR	A,$32	/ EE 20
	RST	$40	/ EF
	RET	P	/ F0
	POP	AF	/ F1
	JP	P,$32768	/ F2 00 80
	DI		/ F3
	CALL	P,$16384	/ F4 00 40
	PUSH	AF	/ F5
	OR	A,$32	/ F6 20
	RST	$48	/ F7
	RET	M	/ F8
	LD	SP,HL	/ F9
	JP	M,$32768	/ FA 00 80
	EI		/ FB
	CALL	M,$16384	/ FC 00 40
	ADD	IY,BC	/ FD 09
	ADD	IY,DE	/ FD 19
	LD	IY,$32768	/ FD 21 00 80
	LD	(16384),IY	/ FD 22 00 40
	INC	IY	/ FD 23
	INC	IYH	/ FD 24
	DEC	IYH	/ FD 25
	LD	IYH,$32	/ FD 26 20
	ADD	IY,IY	/ FD 29
	LD	IY,(32768)	/ FD 2A 00 80
	DEC	IY	/ FD 2B
	INC	IYL	/ FD 2C
	DEC	IYL	/ FD 2D
	LD	IYL,$16	/ FD 2E 10
	INC	(IY + 32)	/ FD 34 20
	DEC	(IY + 48)	/ FD 35 30
	LD	(IY + 64),$16	/ FD 36 40 10
	ADD	IY,SP	/ FD 39
	LD	B,IYH	/ FD 44
	LD	B,IYL	/ FD 45
	LD	B,(IY + 32)	/ FD 46 20
	LD	C,IYH	/ FD 4C
	LD	C,IYL	/ FD 4D
	LD	C,(IY + 48)	/ FD 4E 30
	LD	D,IYH	/ FD 54
	LD	D,IYL	/ FD 55
	LD	D,(IY + 64)	/ FD 56 40
	LD	E,IYH	/ FD 5C
	LD	E,IYL	/ FD 5D
	LD	E,(IY + 16)	/ FD 5E 10
	LD	IYH,B	/ FD 60
	LD	IYH,C	/ FD 61
	LD	IYH,D	/ FD 62
	LD	IYH,E	/ FD 63
	LD	IYH,IYH	/ FD 64
	LD	IYH,IYL	/ FD 65
	LD	H,(IY + 32)	/ FD 66 20
	LD	IYH,A	/ FD 67
	LD	IYL,B	/ FD 68
	LD	IYL,C	/ FD 69
	LD	IYL,D	/ FD 6A
	LD	IYL,E	/ FD 6B
	LD	IYL,IYH	/ FD 6C
	LD	IYL,IYL	/ FD 6D
	LD	L,(IY + 48)	/ FD 6E 30
	LD	IYL,A	/ FD 6F
	LD	(IY + 64),B	/ FD 70 40
	LD	(IY + 16),C	/ FD 71 10
	LD	(IY + 32),D	/ FD 72 20
	LD	(IY + 48),E	/ FD 73 30
	LD	(IY + 64),H	/ FD 74 40
	LD	(IY + 16),L	/ FD 75 10
	LD	(IY + 32),A	/ FD 77 20
	LD	A,IYH	/ FD 7C
	LD	A,IYL	/ FD 7D
	LD	A,(IY + 48)	/ FD 7E 30
	ADD	A,IYH	/ FD 84
	ADD	A,IYL	/ FD 85
	ADD	A,(IY + 64)	/ FD 86 40
	ADC	A,IYH	/ FD 8C
	ADC	A,IYL	/ FD 8D
	ADC	A,(IY + 16)	/ FD 8E 10
	SUB	A,IYH	/ FD 94
	SUB	A,IYL	/ FD 95
	SUB	A,(IY + 32)	/ FD 96 20
	SBC	A,IYH	/ FD 9C
	SBC	A,IYL	/ FD 9D
	SBC	A,(IY + 48)	/ FD 9E 30
	AND	A,IYH	/ FD A4
	AND	A,IYL	/ FD A5
	AND	A,(IY + 64)	/ FD A6 40
	XOR	A,IYH	/ FD AC
	XOR	A,IYL	/ FD AD
	XOR	A,(IY + 16)	/ FD AE 10
	OR	A,IYH	/ FD B4
	OR	A,IYL	/ FD B5
	OR	A,(IY + 32)	/ FD B6 20
	CP	A,IYH	/ FD BC
	CP	A,IYL	/ FD BD
	CP	A,(IY + 48)	/ FD BE 30
	RLC	(IY + 16),B	/ FD CB 10 00
	RLC	(IY + 32),C	/ FD CB 20 01
	RLC	(IY + 48),D	/ FD CB 30 02
	RLC	(IY + 64),E	/ FD CB 40 03
	RLC	(IY + 16),H	/ FD CB 10 04
	RLC	(IY + 32),L	/ FD CB 20 05
	RLC	(IY + 16)	/ FD CB 10 06
	RLC	(IY + 32),A	/ FD CB 20 07
	RRC	(IY + 48),B	/ FD CB 30 08
	RRC	(IY + 64),C	/ FD CB 40 09
	RRC	(IY + 16),D	/ FD CB 10 0A
	RRC	(IY + 32),E	/ FD CB 20 0B
	RRC	(IY + 48),H	/ FD CB 30 0C
	RRC	(IY + 64),L	/ FD CB 40 0D
	RRC	(IY + 32)	/ FD CB 20 0E
	RRC	(IY + 16),A	/ FD CB 10 0F
	RL	(IY + 32),B	/ FD CB 20 10
	RL	(IY + 48),C	/ FD CB 30 11
	RL	(IY + 64),D	/ FD CB 40 12
	RL	(IY + 16),E	/ FD CB 10 13
	RL	(IY + 32),H	/ FD CB 20 14
	RL	(IY + 48),L	/ FD CB 30 15
	RL	(IY + 48)	/ FD CB 30 16
	RL	(IY + 16),A	/ FD CB 10 17
	RR	(IY + 32),B	/ FD CB 20 18
	RR	(IY + 48),C	/ FD CB 30 19
	RR	(IY + 64),D	/ FD CB 40 1A
	RR	(IY + 16),E	/ FD CB 10 1B
	RR	(IY + 32),H	/ FD CB 20 1C
	RR	(IY + 48),L	/ FD CB 30 1D
	RR	(IY + 64)	/ FD CB 40 1E
	RR	(IY + 16),A	/ FD CB 10 1F
	SLA	(IY + 32),B	/ FD CB 20 20
	SLA	(IY + 48),C	/ FD CB 30 21
	SLA	(IY + 64),D	/ FD CB 40 22
	SLA	(IY + 16),E	/ FD CB 10 23
	SLA	(IY + 32),H	/ FD CB 20 24
	SLA	(IY + 48),L	/ FD CB 30 25
	SLA	(IY + 16)	/ FD CB 10 26
	SLA	(IY + 16),A	/ FD CB 10 27
	SRA	(IY + 32),B	/ FD CB 20 28
	SRA	(IY + 48),C	/ FD CB 30 29
	SRA	(IY + 64),D	/ FD CB 40 2A
	SRA	(IY + 16),E	/ FD CB 10 2B
	SRA	(IY + 32),H	/ FD CB 20 2C
	SRA	(IY + 48),L	/ FD CB 30 2D
	SRA	(IY + 32)	/ FD CB 20 2E
	SRA	(IY + 48),A	/ FD CB 30 2F
	SLL	(IY + 16),B	/ FD CB 10 30
	SLL	(IY + 32),C	/ FD CB 20 31
	SLL	(IY + 48),D	/ FD CB 30 32
	SLL	(IY + 64),E	/ FD CB 40 33
	SLL	(IY + 16),H	/ FD CB 10 34
	SLL	(IY + 32),L	/ FD CB 20 35
	SLL	(IY + 48)	/ FD CB 30 36
	SLL	(IY + 16),A	/ FD CB 10 37
	SRL	(IY + 32),B	/ FD CB 20 38
	SRL	(IY + 48),C	/ FD CB 30 39
	SRL	(IY + 64),D	/ FD CB 40 3A
	SRL	(IY + 16),E	/ FD CB 10 3B
	SRL	(IY + 32),H	/ FD CB 20 3C
	SRL	(IY + 48),L	/ FD CB 30 3D
	SRL	(IY + 64)	/ FD CB 40 3E
	SRL	(IY + 16),A	/ FD CB 10 3F
	BIT	$0,(IY + 16)	/ FD CB 10 46
	BIT	$1,(IY + 32)	/ FD CB 20 4E
	BIT	$2,(IY + 48)	/ FD CB 30 56
	BIT	$3,(IY + 64)	/ FD CB 40 5E
	BIT	$4,(IY + 16)	/ FD CB 10 66
	BIT	$5,(IY + 32)	/ FD CB 20 6E
	BIT	$6,(IY + 48)	/ FD CB 30 76
	BIT	$7,(IY + 64)	/ FD CB 40 7E
	RES	$0,(IY + 16),B	/ FD CB 10 80
	RES	$0,(IY + 32),C	/ FD CB 20 81
	RES	$0,(IY + 48),D	/ FD CB 30 82
	RES	$0,(IY + 64),E	/ FD CB 40 83
	RES	$0,(IY + 16),H	/ FD CB 10 84
	RES	$0,(IY + 32),L	/ FD CB 20 85
	RES	$0,(IY + 48)	/ FD CB 30 86
	RES	$0,(IY + 64),A	/ FD CB 40 87
	RES	$1,(IY + 16),B	/ FD CB 10 88
	RES	$1,(IY + 32),C	/ FD CB 20 89
	RES	$1,(IY + 48),D	/ FD CB 30 8A
	RES	$1,(IY + 64),E	/ FD CB 40 8B
	RES	$1,(IY + 16),H	/ FD CB 10 8C
	RES	$1,(IY + 32),L	/ FD CB 20 8D
	RES	$1,(IY + 48)	/ FD CB 30 8E
	RES	$1,(IY + 64),A	/ FD CB 40 8F
	RES	$2,(IY + 16),B	/ FD CB 10 90
	RES	$2,(IY + 32),C	/ FD CB 20 91
	RES	$2,(IY + 48),D	/ FD CB 30 92
	RES	$2,(IY + 64),E	/ FD CB 40 93
	RES	$2,(IY + 16),H	/ FD CB 10 94
	RES	$2,(IY + 32),L	/ FD CB 20 95
	RES	$2,(IY + 48)	/ FD CB 30 96
	RES	$2,(IY + 64),A	/ FD CB 40 97
	RES	$3,(IY + 16),B	/ FD CB 10 98
	RES	$3,(IY + 32),C	/ FD CB 20 99
	RES	$3,(IY + 48),D	/ FD CB 30 9A
	RES	$3,(IY + 64),E	/ FD CB 40 9B
	RES	$3,(IY + 16),H	/ FD CB 10 9C
	RES	$3,(IY + 32),L	/ FD CB 20 9D
	RES	$3,(IY + 48)	/ FD CB 30 9E
	RES	$3,(IY + 64),A	/ FD CB 40 9F
	RES	$4,(IY + 16),B	/ FD CB 10 A0
	RES	$4,(IY + 32),C	/ FD CB 20 A1
	RES	$4,(IY + 48),D	/ FD CB 30 A2
	RES	$4,(IY + 64),E	/ FD CB 40 A3
	RES	$4,(IY + 16),H	/ FD CB 10 A4
	RES	$4,(IY + 32),L	/ FD CB 20 A5
	RES	$4,(IY + 48)	/ FD CB 30 A6
	RES	$4,(IY + 64),A	/ FD CB 40 A7
	RES	$5,(IY + 16),B	/ FD CB 10 A8
	RES	$5,(IY + 32),C	/ FD CB 20 A9
	RES	$5,(IY + 48),D	/ FD CB 30 AA
	RES	$5,(IY + 64),E	/ FD CB 40 AB
	RES	$5,(IY + 16),H	/ FD CB 10 AC
	RES	$5,(IY + 32),L	/ FD CB 20 AD
	RES	$5,(IY + 48)	/ FD CB 30 AE
	RES	$5,(IY + 64),A	/ FD CB 40 AF
	RES	$6,(IY + 16),B	/ FD CB 10 B0
	RES	$6,(IY + 32),C	/ FD CB 20 B1
	RES	$6,(IY + 48),D	/ FD CB 30 B2
	RES	$6,(IY + 64),E	/ FD CB 40 B3
	RES	$6,(IY + 16),H	/ FD CB 10 B4
	RES	$6,(IY + 32),L	/ FD CB 20 B5
	RES	$6,(IY + 48)	/ FD CB 30 B6
	RES	$6,(IY + 64),A	/ FD CB 40 B7
	RES	$7,(IY + 16),B	/ FD CB 10 B8
	RES	$7,(IY + 32),C	/ FD CB 20 B9
	RES	$7,(IY + 48),D	/ FD CB 30 BA
	RES	$7,(IY + 64),E	/ FD CB 40 BB
	RES	$7,(IY + 16),H	/ FD CB 10 BC
	RES	$7,(IY + 32),L	/ FD CB 20 BD
	RES	$7,(IY + 48)	/ FD CB 30 BE
	RES	$7,(IY + 64),A	/ FD CB 40 BF
	SET	$0,(IY + 16),B	/ FD CB 10 C0
	SET	$0,(IY + 32),C	/ FD CB 20 C1
	SET	$0,(IY + 48),D	/ FD CB 30 C2
	SET	$0,(IY + 64),E	/ FD CB 40 C3
	SET	$0,(IY + 16),H	/ FD CB 10 C4
	SET	$0,(IY + 32),L	/ FD CB 20 C5
	SET	$0,(IY + 48)	/ FD CB 30 C6
	SET	$0,(IY + 64),A	/ FD CB 40 C7
	SET	$1,(IY + 16),B	/ FD CB 10 C8
	SET	$1,(IY + 32),C	/ FD CB 20 C9
	SET	$1,(IY + 48),D	/ FD CB 30 CA
	SET	$1,(IY + 64),E	/ FD CB 40 CB
	SET	$1,(IY + 16),H	/ FD CB 10 CC
	SET	$1,(IY + 32),L	/ FD CB 20 CD
	SET	$1,(IY + 48)	/ FD CB 30 CE
	SET	$1,(IY + 64),A	/ FD CB 40 CF
	SET	$2,(IY + 16),B	/ FD CB 10 D0
	SET	$2,(IY + 32),C	/ FD CB 20 D1
	SET	$2,(IY + 48),D	/ FD CB 30 D2
	SET	$2,(IY + 64),E	/ FD CB 40 D3
	SET	$2,(IY + 16),H	/ FD CB 10 D4
	SET	$2,(IY + 32),L	/ FD CB 20 D5
	SET	$2,(IY + 48)	/ FD CB 30 D6
	SET	$2,(IY + 64),A	/ FD CB 40 D7
	SET	$3,(IY + 16),B	/ FD CB 10 D8
	SET	$3,(IY + 32),C	/ FD CB 20 D9
	SET	$3,(IY + 48),D	/ FD CB 30 DA
	SET	$3,(IY + 64),E	/ FD CB 40 DB
	SET	$3,(IY + 16),H	/ FD CB 10 DC
	SET	$3,(IY + 32),L	/ FD CB 20 DD
	SET	$3,(IY + 48)	/ FD CB 30 DE
	SET	$3,(IY + 64),A	/ FD CB 40 DF
	SET	$4,(IY + 16),B	/ FD CB 10 E0
	SET	$4,(IY + 32),C	/ FD CB 20 E1
	SET	$4,(IY + 48),D	/ FD CB 30 E2
	SET	$4,(IY + 64),E	/ FD CB 40 E3
	SET	$4,(IY + 16),H	/ FD CB 10 E4
	SET	$4,(IY + 32),L	/ FD CB 20 E5
	SET	$4,(IY + 48)	/ FD CB 30 E6
	SET	$4,(IY + 64),A	/ FD CB 40 E7
	SET	$5,(IY + 16),B	/ FD CB 10 E8
	SET	$5,(IY + 32),C	/ FD CB 20 E9
	SET	$5,(IY + 48),D	/ FD CB 30 EA
	SET	$5,(IY + 64),E	/ FD CB 40 EB
	SET	$5,(IY + 16),H	/ FD CB 10 EC
	SET	$5,(IY + 32),L	/ FD CB 20 ED
	SET	$5,(IY + 48)	/ FD CB 30 EE
	SET	$5,(IY + 64),A	/ FD CB 40 EF
	SET	$6,(IY + 16),B	/ FD CB 10 F0
	SET	$6,(IY + 32),C	/ FD CB 20 F1
	SET	$6,(IY + 48),D	/ FD CB 30 F2
	SET	$6,(IY + 64),E	/ FD CB 40 F3
	SET	$6,(IY + 16),H	/ FD CB 10 F4
	SET	$6,(IY + 32),L	/ FD CB 20 F5
	SET	$6,(IY + 48)	/ FD CB 30 F6
	SET	$6,(IY + 64),A	/ FD CB 40 F7
	SET	$7,(IY + 16),B	/ FD CB 10 F8
	SET	$7,(IY + 32),C	/ FD CB 20 F9
	SET	$7,(IY + 48),D	/ FD CB 30 FA
	SET	$7,(IY + 64),E	/ FD CB 40 FB
	SET	$7,(IY + 16),H	/ FD CB 10 FC
	SET	$7,(IY + 32),L	/ FD CB 20 FD
	SET	$7,(IY + 48)	/ FD CB 30 FE
	SET	$7,(IY + 64),A	/ FD CB 40 FF
	POP	IY	/ FD E1
	EX	(SP),IY	/ FD E3
	PUSH	IY	/ FD E5
	JP	(IY)	/ FD E9
	LD	SP,IY	/ FD F9
	CP	A,$32	/ FE 20
	RST	$56	/ FF
