/*

SUDOKU+ SOLVER & GENERATOR

Author : Ayush Kumar
Created on 10/2/18

*/

#include <iostream>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <fstream>
#include <cassert>
#include <string>
#include <ctime>

using namespace std;
#define N 9
#define Prop(i, j, k) (size*size*((i)-1) + size*((j)-1) + (k))
#define Iter(i, a, b) for(uint i = (a); i <= (b); i++)
typedef unsigned int uint;

class SudokuSolver {
	typedef unsigned int uint;
	typedef vector<int> vi;
protected:
	uint size;
	vector<vector<uint> > sudoku;
	vector<vector<uint> > model_sudoku;
	vector<vector<int> > clauses;
	vector<int> model;
	bool satisfiable;

	void addClause(vector<int>& cl) {
		clauses.push_back(cl);
	}

	void removeClause() {
		clauses.pop_back();
	}

	void solveSAT() {
		FILE* data_file = fopen("minisat_in", "w");
		fprintf(data_file, "c Sudoku+ Solver\n");
		fprintf(data_file, "c Built by Ayush Kumar(170195) and Deepesh Kumar Lall(170236)\n");
		fprintf(data_file, "p cnf %d %lu\n", size*size*size, clauses.size());
		Iter(i, 0, clauses.size()-1) {
			Iter(j, 0, clauses[i].size()-1) {
				fprintf(data_file, "%d ", clauses[i][j]);
			}
			fprintf(data_file, "0\n");
		}
		fclose(data_file);
		float rnd1 = 100.00001 + static_cast <float> (rand()) /(static_cast <float> (RAND_MAX/(99999999.99999-100.00001)));
		float rnd2 = 0.98;
		string cmd = "minisat -verb=0 -rnd-seed=" + to_string(rnd1) + " -rnd-freq=" + to_string(rnd2) + " minisat_in minisat_out >> abc";
		system(cmd.c_str());
		ifstream model_file;
		model_file.open("minisat_out");
		string sol;
		model_file >> sol;
		satisfiable = (sol == "SAT") ? true : false;
		model.clear();
		if (satisfiable) {
			Iter(i, 1, size*size*size) {
				int tmp;
				model_file >> tmp;
				model.push_back(tmp);
			}
		}
		model_file.close();
		remove("abc");
		remove("minisat_in");
		remove("minisat_out");
	}

	void convertToCNF() {
		//constr1 : at least one number in each cell 
		//constr2 : exactly one number in each cell
		//constr3 : no two cells in same row are same
		//constr4 : no two cells in same column are same
		//constr5 : no two cells in same diagonal1 are same
		//constr6 : no two cells in same diagonal2 are same
		//constr7 : numbers in the initial sudoku should remain the same
		//constr8 : numbers in the 3 x 3 submatrix should be distinct

		clauses.clear();

		Iter(i, 1, size) {
			Iter(j, 1, size) {
				if (sudoku[i-1][j-1] != 0) {
					vi constr7;
					constr7.push_back(Prop(i, j, sudoku[i-1][j-1]));
					addClause(constr7);
				}
			}
		}

		Iter(i, 1, size) {
			Iter(j, 1, size) {
				vi constr1;
				Iter(k, 1, size) {
					constr1.push_back(Prop(i, j, k));
					Iter(l, k+1, size) {
						vi constr2;
						constr2.push_back(-Prop(i, j, k));
						constr2.push_back(-Prop(i, j, l));
						addClause(constr2);
					}
				}
				addClause(constr1);
			}
		}

		Iter(i, 1, size) {
			Iter(j1, 1, size) {
				Iter(j2, j1+1, size) {
					Iter(k, 1, size) {
						vi constr3, constr4;
						constr3.push_back(-Prop(i, j1, k));
						constr3.push_back(-Prop(i, j2, k));
						constr4.push_back(-Prop(j1, i, k));
						constr4.push_back(-Prop(j2, i, k));
						addClause(constr3);
						addClause(constr4);
					}
				}
			}
		}
		
		Iter(i1, 1, size) {
			Iter(i2, i1+1, size) {
				Iter(k, 1, size) {
					vi constr5, constr6;
					constr5.push_back(-Prop(i1, i1, k));
					constr5.push_back(-Prop(i2, i2, k));
					constr6.push_back(-Prop(i1, size+1-i1, k));
					constr6.push_back(-Prop(i2, size+1-i2, k));
					addClause(constr5);
					addClause(constr6);
				}
			}
		}
		
		uint tmp = sqrt(size); 
		Iter(i, 0, size-1) {
			Iter(j1, 0, size-1) {
				Iter(j2, j1+1, size-1) {
					Iter(k, 1, size) {
						vi constr8;
						constr8.push_back(-Prop((i/tmp)*tmp+1+j1/tmp, (i%tmp)*tmp+1+j1%tmp, k));
						constr8.push_back(-Prop((i/tmp)*tmp+1+j2/tmp, (i%tmp)*tmp+1+j2%tmp, k));
						addClause(constr8);
					}
				}
			}
		}
	}	

