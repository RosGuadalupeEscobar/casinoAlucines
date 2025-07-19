#ifndef ROULETTE_H
#define ROULETTE_H

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <string>
#include <algorithm>
#include <limits>
#include <windows.h>
#include "Player_profiles.h"

using namespace std;

// Returns the color associated with a roulette number
inline string getColor(int number)
{
    int reds[] = {1, 3, 5, 7, 9, 12, 14, 16, 18, 19, 21, 23, 25, 27, 30, 32, 34, 36};
    if (number == 0)
        return "Green";
    for (int i = 0; i < 18; i++)
        if (number == reds[i])
            return "Red";
    return "Black";
}

// Converts a string to lowercase
inline string toLower(string str)
{
    transform(str.begin(), str.end(), str.begin(), ::tolower);
    return str;
}

// Validates the player's bet
inline int validateBet(int money)
{
    int bet;
    cin >> bet;

    if (cin.fail())
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input. Betting $1 by default.\n";
        return 1;
    }

    if (bet > money || bet < 1)
    {
        cout << "Invalid bet. Betting $1 by default.\n";
        return 1;
    }

    return bet;
}

// Main roulette game for 2 players
inline void playRoulette()
{
    srand((unsigned)time(nullptr));
    int rounds = 5;

    system("cls");
    cout << R"(

  _____   _    _ _      _      _       
 |  __ \ | |  | (_)    | |    | |      
 | |) || || |_ ___ | | __ | |_ ___ 
 |  _  / |  __  | / __|| |/ / | __/ _ \
 | | \ \ | |  | | \__ \|   <  | ||  __/
 ||  \\||  |||/|\\  \\_|

)" << "\n";

    cout << "===== ROULETTE =====\n";
    cout << "Starting balances:\n";
    cout << jugadores[0].name << ": $" << jugadores[0].balance << "\n";
    cout << jugadores[1].name << ": $" << jugadores[1].balance << "\n";
    cout << "Bet types:\n"
         << " 1. Exact number (0‑36) — payout 35×\n"
         << " 2. Color (Red/Black) — payout 2×\n"
         << " 3. Even/Odd — payout 2×\n";
    cout << "Press ENTER to begin...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();

    for (int round = 1; round <= rounds; round++)
    {
        if (jugadores[0].balance <= 500 && jugadores[1].balance <= 500)
            break;

        system("cls");
        cout << "ROUND " << round << "\n";

        int type[2], bet[2], guessNum[2] = {-1, -1};
        string guessStr[2];

        for (int i = 0; i < 2; i++)
        {
            if (jugadores[i].balance <= 100)
            {
                cout << jugadores[i].name << " is out of money.\n";
                continue;
            }

            cout << "\n"
                 << jugadores[i].name << "'s turn — Balance: $" << jugadores[i].balance << "\n";
            cout << "Choose bet type (1‑3): ";
            cin >> type[i];

            if (cin.fail() || type[i] < 1 || type[i] > 3)
            {
                cin.clear();
                cin.ignore(1000, '\n');
                cout << "Invalid choice. Skipping turn.\n";
                type[i] = 0;
                continue;
            }

            cout << "Enter bet amount: $";
            cin >> jugadores[i].bet;

            if (type[i] == 1)
            {
                cout << "Pick number (0‑36): ";
                cin >> guessNum[i];
                if (cin.fail() || guessNum[i] < 0 || guessNum[i] > 36)
                {
                    cin.clear();
                    cin.ignore(1000, '\n');
                    cout << "Invalid number. Skipping.\n";
                    type[i] = 0;
                }
            }
            else if (type[i] == 2)
            {
                cout << "Pick color (Red/Black): ";
                cin >> guessStr[i];
                guessStr[i] = toLower(guessStr[i]);
                if (guessStr[i] != "red" && guessStr[i] != "black")
                {
                    cout << "Invalid color. Skipping.\n";
                    type[i] = 0;
                }
            }
            else if (type[i] == 3)
            {
                cout << "Pick (Even/Odd): ";
                cin >> guessStr[i];
                guessStr[i] = toLower(guessStr[i]);
                if (guessStr[i] != "even" && guessStr[i] != "odd")
                {
                    cout << "Invalid choice. Skipping.\n";
                    type[i] = 0;
                }
            }
        }

        int result = rand() % 37;
        string color = getColor(result);
        cout << "\n>>> Wheel result: " << result << " (" << color << ")\n";

        for (int i = 0; i < 2; i++)
        {
            if (type[i] == 0 || jugadores[i].balance <= 0)
                continue;

            bool win = false;
            int payout = 0;

            if (type[i] == 1 && guessNum[i] == result)
            {
                win = true;
                payout = bet[i] * 35;
            }
            else if (type[i] == 2 && toLower(color) == guessStr[i])
            {
                win = true;
                payout = bet[i] * 2;
            }
            else if (type[i] == 3 && result != 0)
            {
                if ((guessStr[i] == "even" && result % 2 == 0) ||
                    (guessStr[i] == "odd" && result % 2 == 1))
                {
                    win = true;
                    payout = bet[i] * 2;
                }
            }

            if (win)
            {
                cout << jugadores[i].name << " won $" << payout << ".\n";
                jugadores[i].balance += payout;
                jugadores[i].wins++;
            }
            else
            {
                cout << jugadores[i].name << " lost $" << jugadores[i].bet << ".\n";
                jugadores[i].balance -= jugadores[i].bet;
            }
        }

        cout << "\nCurrent balances:\n";
        cout << jugadores[0].name << ": $" << jugadores[0].balance << " — Wins: " << jugadores[0].wins << "\n";
        cout << jugadores[1].name << ": $" << jugadores[1].balance << " — Wins: " << jugadores[1].wins << "\n";

        system("pause");
    }

    cout << "\n===== FINAL RESULT =====\n";
    if (jugadores[0].wins > jugadores[1].wins)
        cout << jugadores[0].name << " wins the game!\n";
    else if (jugadores[1].wins > jugadores[0].wins)
        cout << jugadores[1].name << " wins the game!\n";
    else
        cout << "It's a tie!\n";

    ofstream file("Roulette_scores.txt", ios::app);

    file << "=== ROULETTE ===\n";
    for (int i = 0; i < 2; i++)
    {
        file << "Player: " << jugadores[i].name << "\n";
        file << "Final Money: $" << jugadores[i].balance << "\n";
        file << "Rounds Won: " << jugadores[i].wins << "\n";
    }
    file << "---------------------------\n";
    file.close();

    cout << "Score saved.\n";
}

#endif // ROULETTE_H
