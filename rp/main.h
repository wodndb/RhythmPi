#include <stdio.h>
#include <stdio_ext.h>
#include <stdlib.h>
#include <esUtil.h>
#include <rpUtil.h>
#include <rpNote.h>
#include <rpGpio.h>
#include <kshLoader.h>
#include <wiringPi.h>
#include <signal.h>

// Image macro
#define IMG_MAX_NUM 9

#define IMG_NOTE              0
#define IMG_GPIO_INPUT        1
#define IMG_MAIN              2
#define IMG_SELECT_MENU       3
#define IMG_OPTION            4
#define IMG_END               5
#define IMG_SELECT_CURSOR     6
#define IMG_GOOD              7
#define IMG_BAD               8

// Stage state macro
#define STAGE_MAIN            0x01
#define STAGE_SELECT_MUSIC    0x02
#define STAGE_OPTION          0x04
#define STAGE_PLAY            0x08
#define STAGE_END             0x10
#define STAGE_OPTION_SEL_SPD  0x20

typedef struct _user_data
{
	// Handle to a program object
	GLuint programObject[IMG_MAX_NUM];

	// Attribute locations
	GLint  positionLoc[IMG_MAX_NUM];
	GLint  texCoordLoc[IMG_MAX_NUM];

	// Sampler location
	GLint samplerLoc[IMG_MAX_NUM];

	// Texture handle
	GLuint textureId[IMG_MAX_NUM];

	char *image[IMG_MAX_NUM];
	int width[IMG_MAX_NUM], height[IMG_MAX_NUM];

	// stage state : MAIN / SELECT MUSIC / OPTION / PLAY / END
	int stageState;
	// selected music
	int musicNum;
	pid_t musicPid;  // process that play music (omxplayer)
	// selectedOption
	int selectedOption;
	int selectedSpeed;
	int speed;

	// wodndb data
	float temp;
	float measureBar;
	int prevGpioStat;
	int gpioStat;
	FILE* kshFile;
	KshInfo *ki;
	QType *qtNote;
} UserData;

GLfloat vVertices[] = { -0.1f, -0.1f, 0.0f, 
	-0.1f,  0.1f, 0.0f,
	-0.3f, -0.1f, 0.0f,
	-0.3f,  0.1f, 0.0f };

GLfloat vRpVertices[7][20] ={{ -0.4f,  0.1f, 0.0f,  //BT-0
	0.0f,  1.0f,
	-0.4f, -0.1f, 0.0f,
	0.0f,  0.0f,
	-0.2f, -0.1f, 0.0f,
	1.0f,  0.0f,
	-0.2f,  0.1f, 0.0f,
	1.0f,  1.0f       },
		{ -0.2f,  0.1f, 0.0f,  //BT-1
			0.0f,  1.0f,
			-0.2f, -0.1f, 0.0f,
			0.0f,  0.0f,
			0.0f, -0.1f, 0.0f,
			1.0f,  0.0f,
			0.0f,  0.1f, 0.0f,
			1.0f,  1.0f       },
		{  0.0f,  0.1f, 0.0f,  //BT-2
			0.0f,  1.0f,
			0.0f, -0.1f, 0.0f,
			0.0f,  0.0f,
			0.2f, -0.1f, 0.0f,
			1.0f,  0.0f,
			0.2f,  0.1f, 0.0f,
			1.0f,  1.0f       },
		{  0.2f,  0.1f, 0.0f,  //BT-3
			0.0f,  1.0f,
			0.2f, -0.1f, 0.0f,
			0.0f,  0.0f,
			0.4f, -0.1f, 0.0f,
			1.0f,  0.0f,
			0.4f,  0.1f, 0.0f,
			1.0f,  1.0f       },
		{  4.0f,  0.0f, 0.0f,  //blank
			0.0f,  1.0f,
			4.0f,  0.0f, 0.0f,
			0.0f,  0.0f,
			-4.0f,  0.0f, 0.0f,
			1.0f,  0.0f,
			-4.0f,  0.0f, 0.0f,
			1.0f,  1.0f       },
		{ -0.4f,  0.1f, 0.0f,  //FX-L
			0.0f,  1.0f,
			-0.4f, -0.1f, 0.0f,
			0.0f,  0.0f,
			0.0f, -0.1f, 0.0f,
			1.0f,  0.0f,
			0.0f,  0.1f, 0.0f,
			1.0f,  1.0f       },
		{  0.0f,  0.1f, 0.0f,  //FX-R
			0.0f,  1.0f,
			0.0f, -0.1f, 0.0f,
			0.0f,  0.0f,
			0.4f, -0.1f, 0.0f,
			1.0f,  0.0f,
			0.4f,  0.1f, 0.0f,
			1.0f,  1.0f       } };

