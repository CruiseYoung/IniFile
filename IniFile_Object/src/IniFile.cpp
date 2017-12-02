#include "inifile.h"
#include <fstream>
#include <algorithm>
#include <iterator>
#include <cctype>
#include <cassert>
#include <sys/stat.h>

using std::vector;
using std::string;
using std::ifstream;
using std::ofstream;

// replace all old_value with new_value in 'str'
static string& replace_all(string& str, string const& old_value, string const& new_value)
{
    //assert(!str.empty());
    assert(!old_value.empty());

    if (old_value == new_value/* || new_value == ""*/)
    {
        return str;
    }

    for (string::size_type pos = 0, last_pos = 0;
        (pos = str.find(old_value, last_pos)) != string::npos;
        last_pos = pos + new_value.length())
    {
        str.replace(pos, old_value.length(), new_value);
    }

    return str;
}

// A function to trim whitespace from both sides of a given string
static void Trim(string& str, string const& ChrsToTrim = " \t\n\r", int TrimDir = 0)
{
   // assert(!str.empty());

    size_t startIndex = str.find_first_not_of(ChrsToTrim);
    if (startIndex == string::npos)
    {
        str.clear();//str.erase();
        return;
    }

    if (TrimDir < 2)
    {
        str.erase(0, startIndex);
    }

    if (TrimDir != 1)
    {
        str.erase(str.find_last_not_of(ChrsToTrim) + 1);
    }
}

inline void TrimRight(string& str, string const& ChrsToTrim = " \t\n\r")
{
    Trim(str, ChrsToTrim, 2);
}

inline void TrimLeft(string& str, string const& ChrsToTrim = " \t\n\r")
{
    Trim(str, ChrsToTrim, 1);
}

// A function to transform a string to uppercase if neccessary
inline void UCase(string& str, bool ucase)
{
    //assert(!str.empty());
    std::transform(str.begin(), str.end(), str.begin(), ucase ? toupper : tolower);
}

static std::vector<std::string> Split(string const& str, string const& delims)
{
    //assert(!str.empty());
    assert(!delims.empty());

    vector<string> result;

    for (string::size_type beg_idx = 0, end_idx = string::npos;
        (beg_idx = str.find_first_not_of(delims, beg_idx)) != string::npos;
        beg_idx = end_idx)
    {
        end_idx = str.find_first_of(delims, beg_idx);
        if (end_idx == string::npos)
        {
            end_idx = str.length();
        }

        string sub_str = str.substr(beg_idx, end_idx - beg_idx);
        result.push_back(sub_str);
    }

    return result;
}

static std::vector<std::string> SplitHasEmpty(string const& str, string const& delims)
{
    //assert(!str.empty());
    assert(!delims.empty());

    vector<string> result;

    for (string::size_type beg_idx = 0, end_idx = 0;
        end_idx != string::npos; beg_idx = end_idx + 1)
    {
        end_idx = str.find_first_of(delims, beg_idx);
        if (end_idx == string::npos)
        {
            end_idx = str.length();
        }

        string sub_str = str.substr(beg_idx, end_idx - beg_idx);
        result.push_back(sub_str);
    }

    return result;
}

static bool IsValidKeyValue(string const& s)
{
    if (s.empty())
    {
        return false;
    }

    for (unsigned int i = 0; i < s.size(); ++i)
    {
        if (/*ispunct(s[i])*/s[i] > 0 && !isalnum(s[i]) && s[i] != '_')
        {
            return false;
        }
    }

    return true;
}

static bool IsSection(string const& s)
{
    if (s.empty() || s[0] != '[' || s[s.length() - 1] != ']')
    {
        return false;
    }

    string s_str = s.substr(1, s.length() - 2);
    Trim(s_str);

    return IsValidKeyValue(s_str);
}

static bool IsRecord(string const& s)
{
    if (s.empty())
    {
        return false;
    }

    string::size_type pos = s.find('=');
    if (pos == string::npos || pos == 0)
    {
        return false;
    }

    return true;

#if 0
    string s_key = s.substr(0, pos);
    TrimRight(s_key);

    if (!IsValidKeyValue(s_key))
    {
        false;
    }

    string s_value = s.substr(pos + 1);
    TrimLeft(s_value);

    return IsValidKeyValue(s_value);
#endif
}

