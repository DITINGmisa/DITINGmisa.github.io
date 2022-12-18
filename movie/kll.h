#ifndef KLL_H //就是头文件名（全大写后加个_H
#define KLL_H

#include"iostream"
#include"fstream"
#include <cstring>

using namespace std;
//1）定义主数据库文件
struct MovieRecType
{
	int mNo;//编号
	char mName[100];
	int namenext;
	char director[100];
	int dirnext;
	char table[11];
	int tblnext;
	float star;
	int cost;
	int watchnum;
	char location[20];
	char password[8];

	void serilize(ofstream& out)
	{
		out.write((const char*)this, sizeof(*this));
	}
	void deserilize(ifstream& in)
	{
		in.read((char*)this, sizeof(*this));
	}
	MovieRecType& operator=(const MovieRecType& other)
	{
		memcpy(this, &other, sizeof(*this));
		return *this;
	}
};

struct MovieDbaseFile {
	int len;
	int allocated;
	MovieRecType* MovieDbase;
	void serilize(ofstream& out)
	{
		out << len;
		for (int i = 0; i < len; i++)
		{
			MovieDbase[i].serilize(out);
			//		    cout << i<<endl;
		}

	}
	void deserilize(ifstream& in)
	{
		if (MovieDbase != NULL)
			delete[] MovieDbase;
		in >> len;
		cout << "读入" << len << "条记录\n";
		MovieDbase = new MovieRecType[len * 2 + 50];
		allocated = 2 * len + 50;
		for (int i = 0; i < len; i++)
		{
			MovieDbase[i].deserilize(in);
			cout << MovieDbase[i].mNo << endl;
		}

	}
	MovieDbaseFile() :len(0), allocated(100) {
		MovieDbase = new MovieRecType[allocated];
	}
	~MovieDbaseFile() {
		if (MovieDbase != NULL)
		{
			delete[] MovieDbase;
		}
		len = 0;
		MovieDbase = NULL;
	}
	void append(const MovieRecType& n)
	{
		if (allocated == len)
		{
			allocated = allocated * 2;
			MovieRecType* nb = new MovieRecType[allocated];
			memcpy(nb, MovieDbase, len * sizeof(MovieRecType));
			delete[] MovieDbase;
			MovieDbase = nb;
		}
		MovieDbase[len] = n;
		len++;
		cout << "已有" << len << "条记录。\n";
	}
};

//2)定义编号索引文件
struct MIDRecType {
	int mNo;
	int RecNo;
	void serilize(ofstream& out)
	{
		out.write((const char*)this, sizeof(*this));
	}
	void deserilize(ifstream& in)
	{
		in.read((char*)this, sizeof(*this));
	}
	MIDRecType& operator=(const MIDRecType& other)
	{
		mNo = other.mNo;
		RecNo = other.RecNo;
		return *this;
	}
};

struct MNoFile {
	int len;
	int allocated;
	MIDRecType* BnoIdx;

	void serilize(ofstream& out)
	{
		out << len;
		cout << "写入" << len << "条记录\n";
		for (int i = 0; i < len; i++)
		{
			BnoIdx[i].serilize(out);
		}
	}

	void deserilize(ifstream& in)
	{
		in >> len;
		cout << "读入" << len << "条索引\n";
		allocated = 2 * len + 50;
		if (BnoIdx != NULL)
		{
			delete[]BnoIdx;
		}
		BnoIdx = new MIDRecType[allocated];
		for (int i = 0; i < len; i++)
		{
			//cout << BnoIdx[i].RecNo<<endl;
			//cout << BnoIdx[i].mNo << endl;
			BnoIdx[i].deserilize(in);
		}
	}

	void append(const MIDRecType& bir)
	{
		if (allocated == len)
		{
			allocated = allocated * 2;
			MIDRecType* nb = new MIDRecType[allocated];
			memcpy(nb, BnoIdx, len * sizeof(MIDRecType));
			delete[]BnoIdx;
			BnoIdx = nb;
		}
		BnoIdx[len] = bir;
		len++;
	}
	~MNoFile()
	{
		if (BnoIdx != NULL)
			delete[]BnoIdx;
		BnoIdx = NULL;
		len = 0;
	}
	MNoFile() :len(0), allocated(100)
	{
		BnoIdx = new MIDRecType[allocated];
	}
};

//3)定义片名链头索引的文件
struct MNRecType {
	char mName[100];
	int lhead;
	int RecNum;
	void serilize(ofstream& out)
	{
		out.write((const char*)this, sizeof(*this));
	}
	void deserilize(ifstream& in)
	{
		in.read((char*)this, sizeof(*this));
	}
	MNRecType& operator=(const MNRecType& o)
	{
		memcpy(this, &o, sizeof(*this));
		return *this;
	}
};

