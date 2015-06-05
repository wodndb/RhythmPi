//
// Book:      OpenGL(R) ES 2.0 Programming Guide
// Authors:   Aaftab Munshi, Dan Ginsburg, Dave Shreiner
// ISBN-10:   0321502795
// ISBN-13:   9780321502797
// Publisher: Addison-Wesley Professional
// URLs:      http://safari.informit.com/9780321563835
//            http://www.opengles-book.com
//

// Hello_Triangle.c
//
//    This is a simple example that draws a single triangle with
//    a minimal vertex/fragment shader.  The purpose of this 
//    example is to demonstrate the basic concepts of 
//    OpenGL ES 2.0 rendering.

#include <stdio.h>
#include <stdio_ext.h>
#include <stdlib.h>
#include <esUtil.h>
#include <rpUtil.h>
#include <rpNote.h>
#include <kshLoader.h>
#include <wiringPi.h>

typedef struct _user_data
{
   // Handle to a program object
   GLuint programObject;
   float temp;
   float measureBar;
   FILE* kshFile;
   KshInfo *ki;
   QType *qtNote;
} UserData;

GLfloat vVertices[] = { -0.1f, -0.1f, 0.0f, 
                        -0.1f,  0.1f, 0.0f,
                        -0.3f, -0.1f, 0.0f,
			               -0.3f,  0.1f, 0.0f };

GLfloat vRpVertices[7][12] = { { -0.2f, -0.1f, 0.0f,  //BT-0
                                 -0.2f,  0.1f, 0.0f,
                                 -0.4f, -0.1f, 0.0f,
                                 -0.4f,  0.1f, 0.0f },
                               {  0.0f, -0.1f, 0.0f,  //BT-1
                                  0.0f,  0.1f, 0.0f,
                                 -0.2f, -0.1f, 0.0f,
                                 -0.2f,  0.1f, 0.0f },
                               {  0.2f, -0.1f, 0.0f,  //BT-2
                                  0.2f,  0.1f, 0.0f,
                                  0.0f, -0.1f, 0.0f,
                                  0.0f,  0.1f, 0.0f },
                               {  0.4f, -0.1f, 0.0f,  //BT-3
                                  0.4f,  0.1f, 0.0f,
                                  0.2f, -0.1f, 0.0f,
                                  0.2f,  0.1f, 0.0f },
                               {  4.0f,  0.0f, 0.0f,  //blank
                                  4.0f,  0.0f, 0.0f,
                                 -4.0f,  0.0f, 0.0f,
                                 -4.0f,  0.0f, 0.0f },
                               {  0.0f, -0.1f, 0.0f,  //FX-L
                                  0.0f,  0.1f, 0.0f,
                                 -0.4f, -0.1f, 0.0f,
                                 -0.4f,  0.1f, 0.0f },
                               {  0.4f, -0.1f, 0.0f,  //FX-R
                                  0.4f,  0.1f, 0.0f,
                                  0.0f, -0.1f, 0.0f,
                                  0.0f,  0.1f, 0.0f } };
 
GLfloat tVertices[] = {  0.2f, -0.1f, 0.0f, 
                         0.2f,  0.1f, 0.0f,
                        -0.2f, -0.1f, 0.0f,
			               -0.2f,  0.1f, 0.0f };

///
// Create a shader object, load the shader source, and
// compile the shader.
//
GLuint LoadShader ( GLenum type, const char *shaderSrc )
{
   GLuint shader;
   GLint compiled;
   
   // Create the shader object
   shader = glCreateShader ( type );

   if ( shader == 0 )
   	return 0;

   // Load the shader source
   glShaderSource ( shader, 1, &shaderSrc, NULL );
   
   // Compile the shader
   glCompileShader ( shader );

   // Check the compile status
   glGetShaderiv ( shader, GL_COMPILE_STATUS, &compiled );

   if ( !compiled ) 
   {
      GLint infoLen = 0;

      glGetShaderiv ( shader, GL_INFO_LOG_LENGTH, &infoLen );
      
      if ( infoLen > 1 )
      {
         char* infoLog = malloc (sizeof(char) * infoLen );

         glGetShaderInfoLog ( shader, infoLen, NULL, infoLog );
         esLogMessage ( "Error compiling shader:\n%s\n", infoLog );            
         
         free ( infoLog );
      }

      glDeleteShader ( shader );
      return 0;
   }

   return shader;

}

