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
			WallCreation(MazeMap, FirstPosition.X, FirstPosition.Y);
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
	//if (FirstPosition.X == MAP_SIZE_X - 2)
	//{
	//	// �}�b�v�𒴂��Ȃ��悤1������
	//	FirstPosition.X = MAP_SIZE_X - 3;
	//}
	//if (FirstPosition.Y == MAP_SIZE_Y - 2)
	//{
	//	// �}�b�v�𒴂��Ȃ��悤1������
	//	FirstPosition.Y = MAP_SIZE_Y - 3;
	//}

	// �S�[������
	GoalFixed_02(MazeMap, FirstPosition);

	// �S�[�����瓹�𐶐�(�P��)
	PathCreate(MazeMap, FirstPosition);

	// �ŏ��̓������グ��܂ŁANextPosition������:�@
	// �@�l���������đS�ē��ł���΍��Ȃ�����(FALSE)��Ԃ��֐������
	// ���I������̏���
	while (itido)
	{
		if (CheckIfCreatable(MazeMap, NextPosition))
		{
			PathCreate(MazeMap, NextPosition);
		}
		else
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

	// �`��
	Draw();
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

	itido = TRUE;
}

void WallCreation(short Map[][MAP_SIZE_X], short x, short y)
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
			WallCreation(Map, x - 2, y);
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
			WallCreation(Map, x, y - 2);
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
			WallCreation(Map, x + 2, y);
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
			WallCreation(Map, x, y + 2);
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
	NextPosition = Position;
	short SaveDirection = 0;

	// ���[�v�p�ϐ�
	short j;	// ��
	short i;	// �c

	// 2�}�X��̏�Ԃ��ǂȂ�A���ɂ��邱�Ƃ��o����
	// �J�������̌���
	OpenDirection = rand() % EDirection::Dir_Max;
	SaveDirection = OpenDirection;

	// Next���ς���Ă��Ȃ�(�������Ȃ�����)/4���������s������Open���ꏄ�����ꍇ���[�v�I��
	while ((Position.X == NextPosition.X && Position.Y == NextPosition.Y) ||
		SaveDirection == OpenDirection)
	{
		// �O��̍��W�Ɠ����ł���Ώ������΂�if�������
		switch (OpenDirection)
		{
		case EDirection::Dir_Left:
			if (Map[Position.Y][Position.X - 2] == Map[SavePosition.Y][SavePosition.X])
				break;

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
			}
			break;
		case EDirection::Dir_Up:
			if (Map[Position.Y - 2][Position.X] == Map[SavePosition.Y][SavePosition.X])
				break;

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
				NextPosition.Y = Position.Y - 2;
			}
			break;
		case EDirection::Dir_Right:
			// �z��͈̔͊O�ɍs���Ȃ��悤��
			if (Position.X == 26)
			{
				if (Map[Position.Y][Position.X + 1] == Map[SavePosition.Y][SavePosition.X])
					break;

				// ���ɂ��鏈��
				if (Map[Position.Y][Position.X + 1] == EChip::Chip_Wall)
				{
					for (j = Position.X; j <= Position.X + 1; j++)
					{
						if (Map[Position.Y][j] != EChip::Chip_Goal)
						{
							Map[Position.Y][j] = EChip::Chip_Path;
							// �T���ς݂ł���
							SearchedMap[Position.Y][j] = Map[Position.Y][j];
						}
					}
					NextPosition.X = Position.X + 1;
				}
			}
			else
			{
				if (Map[Position.Y][Position.X + 2] == Map[SavePosition.Y][SavePosition.X])
					break;

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
				}
			}
			break;
		case EDirection::Dir_Down:
			// �z��͈̔͊O�ɍs���Ȃ��悤��
			if (Position.Y == 26)
			{
				if (Map[Position.Y + 1][Position.X] == Map[SavePosition.Y][SavePosition.X])
					break;

				if (Map[Position.Y + 1][Position.X] == EChip::Chip_Wall)
				{
					// ���ɂ��鏈��
					for (i = Position.Y; i <= Position.Y + 1; i++)
					{
						if (Map[i][Position.X] != EChip::Chip_Goal)
						{
							Map[i][Position.X] = EChip::Chip_Path;
							// �T���ς݂ł���
							SearchedMap[i][Position.X] = Map[i][Position.X];
						}
					}
					// ����T�����W�̍X�V
					NextPosition.Y = Position.Y + 1;
				}
			}
			else
			{
				if (Map[Position.Y + 2][Position.X] == Map[SavePosition.Y][SavePosition.X])
					break;

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
					NextPosition.Y = Position.Y + 2;
				}
			}
			break;
		default:
			break;
		}
		// 0,1,2,3
		OpenDirection = ++OpenDirection % 4;
		// �����Đ���
		//OpenDirection = rand() % EDirection::Dir_Max;
	}

	// ����̍��W��ۑ�
	SavePosition = Position;
}

