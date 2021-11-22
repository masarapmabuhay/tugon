/*
 * Copyright 2021 SYSON, MICHAEL B.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 *     
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * @company: USBONG
 * @author: SYSON, MICHAEL B. 
 * @date created: 20211107
 * @date updated: 20211122
 * @website address: http://www.usbong.ph
 *
 * Reference:
 * 1) https://www.libsdl.org/projects/SDL_mixer/
 * --> https://www.libsdl.org/projects/SDL_mixer/docs/index.html
 * --> https://www.libsdl.org/projects/SDL_mixer/docs/demos/
 * --> all last accessed: 20211107
 * --> https://www.libsdl.org/projects/SDL_mixer/docs/SDL_mixer.html;
 * --> last accessed: 2021110
 *
 * Acknowledgments:
 * 1) "Bulalakaw Wars" Team (2007): 
 * Syson, M., Camacho, R., Gonzales, D., Del Rosario, R., Vidal, E., et al.
 *
*/

//TO-DO: -reverify: cause need to add "SDL2_mixer.dll" in output folder 

//edited by Mike, 20211107
/*
#include "SDL.h"
#include "SDL_thread.h"
#include "SDL_mixer.h"
*/

#ifdef _WIN32 //Windows machine
	#include <windows.h> //added by Mike, 20211120

	#include <SDL.h>
	#include <SDL_image.h>
	#include <SDL_thread.h>
	#include <SDL_mixer.h>
#else
	#include <SDL2/SDL.h>
	#include <SDL2/SDL_image.h>
	#include <SDL2/SDL_mixer.h>
#endif

#include <stdarg.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

//TO-DO: -reverify: this if necessary

/* set this to any of 512,1024,2048,4096              */
/* the lower it is, the more FPS shown and CPU needed */
#define BUFFER 1024
#define W 640 /* NEVER make this be less than BUFFER! */
#define H 480
#define H2 (H/2)
#define H4 (H/4)
#define Y(sample) (((sample)*H)/4/0x7fff)

Sint16 stream[2][BUFFER*2*2];
int len=BUFFER*2*2, done=0, need_refresh=0, bits=0, which=0,
	sample_size=0, position=0, rate=0;
SDL_Surface *s=NULL;
Uint32 flips=0;
Uint32 black,white;
float dy;

Mix_Music* sound_1; //added by Mike, 20211110


/******************************************************************************/
/* some simple exit and error routines                                        */

void errorv(char *str, va_list ap)
{
	vfprintf(stderr,str,ap);
	fprintf(stderr,": %s.\n", SDL_GetError());
}

void cleanExit(char *str,...)
{
	va_list ap;
	va_start(ap, str);
	errorv(str,ap);
	va_end(ap);
	Mix_CloseAudio();
	SDL_Quit();
	exit(1);
}

/******************************************************************************/
/* the postmix processor, only copies the stream buffer and indicates         */
/* a need for a screen refresh                                                */

static void postmix(void *udata, Uint8 *_stream, int _len)
{
	position+=_len/sample_size;
	/* fprintf(stderr,"pos=%7.2f seconds \r",position/(float)rate); */
	if(need_refresh)
		return;
	/* save the stream buffer and indicate that we need a redraw */
	len=_len;
	memcpy(stream[(which+1)%2],_stream,len>s->w*4?s->w*4:len);
	which=(which+1)%2;
	need_refresh=1;
}

/******************************************************************************/
/* redraw the wav and reset the need_refresh indicator                        */

void refresh()
{
	int x;
	Sint16 *buf;

	/*fprintf(stderr,"len=%d   \r",len); */

	buf=stream[which];
	need_refresh=0;
	
	SDL_LockSurface(s);
	/* clear the screen */
	/*SDL_FillRect(s,NULL,black); */

	/* draw the wav from the saved stream buffer */
	for(x=0;x<W*2;x++)
	{
		const int X=x>>1, b=x&1 ,t=H4+H2*b;
		int y1,h1;
		if(buf[x]<0)
		{
			h1=-Y(buf[x]);
			y1=t-h1;
		}
		else
		{
			y1=t;
			h1=Y(buf[x]);
		}
		{
			SDL_Rect r={X,H2*b,1};
			r.h=y1-r.y;
			SDL_FillRect(s,&r,0);
		}
		{
			SDL_Rect r={X,y1,1,h1};
			SDL_FillRect(s,&r,white);
		}
		{
			SDL_Rect r={X,y1+h1,1};
			r.h=H2+H2*b-r.y;
			SDL_FillRect(s,&r,0);
		}
	}
	SDL_UnlockSurface(s);
	
	//edited by Mike, 20211107
//	SDL_Flip(s);
//	SDL_RenderPresent(s);

	flips++;
}

/******************************************************************************/

void print_init_flags(int flags)
{
#define PFLAG(a) if(flags&MIX_INIT_##a) printf(#a " ")
	PFLAG(FLAC);
	PFLAG(MOD);
	PFLAG(MP3);
	PFLAG(OGG);
	if(!flags)
		printf("None");
	printf("\n");
}