IniFile::IniFile(void)                                                      // Default constructor
    : m_Loaded(false)
{
}

IniFile::IniFile(string const& FileName)                                    // constructor
    : m_FileName(FileName)
    , m_Loaded(false)
{
    assert(!FileName.empty());
}

void IniFile::SetFileName(string const& FileName)
{
    assert(!FileName.empty());
    m_FileName = FileName;
}

bool IniFile::Load()
{
    if (m_Loaded)
    {
        return true;
    }

    string const& FileName = m_FileName;
    ifstream inFile(FileName.c_str());                                      // Create an input filestream
    if (!inFile)
    {
        return false;                                                       // If the input file doesn't open, then return
    }

    vector<Record>& content = m_content;
    content.clear();                                                        // Clear the content vector
    string comments;                                                        // A string to store comments in
    string CurrentSection;                                                  // Holds the current section name

    string s;                                                               // Holds the current line from the ini file
    while (!getline(inFile, s).eof())                                       // Read until the end of the file
    {
        Trim(s);                                                            // Trim whitespace from the ends
        if (s.empty())                                                      // Make sure its not a blank line
        {
            continue;
        }

        Record r;                                                           // Define a new record
        bool isComment = false;
        if ((s[0] == '#') || (s[0] == ';'))                                 // Is this a commented line?
        {
            string sub = s.substr(1);
            Trim(sub);
            if (!(IsSection(sub) || IsRecord(sub)))

            //if ((s.find('[') == string::npos)                             // If there is no [ or =
            //    && (s.find('=') == string::npos))                         // Then it's a comment
            {
                isComment = true;
                comments += (s + '\n');                                     // Add the comment to the current comments string
            }
            else
            {
                r.Commented = s[0];                                         // Save the comment character
                s.erase(s.begin());                                         // Remove the comment for further processing
                Trim(s);
            }// Remove any more whitespace
        }
        else
        {
            r.Commented = ' ';                                              // else mark it as not being a comment
        }

        if (!isComment)
        {
            if(s.find('[') != string::npos)                                 // Is this line a section?
            {
                s.erase(s.begin());                                         // Erase the leading bracket
                s.erase(s.find(']'));                                       // Erase the trailing bracket
                r.Comments = comments;                                      // Add the comments string (if any)
                comments = "";                                              // Clear the comments for re-use
                Trim(s);
                r.Section = s;                                              // Set the Section value
                r.Key = "";                                                 // Set the Key value
                r.Value = "";                                               // Set the Value value
                CurrentSection = s;
            }

            if (s.find('=') != string::npos)                                // Is this line a Key/Value?
            {
                r.Comments = comments;                                      // Add the comments string (if any)
                comments = "";                                              // Clear the comments for re-use
                r.Section = CurrentSection;                                 // Set the section to the current Section
                r.Key = s.substr(0, s.find('='));                           // Set the Key value to everything before the = sign
                Trim(r.Key);
                r.Value = s.substr(s.find('=') + 1);                        // Set the Value to everything after the = sign
                Trim(r.Value);
            }
        }

        if (comments == "")                                                 // Don't add a record yet if its a comment line
        {
            content.push_back(r);                                           // Add the record to content
        }
    }

    //inFile.close();                                                         // Close the file
    m_Loaded = true;
    return true;
}

bool IniFile::Save()
{
    string const& FileName = m_FileName;
    ofstream outFile(FileName.c_str());                                     // Create an output filestream
    if (!outFile)
    {
        return false;                                                       // If the output file doesn't open, then return
    }

    vector<Record>& content = m_content;
    for (vector<Record>::iterator iter = content.begin();
        iter != content.end(); ++iter)                                      // Loop through each vector
    {
        outFile << iter->Comments;                                          // Write out the comments
        if (iter->Commented != ' ')
        {
            outFile << iter->Commented;
        }

        if (iter->Key == "")                                                // Is this a section?
        {
            outFile<< "[" << iter->Section << "]" << std::endl;             // Then format the section
        }
        else
        {
            outFile << iter->Key << "=" << iter->Value << std::endl;        // Else format a key/value
        }
    }

    //outFile.close();                                                        // Close the file
    return true;
}

