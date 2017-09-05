#include "ChessGame.h"
#include "OpeningBook.h"

using namespace DopaChess;

DopaChess::Color ChessGame::EnemyColor[2]
{
	DopaChess::Color::Black, DopaChess::Color::White
};

unsigned char ChessGame::CaseLine[]
{
	0,  0,  0,  0,  0,  0,  0,  0,
	1,  1,  1,  1,  1,  1,  1,  1,
	2,  2,  2,  2,  2,  2,  2,  2,
	3,  3,  3,  3,  3,  3,  3,  3,
	4,  4,  4,  4,  4,  4,  4,  4,
	5,  5,  5,  5,  5,  5,  5,  5,
	6,  6,  6,  6,  6,  6,  6,  6,
	7,  7,  7,  7,  7,  7,  7,  7
};

unsigned char ChessGame::CaseColumn[]
{
	0,  1,  2,  3,  4,  5,  6,  7,
	0,  1,  2,  3,  4,  5,  6,  7,
	0,  1,  2,  3,  4,  5,  6,  7,
	0,  1,  2,  3,  4,  5,  6,  7,
	0,  1,  2,  3,  4,  5,  6,  7,
	0,  1,  2,  3,  4,  5,  6,  7,
	0,  1,  2,  3,  4,  5,  6,  7,
	0,  1,  2,  3,  4,  5,  6,  7
};

ChessGame::ChessGame()
{
	initPawnsAttacByCase();
	initMovesDataBase();
}

void ChessGame::initEmptyChessboard()
{
	for (int i = 0; i < 64; i++)
	{
		mChessboard.Cases[i].Empty = true;
	}
	mChessboard.WhiteCastlingShortEnabled = true;
	mChessboard.WhiteCastlingLongEnabled = true;
	mChessboard.BlackCastlingShortEnabled = true;
	mChessboard.BlackCastlingLongEnabled = true;
}

void ChessGame::initChessboard()
{
	mChessboard = getStartChessboard();
}

Chessboard ChessGame::getStartChessboard()
{
	Chessboard lChessboard;
	for (int i = 0; i < 64; i++)
	{
		lChessboard.Cases[i].Empty = true;
		lChessboard.Cases[i].PieceType = PieceType::Pawn;
		lChessboard.Cases[i].Color = Color::White;
	}
	lChessboard.WhiteCastlingShortEnabled = true;
	lChessboard.WhiteCastlingLongEnabled = true;
	lChessboard.BlackCastlingShortEnabled = true;
	lChessboard.BlackCastlingLongEnabled = true;

	for (int i = 8; i < 16; i++)
	{
		lChessboard.Cases[i].Empty = false;
		lChessboard.Cases[i].Color = Color::White;
		lChessboard.Cases[i].PieceType = PieceType::Pawn;
	}

	for (int i = 48; i < 56; i++)
	{
		lChessboard.Cases[i].Empty = false;
		lChessboard.Cases[i].Color = Color::Black;
		lChessboard.Cases[i].PieceType = PieceType::Pawn;
	}

	lChessboard.Cases[0].Empty = false;
	lChessboard.Cases[0].Color = Color::White;
	lChessboard.Cases[0].PieceType = PieceType::Rook;
	lChessboard.Cases[7].Empty = false;
	lChessboard.Cases[7].Color = Color::White;
	lChessboard.Cases[7].PieceType = PieceType::Rook;
	lChessboard.Cases[56].Empty = false;
	lChessboard.Cases[56].Color = Color::Black;
	lChessboard.Cases[56].PieceType = PieceType::Rook;
	lChessboard.Cases[63].Empty = false;
	lChessboard.Cases[63].Color = Color::Black;
	lChessboard.Cases[63].PieceType = PieceType::Rook;

	lChessboard.Cases[1].Empty = false;
	lChessboard.Cases[1].Color = Color::White;
	lChessboard.Cases[1].PieceType = PieceType::Knight;
	lChessboard.Cases[6].Empty = false;
	lChessboard.Cases[6].Color = Color::White;
	lChessboard.Cases[6].PieceType = PieceType::Knight;
	lChessboard.Cases[57].Empty = false;
	lChessboard.Cases[57].Color = Color::Black;
	lChessboard.Cases[57].PieceType = PieceType::Knight;
	lChessboard.Cases[62].Empty = false;
	lChessboard.Cases[62].Color = Color::Black;
	lChessboard.Cases[62].PieceType = PieceType::Knight;

	lChessboard.Cases[2].Empty = false;
	lChessboard.Cases[2].Color = Color::White;
	lChessboard.Cases[2].PieceType = PieceType::Bishop;
	lChessboard.Cases[5].Empty = false;
	lChessboard.Cases[5].Color = Color::White;
	lChessboard.Cases[5].PieceType = PieceType::Bishop;
	lChessboard.Cases[58].Empty = false;
	lChessboard.Cases[58].Color = Color::Black;
	lChessboard.Cases[58].PieceType = PieceType::Bishop;
	lChessboard.Cases[61].Empty = false;
	lChessboard.Cases[61].Color = Color::Black;
	lChessboard.Cases[61].PieceType = PieceType::Bishop;

	lChessboard.Cases[4].Empty = false;
	lChessboard.Cases[4].Color = Color::White;
	lChessboard.Cases[4].PieceType = PieceType::Queen;
	lChessboard.Cases[60].Empty = false;
	lChessboard.Cases[60].Color = Color::Black;
	lChessboard.Cases[60].PieceType = PieceType::Queen;

	lChessboard.Cases[3].Empty = false;
	lChessboard.Cases[3].Color = Color::White;
	lChessboard.Cases[3].PieceType = PieceType::King;
	lChessboard.Cases[59].Empty = false;
	lChessboard.Cases[59].Color = Color::Black;
	lChessboard.Cases[59].PieceType = PieceType::King;
	lChessboard.KingsPosition[(unsigned char)Color::White] = 3;
	lChessboard.KingsPosition[(unsigned char)Color::Black] = 59;

	return lChessboard;
}

