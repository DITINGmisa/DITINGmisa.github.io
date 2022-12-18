#ifndef LIB_H //就是头文件名（全大写后加个_H
#define LIB_H
#include "kll.h"
#include <iomanip>
#include <cstring>
static const char* smdf = "MovieBase.txt";
static const char* smif = "Noidx.txt";
static const char* slf1 = "Nameidx.txt";
static const char* slf2 = "Directoridx.txt";
static const char* slf3 = "Tableidx.txt";
static const char* saf = "Audience.txt";
static const char* smrf = "Record.txt";
static const char* smrnf = "RecordNew.txt";
static const char* srcf = "Recommend.txt";
using namespace std;
class Lib
{
public:
	//取MovieDbaseFile结构的len
	int getNrbooks()const {
		return mdf.len;
	}
	//追加一条主数据库记录
	void AppDbaseRec(const MovieRecType& newbook)
	{
		mdf.append(newbook);
	}
	//计算相似度
	float similar(int aNo1, int aNo2)
	{
		float a = 0, b = 0, c = 0, sml = 0;//分别算a+b,b+c两部分，sml = 1-(a + b + c)/( count1 + count2 + count3 )/10
		int count1 = 0, count2 = 0, count3 = 0;
		for (int i = 0; i < mrf.len; i++)
		{
			if (mrf.Record[i].aNo == aNo1)
			{
				int t = -1;
				for (int j = 0; j < mrf.len; j++)
				{
					if (mrf.Record[j].aNo == aNo2 && mrf.Record[j].mNo == mrf.Record[i].mNo)
					{
						b += abs(mrf.Record[i].valuate - mrf.Record[j].valuate);
						t = 0;
						count2++;
					}
				}
				if (t == -1)
				{
					a += mrf.Record[i].valuate / 2;
					count1++;
				}
			}
		}
		for (int i = 0; i < mrf.len; i++)
		{
			if (mrf.Record[i].aNo == aNo2)
			{
				int t = -1;
				for (int j = 0; j < mrf.len; j++)
				{
					if (mrf.Record[j].aNo == aNo1 && mrf.Record[j].mNo == mrf.Record[i].mNo)
					{
						t = 0;
					}
				}
				if (t == -1)
				{
					c += mrf.Record[i].valuate / 2;
					count3++;
				}
			}
		}
		sml = 1 - (a + b + c) / (count1 + count2 + count3) / 10;
		return sml;
	}
	//兴趣评级
	float recomrank(int n, int aNo)
	{
		float rank;
		rank = similar(mrf.Record[n].aNo, aNo) * (0.7 * mrf.Record[n].valuate + 0.3 * mdf.MovieDbase[MNoFind(mrf.Record[n].mNo)].star);
		return rank;
	}
	//按浏览量排序
	void sort1()
	{
		int i, j;
		RecommendRecType* temp = (RecommendRecType*)malloc(sizeof(RecommendRecType));
		for (i = 0; i < 4; i++)
		{
			for (j = 0; j < 4 - i; j++)
			{
				if (mdf.MovieDbase[rcf.RecommendDbase[j].num].watchnum < mdf.MovieDbase[rcf.RecommendDbase[j + 1].num].watchnum)
				{
					temp->num = rcf.RecommendDbase[j].num;
					rcf.RecommendDbase[j].num = rcf.RecommendDbase[j + 1].num;
					rcf.RecommendDbase[j + 1].num = temp->num;
					temp->recorank = rcf.RecommendDbase[j].recorank;
					rcf.RecommendDbase[j].recorank = rcf.RecommendDbase[j + 1].recorank;
					rcf.RecommendDbase[j + 1].recorank = temp->recorank;
				}
			}

		}
	}
	//按评分排序
	void sort2()
	{
		int i, j;
		RecommendRecType* temp = (RecommendRecType*)malloc(sizeof(RecommendRecType));
		for (i = 5; i < 10; i++)
		{
			for (j = 5; j < 9; j++)
				if (mdf.MovieDbase[rcf.RecommendDbase[j].num].star < mdf.MovieDbase[rcf.RecommendDbase[j + 1].num].star)
				{
					temp->num = rcf.RecommendDbase[j].num;
					rcf.RecommendDbase[j].num = rcf.RecommendDbase[j + 1].num;
					rcf.RecommendDbase[j + 1].num = temp->num;
					temp->recorank = rcf.RecommendDbase[j].recorank;
					rcf.RecommendDbase[j].recorank = rcf.RecommendDbase[j + 1].recorank;
					rcf.RecommendDbase[j + 1].recorank = temp->recorank;
				}
		}
	}
	//个性化排序
	void sort3(int aNo)
	{
		int i, j;
		RecommendRecType* temp = (RecommendRecType*)malloc(sizeof(RecommendRecType));
		for (i = aNo * 5 + 5; i < aNo * 5 + 10; i++)
		{
			for (j = aNo * 5 + 5; j < aNo * 5 + 9; j++)
				if (rcf.RecommendDbase[j].recorank < rcf.RecommendDbase[j + 1].recorank)
				{
					temp->num = rcf.RecommendDbase[j].num;
					rcf.RecommendDbase[j].num = rcf.RecommendDbase[j + 1].num;
					rcf.RecommendDbase[j + 1].num = temp->num;
					temp->recorank = rcf.RecommendDbase[j].recorank;
					rcf.RecommendDbase[j].recorank = rcf.RecommendDbase[j + 1].recorank;
					rcf.RecommendDbase[j + 1].recorank = temp->recorank;
				}
		}
	}
	//按浏览量推荐
	void recommend1()
	{
		for (int i = 0; i < mdf.len; i++)
		{
			if (mdf.MovieDbase[i].watchnum > mdf.MovieDbase[rcf.RecommendDbase[4].num].watchnum && i != rcf.RecommendDbase[0].num && i != rcf.RecommendDbase[1].num
				&& i != rcf.RecommendDbase[2].num && i != rcf.RecommendDbase[3].num && i != rcf.RecommendDbase[4].num)
			{
				rcf.RecommendDbase[4].num = i;
				sort1();
			}
		}
		cout << "最多播放：\n" << "=============================================\n"
			<< "编号  影片名    导演    标签   星级    价格   浏览量\n";
		for (int i = 0; i < 5; i++)
		{
			MovieRecType tmp = mdf.MovieDbase[rcf.RecommendDbase[i].num];
			cout << tmp.mNo << "\t" << tmp.mName << "\t"
				<< tmp.director << "\t" << tmp.table << "\t"
				<< tmp.star << "\t" << tmp.cost << "\t" << tmp.watchnum << "\n";
		}
		cout << endl << "=============================================\n\n";
	}
	//按评分推荐
	void recommend2()
	{
		for (int i = 0; i < mdf.len; i++)
		{
			if (mdf.MovieDbase[i].star > mdf.MovieDbase[rcf.RecommendDbase[9].num].star && i != rcf.RecommendDbase[5].num && i != rcf.RecommendDbase[6].num
				&& i != rcf.RecommendDbase[7].num && i != rcf.RecommendDbase[8].num && i != rcf.RecommendDbase[9].num)
			{
				rcf.RecommendDbase[9].num = i;
				sort2();
			}
		}
		cout << "最受好评：\n" << "=============================================\n"
			<< "编号  影片名    导演    标签   星级    价格   浏览量\n";
		for (int i = 5; i < 10; i++)
		{
			MovieRecType tmp = mdf.MovieDbase[rcf.RecommendDbase[i].num];
			cout << tmp.mNo << "\t" << tmp.mName << "\t"
				<< tmp.director << "\t" << tmp.table << "\t"
				<< tmp.star << "\t" << tmp.cost << "\t" << tmp.watchnum << "\n";
		}
		cout << endl << "=============================================\n\n";
	}
	//个性化推荐
	void recommend3(int aNo)
	{
		int flag = -1;
		for (int i = 0; i < mrf.len; i++)
		{
			//cout << mrf.Record[i].mNo << "  " << recomrank(i, aNo) << endl;
			for (int j = 0; j < mrf.len; j++)
			{
				if (mrf.Record[j].aNo == aNo && mrf.Record[j].mNo == mrf.Record[i].mNo)
				{
					flag = 0;
				}
			}
			if (flag == 0)
			{
				flag = -1;
			}
			else
			{
				//cout << mrf.Record[i].mNo << "    " << recomrank(i, aNo) << endl;
				if (recomrank(i, aNo) > rcf.RecommendDbase[aNo * 5 + 9].recorank && mrf.Record[i].mNo != rcf.RecommendDbase[aNo * 5 + 5].num
					&& mrf.Record[i].mNo != rcf.RecommendDbase[aNo * 5 + 6].num && mrf.Record[i].mNo != rcf.RecommendDbase[aNo * 5 + 7].num
					&& mrf.Record[i].mNo != rcf.RecommendDbase[aNo * 5 + 8].num && mrf.Record[i].mNo != rcf.RecommendDbase[aNo * 5 + 9].num)
				{
					rcf.RecommendDbase[aNo * 5 + 9].num = mrf.Record[i].mNo;
					rcf.RecommendDbase[aNo * 5 + 9].recorank = recomrank(i, aNo);
					sort3(aNo);
				}
			}
		}
		cout << "\n会员号" << aNo << ",为您个性推荐：\n" << "=============================================\n"
			<< "编号" << "\t" << "影片名" << "\t" << "导演" << "\t" << "标签" << "\t" << "星级" << "\t" << "价格"
			<< "\t" << "浏览量" << "\t" << "推荐度\n";
		for (int i = aNo * 5 + 5; i < aNo * 5 + 9; i++)
		{
			MovieRecType tmp = mdf.MovieDbase[MNoFind(rcf.RecommendDbase[i].num)];
			cout << tmp.mNo << "\t" << tmp.mName << "\t"
				<< tmp.director << "\t" << tmp.table << "\t"
				<< tmp.star << "\t" << tmp.cost << "\t"
				<< tmp.watchnum << "\t" << rcf.RecommendDbase[i].recorank << "\n";
		}
		cout << endl << "=============================================\n\n";
	}

