#pragma once
#include <string>
#include <list>
#include <sstream>
#pragma warning(disable:4786)

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
    struct Record
    {
        std::string Comments;
        char Commented;
        std::string Section;
        std::string Key;
        std::string Value;
    };

    enum CommentChar
    {
        Pound = '#',
        SemiColon = ';'
    };

    static bool FileExist(std::string FileName);
    static bool Create(std::string FileName);

    static std::list<std::string> GetSectionNames(std::string FileName);
    static std::list<Record> GetSection(std::string SectionName, std::string FileName);
    static std::list<Record> GetRecord(std::string KeyName, std::string SectionName, std::string FileName);
    static std::string GetValue(std::string KeyName, std::string SectionName, std::string FileName);
    static std::string Content(std::string FileName);

    static bool SectionExists(std::string SectionName, std::string FileName);
    static bool RecordExists(std::string KeyName, std::string SectionName, std::string FileName);

    static bool AddSection(std::string SectionName, std::string FileName);
    static bool SetValue(std::string KeyName, std::string Value, std::string SectionName, std::string FileName);

    static bool DeleteSection(std::string SectionName, std::string FileName);
    static bool DeleteRecord(std::string KeyName, std::string SectionName, std::string FileName);

    static bool RenameSection(std::string OldSectionName, std::string NewSectionName, std::string FileName);
    static bool Sort(std::string FileName, bool Descending);

    static bool SetSectionComments(std::string Comments, std::string SectionName, std::string FileName);
    static bool SetRecordComments(std::string Comments, std::string KeyName, std::string SectionName, std::string FileName);

    static bool CommentSection(char CommentChar, std::string SectionName, std::string FileName);
    static bool CommentRecord(CommentChar cc, std::string KeyName, std::string SectionName, std::string FileName);

    static bool UnCommentSection(std::string SectionName, std::string FileName);
    static bool UnCommentRecord(std::string KeyName, std::string SectionName, std::string FileName);

private:
	static std::list<Record> GetSections(std::string FileName);
	static bool Load(std::string FileName, std::list<Record>& content);	
	static bool Save(std::string FileName, std::list<Record>& content);

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
		bool operator()(Record& Start, Record& End) const
		{
			return Start.Section < End.Section;
		}
	};

	struct DescendingSectionSort
	{
		bool operator()(Record& Start, Record& End) const
		{
			return Start.Section > End.Section;
		}
	};

	struct AscendingRecordSort
	{
		bool operator()(Record& Start, Record& End) const
		{
			return Start.Key < End.Key;
		}
	};

	struct DescendingRecordSort
	{
		bool operator()(Record& Start, Record& End) const
		{
			return Start.Key > End.Key;
		}
	};
};