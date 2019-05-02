#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cassert>
#include "ScriptReader.h"
#include "CommandScript.h"

using namespace std;

void trim(string & s)
{
	if (!s.empty())
	{
		s.erase(0, s.find_first_not_of(" \t\f\v\n\r"));
		s.erase(s.find_last_not_of(" \t\f\v\n\r") + 1);
	}
}

bool FileCmp(const string& file1, const string& file2)
{
	ifstream f1(file1);
	ifstream f2(file2);
	string s1, s2;
	while (!f1.eof())
	{
		s1 += f1.get();
	}
	while (!f2.eof())
	{
		s2 += f2.get();
	}
	//cout << s1 << endl << endl;
	//cout << s2 << endl << endl;
	return s1 == s2;
}

void TestStatement()
{
	string statement;
	Statement parser;

	statement = "";
	parser.parse(statement);
	assert(parser.getName() == "");
	assert(parser.getParaNum() == 0);

	statement = "    ";
	parser.parse(statement);
	assert(parser.getName() == "");
	assert(parser.getParaNum() == 0);

	statement = "Print";
	parser.parse(statement);
	assert(parser.getName() == "Print");
	assert(parser.getParaNum() == 0);

	statement = "Print  ";
	parser.parse(statement);
	assert(parser.getName() == "Print");
	assert(parser.getParaNum() == 0);

	statement = "  Print";
	parser.parse(statement);
	assert(parser.getName() == "Print");
	assert(parser.getParaNum() == 0);

	statement = "   Print  ";
	parser.parse(statement);
	assert(parser.getName() == "Print");
	assert(parser.getParaNum() == 0);

	statement = "Print 123";
	parser.parse(statement);
	assert(parser.getName() == "Print");
	assert(parser.getParaNum() == 1);
	assert(parser.getPara(1) == "123");

	statement = "Print    123";
	parser.parse(statement);
	assert(parser.getName() == "Print");
	assert(parser.getPara(1) == "123");

	statement = "  Print 123";
	parser.parse(statement);
	assert(parser.getName() == "Print");
	assert(parser.getParaNum() == 1);
	assert(parser.getPara(1) == "123");

	statement = "Print 123  ";
	parser.parse(statement);
	assert(parser.getName() == "Print");
	assert(parser.getParaNum() == 1);
	assert(parser.getPara(1) == "123");

	statement = "  Print  123    ";
	parser.parse(statement);
	assert(parser.getName() == "Print");
	assert(parser.getParaNum() == 1);
	assert(parser.getPara(1) == "123");

	statement = "Print 123 456";
	parser.parse(statement);
	assert(parser.getName() == "Print");
	assert(parser.getParaNum() == 2);
	assert(parser.getPara(1) == "123");
	assert(parser.getPara(2) == "456");

	statement = "Print 123 456 789";
	parser.parse(statement);
	assert(parser.getName() == "Print");
	assert(parser.getParaNum() == 3);
	assert(parser.getPara(1) == "123");
	assert(parser.getPara(2) == "456");
	assert(parser.getPara(3) == "789");

	statement = " Print 123  456 789  ";
	parser.parse(statement);
	assert(parser.getName() == "Print");
	assert(parser.getParaNum() == 3);
	assert(parser.getPara(1) == "123");
	assert(parser.getPara(2) == "456");
	assert(parser.getPara(3) == "789");

	statement = "Print 123 456 789 abc";
	parser.parse(statement);
	assert(parser.getName() == "Print");
	assert(parser.getParaNum() == 4);
	assert(parser.getPara(1) == "123");
	assert(parser.getPara(2) == "456");
	assert(parser.getPara(3) == "789");
	assert(parser.getPara(4) == "abc");

	statement = "Print    123  456            789  abc";
	parser.parse(statement);
	assert(parser.getName() == "Print");
	assert(parser.getParaNum() == 4);
	assert(parser.getPara(1) == "123");
	assert(parser.getPara(2) == "456");
	assert(parser.getPara(3) == "789");
	assert(parser.getPara(4) == "abc");
}

void TestScriptReader(int cTestCase)
{
	for (int i = 1; i <= cTestCase; ++i)
	{
		string input = "Test\\TestScriptReader\\test" + to_string(i) + ".txt";
		string output = "Test\\TestScriptReader\\out.txt";
		ofstream fout(output);
		string ans = "Test\\TestScriptReader\\test" + to_string(i) + "_r" + ".txt";
		ScriptReader reader(input);
		fout << reader.toString();
		fout.close();
		if (!FileCmp(output, ans))
		{
			cout << "ScriptReader test: case " << i << " failed." << endl;
			return;
		}
	}
	cout << "ScriptReader test passed." << endl;
}

void TestPreProcess(int cTestCase)
{
	RuntimeError error;
	for (int i = 1; i <= cTestCase; ++i)
	{
		string input = "Test\\TestPreProcess\\test" + to_string(i) + ".txt";
		string output = "Test\\TestPreProcess\\out.txt";
		ofstream fout(output);
		string ans = "Test\\TestPreProcess\\test" + to_string(i) + "_r" + ".txt";
		ScriptReader reader(input);
		//assert(CommandScriptEngine::preProcess(reader, error));
		if (!CommandScriptEngine::preProcess(reader, error))
        {
            cout << "Case " << i << endl;
            cout << error.getLine() << endl;
            cout << error.getErrorText() << endl;
            return;
        }
		fout << reader.toString();
		fout.close();
		if (!FileCmp(output, ans))
		{
			cout << "PreProcess test: case " << i << " failed." << endl;
			return;
		}
	}
	cout << "PreProcess test passed." << endl;
}

void TestErrorInfo(int cTestCace)
{
	RuntimeError error;
	for (int i = 1; i <= cTestCace; ++i)
	{
		string input = "Test\\TestErrorInfo\\test" + to_string(i) + ".txt";
		string output = "Test\\TestErrorInfo\\out.txt";
		ofstream fout(output);
		string ans = "Test\\TestErrorInfo\\test" + to_string(i) + "_r" + ".txt";
		ScriptReader reader(input);
		assert(!CommandScriptEngine::preProcess(reader, error));
		fout << error.getLine() << endl;
		fout << error.getErrorText();
		fout.close();
		if (!FileCmp(output, ans))
		{
			cout << "ErrorInfo test: case " << i << " failed." << endl;
			return;
		}
	}
	cout << "ErrorInfo test passed." << endl;
}
