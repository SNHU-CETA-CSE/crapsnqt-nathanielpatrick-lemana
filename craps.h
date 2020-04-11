//
// Created by Arana Fireheart on 2/2/20.
//

#ifndef CRAPSSTARTER_CRAPS_H
#define CRAPSSTARTER_CRAPS_H

#include "ui_CrapsMainWindow.h"
#include "die.h"
#include <QMainWindow>

class CrapsMainWindow : public QMainWindow, private Ui::CrapsMainWindow {
Q_OBJECT

public:
    CrapsMainWindow(QMainWindow *parent = nullptr);
    void printStringRep();
    void updateUI();

private:
    Die die1, die2;
    bool firstRoll = true;
    int winsCount;
    int lossCount;
    float currentBankValue;
    int rollValue;

public Q_SLOTS:
    void rollButtonClickedHandler();

};
//#include "moc_craps.cpp"
#endif //CRAPSSTARTER_CRAPS_H