bool IniFile::FileExist()
{
    string const& FileName = m_FileName;

    struct stat stFileInfo;
    if (stat(FileName.c_str(), &stFileInfo) != 0)
    {
        return false;
    }

    return true;
}

bool IniFile::Create()
{
    if (FileExist())
    {
        return true;
    }

    return Save();                                                          // Save
}

vector<string> IniFile::GetSectionNames()
{
    vector<string> data;                                                    // Holds the return data
    vector<Record>& content = m_content;                                    // Holds the current record

    if (!Load())                                                            // Make sure the file is loaded
    {
        return data;                                                        // Return the data
    }

    for (vector<Record>::iterator iter = content.begin();
        iter != content.end(); ++iter)                                      // Loop through the content
    {
        if (iter->Key == "")                                                // If there is no key value, then its a section
        {
            data.push_back(iter->Section);                                  // Add the section to the return data
        }
    }

    return data;                                                            // Return the data
}

vector<IniFile::Record> IniFile::GetSections()
{
    vector<Record> data;                                                    // Holds the return data
    vector<Record>& content = m_content;                                    // Holds the current record

    if (!Load())                                                            // Make sure the file is loaded
    {
        return data;                                                        // Return the data
    }

    for (vector<Record>::iterator iter = content.begin();
        iter != content.end(); ++iter)                                      // Loop through the content
    {
        if (iter->Key == "")                                                // If this is a section
        {
            data.push_back(*iter);                                          // Add the record to the return data
        }
    }

    return data;                                                            // Return the data
}

vector<IniFile::Record> IniFile::GetSection(string const& SectionName)
{
    vector<Record> data;                                                    // Holds the return data
    vector<Record>& content = m_content;                                    // Holds the current record

    if (!Load())                                                            // Make sure the file is loaded
    {
        return data;                                                        // Return the data
    }

    assert(!SectionName.empty());
    for (vector<Record>::iterator iter = content.begin();
        iter != content.end(); ++iter)                                      // Loop through the content
    {
        if ((iter->Section == SectionName)                                  // If this is the section name we want
            && (iter->Key != ""))                                           // but not the section name itself
        {
            data.push_back(*iter);                                          // Add the record to the return data
        }
    }

    return data;                                                            // Return the data
}

vector<IniFile::Record> IniFile::GetRecord(string const& KeyName, string const& SectionName)
{
    vector<Record> data;                                                    // Holds the return data
    vector<Record>& content = m_content;                                    // Holds the current record

    if (!Load())                                                            // Make sure the file is loaded
    {
        return data;                                                        // Return the Record
    }

    assert(!KeyName.empty());
    assert(!SectionName.empty());
    vector<Record>::iterator iter = find_if(
        content.begin(), content.end(),
        IniFile::RecordSectionKeyIs(SectionName, KeyName));                 // Locate the Record
    if (iter == content.end())
    {
        return data;                                                        // The Record was not found
    }
    data.push_back(*iter);                                                  // The Record was found

    return data;                                                            // Return the Record
}

string IniFile::GetValue(string const& KeyName, string const& SectionName, string const& DefValue/* = ""*/)
{
    vector<Record> record = GetRecord(KeyName, SectionName);                // Get the Record
    if (record.empty())                                                     // No value was found
    {
        return DefValue;
    }

    return record.front().Value;                                            // Make sure there is a value to return And return the value
}

string IniFile::Content()
{
    string s;                                                               // Hold our return string
    vector<Record>& content = m_content;                                    // Holds the current record

    if (!Load())                                                            // Make sure the file is loaded
    {
        return s;
    }

    for (vector<Record>::iterator iter = content.begin();
        iter != content.end(); ++iter)                                      // Loop through the content
    {
        if (iter->Comments != "")
        {
            s += iter->Comments;                                            // Add the comments
        }

        if (iter->Commented != ' ')
        {
            s += iter->Commented;                                           // If this is commented, then add it
        }

        if ((iter->Key == ""))                                              // Is this a section?
        {
            s += '[' + iter->Section + ']';                                 // Add the section
        }
        else
        {
            s += iter->Key + '=' + iter->Value;                             // Or the Key value to the return srting
        }

        if (iter != content.end())
        {
            s += '\n';                                                      // If this is not the last line, add a CrLf
        }
    }

    return s;                                                               // Return the contents
}

