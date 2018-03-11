#include "InfixEvaluator.h"

#include <sstream>
#include <cctype>
using std::string;
using std::istringstream;

const char BINARY_OPERATOR[] = { 'x','O','A','E','N','G','L' };
const std::string OPERATORS = "OAENGL><+-*/%^()MUD!";
const int PRECEDENCE[] = { 1, 2, 3, 3, 4, 4, 4, 4, 5, 5, 6, 6, 6, 7, -1, -1, 8, 8, 8, 8 };
const std::string TWO_DIGIT_BINARY_OP = "||&&==!=>=<=";




bool InfixEvaluator::is_operator(const char& c)
{
	return (OPERATORS.find(c) != std::string::npos);
}

int InfixEvaluator::precedence(char op) const {
	return PRECEDENCE[OPERATORS.find(op)];
}


BINARY_OPERATOR_FLAG InfixEvaluator::operandDecoder(string& s)
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

UNARY_OPERATOR_FLAG unaryCharToFlag(const char& c)
{
	if (c == 'M') return NEGATIVE;
	if (c == 'U') return INCREMENT;
	if (c == 'D') return DECREMENT;
	if (c == '!') return LOGICAL_NEGATION;


}

int unaryOperation(int& i, const UNARY_OPERATOR_FLAG& flag)
{
	switch (flag)
	{
	case NEGATIVE:
		i = 0 - i;
		break;
	case INCREMENT:
		i+=1;
		break;
	case DECREMENT:
		i -= 1;
		break;
	case LOGICAL_NEGATION:
		if (i != 0)
			i = 0;
		else
			i = 1;
		break;

	}
	return i;

}


void InfixEvaluator::runOperation()
{
	int rhs = operand_stack.top();
	operand_stack.pop();
	int lhs = operand_stack.top();
	operand_stack.pop();
	operand_stack.push(applyOperation(operator_stack.top(), lhs, rhs));
	operator_stack.pop();

}




int InfixEvaluator::applyOperation(char op, int lhs, int rhs)
{
	/* TODO: Complete all operations */
	switch (op)
	{
	case '+': return lhs + rhs;
	case '-': return lhs - rhs;
	case '*': return lhs * rhs;
	case '/': return lhs / rhs;
	case '%': return lhs % rhs;
	case 'G': return lhs >= rhs;
	case 'L': return lhs <= rhs;
	case '<': return lhs < rhs;
	case '>': return lhs > rhs;
	case 'O': return lhs || rhs;
	case 'A': return lhs && rhs;
	case 'N': return lhs != rhs;
	case 'E': return lhs == rhs;
	case '^': return static_cast<int>(pow(lhs, rhs));

		/*TODO: finish*/



	}

	return 0;

}



void InfixEvaluator::replaceLogicalOperators(string& expr)
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



/**
Replaces Negatives with 'M' to differentiate negatives from minus operator
@param string for parsing
*/
string InfixEvaluator::findNegatives(string& s)
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



/** Evaluates an infix expression.
@param expression The expression to be evaluated
@return The value of the expression
@throws Syntax_Error if a syntax error is detected
*/
int InfixEvaluator::eval(const std::string& expression)
{
	// Be sure the operand_stack is empty
	while (!operand_stack.empty())
		operand_stack.pop();
	// Be sure the operator_stack is empty
	while (!operator_stack.empty())
		operator_stack.pop();

	/* Result */
	int result = 0;
	/* String to parse through, add a buffer char*/
	string expr = expression + " ";
	/* Iterator for string parsing */
	size_t position = 0;
	/* String for operators and operands */
	string next_chars = "";
	UNARY_OPERATOR_FLAG unaryOperatorFlag = NONE;

	expr = findNegatives(expr);
	replaceLogicalOperators(expr);

	/* Loop through string */
	while (position < expr.size() - 1)
	{
		string nextTwo = expr.substr(position, 2);

		/* Ignore whitespace */
		if (expr.at(position) == ' ')
		{
			/* Do nothing */
		}
		else if (isdigit(expr.at(position)))
		{
			int op = strtol(&expr.at(position), NULL, 10);
			while (isdigit(expr.at(position + 1)) && position < expr.size() - 1)
				position++;

			while (!operator_stack.empty() && precedence(operator_stack.top()) == 8)
			{
				op = unaryOperation(op, unaryCharToFlag(operator_stack.top()));
				operator_stack.pop();
			}

			operand_stack.push(op);
		}


		/* Handle opening brace*/
		else if (expr.at(position) == '(')
			operator_stack.push('(');

		/* Handle closing brace*/
		else if (expr.at(position) == ')')
		{
			/* todo: check for error*/
			{
			
			}

			/* operator*/
			while (operator_stack.top() != '(')
			{
				runOperation();
			}
			operator_stack.pop();

		}

		/* Handle all other operators */
		else if (is_operator(expr.at(position)))
		{
			while (!operator_stack.empty() && 
					precedence(operator_stack.top()) >= precedence(expr.at(position)) &&
					precedence(operator_stack.top()) != 8)
			{
				runOperation();
			}

			operator_stack.push(expr.at(position));

		
		}




		++position;
	}

	while (!operator_stack.empty())
	{
		runOperation();
	}

	result = operand_stack.top();



	return result;

}




