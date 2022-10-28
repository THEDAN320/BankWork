#include <iostream>
#include <fstream>
#include <ctime>
#include <map>
#include <limits>
using namespace std;

//getting User id and card status
int id ,StatusCard;
void getid(){
	ifstream idFile("id.txt");
	idFile>>id>>StatusCard; 
	idFile.close();
}

//create mini BD to balance of all users and get User balance
map <int, int> AllUsersBalance;
int UserBalance;
void getbalance(){
    ifstream balance("balance.txt");
	while(!balance.eof()){
		int Allid, AllBalance;
		balance>>Allid>>AllBalance;
		AllUsersBalance[Allid] = AllBalance;
	}
	balance.close();
	UserBalance = AllUsersBalance[id];
}

//Save operation to log
void SaveOperation(string OperationType,int UserBalance){
    ofstream operation("operation.txt",ios::app);
    time_t seconds = time(NULL);
    tm* timeinfo = localtime(&seconds);
    operation<<id<<"| "<<OperationType<<"| "<<UserBalance<<"| "<<asctime(timeinfo);
    operation.close();
}

//create func to balance operation
void OperationWithBalance(int id,int UserBalance){
    AllUsersBalance[id] = UserBalance;
    ofstream BalanceFile("balance.txt");
    BalanceFile<<"";
    BalanceFile.close();
    map<int, int> ::iterator it;
    ofstream BalanceFile1("balance.txt",ios::app);
    
    for (it = AllUsersBalance.begin(); it != AllUsersBalance.end(); ++it){
    BalanceFile1<<it->first<<" "<<it->second<<"\n";
	} BalanceFile1.close();
}

//Enter to personal menu func
int EnterInPersonalMenu(){ 
    if(StatusCard == 0){
        printf("You card has been locked\n");
        return 1;
    }
    
    int EnterUserPin,UserPin,AttempLimit = 3;
    ifstream PinFile("pins.txt");
    PinFile>>UserPin;
    PinFile.close();
    printf("Enter you pin - ");
    
    while(!(cin>>EnterUserPin) or cin.get() != '\n' or EnterUserPin!=UserPin) {
        AttempLimit-=1;
        if(AttempLimit == 0) return 1;
        printf("Invalid input! Attemps left - %d\n",AttempLimit);
        cin.clear();
        cin.sync();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout<<"Enter you pin - ";
    }
    if(EnterUserPin == UserPin);return 0;
}

//Choice User func
int UserChoiceFunc(){
    int UserChoiceInFunc;
    printf("\nUp the balance[1]\nWithdraw money[2]\nPay be phone[3]\nTransfer money[4]\nbalans[5]\nOperation[6]\nExit[7]\n\nSelect operation - ");
    
    while(!(cin>>UserChoiceInFunc) or cin.get() != '\n' or UserChoiceInFunc < 1 or UserChoiceInFunc > 7) {
	        printf("\nInvalid input!\n");
            cin.clear();
            cin.sync();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout<<"\nUp the balance[1]\nWithdraw money[2]\nPay be phone[3]\nTransfer money[4]\nbalans[5]\nOperation[6]\nExit[7]\n\nSelect operation - ";
	}
	cout<<"\n";
	return UserChoiceInFunc;
}

//Up balance func
void UpTheBalance(){
    int UserDeposit;
    string OperationType = "UpTheBalance";
    printf("\nexit[0]\nbalance - %d\nHow much do you want to put into the account? - ",UserBalance);
    
    while(!(cin>>UserDeposit) or cin.get() != '\n' or UserDeposit < 0) {
        printf("Invalid input!\n\n");
        cin.clear();
        cin.sync();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout<<"\nexit[0]\nbalance - %d\nHow much do you want to put into the account? - ",UserBalance;
    } if(UserDeposit == 0) {return;}
    
    UserBalance += UserDeposit;
    OperationWithBalance(id,UserBalance);
    printf("Successful!\n");
    SaveOperation(OperationType,UserDeposit);
}

//Width draw money func
void WithdrawMoney(){
    int UserDeposit;
    string OperationType = "WithdrawMoney";
    printf("\nexit[0]\nbalance - %d\nHow much do you want to withdraw? - ",UserBalance);
    
    while(!(cin>>UserDeposit) or cin.get() != '\n' or UserDeposit < 0 or UserBalance - UserDeposit < 0) {
        printf("Invalid input!\n\n");
        cin.clear();
        cin.sync();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        printf("\nexit[0]\nbalance - %d\nHow much do you want to withdraw? - ",UserBalance);
    } if(UserDeposit == 0) {return;}
    
    UserBalance -= UserDeposit;
    OperationWithBalance(id,UserBalance);
    printf("Successful!\n");
    SaveOperation(OperationType,UserDeposit);
}

