#include "types.h"

extern struct NODE* root;
extern struct NODE* cwd;

//make directory
void mkdir(char pathName[]){
    if(strcmp(pathName, "/") == 0){
        printf("MKDIR ERROR: no path provided\n");
        return;
    }
    char baseName[64] = "";
    char dirName[128] = "";

    struct NODE* parent = splitPath(pathName, baseName, dirName);

    if(parent == NULL){
        return;
    }

    if(strlen(baseName) == 0){
        printf("MKDIR ERROR: no path provided\n");
        return;
    }
    struct NODE* current = parent->childPtr;
    while(current != NULL){
        if(strcmp(current->name, baseName) == 0){
            printf("MKDIR ERROR: directory %s already exists\n", pathName);
            return;
        }
        current = current->siblingPtr;
    }
    struct NODE* newNode = (struct NODE*)malloc(sizeof(struct NODE));
    if(newNode == NULL){
        return;
    }

    strcpy(newNode->name, baseName);
    newNode->fileType = 'D';
    newNode->childPtr = NULL;
    newNode->siblingPtr = NULL;
    newNode->parentPtr = parent;

    if(parent->childPtr == NULL){
        parent->childPtr = newNode;
    }
    else{
        current = parent->childPtr;
        while(current->siblingPtr != NULL){
            current = current->siblingPtr;
        }
        current->siblingPtr = newNode;
    }




    printf("MKDIR SUCCESS: node %s successfully created\n", pathName);
    return;
}

//handles tokenizing and absolute/relative pathing options
struct NODE* splitPath(char* pathName, char* baseName, char* dirName){
    if(pathName == NULL || baseName == NULL || dirName == NULL){
        return NULL;
    }

    baseName[0] = '\0';
    dirName[0] = '\0';

    if(strcmp(pathName, "/") == 0){
        strcpy(dirName, "/");
        baseName[0] = '\0';
        return root;
    }

    char temp[128];
    strncpy(temp, pathName, sizeof(temp) - 1);
    temp[sizeof(temp) - 1] = '\0';

    char *lastSlash = strrchr(temp, '/');

    if(lastSlash != NULL){
        if(lastSlash == temp){
            strcpy(dirName, "/");
            strcpy(baseName, lastSlash + 1);
        }
        else{
            *lastSlash = '\0';
            strncpy(dirName, temp, 127);
            dirName[127] = '\0';
            strcpy(baseName, lastSlash + 1);
        }
    }
    else{
        baseName[0] = '\0';
        strcpy(baseName, temp);
        dirName[0] = '\0';
    }

    struct NODE* current;
    if(pathName[0] == '/'){
        current = root;
    }
    else{  
        current = cwd; 
    }

    
    if(strlen(dirName) == 0){
        return current;
    }
    if(strcmp(dirName, "/") == 0){
        return root;
    }


    char dirCopy[128];
    strncpy(dirCopy, dirName, sizeof(dirCopy) - 1);
    dirCopy[sizeof(dirCopy) - 1] = '\0';
    char *token = strtok(dirCopy, "/");
    while(token != NULL){
        struct NODE* child = current->childPtr;
        int found = 0;
        while(child != NULL){
            if(child->fileType == 'D' && strcmp(child->name, token) == 0){
                current = child;
                found = 1;
                break;
            }
            child = child->siblingPtr;
        }
        if(!found){
            printf("ERROR: directory %s does not exist\n", token);
            return NULL;
        }

        token = strtok(NULL, "/");
    }


    return current;
}
