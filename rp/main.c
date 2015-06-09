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
#include <rpGpio.h>
#include <kshLoader.h>
#include <wiringPi.h>

typedef struct _user_data
{
   // Handle to a program object
   GLuint programObject[2];
   
   // Attribute locations
   GLint  positionLoc[2];
   GLint  texCoordLoc[2];

   // Sampler location
   GLint samplerLoc[2];

   // Texture handle
   GLuint textureId[2];

   char *image[2];
   int width[2], height[2];
   
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
 
GLfloat tVertices[] = {  0.2f, -0.1f, 0.0f, 
                         0.2f,  0.1f, 0.0f,
                        -0.2f, -0.1f, 0.0f,
			      -0.2f,  0.1f, 0.0f };

///
// Create a simple 2x2 texture image with four different colors
//
GLuint CreateSimpleTexture2D(ESContext *esContext, int imageIndex)
{
   // Texture object handle
   GLuint textureId;
   UserData *userData = (UserData*) esContext->userData;
   
   char *pixels = userData->image[imageIndex];

   printf("Create Simple Texture 2D\n");

   // Use tightly packed data
   glPixelStorei ( GL_UNPACK_ALIGNMENT, 1 );

   // Generate a texture object
   glGenTextures ( 1, &textureId );

   // Bind the texture object
   glBindTexture ( GL_TEXTURE_2D, textureId );

   // Load the texture
   glTexImage2D ( GL_TEXTURE_2D, 0, GL_RGBA, 
		  userData->width[imageIndex], userData->height[imageIndex], 
		  0, GL_RGBA, GL_UNSIGNED_BYTE, pixels );

   // Set the filtering mode
   glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
   glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

   return textureId;
}

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
   int i;
   int width, height;
   char* image;
   char imageName[2][80] = {"jan.tga", "gpioinput.tga"};
   //esContext->userData = malloc(sizeof(UserData));

   UserData *userData = esContext->userData;
   GLbyte vShaderStr[] =
      "attribute vec4 a_position;   \n"
      "attribute vec2 a_texCoord;   \n"
      "varying vec2 v_texCoord;     \n"
      "void main()                  \n"
      "{                            \n"
      "   gl_Position = a_position; \n"
      "   v_texCoord = a_texCoord;  \n"
      "}                            \n";
   
   GLbyte fShaderStr[] =  
      "precision mediump float;                            \n"
      "varying vec2 v_texCoord;                            \n"
      "uniform sampler2D s_texture;                        \n"
      "void main()                                         \n"
      "{                                                   \n"
      "  gl_FragColor = texture2D( s_texture, v_texCoord );\n"
      "}                                                   \n";

   for(i = 0; i < 2; i++) {
         printf("load image : %s\n", imageName[i]);
         image = esLoadTGA(imageName[i], &width, &height);
         if (image == NULL) {
             fprintf(stderr, "No such image\n");
             exit(1);
         }
         printf("Width %d height %d\n", width, height);
         
         userData->image[i] = image;
         userData->width[i] = width;
         userData->height[i] = height;
      
         printf("Load the shaders and get a linked program object\n");
      
         // Load the shaders and get a linked program object
         userData->programObject[i] = esLoadProgram ( vShaderStr, fShaderStr );
      
         // Get the attribute locations
         userData->positionLoc[i] = glGetAttribLocation ( userData->programObject[i], "a_position" );
         userData->texCoordLoc[i] = glGetAttribLocation ( userData->programObject[i], "a_texCoord" );
      
         printf("get sampler location\n");
         
         // Get the sampler location
         userData->samplerLoc[i] = glGetUniformLocation ( userData->programObject[i], "s_texture" );
      
         printf("load texture\n");
      
         // Load the texture
         userData->textureId[i] = CreateSimpleTexture2D (esContext, i);
   }

   printf("Load ksh data\n");

   //getKshData
   userData->ki = (KshInfo*)malloc(1 * sizeof(KshInfo));
   userData->kshFile = fopen("../kshLoader/test.ksh", "r");
   userData->qtNote = (QType*)malloc(1 * sizeof(QType));
   userData->temp = -2.0;

   if(userData->kshFile == NULL) {
      printf("file load error!\n");
   }

   getKshInfo(userData->kshFile, userData->ki);
   loadKshNote(userData->kshFile, userData->qtNote);

   // GPIO Initialization
   initPinMode();

   printf("Start registering es functions\n");

   glClearColor ( 0.0f, 0.0f, 0.0f, 1.0f );
   return GL_TRUE;
}

