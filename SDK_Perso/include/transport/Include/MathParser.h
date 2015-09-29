#pragma once

#include "ed/vector.h"
#include "ed/string.h"

#define MAX_EXPR_LEN  255
#define MAX_TOKEN_LEN 80

struct StackElement 
{
	StackElement()
	{
		type = 0;
		opcode = 0;
		value = 0.0f;
	}

	StackElement(int t, int o, float v)
	{
		type = t;
		opcode = o;
		value = v;
	}

	int type;
	int opcode;
	float value;
};

struct ParserNode
{
	ParserNode()
	{
		value = 0.0f;
		left = nullptr;
		right = nullptr;
	}

	ParserNode(float v, ParserNode* l, ParserNode* r)
	{
		value = v;
		left = l;
		right = r;
	}

	float value;
	ParserNode* left;
	ParserNode* right;
};

struct Error
{
	Error()
	{
		error = nullptr;
		position = 0;
	}

	Error(char* e, int p)
	{
		error = e;
		position = p;
	}

	char* error;
	int position;
};

class MathParser
{
public:
	MathParser();
	MathParser(const ed::string &e);
	~MathParser();

	void postfixParseExpression();
	float postfixEvaluate(float x);

	// Использует кэширование

	float postfixEvaluate(int x, float K1);

	// Использует кэширование

public:
	bool infixCompile(char* e);
	void infixDecompile();

	float infixEveluate();
	float infixEveluate(float* newX);
	float infixEveluate(float newX, ...);
	float infixGetResult();

private:
	enum operationCodes
	{
		ADD = 1,
		SUB = 2,
		MUL = 4,
		DIV = 8,
		POW = 16,
		EXP = 32,
		VAR = 64,
		VAL = 128,
		OPR = 256,
		ERR = 512
	};

	ed::vector<StackElement> stackElements;
	ed::vector<float> resultStack;

	float firstOperand;
	float secondOperand;
	float postfixResult;

	ed::string postfixExpression;

	StackElement postfixMakeStackElement(int t, int o, float v);

private:
	enum
	{
		PARSER_PLUS,
		PARSER_MINUS,
		PARSER_MULTIPLY,
		PARSER_DIVIDE,
		PARSER_PERCENT,
		PARSER_POWER,
		PARSER_SIN,
		PARSER_COS,
		PARSER_TG,
		PARSER_CTG,
		PARSER_ARCSIN,
		PARSER_ARCCOS,
		PARSER_ARCTG,
		PARSER_ARCCTG,
		PARSER_SH,
		PARSER_CH,
		PARSER_TH,
		PARSER_CTH,
		PARSER_EXP,
		PARSER_LG,
		PARSER_LN,
		PARSER_SQRT,
		PARSER_X,
		PARSER_L_BRACKET,
		PARSER_R_BRACKET,
		PARSER_E,
		PARSER_PI,
		PARSER_NUMBER,
		PARSER_END
	} tokenType;

	ParserNode* root;

	char* infixExpression;
	char currentToken[MAX_TOKEN_LEN];

	int position;

	const float* x;
	float infixResult;

	std::vector<ParserNode*> history;

	ParserNode* createNode(float v = 0.0f, ParserNode* l = nullptr, ParserNode* r = nullptr);

	ParserNode* expressionMain();
	ParserNode* expressionOne();
	ParserNode* expressionTwo();
	ParserNode* expressionThree();
	ParserNode* expressionFour();
	ParserNode* expressionFive();

	bool getToken();
	bool isDelimiter();
	bool isLetter();
	bool isDigit();
	bool isPoint();

	float calcTree(ParserNode* tree);
	void deleteTree(ParserNode* tree);

	void setX(const float* newX);

	void sendError(int errorCode);

private:
	ed::vector<float> cacheVector;
	bool cache;
};