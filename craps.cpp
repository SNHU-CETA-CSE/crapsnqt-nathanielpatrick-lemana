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
    firstRoll { true },
    rollValue { 0 }

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
    //winsCountUI->setText(QString::fromStdString(std::to_string(winsCount)));

}

// Player asked for another roll of the dice.
void CrapsMainWindow::rollButtonClickedHandler() {
//void Craps::rollButtonClickedHandler() {
    printf("Roll button clicked\n");
    rollValue =  die1.roll() + die2.roll();
    if(firstRoll){
        // Play the game as if it was the first roll
        std::cout << "hello\n";
        firstRoll = false;
    }
    else {
        // Play the game if one of first roll values is eligible for a second roll
        std::cout << "goodbye\n";
        firstRoll = true;
    }
    printStringRep();
    currentBankValue -= 100;
    winsCount += 1;
    updateUI();
}
