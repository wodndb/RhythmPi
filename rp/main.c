#include "main.h"

///
// Create a simple 2x2 texture image with four different colors
//
GLuint CreateSimpleTexture2D(ESContext *esContext, int imageIndex)
{
   // Texture object handle
   GLuint textureId;
   UserData *userData = esContext->userData;
   
   GLubyte *pixels = userData->image[imageIndex];

   printf("Create Simple Texture 2D\n");

   // Use tightly packed data
   glPixelStorei ( GL_UNPACK_ALIGNMENT, 1 );

   // Generate a texture object
   glGenTextures ( 1, &textureId );

   // Bind the texture object
   glBindTexture ( GL_TEXTURE_2D, textureId );
   printf("Loaded texture\n");

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
   char imageName[IMG_MAX_NUM][80] = {"../img/note.tga",
                                      "../img/gpio_input.tga",
                                      "../img/main.tga",
                                      "../img/select_menu.tga",
                                      "../img/option.tga",
                                      "../img/end.tga",
                                      "../img/select_cursor.tga",
                                      "../img/good.tga",
                                      "../img/bad.tga"};

   esContext->userData = malloc(sizeof(UserData));

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

   for(i = 0; i < IMG_MAX_NUM; i++) {
         printf("load image : %s\n", imageName[i]);
         userData->image[i] = esLoadTGA(imageName[i], &userData->width[i], &userData->height[i]);
         if (userData->image[i] == NULL) {
             fprintf(stderr, "No such image\n");
             exit(1);
         }
         printf("Width %d height %d\n", userData->width[i], userData->height[i]);
      
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


   userData->stageState = STAGE_MAIN;
   userData->musicNum = 0;
   userData->musicPid = 0;
   
   userData->selectedOption = 0;
   userData->selectedSpeed = 1;
   userData->speed = 1;

   /*
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
   */

   // GPIO Initialization
   initPinMode();
   userData->gpioStat = 0x00;
   userData->prevGpioStat = 0x00;

   printf("Start registering es functions\n");

   glClearColor ( 0.0f, 0.0f, 0.0f, 1.0f );
   return GL_TRUE;
}

void loadImage(ESContext *esContext, int imageID, GLfloat* imageVertex) {
	GLushort indices[] = {0, 1, 2, 0, 2, 3};
	UserData *userData = (UserData*)esContext->userData;

	// Use the program object
	glUseProgram ( userData->programObject[imageID] );

	// Load the vertex position
	glVertexAttribPointer ( userData->positionLoc[imageID], 3, GL_FLOAT, 
							GL_FALSE, 5 * sizeof(GLfloat), imageVertex );
	// Load the texture coordinate
	glVertexAttribPointer ( userData->texCoordLoc[imageID], 2, GL_FLOAT,
							GL_FALSE, 5 * sizeof(GLfloat), &imageVertex[3] );

	glEnableVertexAttribArray ( userData->positionLoc[imageID] );
	glEnableVertexAttribArray ( userData->texCoordLoc[imageID] );

	// Bind the texture
	glActiveTexture ( GL_TEXTURE0 );
	glBindTexture ( GL_TEXTURE_2D, userData->textureId[imageID] );

	// Set the sampler texture unit to 0
	glUniform1i ( userData->samplerLoc[imageID], 0 );

	glDrawElements ( GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, indices );
}

///
// Draw a triangle using the shader pair created in Init()
//
void Draw ( ESContext *esContext )
{
   int i = 0;
   pid_t pid;
   int hittedGpioStat = 0x00;
   float lowNoteWidth = 0;
   float highNoteWidth = 0;
   UserData *userData = ( UserData* ) esContext->userData;
   QNode* tempQNode = NULL;
   FILE* cmdStream;
   int cmdPid;

   char musicKshFile[4][80] = {"../songs/ksm/colorfulsky/colorfulsky_lt.ksh",
                                "../songs/ksm/bigecho/bigecho_lt.ksh",
                                "../songs/ksm/homura/homura_lt.ksh",
                                "../songs/ksm/dolphinstalker/dolphinstalker_lt.ksh"};

   char musicOggFile[4][80] = {"../songs/ksm/colorfulsky/colorfulsky_lt_f.ogg",
                                "../songs/ksm/bigecho/bigecho_lt_f.ogg",
                                "../songs/ksm/homura/homura_lt_f.ogg",
                                "../songs/ksm/dolphinstalker/dolphinstalker_lt_f.ogg"};
   
   GLushort indices[] = { 0, 1, 2, 0, 2, 3 };

   if(userData->stageState == STAGE_PLAY) {
      tempQNode = userData->qtNote->front;
   }

   // Set the viewport
   glViewport ( 0, 0, esContext->width, esContext->height );
   
   // Clear the color buffer
   glClear ( GL_COLOR_BUFFER_BIT );

   if(userData->stageState == STAGE_MAIN) {
	  loadImage(esContext, IMG_MAIN, vFullScreen);
      if((userData->gpioStat & 0x7F0) != 0 && (userData->prevGpioStat & 0x7F0) == 0) {
            userData->gpioStat = 0x00;
            userData->prevGpioStat = 0x00;
            userData->stageState = STAGE_SELECT_MUSIC;
      }
   }
   else if(userData->stageState == STAGE_SELECT_MUSIC) {
	  loadImage(esContext, IMG_SELECT_MENU, vFullScreen);
      
      setOutputGPIO(userData->gpioStat);
      
      if(checkRotDirection(userData->prevGpioStat, userData->gpioStat, 1) == ROT_RIGHT) {
            userData->musicNum = (userData->musicNum + 3) % 4;
      }
      else if(checkRotDirection(userData->prevGpioStat, userData->gpioStat, 1) == ROT_LEFT) {
            userData->musicNum = (userData->musicNum + 1) % 4;
      }
      
      vMusicSelect[1] = 0.41 - (0.24 * userData->musicNum);
      vMusicSelect[6] = 0.21 - (0.24 * userData->musicNum);
      vMusicSelect[11] = 0.21 - (0.24 * userData->musicNum);
      vMusicSelect[16] = 0.41 - (0.24 * userData->musicNum);
 	
	  loadImage(esContext, IMG_SELECT_CURSOR, vMusicSelect);

	  if((userData->gpioStat & 0x020) != 0 && (userData->prevGpioStat & 0x020) == 0) {
            userData->stageState = STAGE_PLAY;
            
            //Please load ksh file and set music
            printf("Load ksh data\n");
            //getKshData
            userData->ki = (KshInfo*)malloc(1 * sizeof(KshInfo));
            userData->kshFile = fopen(musicKshFile[userData->musicNum], "r");
            userData->qtNote = (QType*)malloc(1 * sizeof(QType));
      
            if(userData->kshFile == NULL) {
               printf("file load error!\n");
               printf("filename : %s\n", musicKshFile[userData->musicNum]);
               exit(1);
            }
      
            getKshInfo(userData->kshFile, userData->ki);
            loadKshNote(userData->kshFile, userData->qtNote);

            userData->temp = -1.0 * ((float)(userData->ki->t - 100) / 80.0 * 3.0);

            
            // Fork for play music
            pid = fork();
            
            switch(pid) {
               case -1:
               {
                  printf("child process can't be created\n");
                  exit(0);
               }
               case 0:     // Play music
               {
                  //system("omxplayer ../songs/ksm/homura/homura.ogg");
                  execlp("omxplayer", "omxplayer", musicOggFile[userData->musicNum], NULL);
               }
               default:
               {
                  userData->musicPid = pid;
               }
           }
      }
      
      setOutputGPIO(userData->gpioStat);
      
   }
   else if(userData->stageState == STAGE_OPTION) {
 	  loadImage(esContext, IMG_OPTION, vFullScreen);

	  setOutputGPIO(userData->gpioStat);
      
      if(checkRotDirection(userData->prevGpioStat, userData->gpioStat, 1) == ROT_RIGHT) {
            userData->selectedOption = (userData->selectedOption + 1) % 4;
      }
      else if(checkRotDirection(userData->prevGpioStat, userData->gpioStat, 1) == ROT_LEFT) {
            userData->selectedOption = (userData->selectedOption + 3) % 4;
      }
      
      vMusicSelect[1] = 0.5 - (0.32 * userData->selectedOption);
      vMusicSelect[6] = 0.3 - (0.32 * userData->selectedOption);
      vMusicSelect[11] = 0.3 - (0.32 * userData->selectedOption);
      vMusicSelect[16] = 0.5 - (0.32 * userData->selectedOption);
      
	  loadImage(esContext, IMG_SELECT_CURSOR, vMusicSelect);
      
      if((userData->gpioStat & 0x020) != 0 && (userData->prevGpioStat & 0x020) == 0) {
            //Reset ksh data -> to do in play music;
            
            if(userData->selectedOption == 0) {
                  //Speed
                  userData->stageState = STAGE_OPTION_SEL_SPD;
            }
            else if(userData->selectedOption == 1) {
                  //Restart
                  userData->stageState = STAGE_PLAY;
                  
                  //Please load ksh file and set music
                  printf("Load ksh data\n");
                  //getKshData
                  userData->ki = (KshInfo*)malloc(1 * sizeof(KshInfo));
                  userData->kshFile = fopen(musicKshFile[userData->musicNum], "r");
                  userData->qtNote = (QType*)malloc(1 * sizeof(QType));
                  userData->temp = -2.0;
            
                  if(userData->kshFile == NULL) {
                     printf("file load error!\n");
                     exit(1);
                  }
            
                  getKshInfo(userData->kshFile, userData->ki);
                  loadKshNote(userData->kshFile, userData->qtNote);
                  
                  // Fork for play music
                  pid = fork();
                  
                  switch(pid) {
                     case -1:
                     {
                        printf("child process can't be created\n");
                        exit(0);
                     }
                     case 0:     // Play music
                     {
                        //system("omxplayer ../songs/ksm/homura/homura.ogg");
                        execlp("omxplayer", "omxplayer", musicOggFile[userData->musicNum], NULL);
                     }
                     default:
                     {
                        userData->musicPid = pid;
                     }
                 }
            }
            else if(userData->selectedOption == 2) {
                  //SelectMusic
                  userData->stageState = STAGE_SELECT_MUSIC;
            }
            else if(userData->selectedOption == 3) {
                  //Main Menu
                  userData->stageState = STAGE_MAIN;
            }
      }
   }
   else if(userData->stageState == STAGE_OPTION_SEL_SPD) {
	  loadImage(esContext, IMG_OPTION, vFullScreen);
      
      setOutputGPIO(userData->gpioStat);
      
      if(checkRotDirection(userData->prevGpioStat, userData->gpioStat, 1) == ROT_RIGHT) {
            userData->selectedSpeed = (userData->selectedSpeed + 1) % 4;
      }
      else if(checkRotDirection(userData->prevGpioStat, userData->gpioStat, 1) == ROT_LEFT) {
            userData->selectedSpeed = (userData->selectedSpeed + 3) % 4;
      }
      
      //----------------Change This Code - move horizental-----------------------//
      vSpeedSelect[0] = -0.13 + (0.23 * userData->selectedSpeed);
      vSpeedSelect[5] = -0.13 + (0.23 * userData->selectedSpeed);
      vSpeedSelect[10] = -0.09 + (0.23 * userData->selectedSpeed);
      vSpeedSelect[15] = -0.09 + (0.23 * userData->selectedSpeed);
      
	  loadImage(esContext, IMG_SELECT_CURSOR, vSpeedSelect);
      
      setOutputGPIO(userData->gpioStat);
      
      if((userData->gpioStat & 0x020) != 0 && (userData->prevGpioStat & 0x020) == 0) {
            //Reset ksh data -> to do in play music;
            userData->speed = userData->selectedSpeed;
            userData->stageState = STAGE_OPTION;
      }
      else if((userData->gpioStat & 0x010) != 0 && (userData->prevGpioStat & 0x020) == 0) {
            userData->selectedSpeed = userData->speed;
            userData->stageState = STAGE_OPTION;
      }
   }
   else if(userData->stageState == STAGE_PLAY) {
         //==================
         
         //printf(".");
      
         // Judge notes : Input from GPIO
         while(tempQNode->link != NULL) {
            if(((float)(tempQNode->note.measure) + (float)(tempQNode->note.order)/(float)(tempQNode->note.max)) * 2.0
                  - userData->temp < -0.5)
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
         while(userData->qtNote->front->link != NULL) {
            if(((float)(userData->qtNote->front->note.measure) * 2.0) - userData->temp < - 10.0)
            {
                  dequeue(userData->qtNote);
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
			   loadImage(esContext, IMG_GPIO_INPUT, vGpioVertices[9-i]);
            }
         }
        
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
       			  
				  loadImage(esContext, IMG_NOTE, vRpVertices[i]);
               }
            }
            tempQNode = tempQNode->link;
         }
         // LED Enable
         setOutputGPIO(userData->gpioStat);
         
         //End Music
         if(userData->qtNote->front->link == NULL) {
               
               free(userData->ki);
               userData->ki = NULL;
               fclose(userData->kshFile);
               free(userData->qtNote);
               userData->qtNote = NULL;
               userData->selectedOption = 0;
               
               userData->stageState = STAGE_END;              
         }

         if((userData->gpioStat & 0x400) != 0) {
               //Initialize ksh Data;
               cmdStream = popen("ps -a | grep omxplayer.bin", "r");
               fscanf(cmdStream, "%d", &cmdPid); 
               kill(cmdPid, SIGKILL);           // close music
               
               pclose(cmdStream);

               cmdStream = popen("ps -a | grep omxplayer", "r");
               fscanf(cmdStream, "%d", &cmdPid); 
               kill(cmdPid, SIGKILL);           // close music

               pclose(cmdStream);

               while(userData->qtNote->front != NULL) {     // free qtNote;
                     dequeue(userData->qtNote);
               }
               free(userData->ki);
               userData->ki = NULL;
               fclose(userData->kshFile);
               free(userData->qtNote);
               userData->qtNote = NULL;
               userData->selectedOption = 0;
               
               userData->stageState = STAGE_OPTION;
         }         

  }
   else if(userData->stageState == STAGE_END) {
	  loadImage(esContext, IMG_END, vFullScreen);
      
      // LED Enable
      setOutputGPIO(userData->gpioStat);
      
      if(checkRotDirection(userData->prevGpioStat, userData->gpioStat, 1) == ROT_RIGHT) {
            userData->selectedOption = (userData->selectedOption + 1) % 2;
      }
      else if(checkRotDirection(userData->prevGpioStat, userData->gpioStat, 1) == ROT_LEFT) {
            userData->selectedOption = (userData->selectedOption + 1) % 2;
      }
      
      vMusicSelect[1] = -0.2 - (0.2 * userData->selectedOption);
      vMusicSelect[6] = -0.4 - (0.2 * userData->selectedOption);
      vMusicSelect[11] = -0.4 - (0.2 * userData->selectedOption);
      vMusicSelect[16] = -0.2 - (0.2 * userData->selectedOption);
 
	  loadImage(esContext, IMG_SELECT_CURSOR, vMusicSelect);
            
      if((userData->gpioStat & 0x020) != 0) {
            if(userData->selectedOption == 0) {
                  // Continue -> select music
                  userData->stageState = STAGE_SELECT_MUSIC;
            }
            else if(userData->selectedOption == 1) {
                  // Exit -> main
                  userData->selectedOption = 0;
                  userData->stageState = STAGE_MAIN;
            }
      }
   }
   else {
         printf("wrong stage number, exit program!");
         exit(1);
   }
 
}

