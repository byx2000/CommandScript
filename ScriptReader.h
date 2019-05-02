#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <fstream>
#include <cassert>

using namespace std;

/*
	�����
	�洢һ����䣬����������е��������ƺͲ���
*/
class Statement
{
public:
	Statement() {}
	Statement(const string & statement, size_t line) : line(line) { parse(statement); }
	void parse(const string& statement);
	string getName() const { return name; }
	size_t getParaNum() const { return para.size(); }
	vector<string> getParas() const { return para; }
	string getPara(size_t i) const { return para[i - 1]; }
	void setName(const string &name) { this->name = name; }
	void setParas(const vector<string> &para) { this->para = para; }
	void setLine(size_t line) { this->line = line; }
	size_t getLine() const { return line; }
	string toString() const;
	void print() const;
	bool operator==(const Statement &s) const { return name == s.name && para == s.para; }
	bool operator!=(const Statement &s) const { return name != s.name || para != s.para; }

private:
	string name; //��������
	vector<string> para; //�������
	size_t line; //������ڵ��к�
};

/*
	�ű���ȡ��
	���ļ��ж�ȡ�ű���������һЩԤ����
*/
class ScriptReader
{
public:
	ScriptReader() {}
	ScriptReader(const string &file) { readScript(file); }
	void readScript(const string &file); //���ļ���ȡ�ű�
	size_t getLength() const { return script.size(); } //��ȡ�ű�����
	string getCommandName(size_t line) const { return script[line - 1].getName(); }
	size_t getCommandParaNum(size_t line) const { return script[line - 1].getParaNum(); }
	vector<string> getCommandParas(size_t line) const { return script[line - 1].getParas(); }
	string getCommandPara(size_t line, size_t i) const { return script[line - 1].getPara(i); }
	void setCommandName(size_t line, const string &name) { script[line - 1].setName(name); }
	void setCommandParas(size_t line, const vector<string> &para) { script[line - 1].setParas(para); }
	void removeStatement(size_t line) { script.erase(script.begin() + (line - 1)); };
	size_t getOriginalLineNum(size_t line) const { return script[line - 1].getLine(); }
	string toString() const;
	void print() const;
	bool operator==(const ScriptReader &reader) const { return script == reader.script; }
	bool operator!=(const ScriptReader &reader) const { return script != reader.script; }

private:
	string file;
	vector<Statement> script;
	static void trim(string &s); //ȥ���ַ�����β�ո�
};
