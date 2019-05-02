#include <iostream>
#include "CommandScript.h"
#include "Test.h"

using namespace std;

//自定义命令：PrintString
class PrintStringCommand : public Command
{
public:
    //返回自定义命令的名称
    string getName() const { return "PrintString"; }

    //para: 传递给命令的参数列表
    //error: 错误类，自定义命令可以调用其setErrorText方法设置错误信息
    bool execute(const vector<string> &para, RuntimeError& error)
    {
        if (para.size() != 1)
        {
            error.setErrorText(ERROR_PARA_COUNT_MISMATCH("PrintString", 1));
            return false;
        }

        if (para[0] == "SPACE")
        {
            cout << " ";
        }
        else if (para[0] == "ENDLINE")
        {

            cout << endl;
        }
        else
        {
            cout << para[0];
        }

        return true;
    }
};

int main()
{
    //测试代码
    TestStatement();
	TestScriptReader(8);
	TestPreProcess(13);
	TestErrorInfo(5);
	cout << endl << endl;

    //初始化自定义命令实例
    PrintStringCommand PrintString;

    //初始化脚本引擎
    CommandScriptEngine engine;

    //向脚本引擎添加命令
    engine.addCommand(PrintString);

    //错误类实例，保存脚本运行时的错误信息
    RuntimeError error;

    //运行脚本
    if (engine.runScript("script.txt", error)) //脚本运行成功
    {
        //输出成功提示
        cout << endl << endl << "Script runs successfully." << endl;
    }
    else //脚本运行失败
    {
        //输出错误信息
        cout << endl << endl << "Line: " << error.getLine() << endl;
        cout << "Info: " << error.getErrorText() << endl;
    }

    return 0;
}