Chessboard* ChessGame::getChessboard()
{
	return &mChessboard;
}

void ChessGame::setChessboard(Chessboard pChessboard)
{
	mChessboard = pChessboard;
}

Case ChessGame::getCaseInfo(unsigned char pCaseNumber)
{
	return mChessboard.Cases[pCaseNumber];
}

MovesList ChessGame::getMoves(unsigned char pCaseNumber)
{
	MovesList lMovesList;
	lMovesList.Count = 0;
	addMoves(&lMovesList, pCaseNumber, true);
	return lMovesList;
}

bool ChessGame::caseIsDangerous(Color pColor, int pCaseNumber)
{
	Color lEnemyColor = EnemyColor[(char)pColor];
	int lCaseNumber = pCaseNumber;
	Case lCase;

	MovesDatabaseCaseList* lPawnAttacCases = &mPawnsAttacByCase[pCaseNumber][(unsigned char)pColor];
	for (int i = 0; i < lPawnAttacCases->CasesCount; i++)
	{
		Case lCase = mChessboard.Cases[lPawnAttacCases->Cases[i]];
		if (lCase.Empty == false && lCase.Color == lEnemyColor && lCase.PieceType == PieceType::Pawn)
		{
			return true;
		}
	}

	MovesDatabaseCase lMovesDatabaseCaseBishop = mMoveDatadase.Pieces[(unsigned char)PieceType::Bishop].Cases[pCaseNumber];
	for (int lOrientation = 0; lOrientation < lMovesDatabaseCaseBishop.OrientationsCount; lOrientation++)
	{
		if (findLastCaseByOrientation(pCaseNumber, &lMovesDatabaseCaseBishop.Orientations[lOrientation], &lCase))
		{
			if (lCase.Color == lEnemyColor && (lCase.PieceType == PieceType::Bishop || lCase.PieceType == PieceType::Queen))
			{
				return true;
			}
		}
	}

	MovesDatabaseCase lMovesDatabaseCaseRook = mMoveDatadase.Pieces[(unsigned char)PieceType::Rook].Cases[pCaseNumber];
	for (int lOrientation = 0; lOrientation < lMovesDatabaseCaseRook.OrientationsCount; lOrientation++)
	{
		if (findLastCaseByOrientation(pCaseNumber, &lMovesDatabaseCaseRook.Orientations[lOrientation], &lCase))
		{
			if (lCase.Color == lEnemyColor && (lCase.PieceType == PieceType::Rook || lCase.PieceType == PieceType::Queen))
			{
				return true;
			}
		}
	}

	MovesDatabaseCase lMovesDatabaseCaseKnight = mMoveDatadase.Pieces[(unsigned char)PieceType::Knight].Cases[pCaseNumber];
	for (int lOrientation = 0; lOrientation < lMovesDatabaseCaseKnight.OrientationsCount; lOrientation++)
	{
		if (findLastCaseByOrientation(pCaseNumber, &lMovesDatabaseCaseKnight.Orientations[lOrientation], &lCase))
		{
			if (lCase.Color == lEnemyColor && lCase.PieceType == PieceType::Knight)
			{
				return true;
			}
		}
	}

	MovesDatabaseCase lMovesDatabaseCaseKing = mMoveDatadase.Pieces[(unsigned char)PieceType::King].Cases[pCaseNumber];
	for (int lOrientation = 0; lOrientation < lMovesDatabaseCaseKing.OrientationsCount; lOrientation++)
	{
		if (findLastCaseByOrientation(pCaseNumber, &lMovesDatabaseCaseKing.Orientations[lOrientation], &lCase))
		{
			if (lCase.Color == lEnemyColor && lCase.PieceType == PieceType::King)
			{
				return true;
			}
		}
	}

	return false;
}

