#include "../include/RSGL/RSGL.hpp"
#include <iostream>
#include <vector>
#include "../include/glob.h"
#include <sys/syscall.h>

bool running = true;
std::vector<RSGL::music> songs;
std::vector<int> played;

std::vector<RSGL::music> loadFolder(std::string dir){
    std::vector<std::string> files = glob(dir+"*mp3");
    std::string sim;  int dircode=0;
    for (int i=0; !dircode; i++){
        sim += "*/"; glob(dir+sim);  dircode = globReturn;
        if (!dircode){ 
            std::vector<std::string> g = glob(dir+sim+"*mp3"); 
            if (g.size()) files.insert(files.end(),g.begin(),g.end());
        }
    }

    std::vector<RSGL::music> output;
    for (int i=0; i < files.size(); i++) output.insert(output.end(), RSGL::loadMusic(files.at(i))); 
    return output;  
}

int main(){
    RSGL::init();
    songs = loadFolder("/home/manjaro/Music/.music/");
    RSGLWindow window = createWindow("RSGL Music Player",500,500,300,300, {255,255,255});    
    Event e;

    int I=0;  int pause = 0;
    int song = 0; int suffle=0;
    int dir = 1; int start=1;

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
            if (!start){I+=dir;} RSGL::playMusic(songs[I],1); if(dir) dir=1; start=0;
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
            if (pause){RSGL::resumeMusic(); pause=0; Pause.img = RSGL::loadImage("../res/play.png",{100,200,60,60});}
            if (!pause){RSGL::pauseMusic(); pause=1; Pause.img = RSGL::loadImage("../res/pause.png",{100,200,60,60});}
        }
        if (Skip.isClicked()){ RSGL::stopMusic(); dir=1;}
        if (Back.isClicked()){RSGL::stopMusic(); dir=-1;}
        if (Shuffle.isClicked()){ if(suffle) suffle=0; else suffle=1; }
        if (Loop.isClicked()){ if(dir) dir=0; else dir=1; }
        
    }
    RSGL::Quit();
}
