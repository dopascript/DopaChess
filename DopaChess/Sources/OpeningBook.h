#pragma once

#include <string>
#include <vector>
#include "ChessGame.h"

namespace DopaChess
{
	struct ChessboardMoves
	{
		Chessboard Chessboard;
		std::vector<Move> Moves;
	};

	class OpeningBook
	{
	public:
		OpeningBook();
		std::vector<Move> getMoves(Chessboard* pChessboard);
		void addLine(std::string pLine);
	protected:
		void addMoveToChessboard(Chessboard pChessboard, Move pMoves);
		void loadBasicLines();

		std::vector<ChessboardMoves> mChessboardMoves;
	};
}