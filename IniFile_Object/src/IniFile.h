#pragma once
#include <string>
#pragma warning(disable:4786)
#include <vector>
#include <iostream>
#include <algorithm>
#include <functional>

#include <sstream>
#include <sys/stat.h>

// Suggest using the standard library of STD: : to_string and STD: : sto... A series of function 
template<class out_type, class in_value>
static out_type convert(const in_value& t)
{
    out_type result;

    try
    {
		std::stringstream stream;
        stream << t;
        stream >> result;
        stream.clear();
    }
    catch (...) {}

    return result;
}


class CIniFile
{
public:
	template<typename T> using vector = std::vector < T > ;
	using string = std::string;

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
    CIniFile(const string& FileName);
    virtual ~CIniFile(void);

public:
    void SetFileName(const string& FileName);

    bool FileExist();
    bool Create();

    vector<string> GetSectionNames();
    vector<Record> GetSection(const string& SectionName);
    vector<Record> GetRecord(const string& KeyName, const string& SectionName);
    string GetValue(const string& KeyName, const string& SectionName);
    string Content();

    bool SectionExists(const string& SectionName);
    bool RecordExists(const string& KeyName, const string& SectionName);

    bool AddSection(const string& SectionName, bool Saving = true);
    bool SetValue(const string& KeyName, const string& Value, const string& SectionName, bool Saving = true);

    bool DeleteSection(const string& SectionName, bool Saving = true);
    bool DeleteRecord(const string& KeyName, const string& SectionName, bool Saving = true);

    bool RenameSection(const string& OldSectionName, const string& NewSectionName, bool Saving = true);
    bool Sort(bool Descending, bool Saving = true);

    bool SetSectionComments(const string& Comments, const string& SectionName, bool Saving = true);
    bool SetRecordComments(const string& Comments, const string& KeyName, const string& SectionName, bool Saving = true);

    bool CommentSection(char CommentChar, const string& SectionName, bool Saving = true);
    bool CommentRecord(CommentChar cc, const string& KeyName, const string& SectionName, bool Saving = true);

    bool UnCommentSection(const string& SectionName, bool Saving = true);
    bool UnCommentRecord(const string& KeyName, const string& SectionName, bool Saving = true);

	bool Save();
private:
	vector<Record> GetSections();
	bool Load();	

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
    bool m_Loaded;
    vector<Record> m_content;
};