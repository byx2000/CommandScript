#include "CommandScript.h"

bool CommandScriptEngine::addCommand(Command & command)
{
	//命令名为空
	string name = command.getName();
	if (name == "")
	{
		return false;
	}

	//同名命令已存在
	if (commands.count(name) != 0)
	{
		return false;
	}

	//添加命令成功
	commands[name] = &command;
	return true;
}

bool CommandScriptEngine::removeCommand(const Command & command)
{
	if (commands.count(command.getName()) == 0)
	{
		return false;
	}
	commands.erase(command.getName());
	return true;
}

/*预处理*/
bool CommandScriptEngine::preProcess(ScriptReader &script, RuntimeError& error)
{
	if (!processDefine(script, error))
	{
		return false;
	}

	if (!processBlock(script, error))
	{
		return false;
	}

	return true;
}

/*执行脚本*/
bool CommandScriptEngine::runScript(ScriptReader script, RuntimeError& error)
{

	//预处理
	if (!preProcess(script, error))
	{
		return false;
	}

	size_t cLine = script.getLength();
	return runScript(script, error, 1, cLine);
}

bool CommandScriptEngine::runScript(const string & file, RuntimeError& error)
{
	return runScript(ScriptReader(file), error);
}

bool CommandScriptEngine::runScript(const ScriptReader & script, RuntimeError & error, size_t beginLine, size_t endLine)
{
	for (size_t i = beginLine; i <= endLine; ++i)
	{
		//获取命令的名称
		string name = script.getCommandName(i);
		vector<string> para = script.getCommandParas(i);

		//内置命令
		if (name == "Call") //调用子过程
		{
			//参数格式不正确
			if (para.size() != 2)
			{
				error.set(name + ": This command requires TWO parameters.", script.getOriginalLineNum(i));
				return false;
			}

			//提取子过程的开始行号和结束行号
			size_t b = atoi(para[0].c_str());
			size_t e = atoi(para[1].c_str());
			size_t cLine = script.getLength();

			//空指令 Call 0 0
			if (b == 0 && e == 0)
			{
				continue;
			}

			//子过程超出脚本范围
			if (b < 1 || b > cLine
				|| e < 1 || e > cLine
				|| b > e)
			{
				error.set(name + ": The begin line and end line of sub procedure is illegal.", script.getOriginalLineNum(i));
				return false;
			}

			//调用子过程
			if (!runScript(script, error, b, e))
			{
				return false;
			}
		}
		else if (name == "Block") //语句块声明命令
		{
			//跳过声明的语句块
			size_t j;
			for (j = i + 1; j <= endLine; ++j)
			{
				if (script.getCommandName(j) == "}")
				{
					break;
				}
			}
			i = j;
			continue;
		}
		else
		{
			//无法识别的命令
			if (commands.count(name) == 0)
			{
				error.set(name + ": This command is not exist.", script.getOriginalLineNum(i));
				return false;
			}

			//向自定义命令传递参数并执行命令
			if (!commands[name]->execute(para, error))
			{
				error.setLine(script.getOriginalLineNum(i));
				return false;
			}
		}
	}
	return true;
}