bool ChessGame::findLastCaseByOrientation(unsigned char pCaseNumber, MovesDatabaseCaseList* pOrientations, Case *pLastCast)
{
	for (int lDestinationNumber = 0; lDestinationNumber < pOrientations->CasesCount; lDestinationNumber++)
	{
		unsigned char lDestinationCase = pOrientations->Cases[lDestinationNumber];
		if (!mChessboard.Cases[lDestinationCase].Empty)
		{
			*pLastCast = mChessboard.Cases[lDestinationCase];
			return true;
		}
	}
	return false;
}

bool ChessGame::isCheck(Color pColor)
{
	unsigned char lKingPosition = mChessboard.KingsPosition[(unsigned char)pColor];
	return caseIsDangerous(pColor, lKingPosition);
}

void ChessGame::addAllMoves(MovesList* pMovesList, Color pColor, bool pTestCheck)
{
	pMovesList->Count = 0;
	for (int i = 0; i < 64; i++)
	{
		if (mChessboard.Cases[i].Empty == false &&
			mChessboard.Cases[i].Color == pColor)
		{
			addMoves(pMovesList, i, pTestCheck);
		}
	}
}

void ChessGame::addMoves(MovesList* pMovesList, unsigned char caseNumber, bool pTestCheck)
{
	switch (mChessboard.Cases[caseNumber].PieceType)
	{
	case PieceType::King:
		addKingMoves(pMovesList, caseNumber, pTestCheck);
		break;
	case PieceType::Queen:
		addQueenMoves(pMovesList, caseNumber, pTestCheck);
		break;
	case PieceType::Rook:
		addRookMoves(pMovesList, caseNumber, pTestCheck);
		break;
	case PieceType::Bishop:
		addBishopMoves(pMovesList, caseNumber, pTestCheck);
		break;
	case PieceType::Knight:
		addKnightMoves(pMovesList, caseNumber, pTestCheck);
		break;
	case PieceType::Pawn:
		addPawnMoves(pMovesList, caseNumber, pTestCheck);
		break;
	}
}

void ChessGame::addPawnMoves(MovesList* pMovesList, unsigned char pCaseNumber, bool pTestCheck)
{
	Color lColor = mChessboard.Cases[pCaseNumber].Color;
	unsigned char lIncrement = lColor == Color::White ? 8 : -8;
	unsigned char lNextCaseNumber = pCaseNumber;
	unsigned char lIterations = 1;
	if ((lColor == Color::White && pCaseNumber < 16) ||
		(lColor == Color::Black && pCaseNumber > 47))
	{
		lIterations = 2;
	}
	for (int i = 0; i < lIterations; i++)
	{
		lNextCaseNumber += lIncrement;
		if (mChessboard.Cases[lNextCaseNumber].Empty)
		{
			Move lMove = createMove(pCaseNumber, lNextCaseNumber);
			addMove(pMovesList, lMove, pTestCheck);
		}
		else
		{
			break;
		}
	}

	Color lEnemyColor = lColor == Color::Black ? Color::White : Color::Black;
	unsigned char lColumnNumber = CaseColumn[pCaseNumber];
	if (lColor == Color::White)
	{
		if (lColumnNumber > 0)
		{
			Case lCase = mChessboard.Cases[pCaseNumber + 7];
			if (!lCase.Empty && lCase.Color != lColor)
			{
				Move lMove = createMove(pCaseNumber, pCaseNumber + 7);
				addMove(pMovesList, lMove, pTestCheck);
			}
		}
		if (lColumnNumber < 7)
		{
			Case lCase = mChessboard.Cases[pCaseNumber + 9];
			if (!lCase.Empty && lCase.Color != lColor)
			{
				Move lMove = createMove(pCaseNumber, pCaseNumber + 9);
				addMove(pMovesList, lMove, pTestCheck);
			}
		}
	}
	else
	{
		if (lColumnNumber > 0)
		{
			Case lCase = mChessboard.Cases[pCaseNumber - 9];
			if (!lCase.Empty && lCase.Color != lColor)
			{
				Move lMove = createMove(pCaseNumber, pCaseNumber - 9);
				addMove(pMovesList, lMove, pTestCheck);
			}
		}
		if (lColumnNumber < 7)
		{
			Case lCase = mChessboard.Cases[pCaseNumber - 7];
			if (!lCase.Empty && lCase.Color != lColor)
			{
				Move lMove = createMove(pCaseNumber, pCaseNumber - 7);
				addMove(pMovesList, lMove, pTestCheck);
			}
		}
	}
}

