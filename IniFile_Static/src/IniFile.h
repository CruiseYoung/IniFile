#pragma once
#include <string>
#include <vector>
#include <sstream>
#pragma warning(disable:4786)

template <typename Target_t=std::string, typename Source_t = std::string>
static Target_t convert(const Source_t& arg)
{
    std::stringstream interpreter;
    Target_t result;

    if (   !(interpreter << arg)                // 将arg写入流
        || !(interpreter >> result)             // 从流读取结果
        || !(interpreter >> std::ws).eof()      // 流中还有剩余内容？
        )
    {
        // 失败
        //throw std::runtime_error("convert<> failed.");
    }
    interpreter.clear();

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

    static bool FileExist(string const& FileName);
    static bool Create(string const& FileName);

    static std::vector<string> GetSectionNames(string const& FileName);
    static std::vector<Record> GetSection(string const& SectionName, string const& FileName);
    static std::vector<Record> GetRecord(string const& KeyName, string const& SectionName, string const& FileName);
    static string GetValue(string const& KeyName, string const& SectionName, string const& FileName, string const& DefValue = "");
    static string Content(string const& FileName);

    static bool SectionExists(string const& SectionName, string const& FileName);
    static bool RecordExists(string const& KeyName, string const& SectionName, string const& FileName);

    static bool AddSection(string const& SectionName, string const& FileName);
    static bool SetValue(string const& KeyName, string const& Value, string const& SectionName, string const& FileName);

    static bool DeleteSection(string const& SectionName, string const& FileName);
    static bool DeleteRecord(string const& KeyName, string const& SectionName, string const& FileName);

    static bool RenameSection(string const& OldSectionName, string const& NewSectionName, string const& FileName);
    static bool Sort(string const& FileName, bool Descending);

    static bool SetSectionComments(string const& Comments, string const& SectionName, string const& FileName);
    static bool SetRecordComments(string const& Comments, string const& KeyName, string const& SectionName, string const& FileName);

    static bool CommentSection(char CommentChar, string const& SectionName, string const& FileName);
    static bool CommentRecord(CommentChar cc, string const& KeyName, string const& SectionName, string const& FileName);

    static bool UnCommentSection(string const& SectionName, string const& FileName);
    static bool UnCommentRecord(string const& KeyName, string const& SectionName, string const& FileName);

private:
    static std::vector<Record> GetSections(string const& FileName);
    static bool Load(string const& FileName, std::vector<Record>& content);
    static bool Save(string const& FileName, std::vector<Record>& content);

    struct RecordSectionIs : std::unary_function<Record, bool>
    {
        string section_;

        RecordSectionIs(string const& section): section_(section)
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

        RecordSectionKeyIs(string const& section, string const& key): section_(section), key_(key)
        {
        }

        bool operator()(const Record& rec) const
        {
            return ((rec.Section == section_) && (rec.Key == key_));
        }
    };

    struct AscendingSectionSort
    {
        bool operator()(const Record& Start, const Record& End) const
        {
            return Start.Section < End.Section;
        }
    };

    struct DescendingSectionSort
    {
        bool operator()(const Record& Start, const Record& End) const
        {
            return Start.Section > End.Section;
        }
    };

    struct AscendingRecordSort
    {
        bool operator()(const Record& Start, const Record& End) const
        {
            return Start.Key < End.Key;
        }
    };

    struct DescendingRecordSort
    {
        bool operator()(const Record& Start, const Record& End) const
        {
            return Start.Key > End.Key;
        }
    };
};
