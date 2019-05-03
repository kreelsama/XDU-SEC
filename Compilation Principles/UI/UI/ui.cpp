#include "ui.h"
  

UI::UI(QWidget *parent)
	: QWidget(parent)
{
	this->setWindowTitle("JYH's plot function");
	ui.setupUi(this);
	qwtPlot = nullptr;
	connect(ui.btn_cancel, SIGNAL(clicked()), this, SLOT(quit()));
	connect(ui.btn_plot, SIGNAL(clicked()), this, SLOT(procedure()));
	connect(ui.btn_clr, SIGNAL(clicked()), this, SLOT(clear()));
	clear();
}

void UI::quit() {
	close();
}

void UI::procedure() {
	if (!qwtPlot) {
		qwtPlot = new QwtPlot(this);
		qwtPlot->setObjectName(QStringLiteral("qwtPlot"));
		qwtPlot->setGeometry(QRect(240, 80, 400, 200));
	}
	QString cmd = ui.cli->toPlainText();
	QRegularExpression pattern ("(//|--).*\n");
	commands.clear();
	//Regular expressions are actually cheating
	cmd = cmd.replace(pattern, " ");//remove quotations using RegularExpression
	cmd = cmd.replace("\n", " ");
	if (cmd.contains("//")) 
		cmd = cmd.split("//")[0];//remove trailing //
	if (cmd.contains("--"))
		cmd = cmd.split("--")[0];//remove trailing --  

	commands = cmd.toStdString();
	transform(commands.begin(), commands.end(), commands.begin(), toupper);
	cur = 0;
	if (FetchToken()) {//获取一个记号
		return;
	}
	Program();
}

void UI::ErrMsg(char* descrip, char* string) {
	char msg[256] {0};
	QMessageBox::information(this, descrip, string);
}

void UI::clear() {
	cur = 0;
	commands.clear();
	ui.cli->clear();
	scaleX = 1;
	scaleY = 1;
	originx = 0;
	originy = 0;
	rot_angle = 0;
	memset(&token, 0, sizeof(token));
	for(auto p:curves) {
		p->detach();
	}
	for(auto & p:curves) {
		delete p;
	}
	curves.clear();
	if (!qwtPlot) {
		delete qwtPlot;
		qwtPlot = nullptr;
	}
}

int UI::SyntaxError(int case_of) {
	switch (case_of){
	case 1: ErrMsg("Error Token ", token.lexeme);
		break;
	case 2: ErrMsg("Unexpected token", token.lexeme);
		break;
	default: break;
	}
	return 0;
}

char UI::getc() {
	char x = commands[cur];
	cur++;
	return x;
}

int UI::ungetc() {
	if (cur > 0)
		cur--;
	return 0;
} 

Token UI::getTokenType(char* buf) {
	int loop;
	for (loop = 0; loop < sizeof(TokenTab) / sizeof(TokenTab[0]); loop++)
	{	//遍历TokenTab表,找到了就返回正确的记号
		if (!strcmp(TokenTab[loop].lexeme, buf))
			return TokenTab[loop];
	}
	Token errortoken;
	memset(&errortoken, 0, sizeof(Token));
	errortoken.type = ERRTOKEN;
	return errortoken;//返回出错记号
}

int  UI::Program() {
	int ret = 0;
	while(token.type!= NONTOKEN) { 
		if (Statement())
			return 1;
	}
	return 0;
}

int UI::Statement() {
	int ret;
	switch (token.type)
	{
	case ORIGIN: ret = OriginStatement(); break;
	case SCALE: ret = ScaleStatement(); break;
	case ROT:  ret = RotStatement(); break;
	case FOR: ret = ForStatement(); break;
	default: SyntaxError(2); return 1; //否则报错
	}
	return ret;
}

double UI::getExprValue(struct ExprNode *root)//参数是表达式的根
{//后续遍历语法树  根据不同的节点类型计算当前根节点的值
	if (root == NULL) return 0.0;
	switch (root->Opcode)
	{
		//二元运算符
	case PLUS:
		return getExprValue(root->Content.CaseOperator.Left) +
			getExprValue(root->Content.CaseOperator.Right);
	case MINUS:
		return getExprValue(root->Content.CaseOperator.Left) -
			getExprValue(root->Content.CaseOperator.Right);
	case MUL:
		return getExprValue(root->Content.CaseOperator.Left) *
			getExprValue(root->Content.CaseOperator.Right);
	case DIV:
		return getExprValue(root->Content.CaseOperator.Left) /
			getExprValue(root->Content.CaseOperator.Right);
	case POWER:
		return pow(getExprValue(root->Content.CaseOperator.Left),
			getExprValue(root->Content.CaseOperator.Right));
	case FUNC:
		return(*root->Content.CaseFunc.funcptr)
			(getExprValue(root->Content.CaseFunc.Child));
	case CONST_ID:
		return root->Content.CaseConst;
	case T:
		return *(root->Content.CaseParmPtr);
	default:
		return 0.0;
	}//返回值是表达式的值
}

