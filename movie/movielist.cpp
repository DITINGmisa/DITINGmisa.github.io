#include "Lib.h"
int main()
{
	int j, k = 1;
	Lib lib;
	cout << "读原始文件中，请稍后。。。\n";
	lib.readFile();
	lib.Int();
	while (true)
	{
		cout << "\n\n影片购买与后台管理系统\n"
			<< "============\n"
			<< "1.会员管理\n"
			<< "2.影片管理\n"
			<< "3.数据交互\n"
			<< "4.退出系统\n"
			<< "===========\n"
			<< "请选择1-4：";
		cin >> k;
		switch (k)
		{
		case 1:
			lib.ReaderManage();
			break;
		case 2:
			lib.MovieManage();
			break;
		case 3:
			lib.DataManage();
			break;
		case 4:
			cout << "写入文件中，请稍后。。。\n";
			lib.writeFile();
			cout << "再见！\n";
			return 0;
		}

	}
	return 0;
}