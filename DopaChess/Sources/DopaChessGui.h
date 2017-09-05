#pragma once

#include <QtWidgets/QMainWindow>
#include <qlabel.h>
#include <QKeyEvent>
#include <QThread>
#include <QList>
#include <QDateTime>
#include "ui_DopaChess.h"
#include "ChessGame.h"
#include "OpeningBook.h"

enum class GuiState : unsigned char { Intro = 0, Play, Promotion, Edit, PieceSelection };

class DopaChessGui : public QMainWindow
{
	Q_OBJECT

public:
	DopaChessGui(QWidget *parent = Q_NULLPTR);
	
private:
	virtual void mousePressEvent(QMouseEvent * e);
	void mousePressedPlay(int pX, int pY);
	void mousePressedPromotion(int pX, int pY);
	void mousePieceSelection(int pX, int pY);

	virtual void keyPressEvent(QKeyEvent* ke);

	Q_SLOT void AiPlay();
	Q_SLOT void afterAiPlay();

	void refreshImage();
	void ExecMove(DopaChess::Move pMove);

	void initDrawingData();
	QImage drawChessboard();
	QImage drawPromotion();
	QImage drawPieceSelection();

	bool moveAlreadyPlayed(DopaChess::Move pMove);

	DopaChess::ChessGame mChessGame;
	DopaChess::MovesList mMovesList;
	QList<DopaChess::Chessboard> mChessboardsListHistory;
	DopaChess::Color mNextColor;

	QLabel* mMainLabel;
	Ui::DopaChessClass ui;
	DopaChess::Move mLastMove;
	bool mEnded;
	QThread* mAiThread;
	QDateTime mStartComputing;
	GuiState mGuiState;
	int mSelectedCase;

	int mImageCaseSize;
	int mImagePiecesWidth;
	int mImagePiecesHeight;
	QImage mImageChessboard;
	QImage mImagePieces;

	QRect mPromotionPanelRect;
	QRect mPiecePanelRect;

	DopaChess::OpeningBook mOpeningBook;
};
