#pragma once

#include <QtWidgets/QWidget>
#include <cmath>
#include <cstdlib>
#include <string>
#include <QMessageBox>
#include <QRegularExpression>
#include <algorithm>
#include <QWT/qwt_plot.h> 
#include <QWT/qwt_plot_curve.h>  
#include <QList>
#include <QtMath>
#include "ui_ui.h"
using namespace std;

typedef double(*FuncPtr)(double);
enum Token_Type {
	ORIGIN, SCALE, ROT, IS, TO,
	STEP, DRAW, FOR, FROM, T,
	SEMICO,L_BRACKET, R_BRACKET, COMMA, 
	PLUS, MINUS, MUL, DIV, POWER,
	FUNC, CONST_ID, NONTOKEN, ERRTOKEN 
};


typedef struct Token{
	Token_Type type;
	char lexeme[100];
	double value;
	double(*funcptr)(double);
}Token_t;


static Token TokenTab[] = //符号表内容
{
	{ CONST_ID,	"PI", 3.1415926,	nullptr },
	{ CONST_ID, "E",	2.71828,	nullptr },
	{ T,		"T",		0.0,	nullptr },
	{ FUNC,		"SIN",		0.0,	sin },
	{ FUNC,		"COS",		0.0,	cos },
	{ FUNC,		"TAN",		0.0,	tan },
	{ FUNC,		"LN",		0.0,	log },
	{ FUNC,		"EXP",		0.0,	exp },
	{ FUNC,		"SQRT",		0.0,	sqrt },
	{ ORIGIN,	"ORIGIN",	0.0,	nullptr },
	{ SCALE,	"SCALE",	0.0,	nullptr },
	{ ROT,		"ROT",		0.0,	nullptr },
	{ IS,		"IS",		0.0,	nullptr },
	{ FOR,		"FOR",		0.0,	nullptr },
	{ FROM,		"FROM",		0.0,	nullptr },
	{ TO,		"TO",		0.0,	nullptr },
	{ STEP,		"STEP",		0.0,	nullptr },
	{ DRAW,		"DRAW",		0.0,	nullptr },
};

struct ExprNode {
	enum Token_Type Opcode;
	union {
		struct {
			ExprNode *Left, *Right;
		}CaseOperator;
		struct {
			ExprNode * Child;
			double(*funcptr)(double);
		}CaseFunc;
		double CaseConst;
		double *CaseParmPtr;
	}Content;
};

class UI : public QWidget
{
	Q_OBJECT

public:
	UI(QWidget *parent = Q_NULLPTR);

public slots:
	void quit();
	void clear();
	void procedure();

private:
	Ui::UIClass ui;
	QwtPlot *qwtPlot;
	string commands;
	Token_t token;
	QList<QwtPlotCurve*> curves;
	int cur;
	double scaleX, scaleY;
	double originx, originy;
	double rot_angle;
	double Parameter;
	char getc();//从输入中读取一个字符，同时将输入光标后移一位
	int ungetc();//将当前整体输入的光标位置前移一位
	int SyntaxError(int);//语法错误
    static Token getTokenType(char*);//判断token类型
	int Program();//程序
	int Statement();//语句
	int OriginStatement();//Origin语句
	int RotStatement();//Rot语句
	int ScaleStatement();//Scale语句
	int ForStatement();//For语句
	struct ExprNode *Expression();//表达式、二元加减运算表达式
	struct ExprNode *Term();//乘除运算表达式
	struct ExprNode *Factor();//一元加减运算表达式
	struct ExprNode *Component();//幂运算表达式
	struct ExprNode *Atom();//原子表达式

	int FetchToken();//调用词法分析器的GetToken，把得到的当前记录保存起来。如果得到的记号是非法输入errtoken，则指出一个语法错误
	int MatchToken(enum Token_Type The_Token);//匹配当前记号
	void ErrMsg(char *descrip, char *string);//打印错误信息

	double getExprValue(struct ExprNode*);//根据语法树得到值
	static void AddChar(char*, char);//将字符加入lexeme中
	int DelExprTree(struct ExprNode*);//删除一棵树
	struct ExprNode * MakeExprNode(enum Token_Type opcode, ...);//构造树的一个节点
	Token_t GetToken();//得到下一个token
	void DrawLoop(double Start,double End,	double Step, struct ExprNode *HorPtr,struct ExprNode *VerPtr);//画图
};
