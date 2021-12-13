#include<iostream>
#include<string>
#include<algorithm>
#include<vector>


std::string Change_P();		//change expression to reverse polish
int count_act_one_opr();	//count operators which act one stack
inline void Komachi_Cal(std::string expression_p);		//cal & judge to mutch

int num[9] = { 1,2,3,4,5,6,7,8,9 };
std::string stc;
std::string buff;
int now_stack_num = 0;		//when now stack pos
int max_stack_num = 0;		//max stack pos/2
int act_one_opr_num = 0;	//operators which act one stack( s,( )
int answer = 0;
double *stack;		//even num :molecule , odd num :denominator
std::string expression;
//std::vector< Deduplication> ded;

int main() {
	std::string expression_p;
	std::cout << "answer =" ;
	std::cin >> answer;
	std::cout << "Write items in the same order continuously!" << std::endl;
	std::cout << "a*9,+,-,*,/,.,(),^,sqrt(),log()()" << std::endl;
	std::cin >> expression;
	expression_p.reserve(expression.size());
	if ((expression_p = Change_P()) == ""||expression_p.size()<17) {
		std::cout << "The formula is wrong" << std::endl;
	}
	else {
		std::cout << expression_p << std::endl
			<< "**********************************" << std::endl;
		stack = new double[max_stack_num  * 2 + 2];
		do {
			Komachi_Cal(expression_p);
		} while (std::next_permutation(num, num + 9));
	}
	std::cout << "end" << std::endl;
	delete[] stack;

	system("pause");

	return 0;
}

std::string Change_P() {
	int i = 0, k = 0, st, ls, end_m = 0;
	stc.reserve(9);
	buff.reserve(27);
	char j = '1';

	//•‰‚Ì”œ‹Ž‚P
	if (expression[0] == '-'){
		buff += '0';
		stc += '-';
		now_stack_num += 2;
		i++;
	}
	/*•¶Žš—ñ“Ç‚ÝŽæ‚è(*@b : exp)1 2 b -> 1*10+2 ->12 , s :ƒ‹[ƒg ,
	l: log ,
		() b  > s  log > ^ ./* > +-			*/
	while (1) {
		switch (expression[i]) {
		case 0:
			end_m++;
			break;

		case 'a'://number pos
			buff += j;
			if (i != 0 && expression[i - 1] == 'a') {
				now_stack_num--;
				buff += 'b';		//b‰‰ŽZŽq@’¼Úbuff‘}“ü
			}
			now_stack_num++;
			j++;
			break;

		case ')':
			st = stc.find_last_of('(');
			if (st == std::string::npos) {
				return "";
			}
			ls = stc.size() - 1;
			for (k = ls; k > st; --k) {
				buff += stc[k];
				if (stc[k] != 's')
					now_stack_num--;
			}
			stc.erase(st, ls - st + 1);
			break;

		case '(':
			stc += '(';
			break;

		//infix notation
		case '-':
			if (stc.empty()) {
				stc += '-';
			}
			else {
				switch (stc.back()) {
				case '(':
					if (buff.empty()) {
						now_stack_num++;
						buff += '0';
						stc += '-';
					}
					else {
						//judge like (1-2) or (-2)
						if (now_stack_num -stc.size()+count_act_one_opr()==0) {
							now_stack_num++;
							buff += '0';
							stc += '-';
						}
						else {
							stc += '-';
						}
					}
					break;
				case '+':	case '-':
				case '*':	case '/':	case '.':	case '^':	
				case 'l':
					now_stack_num--;
				case 's':
					buff += stc.back();
					stc[stc.size() - 1] = expression[i];
					break;
				}
			}
			break;

		case'+':
			if (stc.empty()) {
				stc += expression[i];
			}
			else {
				switch (stc.back()) {
				case '+':	case '-':	
				case '*':	case '/':	case '.':	case '^':	
				case 'l':
					now_stack_num--;
				case 's':
					buff += stc.back();
					stc[stc.size() - 1] = expression[i];
					break;
				case '(':
					stc += expression[i];
					break;
				}
			}
			break;

		case '/':
		case '*':
		case '^':
			if (stc.empty()) {
				stc += expression[i];
			}
			else {
				switch (stc.back()) {
				case '*':	case '/':	case '.':	case '^':
				case 'l':
					now_stack_num--;
				case 's':
					buff += stc.back();
					stc[stc.size() - 1] = expression[i];
					break;
				case '(':	case '+':	case '-':
					stc += expression[i];
					break;
				}
			}
			break;

		case '.':
			if (stc.empty()) {
				stc += expression[i];
			}
			else {
				switch (stc.back()) {
				case '*':	case '/':	case '^':	
				case 'l':
					now_stack_num--;
				case 's':
					buff += stc.back();
					stc[stc.size() - 1] = expression[i];
					break;
				case '.':
					return "";
				case '(':	case '+':	case '-':
					stc += expression[i];
					break;
				}
			}
			break;
			//unary operator
		case 's':
			if (expression[i + 1] == 'q'&&expression[i + 2] == 'r'
				&&expression[i + 3] == 't'&&expression[i + 4] == '(') {
				i += 3;
				stc += 's';
			}
			else {
				return "";
			}
			break;
			//prefix operator
		case 'l':
			if (expression[i + 1] == 'o'&&expression[i + 2] == 'g'
				&&expression[i + 3] == '(') {
				i += 2;
				stc += 'l';	
			}
			else {
				return "";
			}
			break;
		default:
			return "";
		}
		if (end_m == 1)
			break;
		
		i++;
		if (now_stack_num > max_stack_num)
			max_stack_num = now_stack_num;
	}
	//“Ç‚ÝŽæ‚èŒã,stc‚ð‹ó‚É
	if ((ls = stc.size() - 1) >= 0) {
		for (k = ls; k >= 0; k--) {
			if (stc[k] == '(')
				return "";
			buff += stc[k];
		}
	}
	return buff;
}

