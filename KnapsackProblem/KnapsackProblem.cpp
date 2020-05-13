// KnapsackProblem.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <stdlib.h>
#include <ctime>
using namespace std;

const int GENE = 8;
const int WEIGHT[GENE] = { 45, 35, 25, 5, 25, 3, 2, 2 };
const int POPSIZE = 10;
const float TARGET = 104;
const double CO_PROBABILITY = 0.9;

int chromosome[POPSIZE][GENE];
double fitness[POPSIZE];
int parents[2][GENE];
int children[2][GENE];

void initializePopulation() {
	for (int i = 0; i < POPSIZE; i++) {
		for (int j = 0; j < GENE; j++) {
			chromosome[i][j] = rand() % 2;
		}
	}
}

void printChromosome() {
	for (int i = 0; i < POPSIZE; i++) {
		cout << "Chromosome " << i << "\t";
		for (int j = 0; j < GENE; j++) {
			cout << chromosome[i][j] << "\t";
		}
		cout << "\n";
	}
}

void evaluateChromosome() {
	cout << endl;
	int accumulatedWeight = 0;
	for (int i = 0; i < POPSIZE; i++) {
		accumulatedWeight = 0;
		for (int j = 0; j < GENE; j++) {
			if (chromosome[i][j]) {
				accumulatedWeight += WEIGHT[j];
			}
		}

		// fitness[i] = abs(accumulatedWeight - TARGET) / TARGET;
		fitness[i] = abs(accumulatedWeight - TARGET);
		cout << "Fitness value of chromosome " << i << " is " << fitness[i] << endl;
	}

}

void crossOver() {
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < GENE; j++) {
			children[i][j] = parents[i][j];
		}
	}
	
	if (((rand() % 10) / 10) < CO_PROBABILITY) {
		int co_point = rand() % GENE;
		
		cout << "\n\nCrossover at gene " << co_point + 1 << endl;

		for (int i = co_point; i < GENE; i++) {
			children[0][i] = parents[1][i];
			children[1][i] = parents[0][i];
		}
	}

	cout << "\n\nCrossover results:";

	for (int i = 0; i < 2; i++) {
		cout << "\nChildren " << i << ": ";
		for (int j = 0; j < GENE; j++) {
			cout << children[i][j] << " ";
		}
	}

}

void parentSelection() {
	int player1, player2; // for players
	int indexParents[2]; // Index of selected players

	for (int i = 0; i < 2; i++) { // Selecting 2 parents
		player1 = rand() % POPSIZE;
		player2 = rand() % POPSIZE;

		if (fitness[player1] <= fitness[player2]) {
			indexParents[i] = player1;
		} else {
			indexParents[i] = player2;
		}

		cout << "\nPlayer " << player1 << " vs " << player2 << endl;
		cout << "Player " << fitness[player1] << " vs " << fitness[player2] << endl;
		cout << "Winner " << indexParents[i];
	}

		for (int p = 0; p < 2; p++) {
			cout << "\nParents " << p + 1 << " : ";
			for (int g = 0; g < GENE; g++) {
				parents[p][g] = chromosome[indexParents[p]][g];
				cout << parents[p][g] << " ";
			}
		}
}

int main() {
	int cycles = 0;
	while (true) {
		initializePopulation();
		printChromosome();
		evaluateChromosome();
		parentSelection();
		crossOver();
		cout << "\n\nTotal of " << cycles++ << " cycles ran. \n";
		system("pause");
		system("cls");
	}
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
