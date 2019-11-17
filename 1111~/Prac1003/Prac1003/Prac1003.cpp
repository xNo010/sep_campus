#include <iostream>

struct POINT
{
	int32_t x;
	int32_t y;
};

POINT DoubleValue(POINT Value)
{
	POINT Double = Value;
	Double.x *= 2;
	Double.y *= 2;
	return Double;
}

int main()
{
	POINT Val;

	printf("数値入力\n");
	printf("x値\n");
	scanf_s("%d", &Val.x);
	printf("y値\n");
	scanf_s("%d", &Val.y);

	printf("x:%d y:%d", DoubleValue(Val).x, DoubleValue(Val).y);
}