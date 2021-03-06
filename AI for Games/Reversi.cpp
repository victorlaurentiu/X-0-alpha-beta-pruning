#include "Reversi.h"

Point d[] = { { -1, 0 },{ -1, 1 },{ 0, 1 },{ 1, 1 },{ 1, 0 },{ 1, -1 },{ 0, -1 },{ -1, -1 } };

Reversi::Reversi()
{
	init();
}

Reversi::~Reversi() {}

bool Reversi::isOutsidePoint(Point p) {
	return p.x < 0 || p.y < 0 || p.x >= n || p.y >= n;
}

bool Reversi::isValid(Move* move)
{
	if (!move)	return true;

	RevMove mv = *(RevMove*)move;

	Point v = { mv.row, mv.col };
	if (isOutsidePoint(v) || table[mv.row][mv.col] != 0)
		return false;

	for (int k = 0; k < 8; k++) {
		v = d[k];
		v.x += mv.row;
		v.y += mv.col;

		if (!isOutsidePoint(v) && table[v.x][v.y] == -mv.player) {
			while (!isOutsidePoint(v) && table[v.x][v.y] == -mv.player) {
				v.x += d[k].x;
				v.y += d[k].y;
			}
			if (!isOutsidePoint(v) && table[v.x][v.y] == mv.player) {
				return true;
			}
		}
	}

	return false;
}

void Reversi::setN(int n)
{
	this->n = n;
}

void Reversi::init()
{
	//table = vector< vector<int> >(n, vector<int>(n));
	//table[n / 2 - 1][n / 2 - 1] = table[n / 2][n / 2] = 1;
	//table[n / 2 - 1][n / 2] = table[n / 2][n / 2 - 1] = -1;
	//
	//heuristic = vector< vector<int> >(n, vector<int>(n));
	//heuristic[0][0] = heuristic[n - 1][0] = heuristic[0][n - 1] = heuristic[n - 1][n - 1] = 99;
	//heuristic[1][1] = heuristic[n - 2][1] = heuristic[1][n - 2] = heuristic[n - 2][n - 2] = -24;
	//
	//heuristic[0][1] = heuristic[n - 1][1] = heuristic[1][n - 1] = heuristic[n - 2][n - 1] = -8;
	//heuristic[1][0] = heuristic[n - 2][0] = heuristic[0][n - 2] = heuristic[n - 1][n - 2] = -8;
	//
	//for (int i = 2; i <= n / 2 - 1; i++) {
	//	heuristic[0][i] = heuristic[0][n - i - 1] = heuristic[n - 1][i] = heuristic[n - 1][n - i - 1] = (n - i) * 2;
	//	heuristic[i][0] = heuristic[n - i - 1][0] = heuristic[i][n - 1] = heuristic[n - i - 1][n - 1] = (n - i) * 2;
	//}
	//
	//for (int i = 2; i <= n / 2 - 1; i++) {
	//	heuristic[1][i] = heuristic[1][n - i - 1] = heuristic[n - 2][i] = heuristic[n - 2][n - i - 1] = -(n - i);
	//	heuristic[i][1] = heuristic[n - i - 1][1] = heuristic[i][n - 2] = heuristic[n - i - 1][n - 2] = -(n - i);
	//}
	//
	//for (int i = 2; i <= n / 2 - 1; i++) {
	//	for (int j = 2; j <= n / 2 - 1; j++) {
	//		heuristic[i][j] = heuristic[i][n - j - 1] = heuristic[n - i - 1][j] = heuristic[n - i - 1][n - j - 1] = (n*n - 4*i*j);
	//	}
	//}
}

std::vector<Move*> Reversi::getMoves(int player)
{
	RevMove* mv;
	std::vector<Move*> moves;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			mv = new RevMove(i, j, player, n);
			if (isValid(mv)) {
				moves.push_back(mv);
			}
			else {
				delete mv;
			}
		}
	}
	moves.push_back(new RevMove(-1, -1, player, n));	// add the Pas move

	return moves;
}

Move * Reversi::getNextMove(Move * prevMove, int player)
{
	RevMove* mv;
	RevMove* pmv = (RevMove*)prevMove;

	std::vector<Move*> moves;
	int i = pmv->row;
	int j = pmv->col + 1;
	for (; i < n; i++) {
		for (; j < n; j++) {
			mv = new RevMove(i, j, player, n);
			if (isValid(mv)) {
				return mv;
			}
			delete mv;
		}
		j = 0;
	}

	return NULL;
}