Token_t UI::GetToken() {
	Token token;
	char ch;
	char tokenbuffer[100] {0};
	memset(&token, 0, sizeof(token));
	while (true) {
		ch = getc();
		if (cur == commands.length())//读出错
		{
			token.type = NONTOKEN;
			return token;
		}
		if (!isspace(ch)) {
			AddChar(tokenbuffer, ch);
			break;
		}
	}
	if (isalpha(ch)) {//如果是字母，则可能是保留字、函数、PI、E、T
		while (true) {
			ch = getc();
			if (isalnum(ch)) {
				AddChar(tokenbuffer, ch);
			}
			else {
				ungetc();
				break;
			}
		}
		token = getTokenType(tokenbuffer);
		strcpy(token.lexeme, tokenbuffer);
		return token;
	}
	else if(isdigit(ch)){//如果是数字，则只可能是数字、小数
		while (true) {
			ch = getc();
			if (isdigit(ch)) {
				AddChar(tokenbuffer, ch);
			}
			else
				break;
		}
		if (ch == '.') {
			AddChar(tokenbuffer, ch);
			while (true) {
				ch = getc();
				if (isdigit(ch)) {
					AddChar(tokenbuffer, ch);
				}
				else break;
			}
		}
		else
			ungetc();
		token.type = CONST_ID;//一定是常量
		strcpy(token.lexeme, tokenbuffer);
		token.value = atof(tokenbuffer);
		return token;
	}
	else {
		switch(ch) {
		case ';':token.type = SEMICO; AddChar(tokenbuffer, ch); break;
		case '(':token.type = L_BRACKET; AddChar(tokenbuffer, ch); break;
		case ')':token.type = R_BRACKET; AddChar(tokenbuffer, ch); break;
		case '+':token.type = PLUS; AddChar(tokenbuffer, ch); break;
		case '/':token.type = DIV; AddChar(tokenbuffer, ch); break;
		case ',': token.type = COMMA; AddChar(tokenbuffer, ch); break;
		case '-':token.type = MINUS; AddChar(tokenbuffer, ch); break;
		case '*':
			AddChar(tokenbuffer, ch);
			ch = getc();
			if(ch == '*') {
				token.type = POWER;
				AddChar(tokenbuffer, ch);
				break;
			}
			token.type = MUL;
			ungetc();
			break;
		default:token.type = NONTOKEN; break;
		}
		strcpy(token.lexeme, tokenbuffer);
		return token;
	}
}

int UI::FetchToken() {
	token = GetToken();
	if (token.type == NONTOKEN && cur < commands.length()) {
		SyntaxError(2);
		return 1;
	}
	return 0;
}

int UI::MatchToken(Token_Type The_Token) {
	if (cur == commands.length())
		return 0;
	if(token.type != The_Token) {
		SyntaxError(2);
		return 1;
	}
	if(cur < commands.length())
		return FetchToken();//得到下一个token
	return 0;
}

void UI::AddChar(char *buf, char ch) {//把符号ch加入到缓冲区buf中
	int i = 0;
	for (i = 0; buf[i] != 0; i++) ;
	buf[i] = ch;
}

int UI::DelExprTree(ExprNode* root) {
	if (root == nullptr) return 0;
	switch (root->Opcode)
	{
	case PLUS://二元：：两个孩子的内部节点
	case MINUS:
	case MUL:
	case DIV:
	case POWER:
		DelExprTree(root->Content.CaseOperator.Left);
		DelExprTree(root->Content.CaseOperator.Right);
		break;
	case FUNC:
		DelExprTree(root->Content.CaseFunc.Child);//一元：：一个孩子的内部节点
		break;
	default:
		break;
	}
	delete root;
	return 0;
}

struct ExprNode* UI::Expression() {
	struct ExprNode *left, *right;//左右子树节点指针
	Token_Type token_tmp;//当前记号

	left = Term();//分析左操作数得到其语法树，左操作数是一个乘除运算表达式
	while (token.type == PLUS || token.type == MINUS){
		token_tmp = token.type;//当前记号是加/减
		MatchToken(token_tmp);//匹配记号
		right = Term();//分析右操作数得到其语法树，右操作数是一个乘除运算表达式
		left = MakeExprNode(token_tmp, left, right);//构造运算的语法树，结果为左子树
	}
	return left;
}