void ChessGame::addQueenMoves(MovesList* pMovesList, unsigned char pCaseNumber, bool pTestCheck)
{
	addMovesFromDatabase(pMovesList, pCaseNumber, pTestCheck);
}

void ChessGame::addRookMoves(MovesList* pMovesList, unsigned char pCaseNumber, bool pTestCheck)
{
	addMovesFromDatabase(pMovesList, pCaseNumber, pTestCheck);
}

void ChessGame::addKnightMoves(MovesList* pMovesList, unsigned char pCaseNumber, bool pTestCheck)
{
	addMovesFromDatabase(pMovesList, pCaseNumber, pTestCheck);
}

void ChessGame::addBishopMoves(MovesList* pMovesList, unsigned char pCaseNumber, bool pTestCheck)
{
	addMovesFromDatabase(pMovesList, pCaseNumber, pTestCheck);
}

void ChessGame::addKingMoves(MovesList* pMovesList, unsigned char pCaseNumber, bool pTestCheck)
{
	addMovesFromDatabase(pMovesList, pCaseNumber, pTestCheck);

	if (pTestCheck)
	{
		//White Castling Short
		if (mChessboard.WhiteCastlingShortEnabled &&
			pCaseNumber == 3 &&
			mChessboard.Cases[1].Empty && mChessboard.Cases[2].Empty &&
			mChessboard.Cases[0].Empty == false && mChessboard.Cases[0].Color == Color::White && mChessboard.Cases[0].PieceType == PieceType::Rook &&
			!caseIsDangerous(Color::White, 2))
		{
			Move lMove = createMove(3, 1);
			addMove(pMovesList, lMove, pTestCheck);
		}

		//White Castling Long
		if (mChessboard.WhiteCastlingLongEnabled &&
			pCaseNumber == 3 &&
			mChessboard.Cases[4].Empty && mChessboard.Cases[5].Empty && mChessboard.Cases[6].Empty &&
			mChessboard.Cases[7].Empty == false && mChessboard.Cases[7].Color == Color::White && mChessboard.Cases[7].PieceType == PieceType::Rook &&
			!caseIsDangerous(Color::White, 5))
		{

			Move lMove = createMove(3, 5);
			addMove(pMovesList, lMove, pTestCheck);
		}

		//Black Castling Short
		if (mChessboard.BlackCastlingShortEnabled &&
			pCaseNumber == 59 &&
			mChessboard.Cases[57].Empty && mChessboard.Cases[58].Empty &&
			mChessboard.Cases[56].Empty == false && mChessboard.Cases[56].Color == Color::Black && mChessboard.Cases[56].PieceType == PieceType::Rook &&
			!caseIsDangerous(Color::Black, 58))
		{
			Move lMove = createMove(59, 57);
			addMove(pMovesList, lMove, pTestCheck);
		}

		//Black Castling Long
		if (mChessboard.BlackCastlingLongEnabled &&
			pCaseNumber == 59 &&
			mChessboard.Cases[60].Empty && mChessboard.Cases[61].Empty && mChessboard.Cases[62].Empty &&
			mChessboard.Cases[63].Empty == false && mChessboard.Cases[63].Color == Color::Black && mChessboard.Cases[63].PieceType == PieceType::Rook &&
			!caseIsDangerous(Color::Black, 60))
		{
			Move lMove = createMove(59, 61);
			addMove(pMovesList, lMove, pTestCheck);
		}
	}

}

void ChessGame::addMovesFromDatabase(MovesList* pMovesList, unsigned char pCaseNumber, bool pTestCheck)
{
	Case lCase = mChessboard.Cases[pCaseNumber];
	Color lColor = lCase.Color;
	Color lEnemyColor = EnemyColor[(char)lColor];
	MovesDatabaseCase* lMovesDatabaseCase = &mMoveDatadase.Pieces[(unsigned char)lCase.PieceType].Cases[pCaseNumber];
	for (int lOrientationNum = 0; lOrientationNum < lMovesDatabaseCase->OrientationsCount; lOrientationNum++)
	{
		MovesDatabaseCaseList* lOrientation = &lMovesDatabaseCase->Orientations[lOrientationNum];
		for (int lDestinationNum = 0; lDestinationNum < lOrientation->CasesCount; lDestinationNum++)
		{
			unsigned char lNewPosition = lOrientation->Cases[lDestinationNum];
			if (!mChessboard.Cases[lNewPosition].Empty)
			{
				if (mChessboard.Cases[lNewPosition].Color == lEnemyColor)
				{
					Move lMove = createMove(pCaseNumber, lNewPosition);
					addMove(pMovesList, lMove, pTestCheck);
				}
				break;
			}
			else
			{
				Move lMove = createMove(pCaseNumber, lNewPosition);
				addMove(pMovesList, lMove, pTestCheck);
			}
		}
	}
}

