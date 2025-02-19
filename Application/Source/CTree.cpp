#include "CTree.h"


CTree::CTree(void)
{
	count = 0;
	totalbreadth = totalwidth = 25;
	root = current = nullptr;
}

CTree::CTree(int maxwidth, int maxbreadth)
{
	count = 0;
	root = current = nullptr;
	totalbreadth = maxbreadth;
	totalwidth = maxwidth;
}

CTree::~CTree(void)
{
	if (root != nullptr) {
		current = root;
		PreOrderDelete(current);
	}
}

void CTree::AddGO(GameObject go)
{

	limit.push_back(go);
	count++;
}

void CTree::Split(CQuad*& Q)
{
    //Check if quad needs to be split 
	int numofGO = Q->GetCount();
	if (numofGO <= 10)
		return;
	//order of input
	std::vector<int> quadinputorder;

	//increase when spliiting 
	int depth = Q->GetDepth() + 1;

	std::vector<GameObject> GOlist;
	for (int i = 0; i < numofGO; i++) {
		GOlist.push_back(Q->GetGO(i));
	}

	//Alot of stuff 
	int UPL, UPM, UPR, MIDL,MIDM, MIDR, BOTL, BOTM, BOTR;
	UPL = UPM = UPR = MIDL = MIDM = MIDR = BOTL = BOTM = BOTR = 0;
	int oldw = Q->GetWidth();
	int oldh = Q->GetHeight();
	int N1width = oldw / 3;
	int N2width = N1width;
	int N3width = oldw - (N1width + N2width);
	int N1height = oldh / 3;
	int N2height = N1height;
	int N3height = oldh - (N1height + N2height);
	int xpos = Q->GetXpos();
	int ypos = Q->GetYpos();


	// Check where are all the GOs located 
	for (int i = 0; i < numofGO; i++) {
		if (GOinRange(xpos + N1width - 1, xpos, ypos + N1height - 1, ypos, GOlist[i].pos)) {
			UPL++;
			quadinputorder.push_back(1);
		}
		else if (GOinRange(xpos + N1width + N2width - 1, xpos + N1width, ypos + N1height - 1, ypos, GOlist[i].pos)) {
			UPM++;
			quadinputorder.push_back(2);
		}
		else if (GOinRange(xpos + N3width + N2width + N1width - 1, xpos + N1width + N2width, ypos + N1height - 1, ypos, GOlist[i].pos)) {
			UPR++;
			quadinputorder.push_back(3);
		}
		else if (GOinRange(xpos + N1width - 1, xpos, ypos + N1height + N2height - 1, ypos +N1height, GOlist[i].pos)) {
			MIDL++;
			quadinputorder.push_back(4);
		}
		else if (GOinRange(xpos + N1width + N2width - 1, xpos, ypos + N1height + N2height - 1, ypos + N1height, GOlist[i].pos)) {
			MIDM++;
			quadinputorder.push_back(5);
		}
		else if (GOinRange(xpos + N3width + N2width + N1width - 1, xpos, ypos + N1height + N2height - 1, ypos + N1height, GOlist[i].pos)) {
			MIDR++;
			quadinputorder.push_back(6);
		}
		else if (GOinRange(xpos + N1width - 1, xpos, ypos + N1height + N2height + N3height - 1, ypos + N1height + N2height, GOlist[i].pos)) {
			BOTL++;
			quadinputorder.push_back(7);
		}
		else if (GOinRange(xpos + N1width + N2width - 1,xpos, ypos + N1height + N2height + N3height - 1, ypos + N1height + N2height, GOlist[i].pos)) {
			BOTM++;
			quadinputorder.push_back(8);
		}
		else if (GOinRange(xpos + N3width + N2width + N1width - 1, xpos, ypos + N1height + N2height + N3height - 1, ypos + N1height + N2height, GOlist[i].pos)) {
			BOTR++;
			quadinputorder.push_back(9);
		}
	}	
	//Create Quads
	if (UPL > 0) {
		CQuad* newnode = new CQuad(N1width, N1height, xpos, ypos,depth, UPL_TYPE);
		Q->UpL = newnode;
	}
	if (UPM > 0) {
		CQuad* newnode = new CQuad(N2width, N1height, xpos + N2width, ypos, depth, UPM_TYPE);
		Q->UpM = newnode;
	}
	if (UPR > 0) {
		CQuad* newnode = new CQuad(N3width, N1height, xpos+N1width+N2width, ypos, depth, UPR_TYPE);
		Q->UpR = newnode;
	}
	if (MIDL > 0) {
		CQuad* newnode = new CQuad(N1width, N2height, xpos, ypos+N1height, depth, MIDL_TYPE);
		Q->MidL = newnode;
	}
	if (MIDM > 0) {
		CQuad* newnode = new CQuad(N2width, N2height, xpos + N2width, ypos + N1height, depth, MIDM_TYPE);
		Q->MidM = newnode;
	}
	if (MIDR > 0) {
		CQuad* newnode = new CQuad(N3width, N2height, xpos + N1width + N2width, ypos + N1height, depth, MIDR_TYPE);
		Q->MidR = newnode;
	}
	if (BOTL > 0) {
		CQuad* newnode = new CQuad(N1width, N3height, xpos, ypos + N1height + N2height, depth, BOTL_TYPE);
		Q->BotL = newnode;
	}
	if (BOTM > 0) {
		CQuad* newnode = new CQuad(N2width, N3height, xpos + N2width, ypos + N1height + N2height, depth, BOTM_TYPE);
		Q->BotM = newnode;
	}
	if (BOTR > 0) {
		CQuad* newnode = new CQuad(N3width, N3height, xpos + N1width + N2width, ypos + N1height + N2height, depth, BOTR_TYPE);
		Q->BotR = newnode;
	}

	//Add Game objects inside Quads 
	for (int i = 0; i < numofGO; i++) {
		int type = quadinputorder[i];
		if (type == 1)
			Q->UpL->InputGO(GOlist[i]);
		else if (type == 2)
			Q->UpM->InputGO(GOlist[i]);
		else if (type == 3)
			Q->UpR->InputGO(GOlist[i]);
		else if (type == 4)
			Q->MidL->InputGO(GOlist[i]);
		else if (type == 5)
			Q->MidM->InputGO(GOlist[i]);
		else if (type == 6)
			Q->MidR->InputGO(GOlist[i]);
		else if (type == 7)
			Q->BotL->InputGO(GOlist[i]);
		else if (type == 8)
			Q->BotM->InputGO(GOlist[i]);
		else if (type == 9)
			Q->BotR->InputGO(GOlist[i]);
	}

	//Recursive split 
	if (UPL > 10)
		Split(Q->UpL);
	if (UPM > 10)
		Split(Q->UpM);
	if (UPR > 10)
		Split(Q->UpR);
	if (MIDL > 10)
		Split(Q->MidL);
	if (MIDM > 10)
		Split(Q->MidM);
	if (MIDR > 10)
		Split(Q->MidR);
	if (BOTL > 10)
		Split(Q->BotL);
	if (BOTM > 10)
		Split(Q->BotM);
	if (BOTR > 10)
		Split(Q->BotR);
}

