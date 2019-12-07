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
	Gold,
	Silver,
	Knight,
	Lance,
	Bishop,
	Rook,
	Pawn,
	King,
	PromSilver,
	PromKnight,
	PromLance,
	PromPawn,
	PromBishop,
	PromRook,
	MaxPiece
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
	bool Hand;		// false:��� true:���
	POSITION Pos;	// ���W
	int32_t Piece;	// ��̎��
	
	struct RECORDINFO()
	{
		Hand = false;
		Pos = { 0, 0 };
		Piece = EPiece::None;
	}
};

std::string Kanji[9];											// �����i�[������
int32_t PieceMap[VERT_NUM][HORI_NUM];							// ������
int32_t WhichHand;												// ���v�萔
bool NowHand;													// ������肩��肩
POSITION TopPiecePos[EHand::MaxHand][HORI_NUM];					// ����肻�ꂼ��̈�ԏ�̋���W
int32_t NowTopHoriNum[EHand::MaxHand];							// ���g�̋�����s�ɉ����邩
POSITION SelectPiecePos[EHand::MaxHand];						// �u�҂����v�p��I�����W
POSITION MoveInputPos[EHand::MaxHand];							// �u�҂����v�p�ړ����W
int32_t BackSavePiece[EHand::MaxHand];							// �҂����p�A���̎�Ŏ�������l�����Ă��瑊��ɖ߂�
int32_t CapturedPieceNum[EHand::MaxHand][EPiece::King];			// ������i�[�ϐ�
RECORDINFO InputRecord[EHand::MaxHand];							// ���͂��ꂽ�L�^
RECORDINFO SaveRecord[MAX_SAVE];								// ���͂����L�^��ۑ�����

void Initialize();												// ������
bool InputFunc(POSITION* pPos);									// ���͊֐�
void InputPos_IsPiece(int32_t PieceMap[VERT_NUM][HORI_NUM],
					POSITION* pInputPos, bool Hand);			// ����邩�̔���
bool InputPos_IsMovePiece(int32_t PieceMap[VERT_NUM][HORI_NUM],
					POSITION* pInputPos, bool Hand);			// ������邩�̔���
void Update(bool Hand);											// �X�V����
bool BackCheck(bool Hand);										// �u�҂����v�̔���
bool CapPieceConfirm(POSITION InputPos, bool Hand);				// ������ƂȂ邩�̊m�F
bool IsUseCapPiece(int32_t CapPieceNum[EHand::MaxHand][EPiece::King],
										bool Hand);				// ��������g���̂�
bool IsSelectPromPiece(bool Hand);								// �����֘A
bool IsCheck(bool Hand);										// ����/�l�݂��ǂ���
void NowHandDisp(bool Hand);									// ���̎�̕\��
void Draw(int32_t PieceMap[VERT_NUM][HORI_NUM], bool Hand);		// �����Օ`��
void CapPieceDraw(
	int32_t CapPieceNum[EHand::MaxHand][EPiece::King]);			// ������̕\��
void RecordsDraw(bool Hand);									// �ۑ������L�^�̕\��

#endif /* __SHOGI_H__ */

// ���� �r���f��
// �� ��
// �� ��
// �� ��
// �� ��
// �� �� 
// ��
