#include "CommandScript.h"

bool CommandScriptEngine::addCommand(Command & command)
{
	//������Ϊ��
	string name = command.getName();
	if (name == "")
	{
		return false;
	}

	//ͬ�������Ѵ���
	if (commands.count(name) != 0)
	{
		return false;
	}

	//�������ɹ�
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

/*Ԥ����*/
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

/*ִ�нű�*/
bool CommandScriptEngine::runScript(ScriptReader script, RuntimeError& error)
{
	error.set(ERROR_NO_ERROR, 0);

	//Ԥ����
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
		//��ȡ���������
		string name = script.getCommandName(i);
		vector<string> para = script.getCommandParas(i);

		//��������
		if (name == "Call") //�����ӹ���
		{
			//������ʽ����ȷ
			if (para.size() != 2)
			{
				error.set(ERROR_PARA_COUNT_MISMATCH(name, 2), script.getOriginalLineNum(i));
				return false;
			}

			//��ȡ�ӹ��̵Ŀ�ʼ�кźͽ����к�
			size_t b = atoi(para[0].c_str());
			size_t e = atoi(para[1].c_str());
			size_t cLine = script.getLength();

			//��ָ�� Call 0 0
			if (b == 0 && e == 0)
			{
				continue;
			}

			//�ӹ��̳����ű���Χ
			if (b < 1 || b > cLine
				|| e < 1 || e > cLine
				|| b > e)
			{
				error.set(ERROR_SUB_PROCESS_INVALID, script.getOriginalLineNum(i));
				return false;
			}

			//�����ӹ���
			if (!runScript(script, error, b, e))
			{
				return false;
			}
		}
		else if (name == "Block") //������������
		{
			//��������������
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
			//�޷�ʶ�������
			if (commands.count(name) == 0)
			{
				error.set(ERROR_COMMAND_NOT_FOUND(name), script.getOriginalLineNum(i));
				return false;
			}

			//���Զ�������ݲ�����ִ������
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

	//��һ�α�������ȡ���г�����ʶ��ֵ
	for (size_t i = 1; i <= script.getLength(); ++i)
	{
		//��ȡ��������ƺͲ���
		string name = script.getCommandName(i);
		vector<string> para = script.getCommandParas(i);

		if (name == "Define")
		{
			//��������������Ҫ��
			if (para.size() != 2)
			{
				error.set(ERROR_PARA_COUNT_MISMATCH("Define", 2), script.getOriginalLineNum(i));
				return false;
			}

			string token = para[0];

			//������ʶ���ǺϷ��ı�ʶ��
			if (token == "" || token[0] == ' ' || (token[0] >= '0' && token[0] <= '9'))
			{
				error.set(ERROR_TOKEN_INVALID(token), script.getOriginalLineNum(i));
				return false;
			}

			//�ض���
			if (constToken.count(token) != 0)
			{
				error.set(ERROR_TOKEN_REDEFINE(token), script.getOriginalLineNum(i));
				return false;
			}

			//��ȡ������ֵ
			string val = para[1];

			//��¼������ʶ��ֵ�Ķ�Ӧ��ϵ
			constToken[token] = val;

			//�Ƴ���ǰ�����������
			script.removeStatement(i);
			i--;
		}
	}

	//�ڶ��α������滻���г�����ʶ
	for (size_t i = 1; i <= script.getLength(); ++i)
	{
		//��ȡ����Ĳ���
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

	//��һ�α������Ѽ����д����Ŀ�ʼ�ͽ����к�
	for (size_t i = 1; i <= cLine; ++i)
	{
		//��ȡ��������ƺͲ���
		string name = script.getCommandName(i);
		vector<string> para = script.getCommandParas(i);

		if (name == "Block")
		{
			//ȱ�������ʶ
			if (para.size() != 1)
			{
				error.set(ERROR_PARA_COUNT_MISMATCH("Block", 1), script.getOriginalLineNum(i));
				return false;
			}

			//��ȡ�����ʶ
			string token = para[0];

			//��ʶ���Ϸ�
			if (token[0] >= '0' && token[0] <= '9')
			{
				error.set(ERROR_TOKEN_INVALID(token), script.getOriginalLineNum(i));
				return false;
			}

			//�����ض���
			if (blockToken.count(token) != 0)
			{
				error.set(ERROR_TOKEN_REDEFINE(token), script.getOriginalLineNum(i));
				return false;
			}

			//ȱ��������
			if (i == cLine || script.getCommandName(i + 1) != "{")
			{
				error.set(ERROR_MISSING_LEFT_CURLY_BRACKET, script.getOriginalLineNum(i));
				return false;
			}

			//��¼����鿪ʼ����
			size_t beginLine = i + 2;

			//Ѱ���һ�����
			size_t j;
			for (j = i + 2; j <= cLine; ++j)
			{
				if (script.getCommandName(j) == "}")
				{
					break;
				}
			}

			//ȱ���һ�����
			if (j > cLine)
			{
				error.set(ERROR_MISSING_RIGHT_CURLY_BRACKET, script.getOriginalLineNum(cLine));
				return false;
			}

			//��¼������������
			size_t endLine = j - 1;

			//��¼��ǰ��������Ϣ
			blockToken[token] = pair<size_t, size_t>(beginLine, endLine);
		}
	}

	//�ڶ��α������滻����Callָ��
	for (size_t i = 1; i <= cLine; ++i)
	{
		//��ȡ��������ƺͲ���
		string name = script.getCommandName(i);
		vector<string> para = script.getCommandParas(i);

		if (name == "Call")
		{
			//������ʽ����
			if (para.size() != 1)
			{
				error.set(ERROR_PARA_COUNT_MISMATCH("Call", 1), script.getOriginalLineNum(cLine));
				return false;
			}

			string token = para[0];

			//����δ����
			if (blockToken.count(token) == 0)
			{
				error.set(ERROR_TOKEN_UNDEFINE(token), script.getOriginalLineNum(cLine));
				return false;
			}

			//�������ʶ�滻������Ŀ�ʼ�ͽ����к�
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

bool Command::IsStringInt(const string & s)
{
	if (s.size() == 0)
	{
		return false;
	}
	if (!isdigit(s[0]) && !(s[0] == '-'))
	{
		return false;
	}

	for (size_t i = 1; i < s.size(); ++i)
	{
		if (!isdigit(s[i]))
		{
			return false;
		}
	}
	return true;
}

bool Command::IsStringFloat(const string & s)
{
	if (s.size() == 0)
	{
		return false;
	}
	bool flag = false;
	for (size_t i = 0; i < s.size(); ++i)
	{
		if (s[i] == '.')
		{
			if (flag)
			{
				return false;
			}
			else
			{
				flag = true;
			}
		}
		else if (!isdigit(s[i]))
		{
			return false;
		}
	}
	return true;
}

bool Command::IsStringIdentifier(const string & s)
{
	if (s.size() == 0)
	{
		return false;
	}
	if (isdigit(s[0]))
	{
		return false;
	}
	return true;
}
