/*
		Autor: Matias Echavarria
		Fecha: sábado, 05 de marzo de 2016


para compilar 
gcc -o PDICAlgebra Vectores01.c Algebra01.c Sonido01.c -lopencv_core -lopencv_highgui -lopencv_imgproc -lopenal -lm -Wall

*/
//-----Librerias Estandar------------------
#include "Vectores01.h"

//---varialbes de Opencv

IplImage *ImagenWork=NULL;
CvScalar VectorMedia[8];
CvRect rect[8];
double distPuntos[8];
double FondoDedo[8];
int TrackR=111, TrackG=71, TrackB=71;
int TrackBack=30;
CvScalar Pixel;

//---varialbes de Opencv

//---varialbes para el sonido 
ALCdevice  * openal_output_device;
ALCcontext * openal_output_context;
ALenum current_playing_state;

ALuint internal_buffer;
ALuint streaming_source[1];
//---fin de varialbes de Openal






//----------Trackbar--------
void MiCallbackTrackbarR(int posicion){
	TrackR = posicion;

}
void MiCallbackTrackbarG(int posicion){
	TrackG = posicion;

}
void MiCallbackTrackbarB(int posicion){
	TrackB = posicion;

}

void MiCallbackMouse(int Evento, int x, int y, int flags, void* param )
{

	IplImage *ImagenEntrada=(IplImage*)param;
	int i;
	

	switch( Evento ){
	    case CV_EVENT_LBUTTONDOWN:

	    	Pixel = cvGet2D(ImagenEntrada, y, x);
			TrackR=(int)Pixel.val[2];
			TrackG=(int)Pixel.val[1];
			TrackB=(int)Pixel.val[0];
	        printf("LBUTTONDOWN en x=%d, y=%d. Color R=%d, G=%d, B=%d \n", x, y, TrackR, TrackG, TrackB );
		//getchar();
//	        cvSetTrackbarPos("Rojo", "frameShow", TrackR);//para debug
//	        cvSetTrackbarPos("Verde", "frameShow", TrackG);//para debug
//	        cvSetTrackbarPos("Azul", "frameShow", TrackB);//para debug

			  for(i=0;i<8;i++)
			  {
			    cvSetImageROI(ImagenWork,rect[i]);
			    VectorMedia[i]=cvAvg(ImagenWork,NULL);
			    cvResetImageROI(ImagenWork);
		      printf("Media de la tecla %d:Color R=%lf, G=%lf, B=%lf\n", i, VectorMedia[i].val[2], VectorMedia[i].val[1], VectorMedia[i].val[0]);
			  }

			for(i=0;i<8;i++)
			{
			      FondoDedo[i]=DistanciasPuntos(VectorMedia[i],Pixel);
			      printf("FondoDedo[%d]= %lf \n", i,  FondoDedo[i]);
			}	  

	        break;

	    case CV_EVENT_RBUTTONDOWN:
	        printf("RBUTTONDOWN\n");
	        break;

	    case CV_EVENT_FLAG_CTRLKEY:
	        printf("FLAG_LBUTTONDBLCLK\n");
	        break;
	}
}
//_---------------------------------
//--------TrackBack pala la sencibilidad------


void MiCallbackTrackbar (int posicion)
{
	TrackBack=posicion;
	
	if(TrackBack<0)
	{TrackBack=0;
	}
	if(TrackBack>100)
	{TrackBack=100;
	}
}