bool CommandScriptEngine::processDefine(ScriptReader & script, RuntimeError & error)
{
	unordered_map<string, string> constToken;

	//第一次遍历：获取所有常量标识的值
	for (size_t i = 1; i <= script.getLength(); ++i)
	{
		//获取命令的名称和参数
		string name = script.getCommandName(i);
		vector<string> para = script.getCommandParas(i);

		if (name == "Define")
		{
			//参数个数不符合要求
			if (para.size() != 2)
			{
				error.set("Define: This command requires TWO parameters.", script.getOriginalLineNum(i));
				return false;
			}

			string token = para[0];

			//常量标识不是合法的标识符
			if (token == "" || token[0] == ' ' || (token[0] >= '0' && token[0] <= '9'))
			{
				error.set("Define: Token name \"" + token + "\" is illegal.", script.getOriginalLineNum(i));
				return false;
			}

			//重定义
			if (constToken.count(token) != 0)
			{
				error.set("Define: The token \"" + token + "\" is redefined.", script.getOriginalLineNum(i));
				return false;
			}

			//获取常量的值
			string val = para[1];

			//记录常量标识与值的对应关系
			constToken[token] = val;

			//移除当前常量定义语句
			script.removeStatement(i);
			i--;
		}
	}

	//第二次遍历：替换所有常量标识
	for (size_t i = 1; i <= script.getLength(); ++i)
	{
		//获取命令的参数
		vector<string> para = script.getCommandParas(i);

		for (size_t j = 0; j < para.size(); ++j)
		{
			if (constToken.count(para[j]) != 0)
			{
				string val = constToken[para[j]];
				while (constToken.count(val) != 0 && val != para[j])
				{
					val = constToken[val];
				}
				para[j] = val;
			}
		}

		script.setCommandParas(i, para);
	}

	return true;
}

bool CommandScriptEngine::processBlock(ScriptReader & script, RuntimeError & error)
{
	size_t cLine = script.getLength();
	unordered_map<string, pair<size_t, size_t>> blockToken;

	//第一次遍历：搜集所有代码块的开始和结束行号
	for (size_t i = 1; i <= cLine; ++i)
	{
		//获取命令的名称和参数
		string name = script.getCommandName(i);
		vector<string> para = script.getCommandParas(i);

		if (name == "Block")
		{
			//缺少语句块标识
			if (para.size() != 1)
			{
				error.set("Block: The following block token is missing.", script.getOriginalLineNum(i));
				return false;
			}

			//获取语句块标识
			string token = para[0];

			//标识不合法
			if (token[0] >= '0' && token[0] <= '9')
			{
				error.set("Block: The block token \"" + token + "\"is illegal.", script.getOriginalLineNum(i));
				return false;
			}

			//语句块重定义
			if (blockToken.count(token) != 0)
			{
				error.set("Block: The block token \"" + token + "\" is redefined.", script.getOriginalLineNum(i));
				return false;
			}

			//缺少左花括号
			if (i == cLine || script.getCommandName(i + 1) != "{")
			{
				error.set("Block: Missing \"{\".", script.getOriginalLineNum(i));
				return false;
			}

			//记录代码块开始行数
			size_t beginLine = i + 2;

			//寻找右花括号
			size_t j;
			for (j = i + 2; j <= cLine; ++j)
			{
				if (script.getCommandName(j) == "}")
				{
					break;
				}
			}

			//缺少右花括号
			if (j > cLine)
			{
				error.set("Block: Missing \"}\".", script.getOriginalLineNum(cLine));
				return false;
			}

			//记录代码块结束行数
			size_t endLine = j - 1;

			//记录当前代码块的信息
			blockToken[token] = pair<size_t, size_t>(beginLine, endLine);
		}
	}

	//第二次遍历：替换所有Call指令
	for (size_t i = 1; i <= cLine; ++i)
	{
		//获取命令的名称和参数
		string name = script.getCommandName(i);
		vector<string> para = script.getCommandParas(i);

		if (name == "Call")
		{
			//参数格式错误
			if (para.size() != 1)
			{
				error.set("Call: This command needs TWO parameters.", script.getOriginalLineNum(cLine));
				return false;
			}

			string token = para[0];

			//语句块未定义
			if (blockToken.count(token) == 0)
			{
				error.set("Call: The code block \"" + token + "\" is undefined.", script.getOriginalLineNum(cLine));
				return false;
			}

			//将语句块标识替换成语句块的开始和结束行号
			size_t beginLine = blockToken[token].first;
			size_t endLine = blockToken[token].second;
			para.clear();
			if (beginLine <= endLine)
			{
				para.push_back(to_string(beginLine));
				para.push_back(to_string(endLine));
			}
			else
			{
				para.push_back("0");
				para.push_back("0");
			}
			script.setCommandParas(i, para);
		}
	}

	return true;
}
