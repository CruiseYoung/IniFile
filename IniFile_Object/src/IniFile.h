#pragma once
#include <string>
#pragma warning(disable:4786)
#include <vector>
#include <iostream>
#include <algorithm>
#include <functional>

#include <sstream>
#include <sys/stat.h>
using std::vector;
using std::string;
using std::stringstream;

//using namespace std;

//template<class out_type, class in_value>
//out_type convert(const in_value& t);

template<class out_type, class in_value>
out_type convert(const in_value& t)
{
    stringstream stream;
    stream << t;

    out_type result;
    stream >> result;

    stream.clear();
    return result;
}


class CIniFile
{
public:
    struct Record
    {
        string Comments;
        char Commented;
        string Section;
        string Key;
        string Value;
    };

    enum CommentChar
    {
        Pound = '#',
        SemiColon = ';'
    };

    CIniFile(void);
    CIniFile(string FileName);
    virtual ~CIniFile(void);

    bool FileExist();
    bool Create();

    vector<string> GetSectionNames();
    vector<Record> GetSection(string SectionName);
    vector<Record> GetRecord(string KeyName, string SectionName);
    string GetValue(string KeyName, string SectionName);
    string Content();

    bool SectionExists(string SectionName);
    bool RecordExists(string KeyName, string SectionName);

    bool AddSection(string SectionName);
    bool SetValue(string KeyName, string Value, string SectionName);

    bool DeleteSection(string SectionName);
    bool DeleteRecord(string KeyName, string SectionName);

    bool RenameSection(string OldSectionName, string NewSectionName);
    bool Sort(bool Descending);

    bool SetSectionComments(string Comments, string SectionName);
    bool SetRecordComments(string Comments, string KeyName, string SectionName);

    bool CommentSection(char CommentChar, string SectionName);
    bool CommentRecord(CommentChar cc, string KeyName, string SectionName);

    bool UnCommentSection(string SectionName);
    bool UnCommentRecord(string KeyName, string SectionName);

private:
	vector<Record> GetSections();
	bool Load();	
	bool Save();

	struct RecordSectionIs : std::unary_function<Record, bool>
	{
		std::string section_;

		RecordSectionIs(const std::string& section): section_(section){}

		bool operator()( const Record& rec ) const
		{
			return rec.Section == section_;
		}
	};

	struct RecordSectionKeyIs : std::unary_function<Record, bool>
	{
		std::string section_;
		std::string key_;

		RecordSectionKeyIs(const std::string& section, const std::string& key): section_(section),key_(key){}

		bool operator()( const Record& rec ) const
		{
			return ((rec.Section == section_)&&(rec.Key == key_));
		}
	};

	struct AscendingSectionSort
	{
		bool operator()(Record& Start, Record& End)
		{
			return Start.Section < End.Section;
		}
	};

	struct DescendingSectionSort
	{
		bool operator()(Record& Start, Record& End)
		{
			return Start.Section > End.Section;
		}
	};

	struct AscendingRecordSort
	{
		bool operator()(Record& Start, Record& End)
		{
			return Start.Key < End.Key;
		}
	};

	struct DescendingRecordSort
	{
		bool operator()(Record& Start, Record& End)
		{
			return Start.Key > End.Key;
		}
	};

private:
    string m_FileName;
    //bool m_Descending;
    vector<Record> m_content;
public:
    void SetFileName(string FileName);
};