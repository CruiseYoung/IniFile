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

    CIniFile(void);
    CIniFile(std::string FileName);

public:
    void SetFileName(std::string FileName);

    bool FileExist();
    bool Create();

    std::list<std::string> GetSectionNames();
    std::list<Record> GetSection(std::string SectionName);
    std::list<Record> GetRecord(std::string KeyName, std::string SectionName);
    std::string GetValue(std::string KeyName, std::string SectionName);
    std::string Content();

    bool SectionExists(std::string SectionName);
    bool RecordExists(std::string KeyName, std::string SectionName);

    bool AddSection(std::string SectionName, bool Saving = true);
    bool SetValue(std::string KeyName, std::string Value, std::string SectionName, bool Saving = true);

    bool DeleteSection(std::string SectionName, bool Saving = true);
    bool DeleteRecord(std::string KeyName, std::string SectionName, bool Saving = true);

    bool RenameSection(std::string OldSectionName, std::string NewSectionName, bool Saving = true);
    bool Sort(bool Descending, bool Saving = true);

    bool SetSectionComments(std::string Comments, std::string SectionName, bool Saving = true);
    bool SetRecordComments(std::string Comments, std::string KeyName, std::string SectionName, bool Saving = true);

    bool CommentSection(char CommentChar, std::string SectionName, bool Saving = true);
    bool CommentRecord(CommentChar cc, std::string KeyName, std::string SectionName, bool Saving = true);

    bool UnCommentSection(std::string SectionName, bool Saving = true);
    bool UnCommentRecord(std::string KeyName, std::string SectionName, bool Saving = true);

	bool Save();

private:
	std::list<Record> GetSections();
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

private:
    std::string m_FileName;
    bool m_Loaded;
    std::list<Record> m_content;
};