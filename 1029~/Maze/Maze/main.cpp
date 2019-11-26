#include "Maze01.h"
#include "Maze02.h"

int main()
{
	srand(timeGetTime());

	// �ǂ�������H
	//Maze01();
	
	// ����������H
	Maze02();
}

void Maze02()
{
	Initialize_02();

	// --- �}�b�v�������� ---
	// x:((0�`11) + 2) * 2(����)
	// y:((0�`11) + 2) * 2(����)
	FirstPosition.X = (rand() % (MAP_SIZE_X - 18) + 2) * 2;
	FirstPosition.Y = (rand() % (MAP_SIZE_Y - 18) + 2) * 2;
	COORD StartPosMin = { 0, 0 };	// ���W�ŏ��l
	COORD StartPosMax = { 28, 28 };	// ���W�ő�l

	// �f�o�b�O�p
	//FirstPosition.X = 24;
	//FirstPosition.Y = 12;

	// �S�[������
	GoalFixed_02(MazeMap, FirstPosition);

	// �ŏ��̓������ 1.
	// 1.���[�v�񐔂ōŏ�������ȊO�����f
	// �l���������č��邩�ǂ���(TRUE/FALSE)��Ԃ��֐������
	while (itido)
	{
		if (LoopTimes == 0)
		{
			// �S�[�������}�X�����𐶐�
			PathCreate_02(MazeMap, FirstPosition);
		}
		else
		{
			if (CheckIfCreatable(MazeMap, NextPosition) &&
				(NextPosition.X > StartPosMin.X && StartPosMax.X > NextPosition.X) &&
				(NextPosition.Y > StartPosMin.Y && StartPosMax.Y > NextPosition.Y))
			{
				// ���̍��W���猩�ē��𐶐����J��Ԃ�
				PathCreate_02(MazeMap, NextPosition);
			}
			else
			{
				StartFixed_02(MazeMap, NextPosition);
				itido = FALSE;
			}
		}
		LoopTimes++;
	}

	// �`��
	Draw();
}

void Initialize_02()
{
	// �}�b�v
	for (short i = 0; i < MAP_SIZE_Y; i++)
	{
		for (short j = 0; j < MAP_SIZE_X; j++)
		{
			// �����l�͒[�ɓ���z�u����
			//if (i == 0 || j == 0 || i == MAP_SIZE_Y - 1 || j == MAP_SIZE_X - 1)
			//{
			//	MazeMap[i][j] = EChip::Chip_Path;
			//}
			//else
			{
				MazeMap[i][j] = EChip::Chip_Wall;
			}
		}
	}

	ZeroMemory(SearchedMap, sizeof(SearchedMap));

	FirstPosition.X = FirstPosition.Y = 0;
	NextPosition.X = NextPosition.Y = 0;
	SavePosition.X = SavePosition.Y = 0;
	OpenDirection = 0;

	LoopTimes = 0;

	itido = TRUE;
}

