#include "framework.h"
#include "CSVReader.h"
#include "Convert.h"

#define D_START				0
#define D_COMMA				1
#define D_STRING_START		2
#define D_STRING_CONTINUE	3
#define D_STRING_END		4

void CSVReader::Clear()
{
	for (UINT i = 0; i < m_cvRows.size(); i++)
	{
		m_cvRows[i]->m_cvColumns.clear();
		delete m_cvRows[i];
	}

	m_cvRows.erase(m_cvRows.begin(), m_cvRows.end());
}

void CSVReader::OpenFile(string strFileName)
{
	FILE  *fp;

	Clear();

	fopen_s(&fp, strFileName.c_str(), "r");

	if (fp == NULL)
		return;

	char  buf[10000];

	while (1)
	{
		if (!fgets(buf, 10000, fp))
			break;

		buf[strlen(buf) - 1] = '\0';
		Parsing(buf);
	}

	fclose(fp);
}

void CSVReader::Parsing(char * buf)
{
	int      nStatus = D_START;
	string   data;

	for (unsigned int i = 0; i < strlen(buf); i++)
	{
		char C = buf[i];
		char NC;

		if (i < (strlen(buf) - 1))
			NC = buf[i + 1];
		else
			NC = C;

		switch (C)
		{
		case ',':
			switch (nStatus)
			{
			case D_START:
			{
				ExcelData *pExcelData = new ExcelData();
				pExcelData->m_cvColumns.push_back(data);
				m_cvRows.push_back(pExcelData);
				nStatus = D_COMMA;
				data.clear();
			}
			break;
			case D_COMMA:
			{
				ExcelData *pExcelData = m_cvRows[m_cvRows.size() - 1];
				pExcelData->m_cvColumns.push_back(data);
				data.clear();
			}
			break;
			}
			break;
		case '"':
		{
			switch (nStatus)
			{
			case D_START:
			case D_COMMA:
				nStatus = D_STRING_START;
				break;
			case D_STRING_START:
				if (NC == '"')
					nStatus = D_STRING_CONTINUE;
				else
					nStatus = D_STRING_END;

				break;
			case D_STRING_CONTINUE:
				data += C;
				nStatus = D_STRING_START;
				break;
			case D_STRING_END:
				break;
			}
		}
		break;

		default:
			data += C;
			break;
		}
	}

	if (data.length() > 0)
	{
		ExcelData *pExcelData = m_cvRows[m_cvRows.size() - 1];
		pExcelData->m_cvColumns.push_back(data);
	}
}

bool CSVReader::GetData(UINT row, UINT col, float & ret)
{
	for (UINT i = 0; i < m_cvRows.size(); i++)
	{
		if (i != row) continue;

		ExcelData *pExcelData = m_cvRows.at(i);
		vector<string> colData = pExcelData->m_cvColumns;

		for (UINT j = 0; j < colData.size(); j++)
		{
			if (j != col) continue;

			UINT colSize = colData[j].size();

			string integerPart = "";
			UINT integerPartSize = 0;

			string decimalPart = "";

			for (integerPartSize = 0; integerPartSize < colSize; integerPartSize++)
			{
				char val = colData[j][integerPartSize];

				if (val == '.')
				{
					decimalPart = strchr(colData[j].c_str(), '.');
					break;
				}
				
				integerPart.push_back(val);
			}

			float sum = 0.0f;

			for (UINT k = 0; k < colSize; k++)
			{
				const char* cstr = colData[j].c_str();

				int val = (int)cstr[k] - 48;

				if (val < 0)
					continue;

				if (k <= integerPartSize)				
					sum += val * (float)pow(10, integerPartSize - k - 1);				
				else if (k - integerPartSize < 7)				
					sum += val * (float)(1.0 / pow(10, k - integerPartSize));
				else
					break;
			}

			ret = sum;
			return true;
		}
	}

	ret = 3.1415926f;
	return false;
}

bool CSVReader::GetData(UINT row, UINT col, string& ret)
{
	for (UINT i = 0; i < m_cvRows.size(); i++)
	{
		if (i != row) continue;

		ExcelData *pExcelData = m_cvRows.at(i);

		for (UINT j = 0; j < pExcelData->m_cvColumns.size(); j++)
		{
			if (j != col) continue;
			ret = pExcelData->m_cvColumns[j].c_str();
			return true;
		}
	}

	ret = "Data Missing";
	return false;
}

bool CSVReader::GetData(UINT row, UINT col, wstring& ret)
{
	for (UINT i = 0; i < m_cvRows.size(); i++)
	{
		if (i != row) continue;

		ExcelData *pExcelData = m_cvRows.at(i);

		for (UINT j = 0; j < pExcelData->m_cvColumns.size(); j++)
		{
			if (j != col) continue;

			string  test = pExcelData->m_cvColumns[j].c_str();

			wstring unicode = L"";  
			Convert::Ansi_to_UniCode(unicode, (char*)test.c_str(), test.length());

			_wsetlocale(LC_ALL, L"Korean");
			ret = unicode;

			return true;
		}
	}

	ret = L"Data Missing";
	return false;
}