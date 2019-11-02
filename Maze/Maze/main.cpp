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

void Maze01()
{
	// ������
	Initialize_01();

	// --- �}�b�v�������� ---
	// x:2�`13 * 2(����)
	// y:2�`13 * 2(����)
	FirstPosition.X = (rand() % (MAP_SIZE_X - 18) + 2) * 2;
	FirstPosition.Y = (rand() % (MAP_SIZE_Y - 18) + 2) * 2;

	for (short j = 2; j < MAP_SIZE_Y - 3; j += 2)
	{
		for (short i = 2; i < MAP_SIZE_X - 3; i += 2)
		{
			FirstPosition.X = i;
			FirstPosition.Y = j;
			WallCreation_01(MazeMap, FirstPosition.X, FirstPosition.Y);
		}
	}

	// �X�^�[�g�ƃS�[���̎w��
	//StartFixed(MazeMap);
	//GoalFixed(MazeMap);

	// �`��
	Draw();
}

void Maze02()
{
	Initialize_02();

	// --- �}�b�v�������� ---
	// x:((0�`11) + 2) * 2(����)
	// y:((0�`11) + 2) * 2(����)
	FirstPosition.X = (rand() % (MAP_SIZE_X - 18) + 2) * 2;
	FirstPosition.Y = (rand() % (MAP_SIZE_Y - 18) + 2) * 2;

	// �f�o�b�O�p
	FirstPosition.X = 24;
	FirstPosition.Y = 12;

	// �S�[������
	GoalFixed_02(MazeMap, FirstPosition);

	// �ŏ��̓������ 1.
	// 1.���[�v�񐔂ōŏ�������ȊO�����f
	// �l���������đS�ē��ł���΍��Ȃ�����(FALSE)��Ԃ��֐������
	while (itido)
	{
		if (LoopTimes == 0)
		{
			// �S�[�����瓹�𐶐�
			PathCreate(MazeMap, FirstPosition);
		}
		else
		{
			// if (CheckIfCreatable(MazeMap, NextPosition))
			// {
			// 	PathCreate(MazeMap, NextPosition);
			// }
			// else
			{
				itido = FALSE;
				//// �������Đ���
				//// x:((0�`12) + 2) * 2(����)
				//// y:((0�`12) + 2) * 2(����)
				//NextPosition.X = (rand() % (MAP_SIZE_X - 17) + 2) * 2;
				//NextPosition.Y = (rand() % (MAP_SIZE_Y - 17) + 2) * 2;
				//if (NextPosition.X == MAP_SIZE_X - 2)
				//{
				//	// �}�b�v�𒴂��Ȃ��悤1������
				//	NextPosition.X = MAP_SIZE_X - 3;
				//}
				//if (NextPosition.Y == MAP_SIZE_Y - 2)
				//{
				//	// �}�b�v�𒴂��Ȃ��悤1������
				//	NextPosition.Y = MAP_SIZE_Y - 3;
				//}
				//
				//// �������u�S�[���łȂ��A���łɍ��ꂽ���v�ƂȂ��Ă���ꏊ�ł����
				//if (SearchedMap[NextPosition.Y][NextPosition.X] != EChip::Chip_Goal ||
				//	SearchedMap[NextPosition.Y][NextPosition.X] == EChip::Chip_Path)
				//{
				//	// ����������H�̐������ĊJ
				//	PathCreate(MazeMap, NextPosition);
				//	itido = FALSE;
				//}
			}
		}
		LoopTimes++;
	}

	// �`��
	Draw();

	int a = 0;
}

void Initialize_01()
{
	// �}�b�v
	for (short i = 0; i < MAP_SIZE_Y; i++)
	{
		for (short j = 0; j < MAP_SIZE_X; j++)
		{
			// �ǂ��}�b�v�̒[�ɏ����Ŕz�u����
			if (i == 0 || j == 0 || i == MAP_SIZE_Y - 1 || j == MAP_SIZE_X - 1)
			{
				MazeMap[i][j] = EChip::Chip_Wall;
			}
			else
			{
				MazeMap[i][j] = EChip::Chip_Path;
			}
		}
	}

	FirstPosition.X = FirstPosition.Y = 0;
}

