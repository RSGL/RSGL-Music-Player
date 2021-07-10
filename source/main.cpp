#include "../include/RSGL/RSGL.hpp"
#include <vector>
#include <iostream>
#include <sys/syscall.h>

bool running = true;
std::vector<RSGL::music> songs;
std::vector<int> played;

std::vector<RSGL::music> loadFolder(std::string dir){
    std::vector<RSGL::music> output;
    std::string c = "find " + dir + " -name \"*.mp3\"";
    FILE* f = popen(c.data(),"r"); std::string read; char* fs; char ch; int len;
    if (f){
        while(fgetc(f) != EOF) len++; fs = (char*) malloc(len);
        int i =0; 
        while( ch=fgetc(f) != EOF){ fs[i] = ch; i++; }
        std::cout << fs;
        fclose (f);
    
        if (fs){
            std::string file;
            for (int i=0; i < len; i++){
                if (fs[i] != '\n') file+= fs[i];
                else{output.insert(output.end(), RSGL::loadMusic(file)); file="";}
            }
            return output; 
        } 
    }
    return {};
}

int main(){
    RSGL::init();
    RSGLWindow window = createWindow("RSGL Music Player",500,500,300,300, {255,255,255});    
    Event e;

    int I=0;  int pause = 0;
    int song = 0; int suffle=0;
    int dir = 1;
    /*cAudio::IAudioManager* audioMgr = cAudio::createAudioManager(false);
    cAudio::IAudioSource* mysound = audioMgr->create("song", "/home/manjaro/Music/.music/(Probably) Racist/22. 4 My Nuckas.mp3",true);*/
    //songs = {RSGL::loadMusic("/home/manjaro/Music/.music/(Probably) Racist/22. 4 My Nuckas.mp3"),RSGL::loadMusic("/home/manjaro/Music/.music/(Probably) Racist/4. My Shit.mp3"),RSGL::loadMusic("/home/manjaro/Music/.music/Black Man Of Steal/28. Fly Like Me.mp3")};
    songs = loadFolder("/home/manjaro/Music/.music/");

    RSGL::imgButton Pause = {RSGL::loadImage("../res/pause.png",{100,200,60,60})}; 
    RSGL::imgButton Skip = {RSGL::loadImage("../res/skip.png",{160,200,60,60})}; 
    RSGL::imgButton Back = {RSGL::loadImage("../res/back.png",{40,200,60,60})}; 
    RSGL::imgButton Shuffle = {RSGL::loadImage("../res/shuffle.png",{140,170,30,30})}; 
    RSGL::imgButton Loop = {RSGL::loadImage("../res/loop.png",{100,170,30,30})};
    while (running){
        srand(time(NULL));
        e.CheckEvents();
        RSGL::clear();

        Pause.draw();
        Skip.draw();
        Back.draw();
        Shuffle.draw();
        Loop.draw();
        switch (e.type){
            case RSGL::quit: running = false; break;
            case RSGL::KeyPressed: /*mysound->play();*/ break;
            default: break;
        }

        if (I < songs.size() && !RSGL::isPlaying(songs[song])){
            RSGL::playMusic(songs[I],1);  I+=dir; if(dir) dir=1;
        } 
        song = I;
        if (suffle){ 
            bool New = false;
            while (New == false){
                song = rand() % (songs.size()-1); New = true;
                for(int i=0; played.size(); i++){
                    if (i == song) New = false; break;
                }
            }
        }

        if (Pause.isClicked()){
            if (pause){RSGL::resumeMusic(); pause=0; /*Pause.img = RSGL::loadImage("../res/play.png",{100,200,60,60});*/}
            if (!pause){RSGL::pauseMusic(); pause=1; /*Pause.img = RSGL::loadImage("../res/pause.png",{100,200,60,60});*/}
        }
        if (Skip.isClicked()){ RSGL::stopMusic(); dir=1;}
        if (Back.isClicked()){RSGL::stopMusic(); dir=-1;}
        if (Shuffle.isClicked()){ if(suffle) suffle=0; else suffle=1; }
        if (Loop.isClicked()){ if(dir) dir=0; else dir=1; }
        
    }
    RSGL::Quit();
}
