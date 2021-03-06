// KnapsackProblem.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <stdlib.h>
#include <ctime>
#include <fstream>

using namespace std;

const int GENE = 8;
const int WEIGHT[GENE] = { 45, 35, 25, 5, 25, 3, 2, 2 };
const int POPSIZE = 10;
const float TARGET = 104;
const float MUT_PROBABILITY = 0.9;
const double CO_PROBABILITY = 0.9;

double averageFitness;
double bestFitness = 1000000;
int bestChromosome[GENE];

ofstream bestFitnessFile;
ofstream avgFitnessFile;
ofstream bestChromosomeFile;

int cycles = 0;

int chromosome[POPSIZE][GENE];
int tempChromosome[POPSIZE][GENE];
int newChromosomeCounter = 0;
double fitness[POPSIZE];
int parents[2][GENE];
int children[2][GENE];

// Print a chromosome into space seperated genes.
void printChromosome(int chromosome[GENE]) {
	for (int i = 0; i < GENE; i++) {
		cout << chromosome[i] << " ";
	}
	cout << "\n";
}

void printLine(int num = 10) {
	for (int i = 0; i < 10; i++) {
		cout << "=";
	}

	cout << endl;
}

void printAllChromosomes(string prefix = "\t", string arrayName = "ori") {
	if (arrayName == "ori") {
		for (int i = 0; i < POPSIZE; i++) {
			cout << prefix << "Chromosome " << i << "\t";
			printChromosome(chromosome[i]);
		}
	}
	else if (arrayName == "survival") {
		for (int i = 0; i < (newChromosomeCounter * 2); i++) {
			cout << prefix << "Survival Chromosome " << i << "\t";
			printChromosome(tempChromosome[i]);
		}
	}
}

void initializePopulation() {
	for (int i = 0; i < POPSIZE; i++) {
		cout << "\tChromosome " << i << "\t";
		for (int j = 0; j < GENE; j++) {
			chromosome[i][j] = rand() % 2;
		}
		printChromosome(chromosome[i]);
	}
}

void evaluateChromosome() {
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
		cout << "\tFitness value of chromosome " << i << " is " << fitness[i] << endl;
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

		cout << "\tCrossover at gene " << co_point + 1 << endl;

		for (int i = co_point; i < GENE; i++) {
			children[0][i] = parents[1][i];
			children[1][i] = parents[0][i];
		}
	}

	cout << "\n\tCrossover results:" << endl;

	for (int i = 0; i < 2; i++) {
		cout << "\t\tC" << i << ": ";
		printChromosome(children[i]);
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
		}
		else {
			indexParents[i] = player2;
		}

		cout << "\tPlayer " << player1 << "(" << fitness[player1] << ") vs Player " << player2 << "(" << fitness[player2] << ")\t: Winner " << indexParents[i] << endl;
	}

	cout << endl;
	cout << "\tParents are:\n";


	for (int p = 0; p < 2; p++) {
		cout << "\t\t" << indexParents[p] << "\t: ";
		for (int g = 0; g < GENE; g++) {
			parents[p][g] = chromosome[indexParents[p]][g];
		}
		printChromosome(parents[p]);
	}
}

void mutation() {
	float prob;
	int mut_point;

	for (int c = 0; c < 2; c++) {
		prob = (rand() % 11) / 10.0;

		if (prob < MUT_PROBABILITY) {
			mut_point = rand() % GENE;

			cout << "\tC" << c << ": Mutation at gene = " << mut_point << endl;

			// Bitflip
			children[c][mut_point] = !children[c][mut_point];
		}
		else {
			cout << "\tC" << c << ": No mutation" << endl;
		}

	}

	cout << "\n\tMutation results:" << endl;

	for (int i = 0; i < 2; i++) {
		cout << "\t\tC" << i << ": ";
		printChromosome(children[i]);
	}
}

void survivalSelection() {
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < GENE; j++) {
			tempChromosome[(newChromosomeCounter * 2) + i][j] = children[i][j];
		}
	}

	newChromosomeCounter++;

}

void copyChromosome() {
	for (int i = 0; i < POPSIZE; i++) {
		for (int j = 0; j < GENE; j++) {
			chromosome[i][j] = tempChromosome[i][j];
		}
	}
}

void calculateAverageFitness() {
	double totalFitness = 0;

	for (int i = 0; i < POPSIZE; i++) {
		totalFitness += fitness[i];
	}

	averageFitness = totalFitness / GENE;

	cout << "\nAverage fitness is " << averageFitness << endl;

	avgFitnessFile << "Gen " << cycles << "\t: " << averageFitness << endl;
}

void recordBestFitness() {

	for (int i = 0; i < POPSIZE; i++) {
		if (fitness[i] < bestFitness) {
			bestFitness = fitness[i];
			
			for (int j = 0; j < GENE; j++) {
				bestChromosome[j] = chromosome[i][j];
			}
		}

	}

	cout << "Best chromosome is ";
	printChromosome(bestChromosome);
	cout << "Best fitness is " << bestFitness << endl;

	bestChromosomeFile << "Gen " << cycles << "\t: ";
	for (int i = 0; i < GENE; i++) {
		bestChromosomeFile << bestChromosome[i] << " ";
	}
	bestChromosomeFile << "\n";

	bestFitnessFile << "Gen " << cycles << "\t: " << bestFitness << endl;
}

int main() {
	bestFitnessFile.open("bestFitness.txt");
	avgFitnessFile.open("avgFitness.txt");
	bestChromosomeFile.open("bestChromosome.txt");

	cout << "Initialization" << endl;
	initializePopulation();

	while (true) {
		newChromosomeCounter = 0;

		cout << "\nChromosome evaluation" << endl;
		evaluateChromosome();


		calculateAverageFitness();

		recordBestFitness();

		while (newChromosomeCounter * 2 < GENE) {
			cout << "\nSURVIVAL SELECTION ROUND " << newChromosomeCounter << endl;
			cout << "\nParent selection" << endl;
			parentSelection();

			cout << "\nCrossover" << endl;
			crossOver();

			cout << "\nMutation" << endl;
			mutation();

			survivalSelection();
		}

		cout << "\nSurvival Selection" << endl;
		printAllChromosomes("\t", "survival");

		copyChromosome();

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
