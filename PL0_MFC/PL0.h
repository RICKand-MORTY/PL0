#pragma once
/*** PL0 COMPILER WITH CODE GENERATION ***/
//---------------------------------------------------------------------------
#include <stdio.h>
#include <string>
using std::string;
//---------------------------------------------------------------------------
BOOL List_all;
//---------------------------------------------------------------------------
const  int AL = 10;  /* LENGTH OF IDENTIFIERS */
const  int NORW = 24;  /* # OF RESERVED WORDS */
const  int TXMAX = 100;  /* LENGTH OF IDENTIFIER TABLE */
const  int NMAX = 14;  /* MAX NUMBER OF DEGITS IN NUMBERS */
const  int AMAX = 2047;  /* MAXIMUM ADDRESS */
const  int LEVMAX = 3;  /* MAX DEPTH OF BLOCK NESTING */
const  int CXMAX = 200;  /* SIZE OF CODE ARRAY */

typedef enum {
	NUL, IDENT, NUMBER, PLUS, MINUS, TIMES,
	SLASH, ODDSYM, EQL, NEQ, LSS, LEQ, GTR, GEQ,
	LPAREN, RPAREN, COMMA, SEMICOLON, PERIOD,
	BECOMES, BEGINSYM, ENDSYM, IFSYM, THENSYM,
	WHILESYM, WRITESYM, READSYM, DOSYM, CALLSYM,
	CONSTSYM, VARSYM, PROCSYM, PROGSYM, ELSESYM, FORSYM, STEPSYM, UNTILSYM,
	PLUSPLUS, DECDEC, PLUSEQL, DECEQL, MULEQL, DIVEQL, __SIZE__
} SYMBOL;
char* SYMOUT[] = { "NUL", "IDENT", "NUMBER", "PLUS", "MINUS", "TIMES",
		"SLASH", "ODDSYM", "EQL", "NEQ", "LSS", "LEQ", "GTR", "GEQ",
		"LPAREN", "RPAREN", "COMMA", "SEMICOLON", "PERIOD",
		"BECOMES", "BEGINSYM", "ENDSYM", "IFSYM", "THENSYM",
		"WHILESYM", "WRITESYM", "READSYM", "DOSYM", "CALLSYM",
		"CONSTSYM", "VARSYM", "PROCSYM", "PROGSYM", "ELSESYM", "FORSYM", "STEPSYM", "UNTILSYM",
		"PLUSPLUS", "DECDEC", "PLUSEQL", "DECEQL", "MULEQL", "DIVEQL"
};
typedef  int* SYMSET; // SET OF SYMBOL;
typedef  char ALFA[11];
typedef  enum { CONSTANT, VARIABLE, PROCEDUR } OBJECTS;
typedef  enum { LIT, OPR, LOD, STO, CALL, INI, JMP, JPC } FCT;
typedef struct {
	FCT F;     /*FUNCTION CODE*/
	int L; 	/*0..LEVMAX  LEVEL*/
	int A;     /*0..AMAX    DISPLACEMENT ADDR*/
} INSTRUCTION;
/* LIT O A -- LOAD CONSTANT A             */
/* OPR 0 A -- EXECUTE OPR A               */
/* LOD L A -- LOAD VARIABLE L,A           */
/* STO L A -- STORE VARIABLE L,A          */
/* CAL L A -- CALL PROCEDURE A AT LEVEL L */
/* INI 0 A -- INCREMET T-REGISTER BY A    */
/* JMP 0 A -- JUMP TO A                   */
/* JPC 0 A -- JUMP CONDITIONAL TO A       */
char   CH;  /*LAST CHAR READ*/
SYMBOL SYM; /*LAST SYMBOL READ*/
ALFA   ID;  /*LAST IDENTIFIER READ*/
int    NUM; /*LAST NUMBER READ*/
int    CC;  /*CHARACTER COUNT*/
int    LL;  /*LINE LENGTH*/
int    CX;  /*CODE ALLOCATION INDEX*/
char   LINE[81];
INSTRUCTION  CODE[CXMAX];
ALFA    KWORD[NORW + 1];
SYMBOL  WSYM[NORW + 1];
SYMBOL  SSYM['^' + 1];
ALFA    MNEMONIC[9];
SYMSET  DECLBEGSYS, STATBEGSYS, FACBEGSYS;

struct {
	ALFA NAME;
	OBJECTS KIND;
	union {
		int VAL;   /*CONSTANT*/
		struct { int LEVEL, ADR, SIZE; } vp;  /*VARIABLE,PROCEDUR:*/
	};
} TABLE[TXMAX];

FILE* FIN, * FOUT;
int ERR;

void EXPRESSION(SYMSET FSYS, int LEV, int& TX);
void TERM(SYMSET FSYS, int LEV, int& TX);

void AppendTextToRichEdit(int nID, const char* str)
{
	// ��ȡ���ı���ľ��
	HWND hwnd = GetDlgItem(AfxGetMainWnd()->m_hWnd, nID);

	// ��const char* ת��Ϊ LPCWSTR (��������Ŀ��Unicode)
#ifdef UNICODE
	size_t newsize = strlen(str) + 1;
	wchar_t* wcstring = new wchar_t[newsize];
	size_t convertedChars = 0;
	mbstowcs_s(&convertedChars, wcstring, newsize, str, _TRUNCATE);
	LPCWSTR ptr = wcstring;
#else
	LPCSTR ptr = str;
#endif

	// ��ȡ���ı���ĵ�ǰ�ı�����
	int nLength = GetWindowTextLength(hwnd);

	// ��������ƶ����ı�ĩβ
	SendMessage(hwnd, EM_SETSEL, (WPARAM)nLength, (LPARAM)nLength);

	// ׷���ı�
	SendMessage(hwnd, EM_REPLACESEL, FALSE, (LPARAM)ptr);

	// ���������ڴ�
#ifdef UNICODE
	delete[] wcstring;
#endif
}