bool IniFile::SectionExists(string const& SectionName)
{
    vector<Record>& content = m_content;                                    // Holds the current record

    if (!Load())                                                            // Make sure the file is loaded
    {
        return false;                                                       // In the event the file does not load
    }

    assert(!SectionName.empty());
    vector<Record>::iterator iter = find_if(
        content.begin(), content.end(),
        IniFile::RecordSectionIs(SectionName));                             // Locate the Section
    if (iter == content.end())                                              // The Section was not found
    {
        return false;
    }

    return true;                                                            // The Section was found
}

bool IniFile::RecordExists(string const& KeyName, string const& SectionName)
{
    vector<Record>& content = m_content;                                    // Holds the current record

    if (!Load())                                                            // Make sure the file is loaded
    {
        return false;                                                       // In the event the file does not load
    }

    assert(!KeyName.empty());
    assert(!SectionName.empty());
    vector<Record>::iterator iter = find_if(
        content.begin(), content.end(),
        IniFile::RecordSectionKeyIs(SectionName, KeyName));                 // Locate the Section/Key
    if (iter == content.end())                                              // The Section/Key was not found
    {
        return false;
    }

    return true;                                                            // The Section/Key was found
}

bool IniFile::AddSection(string const& SectionName, bool Saving/* = true*/)
{
    if (!Create())
    {
        return false;
    }

    vector<Record>& content = m_content;                                    // Holds the current record
    if (!Load())                                                            // Make sure the file is loaded
    {
        return false;                                                       // The file did not open
    }

    if (SectionExists(SectionName))
    {
        return true;
    }

    Record s = {"", ' ', SectionName, "", ""};                              // Define a new section
    content.push_back(s);                                                   // Add the section

    if (!Saving)
    {
        return true;
    }
    return Save();                                                          // Save
}

bool IniFile::SetValue(string const& KeyName, string const& Value, string const& SectionName, bool Saving/* = true*/)
{
    if (!Create())
    {
        return false;
    }

    vector<Record>& content = m_content;                                    // Holds the current record
    if (!Load())                                                            // Make sure the file is loaded
    {
        return false;                                                       // In the event the file does not load
    }

    if (!SectionExists(SectionName))                                        // If the Section doesn't exist
    {
        Record s = { "", ' ', SectionName, "", "" };                        // Define a new section
        Record r = { "", ' ', SectionName, KeyName, Value };                // Define a new record

        content.push_back(s);                                               // Add the section
        content.push_back(r);                                               // Add the record

        if (!Saving)
        {
            return true;
        }
        return Save();                                                      // Save
    }

    if (!RecordExists(KeyName, SectionName))                                // If the Key doesn't exist
    {
        Record r = { "", ' ', SectionName, KeyName, Value };                // Define a new record

        vector<Record>::reverse_iterator r_iter = find_if(
            content.rbegin(), content.rend(),
            IniFile::RecordSectionIs(SectionName));                         // Locate the Section
        vector<Record>::iterator iter =  r_iter.base();
        content.insert(iter, r);                                            // Add the record

        if (!Saving)
        {
            return true;
        }
        return Save();                                                      // Save
    }

    vector<Record>::iterator iter = find_if(
        content.begin(), content.end(),
        IniFile::RecordSectionKeyIs(SectionName, KeyName));                 // Locate the Record
    iter->Value = Value;                                                    // Insert the correct value

    if (!Saving)
    {
        return true;
    }
    return Save();                                                          // Save
}

bool IniFile::DeleteSection(string const& SectionName, bool Saving/* = true*/)
{
    vector<Record>& content = m_content;                                    // Holds the current record

    if (!Load())                                                            // Make sure the file is loaded
    {
        return false;                                                       // In the event the file does not load
    }

    if (content.empty())
    {
        return true;
    }

    assert(!SectionName.empty());
    vector<Record>::iterator new_end = remove_if(
        content.begin(), content.end(),
        IniFile::RecordSectionIs(SectionName));
    content.erase(new_end, content.end());

    if (!Saving)
    {
        return true;
    }
    return Save();                                                          // Save
}

