

#include "infixEvaluator.h"
#include "InfixToPostfix.h"
#include <iostream>

using namespace std;

void FullEvalInfixToPost(string& v)
{
	cout << "Infix:" << endl;
	cout << v << endl;
	cout << endl;

	InfixToPostfix j;

	cout << "Infix Condition:" << endl;
	v = j.inputStringConditioner(v);
	cout << v << endl;

	cout << endl;

	cout << "Infix to Postfix Convert:" << endl;
	v = j.convert(v);
	cout << v << endl;

	cout << "Postfix Eval:" << endl;
	cout << j.eval(v) << endl;

	cout << endl;

}

int main()
{
	string s("(-4 + -5) +++25");
	string t("(4 >= -5) + 25");
	string u("4 <= -5 + 25");
	string v("++++4   <= -++5 + 25 >= 37");

	string x("++++2-5*(3^2)");
	string y("2+2^2*3");
	string z("!!!3+2");


	InfixEvaluator i;

	cout << "Infix test:" << endl;
	cout << i.eval(s) << endl;
	cout << i.eval(t) << endl;
	cout << i.eval(u) << endl;
	cout << i.eval(v) << endl;
	cout << i.eval(x) << endl;
	cout << i.eval(y) << endl;
	cout << i.eval(z) << endl;

	cout << endl;


	

	FullEvalInfixToPost(s);
	FullEvalInfixToPost(t);
	FullEvalInfixToPost(u);
	FullEvalInfixToPost(v);
	FullEvalInfixToPost(x);
	FullEvalInfixToPost(y);
	FullEvalInfixToPost(z);


	char wait = getchar();
}