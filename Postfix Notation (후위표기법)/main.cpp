// ���� ǥ���
/*
	���� ǥ����� �־����� �� ���� ǥ������� ��ġ��
	��갪 ����ϱ�
	ex) 
	�Է�
	2+3*(5+4*2)-3+6
	���
	23542*+*+3-6+
*/

#include <iostream>
#include <string>
#include <vector>
#include <stack>
using namespace std;

vector<char> v; // ���� ������ ���� ����
stack<char> st; // ������ ����
string infix; // ����ǥ���
string postfix; // ����ǥ���

//stack<int> st2; // ����� ���� stack

// stack ����
////////////////
int Stack[101], top = -1;

void push(int x) {
	Stack[++top] = x;
}

int empty() {
	if (top < 0) return 1;
	else return 0;
}

void pop() {
	if (!empty()) Stack[top--] = 0;
}

char peek() {
	if (!empty()) return Stack[top];
	else return 0;
}
/////////////////

int getPriority(char op) {
	
	if (op == '*' || op == '/') return 3;
	if (op == '+' || op == '-') return 2;
	if (op == '(') return 1;

	//if (op >= '0' && op <= '9') return -1;
	return -1;
}

void convertPostfix() {
	for (int i = 0; i < infix.length(); i++) {
		char ch = infix[i];
		// ����
		if (ch >= '0' && ch <= '9')
			v.push_back(infix[i]);
		else {
			// '(' or ������ ������ ���������
			if (ch == '(' || st.empty())
				st.push(ch);
			// ')'
			else if (ch == ')') {
				char op = st.top();
				st.pop();
				while (op != '(') {
					v.push_back(op);
					op = st.top();
					st.pop();
				}
			}
			// '+', '-', '-', '/' : 
			// ���� �����ڰ� ������ ������ ž���� ����ġ�� ���� ��
			else if (getPriority(ch) > getPriority(st.top()))
				st.push(ch);
			// '+', '-', '-', '/' : 
			// ���� �����ڰ� ������ ������ ž���� ����ġ�� ���ų� ���� ��
			else {
				while (!st.empty() &&
					getPriority(ch) <= getPriority(st.top())) {
					v.push_back(st.top());
					st.pop();
				}
				st.push(ch);
			}
		}
	}
	// ���� �������� ���� ������ ���� ǥ��Ŀ� �ֱ�
	while (!st.empty()) {
		v.push_back(st.top());
		st.pop();
	}

	for (int i = 0; i < v.size(); i++)
		postfix += v[i];
	cout << "���� ǥ��� : " << postfix << "\n";
}

int calcTwoOprd(int oprd1, int oprd2, char op) {
	if (op == '+') return oprd1 + oprd2;
	if (op == '-') return oprd1 - oprd2;
	if (op == '*') return oprd1 * oprd2;
	if (op == '/') return oprd1 / oprd2;
}

void caculate() {
	for (int i = 0; i < postfix.length(); i++) {
		char ch = postfix[i];
		// ����
		if (ch >= '0' && ch <= '9')
			//st2.push(ch - '0');
			push(ch - '0');
		// ������
		else {
			//int op2 = st2.top();
			//st2.pop();
			//int op1 = st2.top();
			//st2.pop();
			//st2.push(calcTwoOprd(op1, op2, ch));
			int op2 = peek();
			pop();
			int op1 = peek();
			pop();
			push(calcTwoOprd(op1, op2, ch));
		}
	}

	//int ans = st2.top();
	//st2.pop();
	int ans = peek();
	pop();
	cout << "���: " << ans << "\n";
}

int main() {
	//cin >> express;
	infix = "2+3*(5+4*2)-3+6";

	// test
	cout << "���� ǥ��� : " << infix << "\n";

	convertPostfix();

	caculate();

	return 0;
}