/*
 Supported symbols:
 
 +
 -
 *
 /
 ^ (exponent)
 
 sqrt(x)
 
 ln(a)
 log(a, b)
 
 sin(x)
 cos(x)
 tan(x)
 arcsin(x)
 arccos(x)
 arctan(x)
 
 floor(x)
 abs(x)
 
 n!
 
 pi
 e
*/

#include <iostream>
#include <string>
#include <stack>

using namespace std;

const int taylor_precision = 10;
const float pi = 3.1415929;
const float e = 2.71828;

int operation_code (char operation) {
    if (operation == '+') {
        return 1;
    } else if (operation == '-') {
        return 2;
    } else if (operation == '*') {
        return 3;
    } else if (operation == '/') {
        return 4;
    } else if (operation == '^') {
        return 5;
    } else {
        return 999;
    }
}

char operation_string (int code) {
    if (code == 1) {
        return '+';
    } else if (code == 2) {
        return '-';
    } else if (code == 3) {
        return '*';
    } else if (code == 4) {
        return '/';
    } else if (code == 5) {
        return '^';
    } else {
        return '?';
    }
}

float exp_int (float base, int exponent) {
    float product = 1;
    for (int i = 0; i < exponent; i++) {
        product *= base;
    }
    return product;
}

float ln (float x) {
    float x_copy = x;
    float count = 0;
    while (x_copy > 1) {
        x_copy /= e;
        count++;
    }
    float sum = 0;
    for (int n = 1; n < taylor_precision; n++) {
        sum += exp_int(1 - x_copy, n) / n;
    }
    return count - sum;
}

int factorial (int n) {
    int product = 1;
    for (int i = 1; i <= n; i++) {
        product *= i;
    }
    return product;
}

float e_exp (float x) {
    float sum = 0;
    for (int n = 0; n < taylor_precision; n++) {
        sum += exp_int(x, n) / factorial(n);
    }
    return sum;
}

float exp_float (float a, float x) {
    return e_exp(x * ln(a));
}

float log (float a, float b) {
    return ln(b) / ln(a);
}

float float_modulo (float a, float b) {
    float a_copy = a;
    while (a_copy > b) {
        a_copy -= b;
    }
    while (a_copy < 0) {
        a_copy += b;
    }
    return a_copy;
}

float sine (float x) {
    float circle_x = float_modulo(x, 2 * pi);
    float near_x =float_modulo(x, pi);
    if (near_x > pi / 2) {
        near_x = pi - near_x;
    }
    float sum = 0;
    for (int n = 0; n < taylor_precision; n++) {
        sum += exp_int(-1, n) * exp_int(near_x, 2 * n + 1) / factorial(2 * n + 1);
    }
    if (circle_x > pi) {
        return -sum;
    } else {
        return sum;
    }
}

float cosine (float x) {
    return sine(x + pi / 2);
}

float tangent (float x) {
    return sine(x) / cosine(x);
}

float absolute (float x) {
    if (x < 0) {
        return -x;
    } else {
        return x;
    }
}

float floor_ (float x) {
    int x_cut = (int) x;
    float x_cut_float = static_cast<float> (x_cut);
    return x_cut_float;
}

float ceil_ (float x) {
    return floor_(x) + 1.0;
}

float arcsin (float x) {
    float sum = 0;
    for (int n = 0; n < taylor_precision; n++) {
        sum += factorial(2 * n) * exp_int(x, 2 * n + 1) / (exp_int(4, n) * exp_int(factorial(n), 2) * (2 * n + 1));
    }
    return sum;
}

float arccos (float x) {
    return pi / 2 - arcsin(x);
}

float arctan (float x) {
    float sum = 0;
    for (int n = 0; n < taylor_precision; n++) {
        sum += exp_int(-1, n) * exp_int(x, 2 * n + 1) / (2 * n + 1);
    }
    return sum;
}

string clean_space (string expression) {
    string cleaned_expression = expression;
    for (int index = 0; index < cleaned_expression.length(); index++) {
        if (cleaned_expression[index] == ' ') {
            cleaned_expression.erase(index, 1);
            index--;
        }
    }
    return cleaned_expression;
}