///
// Draw a triangle using the shader pair created in Init()
//
void Draw ( ESContext *esContext )
{
   int i = 0;
   int hittedGpioStat = 0x00;
   float lowNoteWidth = 0;
   float highNoteWidth = 0;
   UserData *userData = ( UserData* ) esContext->userData;
   QNode* tempQNode = NULL;
   
   GLushort indices[] = { 0, 1, 2, 0, 2, 3 };
   
   tempQNode = userData->qtNote->front;

   // Set the viewport
   glViewport ( 0, 0, esContext->width, esContext->height );
   
   // Clear the color buffer
   glClear ( GL_COLOR_BUFFER_BIT );

   // Use the program object
   glUseProgram ( userData->programObject[0] );

   //==================
   
   //printf(".");

   // Judge notes : Input from GPIO
   while(tempQNode->link != NULL) {
      if(((float)(tempQNode->note.measure) + (float)(tempQNode->note.order)/(float)(tempQNode->note.max)) * 2.0
            - userData->temp < -0.1)
      {
         if(tempQNode->note.hitted != 1) {
            // Check note type
            for(i = 0; i <= 6; i++) {
               if(((tempQNode->note.type & 0x0FFF) & (RP_NOTE_TYPE_BT_FIRST >> i)) != 0) {
                     // Check note is hitted : BT1 ~ BT4
                     if( (i < 4) && ((userData->gpioStat & (RP_NOTE_TYPE_BT_FIRST >> (i + 2))) != 0)
                     && ((userData->prevGpioStat & (RP_NOTE_TYPE_BT_FIRST >> (i + 2))) == 0)
                     && ((hittedGpioStat & (RP_NOTE_TYPE_BT_FIRST >> (i + 2))) == 0) ) {
                           printf("%x == %x\n", userData->gpioStat, RP_NOTE_TYPE_BT_FIRST >> (i + 2));
                           hittedGpioStat |= (RP_NOTE_TYPE_BT_FIRST >> (i + 2));
                           if((tempQNode->note.type & 0xF000) != 0) {
                              tempQNode->note.hitted = 0x02;
                           }
                           else {
                              tempQNode->note.hitted = 0x01;
                           }
                           printf("button hit\n");
                           break;
                     } // Check note is hitted : FX1, FX2
                     else if( (i > 4) && ((userData->gpioStat & (RP_NOTE_TYPE_BT_FIRST >> (i + 1))) != 0)
                     && ((userData->prevGpioStat & (RP_NOTE_TYPE_BT_FIRST >> (i + 1))) == 0)
                     && ((hittedGpioStat & (RP_NOTE_TYPE_BT_FIRST >> (i + 1))) == 0) ) {
                           printf("%x == %x\n", userData->gpioStat, RP_NOTE_TYPE_BT_FIRST >> (i + 1));
                           hittedGpioStat |= (RP_NOTE_TYPE_BT_FIRST >> (i + 1));
                           if((tempQNode->note.type & 0xF000) != 0) {
                              tempQNode->note.hitted = 0x02;
                           }
                           else {
                              tempQNode->note.hitted = 0x01;
                           }
                           printf("button hit\n");
	                   break;
                     }
               }
            }
         }
         tempQNode = tempQNode->link;
      }
      else {
            // Init QNode and break;
            tempQNode = userData->qtNote->front;
            break;
      }
   }
   
   // Judge notes : Out of line
   while(tempQNode->link != NULL) {
      if(((float)(tempQNode->note.measure) + (float)(tempQNode->note.order)/(float)(tempQNode->note.max)) * 2.0
            - userData->temp < - 1.9)
      {
            dequeue(userData->qtNote);
            tempQNode = userData->qtNote->front;
            printf("pop note : out of lines\n");
      }
      else {
            // Init QNode
            tempQNode = userData->qtNote->front;
            break;
      }
   }
    // Draw about input from GPIO
   for(i = 9; i >= 4; i--) {
      if( (userData->gpioStat & (0x01 << i)) >> i == 0x01 ) {
         //glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, vGpioVertices[9 - i] );
         //glEnableVertexAttribArray( 0 );
         //glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );
         
         glUseProgram ( userData->programObject[1] );
         // Load the vertex position
         glVertexAttribPointer ( userData->positionLoc[1], 3, GL_FLOAT, 
                                 GL_FALSE, 5 * sizeof(GLfloat), vGpioVertices[9 - i] );
         // Load the texture coordinate
         glVertexAttribPointer ( userData->texCoordLoc[1], 2, GL_FLOAT,
                                 GL_FALSE, 5 * sizeof(GLfloat), &vGpioVertices[9 - i][3] );
      
         glEnableVertexAttribArray ( userData->positionLoc[1] );
         glEnableVertexAttribArray ( userData->texCoordLoc[1] );
      
         // Bind the texture
         glActiveTexture ( GL_TEXTURE0 );
         glBindTexture ( GL_TEXTURE_2D, userData->textureId[1] );
      
         // Set the sampler texture unit to 0
         glUniform1i ( userData->samplerLoc[1], 0 );
      
         glDrawElements ( GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, indices );
      }
   }
  
   glUseProgram ( userData->programObject[0] );
   // Draw notes
   while(tempQNode->link != NULL) {
      for(i = 0; i <= 6; i++) {
         if((i != 4) && ((tempQNode->note.type & 0x0FFF) == (RP_NOTE_TYPE_BT_FIRST >> i)) && (tempQNode->note.hitted != 1)) {
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
            vRpVertices[i][6] = ((float)(tempQNode->note.measure) + (float)(tempQNode->note.order)/(float)(tempQNode->note.max)) * 2.0
                                - userData->temp - lowNoteWidth;
            vRpVertices[i][11] = ((float)(tempQNode->note.measure) + (float)(tempQNode->note.order)/(float)(tempQNode->note.max)) * 2.0
                                - userData->temp - lowNoteWidth;
            vRpVertices[i][16] = ((float)(tempQNode->note.measure) + (float)(tempQNode->note.order)/(float)(tempQNode->note.max)) * 2.0
                                 - userData->temp + highNoteWidth;
 
            // Load the vertex position
            glVertexAttribPointer ( userData->positionLoc[0], 3, GL_FLOAT,
                                    GL_FALSE, 5 * sizeof(GLfloat), vRpVertices[i] );
            // Load the texture coordinate
            glVertexAttribPointer ( userData->texCoordLoc[0], 2, GL_FLOAT,
                                    GL_FALSE, 5 * sizeof(GLfloat), &vRpVertices[i][3] );

            glEnableVertexAttribArray( userData->positionLoc[0] );
            glEnableVertexAttribArray( userData->texCoordLoc[0] );

            // Bind the texture
            glActiveTexture ( GL_TEXTURE0 );
            glBindTexture ( GL_TEXTURE_2D, userData->textureId[0] );

            // Set the sampler texture unit to 0
            glUniform1i ( userData->samplerLoc[0], 0 );
         
            glDrawElements ( GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, indices );
         }
      }
      tempQNode = tempQNode->link;
   }


   // LED Enable
   setOutputGPIO(userData->gpioStat);

}

