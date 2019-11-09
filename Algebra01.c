/*
		Autor: Matias Echavarria
		Fecha: sábado, 05 de marzo de 2016


para compilar 
gcc -o PDICAlgebra Vectores01.c Algebra01.c Sonido01.c -lopencv_core -lopencv_highgui -lopencv_imgproc -lopenal -lm -Wall

*/


#include "Vectores01.h"


extern CvScalar VectorMedia[8];
extern double distPuntos[8];
extern double FondoDedo[8];
extern int TrackR, TrackG, TrackB;
extern int TrackBack;
extern CvScalar Pixel;

float mod(vector v)
{
      float mod;
      
      mod=sqrt(v.compx*v.compx+v.compy*v.compy+v.compz*v.compz);
      
      return mod;
}
 vector dospunts(CvScalar p1,CvScalar p2)
{
       struct vector v;
       
       v.compx=(p1.val[0]-p2.val[0]);
       v.compy=(p1.val[1]-p2.val[1]);
       v.compz=(p1.val[2]-p2.val[2]);
       if(v.compx<0){v.compx=0;}
       if(v.compy<0){v.compy=0;}
       if(v.compz<0){v.compz=0;}
       return v;
}
float prodescalar(vector v1,vector v2)
{
      float d;
      d=v1.compx*v2.compx+v1.compy*v2.compy+v1.compz*v2.compz;
      return d;
}
CvScalar puntoenrecta(recta r, float l)
{
      CvScalar p;
      p.val[0]=r.puntoori.val[0]+r.vectordir.compx*l;
      p.val[1]=r.puntoori.val[1]+r.vectordir.compy*l;
      p.val[2]=r.puntoori.val[2]+r.vectordir.compz*l;
      return p;
}
double DistanciasPuntos (CvScalar base, CvScalar dedo )
{
  double distancia;
  distancia=sqrt(pow(base.val[0]-dedo.val[0],2)+pow(base.val[1]-dedo.val[1],2)+pow(base.val[2]-dedo.val[2],2));
  return(distancia);
}

void Impresiones(CvScalar base , CvScalar dedo, CvScalar captado)// para debug
{
  printf("Colores del fondo en Color   R=%f, G=%f, B=%f \n", base.val[2], base.val[1], base.val[0] );
  printf("Colores del dedo en Color    R=%f, G=%f, B=%f \n", dedo.val[2], dedo.val[1], dedo.val[0] );
  printf("Colores del captado en Color R=%f, G=%f, B=%f \n", captado.val[2], captado.val[1], captado.val[0] );
}
float D(plano pl)
{
      float d;
      d=pl.vectorperpen.compx*pl.puntoori.val[0]+pl.vectorperpen.compy*pl.puntoori.val[1]+pl.vectorperpen.compz*pl.puntoori.val[2];
      d=-d;
      return d;
}

float planoenpunto(plano pl,CvScalar p)
{
      float d;
      d=pl.vectorperpen.compx*p.val[0]+pl.vectorperpen.compy*p.val[1]+pl.vectorperpen.compz*p.val[2]+D(pl);      
      return d;
}
CvScalar interseccionplarec(plano pl,recta r)
{
      float l;
      l=-planoenpunto(pl,r.puntoori)/prodescalar(pl.vectorperpen,r.vectordir);
      return (puntoenrecta(r,l));
}

float distanciapunts(CvScalar p1,CvScalar p2)
{
      vector v;
      v=dospunts(p1,p2);    
      return mod(v);
}

double Proyeccion(CvScalar base , CvScalar dedo, CvScalar captado)
{
  double dist=0, DistDedo=0, DistBase=0;
  CvScalar Origen,PuntoAux;
  plano PlanoAux;
  recta RectaAux;
  
  //Armo mi origen de coordenadas
  Origen.val[0]=0;
  Origen.val[1]=0;
  Origen.val[2]=0;
    
  DistDedo= DistanciasPuntos(Origen,dedo);
  DistBase=DistanciasPuntos(Origen,base);
  // esto es para que no sature ya que me tengo que mantener en el cuadrante donde todos son positivos de 0 a 255
  if(DistBase<DistDedo)
  {
  
  //armo la recta en el espacio
  RectaAux.puntoori=base;
  RectaAux.vectordir=dospunts(dedo,base);
  //armo el plano director
    PlanoAux.puntoori=captado;
    PlanoAux.vectorperpen=dospunts(dedo,base);
    //interseccion entre el plano y la recta
    PuntoAux=interseccionplarec(PlanoAux,RectaAux);
    //calculo la distancia entre el fondo y la interseccion
    
//	printf("Punto de interseccion   R=%f, G=%f, B=%f \n", PuntoAux.val[2], PuntoAux.val[1], PuntoAux.val[0] );// para debug
    
    if(DistanciasPuntos(PuntoAux,Origen)<DistanciasPuntos(base,Origen))
    { 
//	printf("caso 3 esta mas lejos de lo que corresponde, PuntoAux-Base-Dedo\n");// para debug
      dist=DistanciasPuntos(base,PuntoAux);
     }else{
	if(DistanciasPuntos(dedo,Origen)<DistanciasPuntos(PuntoAux,Origen))
	{
//	printf("caso 2 casi es un dedo base-dedo-PuntoAux \n");// para debug
	dist=DistanciasPuntos(base,PuntoAux);
	}
	else //está en el medio
	{
//	printf("\ncaso 1 está en el medio base-PuntoAux-dedo\n");// para debug
	dist=DistanciasPuntos(base,PuntoAux);
	}
    }
//	printf("DistDedo>DistBase => Producto=%f \n", dist);// para debug
  }else
  {
  //armo la recta en el espacio
  RectaAux.puntoori=dedo;
  RectaAux.vectordir=dospunts(base,dedo);
  //armo el plano director
    PlanoAux.puntoori=captado;
    PlanoAux.vectorperpen=dospunts(base,dedo);
    //interseccion entre el plano y la recta
    PuntoAux=interseccionplarec(PlanoAux,RectaAux);
    //calculo la distancia entre el fondo y la interseccion
    
// 	printf("Punto de interseccion   R=%lf, G=%lf, B=%lf \n", PuntoAux.val[2], PuntoAux.val[1], PuntoAux.val[0] );// para debug
    
    if(DistanciasPuntos(PuntoAux,Origen)<DistanciasPuntos(dedo,Origen))
    {
//	printf("caso 3__casi es un dedo PuntoAux-dedo-base\n");// para debug
        dist=DistanciasPuntos(base,PuntoAux);      
     }else{
	    if(DistanciasPuntos(base,Origen)<DistanciasPuntos(PuntoAux,Origen))
	    {
//	printf("caso 2___es mas lejos de lo que corresponde dedo-base-PuntoAuxs \n");// para debug
	     dist= DistanciasPuntos(base,PuntoAux);
	    }
	    else //está en el medio
	    {
//	printf("\ncaseo 1___está en el medio dedo-PuntoAux-base\n");// para debug
	    dist=DistanciasPuntos(base,PuntoAux);
	    }
	  }
//   	printf(" no es DistDedo>DistCampato => Producto=%f \n", dist);// para debug
  }
  
//	printf("la distancia proyectadda es: %f\n" ,dist);// para debug
  return(dist);
}
//###############################################################################
//#   				  	  -  -- --- End of File --- --  -						#
//#	 																			#
//###############################################################################
