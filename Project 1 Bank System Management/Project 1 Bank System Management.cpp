// Project 1 Bank System Management.cpp 

#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include<cctype>
#include<limits>

using namespace std;
const string sClientFileName = "Clients.txt";
void ShowMainMenu();

struct stClient {
	string sAccountNumber;
	string sPinCode;
	string sName;
	string sPhoneNumber;
	double dAccountBalance;
	bool bMarkForDelete = false;
};

enum enMainMenu {
	eClientsList = 1,
	eAddNewClient = 2,
	eDeleteClient = 3,
	eUpdateClient = 4,
	eFindClient = 5,
	eExit = 6
};

short ReadMainMenuChoice() {
	cout << "Choose what do you want to do? [1 to 6]: ";
	short Choice = 0;
	while (!(cin >> Choice) || Choice <= 0 || !(Choice >= 1 && Choice <= 6)) {
		cin.clear();
		cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		cout << "Invalid choice!, Choose what do you want to do [1 to 6]: ";
	}
	return Choice;
}

char ReadYesNo(const string& TEXT) {
	char Answer;
	do {
		cout << endl << TEXT;
		cin >> Answer;
		cin.clear();
		cin.ignore(10000, '\n');
	} while (tolower(Answer) != 'y' && tolower(Answer) != 'n');
	return Answer;
}

double ReadNumber(const string& TEXT) {
	double Number = 0;
	cout << TEXT;
	while (!(cin >> Number) || Number <= 0) {
		cin.clear();
		cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		cout << "Invali input!, " + TEXT;
	}
	return Number;
}

string ReadString(const string& TEXT) {
	string MyString;
	cout << TEXT;
	getline(cin >> ws, MyString);
	return MyString;
}

vector <string> Split(string MyString, string Seperator = "#//#") {
	vector <string> veString;
	string sWord;
	short iPosition = 0;
	while ((iPosition = MyString.find(Seperator)) != std::string::npos) {
		sWord = MyString.substr(0, iPosition);
		if (sWord != "") veString.push_back(sWord);
		MyString.erase(0, iPosition + Seperator.length());
	}
	if (MyString != "") veString.push_back(MyString);
	return veString;
}

stClient ConvertLineClientToRecord(string sDataLine, string Seperator = "#//#") {
	vector <string> veClientData = Split(sDataLine, Seperator);
	stClient Client;
	Client.sAccountNumber = veClientData.at(0);
	Client.sPinCode = veClientData.at(1);
	Client.sName = veClientData.at(2);
	Client.sPhoneNumber = veClientData.at(3);
	Client.dAccountBalance = stod(veClientData.at(4));
	return Client;
}

bool isClientExits(string sFileName, string sAccountNumber) {
	vector <stClient> vecClients;
	fstream fMyFile;
	fMyFile.open(sFileName, ios::in);
	if (fMyFile.is_open()) {
		string Line;
		stClient Client;
		while (getline(fMyFile, Line)) {
			Client = ConvertLineClientToRecord(Line);
			if (Client.sAccountNumber == sAccountNumber) {
				fMyFile.close();
				return true;
			}
			vecClients.push_back(Client);
		}
		fMyFile.close();
	}
	return false;
}

stClient ReadNewClient() {
	stClient Client;
	Client.sAccountNumber = ReadString("Enter Account Number: ");
	while ((isClientExits(sClientFileName, Client.sAccountNumber))) {
		cout << "\nClient with account number [" << Client.sAccountNumber << "] already exits\n";
		Client.sAccountNumber = ReadString("Please enter another Account Number: ");
	}
	Client.sPinCode = ReadString("Enter Pin Code: ");
	Client.sName = ReadString("Enter Name: ");
	Client.sPhoneNumber = ReadString("Enter Phone Number: ");
	Client.dAccountBalance = ReadNumber("Enter Account Balance: ");
	return Client;
}

string Tabs(short NumberOfTabs) {
	string T = "";
	for (short i = 1; i <= NumberOfTabs; i++) {
		T += "\t";
	}
	return T;
}

string ConvertRecordClientToLine(stClient Client, string Seperator = "#//#") {
	string ClientRecord = "";
	ClientRecord += Client.sAccountNumber + Seperator;
	ClientRecord += Client.sPinCode + Seperator;
	ClientRecord += Client.sName + Seperator;
	ClientRecord += Client.sPhoneNumber + Seperator;
	ClientRecord += to_string(Client.dAccountBalance);
	return ClientRecord;
}

