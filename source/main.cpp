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
    int song = 0; int suffle=0; int skip=0;
    int dir = 1; int start=1; int load=0;

    RSGL::imgButton Pause = {RSGL::loadImage("../res/pause.png",{100,200,60,60})}; 
    RSGL::imgButton Skip = {RSGL::loadImage("../res/skip.png",{160,200,60,60})}; 
    RSGL::imgButton Back = {RSGL::loadImage("../res/back.png",{40,200,60,60})}; 
    RSGL::imgButton Shuffle = {RSGL::loadImage("../res/shuffle.png",{140,170,30,30})}; 
    RSGL::imgButton Loop = {RSGL::loadImage("../res/loop.png",{100,170,30,30})};
    RSGL::imgButton Load = {RSGL::loadImage("../res/load.png",{0,0,50,50})};

    RSGL::imgButton File = {RSGL::loadImage("../res/file.png",{0,30,100,100})};
    RSGL::imgButton Folder = {RSGL::loadImage("../res/folder.png",{0,50,100,100})};
    while (running){
        srand(time(NULL));
        e.CheckEvents();
        RSGL::clear();

        Pause.draw();
        Skip.draw();
        Back.draw();
        Shuffle.draw();
        Loop.draw();
        Load.draw();
        if(load){
            File.draw();
            Folder.draw();
        }
        switch (e.type){
            case RSGL::quit: running = false; break;
            case RSGL::KeyPressed: /*mysound->play();*/ break;
            default: break;
        }
        if (I < songs.size() && !RSGL::isPlaying(songs[song])){
            if (!start){I+=dir;} RSGL::playMusic(songs[song],1); bool New=true;
            RSGL::notifiy("Playing "+songs[song].title,"New song is playing with RSGL Music Player\n"+songs[song].title);
            for(int i=0; played.size(); i++){if (i == song) New = false; break;}
            if(New){played.insert(played.end(),song); skip=0;} 
            if(dir) dir=1; start=0;
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
        if (Back.isClicked()){RSGL::stopMusic(); dir=-1;if(suffle && played.size()){song=played.at(played.size()-2-skip);skip+=2;}}
        if (Shuffle.isClicked()){ if(suffle) suffle=0; else suffle=1; }
        if (Loop.isClicked()){ if(dir) dir=0; else dir=1; }
        if (Load.isClicked()){
            if(load) load=0;
            else load=1;
        }
        if (File.isClicked() && load){
            std::vector<std::string> files = RSGL::fileDialog("Load a mp3 file",true,false,false);
            for (int i=0; i < files.size(); i++){
                std::string fn = files.at(i);
                songs.insert(songs.end(),RSGL::loadMusic(fn));
            }
        }

        if (Folder.isClicked() && load){
            std::vector<std::string> files = RSGL::fileDialog("Load a folder",true,false,false);
            for (int i=0; i < files.size(); i++){
                std::string fn = files.at(i);
                std::vector<RSGL::music> mus = loadFolder(fn);
                songs.insert(songs.end(),mus.begin(),mus.end());
            }            
        }       
        
    }
    RSGL::Quit();
}