struct ExprNode *UI::Term(){
	struct ExprNode *left, *right;
	Token_Type token_tmp;
	left = Factor();
	while (token.type == MUL || token.type == DIV)
	{
		token_tmp = token.type;
		MatchToken(token_tmp);
		right = Factor();
		left = MakeExprNode(token_tmp, left, right);
	}
	return left;
}

struct ExprNode* UI::MakeExprNode(enum Token_Type opcode, ...) {
	struct ExprNode *ExprPtr = new(struct ExprNode);//为新节点开辟空间
	ExprPtr->Opcode = opcode;//向节点写入记号类别
	va_list ArgPtr;//指向可变函数的参数的指针
	va_start(ArgPtr, opcode);//初始化va_list变量，第一个参数也就是固定参数为opcode
	switch (opcode)//根据记号的类别构造不同的节点
	{
	case CONST_ID://常数节点
		ExprPtr->Content.CaseConst = (double)va_arg(ArgPtr, double);//返回可变参数，可变参数类型是常数
		break;
	case T://参数T节点
		ExprPtr->Content.CaseParmPtr = &Parameter;//返回可变参数，可变参数类型是参数T
		break;
	case FUNC://函数调用节点
		ExprPtr->Content.CaseFunc.funcptr = (FuncPtr)va_arg(ArgPtr, FuncPtr);//可变参数类型是对应函数的指针
		ExprPtr->Content.CaseFunc.Child = (struct ExprNode *)va_arg(ArgPtr, struct ExprNode *);//可变参数类型是节点
		break;
	default://二元运算节点
		ExprPtr->Content.CaseOperator.Left = (struct ExprNode *)va_arg(ArgPtr, struct ExprNode *);//可变参数类型是节点
		ExprPtr->Content.CaseOperator.Right = (struct ExprNode *)va_arg(ArgPtr, struct ExprNode *);//同上
		break;
	}
	va_end(ArgPtr);//结束可变参数的获取

	return ExprPtr;
}

int UI::RotStatement() {
	struct ExprNode *tmp;
	if (MatchToken(ROT))
		return 1;
	if (MatchToken(IS)) 
		return 1;

	tmp = Expression();  //Tree_trace(tmp);
	if (!tmp)
		return 1; 
	rot_angle = getExprValue(tmp);//获得旋转角度
	DelExprTree(tmp);
	if (MatchToken(SEMICO))
		return 1;
	return 0;
}

int UI::ScaleStatement(void)
{
	struct ExprNode *tmp;
	if (MatchToken(SCALE))
		return 1;
	if (MatchToken(IS))
		return 1;
	if (MatchToken(L_BRACKET))
		return 1;

	tmp = Expression();  //Tree_trace(tmp);
	if (!tmp)
		return 1;
	scaleX = getExprValue(tmp);//获取横坐标的比例因子
	DelExprTree(tmp);

	if (MatchToken(COMMA))
		return 1;

	tmp = Expression();    //Tree_trace(tmp);
	if (!tmp)
		return 1;
	scaleY = getExprValue(tmp);//获取纵坐标的比例因子
	DelExprTree(tmp);

	if (MatchToken(R_BRACKET))
		return 1;

	if (MatchToken(SEMICO))
		return 1;
	return 0;
}

int UI::ForStatement(void){//for T from Expression to Expression draw (Expression, Expression)
	double Start, End, Step;//绘图起点、终点、步长
	struct ExprNode *start_ptr, *end_ptr, *step_ptr, *x_ptr, *y_ptr;
	if (MatchToken(FOR))
		return 1;
	if (MatchToken(T))
		return 1;
	if (MatchToken(FROM))
		return 1;
	start_ptr = Expression(); //Tree_trace(start_ptr);//获得参数起点表达式的语法树
	if (!start_ptr)
		return 1;
	Start = getExprValue(start_ptr);//计算参数起点表达式的值
	DelExprTree(start_ptr);//释放参数起点语法树所占空间
	if (MatchToken(TO))
		return 1;
	end_ptr = Expression(); //Tree_trace(end_ptr);//构造参数终点表达式语法树
	if (!end_ptr)
		return 1;
	End = getExprValue(end_ptr);//计算参数终点表达式的值
	DelExprTree(end_ptr);//释放参数终点语法树所占空间
	if (MatchToken(STEP))
		return 1;

	step_ptr = Expression(); //Tree_trace(step_ptr);//构造参数步长表达式语法树
	if (!step_ptr)
		return 1;
	Step = getExprValue(step_ptr);//计算参数步长表达式的值
	DelExprTree(step_ptr);//释放参数步长语法树所占空间

	if (MatchToken(DRAW))
		return 1;
	if (MatchToken(L_BRACKET))
		return 1;

	x_ptr = Expression(); //Tree_trace(x_ptr);
	if (!x_ptr)
		return 1;

	if (MatchToken(COMMA))
		return 1;

	y_ptr = Expression();//Tree_trace(y_ptr);
	if (!y_ptr)
		return 1;

	if (MatchToken(R_BRACKET))
		return 1;

	if (MatchToken(SEMICO))
		return 1;
	DrawLoop(Start, End, Step, x_ptr, y_ptr); //绘制图形
	DelExprTree(x_ptr);//释放横坐标语法树所占空间
	DelExprTree(y_ptr);//释放纵坐标语法树所占空间

	return 0;
}

