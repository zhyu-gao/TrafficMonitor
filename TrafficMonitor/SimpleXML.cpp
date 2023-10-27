#include "stdafx.h"
#include "SimpleXML.h"


CSimpleXML::CSimpleXML(const wstring & xml_path)
{
	ifstream file_stream{ xml_path };
	if (file_stream.fail())
	{
		return;
	}
	//��ȡ�ļ�����
	string xml_str;
	while (!file_stream.eof())
	{
		xml_str.push_back(file_stream.get());
	}
	xml_str.pop_back();
	if (!xml_str.empty() && xml_str.back() != L'\n')		//ȷ���ļ�ĩβ�лس���
		xml_str.push_back(L'\n');
	//�ж��ļ��Ƿ���utf8����
	bool is_utf8;
	if (xml_str.size() >= 3 && xml_str[0] == -17 && xml_str[1] == -69 && xml_str[2] == -65)
	{
		//�����UTF8��BOM����ɾ��BOM
		is_utf8 = true;
		xml_str = xml_str.substr(3);
	}
	else
	{
		is_utf8 = false;
	}
	//ת����Unicode
	m_xml_content = CCommon::StrToUnicode(xml_str.c_str(), is_utf8);
}

CSimpleXML::CSimpleXML()
{
}


CSimpleXML::~CSimpleXML()
{
}

wstring CSimpleXML::GetNode(const wchar_t * node, const wchar_t * parent) const
{
	wstring node_content = _GetNode(parent, m_xml_content);
	return _GetNode(node, node_content);
}

wstring CSimpleXML::GetNode(const wchar_t * node) const
{
	return _GetNode(node, m_xml_content);
}

wstring CSimpleXML::_GetNode(const wchar_t * node, const wstring & content)
{
	wstring result;
	wstring node_start{ L'<' };
	wstring node_end{ L'<' };
	node_start += node;
	node_start += L'>';
	node_end += L'/';
	node_end += node;
	node_end += L'>';

	size_t index_start, index_end;
	index_start = content.find(node_start);
	index_end = content.find(node_end);
	if (index_start == wstring::npos || index_end == wstring::npos)
		return wstring();

	result = content.substr(index_start + node_start.size(), index_end - index_start - node_start.size());
	return result;
}