void PathCreate_02(short Map[MAP_SIZE_Y][MAP_SIZE_X], COORD Position)
{
	// while���̏����p
	short SaveDirection = 0;
	bool ReCreateFlag = TRUE;
	short LoopCount = 0;		// �������[�v�΍�
	COORD StartPosMin = { 0, 0 };	// ���W�ŏ��l
	COORD StartPosMax = { 28, 28 };	// ���W�ő�l
	// ���[�v�p�ϐ�
	short j;	// ��
	short i;	// �c

	// 2�}�X��̏�Ԃ��ǂȂ�A���ɂ��邱�Ƃ��o����
	// �J�������̌���
	OpenDirection = rand() % EDirection::Dir_Max;
	SaveDirection = OpenDirection;

	// Position���z��O�ł͂Ȃ�/�Đ��������𖞂����Ă���ꍇ���[�v���s
	while (((Position.X > StartPosMin.X && StartPosMax.X > Position.X) &&
		(Position.Y > StartPosMin.Y && StartPosMax.Y > Position.Y)))// &&
		//ReCreateFlag)
	{
		switch (OpenDirection)
		{
		case EDirection::Dir_Left:
			// ��ԍ��ɂ���̂ł���Ώ������΂�
			if (Position.X <= StartPosMin.X)
			{
				ReCreateFlag = TRUE;
				Map[Position.Y][Position.X] = EChip::Chip_Path;
				break;
			}
			if (Map[Position.Y][Position.X - 2] == EChip::Chip_Wall)
			{
				NextPosition = { Position.X - 2, Position.Y };
				// ���ɂ��鏈��
				for (j = Position.X - 1; j >= NextPosition.X; j--)
				{
					Map[Position.Y][j] = EChip::Chip_Path;
					// �T���ς݂ł���
					//SearchedMap[Position.Y][j] = Map[Position.Y][j];
				}
			
				// �Đ����̕K�v�Ȃ�
				ReCreateFlag = FALSE;
			}
			break;
		case EDirection::Dir_Up:
			// ��ԏ�ɂ���̂ł���Ώ������΂�
			if (Position.Y <= StartPosMin.Y)
			{
				ReCreateFlag = TRUE;
				Map[Position.Y][Position.X] = EChip::Chip_Path;
				break;
			}

			if (Map[Position.Y - 2][Position.X] == EChip::Chip_Wall)
			{
				NextPosition = { Position.X, Position.Y - 2 };
				// ���ɂ��鏈��
				for (i = Position.Y - 1; i >= NextPosition.Y; i--)
				{
					Map[i][Position.X] = EChip::Chip_Path;
					// �T���ς݂ł���
					//SearchedMap[Position.Y][i] = Map[Position.Y][i];
				}

				// �Đ����̕K�v�Ȃ�
				ReCreateFlag = FALSE;
			}
			break;
		case EDirection::Dir_Right:
			// �E�̍ő�l�ɂ���̂ł���Ώ������΂�
			if (Position.X >= StartPosMax.X)
			{
				ReCreateFlag = TRUE;
				Map[Position.Y][Position.X + 1] = EChip::Chip_Path;
				break;
			}

			if (Map[Position.Y][Position.X + 2] == EChip::Chip_Wall)
			{
				NextPosition = { Position.X + 2, Position.Y };
				// ���ɂ��鏈��
				for (j = Position.X + 1; j <= NextPosition.X; j++)
				{
					Map[Position.Y][j] = EChip::Chip_Path;
					// �T���ς݂ł���
					//SearchedMap[Position.Y][j] = Map[Position.Y][j];
				}

				// �Đ����̕K�v�Ȃ�
				ReCreateFlag = FALSE;
			}
			break;
		case EDirection::Dir_Down:
			// �O��̍��W�Ɠ����ł���Ώ������΂�
			if (Position.Y >= StartPosMax.Y)
			{
				ReCreateFlag = TRUE;
				Map[Position.Y + 1][Position.X] = EChip::Chip_Path;
				break;
			}

			// ���ɂ��鏈��
			if (Map[Position.Y + 2][Position.X] == EChip::Chip_Wall)
			{
				NextPosition = { Position.X, Position.Y + 2 };
				for (i = Position.Y + 1; i <= NextPosition.Y; i++)
				{
					Map[i][Position.X] = EChip::Chip_Path;
					// �T���ς݂ł���
					//SearchedMap[i][Position.X] = Map[i][Position.X];
				}

				// �Đ����̕K�v�Ȃ�
				ReCreateFlag = FALSE;
			}
			break;
		default:
			break;
		}

		// �Đ������K�v�ƂȂ�����
		if (ReCreateFlag)
		{
			// 0->1, 1->2, 2->3, 3->0�ňꏄ
			OpenDirection = ++OpenDirection % 4;
			LoopCount++;
			if (LoopCount == 100)
			{
				printf("�Đ������������[�v�����̂ŏI���\n");
				break;
			}
			// ���̍��W(�ۑ����Ă������W)�ɖ߂��Ă�����(�ꏄ�����ꍇ)
			if (SaveDirection == OpenDirection)
			{
				printf("�ꏄ�����̂ŏI���\n");
				ReCreateFlag = FALSE;
			}

			ReCreateFlag = FALSE;
		}
		else
		{
			break;
		}

		if (LoopCount == 100)
		{
			printf("�V���v���ɖ������[�v�����̂ŏI���\n");
			break;
		}
	}

	// ����̍��W��ۑ�
	SavePosition = Position;
}

// �X�^�[�g�̌���
void StartFixed_02(short Map[][MAP_SIZE_X], COORD Position)
{
	Map[Position.Y][Position.X] = EChip::Chip_Start;
}
// �S�[���̌���
void GoalFixed_02(short Map[][MAP_SIZE_X], COORD Position)
{
	Map[Position.Y][Position.X] = EChip::Chip_Goal;
}

void Draw()
{
	for (short i = 0; i < MAP_SIZE_Y; i++)
	{
		for (short j = 0; j < MAP_SIZE_X; j++)
		{
			switch (MazeMap[i][j])
			{
			case EChip::Chip_Path:
				printf("��");
				break;
			case EChip::Chip_Wall:
				printf("��");
				break;
			case EChip::Chip_Start:
				printf("�X");
				break;
			case EChip::Chip_Goal:
				printf("�S");
				break;
			default:
				break;
			}
		}
		printf("\n");
	}

	std::cout << FirstPosition.X << '\n' << FirstPosition.Y << '\n';
	std::cout << "���[�v��:" << LoopTimes << '\n';
}

// �l���������ē��̐������\���ǂ���������
bool CheckIfCreatable(short Map[][MAP_SIZE_X], COORD CheckPos)
{
	// 4�������ꂼ��2�}�X������āA���ɂȂ��Ă�����CheckPos����͍��Ȃ�
	// ��/��/�E/������
	if (Map[CheckPos.Y][CheckPos.X - 2] == EChip::Chip_Path &&
		Map[CheckPos.Y - 2][CheckPos.X] == EChip::Chip_Path &&
		Map[CheckPos.Y][CheckPos.X + 2] == EChip::Chip_Path &&
		Map[CheckPos.Y + 2][CheckPos.X] == EChip::Chip_Path)
	{
		return FALSE;
	}
	return TRUE;
}