Move ChessGame::createMove(int pCase1Number, int pCase2Number)
{
	Move lMove;

	lMove.Case1Number = pCase1Number;
	lMove.Case1Before = mChessboard.Cases[pCase1Number];
	lMove.Case1After.Empty = true;
	lMove.Case2Number = pCase2Number;
	lMove.Case2Before = mChessboard.Cases[pCase2Number];
	lMove.Case2After.Empty = false;
	lMove.Case2After.Color = mChessboard.Cases[pCase1Number].Color;
	lMove.Case2After.PieceType = mChessboard.Cases[pCase1Number].PieceType;

	lMove.Case3Used = false;
	lMove.Case4Used = false;

	lMove.WhiteCastlingShortEnabledBefore = mChessboard.WhiteCastlingShortEnabled;
	lMove.WhiteCastlingShortEnabledAfter = mChessboard.WhiteCastlingShortEnabled;
	lMove.WhiteCastlingLongEnabledBefore = mChessboard.WhiteCastlingLongEnabled;
	lMove.WhiteCastlingLongEnabledAfter = mChessboard.WhiteCastlingLongEnabled;
	lMove.BlackCastlingShortEnabledBefore = mChessboard.BlackCastlingShortEnabled;
	lMove.BlackCastlingShortEnabledAfter = mChessboard.BlackCastlingShortEnabled;
	lMove.BlackCastlingLongEnabledBefore = mChessboard.BlackCastlingLongEnabled;
	lMove.BlackCastlingLongEnabledAfter = mChessboard.BlackCastlingLongEnabled;

	//Castling
	if (lMove.Case1Before.PieceType == PieceType::King)
	{
		//White Castling Short
		if (pCase1Number == 3 && pCase2Number == 1)
		{
			lMove.Case3Used = true;
			lMove.Case4Used = true;

			lMove.Case3Number = 0;
			lMove.Case3Before = mChessboard.Cases[0];
			lMove.Case3After.Empty = true;

			lMove.Case4Number = 2;
			lMove.Case4Before = mChessboard.Cases[2];
			lMove.Case4After.Empty = false;
			lMove.Case4After.Color = Color::White;
			lMove.Case4After.PieceType = PieceType::Rook;
		}
		//White Castling Long
		else if (pCase1Number == 3 && pCase2Number == 5)
		{
			lMove.Case3Used = true;
			lMove.Case4Used = true;

			lMove.Case3Number = 7;
			lMove.Case3Before = mChessboard.Cases[7];
			lMove.Case3After.Empty = true;

			lMove.Case4Number = 4;
			lMove.Case4Before = mChessboard.Cases[4];
			lMove.Case4After.Empty = false;
			lMove.Case4After.Color = Color::White;
			lMove.Case4After.PieceType = PieceType::Rook;
		}
		//Black Castling Short
		if (pCase1Number == 59 && pCase2Number == 57)
		{
			lMove.Case3Used = true;
			lMove.Case4Used = true;

			lMove.Case3Number = 56;
			lMove.Case3Before = mChessboard.Cases[56];
			lMove.Case3After.Empty = true;

			lMove.Case4Number = 58;
			lMove.Case4Before = mChessboard.Cases[58];
			lMove.Case4After.Empty = false;
			lMove.Case4After.Color = Color::Black;
			lMove.Case4After.PieceType = PieceType::Rook;
		}
		//Black Castling Long
		else if (pCase1Number == 59 && pCase2Number == 61)
		{
			lMove.Case3Used = true;
			lMove.Case4Used = true;

			lMove.Case3Number = 63;
			lMove.Case3Before = mChessboard.Cases[63];
			lMove.Case3After.Empty = true;

			lMove.Case4Number = 60;
			lMove.Case4Before = mChessboard.Cases[60];
			lMove.Case4After.Empty = false;
			lMove.Case4After.Color = Color::Black;
			lMove.Case4After.PieceType = PieceType::Rook;
		}

		if (lMove.Case1Before.Color == Color::White)
		{
			lMove.WhiteCastlingShortEnabledAfter = false;
			lMove.WhiteCastlingLongEnabledAfter = false;
		}
		else
		{
			lMove.BlackCastlingShortEnabledAfter = false;
			lMove.BlackCastlingLongEnabledAfter = false;
		}
	}
	if (lMove.Case1Before.PieceType == PieceType::Rook)
	{
		if (lMove.Case1Before.Color == Color::White)
		{
			if (pCase1Number == 0)
			{
				lMove.WhiteCastlingShortEnabledAfter = false;
			}
			else if (pCase1Number == 7)
			{
				lMove.WhiteCastlingLongEnabledAfter = false;
			}
		}
		else
		{
			if (pCase1Number == 56)
			{
				lMove.BlackCastlingShortEnabledAfter = false;
			}
			else if (pCase1Number == 63)
			{
				lMove.BlackCastlingLongEnabledAfter = false;
			}
		}
	}


	return lMove;
}

