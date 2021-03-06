/*
    date: 2020-08
    compile: g++ fetch.cpp -std=c++17 `sdl2-config --libs` -lSDL2_mixer `taglib-config --libs` `pkg-config --cflags --libs taglib` -lz
*/
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <unistd.h>
#include <taglib/tag.h>
#include <taglib/fileref.h>
#include "json.hpp"
#include <filesystem>
#include <string>

using json = nlohmann::json;
namespace fs = std::filesystem;

// return a vector of strings of all mp3, flac and ogg files in directory and subdirectories.
std::vector<std::string> getFilesInDirectory(std::string path) {
    std::vector<std::string> filenames;

    for(const auto & entry : fs::directory_iterator(path)) {
        //std::cout << entry.path() << ", directory: " << entry.is_directory() << std::endl;
        if(entry.is_directory()) {
            std::vector<std::string> filenamesTemp = getFilesInDirectory(entry.path());
            filenames.insert(filenames.end(), filenamesTemp.begin(), filenamesTemp.end());
        } else {
            if((entry.path().extension() == ".mp3") ||
            (entry.path().extension() == ".ogg") ||
            (entry.path().extension() == ".flac")) {
                filenames.push_back(entry.path());
            }
        }
    }
    return filenames;
}

// create a json parsed string that contain a list of filenames.
json createJsonDatabase(std::string path) {
    json j;
    
    std::vector<std::string> filenames = getFilesInDirectory(path);

    // print vector of strings
    /* printf("\n      -- ALL FILES --\n");
    for(int i = 0; i < filenames.size(); i++){ 
        printf("%s\n", filenames[i].c_str());
    } */

    TagLib::String fTitle;
    TagLib::String fArtist;
    TagLib::String fAlbum;
    TagLib::String fGenre;
    unsigned int fYear;

    for(int i = 0; i < filenames.size(); i++){

        //get music infos
        TagLib::FileRef f(filenames[i].c_str());
        fTitle = f.tag()->title();
        fArtist = f.tag()->artist();
        fAlbum = f.tag()->album();
        fGenre = f.tag()->genre();
        fYear = f.tag()->year();

        j["musics"][i]["file"] = filenames[i].c_str();
        j["musics"][i]["title"] = fTitle.toCString(true);
        j["musics"][i]["artist"] = fArtist.toCString(true);
        j["musics"][i]["album"] = fAlbum.toCString(true);
        j["musics"][i]["genre"] = fGenre.toCString(true);
        j["musics"][i]["year"] = fYear;
    }


    return j;
}

int main() {
    json j = createJsonDatabase("musics/");
    std::string s = j.dump(4);

    printf("%s\n", s.c_str());

    FILE *pFile = fopen ("db.json","w");
    // fprintf(pFile, "%s\n", s.c_str());
    printf("Database created.\n");

    /* while(getchar() == 0); */
    return 0;
}
