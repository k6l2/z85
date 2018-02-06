#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include "Z85.h"
using namespace std;
int mainOutput(int argc, char** argv)
{
	if (argc != 3)
	{
		cerr << "ERROR: you must provide an output filename string!\n";
		return EXIT_FAILURE;
	}
	/// string inputTotal;
	/// string nextLine;
	/// while (getline(cin, nextLine))
	/// {
	/// 	inputTotal += nextLine;
	/// }
	/// cout << "inputTotal.size()=" << inputTotal.size() << endl;
	// Read entire cin into a string //
	string strInput{ istreambuf_iterator<char>(cin),{} };
	cout << "strInput.size()=" << strInput.size() << endl;
	// convert the standard input ASCII into binary form via Z85::decode //
	const unsigned long long decodedSize = Z85::decodedFileSize(strInput.size());
	char* decodedFile = new char[decodedSize];
	Z85::decode((char*)strInput.c_str(), decodedFile);
	// create a file using the decoded binary data, and argv[2] as the filename //
	fstream outputFile;
	outputFile.open(argv[2], std::ios::out | std::ios::binary);
	if (!outputFile.is_open())
	{
		cerr << "ERROR: could not open file! filename=\"" << argv[2] << "\"\n";
		return EXIT_FAILURE;
	}
	outputFile.write(decodedFile, decodedSize);
	outputFile.close();
	return EXIT_SUCCESS;
}
int main(int argc, char** argv)
{
	static const streamsize BUFFER_SIZE = 256;
	if (argc != 2 && argc != 3)
	{
		cerr << "usage: z85 [-o] [binary file]\n\n";
		cerr << "\tz85.exe will output an ASCII string of the binary file!\n";
		cerr << "\t-o - will cause z85.exe to read ASCII from stdin until EOF,\n";
		cerr << "\t\tand then output a binary file with the name [binary file]\n\n";
		cerr << "\tCygwin Example: ./z85 image.png > /dev/clipboard\n";
		cerr << "\tThis will send the Z85 encoded png image provided into the clipboard.\n\n";
		cerr << "\tCygwin Example 2: cat /dev/clipboard | ./z85 -o image.png\n";
		cerr << "\tThis will send ASCII from the clipboard until EOF is reached,\n";
		cerr << "\t\tand then output a binary file in the cwd called \"image.png\"\n";
		return EXIT_FAILURE;
	}
	if (string{ argv[1] } == string{ "-o" })
	{
		return mainOutput(argc, argv);
	}
	fstream file;
	file.open(argv[1], fstream::in | fstream::binary);
	/// // Stop eating new lines in binary mode!!!
	/// file.unsetf(std::ios::skipws);
	if (!file.is_open())
	{
		cerr << "ERROR: could not open file! filename=\"" << argv[1] << "\"\n";
		return EXIT_FAILURE;
	}
	file.seekg(0, ios::end);
	const streampos fileSize = file.tellg();
	file.seekg(0, ios::beg);
	char* fileData = new char[fileSize];
	file.read(fileData, fileSize);
	file.close();
	const unsigned long long encodedFileSize = Z85::encodedFileSize(fileSize);
	char* encodedData = new char[encodedFileSize + 1];
	encodedData[encodedFileSize] = 0;
	Z85::encode(fileData, fileSize, encodedData);
	cout << encodedData;
	/// DEBUG /// ////////////////////////////////////////
	/// for (streampos p = 0; p < fileSize; p += 1)
	/// {
	/// 	fileData[p] = 0;
	/// }
	/// Z85::decode(encodedData, fileData);
	return EXIT_SUCCESS;
}
