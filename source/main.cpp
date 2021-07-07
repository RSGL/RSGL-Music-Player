#include "../include/RSGL/RSGL.hpp"
#include <vector>
#include <iostream>
bool running = true;
std::vector<RSGL::music> songs;
std::vector<int> played;

int main(){
    RSGL::init();
    RSGLWindow window = createWindow("name",500,500,500,500, {255,255,255});    
    Event e;

    int I=0;  int pause = 0;
    int song = 0; int suffle=0;
    int dir = 1; int loop = 1;
    std::string dirr = "~/music/.music/(Probably) Racist/";

    songs = {RSGL::loadMusic(dirr+"4. My Shit.mp3"),RSGL::loadMusic(dirr+"22. 4 My Nuckas.mp3"),RSGL::loadMusic(dirr+"17. Only 17.mp3")};
    
    RSGL::imgButton Pause = {RSGL::loadImage("../res/pause.png",{200,400,60,60})}; 
    RSGL::imgButton Skip = {RSGL::loadImage("../res/skip.png",{260,400,60,60})}; 
    RSGL::imgButton Back = {RSGL::loadImage("../res/back.png",{140,400,60,60})}; 
    RSGL::imgButton Shuffle = {RSGL::loadImage("../res/shuffle.png",{240,370,30,30})}; 
    RSGL::imgButton Loop = {RSGL::loadImage("../res/loop.png",{200,370,30,30})}; 

    while (running){
        RSGL::playMusic(songs[0],1);
        srand(time(NULL));
        e.CheckEvents();
        RSGL::clear();

        Pause.draw();
        Skip.draw();
        Back.draw();
        Shuffle.draw();
        Loop.draw();

        if (e.type == RSGL::quit) running = false;
        if (I < songs.size() && !RSGL::isPlaying(songs[song])){
            //I+=dir; RSGL::playMusic(songs[I],loop); dir=1;
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
            if (pause){RSGL::resumeMusic(); pause=0; Pause.img = RSGL::loadImage("../res/play.png",{200,400,60,60});}
            if (!pause){RSGL::pauseMusic(); pause=1; Pause.img = RSGL::loadImage("../res/pause.png",{200,400,60,60});}
        }
        if (Skip.isClicked()){ RSGL::stopMusic(); dir=1;}
        if (Back.isClicked()){RSGL::stopMusic(); dir=-1;}
        if (Shuffle.isClicked()){ if(suffle) suffle=0; else suffle=1; }
        if (Loop.isClicked()){ if(loop == 1) loop=-1; else loop=1; }
        
    }
    RSGL::Quit();
}