/ Look in http://www.mlsite.net/8086/
/ Look in http://www.srmuniv.ac.in/sites/default/files/2017/15CS205J-unitII.pdf
/addb reg8,reg8
	ADDB	%AL,%AL			/ 00 C0
	ADDB	%CL,%AL			/ 00 C8
	ADDB	%DL,%AL			/ 00 D0
	ADDB	%BL,%AL			/ 00 D8
	ADDB	%AH,%AL			/ 00 E0
	ADDB	%CH,%AL			/ 00 E8
	ADDB	%DH,%AL			/ 00 F0
	ADDB	%BH,%AL			/ 00 F8

	ADDB	%AL,%CL			/ 00 C1
	ADDB	%CL,%CL			/ 00 C9
	ADDB	%DL,%CL			/ 00 D1
	ADDB	%BL,%CL			/ 00 D9
	ADDB	%AH,%CL			/ 00 E1
	ADDB	%CH,%CL			/ 00 E9
	ADDB	%DH,%CL			/ 00 F1
	ADDB	%BH,%CL			/ 00 F9

	ADDB	%AL,%DL			/ 00 C2
	ADDB	%CL,%DL			/ 00 CA
	ADDB	%DL,%DL			/ 00 D2
	ADDB	%BL,%DL			/ 00 DA
	ADDB	%AH,%DL			/ 00 E2
	ADDB	%CH,%DL			/ 00 EA
	ADDB	%DH,%DL			/ 00 F2
	ADDB	%BH,%DL			/ 00 FA

	ADDB	%AL,%BL			/ 00 C3
	ADDB	%CL,%BL			/ 00 CB
	ADDB	%DL,%BL			/ 00 D3
	ADDB	%BL,%BL			/ 00 DB
	ADDB	%AH,%BL			/ 00 E3
	ADDB	%CH,%BL			/ 00 EB
	ADDB	%DH,%BL			/ 00 F3
	ADDB	%BH,%BL			/ 00 FB

	ADDB	%AL,%AH			/ 00 C4
	ADDB	%CL,%AH			/ 00 CC
	ADDB	%DL,%AH			/ 00 D4
	ADDB	%BL,%AH			/ 00 DC
	ADDB	%AH,%AH			/ 00 E4
	ADDB	%CH,%AH			/ 00 EC
	ADDB	%DH,%AH			/ 00 F4
	ADDB	%BH,%AH			/ 00 FC

	ADDB	%AL,%CH			/ 00 C5
	ADDB	%CL,%CH			/ 00 CD
	ADDB	%DL,%CH			/ 00 D5
	ADDB	%BL,%CH			/ 00 DD
	ADDB	%AH,%CH			/ 00 E5
	ADDB	%CH,%CH			/ 00 ED
	ADDB	%DH,%CH			/ 00 F5
	ADDB	%BH,%CH			/ 00 FD

	ADDB	%AL,%DH			/ 00 C6
	ADDB	%CL,%DH			/ 00 CE
	ADDB	%DL,%DH			/ 00 D6
	ADDB	%BL,%DH			/ 00 DE
	ADDB	%AH,%DH			/ 00 E6
	ADDB	%CH,%DH			/ 00 EE
	ADDB	%DH,%DH			/ 00 F6
	ADDB	%BH,%DH			/ 00 FE

	ADDB	%AL,%BH			/ 00 C7
	ADDB	%CL,%BH			/ 00 CF
	ADDB	%DL,%BH			/ 00 D7
	ADDB	%BL,%BH			/ 00 DF
	ADDB	%AH,%BH			/ 00 E7
	ADDB	%CH,%BH			/ 00 EF
	ADDB	%DH,%BH			/ 00 F7
	ADDB	%BH,%BH			/ 00 FF

/addw reg16,reg16
/	ADDW	%AX,%AX			/ 01 C0
/	ADDW	%CX,%AX			/ 01 C8
/	ADDW	%DX,%AX			/ 01 D0
/	ADDW	%BX,%AX			/ 01 D8
/	ADDW	%SP,%AX			/ 01 E0
/	ADDW	%BP,%AX			/ 01 E8
/	ADDW	%SI,%AX			/ 01 F0
/	ADDW	%DI,%AX			/ 01 F8
/
/--
/	ADDB	%AL,(%AX)		/ 02
/	ADDW	%AX,(%AX)		/ 03
/	ADDB	64,%AL			/ 04 40
/	ADDW	32,%AX			/ 05 20
