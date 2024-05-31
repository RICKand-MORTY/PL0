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
	// 获取富文本框的句柄
	HWND hwnd = GetDlgItem(AfxGetMainWnd()->m_hWnd, nID);

	// 将const char* 转换为 LPCWSTR (如果你的项目是Unicode)
#ifdef UNICODE
	size_t newsize = strlen(str) + 1;
	wchar_t* wcstring = new wchar_t[newsize];
	size_t convertedChars = 0;
	mbstowcs_s(&convertedChars, wcstring, newsize, str, _TRUNCATE);
	LPCWSTR ptr = wcstring;
#else
	LPCSTR ptr = str;
#endif

	// 获取富文本框的当前文本长度
	int nLength = GetWindowTextLength(hwnd);

	// 将插入点移动到文本末尾
	SendMessage(hwnd, EM_SETSEL, (WPARAM)nLength, (LPARAM)nLength);

	// 追加文本
	SendMessage(hwnd, EM_REPLACESEL, FALSE, (LPARAM)ptr);

	// 清理分配的内存
#ifdef UNICODE
	delete[] wcstring;
#endif
}

//---------------------------------------------------------------------------
int SymIn(SYMBOL SYM, SYMSET S1) {
	return S1[SYM];
}
//---------------------------------------------------------------------------
/*合并S1和S2两个符号集*/
SYMSET SymSetUnion(SYMSET S1, SYMSET S2) {
	SYMSET S = (SYMSET)malloc(sizeof(int) * __SIZE__);
	for (int i = 0; i < __SIZE__; i++)
		if (S1[i] || S2[i]) S[i] = 1;
		else S[i] = 0;
	return S;
}
//---------------------------------------------------------------------------
/*SY加入到S中*/
SYMSET SymSetAdd(SYMBOL SY, SYMSET S) {
	SYMSET S1;
	S1 = (SYMSET)malloc(sizeof(int) * __SIZE__);
	for (int i = 0; i < __SIZE__; i++) S1[i] = S[i];
	S1[SY] = 1;
	return S1;
}
//---------------------------------------------------------------------------
/*创建一个新的符号集合，其中只包含了给定的符号*/
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
/*创建一个空的符号集合*/
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
			//SYM = SSYM[CH];			//让词法分析能继续
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
在符号表中查找指定的标识符,
用于确定标识符是否已经声明，并获取其对应的符号表索引
@ID:要查找的标识符
@TX:当前符号表索引
retn:ID在符号表的索引
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
@FSYS:合法的符号集
@LEV:当前层次
@TX:符号表的索引
*/
void FACTOR(SYMSET FSYS, int LEV, int& TX) {
	int i;
	// 检查当前符号是否在因子的开始符号集合中
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
			/*处理表达式的i++和i--*/
			if (SYM == PLUSPLUS)
			{
				GEN(LIT, 0, 1);
				GEN(OPR, 0, 2);
				GEN(STO, LEV - TABLE[i].vp.LEVEL, TABLE[i].vp.ADR);
				//用修改前的值参与运算
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
				//同上
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
		if (ADDOP == MINUS) GEN(OPR, 0, 1);/*减号取反*/
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
语法分析
@FSYS 符号集合，用于指定在语句分析过程中可以接受的符号
@LEV 当前代码块的层次(静态层次)
@TX 符号表索引的引用,符号表用于存储程序中的标识符及其属性
*/
void STATEMENT(SYMSET FSYS, int LEV, int& TX) {   /*STATEMENT*/
	int i, CX1, CX2, CX3;
	BOOL then_no = FALSE;
	switch (SYM) {
	case IDENT: /*标识符*/
		i = PPOSITION(ID, TX);	/* 查找标识符在符号表中的位置 */
		if (i == 0) Error(11);	/* 标识符未声明 */
		else if (TABLE[i].KIND != VARIABLE) 
		{ /* 赋值给非变量 */
				Error(12); i = 0;
		}
		GetSym();
		if (SYM == BECOMES) 
		{
			/* 处理 = */
			GetSym();
			EXPRESSION(FSYS, LEV, TX);		/* 处理表达式 */
			/* 生成存储指令 */
			if (i != 0) GEN(STO, LEV - TABLE[i].vp.LEVEL, TABLE[i].vp.ADR);
		}
		else if(SYM == MULEQL)
		{
			/* 处理 *= */
			GetSym();
			EXPRESSION(FSYS, LEV, TX);
			if (i != 0)
			{
				GEN(LOD, LEV - TABLE[i].vp.LEVEL, TABLE[i].vp.ADR);
				GEN(OPR, 0, 4);
				GEN(STO, LEV - TABLE[i].vp.LEVEL, TABLE[i].vp.ADR);  /* 存储结果 */
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
				Error(11);		//标识符未声明
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
				Error(11);		//标识符未声明
			}
			GetSym();
		}
		else if (SYM == DIVEQL)
		{
			/* 处理 /= */
			/*必须先入栈，否则除数与被除数颠倒了*/
			GEN(LOD, LEV - TABLE[i].vp.LEVEL, TABLE[i].vp.ADR);	
			GetSym();
			EXPRESSION(FSYS, LEV, TX);
			if (i != 0)
			{
				/* 执行除法操作 */
				GEN(OPR, 0, 5);
				GEN(STO, LEV - TABLE[i].vp.LEVEL, TABLE[i].vp.ADR);
			}
		}
		else Error(13);					/* 缺少符号 */
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
		if (SYM != LPAREN) Error(34);		/* 缺少左括号 */
		else
			do {
				GetSym();
				if (SYM == IDENT) i = PPOSITION(ID, TX);	/* 查找标识符 */
				else i = 0;
				if (i == 0) Error(35);						/* 标识符未声明 */
				else {
					GEN(OPR, 0, 16);						/* 生成读指令 */
					/* 生成存储指令 */
					GEN(STO, LEV - TABLE[i].vp.LEVEL, TABLE[i].vp.ADR);
				}
				GetSym();
			} while (SYM == COMMA);				/* 处理逗号分隔的标识符 */
			if (SYM != RPAREN) {
				Error(33);
				/* 跳过错误直到找到合法符号 */
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
		/* 换行? */
		GEN(OPR, 0, 15);
		break; /*WRITESYM*/
	case CALLSYM:	/* 调用过程语句 */
		GetSym();
		if (SYM != IDENT) Error(14);	/* 缺少标识符 */
		else {
			i = PPOSITION(ID, TX);	/* 查找标识符 */
			if (i == 0) Error(11);	/* 标识符未声明 */
			else
				if (TABLE[i].KIND == PROCEDUR)
					/* 生成调用指令 */
					GEN(FCT::CALL, LEV - TABLE[i].vp.LEVEL, TABLE[i].vp.ADR);
				else Error(15);
			GetSym();
		}
		break;
	case IFSYM:
		GetSym();
		/* 处理条件 */
		CONDITION(SymSetUnion(SymSetNew(THENSYM, DOSYM), FSYS), LEV, TX);
		if (SYM == THENSYM)
		{
			GetSym();
			CX1 = CX;  /* 记录当前代码索引，用于之后的回填 */
			GEN(JPC, 0, 0);	 /* THEN的跳转,到ELSE */
			if (SYM != ELSESYM)
			{
				STATEMENT(FSYS, LEV, TX);
			}
			else
			{
				then_no = TRUE;
			}
		}
		else Error(16);	/* 缺少then */
		if (!then_no)
		{
			GetSym();  /* 找ELSE */
		}
		if (SYM == ELSESYM)
		{
			CX2 = CX;	/* 记录else语句前的代码索引 */
			GEN(JMP, 0, 0);
			CODE[CX1].A = CX;	/* 回填跳转地址，将跳转目标设置为then后 */
			if (SYM == ELSESYM) { /* 如果遇到else关键字 */
				GetSym(); /* 读取下一个符号 */
				STATEMENT(FSYS, LEV, TX); /* 处理else后面的语句 */
			}
			CODE[CX2].A = CX; /* 回填无条件跳转指令的目标地址 */
		}
		else
		{
			CODE[CX1].A = CX;	/* 没有跟ELSE */
			STATEMENT(FSYS, LEV, TX);
		}
		break;
	case FORSYM:  /* FOR循环语句 */
		GetSym();
		if (SYM != IDENT) Error(14);  /* 缺少标识符 */
		else {
			i = PPOSITION(ID, TX);  /* 查找标识符 */
			if (i == 0) Error(11);  /* 标识符未声明 */
			else if (TABLE[i].KIND != VARIABLE) {  /* 赋值给非变量 */
				Error(12); i = 0;
			}
			GetSym();
			if (SYM == BECOMES) {  /* 处理赋值符号:= */
				GetSym();
				EXPRESSION(SymSetUnion(SymSetNew(STEPSYM), FSYS), LEV, TX);  /* 处理表达式 */
				/* 生成存储指令 */
				if (i != 0) GEN(STO, LEV - TABLE[i].vp.LEVEL, TABLE[i].vp.ADR);
			}
			else
			{
				Error(13);  /* 缺少赋值符号:= */
			}
			if (SYM == STEPSYM) {  /* 处理步长关键字STEP */
				GetSym();
				CX1 = CX;
				GEN(JMP, 0, 0);		/*第一次不走STEP*/
				CX3 = CX;
				if (SYM == IDENT)
				{
					i = PPOSITION(ID, TX);  /* 查找标识符 */
					if (i == 0) Error(11);  /* 标识符未声明 */
					else if (TABLE[i].KIND != VARIABLE) {  /* 赋值给非变量 */
						Error(12); i = 0;
					}
					GetSym();
					if (SYM == BECOMES) {  /* 处理赋值符号:= */
						GetSym();
						EXPRESSION(SymSetUnion(SymSetNew(UNTILSYM), FSYS), LEV, TX);  /* 处理表达式 */
						if (i != 0) GEN(STO, LEV - TABLE[i].vp.LEVEL, TABLE[i].vp.ADR);	/*入栈(写回)*/
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
				Error(20);  /* 缺少步长关键字STEP */
			}
			if (SYM == UNTILSYM) {  /* 处理终止条件关键字UNTIL */
				GetSym();
				CODE[CX1].A = CX;	/*回填STEP前的JMP*/
				CONDITION(SymSetUnion(SymSetNew(DOSYM), FSYS), LEV, TX);  /* 处理终止条件 */
				GEN(OPR, 0, 17);		/*取反，因为是UNTIL, 17是新定义的非运算*/
				/* 生成条件跳转指令，这里会跳到循环结束 */
				CX2 = CX; 
				GEN(JPC, 0, 0);	
			}
			else
			{
				Error(21);  /* 缺少终止条件关键字UNTIL */
			}
			if (SYM == DOSYM) {  /* 处理DO关键字 */
				GetSym();
				STATEMENT(FSYS, LEV, TX);  /* 处理DO后面的语句 */
				/* 生成无条件跳转指令，跳转到STEP */
				GEN(JMP, 0, CX3);	
				/* 回填跳转地址，将跳转目标设置为DO后面的语句 */
				CODE[CX2].A = CX;
			}
			else Error(18);  /* 缺少DO关键字 */
		}
		break;

	case BEGINSYM:
		GetSym();
		/* 处理第一个语句 */
		STATEMENT(SymSetUnion(SymSetNew(SEMICOLON, ENDSYM), FSYS), LEV, TX);
		while (SymIn(SYM, SymSetAdd(SEMICOLON, STATBEGSYS))) {
			/* 处理语句序列 */
			if (SYM == SEMICOLON) GetSym();	 /* 处理分号 */
			else Error(10);		/* 缺少分号 */
			/* 处理下一个语句 */
			STATEMENT(SymSetUnion(SymSetNew(SEMICOLON, ENDSYM), FSYS), LEV, TX);
		}
		if (SYM == ENDSYM) GetSym();
		else Error(17);	/* 缺少end */
		break;
	case WHILESYM:
		CX1 = CX; GetSym(); CONDITION(SymSetAdd(DOSYM, FSYS), LEV, TX);
		CX2 = CX; GEN(JPC, 0, 0);	 /* 条件跳转 */
		if (SYM == DOSYM) GetSym();
		else Error(18);				/* 缺少do */
		STATEMENT(FSYS, LEV, TX);
		GEN(JMP, 0, CX1);
		CODE[CX2].A = CX;		/* 回填跳转地址 */
		break;
	}
	TEST(FSYS, SymSetNULL(), 19);		/* 测试符号集合，检查语句结束 */
} /*STATEMENT*/
//---------------------------------------------------------------------------
/*
LEV表示当前代码块的层次
TX是符号表当前的索引
FSYS是跟随符号集合
*/
void Block(int LEV, int TX, SYMSET FSYS) {
	/*数据分配索引，用于变量声明时分配内存地址,012给SL DL RA*/
	int DX = 3;    /*DATA ALLOCATION INDEX*/
	int TX0 = TX;  /*INITIAL TABLE INDEX*/
	int CX0 = CX;  /*INITIAL CODE INDEX*/
	// 设置跳转指令，用于过程调用
	TABLE[TX].vp.ADR = CX; GEN(JMP, 0, 0);
	// 检查当前块的嵌套层次是否超过最大允许值
	if (LEV > LEVMAX) Error(32);
	do {
		// 处理常量声明
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
		// 处理变量声明
		if (SYM == VARSYM) {
			GetSym();
			do {
				VarDeclaration(LEV, TX, DX);
				while (SYM == COMMA) { GetSym(); VarDeclaration(LEV, TX, DX); }
				if (SYM == SEMICOLON) GetSym();
				else Error(5);
			} while (SYM == IDENT);
		}
		//过程
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
		// 检查当前符号是否合法
		TEST(SymSetAdd(IDENT, STATBEGSYS), DECLBEGSYS, 7);
	} while (SymIn(SYM, DECLBEGSYS));
	// 回填跳转地址
	CODE[TABLE[TX0].vp.ADR].A = CX;
	// 设置代码段的起始地址和数据段的大小
	TABLE[TX0].vp.ADR = CX;   /*START ADDR OF CODE*/
	TABLE[TX0].vp.SIZE = DX;  /*SIZE OF DATA SEGMENT*/
	// 生成初始化指令
	GEN(INI, 0, DX);
	STATEMENT(SymSetUnion(SymSetNew(SEMICOLON, ENDSYM), FSYS), LEV, TX);
	// 生成返回指令
	GEN(OPR, 0, 0);  /*RETURN*/
	// 检查符号集合
	TEST(FSYS, SymSetNULL(), 8);
	// 列出生成的代码
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