void StartFixed_01(short Map[][MAP_SIZE_X])
{
	short s_x = rand() % MAP_SIZE_X;
	short s_y = rand() % MAP_SIZE_Y;

	// �I�΂ꂽ�ԍ����ǂ������ꍇ��蒼��
	while (Map[s_y][s_x] == EChip::Chip_Wall)
	{
		s_x = rand() % MAP_SIZE_X;
		s_y = rand() % MAP_SIZE_Y;
	}

	Map[s_y][s_x] = EChip::Chip_Start;
}

void GoalFixed_01(short Map[][MAP_SIZE_X])
{
	short g_x = 0;
	short g_y = 0;
	bool AgeinFlag = FALSE;

	// �I�΂ꂽ�ԍ����ǂ������ꍇ��蒼��
	while (AgeinFlag || Map[g_y][g_x] == EChip::Chip_Wall)
	{
		g_x = rand() % MAP_SIZE_X;
		g_y = rand() % MAP_SIZE_Y;

		// 5�}�X��/��/��/�����}�b�v�O�������ꍇ��蒼��
		if (g_x + 5 > MAP_SIZE_X || g_x - 5 < 0 ||
			g_y + 5 > MAP_SIZE_Y || g_y - 5 < 0)
		{
			AgeinFlag = TRUE;
			continue;
		}

		// ��5�}�X�̍��E���X�^�[�g���ǂ���������


		/*else
		{
			// �c��5�}�X�ȓ��ɃX�^�[�g���������ꍇ��蒼��
			for (short j = g_y; j < g_y + 5; j++)
			{
				for (short i = g_x; i < g_x + 5; i++)
				{
					if (Map[j][i] == EChip::Chip_Start)
					{
						AgeinFlag = TRUE;
					}
					else
					{
						AgeinFlag = FALSE;
					}
				}
				for (short i = g_x; i > g_x - 5; i--)
				{
					if (i > MAP_SIZE_X)
					{
						AgeinFlag = TRUE;
						break;
					}
					if (Map[j][i] == EChip::Chip_Start)
					{
						AgeinFlag = TRUE;
					}
					else
					{
						AgeinFlag = FALSE;
					}
				}
			}
			for (short j = g_y; j > g_y - 5; j--)
			{
				for (short i = g_x; i < g_x + 5; i++)
				{
					if (Map[j][i] == EChip::Chip_Start)
					{
						AgeinFlag = TRUE;
					}
					else
					{
						AgeinFlag = FALSE;
					}
				}
				for (short i = g_x; i > g_x - 5; i--)
				{
					if (i > MAP_SIZE_X)
					{
						AgeinFlag = TRUE;
						break;
					}
					if (Map[j][i] == EChip::Chip_Start)
					{
						AgeinFlag = TRUE;
					}
					else
					{
						AgeinFlag = FALSE;
					}
				}
			}
		}*/
	}

	Map[g_y][g_x] = EChip::Chip_Goal;
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

	std::cout << NextPosition.X << '\n' << NextPosition.Y << '\n';

	//std::cout << "Hello World!\n";
	//std::cout << FirstPosition.x << '\n' << FirstPosition.y << '\n';
	//int x = 0;
	//for (int i = 0; i < 10; i++)
	//{
	//	x = (rand() % (MAP_SIZE_X - 18) + 2) * 2;
	//	std::cout << "�����e�X�g:" << x << '\n';
	//}

}

// �l���������Đ����\���ǂ���������
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