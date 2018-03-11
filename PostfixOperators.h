#pragma once

#ifndef POSTFIX_OPERATOR_DEF_H
#define POSTFIX_OPERATOR_DEF_H
#include <string>

enum UNARY_OPERATOR_FLAG :unsigned int
{
	NONE = 0,
	NEGATIVE = 1,
	INCREMENT = 2,
	DECREMENT = 3,
	LOGICAL_NEGATION = 4
};



enum BINARY_OPERATOR_FLAG : unsigned int
{
	NO_FLAG = 0,
	LOGICAL_OR = 1,
	LOGICAL_AND = 2,
	EQUAL_TO = 3,
	NOT_EQUAL = 4,
	GREATER_OR_EQUAL = 5,
	LESS_OR_EQUAL = 6,

};

#endif