/******************************************************************************/

//added by Mike, 2021110
//Reference: https://stackoverflow.com/questions/55786004/play-one-sound-after-another-with-sdl-mixer;
//last accessed: 2021110
//answer by:  Nikos C., 20190421T1946
void playAndWait(int channel, Mix_Chunk* chunk, int loops)
{
    channel = Mix_PlayChannel(channel, chunk, loops);
    if (channel < 0) {
        return; // error
    }
    while (Mix_Playing(channel) != 0) {
        SDL_Delay(200);
    }
}

//added by Mike, 2021110
//OK; TO-DO: -add: free used memory 
void playAndWaitMusic(Mix_Music* music, int loops)
{
    int output = Mix_PlayMusic(music, loops);
    if (output < 0) {
        return; // error
    }
    while (Mix_Playing(output) != 0) {
        SDL_Delay(200);
    }
}


//added by Mike, 2021110
int executeSDLWaveSoundOKWithWAVFile(int argc, char **argv) {
    int frequencia = 22050;
    Uint16 formato = AUDIO_S16SYS;
    int canal = 2; // 1 mono; 2 = stereo;
    int buffer = 4096;
    Mix_OpenAudio(frequencia, formato, canal, buffer);

    Mix_Chunk* sound_1;
/*    
    Mix_Chunk* sound_2;
    Mix_Chunk* sound_3;
    Mix_Chunk* sound_4;
*/
    sound_1 = Mix_LoadWAV(argv[1]);
/*
    som_2 = Mix_LoadWAV("D:\\sound1.wav");
    som_3 = Mix_LoadWAV("D:\\sound1.wav");
    som_4 = Mix_LoadWAV("D:\\sound1.wav");
*/

/*
    Mix_PlayChannel(-1, sound_1, 0);
    Mix_PlayChannel(1, sound_2, 0);
    Mix_PlayChannel(2, sound_3, 0);
    Mix_PlayChannel(3, sound_4, 0);
*/

	playAndWait(-1, sound_1, 0);

    return 0;
}

//added by Mike, 2021110
int executeSDLWaveSound(int argc, char **argv) {
    int frequencia = 22050;
    Uint16 formato = AUDIO_S16SYS;
    int canal = 2; // 1 mono; 2 = stereo;
    int buffer = 4096;
    Mix_OpenAudio(frequencia, formato, canal, buffer);
	
	//added by Mike, 2021110
	int volume=SDL_MIX_MAXVOLUME;
	Mix_VolumeMusic(volume);

	//removed by Mike, 20211110
//    Mix_Music* sound_1;

/*    
    Mix_Chunk* sound_2;
    Mix_Chunk* sound_3;
    Mix_Chunk* sound_4;
*/
	
    sound_1 = Mix_LoadMUS(argv[1]);
/*
    som_2 = Mix_LoadWAV("D:\\sound1.wav");
    som_3 = Mix_LoadWAV("D:\\sound1.wav");
    som_4 = Mix_LoadWAV("D:\\sound1.wav");
*/

/*
    Mix_PlayChannel(-1, sound_1, 0);
    Mix_PlayChannel(1, sound_2, 0);
    Mix_PlayChannel(2, sound_3, 0);
    Mix_PlayChannel(3, sound_4, 0);
*/

	playAndWaitMusic(sound_1, 0);

    return 0;
}

//added by Mike, 20211110
void freeAndCloseSDLWaveSound() {
	// free & close
	Mix_FreeMusic(sound_1);
	Mix_CloseAudio();
}

