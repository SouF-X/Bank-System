// Bank 1 Project

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <iomanip>

using namespace std;

const string ClientsFileName = "Clients.txt";

void ShowMainMenue();

struct sClient
{
	string AccountNumber;
	string Name;
	string Phone;
	string PinCode;
	double Balance;
	bool MarkForDelete = false;
};


// End Building Menu & End Bar

// ------------------------------------------------------------------------------------------------------------------- 

// Start Common Functions // 

vector <string> Split(string S1, string Delim)
{
	vector <string> vString;
	short pos = 0;
	string sWord;

	while ((pos = S1.find(Delim)) != std::string::npos)
	{
		sWord = S1.substr(0, pos);
		if (sWord != "")
		{
			vString.push_back(sWord);
		}

		S1.erase(0, pos + Delim.length());
	}

	if (S1 != "")
	{
		vString.push_back(S1);
	}

	return vString;
}

sClient ConvertLineDataToRecord(string Line, string Seperator = "#//#")
{
	sClient ClientData;
	vector <string> vString = Split(Line, Seperator);


	ClientData.AccountNumber = vString[0];
	ClientData.PinCode = vString[1];
	ClientData.Name = vString[2];
	ClientData.Phone = vString[3];
	ClientData.Balance = stod(vString[4]);

	return ClientData;
}

vector <sClient> LoadClientDataFromFile(string FileName)
{
	vector <sClient> vClients;

	fstream MyFile;
	MyFile.open(FileName, ios::in);

	if (MyFile.is_open())
	{
		string Line;
		sClient Client;

		while (getline(MyFile, Line))
		{
			Client = ConvertLineDataToRecord(Line);
			vClients.push_back(Client);
		}

		MyFile.close();

	}

	return vClients;
}

string ConvertRecordToLine(sClient Client, string Seperator = "#//#")
{
	string stClientRecord = "";

	stClientRecord += Client.AccountNumber + Seperator;
	stClientRecord += Client.PinCode + Seperator;
	stClientRecord += Client.Name + Seperator;
	stClientRecord += Client.Phone + Seperator;
	stClientRecord += to_string(Client.Balance) + Seperator;

	return stClientRecord;
}

void PrintClientCard(sClient Client)
{
	cout << "\n\nThe following are the client details:";
	cout << "\n------------------------------------\n";
	cout << "\nAccout Number   : " << Client.AccountNumber;
	cout << "\nPin Code        : " << Client.PinCode;
	cout << "\nName            : " << Client.Name;
	cout << "\nPhone           : " << Client.Phone;
	cout << "\nAccount Balance : " << Client.Balance;
	cout << "\n------------------------------------";
}

bool FindClientByAccountNumber(string AccountNumber, vector <sClient> vClients, sClient& Client)
{
	for (sClient C : vClients)
	{
		if (C.AccountNumber == AccountNumber)
		{
			Client = C;
			return true;
		}
	}

	return false;
}

string ReadClientAccountNumber()
{
	string Number;
	cout << "Please enter AccountNumber? ";
	cin >> Number;
	return Number;
}

// End Common Functions //

// -------------------------------------------------------------------------------------------------------------------

// Start Adding New Clients To File //

bool ClientExistsByAccountNumber(string AccountNumber, string FileName)
{
	vector <sClient> vClients;
	fstream MyFile;

	MyFile.open(FileName, ios::in);

	if (MyFile.is_open())
	{
		string Line;
		sClient Client;

		while (getline(MyFile, Line))
		{
			Client = ConvertLineDataToRecord(Line);
			if (Client.AccountNumber == AccountNumber)
			{
				MyFile.close();
				return true;
			}
			vClients.push_back(Client);
		}

		MyFile.close();
	}

	return false;
}

sClient ReadNewClient()
{
	sClient Client;

	cout << "Enter Account Number? ";

	getline(cin >> ws, Client.AccountNumber);

	while (ClientExistsByAccountNumber(Client.AccountNumber, ClientsFileName))
	{
		cout << "\nClient with [" << Client.AccountNumber << "] alreadt exists, Enter another Account Number? ";
		getline(cin >> ws, Client.AccountNumber);
	}

	cout << "Enter PinCode? ";
	getline(cin, Client.PinCode);

	cout << "Enter Name? ";
	getline(cin, Client.Name);

	cout << "Enter Phone? ";
	getline(cin, Client.Phone);

	cout << "Enter Account Balance? ";
	cin >> Client.Balance;

	return Client;
};

void AddDataLineToFile(string FileName, string stDataLine)
{
	fstream MyFile;

	MyFile.open(FileName, ios::out | ios::app);

	if (MyFile.is_open())
	{
		MyFile << stDataLine << endl;
		MyFile.close();
	}
}

void AddNewClient()
{
	sClient Client;
	Client = ReadNewClient();
	AddDataLineToFile(ClientsFileName, ConvertRecordToLine(Client));
}