int count_act_one_opr() {
	int l = 0, cnt = 0;
	while (stc[l]) {
		switch (stc[l]) {
		case '(':	case 's':
			cnt++;
		}
		l++;
	}
	return cnt;
}

inline void Komachi_Cal(std::string expression_p) {
	int i = 0, j = 0;		//i->expression pos , j->next stc pos
	double	work;		//use operator '.'
	while (1) {
		switch (expression_p[i]) {
		case 0:
			i = -1;
			break;
		case '0':
			stack[j++] = 0;
			stack[j++] = 1;
			break;
		case '1':case '2':case '3':
		case '4':case '5':case '6':
		case '7':case '8':case '9':
			stack[j++] = num[expression_p[i] - '1'];
			stack[j++] = 1;
			break;
		case 'b':
			if (j < 4)
				return;
			j -= 4;
			stack[j] = stack[j] * 10 + stack[j + 2];
			j += 2;
			break;
		case '.':
			if (j < 4)
				return;
			j -= 4;
			work = stack[j+2];
			while (work != 0) {
				work /= 10;
				stack[j] *= 10;
				stack[j + 1] *= 10;
			}
			stack[j] += stack[j + 2];
			j += 2;
			break;
		case '+':
			if (j < 4)
				return;
			j -= 4;
			stack[j] = stack[j] * stack[j + 3] + stack[j + 2] * stack[j + 1];
			stack[j + 1] = stack[j + 1] * stack[j + 3];
			j += 2;
			break;
		case '-':
			if (j < 4)
				return;
			j -= 4;
			stack[j] = stack[j] * stack[j + 3] - stack[j + 2] * stack[j + 1];
			stack[j + 1] = stack[j + 1] * stack[j + 3];
			j += 2;
			break;
		case '*':
			if (j < 4)
				return;
			j -= 4;
			stack[j] *= stack[j + 2];
			stack[j + 1] *= stack[j + 3];
			j += 2;
			break;
		case '/':
			if (j < 4)
				return;
			j -= 4;
			stack[j] *= stack[j + 3];
			stack[j + 1] *= stack[j + 2];
			j += 2;
			break;
		case '^':
			if (j < 4)
				return;
			j -= 4;
			stack[j] = std::pow(stack[j], stack[j + 2] / stack[j + 3]);
			stack[j+1] = std::pow(stack[j+1], stack[j + 2] / stack[j + 3]);
			j += 2;
			break;
		case 's':
			if (j < 2)
				return;
			j -= 2;
			stack[j] = std::pow(stack[j], 0.5);
			stack[j + 1] = std::pow(stack[j + 1],0.5);
			j += 2;
			break;
		case 'l':
			if (j < 4)
				return;
			j -= 4;
			if (stack[j] * stack[j + 1] <= 0)
				return;
			if (stack[j+2] * stack[j + 3] <= 0)
				return;
			stack[j + 1] = std::log(stack[j] / stack[j + 1]);
			stack[j] = std::log(stack[j + 2] / stack[j + 3]);
			j += 2;
			break;
		}
		if (i == -1)
			break;
		i++;
	}
	if (stack[1] != 0) {
		if (stack[0] == answer * stack[1]) {
			i = 0;
			j = 0;
			while (1) {
				switch (expression[i]) {
				case 0:
					i = -1;
					break;
				case 'a':
					std::cout << num[j];
					j++;
					break;
				default:
					std::cout << expression[i];
					break;
				}
				if (i == -1)
					break;
				i++;
			}
			std::cout << std::endl;
		}
	}
}

/////////////////Deduplication/////////////////////

class Deduplication {
public:
	int number;
	int digit;
	int* locate;
	Deduplication(int num,int dig) {
		number = num;
		digit = dig;
		locate = new int[number];
	}


};
	