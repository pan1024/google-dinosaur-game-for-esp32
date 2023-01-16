#include<little_fs/little_fs.h>
bool createDir(fs::FS &fs, const char * path){   
    if(fs.mkdir(path)){
      return true;
    } else {
      return false;
    }
}

bool removeDir(fs::FS &fs, const char * path){
    if(fs.rmdir(path)){
         return true;
    } else {
        return false;
    }
}

String  readFile(fs::FS &fs, const char * path){

    File file = fs.open(path);
    if(!file || file.isDirectory()){
        return "failed";
    }
    String result="";
    while(file.available()){
        result+=(char)file.read();
    }
    return result;
    file.close();
}

bool writeFile(fs::FS &fs, const char * path, const char * message){
    File file = fs.open(path, FILE_WRITE);
    if(!file){
        return false;
    }
    if(file.print(message)){
        return true;
    } else {
        return false;
    }
    file.close();
}

bool appendFile(fs::FS &fs, const char * path, const char * message){
    File file = fs.open(path, FILE_APPEND);
    if(!file){
       return false;
    }
    if(file.print(message)){
       return true;
    } else {
        return false;
    }
    file.close();
}

bool renameFile(fs::FS &fs, const char * path1, const char * path2){
    if (fs.rename(path1, path2)) {
        return true;
    } else {
        return false;
    }
}

bool deleteFile(fs::FS &fs, const char * path){
    if(fs.remove(path)){
      return true;
    } else {
      return false;
    }
}