struct LHFile1 {
	int len;
	int allocated;
	MNRecType* LHFrec1;
	LHFile1() :len(0), allocated(100)
	{
		LHFrec1 = new MNRecType[allocated];
	}
	~LHFile1()
	{
		if (LHFrec1 != NULL)
			delete[]LHFrec1;
		len = 0;
		LHFrec1 = NULL;
	}

	void serilize(ofstream& out)
	{
		out << len;
		for (int i = 0; i < len; i++)
			LHFrec1[i].serilize(out);
	}

	void deserilize(ifstream& in)
	{
		in >> len;
		if (LHFrec1 != NULL)
		{
			delete[]LHFrec1;
		}
		allocated = 2 * len + 50;
		LHFrec1 = new MNRecType[allocated];
		for (int i = 0; i < len; i++)
		{
			LHFrec1[i].deserilize(in);
		}
	}

	void append(const MNRecType& o)
	{
		if (len == allocated)
		{
			allocated *= 2;
			MNRecType* nb = new MNRecType[allocated];
			memcpy(nb, LHFrec1, sizeof(MNRecType) * len);
			delete[]LHFrec1;
			LHFrec1 = nb;
		}
		LHFrec1[len] = o;
		len++;
	}
};

//4)定义导演链头索引文件
struct MDRecType {
	char director[100];
	int lhead;
	int RecNum;
	void serilize(ofstream& out)
	{
		out.write((const char*)this, sizeof(*this));
	}
	void deserilize(ifstream& in)
	{
		in.read((char*)this, sizeof(*this));
	}
	MDRecType& operator=(const MDRecType& o)
	{
		memcpy(this, &o, sizeof(*this));
		return *this;
	}
};

struct LHFile2 {
	int len;
	int allocated;
	MDRecType* LHFrec2;
	LHFile2() :len(0), allocated(100) {
		LHFrec2 = new MDRecType[allocated];
	}
	~LHFile2()
	{
		len = 0;
		if (LHFrec2 != NULL)
			delete[]LHFrec2;
		LHFrec2 = NULL;
	}
	void serilize(ofstream& out)
	{
		out << len;
		for (int i = 0; i < len; i++)
		{
			LHFrec2[i].serilize(out);
		}
	}
	void deserilize(ifstream& in)
	{
		in >> len;
		allocated = 2 * len + 50;
		if (LHFrec2 != NULL)
		{
			delete[]LHFrec2;
		}
		LHFrec2 = new MDRecType[allocated];
		for (int i = 0; i < len; i++)
		{
			LHFrec2[i].deserilize(in);
		}
	}
	void append(const MDRecType& o)
	{
		if (len == allocated)
		{
			allocated *= 2;
			MDRecType* nb = new MDRecType[allocated];
			memcpy(nb, LHFrec2, sizeof(MDRecType) * len);
			delete[]LHFrec2;
			LHFrec2 = nb;
		}
		LHFrec2[len] = o;
		len++;
	}
};

//5)定义标签链头索引文件
struct MTRecType {
	char table[11];
	int lhead;
	int RecNum;
	void serilize(ofstream& out)
	{
		out.write((const char*)this, sizeof(*this));
	}
	void deserilize(ifstream& in)
	{
		in.read((char*)this, sizeof(*this));
	}
	MTRecType& operator=(const MTRecType& o)
	{
		memcpy(this, &o, sizeof(*this));
		return *this;
	}
};

struct LHFile3 {
	int len;
	int allocated;
	MTRecType* LHFrec3;
	LHFile3() :len(0), allocated(100) {
		LHFrec3 = new MTRecType[allocated];
	}
	~LHFile3()
	{
		if (LHFrec3 != NULL)
			delete[]LHFrec3;
		len = 0;
		LHFrec3 = NULL;
	}
	void serilize(ofstream& out)
	{
		out << len;
		for (int i = 0; i < len; i++)
		{
			LHFrec3[i].serilize(out);
		}
	}
	void deserilize(ifstream& in)
	{
		in >> len;
		allocated = 2 * len + 50;
		if (LHFrec3 != NULL)
		{
			delete[]LHFrec3;
		}
		LHFrec3 = new MTRecType[allocated];
		for (int i = 0; i < len; i++)
		{
			LHFrec3[i].deserilize(in);
		}
	}
	void append(const MTRecType& o)
	{
		if (len == allocated)
		{
			allocated *= 2;
			MTRecType* nb = new MTRecType[allocated];
			memcpy(nb, LHFrec3, sizeof(MTRecType) * len);
			delete[]LHFrec3;
			LHFrec3 = nb;
		}
		LHFrec3[len] = o;
		len++;
	}
};