//added by Mike, 20211107
int executeSDLWaveSoundPrev(int argc, char **argv)
{
	int audio_rate,audio_channels;
	Uint16 audio_format;
	Uint32 t;
	Mix_Music *music;
	int volume=SDL_MIX_MAXVOLUME;

	// initialize SDL for audio and video
	if(SDL_Init(SDL_INIT_AUDIO|SDL_INIT_VIDEO)<0)
		cleanExit((char*) "SDL_Init"); //edited by Mike, 20211122
	atexit(SDL_Quit);
	
	int initted=Mix_Init(0);
	printf("Before Mix_Init SDL_mixer supported: ");
	print_init_flags(initted);
	initted=Mix_Init(~0);
	printf("After  Mix_Init SDL_mixer supported: ");
	print_init_flags(initted);
	Mix_Quit();

	if(argc<2 || argc>4)
	{
		fprintf(stderr,"Usage: %s filename [depth] [any 3rd argument...]\n"
				"    filename is any music file supported by your SDL_mixer library\n"
				"    depth is screen depth, default is 8bits\n"
				"    if there is a third argument given, we go fullscreen for maximum fun!\n",
				*argv);
		return 1;
	}

/*	//removed by Mike, 20211107
	// open a screen for the wav output
	if(!(s=SDL_SetVideoMode(W,H,(argc>2?atoi(argv[2]):8),(argc>3?SDL_FULLSCREEN:0)|SDL_HWSURFACE|SDL_DOUBLEBUF)))
		cleanExit("SDL_SetVideoMode");
	SDL_WM_SetCaption("sdlwav - SDL_mixer demo","sdlwav");
*/

/* //removed by Mike, 20211107	
	// hide the annoying mouse pointer
	SDL_ShowCursor(SDL_DISABLE);
	// get the colors we use
	white=SDL_MapRGB(s->format,0xff,0xff,0xff);
	black=SDL_MapRGB(s->format,0,0,0);
*/
	
	//added by Mike, 20211110
	//reference: https://stackoverflow.com/questions/44186167/sdl-2-on-windows-works-incorrectly-with-audio-device;
	//last accessed: 20211005
	//answer by: Bartlomiej Lewandowski, 20170525T1951
	//TO-DO: -reverify: with non-Windows, e.g. Linux, machine
	putenv((char*) "SDL_AUDIODRIVER=DirectSound"); //edited by Mike, 20211122
	
	// initialize sdl mixer, open up the audio device
	if(Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT,2,BUFFER)<0)
		cleanExit((char*)"Mix_OpenAudio"); //edited by Mike, 20211122

	// we play no samples, so deallocate the default 8 channels...
	Mix_AllocateChannels(0);
	
	// print out some info on the formats this run of SDL_mixer supports
	{
		int i,n=Mix_GetNumChunkDecoders();
		printf("There are %d available chunk(sample) decoders:\n",n);
		for(i=0; i<n; ++i)
			printf("	%s\n", Mix_GetChunkDecoder(i));
		n = Mix_GetNumMusicDecoders();
		printf("There are %d available music decoders:\n",n);
		for(i=0; i<n; ++i)
			printf("	%s\n", Mix_GetMusicDecoder(i));
	}

	// print out some info on the audio device and stream
	Mix_QuerySpec(&audio_rate, &audio_format, &audio_channels);
	bits=audio_format&0xFF;
	sample_size=bits/8+audio_channels;
	rate=audio_rate;
	printf("Opened audio at %d Hz %d bit %s, %d bytes audio buffer\n", audio_rate,
			bits, audio_channels>1?"stereo":"mono", BUFFER );

/*	//removed by Mike, 20211107
	// calculate some parameters for the wav display
	dy=s->h/2.0/(float)(0x1<<bits);
*/
	
	// load the song
	if(!(music=Mix_LoadMUS(argv[1])))
		cleanExit((char*) "Mix_LoadMUS(\"%s\")",argv[1]);  //edited by Mike, 20211122

	{
		Mix_MusicType type=Mix_GetMusicType(music);
		printf("Music type: %s\n",
				type==MUS_NONE?"MUS_NONE":
				type==MUS_CMD?"MUS_CMD":
				type==MUS_WAV?"MUS_WAV":
				//type==MUS_MOD_MODPLUG?"MUS_MOD_MODPLUG":
				type==MUS_MOD?"MUS_MOD":
				type==MUS_MID?"MUS_MID":
				type==MUS_OGG?"MUS_OGG":
				type==MUS_MP3?"MUS_MP3":
//				type==MUS_MP3_MAD?"MUS_MP3_MAD":
				type==MUS_FLAC?"MUS_FLAC":
				"Unknown");
	}
	// set the post mix processor up
	Mix_SetPostMix(postmix,argv[1]);

/* //removed by Mike, 20211107	
	SDL_FillRect(s,NULL,black);
	SDL_Flip(s);
	SDL_FillRect(s,NULL,black);
	SDL_Flip(s);
*/

		printf(">>>>> 1\n");

	// start playing and displaying the wav
	// wait for escape key of the quit event to finish
	t=SDL_GetTicks();
	//note: 1 = play x1; -1 = play as loop
//	if(Mix_PlayMusic(music, 1)==-1)
	if(Mix_PlayMusic(music, -1)==-1) {
		cleanExit((char*) "Mix_PlayMusic(0x%p,1)",music); //edited by Mike, 20211122
	}
	Mix_VolumeMusic(volume);

		printf(">>>>> 2\n");
		
	if(Mix_PlayMusic(music, -1)<0){
		printf("Mix_PlayMusic: %s\n", Mix_GetError());
	    printf("error playing music \n");
	}		

/*				
	//while((Mix_PlayingMusic() || Mix_PausedMusic()) && !done)
	while(Mix_PlayingMusic())	
	{
		printf(">>>>> 2.5\n");
	}
*/

	//TO-DO: -reverify: cause to close audio, et cetera 
	//immediately when the music finishes; otherwise, error
	//note: no audible sound output yet
	
	int iCount=0;
//	while (Mix_PlayingMusic() ){
	while (iCount<27){
//	    printf("playing music \n");
//   	SDL_Delay(100);
    	
		// check if music is playing
		printf("%i: music is%s playing.\n", iCount, Mix_PlayingMusic()?"":" not");
		iCount++;    	
	}

	t=SDL_GetTicks()-t;

		printf(">>>>> 3\n");

	// free & close
	Mix_FreeMusic(music);
	Mix_CloseAudio();
	
	return(0);
}



