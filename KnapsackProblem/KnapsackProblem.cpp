// KnapsackProblem.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <stdlib.h>
#include <ctime>
using namespace std;

// If all chromosomes have the same fitness value, halt the program.
const bool HALT_ON_ALL_SAME = true;

// Whether to replace the best chromosome on random
const bool RANDOM_REPLACE_BEST = true;

const int GENE = 8;
const int WEIGHT[GENE] = { 45, 35, 25, 5, 25, 3, 2, 2 };
const int POPSIZE = 10;
const float TARGET = 104;
const float MUT_PROBABILITY = 0.5;

const double CO_PROBABILITY = 0.9;

int chromosome[POPSIZE][GENE];
double fitness[POPSIZE];
int parents[2][GENE];
int children[2][GENE];
int currBestFitnessID = -1;
int currBestFitness = -1;

// Print a chromosome into space seperated genes.
void printChromosome(int chromosome[GENE], int fitnessIndex = 0, bool printFitness = false) {
	for (int i = 0; i < GENE; i++) {
		cout << chromosome[i] << " ";
	}
	if (printFitness) {
		cout << "\t" << fitness[fitnessIndex];
	}
	cout << "\n";
}

void printLine(int num = 20) {
	for (int i = 0; i < 10; i++) {
		cout << "=";
	}

	cout << endl;
}

void printAllChromosomes(string prefix = "\t", bool printFitness = false) {
	for (int i = 0; i < POPSIZE; i++) {
		cout << prefix << "Chr " << i << "\t";
		printChromosome(chromosome[i], i, printFitness);
	}
}

void initializePopulation() {
	for (int i = 0; i < POPSIZE; i++) {
		for (int j = 0; j < GENE; j++) {
			chromosome[i][j] = rand() % 2;
		}
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

ReselectParent:
	// Take 2 random player and select the best of them.
	for (int i = 0; i < 2; i++) {
		player1 = rand() % POPSIZE;
		player2 = rand() % POPSIZE;

		if (fitness[player1] <= fitness[player2]) {
			indexParents[i] = player1;
		}
		else {
			indexParents[i] = player2;
		}

		cout << "\tChr " << player1 << "(" << fitness[player1] << ") vs Chr " << player2 << "(" << fitness[player2] << ")\t: Winner " << indexParents[i] << endl;
	}

	// If parents are the same, reselect parents
	if (indexParents[0] == indexParents[1]) {
		cout << "Parents are the same, reselecting parents." << endl;
		goto ReselectParent;
	}

	cout << endl;
	cout << "\tParents are:\n";


	for (int p = 0; p < 2; p++) {
		cout << "\t\tChr " << indexParents[p] << "\t: ";
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

			cout << "\tC" << c << ": Mutation at gene " << mut_point << endl;

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

void newGenSelection() {
	int worstID = -1;
	int worstFitness = -1;
	// Select the worst
	for (int i = 0; i < POPSIZE; i++) {
		if (fitness[i] > worstFitness) {
			worstFitness = fitness[i];
			worstID = i;
		}
	}

	cout << "\tC0: Replacing gene " << worstID << " as it had the worst fitness at " << worstFitness << endl;

	// Replace the gene with that of children
	for (int i = 0; i < GENE; i++) {
		chromosome[worstID][i] = children[0][i];
	}

	// Random replacement
	int randReplacement = rand() % GENE;

	if (!RANDOM_REPLACE_BEST) {
		if ((fitness[randReplacement] == currBestFitness) || (randReplacement == worstID)) {
			for (int i = 0; i < POPSIZE; i++) {
				if (!((fitness[i] == currBestFitness) || (i == worstID))) {
					randReplacement = i;

				}
			}
		}
	} else {
		if (randReplacement == worstID) {
			randReplacement = (worstID + 1) % GENE;
		}
	}


	cout << "\tC1: Replacing chr " << randReplacement << " by random." << endl;

	for (int i = 0; i < GENE; i++) {
		chromosome[randReplacement][i] = children[1][i];
	}

	cout << "\n\tNew generation: " << endl;

	printAllChromosomes("\t\t");

}

void bestFitness() {
	int bestFitness = TARGET;
	int bestFitnessID = -1;
	for (int i = 0; i < POPSIZE; i++) {
		if (fitness[i] < bestFitness) {
			bestFitness = fitness[i];
			bestFitnessID = i;
		}
	}

	currBestFitness = bestFitness;
	currBestFitnessID = bestFitnessID;
}

int main() {
	int cycles = 0;
	bool breakLoop = false;

	cout << "Chromosome intialization... ";
	initializePopulation();
	cout << "done.\n";

	while (!breakLoop) {
		cout << "Gen " << cycles << endl;
		printLine();

		cout << "\nChromosome evaluation... ";
		evaluateChromosome();
		cout << "done.\n";

		cout << "\t Chr    Gene           \t\tFitness" << endl;
		printAllChromosomes("\t", true);

		cout << "\nParent selection" << endl;
		parentSelection();

		cout << "\nCrossover" << endl;
		crossOver();

		cout << "\nMutation" << endl;
		mutation();

		bestFitness();

		printLine();
		cout << "\nBest chromosome for this generation is chr " << currBestFitnessID << " with fitness of " << currBestFitness << endl;
		printLine();

		// If any chromosome hits the best possible fitness value, halt.
		if (currBestFitness == 0) {
			breakLoop = true;
			break;
		}

		// If all chromosomes have the same fitness, halt based on constant.
		int tempInt = fitness[0];
		for (int i = 1; i < POPSIZE; i++) {
			if (fitness[i] != tempInt) {
				break;
			}

			if (HALT_ON_ALL_SAME && (i == POPSIZE - 1)) {
				breakLoop = true;
			}
		}

		if (breakLoop) {
			break;
		}

		cout << "\nNew gen crafting" << endl;
		newGenSelection();

		cout << "\n\nTotal of " << cycles++ << " cycles ran. \n";
		system("pause");
		system("cls");
	}

	cout << "Program complete, best chromosome is chromosome " << currBestFitnessID << " with a fitness of " << currBestFitness << ". Its gene is: " << endl;
	printChromosome(chromosome[currBestFitnessID]);
	cout << "\n\n";
	system("pause");
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