void SaveDataLineToFile(string sFileName, string sDataLine) {
	fstream fMyFile;
	fMyFile.open(sFileName, ios::out | ios::app);
	if (fMyFile.is_open()) {
		fMyFile << sDataLine << endl;
		fMyFile.close();
	}
}

void AddNewClient() {
	stClient Client = ReadNewClient();
	SaveDataLineToFile(sClientFileName, ConvertRecordClientToLine(Client));
}

void AddClients() {
	char AddMoreClients = 'y';
	do {
		//system("cls");
		cout << "Adding New Client:\n\n";
		AddNewClient();
		AddMoreClients = ReadYesNo("Do you to add more clients? ");
	} while (tolower(AddMoreClients) == 'y');
}

void ShowAddNewClientScreen() {
	cout << "\n" << string(50, '-') << "\n";
	cout << Tabs(1) << "Add New Client Screen";
	cout << "\n" << string(50, '-') << "\n\n";
	AddClients();
}

vector <stClient> LoadDataFromFileToVector(string sFileName) {
	vector <stClient> vClientData;
	fstream fMyFile;
	fMyFile.open(sFileName, ios::in);
	if (fMyFile.is_open()) {
		string Line;
		stClient Client;
		while (getline(fMyFile, Line)) {
			Client = ConvertLineClientToRecord(Line);
			vClientData.push_back(Client);
		}
		fMyFile.close();
	}
	return vClientData;
}

void PrintClientRecord(stClient Client) {
	cout << string(30, '-') << "\n";
	cout << "Account Number: " << Client.sAccountNumber;
	cout << "\nPin Code: " << Client.sPinCode;
	cout << "\nName: " << Client.sName;
	cout << "\nPhone Number: " << Client.sPhoneNumber;
	cout << "\nAccount Balance: " << Client.dAccountBalance;
	cout << "\n" << string(30, '-') << "\n";
}

void PrintAllClientsInfo(vector <stClient> vClients) {
	if (vClients.size() == 0)
		cout << "\nThere is no any client in this system!.\n";
	short ClientNumber = 0;
	cout << Tabs(5) << "Client List (" << vClients.size() << ") Clients.\n";
	cout << string(120, '_') << "\n";
	for (const stClient& Client : vClients) {
		ClientNumber++;
		cout << "\nClient [" << ClientNumber << "] Info: \n";
		PrintClientRecord(Client);
	}
}

void ShowClientListScreen() {
	vector <stClient> vClients = LoadDataFromFileToVector(sClientFileName);
	PrintAllClientsInfo(vClients);
}

bool FindClientByAccountNumber(string AccountNumber, vector <stClient> vClients, stClient& Client) {
	for (stClient Cl : vClients) {
		if (Cl.sAccountNumber == AccountNumber) {
			Client = Cl;
			return true;
		}
	}
	return false;
}

void ShowFindClientScreen() {
	cout << "\n" << string(50, '-') << "\n";
	cout << Tabs(1) << "Find Client Screen";
	cout << "\n" << string(50, '-') << "\n\n";
	stClient Client;
	vector <stClient> vClients = LoadDataFromFileToVector(sClientFileName);
	string AccountNumber = ReadString("Enter Account Number: ");
	if (FindClientByAccountNumber(AccountNumber, vClients, Client))
		PrintClientRecord(Client);
	else
		cout << "\n\nClient with Account Number (" << AccountNumber << ") Not Found!.\n";
}

bool AddMarkOnClientToDelete(string AccountNumber, vector <stClient>& vClients) {
	for (stClient& Client : vClients) {
		if (Client.sAccountNumber == AccountNumber) {
			Client.bMarkForDelete = true;
			return true;
		}
	}
	return false;
}

void SaveDataToFile(string sFileName, vector <stClient> vClients) {
	fstream fMyFile;
	fMyFile.open(sFileName, ios::out);
	if (fMyFile.is_open()) {
		for (stClient& C : vClients) {
			if (C.bMarkForDelete == false) {
				string DataLine = ConvertRecordClientToLine(C);
				fMyFile << DataLine << endl;
			}
		}
		fMyFile.close();
	}
}

bool DeleteClientByAccountNumber(string AccountNumber, vector <stClient>& vClients) {
	stClient Client;
	if (FindClientByAccountNumber(AccountNumber, vClients, Client)) {
		PrintClientRecord(Client);
		char cAnswer = ReadYesNo("\nAre you sure do you want to delete this client? ");
		if (tolower(cAnswer) == 'y') {
			AddMarkOnClientToDelete(AccountNumber, vClients);
			SaveDataToFile(sClientFileName, vClients);
			vClients = LoadDataFromFileToVector(sClientFileName);
			cout << "\n\nClient deleted.\n";
			return true;
		}
	}
	else {
		cout << "\n\nClient with Account Number (" << AccountNumber << ") Not Found!.\n";
		return false;
	}
}