GLfloat vGpioVertices[6][20] ={{ -0.4f,  0.0f, -0.1f,  //BT-0
	0.0f,  1.0f,
	-0.4f, -1.0f, -0.1f,
	0.0f,  0.0f,
	-0.2f, -1.0f, -0.1f,
	1.0f,  0.0f,
	-0.2f,  0.0f, -0.1f, 
	1.0f,  1.0f       },
		{ -0.2f,  0.0f, -0.1f,  //BT-1
			0.0f,  1.0f,
			-0.2f, -1.0f, -0.1f,
			0.0f,  0.0f,
			0.0f, -1.0f, -0.1f,
			1.0f,  0.0f,
			0.0f,  0.0f, -0.1f, 
			1.0f,  1.0f       },
		{  0.0f,  0.0f, -0.1f,  //BT-2
			0.0f,  1.0f,
			0.0f, -1.0f, -0.1f,
			0.0f,  0.0f,
			0.2f, -1.0f, -0.1f,
			1.0f,  0.0f,
			0.2f,  0.0f, -0.1f, 
			1.0f,  1.0f       },
		{  0.2f,  0.0f, -0.1f,  //BT-3
			0.0f,  1.0f,
			0.2f, -1.0f, -0.1f,
			0.0f,  0.0f,
			0.4f, -1.0f, -0.1f,
			1.0f,  0.0f,
			0.4f,  0.0f, -0.1f, 
			1.0f,  1.0f       },
		{ -0.4f,  0.0f, -0.1f,  //FX-L
			0.0f,  1.0f,
			-0.4f, -1.0f, -0.1f,
			0.0f,  0.0f,
			0.0f, -1.0f, -0.1f,
			1.0f,  0.0f,
			0.0f,  0.0f, -0.1f, 
			1.0f,  1.0f       },
		{  0.0f,  0.0f, -0.1f,  //FX-R
			0.0f,  1.0f,
			0.0f, -1.0f, -0.1f,
			0.0f,  0.0f,
			0.4f, -1.0f, -0.1f,
			1.0f,  0.0f,
			0.4f,  0.0f, -0.1f, 
			1.0f,  1.0f       } };

GLfloat vFullScreen[] = { -1.0f,  1.0f, -0.1f,
	0.0f,  1.0f,
	-1.0f, -1.0f, -0.1f,
	0.0f,  0.0f,
	1.0f, -1.0f, -0.1f,
	1.0f,  0.0f,
	1.0f,  1.0f, -0.1f, 
	1.0f,  1.0f       };

GLfloat vMusicSelect[] = { -0.75f,  0.7f,  0.1f,
	0.0f,  1.0f,
	-0.75f,  0.5f,  0.1f,
	0.0f,  0.0f,
	-0.6f,  0.5f,  0.1f,
	1.0f,  0.0f,
	-0.6f,  0.7f,  0.1f, 
	1.0f,  1.0f       };

GLfloat vSpeedSelect[] = { -0.8f,  0.46f,  0.1f,
	0.0f,  1.0f,
	-0.8f,  0.36f,  0.1f,
	0.0f,  0.0f,
	-0.6f,  0.36f,  0.1f,
	1.0f,  0.0f,
	-0.6f,  0.46f,  0.1f, 
	1.0f,  1.0f       };

GLfloat tVertices[] = {  0.2f, -0.1f, 0.0f, 
	0.2f,  0.1f, 0.0f,
	-0.2f, -0.1f, 0.0f,
	-0.2f,  0.1f, 0.0f };

// Create a simple 2x2 texture image
GLuint CreateSimpleTexture2D(ESContext *esContext, int imageIndex);

// Create a shader object, load the shader source, and compile the shader
GLuint LoadShader(GLenum type, const char *shaderSrc);

// Initialize the shader and program object
int Init(ESContext *esContext);

// Draw Background
void loadImage(ESContext *esContext, int imageID, GLfloat* imageVertex);

// Draw a triangle using the shader pair create in Init()
void Draw(ESContext *esContext);

// Clean up
void ShutDown(ESContext *esContext);

void Update(ESContext *esContext, float deltaTime);

int main(int argc, char *argv[]);