///
// Initialize the shader and program object
//
int Init ( ESContext *esContext )
{
   esContext->userData = malloc(sizeof(UserData));

   UserData *userData = esContext->userData;
   GLbyte vShaderStr[] =
      "attribute vec4 vPosition;    \n"
      "attribute vec4 vColour;      \n"
      "varying vec4 vFragmentColour; \n"
      "void main()                  \n"
      "{                            \n"
      "   gl_Position = vPosition;  \n"
      "   vFragmentColour = vColour; \n"
      "}                            \n";
   
   GLbyte fShaderStr[] =  
      "precision mediump float;                     \n"
      "void main()                                  \n"
      "{                                            \n"
      "  gl_FragColor = vec4 ( 1.0, 1.0, 1.0, 1.0); \n"
      "}                                            \n";

   GLuint vertexShader;
   GLuint fragmentShader;
   GLuint programObject;
   GLint linked;

   // Load the vertex/fragment shaders
   vertexShader = LoadShader ( GL_VERTEX_SHADER, vShaderStr );
   fragmentShader = LoadShader ( GL_FRAGMENT_SHADER, fShaderStr );

   // Create the program object
   programObject = glCreateProgram ( );
   
   if ( programObject == 0 )
      return 0;

   glAttachShader ( programObject, vertexShader );
   glAttachShader ( programObject, fragmentShader );

   // Bind vPosition to attribute 0   
   glBindAttribLocation ( programObject, 0, "vPosition" );

   // Link the program
   glLinkProgram ( programObject );

   // Check the link status
   glGetProgramiv ( programObject, GL_LINK_STATUS, &linked );

   if ( !linked ) 
   {
      GLint infoLen = 0;

      glGetProgramiv ( programObject, GL_INFO_LOG_LENGTH, &infoLen );
      
      if ( infoLen > 1 )
      {
         char* infoLog = malloc (sizeof(char) * infoLen );

         glGetProgramInfoLog ( programObject, infoLen, NULL, infoLog );
         esLogMessage ( "Error linking program:\n%s\n", infoLog );            
         
         free ( infoLog );
      }

      glDeleteProgram ( programObject );
      return GL_FALSE;
   }

   // Store the program object
   userData->programObject = programObject;

   //getKshData
   userData->ki = (KshInfo*)malloc(1 * sizeof(KshInfo));
   userData->kshFile = fopen("../kshLoader/test.ksh", "r");
   userData->qtNote = (QType*)malloc(1 * sizeof(QType));
   userData->temp = -1.0;

   if(userData->kshFile == NULL) {
      printf("file load error!\n");
   }

   getKshInfo(userData->kshFile, userData->ki);
   loadKshNote(userData->kshFile, userData->qtNote);

   printf("Start registering es functions\n");

   glClearColor ( 0.0f, 0.0f, 0.0f, 0.0f );
   return GL_TRUE;
}

