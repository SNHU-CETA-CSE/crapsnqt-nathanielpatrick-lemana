#include <iostream>
#include <stdio.h>
//#include <QApplication>
//#include <QWidget>
//#include <QGridLayout>
//#include <QPushButton>
//#include <QLabel>
//#include <QPixmap>

#include "die.h"
#include "craps.h"
#include "ui_CrapsMainWindow.h"
#include <sstream>

CrapsMainWindow :: CrapsMainWindow(QMainWindow *parent):
    // Build a GUI window with two dice.

    currentBankValue{ 10000 },
    winsCount { 0 },
    lossCount { 0 },
    firstRoll { true },
    rollValue { 0 },
    currentBet { 0 },
    previousRoll { 0 }

{
    setupUi(this);

    QObject::connect(rollButton, SIGNAL(clicked()), this, SLOT(rollButtonClickedHandler()));
}
void CrapsMainWindow::printStringRep() {
    // String representation for Craps.
    char buffer[25];
    int length =  sprintf(buffer, "Die1: %i\nDie2: %i\n", die1.getValue(), die2.getValue());
    printf("%s", buffer);
}
void CrapsMainWindow::updateUI() {
//    printf("Inside updateUI()\n");
    std::string die1ImageName = ":/dieImages/" + std::to_string(die1.getValue());
    std::string die2ImageName = ":/dieImages/" + std::to_string(die2.getValue());
    die1UI->setPixmap(QPixmap(QString::fromStdString(die1ImageName)));
    die2UI->setPixmap(QPixmap(QString::fromStdString(die2ImageName)));

    currentBankValueUI->setText(QString::fromStdString(std::to_string(currentBankValue)));
    winsCountUI->setText(QString::fromStdString(std::to_string(winsCount)));
    lossCountUI->setText(QString::fromStdString(std::to_string(winsCount)));


}

// Player asked for another roll of the dice.
void CrapsMainWindow::rollButtonClickedHandler() {
//void Craps::rollButtonClickedHandler() {
    bool rollCompleted = false;
    float localBank = currentBankValue;

    rollValue =  die1.roll() + die2.roll();
    if(firstRoll) {
        currentBet = processBet(currentBankValue);
        // Play the game as if it was the first roll
        std::cout << "This is the first roll\n";
        std::tie(rollCompleted, localBank) = playFirstRoll(rollValue, currentBankValue, currentBet);
        if (rollCompleted) {
            firstRoll = true;
            rollCompleted = false;
        } else {
            previousRoll = rollValue;
            firstRoll = false;
            rollCompleted = false;
        }
    } else {
        // Play the game if one of first roll values is eligible for a second roll
        std::cout << "This is the second roll\n";
        std::tie(rollCompleted, localBank) = playSecondRoll(rollValue, previousRoll, currentBankValue, currentBet);
        if (rollCompleted) {
            previousRoll = rollValue;
            firstRoll = true;
            rollCompleted = false;
        }
    }
    printStringRep();
    updateUI();
}

inline bool isInteger(const std::string & s) {
    if(s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+'))) return false;
    char * p;
    strtol(s.c_str(), &p, 10);
    return (*p == 0);
}

float CrapsMainWindow::processBet(float currentBank) {
    float attemptedBet;
    bool  betAccepted = false;

    std::string userInput = "";
    while (!betAccepted){
        std::cout << "How much would you like to bet? ";
        std::cin >> userInput;
        if(isInteger(userInput)) {
            attemptedBet = stof(userInput);
            if (attemptedBet <= currentBank) {
                betAccepted = true;
                return attemptedBet;
            } else {
                std::cout << "You don't have that much money!" << "\n";
                betAccepted = false;
            }
        } else
            betAccepted = false;
    }
};

std::tuple<bool, float>  CrapsMainWindow::playFirstRoll(int rollValue, float currentBank, float currentBet){
    std::cout << rollValue << "\n";
    switch (rollValue) {
        case 7:
        case 11: {
            currentBank = processWin(rollValue, 1, currentBank, currentBet);
            return std::make_tuple(true, currentBank);
        }
        case 2:
        case 3:
        case 12: {
            currentBank = processLoss(rollValue, 1, currentBank, currentBet);
            return std::make_tuple(true, currentBank);
        }
        default: {
            //rollingForUI->setText(QString::fromStdString(std::to_string(rollValue)));
            return std::make_tuple(false, currentBank);
        }
    }
};

std::tuple<bool, float>  CrapsMainWindow::playSecondRoll(int rollValue, int previousRoll,  float currentBank, float currentBet){
    if (rollValue == previousRoll) {
        return std::make_tuple(true, processWin(rollValue, 2, currentBank, currentBet));
    } else {
        return std::make_tuple(true, processLoss(rollValue, 2, currentBank, currentBet));
    }
};

float CrapsMainWindow::processWin(int rollValue, int rollNumber, float currentBank, float currentBet) {
    std::cout << "You won!" << "\n";
    return calculateCurrentBank(rollValue, rollNumber, currentBank, currentBet, true);
}

float CrapsMainWindow::processLoss(int rollValue, int rollNumber, float currentBank, float currentBet) {
    std::cout << "You lost." << "\n";
    return calculateCurrentBank(rollValue, rollNumber, currentBank, currentBet, false);
};

const float payouts[] = {0.0, 0.0, 1.0, 1.0, 2.0, 1.5, 1.2, 1.0, 1.2, 1.5, 2.0, 1.0, 1.0};

float CrapsMainWindow::calculateCurrentBank(int rollValue, int rollNumber, float currentBank, float currentBet, bool wonBet) {
    if (rollNumber == 1) {
        if(wonBet)
            return currentBank + currentBet;
        else
            return currentBank - currentBet;
    }
    else {
        if(wonBet)
            return currentBank + currentBet * payouts[rollValue];
        else
            return currentBank - currentBet * payouts[rollValue];
    }
};
