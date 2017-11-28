// TestCIniFile::cpp : Defines the entry point for the console application.
//
#include "../src/inifile.h"
#include <iostream>

using namespace std;

void Show(const string& FileName)
{
    cout << endl
        << "++++++++++++++++++++++++++++++++++++++++"
        << endl
        << "+ Contents of the file are below       +"
        << endl
        << "++++++++++++++++++++++++++++++++++++++++"
        << endl
        << IniFile::Content(FileName)
        << endl
        << "++++++++++++++++++++++++++++++++++++++++"
        << endl << endl;
    system("PAUSE");
    system("cls");
}

int main(int argc, char* argv[])
{
    string str_num = "555";
    cout << convert<int>(str_num) << endl;

    float f_num = 666.0f;
    cout << convert<string>(f_num) << endl;

    system("PAUSE");
    system("cls");

    const string FileName = "test.ini";

    // Create a new file
    cout << "TestIniFile - Demo program for the IniFile Class" << endl << endl;
    cout << "Attempting to create a new file called \"test.ini\"" << endl << endl;
    cout << "string FileName = \"test.ini\";" << endl;
    cout << "IniFile::Create(FileName);" << endl << endl;
    if (IniFile::Create(FileName)) cout << "File was successfully created" << endl << endl;
    else cout << "Failed to create the file" << endl << endl;
    Show(FileName);

    // Create a new section
    cout << "TestIniFile - Demo program for the IniFile Class" << endl << endl;
    cout << "Attempting to create a new section called [MySection]" << endl << endl;
    cout << "IniFile::AddSection(\"MySection\", FileName);" << endl << endl;
    if (IniFile::AddSection("MySection", FileName)) cout << "Section was successfully created" << endl << endl;
    else cout << "Failed to create the section" << endl << endl;
    Show(FileName);

    // Add a key to the section
    cout << "TestIniFile - Demo program for the IniFile Class" << endl << endl;
    cout << "Attempting to add a new key/value (MyKey=MyValue) to [MySection]" << endl << endl;
    cout << "IniFile::SetValue(\"MyKey\",\"MyValue\",\"MySection\",FileName);" << endl << endl;
    if (IniFile::SetValue("MyKey", "MyValue", "MySection", FileName)) cout << "Record was successfully created" << endl << endl;
    else cout << "Failed to create the record" << endl << endl;
    Show(FileName);

    // Add a key and create a section
    cout << "TestIniFile - Demo program for the IniFile Class" << endl << endl;
    cout << "Attempting to add a new key/value (TestKey=TestValue)" << endl << "and create a new section [TestSection] at the same time" << endl << endl;
    cout << "IniFile::SetValue(\"TestKey\",\"TestValue\",\"TestSection\",FileName);" << endl << endl;
    if (IniFile::SetValue("TestKey", "TestValue", "TestSection", FileName)) cout << "Record and section were successfully created" << endl << endl;
    else cout << "Failed to create the record and section" << endl << endl;
    Show(FileName);

    // Change a key value
    cout << "TestIniFile - Demo program for the IniFile Class" << endl << endl;
    cout << "Attempting to change the key/value for (MyKey=MyValue) to (MyKey=YourValue)" << endl << endl;
    cout << "IniFile::SetValue(\"MyKey\",\"YourValue\",\"MySection\",FileName);" << endl << endl;
    if (IniFile::SetValue("MyKey", "YourValue", "MySection", FileName)) cout << "Record was successfully changed" << endl << endl;
    else cout << "Failed to change the record" << endl << endl;
    Show(FileName);

    // Get a value
    cout << "TestIniFile - Demo program for the IniFile Class" << endl << endl;
    cout << "Attempting to get the value of MyKey" << endl << endl;
    cout << "IniFile::GetValue(\"MyKey\",\"MySection\",FileName);" << endl << endl;
    string v = IniFile::GetValue("MyKey", "MySection", FileName);
    cout << "The value of MyKey is: " << v.c_str() << endl << endl;
    Show(FileName);

    // Get a vector of Sections
    cout << "TestIniFile - Demo program for the IniFile Class" << endl << endl;
    cout << "Attempting to get a vector of sections" << endl << endl;
    cout << "IniFile::GetSectionNames(FileName);" << endl << endl;
    vector<string> s = IniFile::GetSectionNames(FileName);
    cout << "The sections are returned as a std::vector<std::string>\n\n";
    for (vector<string>::iterator iter = s.begin(); iter != s.end(); ++iter)
        cout << iter->c_str() << endl;
    Show(FileName);

    // Section Exists
    cout << "TestIniFile - Demo program for the IniFile Class" << endl << endl;
    cout << "Attempting to verify that [MySection] exists" << endl << endl;
    cout << "IniFile::SectionExists(\"MySection\",FileName);" << endl << endl;
    if (IniFile::SectionExists("MySection", FileName)) cout << "Section exists" << endl << endl;
    else cout << "Section does not exist" << endl << endl;
    Show(FileName);

    // Record Exists
    cout << "TestIniFile - Demo program for the IniFile Class" << endl << endl;
    cout << "Attempting to verify that MyKey exists" << endl << endl;
    cout << "IniFile::RecordExists(\"MyKey\",\"MySection\",FileName);" << endl << endl;
    if (IniFile::RecordExists("MyKey", "MySection", FileName)) cout << "Record exists" << endl << endl;
    else cout << "Record does not exist" << endl << endl;
    Show(FileName);

    // Case Sensitive
    cout << "TestIniFile - Demo program for the IniFile Class" << endl << endl;
    cout << "BE CAREFUL - functions in IniFile are CASE-SENSITIVE" << endl << endl;
    cout << "IniFile::RecordExists(\"mykey\",\"MySection\",FileName);" << endl << endl;
    if (IniFile::RecordExists("mykey", "MySection", FileName)) cout << "Record exists" << endl << endl;
    else cout << "Record does not exist" << endl << endl;
    Show(FileName);

    // Add a comment to the section
    cout << "TestIniFile - Demo program for the IniFile Class" << endl << endl;
    cout << "Attempting to add comments to [MySection]" << endl << endl;
    cout << "IniFile::SetSectionComments(\"# This Section was created by IniFile\",\"MySection\",FileName);" << endl << endl;
    if (IniFile::SetSectionComments("# This Section was created by IniFile", "MySection", FileName)) cout << "Comments were successfully added" << endl << endl;
    else cout << "Failed to add the comments" << endl << endl;
    Show(FileName);

    // Add a comment to the record
    cout << "TestIniFile - Demo program for the IniFile Class" << endl << endl;
    cout << "Attempting to add comments to MyKey" << endl << endl;
    cout << "IniFile::SetRecordComments(\"# This Key was created by IniFile\",\"MyKey\",\"MySection\",FileName);" << endl << endl;
    if (IniFile::SetRecordComments("# This Key was created by IniFile", "MyKey", "MySection", FileName)) cout << "Comments were successfully added" << endl << endl;
    else cout << "Failed to add the comments" << endl << endl;
    Show(FileName);

    // Rename Section
    cout << "TestIniFile - Demo program for the IniFile Class" << endl << endl;
    cout << "Attempting to rename [MySection] to [YourSection]" << endl << endl;
    cout << "IniFile::RenameSection(\"MySection\",\"YourSection\",FileName);" << endl << endl;
    if (IniFile::RenameSection("MySection", "YourSection", FileName)) cout << "Section was successfully changed" << endl << endl;
    else cout << "Failed to change the section" << endl << endl;
    Show(FileName);

    // Multiple comments
    cout << "TestIniFile - Demo program for the IniFile Class" << endl << endl;
    cout << "Multiple comments can be added by putting \\n# in the comments string" << endl << endl;
    cout << "IniFile::SetSectionComments(\"# This Section was created by IniFile\\n# Kids, don't try this at home \\n\",\"YourSection\",FileName);" << endl << endl;
    if (IniFile::SetSectionComments("# This Section was created by IniFile\n# Kids, don't try this at home", "YourSection", FileName)) cout << "Comments were successfully added" << endl << endl;
    else cout << "Failed to add the comments" << endl << endl;
    Show(FileName);

    // Remove comments
    cout << "TestIniFile - Demo program for the IniFile Class" << endl << endl;
    cout << "Comments are removed by setting them to \"\"" << endl << endl;
    cout << "IniFile::SetRecordComments(\"\",\"MyKey\",\"YourSection\",FileName);" << endl << endl;
    if (IniFile::SetRecordComments("", "MyKey", "YourSection", FileName)) cout << "Comments were successfully removed" << endl << endl;
    else cout << "Failed to remove the comments" << endl << endl;
    Show(FileName);

    // Comment entire section
    cout << "TestIniFile - Demo program for the IniFile Class" << endl << endl;
    cout << "Attempting to comment the entire section [YourSection]" << endl << endl;
    cout << "IniFile::CommentSection(\"#\",\"YourSection\",FileName);" << endl << endl;
    if (IniFile::CommentSection('#', "YourSection", FileName)) cout << "Section was successfully commented" << endl << endl;
    else cout << "Failed to comment the section" << endl << endl;
    Show(FileName);

    // UnComment entire section
    cout << "TestIniFile - Demo program for the IniFile Class" << endl << endl;
    cout << "Attempting to un-comment the entire section [YourSection]" << endl << endl;
    cout << "IniFile::UnCommentSection(\"YourSection\",FileName);" << endl << endl;
    if (IniFile::UnCommentSection("YourSection", FileName)) cout << "Section was successfully un-commented" << endl << endl;
    else cout << "Failed to un-comment the section" << endl << endl;
    Show(FileName);

    // Comment a single record
    cout << "TestIniFile - Demo program for the IniFile Class" << endl << endl;
    cout << "Attempting to comment the record MyKey" << endl << endl;
    cout << "(Note that both # and ; are recognized as commented lines by IniFile)" << endl << endl;
    cout << "IniFile::CommentRecord(IniFile::CommentChar::Pound,\"MyKey\",\"YourSection\",FileName);" << endl << endl;
    if (IniFile::CommentRecord(IniFile::Pound, "MyKey", "YourSection", FileName)) cout << "Record was successfully commented" << endl << endl;
    else cout << "Failed to comment the record" << endl << endl;
    Show(FileName);

    // UnComment a single record
    cout << "TestIniFile - Demo program for the IniFile Class" << endl << endl;
    cout << "Attempting to un-comment the record MyKey" << endl << endl;
    cout << "IniFile::UnCommentRecord(\"MyKey\",\"YourSection\",FileName);" << endl << endl;
    if (IniFile::UnCommentRecord("MyKey", "YourSection", FileName)) cout << "Record was successfully un-commented" << endl << endl;
    else cout << "Failed to un-comment the record" << endl << endl;
    Show(FileName);

    // Sort
    cout << "TestIniFile - Demo program for the IniFile Class" << endl << endl;
    cout << "Attempting to sort the file - false means ASCENDING, true means DESCENDING" << endl << endl;
    cout << "(Note that the comments will stay with their targets)" << endl << endl;
    cout << "IniFile::Sort(FileName,false);" << endl << endl;
    if (IniFile::Sort(FileName, false)) cout << "File was successfully sorted" << endl << endl;
    else cout << "Failed to sort the file" << endl << endl;
    Show(FileName);

    // Delete entire section
    cout << "TestIniFile - Demo program for the IniFile Class" << endl << endl;
    cout << "Attempting to delete the entire section [TestSection]" << endl << endl;
    cout << "IniFile::DeleteSection(\"TestSection\",FileName);" << endl << endl;
    if (IniFile::DeleteSection("TestSection", FileName)) cout << "Section was successfully deleted" << endl << endl;
    else cout << "Failed to delete the section" << endl << endl;
    Show(FileName);

    // Delete record
    cout << "TestIniFile - Demo program for the IniFile Class" << endl << endl;
    cout << "Attempting to delete the record <yKey" << endl << endl;
    cout << "IniFile::DeleteRecord(\"MyKey\",\"YourSection\",FileName);" << endl << endl;
    if (IniFile::DeleteRecord("MyKey", "YourSection", FileName)) cout << "Record was successfully deleted" << endl << endl;
    else cout << "Failed to delete the record" << endl << endl;
    Show(FileName);

    // Content
    cout << "TestIniFile - Demo program for the IniFile Class" << endl << endl;
    cout << "Finally, the content of the file can be retrieved as a std::string" << endl << endl;
    cout << "IniFile::Content(FileName);" << endl << endl;
    cout << "The contents of the file throughout this demo have used this function to display the contents below" << endl;
    Show(FileName);

    return 0;
}
