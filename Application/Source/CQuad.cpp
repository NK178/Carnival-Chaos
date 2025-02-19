#include "CQuad.h"

bool CQuad::flag = false;
CQuad* CQuad::targetquad = nullptr;

CQuad::CQuad(void)
{
	UpL = UpM = UpR = MidL = MidM = MidR = BotL = BotM = BotR = nullptr;
	count = width = height = level = quadtype= 0;
	//pos = CPosition{ 0,0 };
}

CQuad::CQuad(int w, int h, int xpos, int ypos, int level, int quadtype)
{
	UpL = UpM = UpR = MidL = MidM = MidR = BotL = BotM = BotR = nullptr;
	height = h;
	width = w;
	count = 0;
	this->xpos = xpos;
	this->ypos = ypos;
	this->level = level;
	this->quadtype = quadtype;
}

void CQuad::InputGO(GameObject object)
{
	limit.push_back(object);
	count++;
}

void CQuad::PrintStats()
{
	for (int i = 0; i < level; i++) {
		std::cout << "-";
	}
	std::cout << level << ":";
	if (quadtype == 0)
		std::cout << "Root" << " ";
	else if (quadtype == 1)
		std::cout << "UpL" << " ";
	else if (quadtype == 2)
		std::cout << "UpM" << " ";
	else if (quadtype == 3)
		std::cout << "UpR" << " ";
	else if (quadtype == 4)
		std::cout << "MidL" << " ";
	else if (quadtype == 5)
		std::cout << "MidM" << " ";
	else if (quadtype == 6)
		std::cout << "MidR" << " ";
	else if (quadtype == 7)
		std::cout << "BotL" << " ";
	else if (quadtype == 8)
		std::cout << "BotM" << " ";
	else if (quadtype == 9)
		std::cout << "BotR" << " ";

	std::cout << "(" << xpos << "," << ypos << "), ";
	std::cout << "H" << height << "W" << width << " ";
	std::cout << "(" << count << ")" << std::endl;

}

CQuad* CQuad::FindGOAndCollisionCheck(int id)
{
	bool condition = false;
	if (count > 0) {
		for (int i = 0; i < count; i++) {
			if (limit[i].GetID() == id) {
				condition = true;
				break;
			}
		}
	}

	if (condition) {

		if (UpL != nullptr && !flag)
			UpL->FindGO(id);
		if (UpR != nullptr && !flag)
			UpR->FindGO(id);
		if (BotL != nullptr && !flag)
			BotL->FindGO(id);
		if (BotR != nullptr && !flag)
			BotR->FindGO(id);
		//will not come here until GO is found 
		if (!flag) {
			targetquad = this;
			flag = true;
		}

		//reset when hit root
		if (level == 0)
			flag = false;
		return targetquad;
	}

	return nullptr;
}

void CQuad::PrintOtherGOs(int id)
{
	bool check = false;
	std::cout << "GOs in the same quad: ";
	for (int i = 0; i < count; i++) {
		if (id != limit[i].GetID()) {
			std::cout << limit[i].GetID() << ",";
			check = true;
		}
	}
	if (!check)
		std::cout << "No other GOs were found ";
	std::cout << std::endl;
}

GameObject CQuad::GetGO(int iter)
{
	return limit[iter];
}

int CQuad::GetHeight(void)
{
	return height;
}

int CQuad::GetWidth(void)
{
	return width;
}

int CQuad::GetCount(void)
{
	return count;
}

int CQuad::GetDepth(void)
{
	return level;
}

int CQuad::GetXpos(void)
{
	return xpos;
}

int CQuad::GetYpos(void)
{
	return ypos;
}

void CQuad::SetCount(int c)
{
	count = c;
}

void CQuad::IncrementDepth(void)
{
	level += 1;
}