void AddNewClients()
{
	char AddMore = 'Y';

	do
	{
		cout << "Adding new Client\n\n";

		AddNewClient();

		cout << "\n\nClient Added Successfully, do you want to add more client? (Y/N) ? ";
		cin >> AddMore;


	} while (toupper(AddMore) == 'Y');
}

// End Adding New Clients To File //

// -------------------------------------------------------------------------------------------------------------------

// Start Showing All Clients //

void PrintClientCardLine(sClient Client)
{
	cout << "| " << setw(15) << left << Client.AccountNumber;
	cout << "| " << setw(10) << left << Client.PinCode;
	cout << "| " << setw(40) << left << Client.Name;
	cout << "| " << setw(12) << left << Client.Phone;
	cout << "| " << setw(12) << left << Client.Balance;
}

void ShowAllClientsScreen ()
{
	vector <sClient> vClients = LoadClientDataFromFile(ClientsFileName);

	cout << "\n\t\t\t\t\tClient List (" << vClients.size() << ") Client(s).";
	cout << "\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;

	cout << "| " << left << setw(15) << "Account Number";
	cout << "| " << left << setw(10) << "Pin Code";
	cout << "| " << left << setw(40) << "Client Name";
	cout << "| " << left << setw(12) << "Phone";
	cout << "| " << left << setw(12) << "Balance";

	cout << "\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;

	if (vClients.size() == 0)
		cout << "\t\t\t\tNo Clients Available In The System!";
	else

		for (sClient Client : vClients)
		{
			PrintClientCardLine(Client);
			cout << endl;
		}

	cout <<
		"\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;
}

// End Showing All Clients //

// -------------------------------------------------------------------------------------------------------------------

// Star Deleting Clients

bool MarkClientForDeleteByAccountNumber(string AccountNumber, vector <sClient>& vClients)
{
	for (sClient& C : vClients)
	{
		if (C.AccountNumber == AccountNumber)
		{
			C.MarkForDelete = true;
			return true;
		}
	}

	return false;
}

vector <sClient> SaveClientDataToFile(string FileName, vector <sClient> vClients)
{
	fstream MyFile;
	MyFile.open(FileName, ios::out);

	string DataLine;

	if (MyFile.is_open())
	{
		for (sClient C : vClients)
		{
			if (C.MarkForDelete == false)
			{
				DataLine = ConvertRecordToLine(C);
				MyFile << DataLine << endl;
			}
		}

		MyFile.close();
	}

	return vClients;
}

bool DeleteClientByAccountNumber(string AccountNumber, vector <sClient>& vClients)
{
	sClient Client;
	char Answer = 'n';

	if (FindClientByAccountNumber(AccountNumber, vClients, Client))
	{
		PrintClientCard(Client);

		cout << "\n\nAre you sure you want delete this client? y/n ? ";
		cin >> Answer;

		if (Answer == 'y' || Answer == 'Y')
		{
			MarkClientForDeleteByAccountNumber(AccountNumber, vClients);
			SaveClientDataToFile(ClientsFileName, vClients);

			vClients = LoadClientDataFromFile(ClientsFileName);

			cout << "\n\nClient Deleted Successfully.\n";
			return true;
		}
	}
	else
	{
		cout << "\nClient with Accoutn Number (" << AccountNumber << ") is Not Found!\n";
		return false;
	}
}

// End Deleting Clients

// -------------------------------------------------------------------------------------------------------------------

// Start Updating Clients

sClient ChangeClientRecord(string AccountNumber)
{
	sClient ClientData;

	ClientData.AccountNumber = AccountNumber;

	cout << "Enter PinCode? ";
	getline(cin >> ws, ClientData.PinCode);

	cout << "Enter Name? ";
	getline(cin, ClientData.Name);

	cout << "Enter Phone? ";
	getline(cin, ClientData.Phone);

	cout << "Enter Account Balance? ";
	cin >> ClientData.Balance;

	return ClientData;
}

bool UpdateClientByAccountNumber(string AccountNumber, vector <sClient>& vClients)
{
	sClient Client;
	char Answer = 'n';

	if (FindClientByAccountNumber(AccountNumber, vClients, Client))
	{
		PrintClientCard(Client);

		cout << "\n\nAre you sure you want update this client? y/n ? ";
		cin >> Answer;

		if (Answer == 'y' || Answer == 'Y')
		{
			for (sClient& C : vClients)
			{
				if (C.AccountNumber == AccountNumber)
				{
					C = ChangeClientRecord(AccountNumber);
					break;
				}
			}
			SaveClientDataToFile(ClientsFileName, vClients);

			cout << "\n\nClient Updated Successfully.\n";
			return true;
		}
	}
	else
	{
		cout << "\nClient with Accoutn Number (" << AccountNumber << ") is Not Found!\n";
		return false;
	}
}


