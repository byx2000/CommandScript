#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <stack>
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
	void setErrorText(const string &text) { errorText = text; } //���ô�������
	void setLine(size_t line) { this->line = line; } //���ô����к�
	void set(const string &text, size_t line) { errorText = text; this->line = line; } //ͬʱ���ô����������к�
	string getErrorText() { return errorText; } //��ȡ��������
	size_t getLine() { return line; } //��ȡ�����к�
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
	virtual string getName() const { return ""; } //�����Զ�����������ƣ����า�ǣ�
	virtual bool execute(const vector<string> &para, RuntimeError& error) { return false; }; //ִ���Զ���������า�ǣ�
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
	bool addCommand(Command &command); //����Զ�������
	bool removeCommand(const Command &command); //ɾ���Զ�������
	static bool preProcess(ScriptReader &script, RuntimeError& error); //Ԥ����ű�
	bool runScript(ScriptReader script, RuntimeError& error); //���нű�����ScriptReader��
	bool runScript(const string &file, RuntimeError& error); //���нű������ļ���

private:
	unordered_map<string, Command*> commands;
	bool runScript(const ScriptReader& script, RuntimeError& error, size_t beginLine, size_t endLine);
	static bool processDefine(ScriptReader& script, RuntimeError& error); //��������Ԥ����
	static bool processBlock(ScriptReader& script, RuntimeError& error); //����Ԥ����
};
