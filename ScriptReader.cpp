#include "ScriptReader.h"

void ScriptReader::readScript(const string & file)
{
	this->file = file;
	//���ļ�����ű�
	ifstream fin(file);
	string line;
	size_t cLine = 1;
	while (getline(fin, line))
	{
		//ȥ��˫б��ע��
		size_t index = line.find("//");
		if (index != string::npos)
		{
			line.erase(index);
		}

		//ȥ����β�ո�
		trim(line);

		if (line.size() != 0)
		{
			script.push_back(Statement(line, cLine));
		}
		cLine++;
	}
}

string ScriptReader::toString() const
{
	string res = "";
	for (size_t i = 0; i < script.size(); ++i)
	{
		res += (script[i].getName() + " ");
		for (size_t j = 1; j <= script[i].getParaNum(); ++j)
		{
			res += (script[i].getPara(j) + " ");
		}
		trim(res);
		res += "\n";
	}
	res += "\n";
	trim(res);
	return res;
}

void ScriptReader::print() const
{
	cout << toString() << endl;
}

void ScriptReader::trim(string & s)
{
	if (!s.empty())
	{
		s.erase(0, s.find_first_not_of(" \t\f\v\n\r"));
		s.erase(s.find_last_not_of(" \t\f\v\n\r") + 1);
	}
}

void Statement::parse(const string & statement)
{
	para.clear();
	//��һ���������������������ƺͲ���
	stringstream ss(statement);
	ss >> name;
	string p;
	while (ss >> p)
	{
		para.push_back(p);
	}
}

string Statement::toString() const
{
	string res = "";
	res += "Command name: ";
	res += (name + "\n");
	res += "Command para: ";
	for (size_t i = 0; i < para.size(); ++i)
	{
		res += (para[i] + " ");
	}
	return res;
}

void Statement::print() const
{
	cout << toString() << endl;
}
