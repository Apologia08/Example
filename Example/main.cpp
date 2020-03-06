#include <iostream>

const char stageData[] = "\
########\n\
# .. p #\n\
# oo   #\n\
#      #\n\
########";

const int stageWidth = 8;
const int stageHeight = 5;

enum Object
{
	OBJ_SPACE,
	OBJ_WALL,
	OBJ_GOAL,
	OBJ_BLOCK,
	OBJ_BLOCK_ON_GOAL,
	OBJ_MAN,
	OBJ_MAN_ON_GOAL,

	OBJ_UNKNOWN,
};

void Initialize(Object* state, int w, int h, const char* stageData);
void Draw(const Object* state, int w, int h);
void Update(Object* state, char input, int w, int h);
bool CheckClear(const Object* state, int w, int h);

using namespace std;

int main()
{
	Object* state = new Object[stageWidth * stageHeight];

	while (true)
	{
		Draw(state, stageWidth, stageHeight);

		if (CheckClear(state, stageWidth, stageHeight))
		{
			break;
		}

		cout << "a:left d:right w:up s:down commmand?" << endl;
		char input;
		cin >> input;
		Update(state, input, stageWidth, stageHeight);
	}

	cout << "You win" << endl;
	delete[] state;
	state = 0;

	return 0;
}

void Initialize(Object* state, int w, int h, const char* stageData)
{
	//const char의 포인터라는 뜻(주소 값 변경 가능)
	const char* d = stageData;
	int x = 0;
	int y = 0;

	while (*d != '\0')
	{
		Object t;
		switch (*d)
		{
		case'#':
			t = OBJ_WALL;
			break;
		case' ':
			t = OBJ_SPACE;
			break;
		case'o':
			t = OBJ_BLOCK;
			break;
		case'0':
			t = OBJ_BLOCK_ON_GOAL;
			break;
		case'.':
			t = OBJ_GOAL;
			break;
		case'p':
			t = OBJ_MAN;
			break;
		case'P':
			t = OBJ_MAN_ON_GOAL;
			break;
		case'\n':
			x = 0;
			y++;
			t = OBJ_UNKNOWN;
			break;
		default:
			t = OBJ_UNKNOWN;
			break;
		}

		d++;

		if (t != OBJ_UNKNOWN)
		{
			state[x + y * w] = t;
			x++;
		}
	}
}

void Draw(const Object* state, int w, int h)
{
	const char font[] = { ' ','#','.','o','0','p','P' };

	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			Object o = state[j + i * w];
			cout << font[o];
		}
		cout << endl;
	}
}

void Update(Object* state, char input, int w, int h)
{
	int dx = 0, dy = 0;

	switch (input)
	{
	case 'a':
		dx = -1;
		break;
	case 'd':
		dx = 1;
		break;
	case 'w':
		dy = -1;
		break;
	case 's':
		dy = 1;
		break;
	default:
		break;
	}

	int i = -1;

	for (i = 0; i < w * h; i++)
	{
		if (state[i] == OBJ_MAN || state[i] == OBJ_MAN_ON_GOAL)
		{
			break;
		}
	}

	int x = i % w;
	int y = i / w;

	int tx = x + dx;
	int ty = y + dy;

	if (tx < 0 || tx >= w || ty < 0 || ty >= h)
	{
		return;
	}

	int p = x + y * w;
	int tp = tx + ty * w;

	if (state[tp] == OBJ_SPACE || state[tp] == OBJ_GOAL)
	{
		state[tp] = (state[tp] == OBJ_GOAL) ? OBJ_MAN_ON_GOAL : OBJ_MAN;

		state[p] = (state[p] == OBJ_MAN_ON_GOAL) ? OBJ_GOAL : OBJ_SPACE;
	}
	else if (state[tp] == OBJ_BLOCK || state[tp] == OBJ_BLOCK_ON_GOAL)
	{
		int tx2 = tx + dx;
		int ty2 = ty + dy;

		if (tx2 < 0 || tx2 >= w || ty2 < 0 || ty2 >= h)
		{
			return;
		}

		int tp2 = ty2 * w + tx2;

		if (state[tp2] == OBJ_SPACE || state[tp2] == OBJ_GOAL)
		{
			state[tp2] = (state[tp2] == OBJ_GOAL) ? OBJ_BLOCK_ON_GOAL : OBJ_BLOCK;
			state[tp] = (state[tp] == OBJ_BLOCK_ON_GOAL) ? OBJ_MAN_ON_GOAL : OBJ_MAN;
			state[p] = (state[p] == OBJ_MAN_ON_GOAL) ? OBJ_GOAL : OBJ_SPACE;
		}
	}
}

bool CheckClear(const Object* state, int w, int h)
{
	for (int i = 0; i < w * h; i++)
	{
		if (state[i] == OBJ_BLOCK)
		{
			return false;
		}
	}
	return true;
}