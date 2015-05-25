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
    virtual ~CIniFile(void);

    static bool FileExist(const string& FileName);
    static bool Create(const string& FileName);

    static vector<string> GetSectionNames(const string& FileName);
    static vector<Record> GetSection(const string& SectionName, const string& FileName);
    static vector<Record> GetRecord(const string& KeyName, const string& SectionName, const string& FileName);
    static string GetValue(const string& KeyName, const string& SectionName, const string& FileName);
    static string Content(const string& FileName);

    static bool SectionExists(const string& SectionName, const string& FileName);
    static bool RecordExists(const string& KeyName, const string& SectionName, const string& FileName);

    static bool AddSection(const string& SectionName, const string& FileName);
    static bool SetValue(const string& KeyName, const string& Value, const string& SectionName, const string& FileName);

    static bool DeleteSection(const string& SectionName, const string& FileName);
    static bool DeleteRecord(const string& KeyName, const string& SectionName, const string& FileName);

    static bool RenameSection(const string& OldSectionName, const string& NewSectionName, const string& FileName);
    static bool Sort(const string& FileName, bool Descending);

    static bool SetSectionComments(const string& Comments, const string& SectionName, const string& FileName);
    static bool SetRecordComments(const string& Comments, const string& KeyName, const string& SectionName, const string& FileName);

    static bool CommentSection(char CommentChar, const string& SectionName, const string& FileName);
    static bool CommentRecord(CommentChar cc, const string& KeyName, const string& SectionName, const string& FileName);

    static bool UnCommentSection(const string& SectionName, const string& FileName);
    static bool UnCommentRecord(const string& KeyName, const string& SectionName, const string& FileName);

private:
	static vector<Record> GetSections(const string& FileName);
	static bool Load(const string& FileName, vector<Record>& content);	
	static bool Save(const string& FileName, vector<Record>& content);

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
};