bool IniFile::DeleteRecord(string const& KeyName, string const& SectionName, bool Saving/* = true*/)
{
    vector<Record>& content = m_content;                                    // Holds the current record

    if (!Load())                                                            // Make sure the file is loaded
    {
        return false;                                                       // In the event the file does not load
    }

    if (content.empty())
    {
        return true;
    }

    assert(!KeyName.empty());
    assert(!SectionName.empty());
    vector<Record>::iterator iter = find_if(
        content.begin(), content.end(),
        IniFile::RecordSectionKeyIs(SectionName, KeyName));                 // Locate the Section/Key
    if (iter == content.end())
    {
        return false;                                                       // The Section/Key was not found
    }
    content.erase(iter);                                                    // Remove the Record

    if (!Saving)
    {
        return true;
    }
    return Save();                                                          // Save
}

bool IniFile::RenameSection(string const& OldSectionName, string const& NewSectionName, bool Saving/* = true*/)
{
    if (SectionExists(NewSectionName))                                      // If the Section doesn't exist
    {
        return false;
    }

    assert(!OldSectionName.empty());
    vector<Record>& content = m_content;                                    // Holds the current record

    for (vector<Record>::iterator iter = content.begin();
        iter != content.end(); ++iter)                                      // Loop through the records
    {
        if (iter->Section == OldSectionName)                                // Is this the OldSectionName?
        {
            iter->Section = NewSectionName;                                 // Now its the NewSectionName
        }
    }

    if (!Saving)
    {
        return true;
    }
    return Save();                                                      // Save
}

bool IniFile::Sort(bool Descending, bool Saving/* = true*/)
{
    vector<IniFile::Record> sections = GetSections();                       // Get a vector of Sections
    if (sections.empty())                                                   // Is there anything to process?
    {
        return false;                                                       // There were no sections
    }

    if (Descending)                                                         // Descending or Ascending?
    {
        std::sort(sections.begin(), sections.end(), DescendingSectionSort());
    }
    else                                                                    // Sort the Sections
    {
        std::sort(sections.begin(), sections.end(), AscendingSectionSort());
    }

    vector<IniFile::Record> content;                                        // Used to hold the sorted content
    for (vector<Record>::iterator iter = sections.begin();
        iter != sections.end(); ++iter)                                     // For each Section
    {
        content.push_back(*iter);                                           // Add the sorted Section to the content

        vector<IniFile::Record> records = GetSection(iter->Section);        // Get a vector of Records for this section
        if (records.empty())                                                // Is there anything to process?
        {
            continue;                                                       // There were no records
        }

        if (Descending)                                                     // Descending or Ascending?
        {
            std::sort(records.begin(), records.end(), DescendingRecordSort());
        }
        else                                                                // Sort the Records
        {
            std::sort(records.begin(), records.end(), AscendingRecordSort());
        }
        content.insert(content.end(), records.begin(), records.end());      // For each Record Add the sorted Record to the content
    }

    m_content.clear();
    std::copy(content.begin(), content.end(), back_inserter(m_content));

    if (!Saving)
    {
        return true;
    }
    return Save();                                                          // Save
}

bool IniFile::SetSectionComments(string const& Comments, string const& SectionName, bool Saving/* = true*/)
{
    vector<Record>& content = m_content;                                    // Holds the current record

    if (!Load())                                                            // Make sure the file is loaded
    {
        return false;                                                       // In the event the file does not load
    }

    assert(!SectionName.empty());
    vector<Record>::iterator iter = find_if(
        content.begin(), content.end(),
        IniFile::RecordSectionKeyIs(SectionName, ""));                      // Is this the Section? And not a record
    if (iter == content.end())                                              // The Section/Key was not found
    {
        return false;
    }

    assert(!Comments.empty());
    string comments = Comments;
    if (Comments.size() >= 2)                                               // Is there a comment?
    {
        if (Comments.substr(Comments.size() - 2) != "\n")                   // Does the string end in a newline?
        {
            comments += "\n";                                               // If not, add one
        }
    }
    iter->Comments = comments;                                              // Set the comments

    if (!Saving)
    {
        return true;
    }
    return Save();                                                          // Save
}

