
#pragma once

#ifndef INFIX_EVALUATOR_H
#define INFIX_EVALUATOR_H

#include <stack>
#include <string>
#include "PostfixOperators.h"

using namespace std;



class InfixEvaluator
{
	// Private member functions
private:
	/** Evaluates the current operator.
	This function pops the two operands off the operand
	stack and applies the operator.
	@param op A character representing the operator
	@throws Syntax_Error if top is attempted on an empty stack
	*/
//	int eval_op(char op);



	/** Determines whether a character is an operator.
	@param ch The character to be tested
	@return true if the character is an operator
	*/
	bool is_operator(const char& c);

	int precedence(char op) const;

	// Data fields



//	const string InfixEvaluator::OPERATOR_LIST = "+-*/<>%^()";




	std::stack<int> operand_stack;
	std::stack<char> operator_stack;

	string findNegatives(string& s);

	void replaceLogicalOperators(string& expr);


public:
	/** Evaluates a postfix expression.
	@param expression The expression to be evaluated
	@return The value of the expression
	@throws Syntax_Error if a syntax error is detected
	*/
	int eval(const std::string& expression);

	/*  */
	void InfixEvaluator::runOperation();

	/*  */
	int InfixEvaluator::applyOperation(char op, int lhs, int rhs);

	BINARY_OPERATOR_FLAG operandDecoder(string& s);


	
};










#endif