// move used to get the first valid move
Move * Reversi::getInitMove(int player)
{
	return new RevMove(0, -1, player, n);
}

Move* Reversi::readHumanMove(int player)
{
	if (player == -1)
		cout << "X turn:\n";
	else
		cout << "O turn:\n"; 
	
	cout << "Press U to undo or\n";
	cout << "Press P to Pas the move or\n";
	cout << "Write position from:\n";

	vector<Move*> moves = getMoves(player);
	int s = moves.size();
	for (int i = 0; i < s - 1; i++) {
		RevMove* mv = (RevMove*)(moves[i]);
		cout << "{" << mv->row << ", " << mv->col << "}, ";
	}
	cout << endl;

	int i, j;
	i = _getch();
	if (i == 'u') { return new Move(true); }
	if (i == 'p') { return NULL; }

	i -= '0';
	cout << i;
	cin >> j;

	return new RevMove(i, j, player, n);
}

bool Reversi::ended()
{
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
		{
			if (isValid(new RevMove(i, j, 1, n)))
				return false;
			if (isValid(new RevMove(i, j, -1, n)))
				return false;
		}

	return true;
	//return (getMoves(-1).size() + getMoves(1).size() == 0);
}

int Reversi::eval(int player)
{
	int h = 0;
	int my_tiles = 0;
	int op_tiles = 0;

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (table[i][j] == player) {
				h += heuristic[i][j];
				my_tiles++;
			}
			else if (table[i][j] == -player) {
				h -= heuristic[i][j];
				op_tiles++;
			}
		}
	}

	// Mobility
//	int m = getMoves(player).size() - getMoves(-player).size();

	int score = h + (my_tiles - op_tiles) * 15;// +m * 20;
	return score;
}

void Reversi::apply_move(Move * mv)
{
	if (!mv) return;
	RevMove move = *(RevMove*)mv;
	RevMove* temp = (RevMove*)mv;

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			temp->table[i][j] = table[i][j];
		}
	}

	mv = (Move*)temp;
	if (move.row == -1)	// daca am mutarea nula
		return;

	Point v;
	for (int k = 0; k < 8; k++) {
		v = d[k];
		v.x += move.row;
		v.y += move.col;
		if (!isOutsidePoint(v) && table[v.x][v.y] == -move.player) {
			while (!isOutsidePoint(v) && table[v.x][v.y] == -move.player) {
				v.x += d[k].x;
				v.y += d[k].y;
			}
			if (!isOutsidePoint(v) && table[v.x][v.y] == move.player) {
				v.x -= d[k].x;
				v.y -= d[k].y;
				while (table[v.x][v.y] == -move.player) {
					table[v.x][v.y] = move.player;
					v.x -= d[k].x;
					v.y -= d[k].y;
				}
				table[v.x][v.y] = move.player;
			}
		}
	}
}

void Reversi::undo(Move * mv)
{
	if (!mv) return;

	RevMove move = *(RevMove*)mv;
	
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			table[i][j] = move.table[i][j];
		}
	}
}

void Reversi::print()
{
	for (int i = 0; i < n; i++) {
		std::cout << i << " ";
	}
	std::cout << std::endl;

	int x = 0, o = 0;
	for (int i = 0; i < n; i++)	{
		for (int j = 0; j < n; j++) {
			if (table[i][j] == 0)
				std::cout << '.';
			else if (table[i][j] == 1) {
				std::cout << 'O';
				o++;
			}
			else {
				std::cout << 'X';
				x++;
			}
			std::cout << " ";
		}
		std::cout << i << std::endl;
	}
	std::cout << "\n X: " << x << "\n O: " << o << "\n\n";
}

void Reversi::showRezult(int turn)
{
	int s = 0;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (table[i][j] == -1)
				s++;
			else if (table[i][j] == 1)
				s--;
		}
	}

	if (s > 0)
		std::cout << "X WON!" << std::endl;
	else if (s < 0)
		std::cout << "O WON!" << std::endl;
	else
		std::cout << "Draw" << std::endl;
}