void ChessGame::movesListAdd(MovesList* pMovesList, Move pMove)
{
	pMovesList->Moves[pMovesList->Count] = pMove;
	pMovesList->Count++;
}

void ChessGame::applyMove(Move pMove)
{
	mChessboard.Cases[pMove.Case1Number] = pMove.Case1After;
	mChessboard.Cases[pMove.Case2Number] = pMove.Case2After;
	if (pMove.Case3Used)
	{
		mChessboard.Cases[pMove.Case3Number] = pMove.Case3After;
	}
	if (pMove.Case4Used)
	{
		mChessboard.Cases[pMove.Case4Number] = pMove.Case4After;
	}
	mChessboard.WhiteCastlingShortEnabled = pMove.WhiteCastlingShortEnabledAfter;
	mChessboard.WhiteCastlingLongEnabled = pMove.WhiteCastlingLongEnabledAfter;
	mChessboard.BlackCastlingShortEnabled = pMove.BlackCastlingShortEnabledAfter;
	mChessboard.BlackCastlingLongEnabled = pMove.BlackCastlingLongEnabledAfter;

	if (pMove.Case1Before.PieceType == PieceType::King)
	{
		mChessboard.KingsPosition[(unsigned char)pMove.Case1Before.Color] = pMove.Case2Number;
	}
}

void ChessGame::cancelMove(Move pMove)
{
	mChessboard.Cases[pMove.Case1Number] = pMove.Case1Before;
	mChessboard.Cases[pMove.Case2Number] = pMove.Case2Before;
	if (pMove.Case3Used)
	{
		mChessboard.Cases[pMove.Case3Number] = pMove.Case3Before;
	}
	if (pMove.Case4Used)
	{
		mChessboard.Cases[pMove.Case4Number] = pMove.Case4Before;
	}
	mChessboard.WhiteCastlingShortEnabled = pMove.WhiteCastlingShortEnabledBefore;
	mChessboard.WhiteCastlingLongEnabled = pMove.WhiteCastlingLongEnabledBefore;
	mChessboard.BlackCastlingShortEnabled = pMove.BlackCastlingShortEnabledBefore;
	mChessboard.BlackCastlingLongEnabled = pMove.BlackCastlingLongEnabledBefore;

	if (pMove.Case1Before.PieceType == PieceType::King)
	{
		mChessboard.KingsPosition[(unsigned char)pMove.Case1Before.Color] = pMove.Case1Number;
	}
}

void ChessGame::addMove(MovesList* pMovesList, Move pMove, bool pTestCheck)
{
	bool lIsCheck;
	if (pTestCheck)
	{
		applyMove(pMove);
		lIsCheck = isCheck(pMove.Case1Before.Color);
		cancelMove(pMove);
	}


	if (!lIsCheck || !pTestCheck)
	{
		//Promotion
		if (pMove.Case1Before.PieceType == PieceType::Pawn &&
			((pMove.Case1Before.Color == Color::White && CaseLine[pMove.Case1Number] == 6) ||
			(pMove.Case1Before.Color == Color::Black && CaseLine[pMove.Case1Number] == 1)))
		{
			pMove.Case2After.PieceType = PieceType::Queen;
			movesListAdd(pMovesList, pMove);
			pMove.Case2After.PieceType = PieceType::Bishop;
			movesListAdd(pMovesList, pMove);
			pMove.Case2After.PieceType = PieceType::Knight;
			movesListAdd(pMovesList, pMove);
			pMove.Case2After.PieceType = PieceType::Rook;
			movesListAdd(pMovesList, pMove);
		}
		else
		{
			movesListAdd(pMovesList, pMove);
		}
	}
}

bool ChessGame::isCheckMate(Color pColor)
{
	MovesList lMoveList;
	lMoveList.Count = 0;
	addAllMoves(&lMoveList, pColor, true);
	if (lMoveList.Count > 0)
	{
		return false;
	}
	return isCheck(pColor);
}

bool ChessGame::isPat(Color pColor)
{
	MovesList lMoveList;
	lMoveList.Count = 0;
	addAllMoves(&lMoveList, pColor, true);
	if (lMoveList.Count > 0)
	{
		return false;
	}
	return !isCheck(pColor);
}

