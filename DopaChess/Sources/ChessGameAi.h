#pragma once
#include "ChessGame.h"
#include "OpeningBook.h"
#include <vector>
#include <functional>

namespace DopaChess
{
	struct AiResult
	{
		Move Move;
		unsigned int Duration;
		unsigned int EvaluationsCount;
	};


	class ChessGameAi
	{
	public:
		ChessGameAi();
		AiResult getNextMove(ChessGame *pChessGame, Color pColor, std::vector<DopaChess::Chessboard> mChessboardsListHistory);
		void setUpdateFindingCallback(std::function<void(int value, int maxValue, int evalCount)> pCallback);
	protected:
		MovesList getMovesWithEvaluation(ChessGame *pChessGame, Color pColor);
		int evaluationFunctionBasic(ChessGame *pChessGame, Color pColor);
		bool ChessGameAi::moveAlreadyPlayed(ChessGame *pChessGame, const std::vector<DopaChess::Chessboard> &pChessboardsListHistory, DopaChess::Move pMove);
		int alphaBeta(ChessGame *pChessGame, Color pColor, int pDepth, int pAlpha, int pBeta, bool pMax);
		DopaChess::OpeningBook mOpeningBook;

		std::function<void(int value, int maxValue, int evalCount)> mUpdateFindingCallback;

		unsigned int mEvaluationsCount;
		bool mUseOpeningBook;
	};
}