void Initialize_02()
{
	// �}�b�v
	for (short i = 0; i < MAP_SIZE_Y; i++)
	{
		for (short j = 0; j < MAP_SIZE_X; j++)
		{
			// �����l�͒[�ɓ���z�u����
			if (i == 0 || j == 0 || i == MAP_SIZE_Y - 1 || j == MAP_SIZE_X - 1)
			{
				MazeMap[i][j] = EChip::Chip_Path;
			}
			else
			{
				MazeMap[i][j] = EChip::Chip_Wall;
			}

			SearchedMap[i][j] = EChip::Chip_Wall;
		}
	}

	FirstPosition.X = FirstPosition.Y = 0;
	NextPosition.X = NextPosition.Y = 0;
	SavePosition.X = SavePosition.Y = 0;
	OpenDirection = 0;

	LoopTimes = 0;

	itido = TRUE;
}

void WallCreation_01(short Map[][MAP_SIZE_X], short x, short y)
{
	if (Map[y][x] != EChip::Chip_Path)
	{
		return;
	}
	// �����_���ȏꏊ�ɕǂ�z�u
	Map[y][x] = EChip::Chip_Wall;

	// ���������̌���
	short Direction = rand() % 3;//EDirection::Dir_Max;

	// ���ꂼ��2�}�X������āA���ł���Ηׂɕǂ𐶐�����
	switch (Direction)
	{
	case EDirection::Dir_Left:
		if (Map[y][x - 2] == EChip::Chip_Path)
		{
			Map[y][x - 1] = EChip::Chip_Wall;
			WallCreation_01(Map, x - 2, y);
		}
		else if (Map[y][x - 2] == EChip::Chip_Wall)
		{
			Map[y][x - 1] = EChip::Chip_Wall;
		}
		break;
	case EDirection::Dir_Up:
		if (Map[y - 2][x] == EChip::Chip_Path)
		{
			Map[y - 1][x] = EChip::Chip_Wall;
			WallCreation_01(Map, x, y - 2);
		}
		else if (Map[y - 2][x] == EChip::Chip_Wall)
		{
			Map[y - 1][x] = EChip::Chip_Wall;
		}
		break;
	case EDirection::Dir_Right:
		if (Map[y][x + 2] == EChip::Chip_Path)
		{
			Map[y][x + 1] = EChip::Chip_Wall;
			WallCreation_01(Map, x + 2, y);
		}
		else if (Map[y][x + 2] == EChip::Chip_Wall)
		{
			Map[y][x + 1] = EChip::Chip_Wall;
		}
		break;
	case EDirection::Dir_Down:
		if (Map[y + 2][x] == EChip::Chip_Path)
		{
			Map[y + 1][x] = EChip::Chip_Wall;
			WallCreation_01(Map, x, y + 2);
		}
		else if (Map[y + 2][x] == EChip::Chip_Wall)
		{
			Map[y + 2][x] = EChip::Chip_Wall;
		}
		break;
	default:
		break;
	}
}

