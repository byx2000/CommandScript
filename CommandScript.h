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
	运行时错误类
*/
class RuntimeError
{
public:
	RuntimeError() : errorText("no error"), line(0) {}
	void setErrorText(const string &text) { errorText = text; } //设置错误描述
	void setLine(size_t line) { this->line = line; } //设置错误行号
	void set(const string &text, size_t line) { errorText = text; this->line = line; } //同时设置错误描述和行号
	string getErrorText() { return errorText; } //获取错误描述
	size_t getLine() { return line; } //获取错误行号
private:
	string errorText;
	size_t line;
};

/*
	命令类
	用户自定义的命令类必须继承它，并重写getName和execute方法
	getName方法: 返回自定义命令的名称
	execute方法: 根据传入的参数(para)字符串执行命令，同时可修改error以反映运行时错误
*/
class Command
{
public:
	virtual string getName() const { return ""; } //返回自定义命令的名称（子类覆盖）
	virtual bool execute(const vector<string> &para, RuntimeError& error) { return false; }; //执行自定义命令（子类覆盖）
	bool operator<(const Command &c) const { return c.getName() < getName(); }
	bool operator>(const Command &c) const { return c.getName() > getName(); }
	bool operator==(const Command &c) const { return c.getName() == getName(); }
};

/*
	命令脚本引擎
*/
class CommandScriptEngine
{
public:
	bool addCommand(Command &command); //添加自定义命令
	bool removeCommand(const Command &command); //删除自定义命令
	static bool preProcess(ScriptReader &script, RuntimeError& error); //预处理脚本
	bool runScript(ScriptReader script, RuntimeError& error); //运行脚本（从ScriptReader）
	bool runScript(const string &file, RuntimeError& error); //运行脚本（从文件）

private:
	unordered_map<string, Command*> commands;
	bool runScript(const ScriptReader& script, RuntimeError& error, size_t beginLine, size_t endLine);
	static bool processDefine(ScriptReader& script, RuntimeError& error); //常量定义预处理
	static bool processBlock(ScriptReader& script, RuntimeError& error); //语句块预处理
};
