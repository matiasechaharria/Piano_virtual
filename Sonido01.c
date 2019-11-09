/*
		Autor: Matias Echavarria
		Fecha: sábado, 05 de marzo de 2016


para compilar 
gcc -o PDICAlgebra Vectores01.c Algebra01.c Sonido01.c -lopencv_core -lopencv_highgui -lopencv_imgproc -lopenal -lm -Wall

*/

#include "Vectores01.h"


extern ALCdevice  * openal_output_device;
extern ALCcontext * openal_output_context;
extern ALenum current_playing_state;

extern ALuint internal_buffer;
extern ALuint streaming_source[1];
extern int key;

//********************************************************************************
//----Funciones utilizadas para el sonido 
// Todas funciones que recurren a las librerias de OpenAL

int al_check_error(const char * given_label) { // generic OpenAL error checker

    ALenum al_error;
    al_error = alGetError();

    if(AL_NO_ERROR != al_error) {

        printf("ERROR - %s  (%s)\n", alGetString(al_error), given_label);
        return al_error;
    }
    return 0;
}

void MM_init_al(void) 
{ // initialize OpenAL

    const char * defname = alcGetString(NULL, ALC_DEFAULT_DEVICE_SPECIFIER);

    openal_output_device  = alcOpenDevice(defname);
    openal_output_context = alcCreateContext(openal_output_device, NULL);
    alcMakeContextCurrent(openal_output_context);

    // setup buffer and source

    alGenBuffers(1, & internal_buffer);
    al_check_error("failed call to alGenBuffers");
	alGenSources(1, & streaming_source[0]);
    al_check_error("failed call to alGenSources");
}

void MM_exit_al(void) 
{ // tear down OpenAL after rendering finishes

    // ALenum errorCode = 0;

    // Stop the sources
    alSourceStopv(1, & streaming_source[0]);        //      streaming_source
    int ii;
    for (ii = 0; ii < 1; ++ii) {
        alSourcei(streaming_source[ii], AL_BUFFER, 0);
    }
    // Clean-up
    alDeleteSources(1, &streaming_source[0]);
    alDeleteBuffers(16, &streaming_source[0]);
    alcMakeContextCurrent(NULL);
    alcDestroyContext(openal_output_context);
    alcCloseDevice(openal_output_device);

	printf("fin de envio de sonido\n");
}

void MM_render_one_buffer(int key) {

   

	int seconds = 2,i=0;	// this determines buffer size based on seconds of playback audio independent of sample rate
	//unsigned sample_rate = 22050;
	unsigned sample_rate = 44100;
	double my_pi = 3.14159;
	size_t buf_size = seconds * sample_rate;



	//Muestras de sonido para cada una de las teclas
	short * samples0 = malloc(sizeof(short) * buf_size); // allocates audio buffer memory
	short * samples1 = malloc(sizeof(short) * buf_size); // allocates audio buffer memory
	short * samples2 = malloc(sizeof(short) * buf_size); // allocates audio buffer memory
	short * samples3 = malloc(sizeof(short) * buf_size); // allocates audio buffer memory
	short * samples4 = malloc(sizeof(short) * buf_size); // allocates audio buffer memory
	short * samples5 = malloc(sizeof(short) * buf_size); // allocates audio buffer memory
	short * samples6 = malloc(sizeof(short) * buf_size); // allocates audio buffer memory
	short * samples7 = malloc(sizeof(short) * buf_size); // allocates audio buffer memory



	if(samples0==NULL){printf("error 0 ");}
	if(samples1==NULL){printf("error 1 ");}
	if(samples2==NULL){printf("error 2 ");}
	if(samples3==NULL){printf("error 3 ");}
	if(samples4==NULL){printf("error 4 ");}
	if(samples5==NULL){printf("error 5 ");}
	if(samples6==NULL){printf("error 6 ");}
	if(samples7==NULL){printf("error 7 ");}

		
	// toma el recurso

	alGenBuffers(1, & internal_buffer);
	al_check_error("failed call to alGenBuffers");

	//genera el vector con las muestras del tono a utilizar
	for(i=0; i<buf_size; ++i) 
		{

	// Genero los vectores para luego mandar el que necesito al buffer de audio

	samples0[i] = 32760 * sin( (2.f * my_pi * 65.406)/sample_rate * i ); // populate audio curve data point
	samples1[i] = 32760 * sin( (2.f * my_pi * 130.813)/sample_rate * i ); // populate audio curve data point
	samples2[i] = 32760 * sin( (2.f * my_pi * 261.626)/sample_rate * i ); // populate audio curve data point
	samples3[i] = 32760 * sin( (2.f * my_pi * 523.251)/sample_rate * i ); // populate audio curve data point
	samples4[i] = 32760 * sin( (2.f * my_pi * 1045.502)/sample_rate * i ); // populate audio curve data point
	samples5[i] = 32760 * sin( (2.f * my_pi * 2093.005)/sample_rate * i ); // populate audio curve data point
	samples6[i] = 32760 * sin( (2.f * my_pi * 4186.009 )/sample_rate * i ); // populate audio curve data point
	samples7[i] = 32760 * sin( (2.f * my_pi * 8372.018)/sample_rate * i ); // populate audio curve data point

	}


	// upload buffer to OpenAL
	switch(key)
	{
	case 0:  alBufferData( internal_buffer, AL_FORMAT_MONO16, samples0, buf_size, sample_rate); ;break;
	case 1:  alBufferData( internal_buffer, AL_FORMAT_MONO16, samples1, buf_size, sample_rate); break;
	case 2:  alBufferData( internal_buffer, AL_FORMAT_MONO16, samples2, buf_size, sample_rate); break;
	case 3:  alBufferData( internal_buffer, AL_FORMAT_MONO16, samples3, buf_size, sample_rate); break;
	case 4:  alBufferData( internal_buffer, AL_FORMAT_MONO16, samples4, buf_size, sample_rate); break;
	case 5:  alBufferData( internal_buffer, AL_FORMAT_MONO16, samples5, buf_size, sample_rate); break;
	case 6:  alBufferData( internal_buffer, AL_FORMAT_MONO16, samples6, buf_size, sample_rate); break;
	case 7:  alBufferData( internal_buffer, AL_FORMAT_MONO16, samples7, buf_size, sample_rate); break;
	}

    al_check_error("failed call to alBufferData");


	/// Set-up sound source and play buffer

	alGenSources(1, & streaming_source[0]);
	alSourcei(streaming_source[0], AL_BUFFER, internal_buffer);
	alSourcePlay(streaming_source[0]);
    al_check_error("alGetSourcei AL_SOURCE_STATE 206");

	// ---------------------
	//libero la memoria que tenia asiganada para los tonos del teclado

	free(samples0); // release audio buffer memory after above upload into OpenAL internal buffer
	free(samples1); // release audio buffer memory after above upload into OpenAL internal buffer
	free(samples2); // release audio buffer memory after above upload into OpenAL internal buffer
	free(samples3); // release audio buffer memory after above upload into OpenAL internal buffer
	free(samples4); // release audio buffer memory after above upload into OpenAL internal buffer
	free(samples5); // release audio buffer memory after above upload into OpenAL internal buffer
	free(samples6); // release audio buffer memory after above upload into OpenAL internal buffer
	free(samples7); // release audio buffer memory after above upload into OpenAL internal buffer

}   //  MM_render_one_buffer


//-----Fin de librerias de OpenAL------------------
//###############################################################################
//#   				  	  -  -- --- End of File --- --  -						#
//#	 																			#
//###############################################################################
