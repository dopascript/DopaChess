#include "OpeningBook.h"
#include <QFile>

using namespace DopaChess;

OpeningBook::OpeningBook()
{
	loadBasicLines();
}

std::vector<Move> OpeningBook::getMoves(Chessboard* pChessboard)
{
	for (int i = 0; i < mChessboardMoves.size(); i++)
	{
		if (ChessGame::compareChessboard(&mChessboardMoves[i].Chessboard, pChessboard))
		{
			return mChessboardMoves[i].Moves;
		}
	}
	return std::vector<Move>();
}

void OpeningBook::addLine(std::string pLine)
{
	ChessGame lGame;
	lGame.initChessboard();
	for (int i = 0;i < pLine.length() / 5;i++)
	{

		int lIndex = i * 5;
		unsigned char pos1 = ( 7 -(pLine[lIndex] - 'a')) + ((pLine[lIndex + 1] - '1') * 8);
		unsigned char pos2 = (7 - (pLine[lIndex + 2] - 'a')) + ((pLine[lIndex + 3] - '1') * 8);
		Move lMove = lGame.createMove(pos1, pos2);
		addMoveToChessboard(*lGame.getChessboard(), lMove);
		lGame.applyMove(lMove);
	}
}

void OpeningBook::addMoveToChessboard(Chessboard pChessboard, Move pMoves)
{
	for (int i = 0;i < mChessboardMoves.size();i++)
	{
		if (ChessGame::compareChessboard(&mChessboardMoves[i].Chessboard, &pChessboard))
		{
			for (int j = 0;j < mChessboardMoves[i].Moves.size();j++)
			{
				if (memcmp(&mChessboardMoves[i].Moves[j], &pMoves,sizeof(Move)) == 0)
				{
					return;
				}
			}
			mChessboardMoves[i].Moves.push_back(pMoves);
			return;
		}
	}
	ChessboardMoves lChessboardMoves;
	lChessboardMoves.Chessboard = pChessboard;
	lChessboardMoves.Moves.push_back(pMoves);
	mChessboardMoves.push_back(lChessboardMoves);
}

void OpeningBook::loadBasicLines()
{
	if (!QFile::exists("OpeningBook.txt"))
	{
		return;
	}

	QFile lFile("OpeningBook.txt");
	if (!lFile.open(QIODevice::ReadOnly | QIODevice::Text))
		return;

	while (!lFile.atEnd()) {
		QByteArray lLine = lFile.readLine().trimmed();
		if(lLine != "" && !lLine.startsWith("//"))
		{ 
			addLine(lLine.toStdString());
		}
	}
}