//---------------------------------------------------------------------------
int SymIn(SYMBOL SYM, SYMSET S1) {
	return S1[SYM];
}
//---------------------------------------------------------------------------
/*�ϲ�S1��S2�������ż�*/
SYMSET SymSetUnion(SYMSET S1, SYMSET S2) {
	SYMSET S = (SYMSET)malloc(sizeof(int) * __SIZE__);
	for (int i = 0; i < __SIZE__; i++)
		if (S1[i] || S2[i]) S[i] = 1;
		else S[i] = 0;
	return S;
}
//---------------------------------------------------------------------------
/*SY���뵽S��*/
SYMSET SymSetAdd(SYMBOL SY, SYMSET S) {
	SYMSET S1;
	S1 = (SYMSET)malloc(sizeof(int) * __SIZE__);
	for (int i = 0; i < __SIZE__; i++) S1[i] = S[i];
	S1[SY] = 1;
	return S1;
}
//---------------------------------------------------------------------------
/*����һ���µķ��ż��ϣ�����ֻ�����˸����ķ���*/
SYMSET SymSetNew(SYMBOL a) {
	SYMSET S; int i, k;
	S = (SYMSET)malloc(sizeof(int) * __SIZE__);
	for (i = 0; i < __SIZE__; i++) S[i] = 0;
	S[a] = 1;
	return S;
}
//---------------------------------------------------------------------------
SYMSET SymSetNew(SYMBOL a, SYMBOL b) {
	SYMSET S; int i, k;
	S = (SYMSET)malloc(sizeof(int) * __SIZE__);
	for (i = 0; i < __SIZE__; i++) S[i] = 0;
	S[a] = 1;  S[b] = 1;
	return S;
}
//---------------------------------------------------------------------------
SYMSET SymSetNew(SYMBOL a, SYMBOL b, SYMBOL c) {
	SYMSET S; int i, k;
	S = (SYMSET)malloc(sizeof(int) * __SIZE__);
	for (i = 0; i < __SIZE__; i++) S[i] = 0;
	S[a] = 1;  S[b] = 1; S[c] = 1;
	return S;
}
//---------------------------------------------------------------------------
SYMSET SymSetNew(SYMBOL a, SYMBOL b, SYMBOL c, SYMBOL d) {
	SYMSET S; int i, k;
	S = (SYMSET)malloc(sizeof(int) * __SIZE__);
	for (i = 0; i < __SIZE__; i++) S[i] = 0;
	S[a] = 1;  S[b] = 1; S[c] = 1; S[d] = 1;
	return S;
}
//---------------------------------------------------------------------------
SYMSET SymSetNew(SYMBOL a, SYMBOL b, SYMBOL c, SYMBOL d, SYMBOL e) {
	SYMSET S; int i, k;
	S = (SYMSET)malloc(sizeof(int) * __SIZE__);
	for (i = 0; i < __SIZE__; i++) S[i] = 0;
	S[a] = 1;  S[b] = 1; S[c] = 1; S[d] = 1; S[e] = 1;
	return S;
}
//---------------------------------------------------------------------------
SYMSET SymSetNew(SYMBOL a, SYMBOL b, SYMBOL c, SYMBOL d, SYMBOL e, SYMBOL f) {
	SYMSET S; int i, k;
	S = (SYMSET)malloc(sizeof(int) * __SIZE__);
	for (i = 0; i < __SIZE__; i++) S[i] = 0;
	S[a] = 1;  S[b] = 1; S[c] = 1; S[d] = 1; S[e] = 1; S[f] = 1;
	return S;
}
//---------------------------------------------------------------------------
/*����һ���յķ��ż���*/
SYMSET SymSetNULL() {
	SYMSET S; int i, n, k;
	S = (SYMSET)malloc(sizeof(int) * __SIZE__);
	for (i = 0; i < __SIZE__; i++) S[i] = 0;
	return S;
}
//---------------------------------------------------------------------------
void Error(int n) {
	string spaces(CC - 1, ' ');
	string s = "***" + spaces + "^";
	AppendTextToRichEdit(IDC_RICHEDIT21, (s + std::to_string(n) + '\n').c_str());
	fprintf(FOUT, "%s%d\n", s.c_str(), n);
	ERR++;
} /*Error*/
//---------------------------------------------------------------------------
void GetCh() {
	if (CC == LL) {
		if (feof(FIN)) {
			AppendTextToRichEdit(IDC_RICHEDIT21, "PROGRAM INCOMPLETE\n");
			fprintf(FOUT, "PROGRAM INCOMPLETE\n");
			fclose(FOUT);
			exit(0);
		}
		LL = 0; CC = 0;
		CH = ' ';
		while (!feof(FIN) && CH != 10)
		{
			CH = fgetc(FIN);  LINE[LL++] = CH;
		}
		LINE[LL - 1] = ' ';  LINE[LL] = 0;
		string s = std::to_string(CX);
		while (s.length() < 3) s = " " + s;
		s = s + " " + LINE;
		AppendTextToRichEdit(IDC_RICHEDIT21, (s + '\n').c_str());
		fprintf(FOUT, "%s\n", s.c_str());
	}
	CH = LINE[CC++];
} /*GetCh()*/
//---------------------------------------------------------------------------
void GetSym() {
	int i, J, K;   ALFA  A;
	while (CH <= ' ') GetCh();
	if (CH >= 'A' && CH <= 'Z') { /*ID OR RESERVED WORD*/
		K = 0;
		do {
			if (K < AL) A[K++] = CH;
			GetCh();
		} while ((CH >= 'A' && CH <= 'Z') || (CH >= '0' && CH <= '9'));
		A[K] = '\0';
		strcpy(ID, A); i = 1; J = NORW;
		do {
			K = (i + J) / 2;
			if (strcmp(ID, KWORD[K]) <= 0) J = K - 1;
			if (strcmp(ID, KWORD[K]) >= 0) i = K + 1;
		} while (i <= J);
		if (i - 1 > J) SYM = WSYM[K];
		else SYM = IDENT;
		switch (SYM)
		{
		case ELSESYM:
		{
			//AppendTextToRichEdit(IDC_RICHEDIT21, "keyword ELSE found!\n");
			//Error(10);
			//SYM = IDENT;
			break;
		}
		case FORSYM:
		{
			//AppendTextToRichEdit(IDC_RICHEDIT21, "keyword FOR found!\n");
			//Error(10);
			//SYM = IDENT;
			break;
		}
		case STEPSYM:
		{
			//AppendTextToRichEdit(IDC_RICHEDIT21, "keyword STEP found!\n");
			//Error(10);
			//SYM = IDENT;
			break;
		}
		case UNTILSYM:
		{
			//AppendTextToRichEdit(IDC_RICHEDIT21, "keyword UNTIL found!\n");
			//Error(10);
			//SYM = IDENT;
			break;
		}
		case DOSYM:
		{
			//AppendTextToRichEdit(IDC_RICHEDIT21, "keyword DO found!\n");
			break;
		}
		}
	}
	else if (CH >= '0' && CH <= '9') 
	{ /*NUMBER*/
		K = 0; NUM = 0; SYM = NUMBER;
		do {
			NUM = 10 * NUM + (CH - '0');
			K++; GetCh();
		} while (CH >= '0' && CH <= '9');
		if (K > NMAX) Error(30);
	}
	else if (CH == ':') 
	{
		GetCh();
		if (CH == '=') { SYM = BECOMES; GetCh(); }
		else SYM = NUL;
	}
	/* THE FOLLOWING TWO CHECK WERE ADDED
				   BECAUSE ASCII DOES NOT HAVE A SINGLE CHARACTER FOR <= OR >= */
	else if (CH == '<') 
	{
		GetCh();
		if (CH == '=') { SYM = LEQ; GetCh(); }
		if (CH == '>')
		{ 
			//AppendTextToRichEdit(IDC_RICHEDIT21, "operator <> found!\n");
			SYM = NEQ; 
			GetCh(); 
		}
		else SYM = LSS;
	}
	else if (CH == '>') 
	{
		GetCh();
		if (CH == '=') { SYM = GEQ; GetCh(); }
		else SYM = GTR;
	}
	else if (CH == '+')
	{
		GetCh();
		if (CH == '+')
		{
			//AppendTextToRichEdit(IDC_RICHEDIT21, "operator ++ found!\n");
			SYM = PLUSPLUS;
			GetCh();
		}
		else if (CH == '=')
		{
			//AppendTextToRichEdit(IDC_RICHEDIT21, "operator += found!\n");
			SYM = PLUSEQL;
			GetCh();
		}
		else SYM = PLUS;
	}
	else if (CH == '-')
	{
		GetCh();
		if (CH == '-')
		{
			//AppendTextToRichEdit(IDC_RICHEDIT21, "operator -- found!\n");
			SYM = DECDEC;
			//Error(17);
			//SYM = SSYM[CH];			//�ôʷ������ܼ���
			GetCh();
		}
		else if (CH == '=')
		{
			//AppendTextToRichEdit(IDC_RICHEDIT21, "operator -= found!\n");
			SYM = DECEQL;
			//Error(17);
			//SYM = SSYM[CH];
			GetCh();
		}
		else SYM = MINUS;
	}
	else if (CH == '*')
	{
		GetCh();
		if (CH == '=')
		{
			//AppendTextToRichEdit(IDC_RICHEDIT21, "operator *= found!\n");
			SYM = MULEQL;
			//Error(17);
			//SYM = SSYM[CH];
			GetCh();
		}
		else SYM = TIMES;
	}
	else if (CH == '/')
	{
		GetCh();
		if (CH == '=')
		{
			//AppendTextToRichEdit(IDC_RICHEDIT21, "operator /= found!\n");
			SYM = DIVEQL;
			//Error(17);
			//SYM = SSYM[CH];
			GetCh();
		}
		else SYM = SLASH;
	}
	else { SYM = SSYM[CH]; GetCh(); }
} /*GetSym()*/
//---------------------------------------------------------------------------
void GEN(FCT X, int Y, int Z) {
	if (CX > CXMAX) {
		AppendTextToRichEdit(IDC_RICHEDIT21, "PROGRAM TOO LONG\n");
		fprintf(FOUT, "PROGRAM TOO LONG\n");
		fclose(FOUT);
		exit(0);
	}
	CODE[CX].F = X; CODE[CX].L = Y; CODE[CX].A = Z;
	CX++;
} /*GEN*/
//---------------------------------------------------------------------------
void TEST(SYMSET S1, SYMSET S2, int N) {
	if (!SymIn(SYM, S1)) {
		Error(N);
		while (!SymIn(SYM, SymSetUnion(S1, S2))) GetSym();
	}
} /*TEST*/
//---------------------------------------------------------------------------
void ENTER(OBJECTS K, int LEV, int& TX, int& DX) { /*ENTER OBJECT INTO TABLE*/
	TX++;
	strcpy(TABLE[TX].NAME, ID); TABLE[TX].KIND = K;
	switch (K) {
	case CONSTANT:
		if (NUM > AMAX) { Error(31); NUM = 0; }
		TABLE[TX].VAL = NUM;
		break;
	case VARIABLE:
		TABLE[TX].vp.LEVEL = LEV; TABLE[TX].vp.ADR = DX; DX++;
		break;
	case PROCEDUR:
		TABLE[TX].vp.LEVEL = LEV;
		break;
	}
} /*ENTER*/
//---------------------------------------------------------------------------
/*
�ڷ��ű��в���ָ���ı�ʶ��,
����ȷ����ʶ���Ƿ��Ѿ�����������ȡ���Ӧ�ķ��ű�����
@ID:Ҫ���ҵı�ʶ��
@TX:��ǰ���ű�����
retn:ID�ڷ��ű������
*/
int PPOSITION(ALFA ID, int TX) { /*FIND IDENTIFIER IN TABLE*/
	int i = TX;
	strcpy(TABLE[0].NAME, ID);
	while (strcmp(TABLE[i].NAME, ID) != 0) i--;
	return i;
} /*POSITION*/
//---------------------------------------------------------------------------
void ConstDeclaration(int LEV, int& TX, int& DX) {
	if (SYM == IDENT) {
		GetSym();
		if (SYM == EQL || SYM == BECOMES) {
			if (SYM == BECOMES) Error(1);
			GetSym();
			if (SYM == NUMBER) { ENTER(CONSTANT, LEV, TX, DX); GetSym(); }
			else Error(2);
		}
		else Error(3);
	}
	else Error(4);
} /*ConstDeclaration()*/
//---------------------------------------------------------------------------
void VarDeclaration(int LEV, int& TX, int& DX) {
	if (SYM == IDENT) { ENTER(VARIABLE, LEV, TX, DX); GetSym(); }
	else Error(4);
} /*VarDeclaration()*/
//---------------------------------------------------------------------------
void ListCode(int CX0) {  /*LIST CODE GENERATED FOR THIS Block*/
	if (List_all)
		for (int i = CX0; i < CX; i++) {
			string s = std::to_string(i);
			while (s.length() < 3)s = " " + s;
			s = s + " " + MNEMONIC[CODE[i].F] + " " + std::to_string(CODE[i].L) + " " + std::to_string(CODE[i].A);
			AppendTextToRichEdit(IDC_RICHEDIT21, (s + '\n').c_str());
			fprintf(FOUT, "%3d%5s%4d%4d\n", i, MNEMONIC[CODE[i].F], CODE[i].L, CODE[i].A);
		}
} /*ListCode()*/;
//---------------------------------------------------------------------------
/*
@FSYS:�Ϸ��ķ��ż�
@LEV:��ǰ���
@TX:���ű������
*/
void FACTOR(SYMSET FSYS, int LEV, int& TX) {
	int i;
	// ��鵱ǰ�����Ƿ������ӵĿ�ʼ���ż�����
	TEST(FACBEGSYS, FSYS, 24);
	while (SymIn(SYM, FACBEGSYS)) {
		if (SYM == IDENT) {
			i = PPOSITION(ID, TX);
			if (i == 0) Error(11);
			else
				switch (TABLE[i].KIND) {
				case CONSTANT: GEN(LIT, 0, TABLE[i].VAL); break;
				case VARIABLE: GEN(LOD, LEV - TABLE[i].vp.LEVEL, TABLE[i].vp.ADR); break;
				case PROCEDUR: Error(21); break;
				}
			GetSym();
			/*������ʽ��i++��i--*/
			if (SYM == PLUSPLUS)
			{
				GEN(LIT, 0, 1);
				GEN(OPR, 0, 2);
				GEN(STO, LEV - TABLE[i].vp.LEVEL, TABLE[i].vp.ADR);
				//���޸�ǰ��ֵ��������
				GEN(LOD, LEV - TABLE[i].vp.LEVEL, TABLE[i].vp.ADR);
				GEN(LIT, 0, 1);
				GEN(OPR, 0, 3);
				GetSym();
			}
			else if (SYM == DECDEC)
			{
				GEN(LIT, 0, 1);
				GEN(OPR, 0, 3);
				GEN(STO, LEV - TABLE[i].vp.LEVEL, TABLE[i].vp.ADR);
				//ͬ��
				GEN(LOD, LEV - TABLE[i].vp.LEVEL, TABLE[i].vp.ADR);
				GEN(LIT, 0, 1);
				GEN(OPR, 0, 2);
				GetSym();
			}
		}
		else if (SYM == NUMBER)
		{
				if (NUM > AMAX) { Error(31); NUM = 0; }
				GEN(LIT, 0, NUM); GetSym();
		}
		else if (SYM == LPAREN) 
		{
				GetSym(); EXPRESSION(SymSetAdd(RPAREN, FSYS), LEV, TX);
				if (SYM == RPAREN) GetSym();
				else Error(22);
		}
		TEST(FSYS, FACBEGSYS, 23);
	}
}/*FACTOR*/
//---------------------------------------------------------------------------
void TERM(SYMSET FSYS, int LEV, int& TX) {  /*TERM*/
	SYMBOL MULOP;
	FACTOR(SymSetUnion(FSYS, SymSetNew(TIMES, SLASH)), LEV, TX);
	while (SYM == TIMES || SYM == SLASH) {
		MULOP = SYM;  GetSym();
		FACTOR(SymSetUnion(FSYS, SymSetNew(TIMES, SLASH)), LEV, TX);
		if (MULOP == TIMES) GEN(OPR, 0, 4);
		else GEN(OPR, 0, 5);
	}
} /*TERM*/;
//---------------------------------------------------------------------------
void EXPRESSION(SYMSET FSYS, int LEV, int& TX) {
	SYMBOL ADDOP;
	int i = 0;
	if (SYM == PLUS || SYM == MINUS) {
		ADDOP = SYM; GetSym();
		TERM(SymSetUnion(FSYS, SymSetNew(PLUS, MINUS)), LEV, TX);
		if (ADDOP == MINUS) GEN(OPR, 0, 1);/*����ȡ��*/
	}
	else if (SYM == PLUSPLUS)
	{   /* ++i */
		GetSym();
		if (SYM == IDENT) {
			i = PPOSITION(ID, TX);
			if (i == 0) Error(11);
			else if (TABLE[i].KIND != VARIABLE) {
				Error(12);
				i = 0;
			}
			if (i != 0) GEN(LOD, LEV - TABLE[i].vp.LEVEL, TABLE[i].vp.ADR);
			GEN(LIT, 0, 1);
			GEN(OPR, 0, 2);
			if (i != 0) {
				GEN(STO, LEV - TABLE[i].vp.LEVEL, TABLE[i].vp.ADR);
				GEN(LOD, LEV - TABLE[i].vp.LEVEL, TABLE[i].vp.ADR);
			}
			GetSym();
		}
		else Error(45);
	}
	else if (SYM == DECDEC) {     
		/* --i */
		GetSym();
		if (SYM == IDENT) {
			i = PPOSITION(ID, TX);
			if (i == 0) Error(11);
			else if (TABLE[i].KIND != VARIABLE) {
				Error(12);
				i = 0;
			}
			if (i != 0) GEN(LOD, LEV - TABLE[i].vp.LEVEL, TABLE[i].vp.ADR);
			GEN(LIT, 0, 1);
			GEN(OPR, 0, 3);
			if (i != 0) {
				GEN(STO, LEV - TABLE[i].vp.LEVEL, TABLE[i].vp.ADR);
				GEN(LOD, LEV - TABLE[i].vp.LEVEL, TABLE[i].vp.ADR);
			}
			GetSym();
		}
		else Error(45);
	}
	else TERM(SymSetUnion(FSYS, SymSetNew(PLUS, MINUS)), LEV, TX);
	while (SYM == PLUS || SYM == MINUS) {
		ADDOP = SYM; GetSym();
		TERM(SymSetUnion(FSYS, SymSetNew(PLUS, MINUS)), LEV, TX);
		if (ADDOP == PLUS) GEN(OPR, 0, 2);	
		else GEN(OPR, 0, 3);	
	}
} /*EXPRESSION*/
//---------------------------------------------------------------------------
void CONDITION(SYMSET FSYS, int LEV, int& TX) {
	SYMBOL RELOP;
	if (SYM == ODDSYM) { GetSym(); EXPRESSION(FSYS, LEV, TX); GEN(OPR, 0, 6); }
	else {
		EXPRESSION(SymSetUnion(SymSetNew(EQL, NEQ, LSS, LEQ, GTR, GEQ), FSYS), LEV, TX);
		if (!SymIn(SYM, SymSetNew(EQL, NEQ, LSS, LEQ, GTR, GEQ))) Error(20);
		else {
			RELOP = SYM; GetSym(); EXPRESSION(FSYS, LEV, TX);
			switch (RELOP) {
			case EQL: GEN(OPR, 0, 8);  break;
			case NEQ: GEN(OPR, 0, 9);  break;
			case LSS: GEN(OPR, 0, 10); break;
			case GEQ: GEN(OPR, 0, 11); break;
			case GTR: GEN(OPR, 0, 12); break;
			case LEQ: GEN(OPR, 0, 13); break;
			}
		}
	}
} /*CONDITION*/
//---------------------------------------------------------------------------
/*
�﷨����
@FSYS ���ż��ϣ�����ָ���������������п��Խ��ܵķ���
@LEV ��ǰ�����Ĳ��(��̬���)
@TX ���ű�����������,���ű����ڴ洢�����еı�ʶ����������
*/
void STATEMENT(SYMSET FSYS, int LEV, int& TX) {   /*STATEMENT*/
	int i, CX1, CX2, CX3;
	BOOL then_no = FALSE;
	switch (SYM) {
	case IDENT: /*��ʶ��*/
		i = PPOSITION(ID, TX);	/* ���ұ�ʶ���ڷ��ű��е�λ�� */
		if (i == 0) Error(11);	/* ��ʶ��δ���� */
		else if (TABLE[i].KIND != VARIABLE) 
		{ /* ��ֵ���Ǳ��� */
				Error(12); i = 0;
		}
		GetSym();
		if (SYM == BECOMES) 
		{
			/* ���� = */
			GetSym();
			EXPRESSION(FSYS, LEV, TX);		/* ������ʽ */
			/* ���ɴ洢ָ�� */
			if (i != 0) GEN(STO, LEV - TABLE[i].vp.LEVEL, TABLE[i].vp.ADR);
		}
		else if(SYM == MULEQL)
		{
			/* ���� *= */
			GetSym();
			EXPRESSION(FSYS, LEV, TX);
			if (i != 0)
			{
				GEN(LOD, LEV - TABLE[i].vp.LEVEL, TABLE[i].vp.ADR);
				GEN(OPR, 0, 4);
				GEN(STO, LEV - TABLE[i].vp.LEVEL, TABLE[i].vp.ADR);  /* �洢��� */
			}
		}
		else if (SYM == PLUSPLUS)
		{
			/* i++ */
			if (i != 0)
			{
				GEN(LOD, LEV - TABLE[i].vp.LEVEL, TABLE[i].vp.ADR);
				GEN(LIT, 0, 1);
				GEN(OPR, 0, 2);
				GEN(STO, LEV - TABLE[i].vp.LEVEL, TABLE[i].vp.ADR);
			}
			else
			{
				Error(11);		//��ʶ��δ����
			}
			GetSym();
		}
		else if (SYM == DECDEC)
		{
			/* i-- */
			if (i != 0)
			{
				GEN(LOD, LEV - TABLE[i].vp.LEVEL, TABLE[i].vp.ADR);
				GEN(LIT, 0, 1);
				GEN(OPR, 0, 3);
				GEN(STO, LEV - TABLE[i].vp.LEVEL, TABLE[i].vp.ADR);
			}
			else
			{
				Error(11);		//��ʶ��δ����
			}
			GetSym();
		}
		else if (SYM == DIVEQL)
		{
			/* ���� /= */
			/*��������ջ����������뱻�����ߵ���*/
			GEN(LOD, LEV - TABLE[i].vp.LEVEL, TABLE[i].vp.ADR);	
			GetSym();
			EXPRESSION(FSYS, LEV, TX);
			if (i != 0)
			{
				/* ִ�г������� */
				GEN(OPR, 0, 5);
				GEN(STO, LEV - TABLE[i].vp.LEVEL, TABLE[i].vp.ADR);
			}
		}
		else Error(13);					/* ȱ�ٷ��� */
		break;
	case PLUSPLUS:
		/* ++i */
		GetSym();
		if (SYM == IDENT) 
		{
			i = PPOSITION(ID, TX);
			if (i == 0) Error(11);
			else if (TABLE[i].KIND != VARIABLE) 
			{
				Error(12);
				i = 0;
			}
			if (i != 0) GEN(LOD, LEV - TABLE[i].vp.LEVEL, TABLE[i].vp.ADR);
			GEN(LIT, 0, 1);
			GEN(OPR, 0, 2);
			if (i != 0) GEN(STO, LEV - TABLE[i].vp.LEVEL, TABLE[i].vp.ADR);
			GetSym();
		}
		else Error(45);
		break;
	case DECDEC:
		GetSym();
		if (SYM == IDENT) 
		{
			i = PPOSITION(ID, TX);
			if (i == 0) Error(11);
			else if (TABLE[i].KIND != VARIABLE) 
			{
				Error(12);
				i = 0;
			}
			if (i != 0) GEN(LOD, LEV - TABLE[i].vp.LEVEL, TABLE[i].vp.ADR);
			GEN(LIT, 0, 1);
			GEN(OPR, 0, 3);
			if (i != 0) GEN(STO, LEV - TABLE[i].vp.LEVEL, TABLE[i].vp.ADR);
			GetSym();
		}
		else Error(45);
		break;
	case READSYM:	/*read*/
		GetSym();
		if (SYM != LPAREN) Error(34);		/* ȱ�������� */
		else
			do {
				GetSym();
				if (SYM == IDENT) i = PPOSITION(ID, TX);	/* ���ұ�ʶ�� */
				else i = 0;
				if (i == 0) Error(35);						/* ��ʶ��δ���� */
				else {
					GEN(OPR, 0, 16);						/* ���ɶ�ָ�� */
					/* ���ɴ洢ָ�� */
					GEN(STO, LEV - TABLE[i].vp.LEVEL, TABLE[i].vp.ADR);
				}
				GetSym();
			} while (SYM == COMMA);				/* �����ŷָ��ı�ʶ�� */
			if (SYM != RPAREN) {
				Error(33);
				/* ��������ֱ���ҵ��Ϸ����� */
				while (!SymIn(SYM, FSYS)) GetSym();
			}
			else GetSym();
			break; /* READSYM */
	case WRITESYM:		
		GetSym();
		if (SYM == LPAREN) {
			do {
				GetSym();
				EXPRESSION(SymSetUnion(SymSetNew(RPAREN, COMMA), FSYS), LEV, TX);
				GEN(OPR, 0, 14);
			} while (SYM == COMMA);
			if (SYM != RPAREN) Error(33);
			else GetSym();
		}
		/* ����? */
		GEN(OPR, 0, 15);
		break; /*WRITESYM*/
	case CALLSYM:	/* ���ù������ */
		GetSym();
		if (SYM != IDENT) Error(14);	/* ȱ�ٱ�ʶ�� */
		else {
			i = PPOSITION(ID, TX);	/* ���ұ�ʶ�� */
			if (i == 0) Error(11);	/* ��ʶ��δ���� */
			else
				if (TABLE[i].KIND == PROCEDUR)
					/* ���ɵ���ָ�� */
					GEN(FCT::CALL, LEV - TABLE[i].vp.LEVEL, TABLE[i].vp.ADR);
				else Error(15);
			GetSym();
		}
		break;
	case IFSYM:
		GetSym();
		/* �������� */
		CONDITION(SymSetUnion(SymSetNew(THENSYM, DOSYM), FSYS), LEV, TX);
		if (SYM == THENSYM)
		{
			GetSym();
			CX1 = CX;  /* ��¼��ǰ��������������֮��Ļ��� */
			GEN(JPC, 0, 0);	 /* THEN����ת,��ELSE */
			if (SYM != ELSESYM)
			{
				STATEMENT(FSYS, LEV, TX);
			}
			else
			{
				then_no = TRUE;
			}
		}
		else Error(16);	/* ȱ��then */
		if (!then_no)
		{
			GetSym();  /* ��ELSE */
		}
		if (SYM == ELSESYM)
		{
			CX2 = CX;	/* ��¼else���ǰ�Ĵ������� */
			GEN(JMP, 0, 0);
			CODE[CX1].A = CX;	/* ������ת��ַ������תĿ������Ϊthen�� */
			if (SYM == ELSESYM) { /* �������else�ؼ��� */
				GetSym(); /* ��ȡ��һ������ */
				STATEMENT(FSYS, LEV, TX); /* ����else�������� */
			}
			CODE[CX2].A = CX; /* ������������תָ���Ŀ���ַ */
		}
		else
		{
			CODE[CX1].A = CX;	/* û�и�ELSE */
			STATEMENT(FSYS, LEV, TX);
		}
		break;
	case FORSYM:  /* FORѭ����� */
		GetSym();
		if (SYM != IDENT) Error(14);  /* ȱ�ٱ�ʶ�� */
		else {
			i = PPOSITION(ID, TX);  /* ���ұ�ʶ�� */
			if (i == 0) Error(11);  /* ��ʶ��δ���� */
			else if (TABLE[i].KIND != VARIABLE) {  /* ��ֵ���Ǳ��� */
				Error(12); i = 0;
			}
			GetSym();
			if (SYM == BECOMES) {  /* ����ֵ����:= */
				GetSym();
				EXPRESSION(SymSetUnion(SymSetNew(STEPSYM), FSYS), LEV, TX);  /* ������ʽ */
				/* ���ɴ洢ָ�� */
				if (i != 0) GEN(STO, LEV - TABLE[i].vp.LEVEL, TABLE[i].vp.ADR);
			}
			else
			{
				Error(13);  /* ȱ�ٸ�ֵ����:= */
			}
			if (SYM == STEPSYM) {  /* �������ؼ���STEP */
				GetSym();
				CX1 = CX;
				GEN(JMP, 0, 0);		/*��һ�β���STEP*/
				CX3 = CX;
				if (SYM == IDENT)
				{
					i = PPOSITION(ID, TX);  /* ���ұ�ʶ�� */
					if (i == 0) Error(11);  /* ��ʶ��δ���� */
					else if (TABLE[i].KIND != VARIABLE) {  /* ��ֵ���Ǳ��� */
						Error(12); i = 0;
					}
					GetSym();
					if (SYM == BECOMES) {  /* ����ֵ����:= */
						GetSym();
						EXPRESSION(SymSetUnion(SymSetNew(UNTILSYM), FSYS), LEV, TX);  /* ������ʽ */
						if (i != 0) GEN(STO, LEV - TABLE[i].vp.LEVEL, TABLE[i].vp.ADR);	/*��ջ(д��)*/
					}
					else
					{
						Error(13);
					}
				}
				else
				{
					Error(14);
				}
			}
			else
			{
				Error(20);  /* ȱ�ٲ����ؼ���STEP */
			}
			if (SYM == UNTILSYM) {  /* ������ֹ�����ؼ���UNTIL */
				GetSym();
				CODE[CX1].A = CX;	/*����STEPǰ��JMP*/
				CONDITION(SymSetUnion(SymSetNew(DOSYM), FSYS), LEV, TX);  /* ������ֹ���� */
				GEN(OPR, 0, 17);		/*ȡ������Ϊ��UNTIL, 17���¶���ķ�����*/
				/* ����������תָ����������ѭ������ */
				CX2 = CX; 
				GEN(JPC, 0, 0);	
			}
			else
			{
				Error(21);  /* ȱ����ֹ�����ؼ���UNTIL */
			}
			if (SYM == DOSYM) {  /* ����DO�ؼ��� */
				GetSym();
				STATEMENT(FSYS, LEV, TX);  /* ����DO�������� */
				/* ������������תָ���ת��STEP */
				GEN(JMP, 0, CX3);	
				/* ������ת��ַ������תĿ������ΪDO�������� */
				CODE[CX2].A = CX;
			}
			else Error(18);  /* ȱ��DO�ؼ��� */
		}
		break;

	case BEGINSYM:
		GetSym();
		/* �����һ����� */
		STATEMENT(SymSetUnion(SymSetNew(SEMICOLON, ENDSYM), FSYS), LEV, TX);
		while (SymIn(SYM, SymSetAdd(SEMICOLON, STATBEGSYS))) {
			/* ����������� */
			if (SYM == SEMICOLON) GetSym();	 /* ����ֺ� */
			else Error(10);		/* ȱ�ٷֺ� */
			/* ������һ����� */
			STATEMENT(SymSetUnion(SymSetNew(SEMICOLON, ENDSYM), FSYS), LEV, TX);
		}
		if (SYM == ENDSYM) GetSym();
		else Error(17);	/* ȱ��end */
		break;
	case WHILESYM:
		CX1 = CX; GetSym(); CONDITION(SymSetAdd(DOSYM, FSYS), LEV, TX);
		CX2 = CX; GEN(JPC, 0, 0);	 /* ������ת */
		if (SYM == DOSYM) GetSym();
		else Error(18);				/* ȱ��do */
		STATEMENT(FSYS, LEV, TX);
		GEN(JMP, 0, CX1);
		CODE[CX2].A = CX;		/* ������ת��ַ */
		break;
	}
	TEST(FSYS, SymSetNULL(), 19);		/* ���Է��ż��ϣ���������� */
} /*STATEMENT*/
//---------------------------------------------------------------------------
/*
LEV��ʾ��ǰ�����Ĳ��
TX�Ƿ��ű�ǰ������
FSYS�Ǹ�����ż���
*/
void Block(int LEV, int TX, SYMSET FSYS) {
	/*���ݷ������������ڱ�������ʱ�����ڴ��ַ,012��SL DL RA*/
	int DX = 3;    /*DATA ALLOCATION INDEX*/
	int TX0 = TX;  /*INITIAL TABLE INDEX*/
	int CX0 = CX;  /*INITIAL CODE INDEX*/
	// ������תָ����ڹ��̵���
	TABLE[TX].vp.ADR = CX; GEN(JMP, 0, 0);
	// ��鵱ǰ���Ƕ�ײ���Ƿ񳬹��������ֵ
	if (LEV > LEVMAX) Error(32);
	do {
		// ����������
		if (SYM == CONSTSYM) {
			GetSym();
			do {
				ConstDeclaration(LEV, TX, DX);
				while (SYM == COMMA) {
					GetSym();  ConstDeclaration(LEV, TX, DX);
				}
				if (SYM == SEMICOLON) GetSym();
				else Error(5);
			} while (SYM == IDENT);
		}
		// �����������
		if (SYM == VARSYM) {
			GetSym();
			do {
				VarDeclaration(LEV, TX, DX);
				while (SYM == COMMA) { GetSym(); VarDeclaration(LEV, TX, DX); }
				if (SYM == SEMICOLON) GetSym();
				else Error(5);
			} while (SYM == IDENT);
		}
		//����
		while (SYM == PROCSYM) {
			GetSym();
			if (SYM == IDENT) { ENTER(PROCEDUR, LEV, TX, DX); GetSym(); }
			else Error(4);
			if (SYM == SEMICOLON) GetSym();
			else Error(5);
			Block(LEV + 1, TX, SymSetAdd(SEMICOLON, FSYS));
			if (SYM == SEMICOLON) {
				GetSym();
				TEST(SymSetUnion(SymSetNew(IDENT, PROCSYM), STATBEGSYS), FSYS, 6);
			}
			else Error(5);
		}
		// ��鵱ǰ�����Ƿ�Ϸ�
		TEST(SymSetAdd(IDENT, STATBEGSYS), DECLBEGSYS, 7);
	} while (SymIn(SYM, DECLBEGSYS));
	// ������ת��ַ
	CODE[TABLE[TX0].vp.ADR].A = CX;
	// ���ô���ε���ʼ��ַ�����ݶεĴ�С
	TABLE[TX0].vp.ADR = CX;   /*START ADDR OF CODE*/
	TABLE[TX0].vp.SIZE = DX;  /*SIZE OF DATA SEGMENT*/
	// ���ɳ�ʼ��ָ��
	GEN(INI, 0, DX);
	STATEMENT(SymSetUnion(SymSetNew(SEMICOLON, ENDSYM), FSYS), LEV, TX);
	// ���ɷ���ָ��
	GEN(OPR, 0, 0);  /*RETURN*/
	// �����ż���
	TEST(FSYS, SymSetNULL(), 8);
	// �г����ɵĴ���
	ListCode(CX0);
} /*Block*/
//---------------------------------------------------------------------------
int BASE(int L, int B, int S[]) {
	int B1 = B; /*FIND BASE L LEVELS DOWN*/
	while (L > 0) { B1 = S[B1]; L = L - 1; }
	return B1;
} /*BASE*/
//---------------------------------------------------------------------------
void Interpret() {
	const int STACKSIZE = 500;
	int P, B, T; 		/*PROGRAM BASE TOPSTACK REGISTERS*/
	INSTRUCTION I;
	int S[STACKSIZE];  	/*DATASTORE*/
	AppendTextToRichEdit(IDC_RICHEDIT21, "~~~ RUN PL0 ~~~\n");
	fwprintf(FOUT, L"~~~ RUN PL0 ~~~\n");
	T = 0; B = 1; P = 0;
	S[1] = 0; S[2] = 0; S[3] = 0;
	do {
		I = CODE[P]; P = P + 1;
		switch (I.F) {
		case LIT: T++; S[T] = I.A; break;
		case OPR:
			switch (I.A) { /*OPERATOR*/
			case 0: /*RETURN*/ T = B - 1; P = S[T + 3]; B = S[T + 2]; break;
			case 1: S[T] = -S[T];  break;
			case 2: T--; S[T] = S[T] + S[T + 1];   break;
			case 3: T--; S[T] = S[T] - S[T + 1];   break;
			case 4: T--; S[T] = S[T] * S[T + 1];   break;
			case 5: T--; S[T] = S[T] / S[T + 1]; break;
			case 6: S[T] = (S[T] % 2 != 0);        break;
			case 8: T--; S[T] = S[T] == S[T + 1];  break;
			case 9: T--; S[T] = S[T] != S[T + 1];  break;
			case 10: T--; S[T] = S[T] < S[T + 1];   break;
			case 11: T--; S[T] = S[T] >= S[T + 1];  break;
			case 12: T--; S[T] = S[T] > S[T + 1];   break;
			case 13: T--; S[T] = S[T] <= S[T + 1];  break;
			case 14: AppendTextToRichEdit(IDC_RICHEDIT21, (std::to_string(S[T]) + '\n').c_str()); fwprintf(FOUT, L"%d\n", S[T]); T--;
				break;
			case 15: /*Form1->printfs(""); fprintf(FOUT,"\n"); */ break;
			case 16: T++;
				AppendTextToRichEdit(IDC_RICHEDIT21, ("? " + std::to_string(S[T] + '\n')).c_str());
				fwprintf(FOUT, L"? %d\n", S[T]);
				break;
			case 17: S[T] = !S[T]; break;
			}
			break;
		case LOD: T++; S[T] = S[BASE(I.L, B, S) + I.A]; break;
		case STO: S[BASE(I.L, B, S) + I.A] = S[T]; T--; break;
		case CALL: /*GENERAT NEW Block MARK*/
			S[T + 1] = BASE(I.L, B, S); S[T + 2] = B; S[T + 3] = P;
			B = T + 1; P = I.A; break;
		case INI: T = T + I.A;  break;
		case JMP: P = I.A; break;
		case JPC: if (S[T] == 0) P = I.A;  T--;  break;
		} /*switch*/
	} while (P != 0);
	AppendTextToRichEdit(IDC_RICHEDIT21, "~~~ END PL0 ~~~\n");
	fwprintf(FOUT, L"~~~ END PL0 ~~~\n");
} /*Interpret*/
//---------------------------------------------------------------------------