#pragma once

namespace DopaChess
{
	class OpeningBook;

	enum class PieceType : unsigned char { King = 0, Queen, Rook, Bishop, Knight, Pawn };
	enum class Color : unsigned char { White = 0, Black };

	struct Case
	{
		bool Empty;
		PieceType PieceType;
		Color Color;
	};

	struct Chessboard
	{
		Case Cases[64];
		bool WhiteCastlingShortEnabled;
		bool WhiteCastlingLongEnabled;
		bool BlackCastlingShortEnabled;
		bool BlackCastlingLongEnabled;

		unsigned char KingsPosition[2];
	};

	struct Move
	{
		unsigned char Case1Number;
		Case Case1Before;
		Case Case1After;
		unsigned char Case2Number;
		Case Case2Before;
		Case Case2After;
		bool Case3Used;
		unsigned char Case3Number;
		Case Case3Before;
		Case Case3After;
		bool Case4Used;
		unsigned char Case4Number;
		Case Case4Before;
		Case Case4After;

		bool WhiteCastlingShortEnabledBefore;
		bool WhiteCastlingShortEnabledAfter;
		bool WhiteCastlingLongEnabledBefore;
		bool WhiteCastlingLongEnabledAfter;
		bool BlackCastlingShortEnabledBefore;
		bool BlackCastlingShortEnabledAfter;
		bool BlackCastlingLongEnabledBefore;
		bool BlackCastlingLongEnabledAfter;

		int Value;
	};

	struct MovesList
	{
		unsigned char Count;
		Move Moves[220];
	};

	struct MovesDatabaseCaseList
	{
		unsigned char CasesCount;
		unsigned char Cases[8];
	};

	struct MovesDatabaseCase
	{
		unsigned char OrientationsCount;
		MovesDatabaseCaseList Orientations[8];
	};

	struct MovesDatabasePiece
	{
		MovesDatabaseCase Cases[64];
	};

	struct MovesDatabase
	{
		MovesDatabasePiece Pieces[6];
	};

	class ChessGame
	{
	public:
		ChessGame();

		void initChessboard();
		void initEmptyChessboard();

		Chessboard* getChessboard();
		void setChessboard(Chessboard pChessboard);

		bool isCheckMate(Color pColor);
		bool isPat(Color pColor);
		bool isCheck(Color pColor);
		bool caseIsDangerous(Color pColor, int pCaseNumber);

		Case getCaseInfo(unsigned char pCaseNumber);
		MovesList getMoves(unsigned char pCaseNumber);
		void addAllMoves(MovesList* pMovesList, Color pColor, bool pTestCheck);

		void applyMove(Move pMove);
		void cancelMove(Move pMove);
		Move createMove(int startCaseNumber, int endCaseNumber);

		static DopaChess::Color EnemyColor[2];
		static unsigned char CaseLine[];
		static unsigned char CaseColumn[];

		static void sortMoves(MovesList* pMoves);
		static bool compareChessboard(Chessboard* pChessboard1, Chessboard* pChessboard2);
		static Chessboard getStartChessboard();

	protected:
		void initMovesDataBase();
		void initMovesDataBaseRook(MovesDatabaseCase* pMovesDatabaseCase, unsigned char pCase);
		void initMovesDataBaseBishop(MovesDatabaseCase* pMovesDatabaseCase, unsigned char pCase);
		void initMovesDataBaseQueen(MovesDatabaseCase* pMovesDatabaseCase, unsigned char pCase);
		void initMovesDataBaseKnight(MovesDatabaseCase* pMovesDatabaseCase, unsigned char pCase);
		void initMovesDataBaseKing(MovesDatabaseCase* pMovesDatabaseCase, unsigned char pCase);
		void initMovesDataBasePawn(MovesDatabaseCase* pMovesDatabaseCase, unsigned char pCase);
		void addMovesToDatabase(MovesDatabaseCase* pMovesDatabaseCase, unsigned char pCase, char pX, char pY, int pIterationCount);
		void initPawnsAttacByCase();

		void addMoves(MovesList* pMovesList, unsigned char pCaseNumber, bool pTestCheck);
		void addPawnMoves(MovesList* pMovesList, unsigned char pCaseNumber, bool pTestCheck);
		void addQueenMoves(MovesList* pMovesList, unsigned char pCaseNumber, bool pTestCheck);
		void addRookMoves(MovesList* pMovesList, unsigned char pCaseNumber, bool pTestCheck);
		void addKnightMoves(MovesList* pMovesList, unsigned char pCaseNumber, bool pTestCheck);
		void addBishopMoves(MovesList* pMovesList, unsigned char pCaseNumber, bool pTestCheck);
		void addKingMoves(MovesList* pMovesList, unsigned char pCaseNumber, bool pTestCheck);
		void addMovesFromDatabase(MovesList* pMovesList, unsigned char pCaseNumber, bool pTestCheck);
		bool findLastCaseByOrientation(unsigned char pCaseNumber, MovesDatabaseCaseList* pOrientations, Case *pLastCast);
	
		void addMove(MovesList* pMovesList, Move pMove, bool pTestCheck);
		void movesListAdd(MovesList* pMovesList, Move pMove);

		Chessboard mChessboard;
		MovesDatabase mMoveDatadase;
		MovesDatabaseCaseList mPawnsAttacByCase[64][2];
	};
}

