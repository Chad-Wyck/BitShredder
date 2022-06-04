// https://stackoverflow.com/questions/4373047/read-text-file-into-char-array-c-ifstream
// https://stackoverflow.com/questions/14391707/how-to-copy-bits-from-one-variable-to-another

#include <iostream>
#include <fstream>

using namespace std;

void Encrypt(char data[], size_t filesize, string pass);
void Decrypt(char data[], size_t filesize, string pass);
void ScrambleUp(char data[], size_t filesize, char bitMask);
void ScrambleDown(char data[], size_t filesize, char bitMask);

int main()
{
	bool running = true;

	fstream myFile;
	string filename;
	char password[256];
	int option;

	while (running) {
		cout << endl << "Enter 0 to Decrypt or 1 to Encrypt (Anything else to close application): ";
		cin >> option;
		cin.clear();

		if (option != 0 && option != 1)
			return 0;

		bool toEncrypt = option;

		cout << "Enter File Name: ";
		cin >> filename;

		std::ifstream infile;
		infile.open(filename, std::ios::binary);
		infile.seekg(0, std::ios::end);
		size_t filesize = infile.tellg();

		if (filesize == UINT32_MAX) {
			cout << "Bad File" << endl;
			return 0;
		}

		char* data = new char[filesize]();
		infile.seekg(0, std::ios::beg);
		infile.read(&data[0], filesize);
		infile.close();

		cin.ignore();
		cout << "Enter Password: ";
		cin.getline(password, 256, '\n');

		if (toEncrypt)
			Encrypt(data, filesize, password);
		else if (!toEncrypt)
			Decrypt(data, filesize, password);

		string outfilename;

		cout << "Enter New File Name: ";
		cin >> outfilename;

		std::ofstream outfile(outfilename, std::ofstream::binary);
		outfile.write(data, filesize);
		outfile.close();
		delete[] data;
	}
}

void ScrambleDown(char data[], size_t filesize, char bitMask) {
	char first = data[0];
	 
	for (int i = 0; i < filesize - 1; i++) 
		data[i] = (data[i] & ~bitMask) | (data[i + 1] & bitMask);
	data[filesize - 1] = (data[filesize - 1] & ~bitMask) | (first & bitMask);
}

void ScrambleUp(char data[], size_t filesize, char bitMask) {
	char first = data[filesize - 1];

	for (int i = filesize - 1; i > 0; i--)
		data[i] = (data[i] & ~bitMask) | (data[i - 1] & bitMask);
	data[0] = (data[0] & ~bitMask) | (first & bitMask);
}

void Encrypt(char data[], size_t filesize, string pass) {
	uint16_t passLength = pass.length();
	
	for (int i = 0; i < passLength; i++) {
		ScrambleDown(data, filesize, pass[i]);
		ScrambleUp(data, filesize, ~pass[i]);
	}
}

void Decrypt(char data[], size_t filesize, string pass) {
	uint16_t passLength = pass.length();

	for (int i = passLength - 1; i >= 0; i--) {
		ScrambleUp(data, filesize, pass[i]);
		ScrambleDown(data, filesize, ~pass[i]);
	}
}