///
// Draw a triangle using the shader pair created in Init()
//
void Draw ( ESContext *esContext )
{
   int nCntMax;
   int i = 0;
   float lowNoteWidth = 0;
   float highNoteWidth = 0;
   UserData *userData = ( UserData* ) esContext->userData;
   QNode* tempQNode = NULL;
   tempQNode = userData->qtNote->front;

   // Set the viewport
   glViewport ( 0, 0, esContext->width, esContext->height );
   
   // Clear the color buffer
   glClear ( GL_COLOR_BUFFER_BIT );

   // Use the program object
   glUseProgram ( userData->programObject );

   // Load the vertex data
   /*
   glVertexAttribPointer ( 0, 3, GL_FLOAT, GL_FALSE, 0, vVertices );
   glEnableVertexAttribArray ( 0 );

   glDrawArrays ( GL_TRIANGLE_STRIP, 0, 4 );
   */
   //==================
   
   while(tempQNode->link != NULL) {
      for(i = 0; i <= 6; i++) {
         if(i != 4 && ((tempQNode->note.type & 0x0FFF) == (RP_NOTE_TYPE_BT_FIRST >> i))) {
            //If note is type of long note : start
            if((tempQNode->note.type & RP_NOTE_TYPE_LONG_STT) != 0) {
               lowNoteWidth = 0.025;
               highNoteWidth = 1.0/(float)(tempQNode->note.max);
            }

            //If note is type of long note : middle
            else if((tempQNode->note.type & RP_NOTE_TYPE_LONG_MID) != 0) {
               lowNoteWidth = 1.0/(float)(tempQNode->note.max);
               highNoteWidth = 1.0/(float)(tempQNode->note.max);
            }

            //If note is type of long note : end
            else if((tempQNode->note.type & RP_NOTE_TYPE_LONG_END) != 0) {
               lowNoteWidth = 1.0/(float)(tempQNode->note.max);
               highNoteWidth = 2.0/(float)(tempQNode->note.max) + 0.025;
            }
	
            else {
               lowNoteWidth = 0.025;
               highNoteWidth = 0.025;
            }

            vRpVertices[i][1] = ((float)(tempQNode->note.measure) + (float)(tempQNode->note.order)/(float)(tempQNode->note.max)) * 2.0
                                - userData->temp + highNoteWidth;
            vRpVertices[i][4] = ((float)(tempQNode->note.measure) + (float)(tempQNode->note.order)/(float)(tempQNode->note.max)) * 2.0
                                - userData->temp - lowNoteWidth;
            vRpVertices[i][7] = ((float)(tempQNode->note.measure) + (float)(tempQNode->note.order)/(float)(tempQNode->note.max)) * 2.0
                                - userData->temp + highNoteWidth;
            vRpVertices[i][10] = ((float)(tempQNode->note.measure) + (float)(tempQNode->note.order)/(float)(tempQNode->note.max)) * 2.0
                                 - userData->temp - lowNoteWidth;
 

            glVertexAttribPointer ( 0, 3, GL_FLOAT, GL_FALSE, 0, vRpVertices[i] );
            glEnableVertexAttribArray( 0 );
            glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );
         }
      }

      tempQNode = tempQNode->link;
   }
   /*
      vRpVertices[4][1] = -userData->measureBar + 0.001;
      vRpVertices[4][4] = -userData->measureBar - 0.001;
      vRpVertices[4][7] = -userData->measureBar + 0.001;
      vRpVertices[4][10] = -userData->measureBar - 0.001;

      glVertexAttribPointer ( 0, 3, GL_FLOAT, GL_FALSE, 0, vRpVertices[4] );
      glEnableVertexAttribArray( 0 );
      glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );
   */
}

void Update ( ESContext *esContext, float deltaTime ) {
   int i;
   UserData *userData = ( UserData* ) esContext->userData;
   
   //deltaTime = 120pbm(4/4 bit), userData->ki->t = bpm of song
   userData->temp += deltaTime * (userData->ki->t / 120.0);
   userData->measureBar += deltaTime * (userData->ki->t / 120.0);
   if(userData->measureBar > 1.0) {
      userData->measureBar = -1.0;
   }
}

int main ( int argc, char *argv[] )
{
   ESContext esContext;
   UserData  userData;
   pid_t pid;

   esInitContext ( &esContext );
   esContext.userData = &userData;

   esCreateWindow ( &esContext, "Hello Triangle", esContext.width, esContext.height, ES_WINDOW_RGB );

   if ( !Init ( &esContext ) )
      return 0;

   esRegisterDrawFunc ( &esContext, Draw );
   esRegisterUpdateFunc ( &esContext, Update );

   pid = fork();

   switch(pid) {
      case -1:
      {
         printf("child process can't be created\n");
         return -1;
      }
      case 0:
      {
         //system("omxplayer ../songs/ksm/homura/homura.ogg");
         execlp("omxplayer", "omxplayer", "../songs/ksm/homura/homura_lt_f.ogg");
      }
      default:
      {
         esMainLoop ( &esContext );
      }
   }

   return 0;
}
