#pragma once
#include "ChessGame.h"
#include "OpeningBook.h"
#include <vector>

namespace DopaChess
{
	struct AiResult
	{
		Move Move;
		unsigned int Duration;
		unsigned int MovesCount;
	};


	class ChessGameAi
	{
	public:
		ChessGameAi();
		AiResult getNextMove(ChessGame *pChessGame, Color pColor, std::vector<DopaChess::Chessboard> mChessboardsListHistory);
	protected:
		MovesList getMovesWithEvaluation(ChessGame *pChessGame, Color pColor);
		int evaluationFunctionBasic(ChessGame *pChessGame, Color pColor);
		bool ChessGameAi::moveAlreadyPlayed(ChessGame *pChessGame, const std::vector<DopaChess::Chessboard> &pChessboardsListHistory, DopaChess::Move pMove);
		int alphaBeta(ChessGame *pChessGame, Color pColor, int pDepth, int pAlpha, int pBeta, bool pMax);
		DopaChess::OpeningBook mOpeningBook;

		unsigned int mMovesCount;
		bool mUseOpeningBook;
	};
}
