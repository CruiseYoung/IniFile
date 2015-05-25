#include "IniFile.h"
#include <fstream>
#include <cassert>


CIniFile::CIniFile(void)													// Default constructor
    : m_Loaded(false)
{
}

CIniFile::CIniFile(const string& FileName)									// constructor
    : m_FileName(FileName)
    , m_Loaded(false)
{
}


CIniFile::~CIniFile(void)
{
}

// replace all old_value with new_value in 'str'
std::string& replace_all(std::string& str, const std::string& old_value, const std::string& new_value)
{   
	while (true)
	{   
		std::string::size_type pos(0);
		if ((pos = str.find(old_value)) != std::string::npos)
			str.replace(pos,old_value.length(),new_value);   
		else
			break;   
	}   
	return str;   
}   

// A function to trim whitespace from both sides of a given string
void Trim(std::string& str, const std::string & ChrsToTrim = " \t\n\r", int TrimDir = 0)
{
    std::size_t startIndex = str.find_first_not_of(ChrsToTrim);
    if (startIndex == std::string::npos)
    {
        str.erase();
        return;
    }
	
    if (TrimDir < 2)
        str = str.substr(startIndex, str.size() - startIndex);
    if (TrimDir != 1)
        str = str.substr(0, str.find_last_not_of(ChrsToTrim) + 1);

	// Trim whitespace from both sides of '='
	if (str.at(0) != '#')
		str = replace_all(str, " ", "");
	if (str.at(0) != '#')
		str = replace_all(str, "\t", "");
}

//inline void TrimRight(std::string& str, const std::string & ChrsToTrim = " \t\n\r")
//{
//    Trim(str, ChrsToTrim, 2);
//}

//inline void TrimLeft(std::string& str, const std::string & ChrsToTrim = " \t\n\r")
//{
//    Trim(str, ChrsToTrim, 1);
//}

// A function to transform a string to uppercase if neccessary
//void UCase(std::string& str, bool ucase)
//{
//	if(ucase)
//		transform(str.begin(), str.end(), str.begin(), toupper);
//}

void CIniFile::SetFileName(const string& FileName)
{
    m_FileName = FileName;
}


bool CIniFile::Load()
{
    string& FileName = m_FileName;
    vector<Record>& content = m_content;

    assert(!FileName.empty());
	if (m_Loaded)
		return true;

    string s;																// Holds the current line from the ini file
    string CurrentSection;												    // Holds the current section name

	std::ifstream inFile(FileName.c_str());									// Create an input filestream
    if (!inFile.is_open())
        return false;									                    // If the input file doesn't open, then return
    content.clear();														// Clear the content vector

    string comments = "";													// A string to store comments in

    while (!std::getline(inFile, s).eof())									// Read until the end of the file
    {
        Trim(s);															// Trim whitespace from the ends
        if (!s.empty())														// Make sure its not a blank line
        {
            Record r;														// Define a new record

            if ((s[0] == '#') || (s[0] == ';'))								// Is this a commented line?
            {
                if ((s.find('[') == string::npos) 							// If there is no [ or =
                    && (s.find('=') == string::npos))						// Then it's a comment
                {
                    comments += s + '\n';									// Add the comment to the current comments string
                }
                else
                {
                    r.Commented = s[0];										// Save the comment character
                    s.erase(s.begin());										// Remove the comment for further processing
                    Trim(s);
                }// Remove any more whitespace
            }
            else
				r.Commented = ' ';										    // else mark it as not being a comment

            if (s.find('[') != string::npos)								// Is this line a section?
            {
                s.erase(s.begin());											// Erase the leading bracket
                s.erase(s.find(']'));										// Erase the trailing bracket
                r.Comments = comments;									    // Add the comments string (if any)
                comments = "";												// Clear the comments for re-use
                r.Section = s;												// Set the Section value
                r.Key = "";													// Set the Key value
                r.Value = "";												// Set the Value value
                CurrentSection = s;
            }

            if (s.find('=') != string::npos)								// Is this line a Key/Value?
            {
                r.Comments = comments;									    // Add the comments string (if any)
                comments = "";												// Clear the comments for re-use
                r.Section = CurrentSection;								    // Set the section to the current Section
                r.Key = s.substr(0, s.find('='));							// Set the Key value to everything before the = sign
                r.Value = s.substr(s.find('=') + 1);						// Set the Value to everything after the = sign
            }

            if (comments == "")											    // Don't add a record yet if its a comment line
                content.push_back(r);										// Add the record to content
        }
    }

    inFile.close();															// Close the file

    m_Loaded = true;
    return true;
}

