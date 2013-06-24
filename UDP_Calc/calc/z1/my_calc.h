#include "stdafx.h"
#include <iostream>
#include <cmath>

using namespace std;
char stack[1000] = {0};
int index = 1;
void push(char ch)
{
	if (index == 999) throw "EERROR! Stack iverflow";
	++index;
	stack[index] = ch;
}
char pop()
{
	if (index <= 0) throw "ERROR! The stack is empty";
	--index;
	return stack[index+1];
}
char* RPN(char a[])
{
	int i = 0, j = 0;
	char out[1000];
	while (a[i])
	{
		if (a[i] == ' ') 
		{
			++i;
		}
		else
		{
			if (a[i] == '1' || a[i] == '2' || a[i] == '3' || a[i] == '4' || a[i] == '5' || a[i] == '.' ||
				a[i] == '6' || a[i] == '7' || a[i] == '8' || a[i] == '9' || a[i] == '0' || a[i] == 'x')
			{
				out[j] = a[i];
				++j;
				++i;
			}
			else
			{
				try{
					if (a[i] == '(') 
					{
						push(a[i++]);
					}
					if (a[i] == 's' || a[i] == 'c')
					{
						push(a[i++]);
						i+=2;
					}
					if (a[i] == 'l')
					{
						push(a[i]);
						i+=2;
					}
					if (a[i] == ')')
					{
						out[j++] = ' ';
						char tmp = pop();
						while (tmp != '(')
						{
							out[j++] = tmp;
							tmp = pop();
						}
						tmp = pop();
						if (tmp == 's' || tmp == 'c' || tmp == 'l' || tmp == '^')
							out[j++] = tmp;
						else
							push(tmp);
						++i;
					}
					if (a[i] == '^')
					{
						out[j++] = ' ';
						int tmp = pop();
						while (tmp == '^')
						{
							out[j] = tmp;
							++j;
							tmp = pop();
						}
						push(tmp);
						push(a[i]);
						++i;
					}
					if (a[i] == '*' || a[i] == '/')
					{
						out[j++] = ' ';
						int tmp = pop();
						while (tmp == '^' || tmp == '*' || tmp == '/')
						{
							out [j] = tmp;
							++j;
							tmp = pop();
						}
						push(tmp);
						push(a[i]);
						++i;
					}
					if (a[i] == '+' || a[i] == '-')
					{
						out[j++] = ' ';
						int tmp = pop();
						while (tmp == '^' || tmp == '*' || tmp == '/' || tmp == '+' || tmp == '-')
						{
							out [j] = tmp;
							++j;
							tmp = pop();
						}
						push(tmp);
						push(a[i]);
						++i;
					}
				}
				catch (char error)
				{
					cout << error;
				}
			}
		}
	}
	char tmp = pop();
	while (tmp)
	{
		out[j] = tmp;
		++j;
		tmp = pop();
	}
	out[j] = 0;
	return out;
}
double d_stack[1000] = {0};
int d_index = 0;
void push(double i)
{
	if (d_index == 999) throw "EERROR! Stack iverflow";
	++d_index;
	d_stack[d_index] = i;
}
double d_pop()
{
	if (d_index <= 0) throw "ERROR! The stack is empty";
	--d_index;
	return d_stack[d_index+1];
}


double calc(char a[], double x)
{
	int i = 0;
	while (a[i])
	{
		if (a[i] == ' ') ++i;
		else
		{
			if (a[i] == 'x') push(x); 
			if (a[i] == '1' || a[i] == '2' || a[i] == '3' || a[i] == '4' || a[i] == '5' ||
				a[i] == '6' || a[i] == '7' || a[i] == '8' || a[i] == '9' || a[i] == '0' ||a[i] == '.')
			{
				double tmp = 0;
				double k = 10;
				double h = 0;
				while (a[i] == '1' || a[i] == '2' || a[i] == '3' || a[i] == '4' || a[i] == '5' ||
					a[i] == '6' || a[i] == '7' || a[i] == '8' || a[i] == '9' || a[i] == '0' ||a[i] == '.')
				{
					tmp*=k;
					if (a[i] == '0') {h*=10;}
					if (a[i] == '1') {tmp+=1; h*=10;}
					if (a[i] == '2') {tmp+=2; h*=10;}
					if (a[i] == '3') {tmp+=3; h*=10;}
					if (a[i] == '4') {tmp+=4; h*=10;}
					if (a[i] == '5') {tmp+=5; h*=10;}
					if (a[i] == '6') {tmp+=6; h*=10;}
					if (a[i] == '7') {tmp+=7; h*=10;}
					if (a[i] == '8') {tmp+=8; h*=10;}
					if (a[i] == '9') {tmp+=9; h*=10;}
					if (a[i] == '.') {h = 1; tmp/=k;}
					++i;
				}
				if (h == 0) h = 1; 
				push(tmp/h);
			}
			else
			{
				if (a[i] == '+')
				{
					double a1 = d_pop();
					double b1 = d_pop();
					push(b1+a1);
				}
				if (a[i] == '-')
				{
					double a1 = d_pop();
					double b1 = d_pop();
					push(b1-a1);
				}
				if (a[i] == '*')
				{
					double a1 = d_pop();
					double b1 = d_pop();
					push(b1*a1);
				}
				if (a[i] == '/')
				{
					double a1 = d_pop();
					double b1 = d_pop();
					push(b1/a1);
				}
				if (a[i] == '^')
				{
					double a1 = d_pop();
					double b1 = d_pop();
					push(exp(a1*log(b1)));
				}
				if (a[i] == 's')
				{
					double a1 = d_pop();
					push(sin(a1));
				}
				if (a[i] == 'c')
				{
					double a1 = d_pop();
					push(cos(a1));
				}
				if (a[i] == 'l')
				{
					double a1 = d_pop();
					push(log(a1));
				}
				++i;
			}
		}
	}
	return d_pop();
}

double calculate(char* F, double x)
{
	char *out = RPN(F);
	return calc(out, x);
}

double calculate(char* F)
{
	return calculate(F, 0);
}