int UI::OriginStatement() {
	struct ExprNode *tmp;//语法树节点的类型
	MatchToken(ORIGIN);
	if (MatchToken(IS)) {
		return 1;
	}
	if (MatchToken(L_BRACKET)) {
		return 1;
	}

	tmp = Expression();  //Tree_trace(tmp);
	if (!tmp)
		return 1;
	originx = getExprValue(tmp);//获取横坐标点平移距离
	DelExprTree(tmp);//删除一棵树

	if (MatchToken(COMMA))
		return 1;

	tmp = Expression();   //Tree_trace(tmp);
	if (!tmp)
		return 1;
	originy = getExprValue(tmp);//获取纵坐标点平移距离
	DelExprTree(tmp);//删除一棵树

	if (MatchToken(R_BRACKET))
		return 1;

	if (MatchToken(SEMICO))
		return 1;
	return 0;
}

struct ExprNode *UI::Component()//右结合
{
	struct ExprNode *left, *right;
	left = Atom();
	if (!left)
		return nullptr;
	if (token.type == POWER)
	{
		MatchToken(POWER);
		right = Component();//递归调用Component以实现POWER的右结合
		left = MakeExprNode(POWER, left, right);
		if (!left)
			return nullptr;
	}
	return left;
}

struct ExprNode *UI::Atom(){
	struct Token t = token;
	struct ExprNode *address, *tmp;
	switch (token.type)
	{
	case CONST_ID:
		MatchToken(CONST_ID);
		address = MakeExprNode(CONST_ID, t.value);
		break;
	case T:
		MatchToken(T);
		address = MakeExprNode(T);
		break;
	case FUNC:
		MatchToken(FUNC);
		MatchToken(L_BRACKET);
		tmp = Expression();  //Tree_trace(tmp);
		address = MakeExprNode(FUNC, t.funcptr, tmp);
		MatchToken(R_BRACKET);
		break;
	case L_BRACKET:
		MatchToken(L_BRACKET);
		address = Expression(); //Tree_trace(address);
		MatchToken(R_BRACKET);
		break;
	default:
		SyntaxError(2);
		return nullptr;
	}
	return address;
}

ExprNode* UI::Factor() {
	struct ExprNode *left, *right;
	if (token.type == PLUS) {//匹配一元加运算
		MatchToken(PLUS);
		right = Factor();
	}
	else if (token.type == MINUS)//表达式退化为仅有右操作数的表达式
	{
		MatchToken(MINUS);
		right = Factor();
		left = new ExprNode;
		left->Opcode = CONST_ID;
		left->Content.CaseConst = 0.0;
		right = MakeExprNode(MINUS, left, right);
	}
	else right = Component();//匹配非终结符Component
	return right;
}

void UI::DrawLoop(double Start,double End,double Step,struct ExprNode *HorPtr,struct ExprNode *VerPtr){
	double x, y;
	QVector<QPointF> pts;
	double tmp;
	for (Parameter = Start; Parameter <= End; Parameter += Step){
		QPointF point;
		x = y = 0;
		x = getExprValue(HorPtr);
		y = getExprValue(VerPtr);
		x *= scaleX;
		y *= scaleY;
		tmp = x * cos(rot_angle) + y * sin(rot_angle);
		y = y * cos(rot_angle) - x * sin(rot_angle);
		x = tmp;
		x += originx;
		y += originy;
		point.setX(x);
		point.setY(y);
		pts.append(point);
	}
	QwtPointSeriesData * data = new QwtPointSeriesData(pts);
	QwtPlotCurve *curve = new QwtPlotCurve("Curve Plot");
	curves.append(curve);
	curve->setData(data);
	curve->attach(qwtPlot);
	qwtPlot->replot();
	qwtPlot->show();
}