#include "stdafx.h"
#include "Console.h"
#include "CSharp.h"
#include "FileC.h"
#include "ipify.h"
#include "StringF.h"
#include "VBMathsR.h"
#include "Helper.h"
#include <ctime>
#include <fstream>
#include <future>
#include <iostream>
#include <string> 
#include <thread>
#include <vector>
#define Code 31517
#define NumToAdd 23619

using namespace std;
VBMathsR r;
volatile bool addDot = false;
volatile bool publishProggress = false;
volatile unsigned int k = 0;
volatile unsigned int kCOUNTER = 0;
volatile unsigned long long byteCount = 0;
vector<bool> EncryptDone;
vector<string> EncryptedData;
vector<unsigned long> indexCounter;

void AddDots();
void Help();
void PublishProccess();
void Wrong_Args();
long GetSeed(string password);
void NoPass(const string FileName);
void WithPass(const string FileName, const string Password, const bool noIP);
void EncyptDataNoPass(const vector<char> bytes, const unsigned int index, const long StartSeed);
void EncyptDataWithPass(vector<char> bytes, const unsigned int index, const long StartSeed, const long codeForPass);

int main(int argc, char* argv[])
{
	try {
		if (argc <= 1) Help();
		else if (argc > 4) Wrong_Args();
		else if (argc == 2) NoPass(argv[1]);
		else if (argc == 3) WithPass(argv[1], argv[2], false);
		else if (argc == 4 && StringF::toLower(argv[3]) == "-xxx") WithPass(argv[1], argv[2], true);
		else Wrong_Args();
	}
	catch (exception& ex) {
		WORD s = Console::GetColor();
		cout << "\n";
		Console::SetColor(12);
		cout << ex.what();
		cout << "\n";
		Console::SetColor(s);
	}
	system("pause");
    return 0;
}
void NoPass(const string FileName) {
	//Values
	int RandomNumber;
	unsigned int ThreadsSupported = thread::hardware_concurrency();
	char c;
	unsigned long NumberToWrite;
	ifstream ifs(FileName, ios::binary | ios::ate);
	string PathRemoved = File::RemovePath(FileName);
	string FileNameWithoutEx = PathRemoved.substr(0, PathRemoved.find_last_of("."));
	string FileNameF;
	string To_Write = "#DEC_ENC::";
	string IPAddress;
	string Header = "title Encrypting " + FileName;
	ofstream myfile;
	vector<long> seeds;
	//Title
	system(Header.c_str());
	Header = "";
	//Valid File
	cout << "Validating File";
	if (!File::Exists(FileName)) throw exception("The file does not exist.");
	this_thread::sleep_for(0.5s);
	cout << ".";
	if (File::FileSize(FileName) == 0) throw exception("File is empty");
	this_thread::sleep_for(0.5s);
	cout << ".";
	this_thread::sleep_for(0.3s);
	cout << "." << endl;
	//get IP
	this_thread::sleep_for(0.6s);
	future<void> fut1 = async(AddDots);
	addDot = true;
	cout << "Getting Data From Internet";
	try {
		IPAddress = getIP();
		if (IPAddress == "NA" || IPAddress == "")
			throw exception();
	}
	catch (...) {
		addDot = false;
		throw exception("Cannot connect to internet.");
	}
	addDot = false;
	cout << endl;
	this_thread::sleep_for(0.3s);
	cout << "Enter header of your file:";
	WORD s = Console::GetColor();
	Console::SetColor(14);
	getline(cin, Header);
	Console::SetColor(s);
	if (Header == "") throw exception("Header cannot be empty.");
	system("cls");
	this_thread::sleep_for(0.5s);
	//Main Encryption
	try {
		if (!File::Exists(File::getexepath() + "\\" +FileNameWithoutEx + ".dec") || File::FileSize(File::getexepath() + "\\" + FileNameWithoutEx + ".dec") == 0) {
			FileNameF = File::getexepath() + "\\" + FileNameWithoutEx + ".dec";
		}
		else {
			unsigned short j = 1;
			while (File::Exists(File::getexepath() + "\\" + FileNameWithoutEx + to_string(j) + ".dec") || File::FileSize(File::getexepath() + "\\" + FileNameWithoutEx + to_string(j) +".dec") == 0) {
				j++;
			}
			FileNameF = File::getexepath() + "\\" + FileNameWithoutEx + to_string(j) + ".dec";
		}
		cout << "Reading File...";
		myfile.open(FileNameF);
		clock_t start = clock();
		//Encrypt Header
		for (unsigned int i = 0; i < Header.length(); i++) {
			RandomNumber = ToInt32(r.Rnd() * 50000);
			c = Header.at(i);
			NumberToWrite = ((int)c ^ Code) + NumToAdd + RandomNumber;
			To_Write.append(to_string(NumberToWrite) + " ");
		}
		To_Write.append("::");
		//Encrypt IP
		for (unsigned int i = 0; i < IPAddress.length(); i++) {
			RandomNumber = ToInt32(r.Rnd() * 50000);
			c = IPAddress.at(i);
			NumberToWrite = ((int)c ^ Code) + NumToAdd + RandomNumber;
			To_Write.append(to_string(NumberToWrite) + " ");
		}
		To_Write.append("::");
		//Encrypt FileName
		for (unsigned int i = 0; i < PathRemoved.length(); i++) {
			RandomNumber = ToInt32(r.Rnd() * 50000);
			c = PathRemoved.at(i);
			NumberToWrite = ((int)c ^ Code) + NumToAdd + RandomNumber;
			To_Write.append(to_string(NumberToWrite) + " ");
		}
		To_Write.append("::\n");
		myfile << To_Write;
		//Free resources
		IPAddress.~basic_string();
		To_Write.~basic_string();
		FileNameF.~basic_string();
		Header.~basic_string();
		FileNameWithoutEx.~basic_string();
		PathRemoved.~basic_string();
		//Encrypt Main
		//Generate Seeds
		system("cls");
		cout << "Generating Seeds...";
		byteCount = ifs.tellg();
		{
			//Fix threads supported
			if (ThreadsSupported == 0)
				ThreadsSupported = 1;
			//Fix odd threads
			while (!_HThreads::checkCorrectThreadNumber(ThreadsSupported)) {
				ThreadsSupported--;
			}
			//Seeds and other stuff
			EncryptDone.resize(ThreadsSupported);
			EncryptedData.resize(ThreadsSupported);
			long LastSeed = r.getSeed();
			seeds.push_back(LastSeed);
			for (unsigned long i = 0; i < byteCount / 100000000; i++) {
				for (unsigned long j = 0; j < ThreadsSupported; j++) {
					for (unsigned long k = 0; k < 100000000 / ThreadsSupported; k++) {
						LastSeed = (unsigned long long)(((LastSeed * 0x43fd43fd) + 0xc39ec3) & 0xffffff);
					}
					seeds.push_back(LastSeed);
				}
			}
			indexCounter.resize(ThreadsSupported);
		}
		//Read file 100,000,000 bytes at once
		system("cls");
		ifs.seekg(0, ios::beg);
		publishProggress = true;
		future<void> prog = async(PublishProccess);//Publish progress on new thread for speed
		//At first encrypt each 100,000,000 bytes
		for (kCOUNTER = 0; kCOUNTER < byteCount / 100000000;) {
			vector<future<void>> EncryptTasks;
			bool DONE = false;
			for (unsigned long ind = 0; ind < ThreadsSupported; ind++) {
				EncryptDone[ind] = false;//Reset status of task
				vector<char> bytesRead;
				bytesRead.resize(100000000 / ThreadsSupported);
				ifs.read(&bytesRead[0], 100000000 / ThreadsSupported);
				EncryptTasks.push_back(async(EncyptDataNoPass, bytesRead, ind, seeds[kCOUNTER * 8 + ind]));
			}
			while (1) {
				DONE = true;
				for (unsigned long j = 0; j < ThreadsSupported; j++) {//Check status
					if (EncryptDone[j] == false) {
						DONE = false;
						break;
					}
				}
				if (DONE)
					break;
				this_thread::sleep_for(0.4s);
			}
			for (unsigned long x = 0; x < EncryptedData.size(); x++) {
				myfile << EncryptedData[x];
			}
			//Free memory
			EncryptedData.clear();
			EncryptedData.shrink_to_fit();
			EncryptedData.resize(ThreadsSupported);
			//Restore the numbers
			for (unsigned long x = 0; x < indexCounter.size(); x++) {
				indexCounter[x] = 0;
			}
			kCOUNTER++;
		}
		//Now encrypt the other bytes at last
		{
			unsigned long size = byteCount % 100000000;
			if (size != 0) {
				char * bRead = new char[size];
				ifs.read(bRead, size);
				VBMathsR random;
				random.setSeed(seeds[seeds.size() - 1]);
				string EncryptedDataTEMP;
				for (indexCounter[0] = 0; indexCounter[0] < size; indexCounter[0]++) {
					RandomNumber = ToInt32(random.Rnd() * 50000);
					NumberToWrite = ((unsigned char)bRead[indexCounter[0]] ^ Code) + NumToAdd + RandomNumber;
					EncryptedDataTEMP += to_string(NumberToWrite) + " ";
				}
				delete[] bRead;
				myfile << EncryptedDataTEMP;
			}
		}
		publishProggress = false;
		ifs.close();
		myfile.close();
		SetConsoleTitle(TEXT("Done"));
		double elapsedTime = static_cast<double>(clock() - start) / CLOCKS_PER_SEC;
		system("cls");
		cout << "100% Done";
		this_thread::sleep_for(2s);
		system("cls");
		this_thread::sleep_for(0.2s);
		cout << "Encryption Done!";
		cout << endl;
		cout << endl;
		this_thread::sleep_for(0.2s);
		cout << "Done in " << elapsedTime << " Seconds" << "\n";
		this_thread::sleep_for(0.6s);
		cout << endl;
		cout << endl;
		cout << "By Hirbod Behnam And Hacknet";
		cout << endl;
		this_thread::sleep_for(0.5s);
		cout << endl;
		cout << endl;
		cout << endl;
	}
	catch (exception ex) {
		throw exception(ex.what());
	}
	catch (...) {
		throw exception("An error happened.");
	}
}
void WithPass(const string FileName, const string Password, const bool noIP) {
	//Values
	unsigned int ThreadsSupported = thread::hardware_concurrency();
	int RandomNumber;
	signed char ErrorRange = -1;
	ifstream ifs(FileName, ios::binary | ios::ate);
	char c;
	unsigned long NumberToWrite;
	string PathRemoved = File::RemovePath(FileName);
	string FileNameWithoutEx = PathRemoved.substr(0, PathRemoved.find_last_of("."));
	string FileNameF;
	string To_Write = "#DEC_ENC::";
	string IPAddress;
	string ePass;
	string Header = "title Encrypting " + FileName;
	ofstream myfile;
	vector<long> seeds;
	//Title
	system(Header.c_str());
	Header = "";
	system("cls");
	//Valid File
	cout << "Validating File";
	if (!File::Exists(FileName)) throw exception("File does not exist.");
	this_thread::sleep_for(0.5s);
	cout << ".";
	if (File::FileSize(FileName) == 0) throw exception("File is empty");
	this_thread::sleep_for(0.5s);
	cout << ".";
	if (File::FileSize(FileName) > 500000000) throw exception("The file cannot be more than 500MB.");
	this_thread::sleep_for(0.3s);
	cout << "." << endl;
	//get IP
	this_thread::sleep_for(0.6s);
	cout << "Getting Data From Internet";
	if (!noIP) {
		std::future<void> fut1 = std::async(AddDots);
		addDot = true;
		IPAddress = getIP();
		addDot = false;
		if (IPAddress == "NA" || IPAddress == "") throw exception("Cannot connect to internet.");
	}
	else {
		cout << ".";
		this_thread::sleep_for(0.5s);
		cout << ".";
		this_thread::sleep_for(0.5s);
		cout << ".";
		IPAddress = "XXX.XXX.XXX.XXX";
	}
	cout << endl;
	this_thread::sleep_for(0.3s);
	cout << "Enter header of your file:" << endl;
	WORD s = Console::GetColor();
	Console::SetColor(14);
	getline(cin, Header);
	Console::SetColor(s);
	if (Header == "") throw exception("Header cannot be empty.");
	this_thread::sleep_for(0.5s);
	cout << "Enter password again:";
	Console::SetColor(14);
	do {
		if(ErrorRange >= 0) cout << "Wrong Password:" << endl;
		getline(cin, ePass);
		ErrorRange++;
		if (ErrorRange == 3) throw exception("Wrong Password.\nTerminating Program...");
	} while (ePass != Password);
	Console::SetColor(s);
	system("cls");
	this_thread::sleep_for(0.5s);
	//Main Encryption
	try {
		if (!File::Exists(File::getexepath() + "\\" + FileNameWithoutEx + ".dec") || File::FileSize(File::getexepath() + "\\" + FileNameWithoutEx + ".dec") == 0) {
			FileNameF = File::getexepath() + "\\" + FileNameWithoutEx + ".dec";
		}
		else {
			unsigned short j = 1;
			while (File::Exists(File::getexepath() + "\\" + FileNameWithoutEx + to_string(j) + ".dec") || File::FileSize(File::getexepath() + "\\" + FileNameWithoutEx + to_string(j) + ".dec") == 0) {
				j++;
			}
			FileNameF = File::getexepath() + "\\" + FileNameWithoutEx + to_string(j) + ".dec";
		}
		cout << "Starting Encrypting...";
		myfile.open(FileNameF);
		clock_t start = clock();
		//Encrypt Header
		for (unsigned int i = 0; i < Header.length(); i++) {
			RandomNumber = ToInt32(r.Rnd() * 50000);
			c = Header.at(i);
			NumberToWrite = ((int)c ^ Code) + NumToAdd + RandomNumber;
			To_Write.append(to_string(NumberToWrite) + " ");
		}
		To_Write.append("::");
		//Encrypt IP
		for (unsigned int i = 0; i < IPAddress.length(); i++) {
			RandomNumber = ToInt32(r.Rnd() * 50000);
			c = IPAddress.at(i);
			NumberToWrite = ((int)c ^ Code) + NumToAdd + RandomNumber;
			To_Write.append(to_string(NumberToWrite) + " ");
		}
		To_Write.append("::");
		//Encrypt FileName
		for (unsigned int i = 0; i < PathRemoved.length(); i++) {
			RandomNumber = ToInt32(r.Rnd() * 50000);
			c = PathRemoved.at(i);
			NumberToWrite = ((int)c ^ Code) + NumToAdd + RandomNumber;
			To_Write.append(to_string(NumberToWrite) + " ");
		}
		To_Write.append("::\n");
		myfile << To_Write;
		//Free resources
		PathRemoved.~basic_string();
		IPAddress.~basic_string();
		To_Write.~basic_string();
		FileNameF.~basic_string();
		Header.~basic_string();
		FileNameWithoutEx.~basic_string();
		ePass.~basic_string();
		//Password works
		long codeForPass = 0;
		for (unsigned int i = 0; i < Password.length(); i++) {
			c = Password.at(i);
			NumberToWrite = (int)c ^ 42;
			codeForPass += NumberToWrite;
		}
		r.Rnd(-1);
		r.Randomize(GetSeed(Password));
		//Generate Seeds
		system("cls");
		cout << "Generating Seeds...";
		byteCount = ifs.tellg();
		{
			//Fix threads supported
			if (ThreadsSupported == 0)
				ThreadsSupported = 1;
			//Fix odd threads
			while (!_HThreads::checkCorrectThreadNumber(ThreadsSupported)) {
				ThreadsSupported--;
			}
			//Seeds and other stuff
			EncryptDone.resize(ThreadsSupported);
			EncryptedData.resize(ThreadsSupported);
			long LastSeed = r.getSeed();
			seeds.push_back(LastSeed);
			for (unsigned long i = 0; i < byteCount / 100000000; i++) {
				for (unsigned long j = 0; j < ThreadsSupported; j++) {
					for (unsigned long k = 0; k < 100000000 / ThreadsSupported; k++) {
						LastSeed = (unsigned long long)(((LastSeed * 0x43fd43fd) + 0xc39ec3) & 0xffffff);
					}
					seeds.push_back(LastSeed);
				}
			}
			indexCounter.resize(ThreadsSupported);
		}
		//Encrypt Main
		//Read file 100,000,000 bytes at once
		system("cls");
		ifs.seekg(0, ios::beg);
		publishProggress = true;
		future<void> prog = async(PublishProccess);//Publish progress on new thread for speed
		//At first encrypt each 100,000,000 bytes
		for (kCOUNTER = 0; kCOUNTER < byteCount / 100000000;) {
			vector<future<void>> EncryptTasks;
			bool DONE = false;
			for (unsigned long ind = 0; ind < ThreadsSupported; ind++) {
				EncryptDone[ind] = false;//Reset status of task
				vector<char> bytesRead;
				bytesRead.resize(100000000 / ThreadsSupported);
				ifs.read(&bytesRead[0], 100000000 / ThreadsSupported);
				EncryptTasks.push_back(async(EncyptDataWithPass, bytesRead, ind, seeds[kCOUNTER * 8 + ind], codeForPass));
			}
			while (1) {
				DONE = true;
				for (unsigned long j = 0; j < ThreadsSupported; j++) {//Check status
					if (EncryptDone[j] == false) {
						DONE = false;
						break;
					}
				}
				if (DONE)
					break;
				this_thread::sleep_for(0.4s);
			}
			for (unsigned long x = 0; x < EncryptedData.size(); x++) {
				myfile << EncryptedData[x];
			}
			//Free memory
			EncryptedData.clear();
			EncryptedData.shrink_to_fit();
			EncryptedData.resize(ThreadsSupported);
			//Refresh Numbers
			for (unsigned long x = 0; x < indexCounter.size(); x++) {
				indexCounter[x] = 0;
			}
			kCOUNTER++;
		}
		//Now encrypt the other bytes at last
		{
			unsigned long size = byteCount % 100000000;
			if (size != 0) {
				char * bRead = new char[size];
				ifs.read(bRead, size);
				VBMathsR random;
				random.setSeed(seeds[seeds.size() - 1]);
				string EncryptedDataTEMP;
				for (unsigned long i = 0; i < size; i++) {
					RandomNumber = ToInt32(random.Rnd() * 50000);
					NumberToWrite = ((unsigned char)bRead[i] ^ Code) + NumToAdd + RandomNumber;
					EncryptedDataTEMP += to_string(NumberToWrite) + " ";
				}
				delete[] bRead;
				myfile << EncryptedDataTEMP;
			}
		}
		publishProggress = false;
		ifs.close();
		system("cls");
		myfile.close();
		SetConsoleTitle(TEXT("Done"));
		double elapsedTime = static_cast<double>(clock() - start) / CLOCKS_PER_SEC;
		system("cls");
		cout << "100% Done";
		this_thread::sleep_for(2s);
		system("cls");
		this_thread::sleep_for(0.2s);
		cout << "Encryption Done!";
		cout << endl;
		cout << endl;
		this_thread::sleep_for(0.2s);
		cout << "Done in " << elapsedTime << " Seconds" << endl;
		this_thread::sleep_for(0.2s);
		cout << endl;
		this_thread::sleep_for(0.4s);
		cout << endl;
		cout << "By Hirbod Behnam And Hacknet";
		cout << endl;
		this_thread::sleep_for(0.5s);
		cout << endl;
		cout << endl;
		cout << endl;
	}
	catch (exception ex) {
		throw exception(ex.what());
	}
	catch (...) {
		throw exception("An error happened.");
	}
}
void EncyptDataNoPass(const vector<char> bytes,const unsigned int index, const long StartSeed) {
	VBMathsR random;
	random.setSeed(StartSeed);
	long RandomNumber;
	unsigned long NumberToWrite;
	string TEMP;
	for (indexCounter[index] = 0; indexCounter[index] < bytes.size(); indexCounter[index]++) {
		RandomNumber = ToInt32(random.Rnd() * 50000);
		NumberToWrite = ((unsigned char)bytes[indexCounter[index]] ^ Code) + NumToAdd + RandomNumber;
		TEMP += to_string(NumberToWrite) + " ";
	}
	EncryptedData[index] = TEMP;
	TEMP.~basic_string();
	bytes.~vector();
	EncryptDone[index] = true;
}
void EncyptDataWithPass(const vector<char> bytes, const unsigned int index, const long StartSeed, const long codeForPass) {
	VBMathsR random;
	random.setSeed(StartSeed);
	long RandomNumber;
	unsigned long NumberToWrite;
	string TEMP;
	for (indexCounter[index] = 0; indexCounter[index] < bytes.size(); indexCounter[index]++) {
		RandomNumber = ToInt32(random.Rnd() * 50000);
		NumberToWrite = ((unsigned char)bytes[indexCounter[index]] ^ Code) + NumToAdd + RandomNumber;
		TEMP += to_string(NumberToWrite) + " ";
	}
	EncryptedData[index] = TEMP;
	TEMP.~basic_string();
	bytes.~vector();
	EncryptDone[index] = true;
}
void Help() {
	cout <<  "Encrypher.exe FileName [Password]" << endl;
	cout << "  FileName: The file name with the format." << endl;
	cout << "  Password: The password for encryption.(Cannot be recovred)";
	cout << endl;
	cout << endl;
	cout << endl;
}
void Wrong_Args() {
	WORD s = Console::GetColor();
	Console::SetColor(12);
	cout << "Please enter right args to use the app. Example:" << endl;
	Console::SetColor(s);
	Help();
}
void AddDots() {
	while (addDot) {
		cout << ".";
		this_thread::sleep_for(0.5s);
	}
}
void PublishProccess() {
	unsigned long var;
	const unsigned int TO = indexCounter.size();
	while (1) {
		var = indexCounter[0];
		for (unsigned int i = 1; i < TO; i++) {
			var += indexCounter[i];
		}
		system("cls");
		cout << int((kCOUNTER * 100000000 + var) * 100.0 / byteCount) <<"% Done";
		this_thread::sleep_for(0.2s);
		if (!publishProggress)
			return;
	}
}
long GetSeed(const string password) {
	unsigned char change = 0; //0:XOR 1:Sum 2:Different 3:Bitshift and Or
	long i = 0;
	for (unsigned int j = 0; j < password.length(); j++) {
		switch (change) {
		case 1:
			i += (int)password.at(j);
			break;
		case 2:
			i -= (int)password.at(j);
			break;
		case 3:
			i = i >> (int)password.at(j) | (int)password.at(j);
			break;
		case 4:
			i ^= (int)password.at(j);
			change = 1;
			break;
		case 0:
			i ^= (int)password.at(j);
			break;
		}
		change++;
	}
	return abs(i);
}