	void genModelSudoku() {
		Iter(i, 0, model.size()-1) {
			int prop = model[i];
			if (prop > 0) {
				uint i = (prop-1)/(size*size), j = (prop - 1 - (size*size)*i)/size;
				uint k = (prop-1) % size + 1;
				model_sudoku[i][j] = k;
			}
		}
	}

	bool uniqueModel() {
		//checks if the currently stored sudoku has a unique model solution
		solve();
		vi checker;
		Iter(i, 0, model.size()-1) {
			checker.push_back(-model[i]);
		}
		//convertToCNF();
		addClause(checker);
		solveSAT();
		removeClause();
		if (!satisfiable) {
			solve();
			return true;
		}
		solve();
		return false;
	}	

public:
	SudokuSolver(vector<vector<uint> >& sk) {
		srand(time(0));
		size = sk.size();
		sudoku = sk;
		model_sudoku = vector<vector<uint> > (size, vector<uint>(size, 0));
		satisfiable = false;
	}

	SudokuSolver(uint n) {
		srand(time(0));
		size = n;
		model_sudoku = sudoku = vector<vector<uint> > (size, vector<uint>(size, 0));
		satisfiable = false;
	}

	void solve() {
		convertToCNF();
		solveSAT();
	}

	void printSAT() {
		FILE* sudokusat_file = fopen("sudokusat.txt", "w");
		fprintf(sudokusat_file, "c Sudoku+ Solver\n");
		fprintf(sudokusat_file, "c Built by Ayush Kumar(170195) and Deepesh Kumar Lall(170236)\n");
		fprintf(sudokusat_file, "p cnf %d %lu\n", size*size*size, clauses.size());
		Iter(i, 0, clauses.size()-1) {
			Iter(j, 0, clauses[i].size()-1) {
				fprintf(sudokusat_file, "%d ", clauses[i][j]);
			}
			fprintf(sudokusat_file, "0\n");
		}
		fclose(sudokusat_file);
		printf("The SAT encoding for the given Sudoku+ has been published in sudokusat.txt file.\n\n");
	}

	void printModelSudoku() {
		//assert(satisfiable);
		if (!satisfiable) {
			printf("No solution found for the given Sudoku+! ;(\n\n");
			return;
		}
		if (!uniqueModel()) {
			printf("Multiple solutions found for the given Sudoku+!\n\n");
		}
		solve();
		genModelSudoku();
		FILE* sudokusol_file = fopen("sudokusol.txt", "w");
		Iter(i, 0, size-1) {
			Iter(j, 0, size-1) {
				if (model_sudoku[i][j] == 0) {
					fprintf(sudokusol_file, ".");
				} else {
					fprintf(sudokusol_file, "%d", model_sudoku[i][j]);
				}
				if (j != size-1) {
					fprintf(sudokusol_file, " ");
				}
			}
			if (i != size-1) {
				fprintf(sudokusol_file, "\n");
			}
		}
		fclose(sudokusol_file);
		printf("The Sudoku+ has been solved in sudokusol.txt file.\n\n");
		Iter(i, 0, size-1) {
			Iter(j, 0, size-1) {
				if (model_sudoku[i][j] == 0) {
					printf(".");
				} else {
					printf("%d", model_sudoku[i][j]);
				}
				if (j != size-1) {
					printf(" ");
				}
			}
			printf("\n");
		}
		printf("\n");
	}

