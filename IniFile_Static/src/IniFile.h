#pragma once
#include <string>
#include <vector>
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
        if ((stream >> std::ws).eof())
        {
            // 成功
        }
        stream.clear();
    }
    catch (...)
    {
    }

    return result;
}


class IniFile
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

    static bool FileExist(const string& FileName);
    static bool Create(const string& FileName);

    static std::vector<string> GetSectionNames(const string& FileName);
    static std::vector<Record> GetSection(const string& SectionName, const string& FileName);
    static std::vector<Record> GetRecord(const string& KeyName, const string& SectionName, const string& FileName);
    static string GetValue(const string& KeyName, const string& SectionName, const string& FileName, const string& DefValue = "");
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
    static std::vector<Record> GetSections(const string& FileName);
    static bool Load(const string& FileName, std::vector<Record>& content);
    static bool Save(const string& FileName, std::vector<Record>& content);

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
};
