
#include "InfixToPostfix.h"

#include <sstream>
#include <cctype>
#include <cmath>

using std::string;
using std::istringstream;

const char BINARY_OPERATOR[7] = { 'x','O','A','E','N','G','L' };
const std::string OPERATORS = "OAENGL><+-*/%^()";
const int PRECEDENCE[16] = { 1, 2, 3, 3, 4, 4, 4, 4, 5, 5, 6, 6, 6, 7, -1, -1 };
const std::string TWO_DIGIT_BINARY_OP = "||&&==!=>=<=";
const std::string UNARY_OPERATORS = "MUD";



BINARY_OPERATOR_FLAG InfixToPostfix::operandDecoder(string& s)
{
	BINARY_OPERATOR_FLAG result = NO_FLAG;
	if (s == "||") result = LOGICAL_OR;
	else if (s == "&&") result = LOGICAL_AND;
	else if (s == "==") result = EQUAL_TO;
	else if (s == "!=") result = NOT_EQUAL;
	else if (s == ">=") result = GREATER_OR_EQUAL;
	else if (s == "<=") result = LESS_OR_EQUAL;

	return result;
}

int InfixToPostfix::precedence(char op) const {
	return PRECEDENCE[OPERATORS.find(op)];
}

bool InfixToPostfix::is_operator(const char& c)
{
	return (OPERATORS.find(c) != std::string::npos);
}

bool InfixToPostfix::is_unary_operator(const char& c)
{
	return (UNARY_OPERATORS.find(c) != std::string::npos);
}


string InfixToPostfix::convert(const string& expression)
{
	postfix = "";
	while (!operator_stack.empty())
		operator_stack.pop();

	char unaryOperatorFlag = 0;

	istringstream infix_tokens(expression);
	string next_token;

	while (infix_tokens >> next_token) 
	{
		if (is_operator(next_token[0])) {
			process_operator(next_token[0]);
		}
		else if (isalnum(next_token[0]) ) {

			postfix += next_token;
			postfix += " ";
		}
		else {
			throw invalid_argument("Unexpected Character Encountered");
		}
	} // End while
	  // Pop any remaining operators and append them to postfix
	while (!operator_stack.empty()) {
		char op = operator_stack.top();
		operator_stack.pop();
		if (op == '(' || op == '[' || op == '}') {
			throw invalid_argument("Unmatched open parenthesis");
		}
		postfix += op;
		postfix += " ";
	}
	return postfix;


}


/** Function to process operators.
@param op The operator
@throws Syntax_Error
*/
void InfixToPostfix::process_operator(char op) {
	if (operator_stack.empty() || (op == '(') || (op == '[') || (op == '{')) {
		if (op == ')' || op == ']' || op == '}')
			throw invalid_argument("Unmatched close parenthesis");
		operator_stack.push(op);
	}
	else {
		if (precedence(op) > precedence(operator_stack.top())) {
			operator_stack.push(op);
		}
		else {
			// Pop all stacked operators with equal
			// or higher precedence than op.
			while (!operator_stack.empty()
				&& (operator_stack.top() != '(')
				&& (operator_stack.top() != '[')
				&& (operator_stack.top() != '{')
				&& (precedence(op) <= precedence(operator_stack.top()))) {
				postfix += operator_stack.top();
				postfix += " ";
				operator_stack.pop();
			}
			// assert: Operator stack is empty or 
			//         top of stack is '(' or current
			//         operator precedence > top of stack operator
			//         precedence;
			if (op == ')') {
				if (!operator_stack.empty()
					&& (operator_stack.top() == '(')) {
					operator_stack.pop();
				}
				else {
					throw invalid_argument("Unmatched close parentheses");
				}
			}
			else if (op == ']') {
				if (!operator_stack.empty()
					&& (operator_stack.top() == '[')) {
					operator_stack.pop();
				}
				else {
					throw invalid_argument("Unmatched close parentheses");
				}
			}
			else if (op == '}') {
				if (!operator_stack.empty()
					&& (operator_stack.top() == '{')) {
					operator_stack.pop();
				}
				else {
					throw invalid_argument("Unmatched close parentheses");
				}
			}
			else {
				operator_stack.push(op);
			}
		}
	}
}

int getConsecutiveChars(const string& s)
{
	string::const_iterator itr = s.begin();
	char c = *itr;
	while (itr != s.end() && *itr == c)
		++itr;
	return itr - s.begin();
}

