#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <iomanip>
#include <cstdio>

using namespace std;

const string clientsFileName = "Clients.txt";

struct stClient
{
	string accountNumber = "";
	string name = "";
	string id = "";
	string phoneNumber = "";
	string pincode = "";
	double balance = 0;
};

struct stClientInput
{
	string accountNumber;
	string pin;
};

enum enOptions
{
	QuickWithdraw = 1,
	NormalWithdraw = 2,
	Deposit = 3,
	CheckBalance = 4,
	Logout = 5
};

enum enWithdrawAmount
{
	twenty = 1,
	fifty = 2, 
	hundred = 3, 
	twoHundred = 4, 
	fiveHundred = 5, 
	oneThousand = 6,
	Exit = 7
};

stClient client;

vector<stClient> vClients;

void validationNumber(int& num, int from, int to, string msg)
{
	while (cin.fail() || num < from || num > to)
	{
		if (cin.fail())
		{
			cin.clear();
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}
		cout << msg;
		cin >> num;
	}
}

int readMainMenuOption()
{
	int tmp = 0;

	cout << "Choose what do you want to do: [1 to 5]? ";
	cin >> tmp;

	validationNumber(tmp, 1, 5, "\nInvalid number, please enter a valid number: ");

	return tmp;
}

void showAtmMainMenuScreen()
{
	system("cls");
	cout << "============================================\n";
	cout << right << setw(33) << "ATM Main Menu Screen" << endl;
	cout << "============================================\n";
	cout << "[1] Quick Withdraw.\n";
	cout << "[2] Normal Withdraw.\n";
	cout << "[3] Deposit.\n";
	cout << "[4] Check Balance.\n";
	cout << "[5] Logout.\n";
	cout << "============================================\n";
}

enOptions showMainMenuScreen()
{
	showAtmMainMenuScreen();
	return ((enOptions)readMainMenuOption());
}

//void backToMenuScreen()
//{
//	cout << endl << "Press any key to go back to main menu...";
//	system("pause>0");
//}

void showCheckbalanceScreen()
{
	cout << "--------------------------------------\n";
	cout << setw(30) << "Check Balance Screen\n";
	cout << "--------------------------------------" << endl;
}

void showBalance()
{
	cout << "\nYour balance: " << client.balance << endl;
	cout << "\n--------------------------------------\n";
}

void checkBalacnce()
{
	showCheckbalanceScreen();
	showBalance();
}

string tabs(int numberOfTabs)
{
	string tab = "";

	for (int i = 0; i < numberOfTabs; i++)
	{
		tab += "\t";
	}
	return tab;
}

void showQuickWithdrawScreen()
{
	cout << "============================================\n";
	cout << setw(30) << "Quick Withdraw\n";
	cout << "============================================\n";
	cout << "[1] 20" << tabs(2) << "[2] 50\n";
	cout << "[3] 100" << tabs(2) << "[4] 200\n";
	cout << "[5] 500" << tabs(2) << "[6] 1000\n";
	cout << "[7] Exit\n";
	cout << "============================================\n";
	cout << "Your balance: " << client.balance << endl;
	cout << "============================================\n";
}

int readWithdrawAmount()
{
	int tmp = 0;

	cout << "Choose the amount to withdraw: [1 to 7]? ";
	cin >> tmp;

	validationNumber(tmp, 1, 7, "\nInvalid number, please enter a valid number: ");

	return tmp;
}

enWithdrawAmount getWithdrawAmount()
{
	showQuickWithdrawScreen();
	return ((enWithdrawAmount)readWithdrawAmount());
}

void showNormalWithdrawScreen()
{
	cout << "============================================\n";
	cout << setw(34) << "Normal Withdraw Screen\n";
	cout << "============================================\n";
}

double readTheAmountOfWithdraw()
{
	double withdrawAmount = 0;
	cout << "Please enter amount to withdraw: ";
	cin >> withdrawAmount;
	
	while ((int(withdrawAmount) % 5) != 0)
	{
		cout << "\nInvalid amount, please enter another amount: ";
		cin >> withdrawAmount;
	}

	return withdrawAmount;
}

void checkIfAmountExceedsBalance(double& withdrawAmount)
{
	while (withdrawAmount > client.balance)
	{
		cout << "\nThe amount exceeds the balance, MAX[" << client.balance << "]: ";
		cin >> withdrawAmount;
	}
}

vector<string> vSplit(string str, string delim = "#//#")
{
	vector<string> split;
	short pos = 0;
	string word = "";

	while ((pos = str.find(delim)) != std::string::npos)
	{
		word = str.substr(0, pos);

		if (word != "")
		{
			split.push_back(word);
		}

		str = str.erase(0, pos + delim.length());
	}

	split.push_back(str);

	return split;
}

stClient convertLineToStruct(string line)
{
	vector<string> splitLine = vSplit(line);
	stClient data;

	data.accountNumber = splitLine[0];
	data.name = splitLine[1];
	data.id = splitLine[2];
	data.phoneNumber = splitLine[3];
	data.pincode = splitLine[4];
	data.balance = stod(splitLine[5]);

	return data;
}