//Pay be phone func
void PayBePhone(){
    string OperationType = "PayBePhone";
    int UserDeposit,phone;
    
    printf("\nexit[0]\nEnter phone number - ");
    while(!(cin>>phone) or cin.get() != '\n' or phone < 0) { //void
        printf("Invalid phone number!\n");
        cin.clear();
        cin.sync();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout<<"\nexit[0]\nEnter phone number - ";
    }if(phone == 0) {return;}
    
    printf("\nexit[0]\nbalance - %d\npayment amount? - ",UserBalance);
    while(!(cin>>UserDeposit) or cin.get() != '\n' or UserDeposit < 0 or UserBalance - UserDeposit < 0) {
        printf("Invalid input!\n\n");
        cin.clear();
        cin.sync();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        printf("\nexit[0]\nbalance - %d\npayment amount? - ",UserBalance);
    }if(UserDeposit == 0) {return;}
    
    UserBalance -= UserDeposit;
    OperationWithBalance(id,UserBalance);
    printf("Successful!\n");
    SaveOperation(OperationType,UserDeposit);
}

//Transfer money func
void transferMoney(){
    string OperationType = "transferMoney";
    int UserDeposit, OtherPersonsid,Allid;
    map<int, int> ::iterator it;
    
    while(true){
        printf("\nexit[0]\nEnter the account id to which you want to make the transfer? - ");
        while(!(cin>>OtherPersonsid) or cin.get() != '\n' or OtherPersonsid < 0) { //void
            printf("Invalid id!\n");
            cin.clear();
            cin.sync();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout<<"\nexit[0]\nEnter the account id to which you want to make the transfer? - ";
        } if(OtherPersonsid == 0) {return;}
        
            if(OtherPersonsid == id) {printf("you can't send money to yourself!\n");}
            else if(AllUsersBalance.find(OtherPersonsid) == AllUsersBalance.end()) {printf("there is no such id!\n");}
            else{break;}
    }
    
    printf("\nexit[0]\nbalance - %d\npayment amount? - ",UserBalance);
    while(!(cin>>UserDeposit) or cin.get() != '\n' or UserDeposit < 0 or UserBalance - UserDeposit < 0) {
        printf("Invalid input!\n\n");
        cin.clear();
        cin.sync();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        printf("\nexit[0]\nbalance - %d\npayment amount? - ",UserBalance);
    }if(UserDeposit == 0) {return;}
    
    AllUsersBalance[OtherPersonsid] += UserDeposit;
	UserBalance -= UserDeposit;
	OperationWithBalance(id,UserBalance);
	printf("Successful!\n");
	SaveOperation(OperationType,UserDeposit);
    
}

//Check balance func
void CheckBalance(){
    printf("Your balance - %d\n",UserBalance);
}

//Check operation func
void CheckOperations(){
    int SizeBuffer = 100;
    char* buffer = new char[SizeBuffer+1];
    buffer[SizeBuffer]=0;
    
    ifstream operation("operation.txt");
    while(!operation.eof()){
        operation.getline(buffer,SizeBuffer,'\n');
        cout<<buffer<<"\n";
    }
    operation.close();
}

//Close card func
void CloseCard(){
	StatusCard = 0;
	ofstream idFile("id.txt");
	idFile<<id<<" "<<StatusCard; 
	idFile.close();
	cout<<"Card has been locked!\n";
}

//beggin programm
int main(){
	getid();
    int PermissionToEnter = EnterInPersonalMenu();
    int UserChoice;
    if(PermissionToEnter == 1){
	printf("number of attempts exceeded! no entry!\n");
	CloseCard();
	return 1;
    }
    
    else if(PermissionToEnter == 0){
	    printf("Successful entry!\n\n");
	    getbalance();
	
	    while(true){
	        UserChoice = UserChoiceFunc();
	        if(UserChoice == 1) {UpTheBalance();}
	        else if(UserChoice == 2) {WithdrawMoney();}
	        else if(UserChoice == 3) {PayBePhone();}
	        else if(UserChoice == 4) {transferMoney();}
	        else if(UserChoice == 5) {CheckBalance();}
	        else if(UserChoice == 6) {CheckOperations();}
	        else if(UserChoice == 7) {break;}
	    }
    }
}