bool CIniFile::Save()
{
    string& FileName = m_FileName;
    vector<Record>& content = m_content;

    assert(!FileName.empty());

	std::ofstream outFile(FileName.c_str());								// Create an output filestream
    if (!outFile.is_open())
        return false;									                    // If the output file doesn't open, then return

    for (int i = 0; i < (int)content.size(); i++)							// Loop through each vector
    {
        outFile << content[i].Comments;									    // Write out the comments
		if (content[i].Commented != ' ')
			outFile << content[i].Commented;

		if(content[i].Key == "")											// Is this a section?			
			outFile<< "[" << content[i].Section << "]" << std::endl;		// Then format the section
		else
			outFile << content[i].Key << "=" << content[i].Value << std::endl;	// Else format a key/value
    }

    outFile.close();														// Close the file
    return true;
}

bool CIniFile::FileExist()
{
    string& FileName = m_FileName;
	assert(!FileName.empty());
	
    struct stat stFileInfo;

    if (0 == stat(FileName.c_str(), &stFileInfo))
        return true;

    return false;
}

bool CIniFile::Create()
{
    if (FileExist())
        return true;

    return Save();										                    // Save
}

std::vector<std::string> CIniFile::GetSectionNames()
{
    vector<string> data;													// Holds the return data
    vector<Record>& content = m_content;									// Holds the current record

    if (Load())											                    // Make sure the file is loaded
    {
        for (int i = 0; i < (int)content.size(); i++)						// Loop through the content
        {
            if (content[i].Key == "")										// If there is no key value, then its a section
                data.push_back(content[i].Section);						    // Add the section to the return data
        }
    }

    return data;															// Return the data
}

std::vector<CIniFile::Record> CIniFile::GetSections()
{
    vector<Record> data;													// Holds the return data
    vector<Record>& content = m_content;									// Holds the current record

    if (Load())											                    // Make sure the file is loaded
    {
        for (int i = 0; i < (int)content.size(); i++)						// Loop through the content
        {
            if (content[i].Key == "")										// If this is a section 
                data.push_back(content[i]);								    // Add the record to the return data
        }
    }

    return data;															// Return the data
}

std::vector<CIniFile::Record> CIniFile::GetSection(const string& SectionName)
{
    vector<Record> data;													// Holds the return data
    vector<Record>& content = m_content;									// Holds the current record

    if (Load())											                    // Make sure the file is loaded
    {
        for (int i = 0; i < (int)content.size(); i++)						// Loop through the content
        {
            if ((content[i].Section == SectionName)					        // If this is the section name we want
                && (content[i].Key != ""))									// but not the section name itself
                data.push_back(content[i]);								    // Add the record to the return data
        }
    }

    return data;															// Return the data
}

std::vector<CIniFile::Record> CIniFile::GetRecord(const string& KeyName, const string& SectionName)
{
    vector<Record> data;													// Holds the return data
    vector<Record>& content = m_content;									// Holds the current record

    if (Load())											                    // Make sure the file is loaded
    {
        vector<Record>::iterator iter = std::find_if(content.begin(),
            content.end(),
            CIniFile::RecordSectionKeyIs(SectionName, KeyName));		    // Locate the Record

        if (iter == content.end())
            return data;								                    // The Record was not found

        data.push_back(*iter);											    // The Record was found
    }
    return data;															// Return the Record
}

std::string CIniFile::GetValue(const string& KeyName, const string& SectionName)
{
    vector<Record> record = GetRecord(KeyName, SectionName);	            // Get the Record

    if (!record.empty())													// Make sure there is a value to return
        return record[0].Value;											    // And return the value

    return "";																// No value was found
}

