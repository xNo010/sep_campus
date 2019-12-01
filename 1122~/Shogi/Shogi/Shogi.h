#pragma once
#ifndef __SHOGI_H__
#define __SHOGI_H__

#include <iostream>
#include <string>
#include <Windows.h>

#define VERT_NUM		9		// �c�̐�
#define HORI_NUM		9		// ���̐�
#define MAX_SAVE		100		// �ő�ۑ���

enum EPiece			// ��̎��
{
	None,
	King,
	Gold,
	Silver,
	Knight,
	Lance,
	Bishop,
	Rook,
	Pawn,
	Max
};

enum EHand			// �����
{
	First,
	Second,
	MaxHand
};

struct POSITION		// ���W
{
	int32_t x;
	int32_t y;

	POSITION() { x = 0, y = 0; };

	POSITION(int32_t ax, int32_t ay)
	{
		x = ax;
		y = ay;
	}

	void operator = (const POSITION& p)
	{
		x = p.x;
		y = p.y;
	}

	POSITION operator - (const POSITION& p) const
	{
		return POSITION(x - p.x, y - p.y);
	}

	int32_t operator == (const POSITION& p) const
	{
		return x == p.x && y == p.y;
	}
	int32_t operator != (const POSITION& p) const
	{
		return x != p.x || y != p.y;
	}

};

struct RECORDINFO
{
	int32_t Which;	// 0:��� 1:���
	POSITION Pos;	// ���W
	int32_t Piece;	// ��̎��
	
	struct RECORDINFO()
	{
		Which = false;
		Pos = { 0, 0 };
		Piece = EPiece::None;
	}
};

std::string Kanji[9];							// �����i�[
int32_t PieceMap[VERT_NUM][HORI_NUM];			// ������
int32_t WhichHand;								// ���v�萔
int32_t NowHand;								// ������肩��肩�̔��f
POSITION TopPiecePos[EHand::MaxHand][HORI_NUM];	// ����肻�ꂼ��̈�ԏ�̋���W
int32_t NowTopHoriNum[EHand::MaxHand];			// ���ʂ̍ہA������s�ɉ����邩
POSITION SaveInputPos[EHand::MaxHand];			// �u�҂����v�p��I�����W
POSITION MoveInputPos[EHand::MaxHand];			// �u�҂����v�p�ړ����W
RECORDINFO InputRecord;							// ���͂��ꂽ�L�^
RECORDINFO SaveRecord[MAX_SAVE];				// ���͂����L�^��ۑ�����

void Initialize();													// ������
bool InputFunc(POSITION* pPos);										// ���͊֐�
bool InputPos_IfPiece(int32_t PieceMap[VERT_NUM][HORI_NUM]);		// ����邩�̔���
bool InputPos_IfMovePiece(int32_t PieceMap[VERT_NUM][HORI_NUM]);	// ������邩�̔���
void Update();														// �X�V����
bool BackCheck();													// �u�҂����v�̔���
void Draw();														// �}�b�v�`��
void RecordsDraw();													// �ۑ������L�^�̕\��

#endif /* __SHOGI_H__ */

// ���� �r���f��
// �� ��
// �� ��
// �� ��
// �� ��
// �� �� 
// ��
