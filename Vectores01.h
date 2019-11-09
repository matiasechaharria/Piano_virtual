/*
		Autor: Matias Echavarria
		Fecha: sábado, 05 de marzo de 2016


para compilar 
gcc -o PDICAlgebra Vectores01.c Algebra01.c Sonido01.c -lopencv_core -lopencv_highgui -lopencv_imgproc -lopenal -lm -Wall

*/

/*
 
	Para descargar las librerias de OpenAl
 	sudo apt-get install libopenal-dev

	Para descargar las librerias de OpenCV
	sudo apt-get install libopencv-dev
*/


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>		

#define ESC_KEY			27
//--------Fin de Librerias Estandar------------------

//-----Librerias de OpenCV------------------
#include <opencv2/core/core_c.h>

/* Contiene funciones para interactuar con la interfaz gráfica y las funciones
 * para abrir y almacenar imágenes y video
 */
#include <opencv2/highgui/highgui_c.h>
#include <opencv2/imgproc/types_c.h>
#include <opencv2/imgproc/imgproc_c.h>

//----- Fin de Librerias de OpenCV------------------
  


//-----Librerias de OpenAL------------------
#include <AL/al.h>
#include <AL/alc.h>

#define ESC_KEY		27
struct vector
{
       float compx;
       float compy;
       float compz;
} typedef vector;
struct recta
{
       CvScalar puntoori;
       vector vectordir;
} typedef recta;
struct plano
{
       CvScalar puntoori;
       vector vectorperpen;
}typedef plano;



//funciones algebraicas
/*
 * todo esto es porque para calcular la distancia entre el punto proyectado sobre la recta que pasa entre el punto del Pixel a detectar y el fondo
 */
float mod(vector v);
vector dospunts(CvScalar p1,CvScalar p2);
float prodescalar(vector v1,vector v2);
CvScalar puntoenrecta(recta r, float l);
double DistanciasPuntos (CvScalar base, CvScalar dedo );
void Impresiones(CvScalar base , CvScalar dedo, CvScalar captado);
float D(plano pl);
float planoenpunto(plano pl,CvScalar p);
CvScalar interseccionplarec(plano pl,recta r);
float distanciapunts(CvScalar p1,CvScalar p2);
double Proyeccion(CvScalar base , CvScalar dedo, CvScalar captado);

//---Funciones para el sonido---- 
int al_check_error(const char * given_label);
void MM_init_al(void) ;
void MM_exit_al(void) ;
void MM_render_one_buffer(int key) ;
//###############################################################################
//#   				  	  -  -- --- End of File --- --  -						#
//#	 																			#
//###############################################################################