string postfix(string expression) {
    int index = 0;
    string postfix_expression = "";
    stack<int> operations; // add 1, subtract 2, multiply 3, divide 4, exponentiation 5, unknown 999
    
    int previous_type = 2; // closing parenthesis 1, number 1, pi 1, e 1, factorial 1, others 2
    int current_operation_code = 0;
    
    while (index < expression.length()){
        if (isdigit(expression[index]) || (expression[index] == '-' && previous_type == 2)){
            string current_number = "";
            do {
                current_number += expression[index];
                index++;
            } while(isdigit(expression[index]) || expression[index] == '.');
            postfix_expression += current_number;
            postfix_expression += " ";
            previous_type = 1;
        } else if (isalpha(expression[index])) {
            string current_operation = "";
            while(isalpha(expression[index])){
                current_operation += expression[index];
                index++;
            }
            previous_type = 1;
            
            if (current_operation == "pi") {
                postfix_expression += to_string(pi);
                postfix_expression += " ";
                continue;
            }
            if (current_operation == "e") {
                postfix_expression += to_string(e);
                postfix_expression += " ";
                continue;
            }
            
            int moving_parenthesis_depth = 0;
            string first_operand = "";
            index++; // skip opening parenthesis
            while (!((expression[index] == ',' || expression[index] == ')') && moving_parenthesis_depth == 0)) {
                first_operand += expression[index];
                if (expression[index] == '(') {
                    moving_parenthesis_depth++;
                } else if (expression[index] == ')') {
                    moving_parenthesis_depth--;
                }
                index++;
            }
            string postfix_first_operand = postfix(first_operand);
            postfix_expression += postfix_first_operand;
            postfix_expression += " ";
            
            if (current_operation == "log") {
                moving_parenthesis_depth = 0;
                string second_operand = "";
                index++; // skip comma
                while (!(expression[index] == ')' && moving_parenthesis_depth == 0)) {
                    second_operand += expression[index];
                    if (expression[index] == '(') {
                        moving_parenthesis_depth++;
                    } else if (expression[index] == ')') {
                        moving_parenthesis_depth--;
                    }
                    index++;
                }
                string postfix_second_operand = postfix(second_operand);
                postfix_expression += postfix_second_operand;
                postfix_expression += " ";
            }
            
            index++; // skip closing parenthesis
            
            if (current_operation == "log") {
                postfix_expression += "l";
                postfix_expression += " ";
            } else if (current_operation == "sin") {
                postfix_expression += "s";
                postfix_expression += " ";
            } else if (current_operation == "cos") {
                postfix_expression += "c";
                postfix_expression += " ";
            } else if (current_operation == "tan") {
                postfix_expression += "t";
                postfix_expression += " ";
            } else if (current_operation == "ln") {
                postfix_expression += "L";
                postfix_expression += " ";
            } else if (current_operation == "sqrt") {
                postfix_expression += "r";
                postfix_expression += " ";
            } else if (current_operation == "abs") {
                postfix_expression += "a";
                postfix_expression += " ";
            } else if (current_operation == "floor") {
                postfix_expression += "F";
                postfix_expression += " ";
            } else if (current_operation == "ceil") {
                postfix_expression += "C";
                postfix_expression += " ";
            } else if (current_operation == "arcsin") {
                postfix_expression += "q";
                postfix_expression += " ";
            } else if (current_operation == "arccos") {
                postfix_expression += "w";
                postfix_expression += " ";
            } else if (current_operation == "arctan") {
                postfix_expression += "e";
                postfix_expression += " ";
            }
        } else if (operation_code(expression[index]) != 999) {
            current_operation_code = operation_code(expression[index]);
            if (operations.size() == 0 || current_operation_code > operations.top()) {
                operations.push(current_operation_code);
            } else {
                while(operations.size() > 0 && current_operation_code <= operations.top()) {
                    postfix_expression += operation_string(operations.top());
                    postfix_expression += " ";
                    operations.pop();
                }
                operations.push(current_operation_code);
            }
            previous_type = 2;
            index++;
        } else if (expression[index] == '(') {
            index++; //skip parenthesis
            string sub_expression = "";
            int moving_parenthesis_depth = 0;
            while(!(expression[index] == ')' && moving_parenthesis_depth == 0)) {
                sub_expression += expression[index];
                if (expression[index] == '(') {
                    moving_parenthesis_depth++;
                } else if (expression[index] == ')') {
                    moving_parenthesis_depth--;
                }
                index++;
            }
            postfix_expression += postfix(sub_expression);
            postfix_expression += " ";
            previous_type = 1;
            index++; //skip parenthesis
        } else if (expression[index] == '!') {
            postfix_expression += "!";
            postfix_expression += " ";
            previous_type = 1;
            index++;
        } else {
            index++;
        }
    }
    while (operations.size() > 0) {
        postfix_expression += operation_string(operations.top());
        postfix_expression += " ";
        operations.pop();
    }
    
    return postfix_expression;
}

