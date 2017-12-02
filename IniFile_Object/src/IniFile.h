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

    IniFile(void);
    IniFile(string const& FileName);

public:
    void SetFileName(string const& FileName);

    bool FileExist();
    bool Create();

    std::vector<string> GetSectionNames();
    std::vector<Record> GetSection(string const& SectionName);
    std::vector<Record> GetRecord(string const& KeyName, string const& SectionName);
    string GetValue(string const& KeyName, string const& SectionName, string const& DefValue = "");
    string Content();

    bool SectionExists(string const& SectionName);
    bool RecordExists(string const& KeyName, string const& SectionName);

    bool AddSection(string const& SectionName, bool Saving = true);
    bool SetValue(string const& KeyName, string const& Value, string const& SectionName, bool Saving = true);

    bool DeleteSection(string const& SectionName, bool Saving = true);
    bool DeleteRecord(string const& KeyName, string const& SectionName, bool Saving = true);

    bool RenameSection(string const& OldSectionName, string const& NewSectionName, bool Saving = true);
    bool Sort(bool Descending, bool Saving = true);

    bool SetSectionComments(string const& Comments, string const& SectionName, bool Saving = true);
    bool SetRecordComments(string const& Comments, string const& KeyName, string const& SectionName, bool Saving = true);

    bool CommentSection(char CommentChar, string const& SectionName, bool Saving = true);
    bool CommentRecord(CommentChar cc, string const& KeyName, string const& SectionName, bool Saving = true);

    bool UnCommentSection(string const& SectionName, bool Saving = true);
    bool UnCommentRecord(string const& KeyName, string const& SectionName, bool Saving = true);

    bool Save();

private:
    std::vector<Record> GetSections();
    bool Load();

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

private:
    string m_FileName;
    bool m_Loaded;
    std::vector<Record> m_content;
};
