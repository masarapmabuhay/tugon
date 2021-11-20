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
 * @date updated: 20211120
 * @website address: http://www.usbong.ph
 *
 * Reference:
 * 1) https://www.libsdl.org/projects/SDL_mixer/
 * --> https://www.libsdl.org/projects/SDL_mixer/docs/index.html
 * --> https://www.libsdl.org/projects/SDL_mixer/docs/demos/
 * --> all last accessed: 20211107
 *
 * Acknowledgments:
 * 1) "Bulalakaw Wars" Team (2007): 
 * Syson, M., Camacho, R., Gonzales, D., Del Rosario, R., Vidal, E., et al.
 *
*/

/*
class Sdlwav {
      public:
       void sound_start();
       void sound_stop();
       void play_sound_clip(SoundClip *sound_clip_ptr);
       void stop_sound_clip(SoundClip *sound_clip_ptr);
       SoundClip *load_sound_clip(char *filename);
       void unload_sound_clip(SoundClip *sound_clip_ptr);
};
*/

//Mix_Music* sound_1;

void errorv(char *str, va_list ap);
void cleanExit(char *str,...);
static void postmix(void *udata, Uint8 *_stream, int _len);
void refresh();
void print_init_flags(int flags);
int executeSDLWaveSound(int argc, char **argv);
void freeAndCloseSDLWaveSound(); //added by Mike, 20211110

