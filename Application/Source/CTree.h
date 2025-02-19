#pragma once
#include "CQuad.h"
#include "CollisionDetection.h"
#include <iostream>

class CTree
{
private:
	CQuad* root, *current;
	int xpos, zpos;
	int count;
	int totalbreadth;
	int totalwidth; 
	bool GOinRange(int maxX, int minX, int maxZ, int minZ, glm::vec3 pos);
	void PreOrderStats(CQuad* p);
	void PreOrderDelete(CQuad* p);

	enum ENUM_QUADTYPE {
		ROOT_TYPE,
		UPL_TYPE,
		UPM_TYPE,
		UPR_TYPE,
		MIDL_TYPE,
		MIDM_TYPE,
		MIDR_TYPE,
		BOTL_TYPE,
		BOTM_TYPE,
		BOTR_TYPE,
		NUM_TYPE
	};


public:
	int targettype = 0;
	std::vector<GameObject> limit;
	CTree(int maxwidth, int maxbreadth,int xpos, int zpos);
	~CTree(void);
	void AddGO(GameObject go);
	//void AddGO(std::string ID, std::string X, std::string Y);
	void Split(CQuad* &Q);
	void CreateQuads(void);
	void PrintTree(void);
	void CheckCollisionWNearbyGOs(int go_id, std::vector<int>& idlist);
};

