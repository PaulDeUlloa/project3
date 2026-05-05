#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <cctype>
using namespace std;

class Expression
{
private:
    int operand1;
    int operand2;
    char expressionOperator;

public:
    Expression(int firstOperand, char op, int secondOperand)
    {
        operand1 = firstOperand;
        expressionOperator = op;
        operand2 = secondOperand;
    }

    virtual ~Expression()
    {
    }

    int getResult() const
    {
        int result = 0;

        if (expressionOperator == '+')
            result = operand1 + operand2;
        else if (expressionOperator == '-')
            result = operand1 - operand2;
        else if (expressionOperator == '*')
            result = operand1 * operand2;
        else if (expressionOperator == '/')
            result = operand1 / operand2;
        else
            result = operand1 % operand2;

        return result;
    }

    char getOperator() const
    {
        return expressionOperator;
    }

    virtual string toString() const
    {
        stringstream output;

        output << setw(10) << operand1
               << setw(10) << expressionOperator
               << setw(10) << operand2
               << setw(10) << "="
               << setw(10) << getResult();

        return output.str();
    }
};

class NamedExpression : public Expression
{
private:
    string name;

public:
    NamedExpression(string expressionName, int firstOperand, char op, int secondOperand)
        : Expression(firstOperand, op, secondOperand)
    {
        name = expressionName;
    }

    string toString() const
    {
        stringstream output;

        output << Expression::toString()
               << "  NAME(" << name << ")";

        return output.str();
    }
};

bool isSupportedOperator(char op);
bool isValidName(string name);
void addExpression(vector<Expression *> &expressions);
void listAllExpressions(const vector<Expression *> &expressions);
void listByOperator(const vector<Expression *> &expressions);
void listSummary(const vector<Expression *> &expressions);
void deleteExpressions(vector<Expression *> &expressions);

bool isSupportedOperator(char op)
{
    bool validOperator = false;

    if (op == '+' || op == '-' || op == '*' || op == '/' || op == '%')
        validOperator = true;

    return validOperator;
}

bool isValidName(string name)
{
    bool validName = false;

    if (name.length() > 0 && isalpha(name[0]))
        validName = true;

    return validName;
}

void addExpression(vector<Expression *> &expressions)
{
    string line;
    string name = "";
    int firstOperand;
    int secondOperand;
    char op;
    bool hasExpressionName = false;
    bool goodExpression = true;

    cout << "Please enter an expression:" << endl;
    cin.ignore(1000, '\n');
    getline(cin, line);

    int equalsIndex = line.find('=');

    if (equalsIndex != string::npos)
    {
        hasExpressionName = true;
        name = line.substr(0, equalsIndex);

        while (name.length() > 0 && name[name.length() - 1] == ' ')
            name.erase(name.length() - 1, 1);

        if (!isValidName(name))
        {
            cout << "Invalid expression name. Name must start out with an alphabet character." << endl;
            goodExpression = false;
        }

        line = line.substr(equalsIndex + 1);
    }

    if (goodExpression)
    {
        stringstream input(line);

        if (!(input >> firstOperand))
        {
            cout << "Invalid operand. Number is expected." << endl;
            goodExpression = false;
        }
        else
        {
            input >> op;

            if (!(input >> secondOperand))
            {
                cout << "Invalid operand. Number is expected." << endl;
                goodExpression = false;
            }
            else if (!isSupportedOperator(op))
            {
                cout << "Unsupported expression operator." << endl;
                goodExpression = false;
            }
        }
    }

    if (goodExpression)
    {
        Expression *newExpression;

        if (hasExpressionName)
            newExpression = new NamedExpression(name, firstOperand, op, secondOperand);
        else
            newExpression = new Expression(firstOperand, op, secondOperand);

        expressions.push_back(newExpression);
        cout << newExpression->toString() << endl;
    }
}

void listAllExpressions(const vector<Expression *> &expressions)
{
    if (expressions.size() == 0)
    {
        cout << "There is no expression." << endl;
    }
    else
    {
        cout << "All expressions:" << endl;

        for (int i = 0; i < expressions.size(); i++)
            cout << expressions[i]->toString() << endl;
    }
}

void listByOperator(const vector<Expression *> &expressions)
{
    if (expressions.size() == 0)
    {
        cout << "There is no expression." << endl;
    }
    else
    {
        char userSelectedOperator;
        bool foundMatch = false;

        cout << "Please enter the operator:" << endl;
        cin >> userSelectedOperator;

        for (int i = 0; i < expressions.size(); i++)
        {
            if (expressions[i]->getOperator() == userSelectedOperator)
            {
                cout << expressions[i]->toString() << endl;
                foundMatch = true;
            }
        }

        if (!foundMatch)
            cout << "No expression is found with the operator: "
                 << userSelectedOperator << endl;
    }
}

void listSummary(const vector<Expression *> &expressions)
{
    if (expressions.size() == 0)
    {
        cout << "There is no expression." << endl;
    }
    else
    {
        int plusCount = 0;
        int minusCount = 0;
        int multiplyCount = 0;
        int divideCount = 0;
        int modulusCount = 0;

        int largestResult = expressions[0]->getResult();
        int smallestResult = expressions[0]->getResult();

        for (int i = 0; i < expressions.size(); i++)
        {
            char currentOperator = expressions[i]->getOperator();
            int currentResult = expressions[i]->getResult();

            if (currentOperator == '+')
                plusCount++;
            else if (currentOperator == '-')
                minusCount++;
            else if (currentOperator == '*')
                multiplyCount++;
            else if (currentOperator == '/')
                divideCount++;
            else if (currentOperator == '%')
                modulusCount++;

            if (currentResult > largestResult)
                largestResult = currentResult;

            if (currentResult < smallestResult)
                smallestResult = currentResult;
        }

        cout << "Total number of expressions: " << expressions.size() << endl;
        cout << "Number of '+' expressions:   " << plusCount << endl;
        cout << "Number of '-' expressions:   " << minusCount << endl;
        cout << "Number of '*' expressions:   " << multiplyCount << endl;
        cout << "Number of '/' expressions:   " << divideCount << endl;
        cout << "Number of '%' expressions:   " << modulusCount << endl;
        cout << "Largest expression:          " << largestResult << endl;
        cout << "Smallest expression:         " << smallestResult << endl;
    }
}

void deleteExpressions(vector<Expression *> &expressions)
{
    for (int i = 0; i < expressions.size(); i++)
        delete expressions[i];

    expressions.clear();
}

int main()
{
    vector<Expression *> expressions;
    bool programRunning = true;
    string command;

    cout << "Welcome to the named expression management program." << endl;

    while (programRunning)
    {
        cout << "Please enter a command (add, listall, listbyoperator, listsummary, and exit): ";
        cin >> command;

        if (command == "exit")
        {
            cout << endl;
            cout << "Thank you. Goodbye." << endl;
            programRunning = false;
        }
        else if (command == "add")
        {
            addExpression(expressions);
        }
        else if (command == "listall")
        {
            listAllExpressions(expressions);
        }
        else if (command == "listbyoperator")
        {
            listByOperator(expressions);
        }
        else if (command == "listsummary")
        {
            listSummary(expressions);
        }
        else
        {
            cout << "Unsupported command. Please try again." << endl;
        }
    }

    deleteExpressions(expressions);

    return 0;
}