//6)定义观众文件
struct MARecType {
	int aNo;
	char name[8];
	char password[9];
	int money;
	void serilize(ofstream& out)
	{
		out.write((const char*)this, sizeof(*this));
	}
	void deserilize(ifstream& in)
	{
		in.read((char*)this, sizeof(*this));
	}
	MARecType& operator=(const MARecType& o)
	{
		memcpy(this, &o, sizeof(*this));
		return *this;
	}
};

struct AudiFile {
	int len;
	int allocated;
	MARecType* AudiRec;
	AudiFile() :len(0), allocated(100) {
		AudiRec = new MARecType[allocated];
	}
	~AudiFile()
	{
		if (AudiRec != NULL)
			delete[]AudiRec;
		len = 0;
	}
	void serilize(ofstream& out)
	{
		out << len;
		for (int i = 0; i < len; i++)
		{
			AudiRec[i].serilize(out);
		}
	}
	void deserilize(ifstream& in)
	{
		in >> len;
		allocated = 2 * len + 50;
		if (AudiRec != NULL)
		{
			delete[]AudiRec;
		}
		AudiRec = new MARecType[allocated];
		for (int i = 0; i < len; i++)
		{
			AudiRec[i].deserilize(in);
		}
	}
	void append(const MARecType& o)
	{
		if (len == allocated)
		{
			allocated *= 2;
			MARecType* nb = new MARecType[allocated];
			memcpy(nb, AudiRec, sizeof(MARecType) * len);
			delete[]AudiRec;
			AudiRec = nb;
		}
		AudiRec[len] = o;
		len++;
	}
};

//7)定义用户观影记录系统
struct MRecordRecType {
	int aNo;
	int mNo;
	double valuate = -1;
	void serilize(ofstream& out)
	{
		out.write((const char*)this, sizeof(*this));
	}
	void deserilize(ifstream& in)
	{
		in.read((char*)this, sizeof(*this));
	}
	MRecordRecType& operator=(const MRecordRecType& o)
	{
		aNo = o.aNo;
		mNo = o.mNo;
		valuate = o.valuate;
		return *this;
	}
};

struct MRecordFile {
	int len;
	int allocated;
	MRecordRecType* Record;
	MRecordFile() :len(0), allocated(100) {
		Record = new MRecordRecType[allocated];
	}
	~MRecordFile()
	{
		if (Record != NULL)
		{
			delete[]Record;
		}
	}
	void serilize(ofstream& out)
	{
		out << len;
		for (int i = 0; i < len; i++)
		{
			Record[i].serilize(out);
		}
	}
	void deserilize(ifstream& in)
	{
		in >> len;
		allocated = len * 2;
		if (Record != NULL)
		{
			delete Record;
			Record = new MRecordRecType[allocated];
		}
		for (int i = 0; i < len; i++)
		{
			Record[i].deserilize(in);
		}
	}

	void append(const MRecordRecType& o)
	{
		if (len == allocated)
		{
			allocated *= 2;
			MRecordRecType* nb = new MRecordRecType[allocated];
			memcpy(nb, Record, sizeof(nb[0]) * len);
			delete[]Record;
			Record = nb;
		}
		Record[len] = o;
		len++;
	}

};

//定义一个推荐影片存储文件
struct RecommendRecType
{
	int num;
	float recorank;
	void serilize(ofstream& out)
	{
		out.write((const char*)this, sizeof(*this));
	}
	void deserilize(ifstream& in)
	{
		in.read((char*)this, sizeof(*this));
	}
	RecommendRecType& operator=(const RecommendRecType& other)
	{
		memcpy(this, &other, sizeof(*this));
		return *this;
	}
};
struct RecommendFile
{
	int len;
	int allocated;
	RecommendRecType* RecommendDbase;
	RecommendFile() :len(0), allocated(100) {
		RecommendDbase = new RecommendRecType[allocated];
	}
	~RecommendFile() {
		if (RecommendDbase != NULL)
		{
			delete[] RecommendDbase;
		}
		len = 0;
		RecommendDbase = NULL;
	}
	void append(const RecommendRecType& n)
	{
		if (allocated == len)
		{
			allocated = allocated * 2;
			RecommendRecType* nb = new RecommendRecType[allocated];
			memcpy(nb, RecommendDbase, len * sizeof(RecommendRecType));
			delete[] RecommendDbase;
			RecommendDbase = nb;
		}
		RecommendDbase[len] = n;
		len++;
	}
};


#endif