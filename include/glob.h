#include <iostream>
#include <glob.h> // glob(), globfree()
#include <string.h> // memset()
#include <vector>

std::string globError;
int globReturn;

std::vector<std::string> ERRORglob(std::string error,glob_t glob_result){
    globfree(&glob_result);
    globError = "glob() failed with globReturn value " + std::to_string(globReturn) + error;
    return {};
}

std::vector<std::string> glob(const std::string& pattern) {
    // glob struct resides on the stack
    glob_t glob_result;
    memset(&glob_result, 0, sizeof(glob_result));
    // do the glob operation
    globReturn = glob(pattern.c_str(), GLOB_TILDE, NULL, &glob_result);
    switch (globReturn){
        case 1: return ERRORglob("; Glob ran out of memory",glob_result); break;
        case 2: return ERRORglob("; Glob had an error while reading the folder",glob_result); break;
        case 3: return ERRORglob("; Glob found no matches",glob_result); break;
        default: globReturn=0; break;
    }
    // collect all the filenames into a std::list<std::string>
    std::vector<std::string> filenames;
    for(size_t i = 0; i < glob_result.gl_pathc; ++i) {
        filenames.push_back(std::string(glob_result.gl_pathv[i]));
    }

    // cleanup
    globfree(&glob_result);

    // done
    return filenames;
}