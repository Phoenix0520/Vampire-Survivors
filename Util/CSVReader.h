#pragma once

using namespace std;

typedef unsigned int UINT;

class ExcelData
{
public:
	ExcelData() {}
	~ExcelData() { m_cvColumns.clear(); }

public:
	vector<string> m_cvColumns;



};

class CSVReader
{
public:
	CSVReader() {}
	~CSVReader() { Clear(); }

public:
	void Clear();

	void OpenFile(string fileName);
	void Parsing(char* buf);
	//void GetData(UINT row, UINT col, int& ret);
	//void GetData(UINT row, UINT col, UINT& ret);
	bool GetData(UINT row, UINT col, float& ret);
	bool GetData(UINT row, UINT col, string& ret);
	bool GetData(UINT row, UINT col, wstring& ret);
	

public:
	vector<ExcelData*> m_cvRows;

};