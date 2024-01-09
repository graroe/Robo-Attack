


void init3DSurfaceWindow();

typedef struct Vector3D
{
	GLdouble x, y, z;
} Vector3D;


void display3D();
void reshape3D(int w, int h);
void mouseButtonHandler3D(int button, int state, int x, int y);
void mouseScrollWheelHandler3D(int button, int dir, int xMouse, int yMouse);
void mouseMotionHandler3D(int xMouse, int yMouse);
void keyboardHandler3D(unsigned char key, int x, int y);

void drawGround();
void drawQuads();
void drawQuadsAsLines();
void drawQuadsAsPoints();
void generateArrays();
void generateVBOs();