string convertStructToLine(stClient clientData, string separator = "#//#")
{
	string str = "";

	str = clientData.accountNumber + separator + clientData.name + separator + clientData.id + separator + clientData.phoneNumber + separator + clientData.pincode + separator + to_string(clientData.balance);

	return str;
}

vector<stClient> loadClientsDataFromFileToVector(string fileName)
{
	fstream myFile;
	vector<stClient> vClients;
	string line = "";

	myFile.open(fileName, ios::in);

	if (myFile.is_open())
	{
		while (getline(myFile, line))
		{
			vClients.push_back(convertLineToStruct(line));
		}

		myFile.close();
	}

	return vClients;
}

void reloadDataFromVectorToFile(string fileName)
{
	fstream myFile;

	myFile.open(fileName, ios::out);

	if (myFile.is_open())
	{
		for (stClient& data : vClients)
		{
			if (data.accountNumber == client.accountNumber)
				data.balance = client.balance;
			myFile << convertStructToLine(data) << endl;
		}
		myFile.close();
	}
}

void performWithdraw(double amount)
{
	client.balance -= amount;
	cout << "\nDone successfully!\n";
	cout << "Your balance: " << client.balance << endl;

	reloadDataFromVectorToFile(clientsFileName);
}

void normalWithdraw()
{
	showNormalWithdrawScreen();
	double withdrawAmount = readTheAmountOfWithdraw();
	checkIfAmountExceedsBalance(withdrawAmount);
	cout << "\nAre you sure you want to perform this transaction? [y/n]? ";
	char check = 'n';
	cin >> check;
	if (tolower(check) == 'y')
		performWithdraw(withdrawAmount);
	//else..

}

bool check(stClientInput clientInput)
{
	for (stClient& data : vClients)
	{
		if (data.accountNumber == clientInput.accountNumber && data.pincode == clientInput.pin)
		{
			client = data;
			return true;
		}
	}

	return false;
}

void showLoginScreen()
{
	system("cls");
	cout << "--------------------------------------\n";
	cout << setw(27) << "Login Screen\n";
	cout << "--------------------------------------" << endl;
}

stClientInput getClientInput()
{
	stClientInput clientInput;

	cout << "Please enter username: ";
	getline(cin >> ws, clientInput.accountNumber);

	cout << "Please enter password: ";
	getline(cin >> ws, clientInput.pin);

	return clientInput;
}

void loginScreen(string fileName)
{
	showLoginScreen();

	stClientInput clientInput = getClientInput();

	while (!check(clientInput))
	{
		system("cls");
		showLoginScreen();
		cout << "Invalid username/password!\n";
		clientInput = getClientInput();
	}
}

void showDepositScreen()
{
	cout << "============================================\n";
	cout << right << setw(30) << "Deposit Screen" << endl;
	cout << "============================================\n";
}

double getDepositAmount()
{
	double depositAmount = 0;
	cout << "\nPlease enter deposit amount: ";
	cin >> depositAmount;

	return depositAmount;
}

void performDeposit(double depositAmount)
{
	client.balance += depositAmount;

	cout << "\nDone successfully!\n";
	cout << "Your balance: " << client.balance << endl;

	reloadDataFromVectorToFile(clientsFileName);
}

void deposit()
{
	showDepositScreen();

	double depositAmount = getDepositAmount();

	cout << "\nAre you sure you want to perform this transaction? [y/n]? ";
	char check = 'n';
	cin >> check;

	if (tolower(check) == 'y')
		performDeposit(depositAmount);
	//else
		//....

}

void quickWithdraw()
{
	enWithdrawAmount withdrawAmount = getWithdrawAmount();

	switch (withdrawAmount)
	{
	case enWithdrawAmount::twenty:
		performWithdraw(20);
		break;
	case enWithdrawAmount::fifty:
		performWithdraw(50);
		break;

	case enWithdrawAmount::hundred:
		performWithdraw(100);
		break;

	case enWithdrawAmount::twoHundred:
		performWithdraw(200);
		break;

	case enWithdrawAmount::fiveHundred:
		performWithdraw(500);
		break;

	case enWithdrawAmount::oneThousand:
		performWithdraw(1000);
		break;
	case enWithdrawAmount::Exit:
		return;
	}
}

void backToMenuScreen()
{
	cout << endl
		<< "Press any key to go back to main menu...";
	system("pause>0");
}

void startProgram()
{
	vClients = loadClientsDataFromFileToVector(clientsFileName);

	loginScreen(clientsFileName);

	while(true)
	{
		enOptions option = showMainMenuScreen();

		switch (option)
		{
		case enOptions::QuickWithdraw:
			system("cls");
			quickWithdraw();
			backToMenuScreen();
			break;
		case enOptions::NormalWithdraw:
			system("cls");
			normalWithdraw();
			backToMenuScreen();
			break;
		case enOptions::Deposit:
			system("cls");
			deposit();
			backToMenuScreen();

			break;
		case enOptions::CheckBalance:
			system("cls");
			checkBalacnce();
			backToMenuScreen();

			break;
		case enOptions::Logout:
			system("cls");
			startProgram();
		}
	}
}

int main()
{
	startProgram();
	return 0;
}