///
// Cleanup
//
void ShutDown ( ESContext *esContext )
{
   int i;
   UserData *userData = esContext->userData;

   for(i = 0; i < IMG_MAX_NUM; i++) {
      // Delete texture object
      glDeleteTextures ( 1, &userData->textureId[i] );

      // Delete program object
      glDeleteProgram ( userData->programObject[i] );   
   }

   free(esContext->userData);
}

void Update ( ESContext *esContext, float deltaTime ) {
   UserData *userData = ( UserData* ) esContext->userData;

   
   if(userData->stageState == STAGE_PLAY) {
      //deltaTime = 120pbm(4/4 bit), userData->ki->t = bpm of song
      userData->temp += deltaTime * (userData->ki->t / 120.0);
      userData->measureBar += deltaTime * (userData->ki->t / 120.0);
   }
   else {
      userData->temp += deltaTime;
   }
   
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

   esInitContext ( &esContext );
   esContext.userData = &userData;

   esCreateWindow ( &esContext, "Hello Triangle", esContext.width, esContext.height, ES_WINDOW_RGB );

   if ( !Init ( &esContext ) )
      return 0;

   esRegisterDrawFunc ( &esContext, Draw );
   esRegisterUpdateFunc ( &esContext, Update );
   
   printf("Enter to esMainLoop\n");

   esMainLoop ( &esContext );

   printf("Shutdown\n");

   ShutDown ( &esContext );

   printf("Goodbye\n");

   return 0;
}
