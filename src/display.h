extern void display();
extern void init();
extern void reshape(int x, int y);
extern void DrawCube(void);
extern void init_material(float Ka, float Kd, float Ks, float Kp, float Mr, float Mg, float Mb);
extern void init_light(int light_source, float Lx, float Ly, float Lz, float Lr, float Lg, float Lb);
extern int ballno;