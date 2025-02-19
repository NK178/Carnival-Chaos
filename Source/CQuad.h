#pragma once
#include "GameObject.h"
#include <vector>
#include <iostream>

class CQuad
{
private:
	int count;
	int height;
	int width;
	int xpos;
	int ypos;
	std::vector<GameObject> limit;
	int level;
	int quadtype;
	static bool flag;
	static CQuad* targetquad;

public:
	//CPosition pos;
	CQuad* UpL;
	CQuad* UpM;
	CQuad* UpR;
	CQuad* MidL;
	CQuad* MidM;
	CQuad* MidR;
	CQuad* BotL;
	CQuad* BotM;
	CQuad* BotR;
	void InputGO(GameObject object);
	GameObject GetGO(int iter);

	CQuad(void);
	CQuad(int w, int h, int xpos, int ypos, int level, int quadtype);
	int GetHeight(void);
	int GetWidth(void);
	int GetCount(void);
	int GetDepth(void);
	int GetXpos(void);
	int GetYpos(void);
	void SetCount(int c);
	void IncrementDepth(void);

	CQuad* FindGO(int id);

	void PrintStats();
	void PrintOtherGOs(int id);

};