std::string CIniFile::Content()
{
    string s = "";															// Hold our return string
    vector<Record>& content = m_content;									// Holds the current record

    if (Load())											                    // Make sure the file is loaded
    {
        for (int i = 0; i < (int)content.size(); i++)						// Loop through the content
        {
            if (content[i].Comments != "")
				s += content[i].Comments;									// Add the comments
            if (content[i].Commented != ' ')
				s += content[i].Commented;									// If this is commented, then add it
            if ((content[i].Key == ""))										// Is this a section?
                s += '[' + content[i].Section + ']';						// Add the section
            else
                s += content[i].Key + '=' + content[i].Value;			    // Or the Key value to the return srting

            if (i != content.size())
                s += '\n';								                    // If this is not the last line, add a CrLf
        }
        return s;															// Return the contents
    }

    return "";
}

bool CIniFile::SectionExists(const string& SectionName)
{
    vector<Record>& content = m_content;									// Holds the current record

    if (Load())											                    // Make sure the file is loaded
    {
        vector<Record>::iterator iter = std::find_if(content.begin(),
            content.end(),
            CIniFile::RecordSectionIs(SectionName));					    // Locate the Section

        if (iter == content.end())
            return false;							                        // The Section was not found
    }
    else
        return false;

    return true;															// The Section was found
}

bool CIniFile::RecordExists(const string& KeyName, const string& SectionName)
{
    vector<Record>& content = m_content;									// Holds the current record

    if (Load())											                    // Make sure the file is loaded
    {
        vector<Record>::iterator iter = std::find_if(content.begin(),
            content.end(),
            CIniFile::RecordSectionKeyIs(SectionName, KeyName));	        // Locate the Section/Key

        if (iter == content.end())
            return false;							                        // The Section/Key was not found
    }
    else
        return false;

    return true;															// The Section/Key was found
}

bool CIniFile::AddSection(const string& SectionName, bool Saving/* = true*/)
{
    vector<Record>& content = m_content;									// Holds the current record

    if (!Create())
        return false;

    if (Load())											                    // Make sure the file is loaded
    {
        if (!SectionExists(SectionName))
        {
 			Record s = {"", ' ', SectionName, "", ""};						// Define a new section
//	        Record s;
//	        s.Comments  = "";
//	        s.Commented = ' ';
//	        s.Section   = SectionName;
//	        s.Key       = "";
//	        s.Value     = "";

            content.push_back(s);											// Add the section
            if (Saving)
                return Save();												// Save
            else
                return true;
        }
		else
        	return true;
    }

    return false;															// The file did not open
}

bool CIniFile::SetValue(const string& KeyName, const string& Value, const string& SectionName, bool Saving/* = true*/)
{
    vector<Record>& content = m_content;									// Holds the current record

    if (!Create())
        return false;

    if (Load())																// Make sure the file is loaded
    {
        if (!SectionExists(SectionName))						    		// If the Section doesn't exist
        {
            Record s = { "", ' ', SectionName, "", "" };					// Define a new section
            Record r = { "", ' ', SectionName, KeyName, Value };			// Define a new record
//			Record s;
//			s.Comments  = "";
//			s.Commented = ' ';
//			s.Section   = SectionName;
//			s.Key       = "";
//			s.Value     = "";

//			Record r;
//			r.Comments  = "";
//			r.Commented = ' ';
//			r.Section   = SectionName;
//			r.Key       = KeyName;
//			r.Value     = Value;
            content.push_back(s);											// Add the section
            content.push_back(r);											// Add the record
            if (Saving)
                return Save();												// Save
            else
                return true;
        }

        if (!RecordExists(KeyName, SectionName))				    		// If the Key doesn't exist
        {
            vector<Record>::iterator iter = std::find_if(content.begin(),
                content.end(),
                CIniFile::RecordSectionIs(SectionName));					// Locate the Section
            iter++;															// Advance just past the section
            Record r = { "", ' ', SectionName, KeyName, Value };			// Define a new record
//			Record r;
//			r.Comments  = "";
//			r.Commented = ' ';
//			r.Section   = SectionName;
//			r.Key       = KeyName;
//			r.Value     = Value;

            //content.insert(iter,r);										// Add the record
            vector<Record>::iterator orderiter = std::find_if_not(iter,
                content.end(),
                CIniFile::RecordSectionIs(SectionName));
            content.insert(orderiter, r);									// Add the record
            if (Saving)
                return Save();												// Save
            else
                return true;
        }

        vector<Record>::iterator iter = std::find_if(content.begin(),
            content.end(),
            CIniFile::RecordSectionKeyIs(SectionName, KeyName));	        // Locate the Record

        iter->Value = Value;												// Insert the correct value
        if (Saving)
            return Save();												    // Save
        else
            return true;
    }

    return false;															// In the event the file does not load
}

