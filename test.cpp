//学习一下命令行参数
#include <iostream>
#include <unistd.h>
using namespace std;

int main(int argc, char **argv)
{
    cout << "Hello, from StackOperation!\n";
    //命令行输入的参数数量（至少为1）
    cout << "argc=" << argc << endl;
    //第一个参数，就是文件名
    cout << "argv[0]=" << argv[0] << endl;
    //第二个参数
    cout << "argv[1]=" << argv[1] << endl;
    //第三个参数
    cout << "argv[2]=" << argv[2] << endl;
    pause();
    //暂停，需要手动关闭。
    return 0;
}
