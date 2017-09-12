#include "DopaChessGui.h"
#include <QGraphicsScene>
#include <QPainter>
#include <QLabel>
#include <QApplication>
#include <QDesktopWidget>
#include <QMouseEvent>
#include <QThread>
#include "ChessGame.h"

DopaChessGui::DopaChessGui(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	mEnded = false;
	mAiThread = nullptr;
	resize(512, 512 + ui.statusBar->height());
	setFixedSize(512, 512 + ui.statusBar->height());

	mMainLabel = new QLabel(ui.centralWidget);
	mProgressBar = new QProgressBar();
	mProgressBar->hide();
	ui.statusBar->addPermanentWidget(mProgressBar, 2);

	mStatusBarLabel = new QLabel();
	ui.statusBar->addPermanentWidget(mStatusBarLabel, 2);

	mChessGame.initChessboard();
	mChessboardsListHistory.push_back(*mChessGame.getChessboard());

	initDrawingData();

	mMovesList.Count = 0;

	mNextColor = DopaChess::Color::White;
	mGuiState = GuiState::Intro;

	ChessGameAi.setUpdateFindingCallback(std::bind(&DopaChessGui::onUpdateFindingCallback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

	refreshImage();
}

void DopaChessGui::mousePressEvent(QMouseEvent * e)
{
	if (mAiThread != nullptr)
	{
		return;
	}

	QImage lImage;
	switch (mGuiState)
	{
	case GuiState::Intro:
		break;
	case GuiState::Play:
		if (e->button() == Qt::MouseButton::LeftButton)
		{
			mousePressedPlay(e->x(), e->y());
		}
		else
		{
			unsigned char lColumn = 7 - ((e->x() - mMainLabel->x()) / mImageCaseSize);
			unsigned char lLine = 7 - ((e->y() - mMainLabel->y()) / mImageCaseSize);
			mSelectedCase = lColumn + (8 * lLine);
			mGuiState = GuiState::PieceSelection;
			refreshImage();
		}
		break;
	case GuiState::Promotion:
		mousePressedPromotion(e->x(), e->y());
		break;
	case GuiState::PieceSelection:
		mousePieceSelection(e->x(), e->y());
		break;
	default:
		break;
	}
	
}

void DopaChessGui::mousePressedPlay(int pX, int pY)
{
	unsigned char lColumn = 7 - ((pX - mMainLabel->x()) / mImageCaseSize);
	unsigned char lLine = 7 - ((pY - mMainLabel->y()) / mImageCaseSize);
	unsigned char lCaseNumber = (lLine * 8) + lColumn;

	if (mMovesList.Count == 0)
	{
		DopaChess::Case lCase = mChessGame.getCaseInfo(lCaseNumber);
		if (!lCase.Empty && lCase.Color == mNextColor)
		{
			mMovesList = mChessGame.getMoves(lCaseNumber);
		}
	}
	else
	{
		//Is promotion?
		if (mMovesList.Count > 1 && mMovesList.Moves[0].Case1Before.PieceType == DopaChess::PieceType::Pawn && 
			(mMovesList.Moves[0].Case2Number <= 7 || mMovesList.Moves[0].Case2Number >= 56))
		{
			DopaChess::MovesList lFilterMoveList;
			lFilterMoveList.Count = 0;
			for (int i = 0;i < mMovesList.Count;i++)
			{
				if (mMovesList.Moves[i].Case2Number == lCaseNumber)
				{
					lFilterMoveList.Moves[lFilterMoveList.Count] = mMovesList.Moves[i];
					lFilterMoveList.Count++;
				}
			}
			mMovesList = lFilterMoveList;
			mGuiState = GuiState::Promotion;
		}
		else
		{
			for (int i = 0; i < mMovesList.Count; i++)
			{
				if (mMovesList.Moves[i].Case2Number == lCaseNumber)
				{
					mLastMove = mMovesList.Moves[i];
					ExecMove(mMovesList.Moves[i]);
					break;
				}
			}
			mMovesList.Count = 0;
		}
	}
	refreshImage();
}

void DopaChessGui::mousePressedPromotion(int pX, int pY)
{
	if (!mPromotionPanelRect.contains(pX, pY))
	{
		mGuiState = GuiState::Play;
		refreshImage();
		return;
	}
	int lSelectedIndex = (pX - mPromotionPanelRect.x()) / mImageCaseSize;

	DopaChess::PieceType lSelectedPiece;
	switch (lSelectedIndex)
	{
	case 0:
		lSelectedPiece = DopaChess::PieceType::Queen;
		break;
	case 1:
		lSelectedPiece = DopaChess::PieceType::Bishop;
		break;
	case 2:
		lSelectedPiece = DopaChess::PieceType::Rook;
		break;
	case 3:
		lSelectedPiece = DopaChess::PieceType::Knight;
		break;
	default:
		break;
	}

	DopaChess::Move lSelectedMove;
	for (int i = 0;i < mMovesList.Count;i++)
	{
		if (mMovesList.Moves[i].Case2After.PieceType == lSelectedPiece)
		{
			lSelectedMove = mMovesList.Moves[i];
			break;
		}
	}

	mGuiState = GuiState::Play;
	ExecMove(lSelectedMove);
	refreshImage();
}

void DopaChessGui::mousePieceSelection(int pX, int pY)
{
	if (!mPiecePanelRect.contains(pX, pY))
	{
		mGuiState = GuiState::Play;
		refreshImage();
		return;
	}
	int lSelectedIndex = (pX - mPiecePanelRect.x()) / mImageCaseSize;
	int lColorIndex = (pY - mPiecePanelRect.y()) / mImageCaseSize;
	DopaChess::Color lColor = lColorIndex == 0 ? DopaChess::Color::White : DopaChess::Color::Black;
	if (lSelectedIndex > 5)
	{
		mChessGame.getChessboard()->Cases[mSelectedCase].Empty = true;
	}
	else
	{
		mChessGame.getChessboard()->Cases[mSelectedCase].Empty = false;
		mChessGame.getChessboard()->Cases[mSelectedCase].Color = lColor;
		mChessGame.getChessboard()->Cases[mSelectedCase].PieceType = (DopaChess::PieceType)lSelectedIndex;
	}
	mMovesList.Count = 0;
	mChessboardsListHistory.clear();
	mGuiState = GuiState::Play;
	refreshImage();
}

void DopaChessGui::keyPressEvent(QKeyEvent* ke)
{
	if (mAiThread != nullptr)
	{
		return;
	}

	int key = ke->nativeVirtualKey();
	if (mGuiState == GuiState::Intro)
	{
		mGuiState = GuiState::Play;
		refreshImage();
	}
	else if (key == 32) //Space
	{
		if (mEnded || mAiThread != nullptr)
		{
			return;
		}
		mProgressBar->setValue(0);
		mProgressBar->show();
		mStatusBarLabel->setText("");

		mAiThread = new QThread();
		connect(mAiThread, &QThread::started, this, &DopaChessGui::AiPlay, Qt::ConnectionType::DirectConnection);
		mAiThread->start();
	}
	else if (key == 8)
	{
		if (mChessboardsListHistory.size() > 1)
		{
			mEnded = false;
			mChessboardsListHistory.pop_back();
			mChessGame.setChessboard(mChessboardsListHistory.back());
			mNextColor = mNextColor == DopaChess::Color::White ? DopaChess::Color::Black : DopaChess::Color::White;
		}
		refreshImage();
	}
	else if (key == 69) //E
	{
		mEnded = false;
		mChessboardsListHistory.clear();
		mMovesList.Count = 0;
		mChessGame.initEmptyChessboard();
		mChessboardsListHistory.push_back(*mChessGame.getChessboard());
		refreshImage();
	}
	else if (key == 87) //W
	{
		mNextColor = DopaChess::Color::White;
	}
	else if (key == 66) //B
	{
		mNextColor = DopaChess::Color::Black;
	}
	else if (key == 73) //I
	{
		mEnded = false;
		mChessboardsListHistory.clear();
		mMovesList.Count = 0;
		mChessGame.initChessboard();
		mChessboardsListHistory.push_back(*mChessGame.getChessboard());
		refreshImage();
	}
	else if (key == 27 && mGuiState == GuiState::PieceSelection) // Escap
	{
		mGuiState = GuiState::Play;
		refreshImage();
	}
	QMainWindow::keyPressEvent(ke); 
}

void DopaChessGui::AiPlay()
{
	mLastAiResult = ChessGameAi.getNextMove(&mChessGame, mNextColor, mChessboardsListHistory);
	ExecMove(mLastAiResult.Move);

	mAiThread->terminate();
	QMetaObject::invokeMethod(this, "afterAiPlay", Qt::QueuedConnection);
}

void DopaChessGui::afterAiPlay()
{
	delete mAiThread;
	mAiThread = nullptr;

	int lTotalTime = mLastAiResult.Duration / 1000;
	int lEvaluationsCount = mLastAiResult.EvaluationsCount;
	int lMoveBySecond = 0;
	if (mLastAiResult.Duration > 0)
	{
		lMoveBySecond = (lEvaluationsCount / mLastAiResult.Duration) * 1000;
	}

	mProgressBar->hide();

	QString lResultInfo;
	lResultInfo = "Evaluations count : " + QString::number(lEvaluationsCount);
	lResultInfo += ", total time : " + QString::number(lTotalTime) + " seconds";
	lResultInfo += " -> Evaluations/seconds : " + QString::number(lMoveBySecond) + "";
	printf((lResultInfo + "\n").toStdString().c_str());

	refreshImage();
}


void DopaChessGui::onUpdateFindingCallback(int pValue, int pMaxValue, int pEvalCount)
{
	QMetaObject::invokeMethod(this, "updateProgressBar", Qt::QueuedConnection, Q_ARG(int, pValue), Q_ARG(int, pMaxValue), Q_ARG(int, pEvalCount));
}

void DopaChessGui::updateProgressBar(int pValue, int pMaxValue, int pEvalCount)
{
	mProgressBar->setMaximum(pMaxValue);
	mProgressBar->setValue(pValue);

	//QString lEvalCountStr = QString::number(pEvalCount);
	//for (int i = lEvalCountStr.size() - 3; i > 0; i -= 3)
	//	lEvalCountStr.insert(i, " ");
	//mStatusBarLabel->setText(lEvalCountStr + " evaluations");
}

void DopaChessGui::ExecMove(DopaChess::Move pMove)
{
	mChessGame.applyMove(pMove);
	mChessboardsListHistory.push_back(*mChessGame.getChessboard());
	mNextColor = mNextColor == DopaChess::Color::White ? DopaChess::Color::Black : DopaChess::Color::White;
	mMovesList.Count = 0;

	if (mChessGame.isCheckMate(mNextColor))
	{
		if (mNextColor == DopaChess::Color::White)
		{
			mStatusBarLabel->setText("White is Checkmate");
		}
		else
		{
			mStatusBarLabel->setText("Black is Checkmate");
		}

		mEnded = true;
	}
	else if (mChessGame.isPat(mNextColor))
	{
		mStatusBarLabel->setText("PATT");
		mEnded = true;
	}
	else
	{
		if (mNextColor == DopaChess::Color::White)
		{
			mStatusBarLabel->setText("It is White's move");
		}
		else
		{
			mStatusBarLabel->setText("It is Black's move");
		}
	}


}

void DopaChessGui::refreshImage()
{
	QImage lImage;
	switch (mGuiState)
	{
	case GuiState::Intro:
		lImage = drawIntro();
		break;
	case GuiState::Play:
		lImage = drawChessboard();
		break;
	case GuiState::Promotion:
		lImage = drawPromotion();
		break;
	case GuiState::PieceSelection:
		lImage = drawPieceSelection();
		break;
	default:
		break;
	}
	mMainLabel->setPixmap(QPixmap::fromImage(lImage));
}

void DopaChessGui::initDrawingData()
{
	mImageChessboard = QImage("chessBoard.png");
	mImagePieces = QImage("pieces.png");

	mImageCaseSize = mImageChessboard.width() / 8;
	mImagePiecesWidth = mImagePieces.width() / 6;
	mImagePiecesHeight = mImagePieces.height() / 2;

	QSize lPanelSize(mImageCaseSize * 4, mImageCaseSize);
	QPoint lPanelPosition((mImageChessboard.width() - lPanelSize.width()) / 2, (mImageChessboard.height() - lPanelSize.height()) / 2);
	mPromotionPanelRect = QRect(lPanelPosition, lPanelSize);

	QSize lPanel2Size(mImageCaseSize * 7, mImageCaseSize * 2);
	QPoint lPanel2Position((mImageChessboard.width() - lPanel2Size.width()) / 2, (mImageChessboard.height() - lPanel2Size.height()) / 2);
	mPiecePanelRect = QRect(lPanel2Position, lPanel2Size);
}

QImage DopaChessGui::drawChessboard()
{
	QImage lResult = mImageChessboard.copy();
	QPainter lPaint;
	lPaint.begin(&lResult);
	for (int i = 0; i < 64; i++)
	{
		DopaChess::Case lCase = mChessGame.getChessboard()->Cases[i];
		if (lCase.Empty)
		{
			continue;
		}

		int column = 7 - (i % 8);
		int line = 7 - (i / 8);
		QRect lDestination(column * mImageCaseSize, line * mImageCaseSize, mImageCaseSize, mImageCaseSize);

		int lOrigineY = lCase.Color == DopaChess::Color::White ? 0 : mImagePiecesHeight;
		int lOrigineX = ((unsigned char)lCase.PieceType) * mImagePiecesWidth;
		QRect lOrigine(lOrigineX, lOrigineY, mImagePiecesWidth, mImagePiecesHeight);

		lPaint.drawImage(lDestination, mImagePieces, lOrigine);
	}

	for (int i = 0; i < mMovesList.Count; i++)
	{
		int column = 7 - (mMovesList.Moves[i].Case2Number % 8);
		int line = 7 - (mMovesList.Moves[i].Case2Number / 8);
		QRect lRect(column * mImageCaseSize, line * mImageCaseSize, mImageCaseSize, mImageCaseSize);
		lPaint.setPen(QPen(Qt::red, 2));
		lPaint.drawRect(lRect);
	}

	lPaint.end();

	return lResult;
}

QImage DopaChessGui::drawPromotion()
{
	QImage lImage = drawChessboard();
	QPainter lPaint;
	lPaint.begin(&lImage);

	int lColorDecal = mMovesList.Moves[0].Case1Before.Color == DopaChess::Color::White ? 0 : mImageCaseSize;
	lPaint.fillRect(mPromotionPanelRect, Qt::black);
	QRect lSource;
	for (int i = 0;i < 4;i++)
	{
		switch (i)
		{
		case 0:
			lSource = QRect(mImageCaseSize * (unsigned char)DopaChess::PieceType::Queen, lColorDecal,mImageCaseSize, mImageCaseSize);
			break;
		case 1:
			lSource = QRect(mImageCaseSize * (unsigned char)DopaChess::PieceType::Bishop, lColorDecal, mImageCaseSize, mImageCaseSize);
			break;
		case 2:
			lSource = QRect(mImageCaseSize * (unsigned char)DopaChess::PieceType::Rook, lColorDecal, mImageCaseSize, mImageCaseSize);
			break;
		case 3:
			lSource = QRect(mImageCaseSize * (unsigned char)DopaChess::PieceType::Knight, lColorDecal, mImageCaseSize, mImageCaseSize);
			break;
		default:
			break;
		}
		QRect lDestination(mPromotionPanelRect.x() + (i * mImageCaseSize), mPromotionPanelRect.y(), mImageCaseSize, mImageCaseSize);

		lPaint.drawImage(lDestination, mImagePieces, lSource);
	}
	lPaint.end();
	return lImage;
}

QImage DopaChessGui::drawPieceSelection()
{
	QImage lImage = drawChessboard();
	QPainter lPaint;
	lPaint.begin(&lImage);
	lPaint.fillRect(mPiecePanelRect, Qt::black);

	int lColorDecal = mMovesList.Moves[0].Case1Before.Color == DopaChess::Color::White ? 0 : mImageCaseSize;
	for (int i = 0; i < 7; i++)
	{
		QRect lDestinationWhite(mPiecePanelRect.x() + (i * mImageCaseSize), mPiecePanelRect.y(), mImageCaseSize, mImageCaseSize);
		QRect lSourceWhite(i * mImageCaseSize, 0, mImageCaseSize, mImageCaseSize);
		lPaint.drawImage(lDestinationWhite, mImagePieces, lSourceWhite);

		QRect lDestinationBlack(mPiecePanelRect.x() + (i * mImageCaseSize), mPiecePanelRect.y() + mImageCaseSize, mImageCaseSize, mImageCaseSize);
		QRect lSourceBlack(i * mImageCaseSize, mImageCaseSize, mImageCaseSize, mImageCaseSize);
		lPaint.drawImage(lDestinationBlack, mImagePieces, lSourceBlack);
	}
	lPaint.end();
	return lImage;
}

QImage DopaChessGui::drawIntro()
{
	return QImage("title.png");
}