void CTree::CreateQuads(void)
{
	//CQuad::CQuad(int w, int h, int xpos, int ypos, int level, int quadtype, CPosition pos)
	root = new CQuad(totalwidth, totalbreadth, 0, 0, 0, ROOT_TYPE); //big quad 
	for (int i = 0; i < count; i++) {
		root->InputGO(limit[i]);
	}
	root->SetCount(count);
	Split(root);
}


void CTree::PrintTree(void)
{
	if (root != nullptr) {
		PreOrderStats(root);
	}
	std::cout << std::endl;
}

void CTree::CheckCollisionWNearbyGOs(int go_id)
{
	if (root != nullptr) {
		CQuad* point = root->FindGO(go_id);
		if (point == nullptr)
			std::cout << "Invalid Input " << std::endl;
		else
			point->PrintOtherGOs(go_id);
	}
}

bool CTree::GOinRange(int maxX, int minX, int maxZ, int minZ, glm::vec3 pos)
{
	if ((pos.x - minX) <= (maxX - minX)) {
		if ((pos.y - minZ) <= (maxZ - minZ)) {
			return true;
		}
	}
	return false;
}

void CTree::PreOrderStats(CQuad* p) //rename 24/1/25
{
	if (p != nullptr) {
		p->PrintStats();
		PreOrderStats(p->UpL);
		PreOrderStats(p->UpM);
		PreOrderStats(p->UpR);
		PreOrderStats(p->MidL);
		PreOrderStats(p->MidM);
		PreOrderStats(p->MidR);
		PreOrderStats(p->BotL);
		PreOrderStats(p->BotM);
		PreOrderStats(p->BotR);
	}
}

void CTree::PreOrderDelete(CQuad* p)
{
	if (p != nullptr) {
		PreOrderDelete(p->UpL);
		PreOrderDelete(p->UpM);
		PreOrderDelete(p->UpR);
		PreOrderDelete(p->MidL);
		PreOrderDelete(p->MidM);
		PreOrderDelete(p->MidR);
		PreOrderDelete(p->BotL);
		PreOrderDelete(p->BotM);
		PreOrderDelete(p->BotR);
		delete p;
	}
}