void ChessGame::initMovesDataBase()
{
	for (unsigned char lPieceNum = 0; lPieceNum < 6; lPieceNum++)
	{
		for (unsigned char lCase = 0; lCase < 66; lCase++)
		{
			switch ((PieceType)lPieceNum)
			{
			case PieceType::Rook:
				mMoveDatadase.Pieces[lPieceNum].Cases[lCase].OrientationsCount = 0;
				initMovesDataBaseRook(&mMoveDatadase.Pieces[lPieceNum].Cases[lCase], lCase);
				break;
			case PieceType::Bishop:
				mMoveDatadase.Pieces[lPieceNum].Cases[lCase].OrientationsCount = 0;
				initMovesDataBaseBishop(&mMoveDatadase.Pieces[lPieceNum].Cases[lCase], lCase);
				break;
			case PieceType::Knight:
				mMoveDatadase.Pieces[lPieceNum].Cases[lCase].OrientationsCount = 0;
				initMovesDataBaseKnight(&mMoveDatadase.Pieces[lPieceNum].Cases[lCase], lCase);
				break;
			case PieceType::King:
				mMoveDatadase.Pieces[lPieceNum].Cases[lCase].OrientationsCount = 0;
				initMovesDataBaseKing(&mMoveDatadase.Pieces[lPieceNum].Cases[lCase], lCase);
				break;
			case PieceType::Queen:
				mMoveDatadase.Pieces[lPieceNum].Cases[lCase].OrientationsCount = 0;
				initMovesDataBaseQueen(&mMoveDatadase.Pieces[lPieceNum].Cases[lCase], lCase);
				break;
			case PieceType::Pawn:
				mMoveDatadase.Pieces[lPieceNum].Cases[lCase].OrientationsCount = 0;
				initMovesDataBasePawn(&mMoveDatadase.Pieces[lPieceNum].Cases[lCase], lCase);
				break;
			}

		}
	}
}

void ChessGame::initPawnsAttacByCase()
{
	for (int i = 0; i < 64; i++)
	{
		MovesDatabaseCaseList *lWhiteCaseList = &mPawnsAttacByCase[i][(unsigned char)Color::White];
		MovesDatabaseCaseList *lBlackCaseList = &mPawnsAttacByCase[i][(unsigned char)Color::Black];

		lWhiteCaseList->CasesCount = 0;
		lBlackCaseList->CasesCount = 0;

		if (CaseColumn[i] > 0)
		{
			lWhiteCaseList->Cases[lWhiteCaseList->CasesCount] = i + 7;
			lBlackCaseList->Cases[lBlackCaseList->CasesCount] = i - 9;
			lWhiteCaseList->CasesCount++;
			lBlackCaseList->CasesCount++;
		}

		if (CaseColumn[i] < 7)
		{
			lWhiteCaseList->Cases[lWhiteCaseList->CasesCount] = i + 9;
			lBlackCaseList->Cases[lBlackCaseList->CasesCount] = i - 7;
			lWhiteCaseList->CasesCount++;
			lBlackCaseList->CasesCount++;
		}
	}
}

void ChessGame::addMovesToDatabase(MovesDatabaseCase* pMovesDatabaseCase, unsigned char pCase, char pDecalX, char pDecalY, int pIterationCount)
{
	int lFirst = true;
	unsigned lDecalage = (pDecalY * 8) + pDecalX;
	int lCurrentCase = pCase;
	int lIteration = 0;
	int lOrientationNum = pMovesDatabaseCase->OrientationsCount;
	while (CaseColumn[lCurrentCase] + pDecalX >= 0 && CaseColumn[lCurrentCase] + pDecalX <= 7 &&
		CaseLine[lCurrentCase] + pDecalY >= 0 && CaseLine[lCurrentCase] + pDecalY <= 7 &&
		lIteration < pIterationCount)
	{
		if (lFirst)
		{
			pMovesDatabaseCase->OrientationsCount++;
			pMovesDatabaseCase->Orientations[lOrientationNum].CasesCount = 0;
			lFirst = false;
		}
		lCurrentCase += lDecalage;
		unsigned char lCasesNum = pMovesDatabaseCase->Orientations[lOrientationNum].CasesCount;
		pMovesDatabaseCase->Orientations[lOrientationNum].Cases[lCasesNum] = lCurrentCase;
		pMovesDatabaseCase->Orientations[lOrientationNum].CasesCount++;
		lIteration++;
	}

}

void ChessGame::initMovesDataBaseRook(MovesDatabaseCase* pMovesDatabaseCase, unsigned char pCase)
{
	addMovesToDatabase(pMovesDatabaseCase, pCase, 1, 0, 8);
	addMovesToDatabase(pMovesDatabaseCase, pCase, 0, -1, 8);
	addMovesToDatabase(pMovesDatabaseCase, pCase, -1, 0, 8);
	addMovesToDatabase(pMovesDatabaseCase, pCase, 0, 1, 8);
}

