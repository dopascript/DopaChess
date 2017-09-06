#include "ChessGameAi.h"
#include "ChessGame.h"
#include <climits>
#include <vector>
#include <chrono>

using namespace DopaChess;

ChessGameAi::ChessGameAi()
{
	mUseOpeningBook = true;
}

AiResult ChessGameAi::getNextMove(ChessGame *pChessGame, Color pColor, std::vector<DopaChess::Chessboard> mChessboardsListHistory)
{
	mMovesCount = 0;
	auto start = std::chrono::steady_clock::now();
	std::vector<DopaChess::Move> mMoves;
	if (mUseOpeningBook)
	{
		mMoves = mOpeningBook.getMoves(pChessGame->getChessboard());
	}
	AiResult lAiResult;
	if (mMoves.size() > 0)
	{
		lAiResult.Move = mMoves[0];
	}
	else
	{
		DopaChess::MovesList lMoves = getMovesWithEvaluation(pChessGame, pColor);
		for (int i = 0; i < lMoves.Count; i++)
		{
			if (!moveAlreadyPlayed(pChessGame, mChessboardsListHistory, lMoves.Moves[i]))
			{
				lAiResult.Move = lMoves.Moves[i];
				break;
			}
		}
	}
	auto end = std::chrono::steady_clock::now();
	lAiResult.MovesCount = mMovesCount;
	lAiResult.Duration = std::chrono::duration<double, std::milli>(end - start).count();
	return lAiResult;
}

bool ChessGameAi::moveAlreadyPlayed(ChessGame *pChessGame, const std::vector<DopaChess::Chessboard> &pChessboardsListHistory, DopaChess::Move pMove)
{
	pChessGame->applyMove(pMove);
	DopaChess::Chessboard lChessboardToFind = *pChessGame->getChessboard();
	pChessGame->cancelMove(pMove);
	for (int i = pChessboardsListHistory.size() - 1; i >= 0; i--)
	{
		if (memcmp(&lChessboardToFind, &pChessboardsListHistory[i], sizeof(DopaChess::Chessboard)) == 0)
		{
			return true;
		}
	}
	return false;
}

MovesList ChessGameAi::getMovesWithEvaluation(ChessGame *pChessGame, Color pColor)
{
	MovesList lMoves;
	lMoves.Count = 0;
	pChessGame->addAllMoves(&lMoves, pColor, true);

	for (int i = 0; i < lMoves.Count; i++)
	{
		Move lMove = lMoves.Moves[i];
		pChessGame->applyMove(lMove);
		lMoves.Moves[i].Value = alphaBeta(pChessGame, pColor, 4, INT_MIN, INT_MAX, false);
		pChessGame->cancelMove(lMove);
	}

	ChessGame::sortMoves(&lMoves);
	return lMoves;
}

int ChessGameAi::alphaBeta(ChessGame *pChessGame, Color pColor, int pDepth, int pAlpha, int pBeta, bool pMax)
{
	if (pDepth == 0)
	{
		return evaluationFunctionBasic(pChessGame, pColor);
	}

	Color lMoveColor = pMax ? pColor : ChessGame::EnemyColor[(char)pColor];

	MovesList lMoves;
	pChessGame->addAllMoves(&lMoves, lMoveColor, true);
	if (lMoves.Count == 0)
	{
		return !pMax ? INT_MAX - 100 : INT_MIN + 100;
	}

	for (int i = 0; i < lMoves.Count; i++)
	{
		pChessGame->applyMove(lMoves.Moves[i]);
		int lResult = alphaBeta(pChessGame, pColor, pDepth - 1, pAlpha, pBeta, !pMax);
		pChessGame->cancelMove(lMoves.Moves[i]);
		lResult--;
		if (pMax)
		{
			pAlpha = pAlpha < lResult ? lResult : pAlpha;
		}
		else
		{
			pBeta = pBeta > lResult ? lResult : pBeta;
		}
		if (pBeta < pAlpha)
		{
			break;
		}
	}

	if (pMax)
	{
		return pAlpha;
	}
	else
	{
		return pBeta;
	}
}