bool IniFile::SetRecordComments(string const& Comments, string const& KeyName, string const& SectionName, bool Saving/* = true*/)
{
    vector<Record>& content = m_content;                                    // Holds the current record

    if (!Load())                                                            // Make sure the file is loaded
    {
        return false;                                                       // In the event the file does not load
    }

    assert(!KeyName.empty());
    assert(!SectionName.empty());
    vector<Record>::iterator iter = find_if(
        content.begin(), content.end(),
        IniFile::RecordSectionKeyIs(SectionName, KeyName));                 // Locate the Section/Key
    if (iter == content.end())                                              // The Section/Key was not found
    {
        return false;
    }

    assert(!Comments.empty());
    string comments = Comments;
    if (Comments.size() >= 2)                                               // Is there a comment?
    {
        if (Comments.substr(Comments.size() - 2) != "\n")                   // Does the string end in a newline?
        {
            comments += "\n";                                               // If not, add one
        }
    }
    iter->Comments = comments;                                              // Set the comments

    if (!Saving)
    {
        return true;
    }
    return Save();                                                          // Save
}

bool IniFile::CommentSection(char CommentChar, string const& SectionName, bool Saving/* = true*/)
{
    vector<Record>& content = m_content;                                    // Holds the current record

    if (!Load())                                                            // Make sure the file is loaded
    {
        return false;                                                       // In the event the file does not load
    }

    if (content.empty())
    {
        return false;
    }

    assert(!SectionName.empty());
    for (vector<Record>::iterator iter = content.begin();
        iter != content.end(); ++iter)
    {
        if (iter->Section == SectionName)                                   // Is this the right section?
        {
            iter->Commented = CommentChar;                                  // Change the comment value
        }
    }

    if (!Saving)
    {
        return true;
    }
    return Save();                                                          // Save
}

bool IniFile::CommentRecord(CommentChar cc, string const& KeyName, string const& SectionName, bool Saving/* = true*/)
{
    vector<Record>& content = m_content;                                    // Holds the current record

    if (!Load())                                                            // Make sure the file is loaded
    {
        return false;                                                       // In the event the file does not load
    }

    if (content.empty())
    {
        return false;
    }

    assert(!KeyName.empty());
    assert(!SectionName.empty());
    vector<Record>::iterator iter = find_if(
        content.begin(), content.end(),
        IniFile::RecordSectionKeyIs(SectionName, KeyName));                 // Locate the Section/Key
    if (iter == content.end())                                              // The Section/Key was not found
    {
        return false;
    }
    iter->Commented = cc;                                                   // Change the Comment value

    if (!Saving)
    {
        return true;
    }
    return Save();                                                          // Save
}

bool IniFile::UnCommentSection(string const& SectionName, bool Saving/* = true*/)
{
    vector<Record>& content = m_content;                                    // Holds the current record

    if (!Load())                                                            // Make sure the file is loaded
    {
        return false;                                                       // In the event the file does not load
    }

    if (content.empty())
    {
        return false;
    }

    assert(!SectionName.empty());
    for (vector<Record>::iterator iter = content.begin();
        iter != content.end(); ++iter)
    {
        if (iter->Section == SectionName)                                   // Is this the right section?
        {
            iter->Commented = ' ';                                          // Remove the comment value
        }
    }

    if (!Saving)
    {
        return true;
    }
    return Save();                                                          // Save
}

bool IniFile::UnCommentRecord(string const& KeyName, string const& SectionName, bool Saving/* = true*/)
{
    vector<Record>& content = m_content;                                    // Holds the current record

    if (!Load())                                                             // Make sure the file is loaded
    {
        return false;                                                       // In the event the file does not load
    }

    if (content.empty())
    {
        return false;
    }

    assert(!KeyName.empty());
    assert(!SectionName.empty());
    vector<Record>::iterator iter = find_if(
        content.begin(), content.end(),
        IniFile::RecordSectionKeyIs(SectionName, KeyName));                 // Locate the Section/Key
    if (iter == content.end())                                              // The Section/Key was not found
    {
        return false;
    }
    iter->Commented = ' ';                                                  // Remove the Comment value

    if (!Saving)
    {
        return true;
    }
    return Save();                                                          // Save
}
