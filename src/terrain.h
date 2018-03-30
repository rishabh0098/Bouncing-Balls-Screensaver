class Terrain
{
public:
	float cx[30],cz[30];
	int sel[30];
	Terrain();
	void drawCone(float xx, float yy);
	void drawSphere(float xx, float yy);
	void drawTerrain();
};