bool CIniFile::DeleteSection(const string& SectionName, bool Saving/* = true*/)
{
    vector<Record>& content = m_content;									// Holds the current record

    if (Load())																// Make sure the file is loaded
    {
        for (int i = (int)content.size() - 1; i > -1; i--)					// Iterate backwards through the content
        {
            if (content[i].Section == SectionName)						    // Is this related to the Section?
                content.erase(content.begin() + i);						    // Then erase it
        }
        if (Saving)
            return Save();												    // Save
        else
            return true;
    }

    return false;															// In the event the file does not load
}

bool CIniFile::DeleteRecord(const string& KeyName, const string& SectionName, bool Saving/* = true*/)
{
    vector<Record>& content = m_content;									// Holds the current record

    if (Load())																// Make sure the file is loaded
    {
        vector<Record>::iterator iter = std::find_if(content.begin(),
            content.end(),
            CIniFile::RecordSectionKeyIs(SectionName, KeyName));	        // Locate the Section/Key

        if (iter == content.end())
            return false;							                        // The Section/Key was not found

        content.erase(iter);												// Remove the Record
        if (Saving)
            return Save();												    // Save
        else
            return true;
    }

    return false;															// In the event the file does not load
}

bool CIniFile::RenameSection(const string& OldSectionName, const string& NewSectionName, bool Saving/* = true*/)
{
    vector<Record>& content = m_content;									// Holds the current record

    if (!SectionExists(NewSectionName))                            			// If the Section doesn't exist
    {
        for (vector<Record>::iterator iter = content.begin();
            iter < content.end(); iter++)									// Loop through the records
        {
            if (iter->Section == OldSectionName)							// Is this the OldSectionName?
                iter->Section = NewSectionName;							    // Now its the NewSectionName
        }

        if (Saving)
            return Save();												    // Save
        else
            return true;
    }

    return false;															// In the event the file does not load
}

bool CIniFile::Sort(bool Descending, bool Saving/* = true*/)
{
    vector<CIniFile::Record> content;										// Used to hold the sorted content
    vector<CIniFile::Record> sections = GetSections();			            // Get a list of Sections

    if (!sections.empty())													// Is there anything to process?
    {
        if (Descending)													    // Descending or Ascending?
            std::sort(sections.begin(), sections.end(), DescendingSectionSort());
        else																// Sort the Sections
            std::sort(sections.begin(), sections.end(), AscendingSectionSort());

        for (vector<Record>::iterator iter = sections.begin(); iter < sections.end(); iter++) // For each Section
        {
            content.push_back(*iter);										// Add the sorted Section to the content

            vector<CIniFile::Record> records = GetSection(iter->Section);   // Get a list of Records for this section

            if (Descending)												    // Descending or Ascending?
                std::sort(records.begin(), records.end(), DescendingRecordSort());
            else															// Sort the Records
                std::sort(records.begin(), records.end(), AscendingRecordSort());

            for (vector<Record>::iterator it = records.begin(); it < records.end(); it++) // For each Record
                content.push_back(*it);									    // Add the sorted Record to the content
        }

        m_content.clear();
        std::copy(content.begin(), content.end(), std::back_inserter(m_content));

        if (Saving)
            return Save();												    // Save
        else
            return true;
    }

    return false;															// There were no sections
}

