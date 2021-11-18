#include <iostream>
#include <iomanip>
#include <ctime>
#include <cstdlib>
#include <string>
using namespace std;

void displayMenu();
void dice();
void guessingGame();
void instructions();
void menu();
void UStoMEX(double MEXcurrency);
void MEXtoUS(double UScurrency);
void YENtoUS(double UScurrency);
void UStoYEN(double YENcurrency);
void exit();

int main() {

    displayMenu();

    return 0;
}

void displayMenu()
{
    int opt;
    int flag = -1;
    cout <<"\nHello User, Welcome" << endl;
    cout <<"1. Dice" << endl;
    cout <<"2. Casino Guessing game" << endl;
    cout <<"3. Currency Converter"  << endl;
    cout <<"4. Exit" << endl;
    cin >> opt;

    while(flag == -1){
        switch (opt)
        {
        case 1:
            dice();
            break;

        case 2:
            guessingGame();
            break;

        case 3:
            menu();
            break;

        case 4:
            exit(0);
            break;

        default:
            cout <<"This is not a valid option" << endl;
            break;
        }
    }
}

void dice()
{
    cout << " ---" << endl;
    cout << "| " << rand()% 6+1 << " |" << endl;
    cout << " ---" << endl;
    displayMenu();
}

void instructions()
{
    cout <<"\t\t============MORALES CASINO NUMBER GUESSING RULES===========\n";
    cout <<"\t1. Choose a number between 1 and 10"<<endl;
    cout <<"\t2. Winner will get 10 times of the money bet"<<endl;
    cout <<"\t3. Wrong number choice and you will lose the amount bet\n\n";
}


void guessingGame()
{

    string player_name; //stores players name
    int balance; //player balance
    int betting_ammount; //how much money is player risking
    int guess; //Players guess
    int dice; //Random number is stored here
    char choice; //used to restart game if wanted
    char quitting;
    srand(time(0)); //'seed' the random generator

    cout <<"\nWhat is your name?: ";
    getline(cin, player_name); //Used to get full names while also being able to accept spaces
    cout <<"\n\nEnter the balance you will like to start the game with: $";
    cin >> balance;

    do {
        system("cls"); //clears everything on screen
        instructions(); //goes to function instructions
        cout <<"\n\nYour current balance is: $" << balance<<endl;

        do{
            cout <<"Hey, " << player_name <<", enter amount to bet: $";
            cin >> betting_ammount;

        if(betting_ammount > balance)
            cout <<"Your betting balance cannot be greater than your current balance! \n"
                 <<" Please Re-enter amount to bet: $";
        }while(betting_ammount > balance);

    do{
        cout <<"Guess any betting number between 1 and 10: ";
        cin >> guess;

        if(guess <=0 || guess > 10)
            cout <<"Your guess should not be between 1 and 10 \n"
                 <<"Re-enter your guess";
    }while(guess <= 0 || guess > 10);
    dice = rand()%10 + 1;
    if(dice == guess){
        cout <<"\n\nCONGRATUALATIONS YOU HAVE WONE THIS ROUND!!!\n"
             <<" You have won: $"<<betting_ammount * 10;
        balance = balance + (betting_ammount * 10);
    }
    else{
        cout <<"Better luck next time. You lost: $" << betting_ammount <<endl;
        balance = balance - betting_ammount;
    }

        cout <<"The winning number was: " << dice <<endl;
        cout<< endl << player_name <<", You have a balance of: $" << balance <<endl;

        if(balance == 0){
            cout <<"You do not have any more money to play ";
            break;
        }

        cout <<"\n\n-->Do you want to play again (Y/N)?: ";
        cin >> choice;
    }while(choice =='Y' || choice == 'y');

    cout <<"\n\n";
    cout <<"\n\nThanks for playing at Morales Casino. Your total balance is: $"<< balance <<endl;
    cout <<"Do you wish to clear the screen (Y/N)?: ";
    cin >> quitting;
    if(quitting == 'y' || quitting == 'Y'){
        system("cls");
        displayMenu();
    }
    else
        displayMenu();
}





void menu()
{
    double money;

    int options;
    cout << "\n\nPlease pick an option" <<endl;
    cout << "1. Convert from US dollars to MEX pesos" <<endl;
    cout << "2. Convert from MEX pesos to US dollars" << endl;
    cout << "3. Convert from JPN yen to US dollars" << endl;
    cout << "4. Convert from US dollar to JPN yen" << endl;
    cout << "5. exit" <<endl;

    while(true){
    cin >> options;
    switch (options)
    {
    case 1:
        cout << "\n\nPut in the money you are going to be converting: ";
        cin >> money;
        UStoMEX(money);
        break;
    case 2:
        cout << "\n\nPut in the money you are going to be converting: ";
        cin >> money;
        MEXtoUS(money);
        break;
    case 3:
        cout << "\n\nPut in the amount you are going to be converting: ";
        cin >> money;
        YENtoUS(money);
        break;
    case 4:
        cout << "\n\nPut in the amount you are going to be converting: ";
        cin >> money;
        UStoYEN(money);
        break;
    case 5:
        system("cls");
        displayMenu();
        break;
    default:
        cout << "this is not a valid option. Please try again: ";
        break;
    }
    }
}

void UStoMEX(double MEXcurrency)
{
    double pesos;
    pesos = MEXcurrency * 19.96;
    cout << "The conversions is: "<< pesos << " pesos";
    menu();
}

void MEXtoUS(double UScurrency)
{
    double dollars;
    dollars = UScurrency/19.96;
    cout << "The conversion is: " << dollars << " dollars";
    menu();
}

void UStoYEN(double YENcurrency)
{
    float yen;
    yen = YENcurrency * 114.25;
    cout << "The conversions is: " << yen << " yen";
    menu();
}

void YENtoUS(double UScurrency)
{
    double dollars;
    dollars = UScurrency / 114.25;
    cout << "The conversions is: " << dollars << " dollars";
    menu();
}
