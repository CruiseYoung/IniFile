// TestCIniFile::cpp : Defines the entry point for the console application.
//
#include "../src/IniFile.h"
#include <iostream>

using namespace std;

void Show(CIniFile& IniFile)
{
	cout << endl
		<< "++++++++++++++++++++++++++++++++++++++++"
		<< endl
		<< "+ Contents of the file are below       +"
		<< endl
		<< "++++++++++++++++++++++++++++++++++++++++"
		<< endl
		<< IniFile.Content()
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
	CIniFile IniFile(FileName);

	// Create a new file
	cout << "TestIniFile - Demo program for the CIniFile Class" << endl << endl;
	cout << "Attempting to create a new file called \"test.ini\"" << endl << endl;
	cout << "string FileName = \"test.ini\";" << endl;
	cout << "CIniFile::Create(FileName);" << endl << endl;
	if (IniFile.Create()) cout << "File was successfully created" << endl << endl;
	else cout << "Failed to create the file" << endl << endl;
	Show(IniFile);

	// Create a new section
	cout << "TestIniFile - Demo program for the CIniFile Class" << endl << endl;
	cout << "Attempting to create a new section called [MySection]" << endl << endl;
	cout << "CIniFile::AddSection(\"MySection\", FileName);" << endl << endl;
	if (IniFile.AddSection("MySection")) cout << "Section was successfully created" << endl << endl;
	else cout << "Failed to create the section" << endl << endl;
	Show(IniFile);

	// Add a key to the section
	cout << "TestIniFile - Demo program for the CIniFile Class" << endl << endl;
	cout << "Attempting to add a new key/value (MyKey=MyValue) to [MySection]" << endl << endl;
	cout << "CIniFile::SetValue(\"MyKey\",\"MyValue\",\"MySection\",FileName);" << endl << endl;
	if (IniFile.SetValue("MyKey", "MyValue", "MySection")) cout << "Record was successfully created" << endl << endl;
	else cout << "Failed to create the record" << endl << endl;
	Show(IniFile);

	// Add a key and create a section
	cout << "TestIniFile - Demo program for the CIniFile Class" << endl << endl;
	cout << "Attempting to add a new key/value (TestKey=TestValue)" << endl << "and create a new section [TestSection] at the same time" << endl << endl;
	cout << "CIniFile::SetValue(\"TestKey\",\"TestValue\",\"TestSection\",FileName);" << endl << endl;
	if (IniFile.SetValue("TestKey", "TestValue", "TestSection")) cout << "Record and section were successfully created" << endl << endl;
	else cout << "Failed to create the record and section" << endl << endl;
	Show(IniFile);

	// Change a key value
	cout << "TestIniFile - Demo program for the CIniFile Class" << endl << endl;
	cout << "Attempting to change the key/value for (MyKey=MyValue) to (MyKey=YourValue)" << endl << endl;
	cout << "CIniFile::SetValue(\"MyKey\",\"YourValue\",\"MySection\",FileName);" << endl << endl;
	if (IniFile.SetValue("MyKey", "YourValue", "MySection")) cout << "Record was successfully changed" << endl << endl;
	else cout << "Failed to change the record" << endl << endl;
	Show(IniFile);

	// Get a value
	cout << "TestIniFile - Demo program for the CIniFile Class" << endl << endl;
	cout << "Attempting to get the value of MyKey" << endl << endl;
	cout << "CIniFile::GetValue(\"MyKey\",\"MySection\",FileName);" << endl << endl;
	string v = IniFile.GetValue("MyKey", "MySection");
	cout << "The value of MyKey is: " << v.c_str() << endl << endl;
	Show(IniFile);

	// Get a list of Sections
	cout << "TestIniFile - Demo program for the CIniFile Class" << endl << endl;
	cout << "Attempting to get a list of sections" << endl << endl;
	cout << "CIniFile::GetSectionNames(FileName);" << endl << endl;
	list<string> s = IniFile.GetSectionNames();
	cout << "The sections are returned as a std::list<std::string>\n\n";
	for (list<string>::iterator iter = s.begin(); iter != s.end(); ++iter)
		cout << iter->c_str() << endl;
	Show(IniFile);

	// Section Exists
	cout << "TestIniFile - Demo program for the CIniFile Class" << endl << endl;
	cout << "Attempting to verify that [MySection] exists" << endl << endl;
	cout << "CIniFile::SectionExists(\"MySection\",FileName);" << endl << endl;
	if (IniFile.SectionExists("MySection")) cout << "Section exists" << endl << endl;
	else cout << "Section does not exist" << endl << endl;
	Show(IniFile);

	// Record Exists
	cout << "TestIniFile - Demo program for the CIniFile Class" << endl << endl;
	cout << "Attempting to verify that MyKey exists" << endl << endl;
	cout << "CIniFile::RecordExists(\"MyKey\",\"MySection\",FileName);" << endl << endl;
	if (IniFile.RecordExists("MyKey", "MySection")) cout << "Record exists" << endl << endl;
	else cout << "Record does not exist" << endl << endl;
	Show(IniFile);

	// Case Sensitive
	cout << "TestIniFile - Demo program for the CIniFile Class" << endl << endl;
	cout << "BE CAREFUL - functions in CIniFile are CASE-SENSITIVE" << endl << endl;
	cout << "CIniFile::RecordExists(\"mykey\",\"MySection\",FileName);" << endl << endl;
	if (IniFile.RecordExists("mykey", "MySection")) cout << "Record exists" << endl << endl;
	else cout << "Record does not exist" << endl << endl;
	Show(IniFile);

	// Add a comment to the section
	cout << "TestIniFile - Demo program for the CIniFile Class" << endl << endl;
	cout << "Attempting to add comments to [MySection]" << endl << endl;
	cout << "CIniFile::SetSectionComments(\"# This Section was created by CIniFile\",\"MySection\",FileName);" << endl << endl;
	if (IniFile.SetSectionComments("# This Section was created by CIniFile", "MySection")) cout << "Comments were successfully added" << endl << endl;
	else cout << "Failed to add the comments" << endl << endl;
	Show(IniFile);

	// Add a comment to the record
	cout << "TestIniFile - Demo program for the CIniFile Class" << endl << endl;
	cout << "Attempting to add comments to MyKey" << endl << endl;
	cout << "CIniFile::SetRecordComments(\"# This Key was created by CIniFile\",\"MyKey\",\"MySection\",FileName);" << endl << endl;
	if (IniFile.SetRecordComments("# This Key was created by CIniFile", "MyKey", "MySection")) cout << "Comments were successfully added" << endl << endl;
	else cout << "Failed to add the comments" << endl << endl;
	Show(IniFile);

	// Rename Section
	cout << "TestIniFile - Demo program for the CIniFile Class" << endl << endl;
	cout << "Attempting to rename [MySection] to [YourSection]" << endl << endl;
	cout << "CIniFile::RenameSection(\"MySection\",\"YourSection\",FileName);" << endl << endl;
	if (IniFile.RenameSection("MySection", "YourSection")) cout << "Section was successfully changed" << endl << endl;
	else cout << "Failed to change the section" << endl << endl;
	Show(IniFile);

	// Multiple comments
	cout << "TestIniFile - Demo program for the CIniFile Class" << endl << endl;
	cout << "Multiple comments can be added by putting \\n# in the comments string" << endl << endl;
	cout << "CIniFile::SetSectionComments(\"# This Section was created by CIniFile\\n# Kids, don't try this at home \\n\",\"YourSection\",FileName);" << endl << endl;
	if (IniFile.SetSectionComments("# This Section was created by CIniFile\n# Kids, don't try this at home", "YourSection")) cout << "Comments were successfully added" << endl << endl;
	else cout << "Failed to add the comments" << endl << endl;
	Show(IniFile);

	// Remove comments
	cout << "TestIniFile - Demo program for the CIniFile Class" << endl << endl;
	cout << "Comments are removed by setting them to \"\"" << endl << endl;
	cout << "CIniFile::SetRecordComments(\"\",\"MyKey\",\"YourSection\",FileName);" << endl << endl;
	if (IniFile.SetRecordComments("", "MyKey", "YourSection")) cout << "Comments were successfully removed" << endl << endl;
	else cout << "Failed to remove the comments" << endl << endl;
	Show(IniFile);

	// Comment entire section
	cout << "TestIniFile - Demo program for the CIniFile Class" << endl << endl;
	cout << "Attempting to comment the entire section [YourSection]" << endl << endl;
	cout << "CIniFile::CommentSection(\"#\",\"YourSection\",FileName);" << endl << endl;
	if (IniFile.CommentSection('#', "YourSection")) cout << "Section was successfully commented" << endl << endl;
	else cout << "Failed to comment the section" << endl << endl;
	Show(IniFile);

	// UnComment entire section
	cout << "TestIniFile - Demo program for the CIniFile Class" << endl << endl;
	cout << "Attempting to un-comment the entire section [YourSection]" << endl << endl;
	cout << "CIniFile::UnCommentSection(\"YourSection\",FileName);" << endl << endl;
	if (IniFile.UnCommentSection("YourSection")) cout << "Section was successfully un-commented" << endl << endl;
	else cout << "Failed to un-comment the section" << endl << endl;
	Show(IniFile);

	// Comment a single record
	cout << "TestIniFile - Demo program for the CIniFile Class" << endl << endl;
	cout << "Attempting to comment the record MyKey" << endl << endl;
	cout << "(Note that both # and ; are recognized as commented lines by CIniFile)" << endl << endl;
	cout << "CIniFile::CommentRecord(CIniFile::CommentChar::Pound,\"MyKey\",\"YourSection\",FileName);" << endl << endl;
	if (IniFile.CommentRecord(CIniFile::Pound, "MyKey", "YourSection")) cout << "Record was successfully commented" << endl << endl;
	else cout << "Failed to comment the record" << endl << endl;
	Show(IniFile);

	// UnComment a single record
	cout << "TestIniFile - Demo program for the CIniFile Class" << endl << endl;
	cout << "Attempting to un-comment the record MyKey" << endl << endl;
	cout << "CIniFile::UnCommentRecord(\"MyKey\",\"YourSection\",FileName);" << endl << endl;
	if (IniFile.UnCommentRecord("MyKey", "YourSection")) cout << "Record was successfully un-commented" << endl << endl;
	else cout << "Failed to un-comment the record" << endl << endl;
	Show(IniFile);

	// Sort
	cout << "TestIniFile - Demo program for the CIniFile Class" << endl << endl;
	cout << "Attempting to sort the file - false means ASCENDING, true means DESCENDING" << endl << endl;
	cout << "(Note that the comments will stay with their targets)" << endl << endl;
	cout << "CIniFile::Sort(FileName,false);" << endl << endl;
	if (IniFile.Sort(false)) cout << "File was successfully sorted" << endl << endl;
	else cout << "Failed to sort the file" << endl << endl;
	Show(IniFile);

	// Delete entire section
	cout << "TestIniFile - Demo program for the CIniFile Class" << endl << endl;
	cout << "Attempting to delete the entire section [TestSection]" << endl << endl;
	cout << "CIniFile::DeleteSection(\"TestSection\",FileName);" << endl << endl;
	if (IniFile.DeleteSection("TestSection")) cout << "Section was successfully deleted" << endl << endl;
	else cout << "Failed to delete the section" << endl << endl;
	Show(IniFile);

	// Delete record
	cout << "TestIniFile - Demo program for the CIniFile Class" << endl << endl;
	cout << "Attempting to delete the record <yKey" << endl << endl;
	cout << "CIniFile::DeleteRecord(\"MyKey\",\"YourSection\",FileName);" << endl << endl;
	if (IniFile.DeleteRecord("MyKey", "YourSection")) cout << "Record was successfully deleted" << endl << endl;
	else cout << "Failed to delete the record" << endl << endl;
	Show(IniFile);

	// Content
	cout << "TestIniFile - Demo program for the CIniFile Class" << endl << endl;
	cout << "Finally, the content of the file can be retrieved as a std::string" << endl << endl;
	cout << "CIniFile::Content(FileName);" << endl << endl;
	cout << "The contents of the file throughout this demo have used this function to display the contents below" << endl;
	Show(IniFile);

	return 0;
}