void ChessGame::initMovesDataBaseBishop(MovesDatabaseCase* pMovesDatabaseCase, unsigned char pCase)
{
	addMovesToDatabase(pMovesDatabaseCase, pCase, 1, 1, 8);
	addMovesToDatabase(pMovesDatabaseCase, pCase, 1, -1, 8);
	addMovesToDatabase(pMovesDatabaseCase, pCase, -1, -1, 8);
	addMovesToDatabase(pMovesDatabaseCase, pCase, -1, 1, 8);
}

void ChessGame::initMovesDataBaseQueen(MovesDatabaseCase* pMovesDatabaseCase, unsigned char pCase)
{
	addMovesToDatabase(pMovesDatabaseCase, pCase, 1, 0, 8);
	addMovesToDatabase(pMovesDatabaseCase, pCase, 0, -1, 8);
	addMovesToDatabase(pMovesDatabaseCase, pCase, -1, 0, 8);
	addMovesToDatabase(pMovesDatabaseCase, pCase, 0, 1, 8);
	addMovesToDatabase(pMovesDatabaseCase, pCase, 1, 1, 8);
	addMovesToDatabase(pMovesDatabaseCase, pCase, 1, -1, 8);
	addMovesToDatabase(pMovesDatabaseCase, pCase, -1, -1, 8);
	addMovesToDatabase(pMovesDatabaseCase, pCase, -1, 1, 8);
}

void ChessGame::initMovesDataBaseKnight(MovesDatabaseCase* pMovesDatabaseCase, unsigned char pCase)
{
	addMovesToDatabase(pMovesDatabaseCase, pCase, 2, 1, 1);
	addMovesToDatabase(pMovesDatabaseCase, pCase, 1, 2, 1);
	addMovesToDatabase(pMovesDatabaseCase, pCase, -1, 2, 1);
	addMovesToDatabase(pMovesDatabaseCase, pCase, -2, 1, 1);
	addMovesToDatabase(pMovesDatabaseCase, pCase, -2, -1, 1);
	addMovesToDatabase(pMovesDatabaseCase, pCase, -1, -2, 1);
	addMovesToDatabase(pMovesDatabaseCase, pCase, 1, -2, 1);
	addMovesToDatabase(pMovesDatabaseCase, pCase, 2, -1, 1);
}

void ChessGame::initMovesDataBaseKing(MovesDatabaseCase* pMovesDatabaseCase, unsigned char pCase)
{
	addMovesToDatabase(pMovesDatabaseCase, pCase, 1, 0, 1);
	addMovesToDatabase(pMovesDatabaseCase, pCase, 0, -1, 1);
	addMovesToDatabase(pMovesDatabaseCase, pCase, -1, 0, 1);
	addMovesToDatabase(pMovesDatabaseCase, pCase, 0, 1, 1);
	addMovesToDatabase(pMovesDatabaseCase, pCase, 1, 1, 1);
	addMovesToDatabase(pMovesDatabaseCase, pCase, 1, -1, 1);
	addMovesToDatabase(pMovesDatabaseCase, pCase, -1, -1, 1);
	addMovesToDatabase(pMovesDatabaseCase, pCase, -1, 1, 1);
}

void ChessGame::initMovesDataBasePawn(MovesDatabaseCase* pMovesDatabaseCase, unsigned char pCase)
{

}

void ChessGame::sortMoves(MovesList* pMoves)
{
	for (int i = 0;i < pMoves->Count - 1;i++)
	{
		bool lChanged = false;
		for (int j = 0; j < pMoves->Count - 1; j++)
		{
			if (pMoves->Moves[j].Value < pMoves->Moves[j + 1].Value)
			{
				Move lTmp = pMoves->Moves[j + 1];
				pMoves->Moves[j + 1] = pMoves->Moves[j];
				pMoves->Moves[j] = lTmp;
				lChanged = true;
			}
		}
		if (!lChanged)
		{
			break;
		}
	}
}

bool ChessGame::compareChessboard(Chessboard* pChessboard1, Chessboard* pChessboard2)
{
	for (int i = 0;i < 64;i++)
	{
		if (pChessboard1->Cases[i].Empty == false && pChessboard2->Cases[i].Empty == false)
		{
			continue;
		}
		else if (memcmp(&pChessboard1->Cases[i], &pChessboard2->Cases[i], sizeof(Case)) != 0)
		{
			return false;
		}
	}
	return pChessboard1->BlackCastlingLongEnabled == pChessboard2->BlackCastlingLongEnabled &&
		pChessboard1->BlackCastlingShortEnabled == pChessboard2->BlackCastlingShortEnabled &&
		pChessboard1->WhiteCastlingLongEnabled == pChessboard2->WhiteCastlingLongEnabled &&
		pChessboard1->WhiteCastlingShortEnabled == pChessboard2->WhiteCastlingShortEnabled;
}