void PathCreate(short Map[][MAP_SIZE_X], COORD Position)
{
	// while���̏����p
	short SaveDirection = 0;
	bool ReCreateFlag = TRUE;
	short LoopCount = 0;		// �������[�v�΍�
	COORD StartPosMax = {24, 24};
	// ���[�v�p�ϐ�
	short j;	// ��
	short i;	// �c

	// 2�}�X��̏�Ԃ��ǂȂ�A���ɂ��邱�Ƃ��o����
	// �J�������̌���
	OpenDirection = rand() % EDirection::Dir_Max;
	SaveDirection = OpenDirection;

	// Next���ő�̐��ȊO�̒l/Next���J�n���W�ς���Ă��Ȃ�(�������Ȃ�����)/�Đ��������𖞂����Ă���ꍇ���[�v���s
	while (/*(NextPosition.X != StartPosMax.X && NextPosition.Y != StartPosMax.Y) ||*/
		(Position.X == NextPosition.X && Position.Y == NextPosition.Y) || 
		ReCreateFlag)
	{
		switch (OpenDirection)
		{
		case EDirection::Dir_Left:
			// �O��̍��W�Ɠ����ł���Ώ������΂�
			if (Map[Position.Y][Position.X - 2] == Map[SavePosition.Y][SavePosition.X])
			{
				ReCreateFlag = TRUE;
				break;
			}

			if (Map[Position.Y][Position.X - 2] == EChip::Chip_Wall)
			{
				// ���ɂ��鏈��
				for (j = Position.X; j >= Position.X - 2; j--)
				{
					if (Map[Position.Y][j] != EChip::Chip_Goal)
					{
						Map[Position.Y][j] = EChip::Chip_Path;
						// �T���ς݂ł���
						SearchedMap[Position.Y][j] = Map[Position.Y][j];
					}
				}
				NextPosition.X = Position.X - 2;
				NextPosition.Y = Position.Y;

				// �Đ����̕K�v�Ȃ�
				ReCreateFlag = FALSE;
			}
			break;
		case EDirection::Dir_Up:
			// �O��̍��W�Ɠ����ł���Ώ������΂�
			if (Map[Position.Y - 2][Position.X] == Map[SavePosition.Y][SavePosition.X])
			{
				ReCreateFlag = TRUE;
				break;
			}

			if (Map[Position.Y - 2][Position.X] == EChip::Chip_Wall)
			{
				// ���ɂ��鏈��
				for (i = Position.Y; i >= Position.Y - 2; i--)
				{
					if (Map[i][Position.X] != EChip::Chip_Goal)
					{
						Map[i][Position.X] = EChip::Chip_Path;
						// �T���ς݂ł���
						SearchedMap[Position.Y][i] = Map[Position.Y][i];
					}
				}
				NextPosition.X = Position.X;
				NextPosition.Y = Position.Y - 2;

				// �Đ����̕K�v�Ȃ�
				ReCreateFlag = FALSE;
			}
			break;
		case EDirection::Dir_Right:
			// �O��̍��W�Ɠ����ł���Ώ������΂�
			if (Map[Position.Y][Position.X + 2] == Map[SavePosition.Y][SavePosition.X])
			{
				ReCreateFlag = TRUE;
				break;
			}

			if (Map[Position.Y][Position.X + 2] == EChip::Chip_Wall)
			{
				// ���ɂ��鏈��
				for (j = Position.X; j <= Position.X + 2; j++)
				{
					if (Map[Position.Y][j] != EChip::Chip_Goal)
					{
						Map[Position.Y][j] = EChip::Chip_Path;
						// �T���ς݂ł���
						SearchedMap[Position.Y][j] = Map[Position.Y][j];
					}
				}
				NextPosition.X = Position.X + 2;
				NextPosition.Y = Position.Y;

				// �Đ����̕K�v�Ȃ�
				ReCreateFlag = FALSE;
			}
			break;
		case EDirection::Dir_Down:
			// �O��̍��W�Ɠ����ł���Ώ������΂�
			if (Map[Position.Y + 2][Position.X] == Map[SavePosition.Y][SavePosition.X])
			{
				ReCreateFlag = TRUE;
				break;
			}

			// ���ɂ��鏈��
			if (Map[Position.Y + 2][Position.X] == EChip::Chip_Wall)
			{
				for (i = Position.Y; i <= Position.Y + 2; i++)
				{
					if (Map[i][Position.X] != EChip::Chip_Goal)
					{
						Map[i][Position.X] = EChip::Chip_Path;
						// �T���ς݂ł���
						SearchedMap[i][Position.X] = Map[i][Position.X];
					}
				}
				// ����T�����W�̍X�V
				NextPosition.X = Position.X;
				NextPosition.Y = Position.Y + 2;

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
			// �����Đ���
			//OpenDirection = rand() % EDirection::Dir_Max;

			// �ꏄ�����ꍇ
			if (SaveDirection == OpenDirection)
			{
				printf("�ꏄ�����̂ŏI���\n");
				ReCreateFlag = FALSE;
			}

			LoopCount++;
			if (LoopCount == 100)
			{
				printf("�Đ������������[�v�����̂ŏI���\n");
				break;
			}
		}

		// �Ō�Ƀ|�W�V�����̒���
		// 28����n�܂�Ɣz��O�ɏo�Ă��܂��\��������
		if (NextPosition.X == StartPosMax.X)
		{
			NextPosition.X = StartPosMax.X - 2;
		}
		if (NextPosition.Y == StartPosMax.Y)
		{
			NextPosition.Y = StartPosMax.Y - 2;
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

// �S�[���̌���
void GoalFixed_02(short Map[][MAP_SIZE_X], COORD Position)
{
	if (Position.X < 0 || Position.X > MAP_SIZE_X - 1 ||
		Position.Y < 0 || Position.Y > MAP_SIZE_Y - 1)
	{
		printf("Map�z��͈̔͊O�ł��B\n");
		return;
	}

	Map[Position.Y][Position.X] = EChip::Chip_Goal;
	SearchedMap[Position.Y][Position.X] = Map[Position.Y][Position.X];
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

	//std::cout << "Hello World!\n";
	//std::cout << FirstPosition.x << '\n' << FirstPosition.y << '\n';
	//int x = 0;
	//for (int i = 0; i < 10; i++)
	//{
	//	x = (rand() % (MAP_SIZE_X - 18) + 2) * 2;
	//	std::cout << "�����e�X�g:" << x << '\n';
	//}

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