	/*
	bool isSatisfiable() {
		return satisfiable;
	}

	void tweakValue(uint i, uint j, uint k) {
		assert(i < size && j < size && k <= size);
		sudoku[i][j] = k;
	}

	uint getValue(uint i, uint j) {
		return sudoku[i][j];
	}
	*/
};

class SudokuGenerator : public SudokuSolver {
	typedef unsigned int uint;
	typedef vector<int> vi;
public:
	SudokuGenerator(uint n) : SudokuSolver(n) {
		size = n;
	}

	void generate() {
		sudoku = vector<vector<uint> > (size, vector<uint>(size, 0));
		solve();
		genModelSudoku();
		sudoku = model_sudoku;
		while(true) {
			uint i, j, k;
			while(sudoku[(i = rand() % size)][(j = rand() % size)] == 0) {
				continue;
			}
			k = sudoku[i][j];
			sudoku[i][j] = 0;
			//solve();	
			if (!uniqueModel()) {
				sudoku[i][j] = k;
				bool possible_flag = false;
				Iter(i, 0, size-1) {
					Iter(j, 0, size-1) {
						if (sudoku[i][j] == 0) {continue;}
						uint tmp = sudoku[i][j];
						sudoku[i][j] = 0;
						if (uniqueModel()) {possible_flag = true; break;}
						else {sudoku[i][j] = tmp;}
					}
					if (possible_flag) {break;}
				}
				if (possible_flag) {continue;}
				//this is the best possible unique model
				break;
			}
		}

	}
	
	void printSudoku() {
		FILE* newsudoku_file = fopen("newsudoku.txt", "w");
		Iter(i, 0, size-1) {
			Iter(j, 0, size-1) {
				if (sudoku[i][j] == 0) {
					fprintf(newsudoku_file, ".");
				} else {
					fprintf(newsudoku_file, "%d", sudoku[i][j]);
				}
				if (j != size-1) {
					fprintf(newsudoku_file, " ");
				}
			}
			if (i != size-1) {
				fprintf(newsudoku_file, "\n");
			}
		}
		fclose(newsudoku_file);
		printf("New Sudoku+ has been generated in newsudoku.txt file.\n\n");
		Iter(i, 0, size-1) {
			Iter(j, 0, size-1) {
				if (sudoku[i][j] == 0) {
					printf(".");
				} else {
					printf("%d", sudoku[i][j]);
				}
				if (j != size-1) {
					printf(" ");
				}
			}
			printf("\n");
		}
		printf("\n");
	}
};

int main() {
	printf("#######SUDOKU+ SOLVER & GENERATOR#######\n\n");

	printf("Enter the filename containing sudoku+ : ");
	string filename; cin >> filename;
	printf("\n");
	FILE* sudoku_file = fopen(filename.c_str(), "r");
	if (!sudoku_file) {
		printf("Unable to open file!\n\n");
		return 0;
	}
	vector<vector<uint> > sudoku(N, vector<uint>(N));
	uint cnt = 0;
	while(true) {
		char ch='\n';
		while (ch == '\n' || ch == ' ') {
			fscanf(sudoku_file, "%c", &ch);
		}
		uint d = ((ch - '0') <= 9 && 1 <= (ch -'0')) ? (ch - '0') : 0;
		sudoku[cnt/N][cnt%N] = d;
		cnt++;
		if (cnt >= N*N) {break;}
	}
	fclose(sudoku_file);

	//Solve the given sudoku
	SudokuSolver mysudoku(sudoku);
	mysudoku.solve();
	mysudoku.printSAT();
	printf("Solving Sudoku+...\n\n");
	mysudoku.printModelSudoku();
	
	//Generate a new sudoku
	printf("Generating new Sudoku+...\n\n");
	SudokuGenerator newsudoku(N);
	newsudoku.generate();
	newsudoku.printSudoku();
	
	printf("########################################\n");
	return 0;
}