void ShowDeleteClientScreen() {
	cout << "\n" << string(50, '-') << "\n";
	cout << Tabs(1) << "Delete Client Screen";
	cout << "\n" << string(50, '-') << "\n\n";
	string AccountNumber = ReadString("Enter Account Number: ");
	vector <stClient> vClients = LoadDataFromFileToVector(sClientFileName);
	DeleteClientByAccountNumber(AccountNumber, vClients);
}

stClient ChangeClientRecord(string AccountNumber) {
	stClient Client;
	Client.sAccountNumber = AccountNumber;
	Client.sPinCode = ReadString("\nEnter Pin Code: ");
	Client.sName = ReadString("Enter Name: ");
	Client.sPhoneNumber = ReadString("Enter Phone Number: ");
	Client.dAccountBalance = ReadNumber("Enter Account Balance: ");
	return Client;
}

bool UpdateClientByAccountNumber(string AccountNumber, vector <stClient>& vClients) {
	stClient Client;
	if (FindClientByAccountNumber(AccountNumber, vClients, Client)) {
		PrintClientRecord(Client);
		char cAnswer = ReadYesNo("Are you sure do you to update this client? ");
		if (tolower(cAnswer) == 'y') {
			for (stClient& Cl : vClients) {
				if (Cl.sAccountNumber == AccountNumber) {
					Cl = ChangeClientRecord(AccountNumber);
					break;
				}
			}
			SaveDataToFile(sClientFileName, vClients);
			cout << "\n\nClient Updated.\n";
			return true;
		}
	}
	else {
		cout << "\n\nClient with Account Number (" << AccountNumber << ") Not Found!.\n";
		return false;
	}
}

void ShowUpdateClientScreen() {
	cout << "\n" << string(50, '-') << "\n";
	cout << Tabs(1) << "Update Client Info Screen";
	cout << "\n" << string(50, '-') << "\n\n";
	string AccountNumber = ReadString("Enter Account Number: ");
	vector <stClient> vClients = LoadDataFromFileToVector(sClientFileName);
	UpdateClientByAccountNumber(AccountNumber, vClients);
}

void ShowEndProgramScreen() {
	cout << "\n" << string(50, '-') << "\n";
	cout << Tabs(1) << "(-: End Program :-)";
	cout << "\n" << string(50, '-') << "\n\n";
}

void GoBackToMainMenu() {
	cout << "\n\nPress enter to go back to Main Menu";
	system("pause>=0");
	ShowMainMenu();
}

void ChooseMainMenuChoice(enMainMenu MainChoice) {
	switch (MainChoice) {
	case enMainMenu::eClientsList: {
		system("cls");
		ShowClientListScreen();
		GoBackToMainMenu();
		break;
	}
	case enMainMenu::eAddNewClient: {
		system("cls");
		ShowAddNewClientScreen();
		GoBackToMainMenu();
		break;
	}
	case enMainMenu::eDeleteClient: {
		system("cls");
		ShowDeleteClientScreen();
		GoBackToMainMenu();
		break;
	}
	case enMainMenu::eUpdateClient: {
		system("cls");
		ShowUpdateClientScreen();
		GoBackToMainMenu();
		break;
	}
	case enMainMenu::eFindClient: {
		system("cls");
		ShowFindClientScreen();
		GoBackToMainMenu();
		break;
	}
	case enMainMenu::eExit: {
		system("cls");
		ShowEndProgramScreen();
		break;
	}
	}
}

void ShowMainMenu() {
	system("cls");
	cout << string(50, '=') << "\n";
	cout << Tabs(2) << "Main Menu Screen\n";
	cout << string(50, '=') << "\n";
	cout << Tabs(2) << "[1] Show Client List.\n";
	cout << Tabs(2) << "[2] Add New Client.\n";
	cout << Tabs(2) << "[3] Delete Client.\n";
	cout << Tabs(2) << "[4] Update Client Info.\n";
	cout << Tabs(2) << "[5] Find Clinet.\n";
	cout << Tabs(2) << "[6] Exit.\n";
	cout << string(50, '=') << "\n\n";
	ChooseMainMenuChoice(static_cast<enMainMenu>(ReadMainMenuChoice()));
}

int main() {
	ShowMainMenu();
	system("pause>=0");
}