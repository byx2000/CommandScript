#include <iostream>
#include "CommandScript.h"
#include "Test.h"

using namespace std;

//�Զ������PrintString
class PrintStringCommand : public Command
{
public:
    //�����Զ������������
    string getName() const { return "PrintString"; }

    //para: ���ݸ�����Ĳ����б�
    //error: �����࣬�Զ���������Ե�����setErrorText�������ô�����Ϣ
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
    //���Դ���
    TestStatement();
	TestScriptReader(8);
	TestPreProcess(13);
	TestErrorInfo(5);
	cout << endl << endl;

    //��ʼ���Զ�������ʵ��
    PrintStringCommand PrintString;

    //��ʼ���ű�����
    CommandScriptEngine engine;

    //��ű������������
    engine.addCommand(PrintString);

    //������ʵ��������ű�����ʱ�Ĵ�����Ϣ
    RuntimeError error;

    //���нű�
    if (engine.runScript("script.txt", error)) //�ű����гɹ�
    {
        //����ɹ���ʾ
        cout << endl << endl << "Script runs successfully." << endl;
    }
    else //�ű�����ʧ��
    {
        //���������Ϣ
        cout << endl << endl << "Line: " << error.getLine() << endl;
        cout << "Info: " << error.getErrorText() << endl;
    }

    return 0;
}
