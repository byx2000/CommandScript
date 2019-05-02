#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <fstream>
#include <cassert>

using namespace std;

/*
	语句类
	存储一条语句，并解析语句中的命令名称和参数
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
	string name; //命令名称
	vector<string> para; //命令参数
	size_t line; //语句所在的行号
};

/*
	脚本读取类
	从文件中读取脚本，并进行一些预处理
*/
class ScriptReader
{
public:
	ScriptReader() {}
	ScriptReader(const string &file) { readScript(file); }
	void readScript(const string &file); //从文件读取脚本
	size_t getLength() const { return script.size(); } //获取脚本行数
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
	static void trim(string &s); //去除字符串首尾空格
};
