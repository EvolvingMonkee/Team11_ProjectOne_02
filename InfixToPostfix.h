#pragma once

#ifndef INFIX_POSTFIX_H
#define INFIX_POSTFIX_H

#include <stack>
#include <string>
#include "PostfixOperators.h"

using namespace std;

class InfixToPostfix
{

	std::string postfix;
	std::stack<int> operand_stack;
	std::stack<char> operator_stack;



	int precedence(char op) const;

	bool is_operator(const char& c);

	bool is_unary_operator(const char& c);


public:
	/* Postfix conversion */
	string convert(const string& expression);
	void process_operator(char op);
	string inputStringConditioner(const string& expression);
	void replaceLogicalOperators(string& expression);
	string findNegatives(string& s);
	BINARY_OPERATOR_FLAG operandDecoder(string& s);
	int eval(const std::string& expression);
	int eval_op(char op);
	void insertSpaces(string& s);



};


#endif