///
// Cleanup
//
void ShutDown ( ESContext *esContext )
{
   int i;
   UserData *userData = esContext->userData;

   for(i = 0; i < 2; i++) {
      // Delete texture object
      glDeleteTextures ( 1, &userData->textureId[i] );

      // Delete program object
      glDeleteProgram ( userData->programObject[i] );   
   }

   free(esContext->userData);
}

void Update ( ESContext *esContext, float deltaTime ) {
   UserData *userData = ( UserData* ) esContext->userData;
   
   //deltaTime = 120pbm(4/4 bit), userData->ki->t = bpm of song
   userData->temp += deltaTime * (userData->ki->t / 120.0);
   userData->measureBar += deltaTime * (userData->ki->t / 120.0);
   userData->prevGpioStat = userData->gpioStat;
   userData->gpioStat = inputGPIOStat();
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

   // Fork for play music
   pid = fork();

   switch(pid) {
      case -1:
      {
         printf("child process can't be created\n");
         return -1;
      }
      case 0:     // Play music
      {
         //system("omxplayer ../songs/ksm/homura/homura.ogg");
         execlp("omxplayer", "omxplayer", "../songs/ksm/homura/homura_lt_f.ogg");
      }
      default:
      {
         esMainLoop ( &esContext );
      }
   }

   ShutDown ( &esContext );

   return 0;
}