void InfixToPostfix::replaceLogicalOperators(string& expr)
{
	size_t index = 0;
	while (index < expr.size())
	{
		string nextTwo = expr.substr(index, 2);
		string nextFour = expr.substr(index, 4);
		BINARY_OPERATOR_FLAG fl = operandDecoder(nextTwo);
		if (fl != NONE)
		{
			expr.at(index) = OPERATORS[fl - 1];
			expr.at(index + 1) = ' ';
			++index;
		}
		else if (nextTwo == "++" && nextFour.at(2) != '+' || nextFour == "++++")
		{
			expr.at(index) = 'U';
			expr.at(index + 1) = ' ';
			++index;
		}
		else if (nextTwo == "--" && nextFour.at(2) != '-' || nextFour == "----")
		{
			expr.at(index) = 'D';
			expr.at(index + 1) = ' ';
			++index;
		}


		++index;
	}


}

void InfixToPostfix::insertSpaces(string& s)
{
	size_t index = 0;
	while (index < s.size() - 1)
	{
		/* If next char is not a whitespace, determine if it should be and insert it*/
		if (s.at(index + 1) != ' ')
		{
			if ( (isdigit(s.at(index)) && !isdigit(s.at(index + 1))) || is_operator(s.at(index)) || isalpha(s.at(index)) ) 
			{
				s.insert(index + 1, " ");
			}
		}

		index++;
	}


}

string InfixToPostfix::inputStringConditioner(const string& expression)
{
	/* String to parse through, add a buffer char*/
	string expr = expression + " ";

	/* Find Negatives and Change to 'M' */
	expr = findNegatives(expr);

	/* Replace logical operators with one-letter code*/
	replaceLogicalOperators(expr);

	insertSpaces(expr);

	return expr;

}



/**
Replaces Negatives with 'M' to differentiate negatives from minus operator
@param string for parsing
*/
string InfixToPostfix::findNegatives(string& s)
{
	string::iterator itr = s.begin();
	if (*itr == '-' && isdigit(*(itr + 1)))
	{
		*itr = 'M';
	}


	int previousOperatorFlag = 0;

	while (itr != s.end())
	{
		if (is_operator(*(itr)) && *(itr) != '-')
		{
			previousOperatorFlag = 1;
		}
		else if (isdigit(*(itr)))
		{
			previousOperatorFlag = 0;
		}

		if (*itr == '-')
		{
			if (previousOperatorFlag != 0)
			{
				*itr = 'M';
			}
		}


		++itr;
	}

	return s;

}



/** Evaluates a postfix expression.
@param expression The expression to be evaluated
@return The value of the expression
@throws Syntax_Error if a syntax error is detected
*/
int InfixToPostfix::eval(const std::string& expression) {
	// Be sure the stack is empty
	while (!operand_stack.empty())
		operand_stack.pop();

	// Process each token
	istringstream tokens(expression);
	char next_char;
	int incDecFlag = 0;
	int negFlag = 0;
	while (tokens >> next_char) {
		if (isdigit(next_char)) {
			tokens.putback(next_char);
			int value;
			tokens >> value;
			if (negFlag)
			{
				value = 0 - value;
				negFlag = 0;
			}
			value += incDecFlag;
			incDecFlag = 0;
			operand_stack.push(value);
		}
		else if (is_operator(next_char)) {
			int result = eval_op(next_char);
			operand_stack.push(result);
		}
		else if (next_char == 'M')
			negFlag = 1;
		else if (next_char == 'U')
			incDecFlag++;
		else if (next_char == 'D')
			incDecFlag--;
		else {
			throw invalid_argument("Invalid character encountered");
		}
	}
	if (!operand_stack.empty()) {
		int answer = operand_stack.top();
		operand_stack.pop();
		if (operand_stack.empty()) {
			return answer;
		}
		else {
			throw invalid_argument("Stack should be empty");
		}
	}
	else {
		throw invalid_argument("Stack is empty");
	}
}



/** Evaluates the current operator.
This function pops the two operands off the operand
stack and applies the operator.
@param op A character representing the operator
@throws Syntax_Error if top is attempted on an empty stack
*/
int InfixToPostfix::eval_op(char op) {
	if (operand_stack.empty())
		throw invalid_argument("Stack is empty");
	int rhs = operand_stack.top();
	operand_stack.pop();
	if (operand_stack.empty())
		throw invalid_argument("Stack is empty");
	int lhs = operand_stack.top();
	operand_stack.pop();
	int result = 0;
	switch (op) {
	case '+': result = lhs + rhs;
		break;
	case '-': result = lhs - rhs;
		break;
	case '*': result = lhs * rhs;
		break;
	case '/': result = lhs / rhs;
		break;
	case '%': result = lhs % rhs;
		break;
	case 'G': result = lhs >= rhs;
		break;
	case 'L': result = lhs <= rhs;
		break;
	case '<': result = lhs < rhs;
		break;
	case '>': result = lhs > rhs;
		break;
	case 'O': result = lhs || rhs;
		break;
	case 'A': result = lhs && rhs;
		break;
	case 'N': result = lhs != rhs;
		break;
	case 'E': result = lhs == rhs;
		break;
	case '^': result = pow(lhs,rhs);
		break;

	}
	return result;
}