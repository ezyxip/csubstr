#include <stdio.h>
#include <stdlib.h>
#define INPUT_FILE "../input.txt"
#define  OUTPUT_FILE "../output.txt"

char* readFile(char* name);
unsigned runToStr(const char* str, int startIndex, int steps);
unsigned isMatch(char* str, int startIndex, char* pattern);
char* normalize(char* str);
char* denormalize(char* pattern, char* str);
void writeTextToFile(const char *filename, const char *text);

int main() {
    char* str = readFile(INPUT_FILE);
    str = normalize(str);

    int patternSize = 1;
    for(int i = 0; str[i]; i++){
        if(str[i] == '\n') break;
        patternSize++;
    }
    char pattern[patternSize];
    for(int i = 0; str[i]; i++){
        if(str[i] == '\n') break;
        pattern[i] = str[i];
    }
    pattern[patternSize-1] = '\0';
    patternSize--;

    int strSize = 1;
    int index1 = patternSize;
    while(str[index1++]) strSize++;

    int countOfEntry = 0;
    for(int i = patternSize; str[i] && runToStr(str, i, patternSize - 1); i++){
        if(isMatch(str, i, pattern)) countOfEntry++;
    }

    char* newStr = calloc(strSize + countOfEntry, 1);
    newStr[strSize + countOfEntry - 1] = '\0';

    for(int i = patternSize, nsi = 0; str[i]; i++, nsi++){
        if(runToStr(str, i, patternSize - 1) && isMatch(str, i, pattern)){
            countOfEntry++;
            newStr[nsi] = '@';
            nsi++;
        }
        newStr[nsi] = str[i];
    }
    char* newFile = calloc(patternSize + strSize + countOfEntry - 1, 1);
    int index2 = 0;
    for(int i = 0; pattern[i]; i++){
        newFile[index2++] = pattern[i];
    }
    for(int i = 0; newStr[i]; i++){
        newFile[index2++] = newStr[i];
    }
    newFile = denormalize(readFile(INPUT_FILE), newFile);
    writeTextToFile(OUTPUT_FILE, newFile);
    free(newStr);
    free(str);
    return 0;

}

unsigned isDelimiter(char a){
    if((a == ' ') || (a == '\n') || (a == '\t') || (a == '\r')) return 1;
    return 0;
}

char* denormalize(char* pattern, char* str){
    int size = 1;
    int flag = 0;
    for (int i = 0; pattern[i]; i++){
        if(isDelimiter(pattern[i]) && flag){
            size++;
        } else if(isDelimiter(pattern[i]) && !flag){
            flag = 1;
        } else {
            flag = 0;
        }
    }
    for (int i = 0; str[i]; i++) size++;
    char* res = calloc(size, 1);
    int index = 0;
    int patternDelimiterIndex = 0;
    for(int i = 0; str[i]; i++){
        if(isDelimiter(str[i])){
            for( ;pattern[patternDelimiterIndex]; patternDelimiterIndex++){
                if(isDelimiter(pattern[patternDelimiterIndex])) break;
            }
            for(; isDelimiter(pattern[patternDelimiterIndex]); patternDelimiterIndex++) res[index++] = pattern[patternDelimiterIndex];
        } else {
            res[index++] = str[i];
        }
    }

    return res;
}

char* normalize(char* str){
    int resultSize = 0;
    int flag = 0;
    for(int i = 0; str[i]; i++){
        if(isDelimiter(str[i]) && flag) {
            continue;
        }
        else if (isDelimiter(str[i]) && !flag){
            flag = 1;
            resultSize++;
        } else {
            resultSize++;
            flag = 0;
        }
    }

    char* res = calloc(resultSize + 1, 1);
    int index = 0;
    flag = 0;
    for(int i = 0; str[i]; i++){
        if(isDelimiter(str[i]) && flag)
            continue;
        else if (isDelimiter(str[i]) && !flag) {
            flag = 1;
            res[index++] = str[i];
        } else {
            res[index++] = str[i];
            flag = 0;
        }
    }
    return res;
}

unsigned delimitersEqual(char a, char b){
    if(isDelimiter(a) && isDelimiter(b))
        return 1;
    else
        return 0;
}

unsigned isMatch(char* str, int startIndex, char* pattern){
    int i = startIndex;
    for(int ip = 0; pattern[ip]; i++, ip++){
        if(delimitersEqual(str[i], pattern[ip])) continue;
        if(str[i] != pattern[ip]) return 0;
    }
    if(!str[i] || isDelimiter(str[i]))
        return 1;
    return 0;
}



unsigned runToStr(const char* str, int startIndex, int steps){
    for(int i = startIndex; i < startIndex + steps; i++){
        if(!str[i]) return 0;
    }
    return 1;
}

char* readFile(char* name){
    FILE* file = fopen(name, "r");

    if (file == NULL) {
        perror("No such file");
    }

    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);
    char c;
    while ((c = fgetc(file)) != EOF) {
        if(c == '\n') fileSize--;
    }
    fseek(file, 0, SEEK_SET);

    char* buffer = (char*)malloc(fileSize + 1);
    if (buffer == NULL) {
        perror("Ошибка выделения памяти");
    }

    fread(buffer, fileSize, 1, file);
    buffer[fileSize] = '\0';

    fclose(file);
    return buffer;
}

void writeTextToFile(const char *filename, const char *text) {
    FILE *file = fopen(filename, "w"); // открываем файл для записи
    if (file) {
        fputs(text, file); // записываем текст в файл
        fclose(file); // закрываем файл
    } else {
        printf("Не удалось открыть файл %s для записи\n", filename);
    }
}