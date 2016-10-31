#pragma once
#include <string>
#include <list>
#include <sstream>
#pragma warning(disable:4786)

template <class out_type, class in_value>
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
	catch (...)
	{
	}

	return result;
}


class CIniFile
{
public:
	typedef std::string string;

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

public:
	void SetFileName(const string& FileName);

	bool FileExist();
	bool Create();

	std::list<string> GetSectionNames();
	std::list<Record> GetSection(const string& SectionName);
	std::list<Record> GetRecord(const string& KeyName, const string& SectionName);
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
	std::list<Record> GetSections();
	bool Load();

	struct RecordSectionIs : std::unary_function<Record, bool>
	{
		string section_;

		RecordSectionIs(const string& section): section_(section)
		{
		}

		bool operator()(const Record& rec) const
		{
			return rec.Section == section_;
		}
	};

	struct RecordSectionKeyIs : std::unary_function<Record, bool>
	{
		string section_;
		string key_;

		RecordSectionKeyIs(const string& section, const string& key): section_(section), key_(key)
		{
		}

		bool operator()(const Record& rec) const
		{
			return ((rec.Section == section_) && (rec.Key == key_));
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
	string m_FileName;
	bool m_Loaded;
	std::list<Record> m_content;
};