const int PieceSquareTables[2][6][64] =
{
	//White
	{
		//King
		{
			20, 30, 10, 0, 0, 10, 30, 20,
			20, 20, 0, 0, 0, 0, 20, 20,
			-10, -20, -20, -20, -20, -20, -20, -10,
			-20, -30, -30, -40, -40, -30, -30, -20,
			-30, -40, -40, -50, -50, -40, -40, -30,
			-30, -40, -40, -50, -50, -40, -40, -30,
			-30, -40, -40, -50, -50, -40, -40, -30,
			-30, -40, -40, -50, -50, -40, -40, -30
		},
	//Queen
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
			0,  0,  0,  0,  0,  0, 0, 0,
			50, 50, 50, 50, 50, 50,50,50,
			10, 10, 20, 30, 30, 20,10,10,
			5,  5, 10, 25, 25, 10, 5, 5,
			0,  0,  0, 20, 20,  0, 0, 0,
			5, -5,-10,  0,  0,-10,-5, 5,
			5, 10, 10,-20,-20, 10,10, 5,
			0,  0,  0,  0,  0,  0, 0, 0
		}
	},
	//Black
	{
		//King
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
	//Queen
		{
			-20, -10, -10, -5, -5, -10, -10, -20,
			-10, 0, 0, 0, 0, 5, 0, -10,
			-10, 0, 5, 5, 5, 5, 5, -10,
			-5, 0, 5, 5, 5, 5, 0, 0,
			-5, 0, 5, 5, 5, 5, 0, -5,
			-10, 0, 5, 5, 5, 5, 0, -10,
			-10, 0, 0, 0, 0, 0, 0, -10,
			-20, -10, -10, -5, -5, -10, -10, -20
		},
	//Rook
		{
			0, 0, 0, 5, 5, 0, 0, 0,
			-5, 0, 0, 0, 0, 0, 0, -5,
			-5, 0, 0, 0, 0, 0, 0, -5,
			-5, 0, 0, 0, 0, 0, 0, -5,
			-5, 0, 0, 0, 0, 0, 0, -5,
			-5, 0, 0, 0, 0, 0, 0, -5,
			5, 10, 10, 10, 10, 10, 10, 5,
			0, 0, 0, 0, 0, 0, 0, 0
		},
	//Bishops
		{
			-20, -10, -10, -10, -10, -10, -10, -20,
			-10, 5, 0, 0, 0, 0, 5, -10,
			-10, 10, 10, 10, 10, 10, 10, -10,
			-10, 0, 10, 10, 10, 10, 0, -10,
			-10, 5, 5, 10, 10, 5, 5, -10,
			-10, 0, 5, 10, 10, 5, 0, -10,
			-10, 0, 0, 0, 0, 0, 0, -10,
			-20, -10, -10, -10, -10, -10, -10, -20
		},
	//Knights
		{
			-50, -40, -30, -30, -30, -30, -40, -50,
			-40, -20, 0, 5, 5, 0, -20, -40,
			-30, 5, 10, 15, 15, 10, 5, -30,
			-30, 0, 15, 20, 20, 15, 0, -30,
			-30, 5, 15, 20, 20, 15, 5, -30,
			-30, 0, 10, 15, 15, 10, 0, -30,
			-40, -20, 0, 0, 0, 0, -20, -40,
			-50, -40, -30, -30, -30, -30, -40, -50
		},
	//Pawn
		{
			0, 0, 0, 0, 0, 0, 0, 0,
			5, 10, 10, -20, -20, 10, 10, 5,
			5, -5, -10, 0, 0, -10, -5, 5,
			0, 0, 0, 20, 20, 0, 0, 0,
			5, 5, 10, 25, 25, 10, 5, 5,
			10, 10, 20, 30, 30, 20, 10, 10,
			50, 50, 50, 50, 50, 50, 50, 50,
			0, 0, 0, 0, 0, 0, 0, 0
		}
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

int ChessGameAi::evaluationFunctionBasic(ChessGame* pChessGame, Color pColor)
{
	mMovesCount++;

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

			lPiecesScore[(char)lCase.Color] += (PiecesValues[(char)lCase.PieceType] * 2) + PieceSquareTables[(char)lCase.Color][(char)lCase.PieceType][i];
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

	MovesList lMoveList;
	lMoveList.Count = 0;
	pChessGame->addAllMoves(&lMoveList, (Color)lTabIndex, false);
	lTotalScore += lPiecesScore[lTabIndex] + lMoveList.Count;

	lMoveList.Count = 0;
	pChessGame->addAllMoves(&lMoveList, (Color)lEnemyTabIndex, false);
	lTotalScore -= lPiecesScore[lEnemyTabIndex] + lMoveList.Count;

	return lTotalScore;
}