	//初始化
	void Int()
	{
		RecommendRecType tmp;
		if (rcf.len <= 0)
		{
			for (int i = 0; i < 10 + af.len * 5; i++)
			{
				tmp.num = i;
				tmp.recorank = 0;
				rcf.append(tmp);
			}

		}

		sort1();
		sort2();
	}
	//修改编号索引表
	void ChangeNoF(int indexofrec)
	{
		int i = indexofrec;
		int j = mif.len - 1;//目前长度
		int k = 0;
		while (j >= 0)
		{
			//找插入位置
			if (mdf.MovieDbase[i].mNo > mif.BnoIdx[j].mNo)
			{
				k = j + 1;
				break;
			}
			j--;
		}
		//在有序索引表中插入一个索引记录
		MIDRecType tmp;
		tmp.mNo = mdf.MovieDbase[i].mNo;
		tmp.RecNo = i;
		mif.append(tmp);
		if (mif.len > 1)								//有序表的插入 
			for (j = mif.len - 2; j >= k; j--)
			{
				mif.BnoIdx[j + 1] = mif.BnoIdx[j];	//记录后移
			}
		mif.BnoIdx[k].mNo = mdf.MovieDbase[i].mNo;
		mif.BnoIdx[k].RecNo = i;
	}
	//修改电影名索引和链头索引
	void ChangeLinkHeadF1(int indexofrec)
	{
		//修改电影当前记录
		int i, j, k;
		char mn[100];
		i = indexofrec;
		strcpy(mn, mdf.MovieDbase[i].mName);			//取记录中的电影名送至变量mn中
		j = 0; k = -1;
		while (j < lf1.len)
		{
			//在链头中查找与次关键字相等的记录
			if (strcmp(mn, lf1.LHFrec1[j].mName) == 0)
			{
				k = j;
				break;
			}
			j++;
		}
		//采用头插法建立索引
		if (k != -1)
		{
			mdf.MovieDbase[i].namenext = lf1.LHFrec1[k].lhead;
			lf1.LHFrec1[k].lhead = i;			//i为主文件的当前记录号
			lf1.LHFrec1[k].RecNum++;
		}
		else
		{
			mdf.MovieDbase[i].namenext = -1;		//用头插法建立链表，指针置-1
			MNRecType tmp;
			tmp.lhead = i;						//i为主文件的当前记录号
			tmp.RecNum = 1;						//计数器置1
			strcpy(tmp.mName, mn);
			lf1.append(tmp);
		}
	}
	//修改导演索引和链头索引
	void ChangeLinkHeadF2(int indexofrec)
	{
		//修改影片当前记录
		int i, j, k;
		char dir[100];
		i = indexofrec;
		strcpy(dir, mdf.MovieDbase[i].director);			//取记录中的导演送至变量dir中
		j = 0; k = -1;
		while (j < lf2.len)
		{
			//在链头中查找与次关键字相等的记录
			if (strcmp(dir, lf2.LHFrec2[j].director) == 0)
			{
				k = j;
				break;
			}
			j++;
		}
		if (k != -1)
		{
			mdf.MovieDbase[i].dirnext = lf2.LHFrec2[k].lhead;
			lf2.LHFrec2[k].lhead = i;			//i为主文件的当前记录号
			lf2.LHFrec2[k].RecNum++;
		}
		else
		{
			MDRecType tmp;
			tmp.lhead = i;						//i为主文件的当前记录号
			tmp.RecNum = 1;						//计数器置1
			strcpy(tmp.director, dir);
			lf2.append(tmp);
			mdf.MovieDbase[i].dirnext = -1;
		}
	}
	//修改关键词1索引和链头索引
	void ChangeLinkHeadF3(int indexofrec)
	{
		//修改电影当前记录
		int i, j, k;
		char table[11];
		i = indexofrec;
		strcpy(table, mdf.MovieDbase[i].table);			//取记录中的关键词送至变量table中
		j = 0; k = -1;
		while (j < lf3.len)
		{
			//在链头中查找与次关键字相等的记录
			if (strcmp(table, lf3.LHFrec3[j].table) == 0)
			{
				k = j;
				break;
			}
			j++;
		}
		if (k != -1)
		{
			mdf.MovieDbase[i].tblnext = lf3.LHFrec3[k].lhead;
			lf3.LHFrec3[k].lhead = i;			//i为主文件的当前记录号
			lf3.LHFrec3[k].RecNum++;
		}
		else
		{
			MTRecType tmp;
			tmp.lhead = i;						//i为主文件的当前记录号
			tmp.RecNum = 1;						//计数器置1
			strcpy(tmp.table, table);
			lf3.append(tmp);
			mdf.MovieDbase[i].tblnext = -1;
		}
	}
	//按编号二分查找
	int MNoFind(int key)
	{
		int low = 0;
		int high = mif.len - 1;
		while (low <= high)
		{
			int mid = (low + high) / 2;
			if (key == mif.BnoIdx[mid].mNo)
				return mif.BnoIdx[mid].RecNo;
			else if (key < mif.BnoIdx[mid].mNo)
				high = mid - 1;
			else
				low = mid + 1;
		}
		return -1;//没找到
	}
	//按片名查找
	int MNameFind(char* key)
	{
		int k = -1;
		for (int i = 0; i < lf1.len; i++)
		{
			if (strcmp(key, lf1.LHFrec1[i].mName) == 0)
			{
				k = lf1.LHFrec1[i].lhead;
				break;
			}
		}
		return k;
	}
	//按导演查找
	int MDirFind(char* key)
	{
		int k = -1;
		for (int i = 0; i < lf2.len; i++)
		{
			if (strcmp(key, lf2.LHFrec2[i].director) == 0)
			{
				k = lf2.LHFrec2[i].lhead;
				break;
			}
		}
		return k;
	}
	//按标签查找
	int MTabFind(char* key)
	{
		int k = -1;
		for (int i = 0; i < lf3.len; i++)
		{
			if (strcmp(key, lf3.LHFrec3[i].table) == 0)
			{
				k = lf3.LHFrec3[i].lhead;
				break;
			}
		}
		return k;
	}
	//输出一条主数据库记录
	void ShowRec(int indexofrec)
	{
		int i = indexofrec;
		const MovieRecType& tmp = mdf.MovieDbase[i];
		cout << "\n编号  影片名    导演    标签   星级    价格   浏览量\n"
			<< tmp.mNo << "\t" << tmp.mName << "\t"
			<< tmp.director << "\t" << tmp.table << "\t"
			<< tmp.star << "\t" << tmp.cost << "\t" << tmp.watchnum
			<< endl << "=============================================\n";
	}
	//电影查询控制
	void SearchMovie()
	{
		char mn[100], dir[100], table[11];
		int k, sel;
		int mno;
		while (true)
		{
			cout << "\n\n\n按关键字查询\n"
				<< "--------------\n"
				<< "1.编号  2.片名\n"
				<< "3.导演  4.类型标签\n"
				<< "5.退出查询\n"
				<< "--------------\n"
				<< "请选择操作：";
			cin >> sel;
			switch (sel)
			{
			case 1:
				cout << "输入编号：";
				cin >> mno;
				k = MNoFind(mno);
				if (k == -1)
				{
					cout << "没有找到编号为 " << mno << " 的影片，请检查是否输入出错\n";
				}
				else ShowRec(k);
				break;
			case 2:
				cout << "输入片名：";
				cin >> mn;
				k = MNameFind(mn);
				if (k == -1)
				{
					cout << "没有找到片名为 " << mn << " 的影片，请检查是否输入出错\n";
				}
				else
				{
					for (int i = k; i != -1; i = mdf.MovieDbase[i].namenext)
						ShowRec(i);
				}
				break;
			case 3:
				cout << "输入导演名：";
				cin >> dir;
				k = MDirFind(dir);
				if (k == -1)
				{
					cout << "没有找到导演名为 " << dir << " 的影片，请检查是否输入出错\n";
				}
				else
				{
					for (int i = k; i != -1; i = mdf.MovieDbase[i].dirnext)
						ShowRec(i);
				}
				break;
			case 4:
				cout << "\n\n\n影片类型有：\n"
					<< "爱情\n动画\n动作\n古装\n剧情\n科幻\n恐怖历史\n奇幻\n武侠\n喜剧\n悬疑\n"
					<< "输入类型标签：";
				cin >> table;
				k = MTabFind(table);
				if (k == -1)
				{
					cout << "没有找到类型标签为 " << table << " 的影片，请检查是否输入出错\n";
				}
				else
				{
					for (int i = k; i != -1; i = mdf.MovieDbase[i].tblnext)
						ShowRec(i);
				}
				break;
			case 5:
				return;
			}
		}
	}
	//服务器端更新数据
	void upgrade()
	{
		MRecordRecType tmp;
		ifstream fin;
		int len,aNo,money;
		fin.open(smrnf);
		if (!fin.is_open())
		{
			cout << "打开文件错误！";
		}
		else
		{
			while (1)
			{
				len = -2;
				fin >> len;
				if (len < 0 && len!=-1)
				{
					break;
				}
				else if(len >= 0)
				{
					for (int i = 0; i < len; i++)
					{
						fin >> tmp.aNo >> tmp.mNo >> tmp.valuate;
						mdf.MovieDbase[MNoFind(tmp.mNo)].star = ((float)mdf.MovieDbase[MNoFind(tmp.mNo)].star * (float)mdf.MovieDbase[MNoFind(tmp.mNo)].watchnum + (float)tmp.valuate) / ((float)mdf.MovieDbase[MNoFind(tmp.mNo)].watchnum + 1);
						mdf.MovieDbase[MNoFind(tmp.mNo)].watchnum++;
						mrf.append(tmp);

					}
					cout << "新写入" << len << "条观影记录" << endl;
					len = -2;
				}
				else
				{
					fin >> len;
					for (int i = 0; i < len; i++)
					{
						fin >> aNo >> money;
						for (int j = 0; j < af.len; j++)
						{
							if (af.AudiRec[j].aNo == aNo)
							{
								af.AudiRec[j].money = money;
							}
						}
					}
					cout << "更新了" << len << "条用户信息" << endl;
					len = -2;
				}
			}
		}
		ofstream del(smrnf, ios_base::out);
		fin.close();
	}
	void DataManage()
	{
		int j;
		while (1)
		{
			cout << "\n\n数据交互子系统\n"
				<< "--------------\n"
				<< "1.获取客户端信息分\n"
				<< "2.更新评分信息\n"
				<< "3.退出系统\n"
				<< "--------------\n"
				<< "请选择1-3：";
			cin >> j;
			switch (j)
			{
			case 1:
				cout << "若要获取客户端信息，请新开终端运行server.py等待用户发送请求\n";
				break;
			case 2:
				//将最新收集的观影记录纳入计算范围
				upgrade();
				break;
			case 3:
				return;
			}

		}
	}
	void MovieManage()
	{
		int j;
		char yn = 'y';
		int index;
		while (1)
		{
			cout << "\n\n影片管理子系统\n"
				<< "--------------\n"
				<< "1.影片信息输入\n"
				<< "2.影片信息查询\n"
				<< "3.退出系统\n"
				<< "--------------\n"
				<< "请选择1-3：";
			cin >> j;
			switch (j)
			{
			case 1:
				while (yn == 'y' || yn == 'Y')
				{
					MovieRecType tmp;
					cout << "编号  影 片 名   导演  标签  价格  网址  密码\n";
					cin >> tmp.mNo >> tmp.mName >> tmp.director
						>> tmp.table >> tmp.cost >> tmp.location >> tmp.password;
					tmp.watchnum = 0;
					tmp.star = 0.0;
					AppDbaseRec(tmp);
					index = getNrbooks() - 1;
					ChangeNoF(index);
					ChangeLinkHeadF1(index);
					ChangeLinkHeadF2(index);
					ChangeLinkHeadF3(index);
					cout << "是否继续输入？(y/n):";
					cin >> yn;
				}
				break;
			case 2:
				SearchMovie();
				break;
			case 3:
				return;
			}
		}
	}
	//会员管理
	void ReaderManage()
	{

		int j, k = -1;
		int l;
		char yn = 'y';
		int auid, money;
		while (1)
		{
			cout << "\n1.新增会员   2.会员充值   3.查询余额  4.退出系统"
				<< "\n请输入：";
			cin >> l;
			switch (l)
			{
			case 1:
				while (yn == 'y' || yn == 'Y')
				{
					MARecType reader;
					cout << "输入会员号  会员名  密码  金额：" << "\n";
					cin >> reader.aNo >> reader.name >> reader.password >> reader.money;
					af.append(reader);
					cout << "已有" << af.len << "个会员\n";
					cout << "是否继续输入？(y/n):";
					cin >> yn;
				}
				break;
			case 2:
				cout << "输入会员号:";
				cin >> auid;
				for (int i = 0; i < af.len; i++)
				{
					if (auid == af.AudiRec[i].aNo)
					{
						k = i;
						break;
					}
				}
				if (k == -1)
				{
					cout << "查无此人\n";
					return;
				}
				else
				{
					cout << "输入充值金额:";
					cin >> money;
					af.AudiRec[k].money += money;
					cout << "当前余额：" << af.AudiRec[k].money << "\n";;
				}
				break;
			case 3:
				cout << "输入会员号:";
				cin >> auid;
				for (int i = 0; i < af.len; i++)
				{
					if (auid == af.AudiRec[i].aNo)
					{
						k = i;
						break;
					}
				}
				if (k == -1)
				{
					cout << "查无此人\n";
					return;
				}
				else
				{
					cout << "当前余额：" << af.AudiRec[k].money << "\n";;
				}
				break;
			case 4:
				return;
			}
		}
	}
	void MovieRcommend()
	{
		int aNo, j = -1;
		cout << "请输入会员号：";
		cin >> aNo;
		for (int i = 0; i < af.len; i++)
		{
			if (aNo == af.AudiRec[i].aNo)
			{
				j = 1;
			}
		}
		if (j == -1)
		{
			cout << "查无此人！";
		}
		else
		{
			recommend1();
			recommend2();
			recommend3(aNo);
		}
	}
	//写各类文件
	void writeFile()
	{
		//写影片主文件
		ofstream fout0(smdf);
		fout0 << mdf.len << "\n";
		for (int i = 0; i < mdf.len; i++)
		{
			fout0 << mdf.MovieDbase[i].mNo << " " << mdf.MovieDbase[i].mName << " " << mdf.MovieDbase[i].namenext
				<< " " << mdf.MovieDbase[i].director << " " << mdf.MovieDbase[i].dirnext << " " << mdf.MovieDbase[i].table
				<< " " << mdf.MovieDbase[i].tblnext << " " << mdf.MovieDbase[i].star << " " << mdf.MovieDbase[i].watchnum
				<< " " << mdf.MovieDbase[i].cost << " " << mdf.MovieDbase[i].location << " " << mdf.MovieDbase[i].password << "\n";
		}
		fout0.close();
		//写编号索引文件
		ofstream fout1(smif);
		fout1 << mif.len << "\n";
		for (int i = 0; i < mif.len; i++)
		{
			fout1 << mif.BnoIdx[i].mNo << " " << mif.BnoIdx[i].RecNo << "\n";
		}
		fout1.close();
		//写片名索引文件
		ofstream fout2(slf1);
		fout2 << lf1.len << "\n";
		for (int i = 0; i < lf1.len; i++)
		{
			fout2 << lf1.LHFrec1[i].mName << " " << lf1.LHFrec1[i].RecNum << " " << lf1.LHFrec1[i].lhead << "\n";
		}
		fout2.close();
		//写导演索引文件
		ofstream fout3(slf2);
		fout3 << lf2.len << "\n";
		for (int i = 0; i < lf2.len; i++)
		{
			fout3 << lf2.LHFrec2[i].director << " " << lf2.LHFrec2[i].RecNum << " " << lf2.LHFrec2[i].lhead << "\n";
		}
		fout3.close();
		//写标签索引文件
		ofstream fout4(slf3);
		fout4 << lf3.len << "\n";
		for (int i = 0; i < lf3.len; i++)
		{
			fout4 << lf3.LHFrec3[i].table << " " << lf3.LHFrec3[i].RecNum << " " << lf3.LHFrec3[i].lhead << "\n";
		}
		fout4.close();
		//写会员索引文件
		ofstream fout5(saf);
		fout5 << af.len << "\n";
		for (int i = 0; i < af.len; i++)
		{
			fout5 << af.AudiRec[i].aNo << " " << af.AudiRec[i].name << " " << af.AudiRec[i].password << " " << af.AudiRec[i].money << "\n";
		}
		fout5.close();
		//写记录索引文件
		ofstream fout6(smrf);
		fout6 << mrf.len << "\n";
		for (int i = 0; i < mrf.len; i++)
		{
			fout6 << mrf.Record[i].aNo << " " << mrf.Record[i].mNo << " " << mrf.Record[i].valuate << "\n";
		}
		fout6.close();
	}
	//读各类文件
	void readFile()
	{

		//读影片主文件作为输入
		MovieRecType tmp1;
		ifstream fin1;
		int len1;
		fin1.open(smdf);
		fin1 >> len1;
		if (len1 <= 0)
		{
			cout << endl << endl << "读入0条电影" << endl;
		}
		else
		{
			for (int i = 0; i < len1; i++)
			{

				fin1 >> tmp1.mNo >> tmp1.mName >> tmp1.namenext
					>> tmp1.director >> tmp1.dirnext >> tmp1.table
					>> tmp1.tblnext >> tmp1.star >> tmp1.watchnum
					>> tmp1.cost >> tmp1.location >> tmp1.password;
				AppDbaseRec(tmp1);
				int index = getNrbooks() - 1;
				ChangeNoF(index);
				ChangeLinkHeadF1(index);
				ChangeLinkHeadF2(index);
				ChangeLinkHeadF3(index);
			}
			cout << endl << endl << "读入" << len1 << "条电影" << endl;
		}
		fin1.close();
		//读入用户文件
		MARecType tmp2;
		ifstream fin2;
		int len2;
		fin2.open(saf);
		fin2 >> len2;
		if (len2 <= 0)
		{
			cout << "读入0个用户" << endl;
		}
		else
		{
			for (int i = 0; i < len2; i++)
			{
				fin2 >> tmp2.aNo >> tmp2.name >> tmp2.password >> tmp2.money;
				af.append(tmp2);
			}
			cout << "读入" << len2 << "个用户" << endl;

		}
		fin2.close();
		//读入记录文件
		MRecordRecType tmp3;
		ifstream fin3;
		int len3;
		fin3.open(smrf);
		fin3 >> len3;
		if (len3 <= 0)
		{
			cout << "读入0条观影记录" << endl;
		}
		else
		{
			for (int i = 0; i < len3; i++)
			{

				fin3 >> tmp3.aNo >> tmp3.mNo >> tmp3.valuate;
				mrf.append(tmp3);
			}
			cout << "读入" << len3 << "条观影记录" << endl;
		}

		fin3.close();

	}
private:
	MovieDbaseFile	mdf;
	MNoFile		mif;
	LHFile1	lf1;
	LHFile2	lf2;
	LHFile3	lf3;
	AudiFile	af;
	MRecordFile	mrf;
	RecommendFile rcf;
};
#endif