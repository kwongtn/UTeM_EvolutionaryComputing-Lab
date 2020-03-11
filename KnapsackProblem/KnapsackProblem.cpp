// KnapsackProblem.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <stdlib.h>
#include <ctime>
using namespace std;

const int GENE = 8;
const int WEIGHT[GENE] = { 45, 35, 25, 5, 25, 3, 2, 2 };
const int POPSIZE = 10;
const int TARGET = 104;

int chromosome[POPSIZE][GENE];
int fitness[POPSIZE];

void initializePopulation() {
    for(int i = 0; i < POPSIZE; i++){
        for(int j = 0; j < GENE; j++){
            chromosome[i][j] = rand() % 2;
        }
    }
}

void printChromosome() {
for(int i = 0; i < POPSIZE; i++){
    cout << "Chromosome " << i << "\t";
        for(int j = 0; j < GENE; j++){
            cout << chromosome[i][j] << "\t";
        }
    cout << "\n";
    }
}

void evaluateChromosome() {
    int accumulatedWeight = 0;
    for(int i = 0; i < POPSIZE; i++) {
        accumulatedWeight = 0;
        for(int j = 0; j < GENE; j++) {
            accumulatedWeight += chromosome[i][j];
        }

        fitness[i] = accumulatedWeight;
        cout << "Fitness value of chromosome " << i << " is " << accumulatedWeight << endl;
    }

}

int main() {
    initializePopulation();
    printChromosome();
    evaluateChromosome();
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