// End Updating Clients

// -------------------------------------------------------------------------------------------------------------------

// Start Finding Clients

void FindClientMenuBar()
{
	system("cls");
	cout << "\n------------------------------------\n";
	cout << "\tFind Clients Screen";
	cout << "\n------------------------------------\n";
}

void FindingClient(string AccountNumber, vector <sClient> vClients)
{
	sClient Client;

	if (FindClientByAccountNumber(AccountNumber, vClients, Client))
	{
		PrintClientCard(Client);
	}
	else
	{
		cout << "\nClient with Accoutn Number (" << AccountNumber << ") is Not Found!\n";
	}

	cout << "\n\nPress any key to go back to Main Menue...";
	system("pause>0");
}

// End Finding Clients

// -------------------------------------------------------------------------------------------------------------------

// Start Showing CRUD

void ShowDeleteClientScreen()
{
	cout << "\n------------------------------------\n";
	cout << "\tDelete Clients Screen";
	cout << "\n------------------------------------\n";

	vector <sClient> vClients = LoadClientDataFromFile(ClientsFileName);
	string AccountNumber = ReadClientAccountNumber();
	DeleteClientByAccountNumber(AccountNumber, vClients);
}

void ShowUpdateClientScreen()
{
	cout << "\n------------------------------------\n";
	cout << "\tUpdate Clients Screen";
	cout << "\n------------------------------------\n";

	vector <sClient> vClients = LoadClientDataFromFile(ClientsFileName);
	string AccountNumber = ReadClientAccountNumber();
	UpdateClientByAccountNumber(AccountNumber, vClients);
}

void ShowAddNewClientsScreen()
{
	cout << "\n------------------------------------\n";
	cout << "\tAdd New Clients Screen";
	cout << "\n------------------------------------\n";

	AddNewClients();
}

void ShowFindClientScreen()
{
	cout << "\n------------------------------------\n";
	cout << "\tFind Clients Screen";
	cout << "\n------------------------------------\n";

	vector <sClient> vClients = LoadClientDataFromFile(ClientsFileName);
	sClient Client;
	string AccountNumber = ReadClientAccountNumber();

	if (FindClientByAccountNumber(AccountNumber, vClients, Client))
		PrintClientCard(Client);
	else
		cout << "\nClient with Account Number [" << AccountNumber << "] is not found!";
}

void ShowEndScreen()
{
	cout << "\n------------------------------------\n";
	cout << "\tProgram Ends :-)";
	cout << "\n------------------------------------\n";
}

// End Showing CRUD

// -------------------------------------------------------------------------------------------------------------------

// Start Main Menue

enum enMainMenueOptions
{
	eListClients = 1, eAddNewClient = 2,
	eDeleteClient = 3, eUpdateClient = 4,
	eFindClient = 5, eExit = 6
};

void GoBackToMainMenue()
{
	cout << "\n\nPress any key to go bak to Main Menue...";
	system("pause>0");
	ShowMainMenue();
}

short ReadMainMenueOption()
{
	cout << "Choose what do you want to do? [1 to 6]? ";
	short Choice = 0;
	cin >> Choice;
	return Choice;
}

void PerformMainMenueOption(enMainMenueOptions MainMenueOptions)
{
	switch (MainMenueOptions)
	{
	case enMainMenueOptions::eListClients:
	{
		system("cls");
		ShowAllClientsScreen();
		GoBackToMainMenue();
		break;
	}
	case enMainMenueOptions::eAddNewClient:
	{
		system("cls");
		ShowAddNewClientsScreen();
		GoBackToMainMenue();
		break;
	}
	case enMainMenueOptions::eDeleteClient:
	{
		system("cls");
		ShowDeleteClientScreen();
		GoBackToMainMenue();
		break;
	}
	case enMainMenueOptions::eFindClient:
	{
		system("cls");
		ShowFindClientScreen();
		GoBackToMainMenue();
		break;
	}
	case enMainMenueOptions::eExit:
	{
		system("cls");
		ShowEndScreen();
		break;
	}
	}
}

void ShowMainMenue()
{
	system("cls");
	cout << "===========================================\n";
	cout << "\t\tMain Menue Screen\n";
	cout << "===========================================\n";
	cout << "\t[1] Show Client List.\n";
	cout << "\t[2] Add New Client.\n";
	cout << "\t[3] Delete Client.\n";
	cout << "\t[4] Update Client Info.\n";
	cout << "\t[5] Find Client.\n";
	cout << "\t[6] Exit.\n";
	cout << "===========================================\n";

	PerformMainMenueOption((enMainMenueOptions)ReadMainMenueOption());

}

// End Main Menue

// -------------------------------------------------------------------------------------------------------------------

int main()
{
	ShowMainMenue();

	return 0;
}