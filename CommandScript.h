#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <map>
#include <unordered_map>
#include "ScriptReader.h"

using namespace std;

/*
	����ʱ������
*/
class RuntimeError
{
public:
	RuntimeError() : errorText("no error"), line(0) {}
	void setErrorText(const string &text) { errorText = text; }
	void setLine(size_t line) { this->line = line; }
	void set(const string &text, size_t line) { errorText = text; this->line = line; }
	string getErrorText() { return errorText; }
	size_t getLine() { return line; }
private:
	string errorText;
	size_t line;
};

/*
	������
	�û��Զ�������������̳���������дgetName��execute����
	getName����: �����Զ������������
	execute����: ���ݴ���Ĳ���(para)�ַ���ִ�����ͬʱ���޸�error�Է�ӳ����ʱ����
*/
class Command
{
public:
	virtual string getName() const { return ""; }
	virtual bool execute(const vector<string> &para, RuntimeError& error) { return false; };
	bool operator<(const Command &c) const { return c.getName() < getName(); }
	bool operator>(const Command &c) const { return c.getName() > getName(); }
	bool operator==(const Command &c) const { return c.getName() == getName(); }
};

/*
	����ű�����
*/
class CommandScriptEngine
{
public:
	bool addCommand(Command &command);
	bool removeCommand(const Command &command);
	static ScriptReader scriptFromFile(const string &file);
	static bool preProcess(ScriptReader &script, RuntimeError& error);
	bool runScript(ScriptReader script, RuntimeError& error);
	bool runScript(const string &file, RuntimeError& error);

private:
	unordered_map<string, Command*> commands;
	bool runScript(const ScriptReader& script, RuntimeError& error, size_t beginLine, size_t endLine);
	static bool processDefine(ScriptReader& script, RuntimeError& error);
	static bool processBlock(ScriptReader& script, RuntimeError& error);
};