float calculate(string postfix_expression) {
    int index = 0;
    stack<float> numbers;
    
    while (index < postfix_expression.length()) {
        if (isdigit(postfix_expression[index]) || (postfix_expression[index] == '-' && isdigit(postfix_expression[index + 1]))) {
            string current_number = "";
            do {
                current_number += postfix_expression[index];
                index++;
            } while(isdigit(postfix_expression[index]) || postfix_expression[index] == '.');
            float current_number_float = stof(current_number);
            numbers.push(current_number_float);
        } else if (postfix_expression[index] == '+') {
            float second_operand = numbers.top();
            numbers.pop();
            float first_operand = numbers.top();
            numbers.pop();
            numbers.push(first_operand + second_operand);
            index++;
        } else if (postfix_expression[index] == '-') {
            float second_operand = numbers.top();
            numbers.pop();
            float first_operand = numbers.top();
            numbers.pop();
            numbers.push(first_operand - second_operand);
            index++;
        } else if (postfix_expression[index] == '*') {
            float second_operand = numbers.top();
            numbers.pop();
            float first_operand = numbers.top();
            numbers.pop();
            numbers.push(first_operand * second_operand);
            index++;
        } else if (postfix_expression[index] == '/') {
            float second_operand = numbers.top();
            numbers.pop();
            float first_operand = numbers.top();
            numbers.pop();
            numbers.push(first_operand / second_operand);
            index++;
        } else if (postfix_expression[index] == '^') {
            float second_operand = numbers.top();
            numbers.pop();
            float first_operand = numbers.top();
            numbers.pop();
            numbers.push(exp_float(first_operand, second_operand));
            index++;
        } else if (postfix_expression[index] == '!') {
            float first_operand = numbers.top();
            numbers.pop();
            numbers.push(factorial(first_operand));
            index++;
        } else if (postfix_expression[index] == 'l') {
            float second_operand = numbers.top();
            numbers.pop();
            float first_operand = numbers.top();
            numbers.pop();
            numbers.push(log(first_operand, second_operand));
            index++;
        } else if (postfix_expression[index] == 'L') {
            float first_operand = numbers.top();
            numbers.pop();
            numbers.push(ln(first_operand));
            index++;
        } else if (postfix_expression[index] == 's') {
            float first_operand = numbers.top();
            numbers.pop();
            numbers.push(sine(first_operand));
            index++;
        } else if (postfix_expression[index] == 'c') {
            float first_operand = numbers.top();
            numbers.pop();
            numbers.push(cosine(first_operand));
            index++;
        } else if (postfix_expression[index] == 't') {
            float first_operand = numbers.top();
            numbers.pop();
            numbers.push(tangent(first_operand));
            index++;
        } else if (postfix_expression[index] == 'r') {
            float first_operand = numbers.top();
            numbers.pop();
            numbers.push(exp_float(first_operand, 1.0 / 2.0));
            index++;
        } else if (postfix_expression[index] == 'a') {
            float first_operand = numbers.top();
            numbers.pop();
            numbers.push(absolute(first_operand));
            index++;
        } else if (postfix_expression[index] == 'F') {
            float first_operand = numbers.top();
            numbers.pop();
            int first_operand_cut = (int) first_operand;
            float first_operand_cut_float = static_cast<float> (first_operand_cut);
            numbers.push(first_operand_cut_float);
            index++;
        } else if (postfix_expression[index] == 'C') {
            float first_operand = numbers.top();
            numbers.pop();
            int first_operand_cut = (int) first_operand;
            first_operand_cut++;
            float first_operand_cut_float = static_cast<float> (first_operand_cut);
            numbers.push(first_operand_cut_float);
            index++;
        } else if (postfix_expression[index] == 'q') {
            float first_operand = numbers.top();
            numbers.pop();
            numbers.push(arcsin(first_operand));
            index++;
        } else if (postfix_expression[index] == 'w') {
            float first_operand = numbers.top();
            numbers.pop();
            numbers.push(arccos(first_operand));
            index++;
        } else if (postfix_expression[index] == 'e') {
            float first_operand = numbers.top();
            numbers.pop();
            numbers.push(arctan(first_operand));
            index++;
        } else {
            index++;
        }
    }
    
    return numbers.top();
}

int main()
{
    cout << "Please input an expression: \n";
    
    string expression;
    getline(cin, expression);
    
    string cleaned_expression = clean_space(expression);
    
    string postfix_expression = postfix(cleaned_expression);
    cout << postfix_expression << "\n";
    
    float result = calculate(postfix_expression);
    cout << "Answer: " << result << "\n";
    
    return 0;
}