bool CIniFile::SetSectionComments(const string& Comments, const string& SectionName, bool Saving/* = true*/)
{
	string comments = Comments;
    vector<Record>& content = m_content;									// Holds the current record

    if (Load())																// Make sure the file is loaded
    {
        for (vector<Record>::iterator iter = content.begin(); iter < content.end(); iter++)	// Loop through the records
        {
            if ((iter->Section == SectionName) &&							// Is this the Section?
                (iter->Key == ""))											// And not a record
            {
                if (Comments.size() >= 2)									// Is there a comment?
                {
                    if (Comments.substr(Comments.size() - 2) != "\n")		// Does the string end in a newline?
						comments += "\n";									// If not, add one
                }

				iter->Comments = comments;								    // Set the comments
                if (Saving)
                    return Save();											// Save
                else
                    return true;
            }
        }
    }

    return false;															// In the event the file does not load
}

bool CIniFile::SetRecordComments(const string& Comments, const string& KeyName, const string& SectionName, bool Saving/* = true*/)
{
	string comments = Comments;
    vector<Record>& content = m_content;									// Holds the current record

    if (Load())																// Make sure the file is loaded
    {
        vector<Record>::iterator iter = std::find_if(content.begin(),
            content.end(),
            CIniFile::RecordSectionKeyIs(SectionName, KeyName));	        // Locate the Section/Key

        if (iter == content.end())
            return false;							                        // The Section/Key was not found

        if (Comments.size() >= 2)											// Is there a comment?
        {
            if (Comments.substr(Comments.size() - 2) != "\n")				// Does the string end in a newline?
				comments += "\n";											// If not, add one
        }

		iter->Comments = comments;										    // Set the comments
        if (Saving)
            return Save();												    // Save
        else
            return true;
    }

    return false;															// In the event the file does not load
}

bool CIniFile::CommentSection(char CommentChar, const string& SectionName, bool Saving/* = true*/)
{
    vector<Record>& content = m_content;									// Holds the current record

    if (Load())																// Make sure the file is loaded
    {
        for (vector<Record>::iterator iter = content.begin(); iter < content.end(); iter++)
        {
            if (iter->Section == SectionName)								// Is this the right section?
                iter->Commented = CommentChar;							    // Change the comment value
        }
        if (Saving)
            return Save();												    // Save
        else
            return true;
    }

    return false;															// In the event the file does not load
}

bool CIniFile::CommentRecord(CommentChar cc, const string& KeyName, const string& SectionName, bool Saving/* = true*/)
{
    vector<Record>& content = m_content;									// Holds the current record

    if (Load())																// Make sure the file is loaded
    {
        vector<Record>::iterator iter = std::find_if(content.begin(),
            content.end(),
            CIniFile::RecordSectionKeyIs(SectionName, KeyName));	        // Locate the Section/Key

        if (iter == content.end())
            return false;							                        // The Section/Key was not found

        iter->Commented = cc;										        // Change the Comment value
        if (Saving)
            return Save();												    // Save
        else
            return true;
    }

    return false;															// In the event the file does not load
}

bool CIniFile::UnCommentSection(const string& SectionName, bool Saving/* = true*/)
{
    vector<Record>& content = m_content;									// Holds the current record

    if (Load())																// Make sure the file is loaded
    {
        for (vector<Record>::iterator iter = content.begin(); iter < content.end(); iter++)
        {
            if (iter->Section == SectionName)								// Is this the right section?
                iter->Commented = ' ';										// Remove the comment value
        }
        if (Saving)
            return Save();												    // Save
        else
            return true;
    }

    return false;															// In the event the file does not load
}

bool CIniFile::UnCommentRecord(const string& KeyName, const string& SectionName, bool Saving/* = true*/)
{
    vector<Record>& content = m_content;									// Holds the current record

    if (Load())																// Make sure the file is loaded
    {
        vector<Record>::iterator iter = std::find_if(content.begin(),
            content.end(),
            CIniFile::RecordSectionKeyIs(SectionName, KeyName));	        // Locate the Section/Key

        if (iter == content.end())
            return false;							                        // The Section/Key was not found

        iter->Commented = ' ';												// Remove the Comment value
        if (Saving)
            return Save();												    // Save
        else
            return true;
    }

    return false;															// In the event the file does not load
}

