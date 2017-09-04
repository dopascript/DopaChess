#include "EvaluationsFunctions.h"

const int PieceSquareTables[6][64] =
{
	//King
	{
		-20,-10,-10, -5, -5,-10,-10,-20,
		-10,  0,  0,  0,  0,  0,  0,-10,
		-10,  0,  5,  5,  5,  5,  0,-10,
		-5,  0,  5,  5,  5,  5,  0, -5,
		0,  0,  5,  5,  5,  5,  0, -5,
		-10,  5,  5,  5,  5,  5,  0,-10,
		-10,  0,  5,  0,  0,  0,  0,-10,
		-20,-10,-10, -5, -5,-10,-10,-20
	},
	//Queen
	{
		-30,-40,-40,-50,-50,-40,-40,-30,
		-30,-40,-40,-50,-50,-40,-40,-30,
		-30,-40,-40,-50,-50,-40,-40,-30,
		-30,-40,-40,-50,-50,-40,-40,-30,
		-20,-30,-30,-40,-40,-30,-30,-20,
		-10,-20,-20,-20,-20,-20,-20,-10,
		20, 20,  0,  0,  0,  0, 20, 20,
		20, 30, 10,  0,  0, 10, 30, 20
	},
	//Rook
	{
		0,  0,  0,  0,  0,  0,  0,  0,
		5, 10, 10, 10, 10, 10, 10,  5,
		-5,  0,  0,  0,  0,  0,  0, -5,
		-5,  0,  0,  0,  0,  0,  0, -5,
		-5,  0,  0,  0,  0,  0,  0, -5,
		-5,  0,  0,  0,  0,  0,  0, -5,
		-5,  0,  0,  0,  0,  0,  0, -5,
		0,  0,  0,  5,  5,  0,  0,  0
	},
	//Bishops
	{
		-20,-10,-10,-10,-10,-10,-10,-20,
		-10,  0,  0,  0,  0,  0,  0,-10,
		-10,  0,  5, 10, 10,  5,  0,-10,
		-10,  5,  5, 10, 10,  5,  5,-10,
		-10,  0, 10, 10, 10, 10,  0,-10,
		-10, 10, 10, 10, 10, 10, 10,-10,
		-10,  5,  0,  0,  0,  0,  5,-10,
		-20,-10,-10,-10,-10,-10,-10,-20
	},
	//Knights
	{
		-50,-40,-30,-30,-30,-30,-40,-50,
		-40,-20,  0,  0,  0,  0,-20,-40,
		-30,  0, 10, 15, 15, 10,  0,-30,
		-30,  5, 15, 20, 20, 15,  5,-30,
		-30,  0, 15, 20, 20, 15,  0,-30,
		-30,  5, 10, 15, 15, 10,  5,-30,
		-40,-20,  0,  5,  5,  0,-20,-40,
		-50,-40,-30,-30,-30,-30,-40,-50
	},
	//Pawn
	{
		0, 0, 0, 0, 0, 0, 0, 0,
		50, 50, 50, 50, 50, 50, 50, 50,
		10, 10, 20, 30, 30, 20, 10, 10,
		5, 5, 10, 25, 25, 10, 5, 5,
		0, 0, 0, 20, 20, 0, 0, 0,
		5, -5, -10, 0, 0, -10, -5, 5,
		5, 10, 10, -20, -20, 10, 10, 5,
		0, 0, 0, 0, 0, 0, 0, 0
	}
};

const int PiecesValues[]
{
	0,//King
	900,//Queen
	500,//Rook
	330,//Bishop
	320,//Knight
	100//Pawn
};

int evaluationFunctionBasic(DopaChess::ChessGame* pChessGame, DopaChess::Color pColor)
{
	DopaChess::Chessboard* lChessboard = pChessGame->getChessboard();
	unsigned char lTabIndex = (unsigned char)pColor;
	DopaChess::Color lEnemyColor = pColor == DopaChess::Color::White ? DopaChess::Color::Black : DopaChess::Color::White;
	unsigned char lEnemyTabIndex = (unsigned char)lEnemyColor;

	int lTotalScore = 0;
	int lPiecesScore[2];
	lPiecesScore[0] = 0;
	lPiecesScore[1] = 0;
	for (int i = 0; i < 64; i++)
	{
		if (!lChessboard->Cases[i].Empty)
		{
			DopaChess::Case lCase = lChessboard->Cases[i];
			lPiecesScore[(unsigned char)lCase.Color] += (PiecesValues[(unsigned char)lCase.PieceType] * 2) + PieceSquareTables[(unsigned char)lCase.PieceType][i];
		}
	}

	if (pChessGame->isCheck(lEnemyColor))
	{
		lTotalScore += 100;
	}
	if (pChessGame->isCheck(pColor))
	{
		lTotalScore -= 100;
	}

	lTotalScore += lPiecesScore[lTabIndex];
	lTotalScore -= lPiecesScore[lEnemyTabIndex];

	return lTotalScore;
}