//_-----------------
int main(void)
{

IplImage *Imagen=NULL,*ImagenShow=NULL;
CvCapture *capture = NULL;
CvScalar media;

int key=0,EndFlag=0;
int  VirtualKey=9,FlagInit =1;
int height=0,width=0;
int i=0,espacioROI=0;
double Producto=0;

//CvScalar bajo,alto;//para debug
//IplImage *ImagenRange=NULL;//para debug


	capture = cvCaptureFromCAM(0);									// Inicializo dispositivo de video
	//     Inicializo la parte de audio
	MM_init_al();

	if( !capture )
	{												// Fallo abriendo dispositivo video
		fprintf(stderr,"Imposible iniciar captura ...255\n");
	}
	else
	{
			Pixel.val[2]=TrackR;
			Pixel.val[1]=TrackG;
			Pixel.val[0]=TrackB;
			
			Imagen = cvQueryFrame(capture);

			height=	Imagen->height;
			width=Imagen->width;
			ImagenWork=cvCreateImage(cvSize(Imagen->width,Imagen->height),Imagen->depth,3);
			ImagenShow=cvCreateImage(cvSize(Imagen->width,Imagen->height),Imagen->depth,3);
//			ImagenRange=cvCreateImage(cvSize(Imagen->width,Imagen->height),Imagen->depth,1);  //para debug
			
			cvCopy(Imagen,ImagenWork,NULL);
			cvCopy(Imagen,ImagenShow,NULL);
		
			//creamos las areas de interes
			espacioROI = (0.25*width/(8));	//El 25% del ancho de cada tecla pertenecerá a una zona muerta
			for(i=0;i<8;i++)
			{
				rect[i]=cvRect((width/8)*i+espacioROI/2,0,(width/8)-espacioROI/2,height/4);
			}

			cvShowImage("frameShow", ImagenShow);
//			cvCreateTrackbar("Rojo", "frameShow", &TrackR, 255, MiCallbackTrackbarR);   //TrackBack para control
//			cvCreateTrackbar("Verde", "frameShow", &TrackG, 255, MiCallbackTrackbarG);  //TrackBack para control
//			cvCreateTrackbar("Azul", "frameShow", &TrackB, 255, MiCallbackTrackbarB);   //TrackBack para control
			cvCreateTrackbar("TrackBack","frameShow", &TrackBack, 100, MiCallbackTrackbar );
			cvSetMouseCallback("frameShow", MiCallbackMouse, (void*)ImagenShow);
			
			  //iniclializo el vector de medias
			  for(i=0;i<8;i++)
			  {
			    cvSetImageROI(ImagenWork,rect[i]);
			    VectorMedia[i]=cvAvg(ImagenWork,NULL);
			    cvResetImageROI(ImagenWork);
			  }
			  for(i=0;i<8;i++)
			  {
			      FondoDedo[i]=DistanciasPuntos(VectorMedia[i],Pixel);
			  }	  
			
		while(!EndFlag)	
		{
		  	
			alGetSourcei(streaming_source[0], AL_SOURCE_STATE, & current_playing_state);
			al_check_error("alGetSourcei AL_SOURCE_STATE ");	
			if(AL_PLAYING == current_playing_state && FlagInit==0)
			{
				  //printf("esta reproduciendo....\n");
				 // pinto la tecla que ha sido presionada
				if(VirtualKey!=9)
				{
				cvRectangle(ImagenShow,cvPoint(width/8*VirtualKey+espacioROI/2,0),cvPoint(width/8*(VirtualKey+1)-espacioROI/2,height/4),CV_RGB(0,255,0),CV_FILLED, 8,0);
				}
			}
			else
			{
				      //recorro las ROI
				  for(i=0;i<8;i++)
				  {
				    
				      cvSetImageROI(ImagenWork,rect[i]);
				      media=cvAvg(ImagenWork,NULL);// tomo la media de la ROI
				      cvResetImageROI(ImagenWork);

				      Producto=Proyeccion( VectorMedia[i] ,Pixel , media);
				        
//				      Impresiones(VectorMedia[i],Pixel,media);//imprime por consola los puntos que está utilizando para debug
				      
				      if((Producto)>(FondoDedo[i])*TrackBack/100)
				      {  
					
					VirtualKey=i;
					break;
				      }else
				      {VirtualKey=9;}
				  }
			  
				  if(VirtualKey!=9)
				  {
//				  printf("la tecla pulsada es %d\n", VirtualKey);//para debug
				  //----le mando al bufer el sonido que tiene que salir
				  MM_render_one_buffer(VirtualKey);//Emite un sonido segun la tecla
				  }
					    
			}// fin del if de reproduccion
			
				    //-----cosas para el debug
				    //----inicio 
/*	      
						  if((TrackB-TrackBack)<0)
						  { bajo.val[0]=0;}
						  else
						  { bajo.val[0]=(TrackB-TrackBack);			  
						  }
						  if((TrackG-TrackBack)<0)
						  { bajo.val[1]=0;}
						  else
						  { bajo.val[1]=(TrackG-TrackBack);			  
						  }
						  if((TrackR-TrackBack)<0)
						  { bajo.val[2]=0;}
						  else
						  { bajo.val[2]=(TrackR-TrackBack);			  
						  }
						  
						  if(255<abs(TrackB+TrackBack))
						  { bajo.val[0]=255;}
						  else
						  { bajo.val[0]=(TrackB+TrackBack);			  
						  }
						  if(255<abs(TrackG+TrackBack))
						  { bajo.val[1]=255;}
						  else
						  { bajo.val[1]=(TrackG+TrackBack);			  
						  }
						  if(255<abs(TrackR+TrackBack))
						  { bajo.val[2]=255;}
						  else
						  { bajo.val[2]=(TrackR+TrackBack);			  
						  }						      
						  cvInRangeS(Imagen,bajo ,alto, ImagenRange);
						  cvShowImage("cvInRanges", ImagenRange);
*/
				    //----Fin de cosas para el debug
				    
			//muestro la imagen final del procesamiento
			cvShowImage("frameShow", ImagenShow);
			//Tomo otra imagen
			Imagen = cvQueryFrame(capture);
			cvCopy(Imagen,ImagenShow,NULL);
			cvCopy(Imagen,ImagenWork,NULL);	//Para debug  

			// Esta es para que el usuario vea el tecaldo
			for(i=0;i<8;i++) 
			{// dibujo las teclas en la imagen
			cvRectangle(ImagenShow,cvPoint(width/8*i,0),cvPoint(width/8*(i+1),height/4),CV_RGB(0,0,0),2,2,0);
			}	
			
			
			// pregunto por Esc para cerrar el programa
			key = cvWaitKey(1);

			switch ((char)key) 
			{

			  case ESC_KEY:
				  printf("Terminando ...\n");
				  EndFlag = 1;
				  break;		
	
			}
			
			
			//Flag solo para inicializar
			FlagInit=0;
		}//Fin del while(!EndFlag)
			
	}// Fin del if( !capture )

//	cvDestroyWindow("cvInRanges");//Para debug
	cvDestroyWindow("frameShow ");	
	cvReleaseCapture(&capture);
	cvReleaseImage(&Imagen);
	cvReleaseImage(&ImagenShow);
//	cvReleaseImage(&ImagenRange);//Para debug
	cvReleaseImage(&ImagenWork);
	//------libero los recursos de OpenAL 
	MM_exit_al();   

	
	printf("Fin del programa \n\n");
return(0);
}

//###############################################################################
//#   				  	  -  -- --- End of File --- --  -						#
//#	 																			#
//###############################################################################
