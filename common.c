#include "common.h"

int doPadding(char* string){
    return (WIDTH - strlen(string)) / 2;
}

void trimNewLine(char* line){
    line[strcspn(line, "\n")] = '\0';
}

int isBlank(const char *s){
    while (*s == ' ' || *s == '\t' || *s == '\n' || *s == '\r') s++;
    return *s == '\0';
}

void aboutProject(){
    FILE* fp = fopen(ABOUT_PROJECT_FILE_PATH, "r");
    if(!fp){
        printf("Error opening the aboutProject.txt file\n");
        return;
    }
    char line[MAXBUFFERSIZE];
    printf("--------------------------------------------------\n");
    while(fgets(line, sizeof(line), fp) != NULL){
        printf("%s", line);
    }
    fclose(fp);
    printf("\n--------------------------------------------------\n");
    return;
}