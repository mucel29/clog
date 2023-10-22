/*

    CLOG - C logging library - Cosmin Asavoae (mucel29) @2020

*/


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdint.h>
#include <time.h>

#ifndef LOG_H
#define LOG_H

#define nullptr 0x0

typedef enum 
{
    LV_INFO,
    LV_WARN,
    LV_ERROR,
    LV_SEVERE,
    LV_DEBUG
} LogLevel;

typedef struct
{
    char* name;
    bool displayTime;
    LogLevel level;
} Logger;

extern const char* mappings[];
extern Logger* lg;

//Text prefixes
//Maybe make the bold and underscore separate(with 0), they seem to stack

#define TX_REGULAR      "\e[0;"
#define TX_UNDERSCORE   "\e[4;"
#define TX_BOLD         "\e[1;"
#define TX_UNKNOWN      ""

//Reset

#define TX_RESET        "\e[0;0m"

//Text colors

#define TX_BLACK        "30m"
#define TX_RED          "31m"
#define TX_GREEN        "32m"
#define TX_YELLOW       "33m"
#define TX_BLUE         "34m"
#define TX_MAGENTA      "35m"
#define TX_CYAN         "36m"
#define TX_WHITE        "37m"

//High intensity text colors

#define HTX_BLACK        "90m"
#define HTX_RED          "91m"
#define HTX_GREEN        "92m"
#define HTX_YELLOW       "93m"
#define HTX_BLUE         "94m"
#define HTX_MAGENTA      "95m"
#define HTX_CYAN         "96m"
#define HTX_WHITE        "97m"

//Background colors

#define BG_BLACK    "\e[40m"
#define BG_RED      "\e[41m"
#define BG_GREEN    "\e[42m"
#define BG_YELLOW   "\e[43m"
#define BG_BLUE     "\e[44m"
#define BG_MAGENTA  "\e[45m"
#define BG_CYAN     "\e[46m"
#define BG_WHITE    "\e[47m"

//High intensity background

#define HBG_BLACK   "\e[0;100m"
#define HBG_RED     "\e[0;101m"
#define HBG_GREEN   "\e[0;102m"
#define HBG_YELLOW  "\e[0;103m"
#define HBG_BLUE    "\e[0;104m"
#define HBG_MAGENTA "\e[0;105m"
#define HBG_CYAN    "\e[0;106m"
#define HBG_WHITE   "\e[0;107m"

//Minecraft colors

#define DARK_RED    TX_RESET    TX_REGULAR TX_RED           //4
#define RED         TX_RESET    TX_REGULAR HTX_RED          //c
#define GOLD        TX_RESET    TX_REGULAR TX_YELLOW        //6
#define YELLOW      TX_RESET    TX_REGULAR HTX_YELLOW       //e
#define DARK_GREEN  TX_RESET    TX_REGULAR TX_GREEN         //2
#define GREEN       TX_RESET    TX_REGULAR HTX_GREEN        //a
#define AQUA        TX_RESET    TX_REGULAR HTX_CYAN         //b
#define DARK_AQUA   TX_RESET    TX_REGULAR TX_CYAN          //3
#define DARK_BLUE   TX_RESET    TX_REGULAR TX_BLUE          //1
#define BLUE        TX_RESET    TX_REGULAR HTX_BLUE         //9
#define DARK_PURPLE TX_RESET    TX_REGULAR TX_MAGENTA       //5
#define L_PURPLE    TX_RESET    TX_REGULAR HTX_MAGENTA      //d
#define LIGHT_GRAY  TX_RESET    TX_REGULAR HTX_WHITE        //7
#define WHITE       TX_RESET    TX_REGULAR TX_WHITE         //f
#define BLACK       TX_RESET    TX_REGULAR TX_BLACK         //0
#define DARK_GRAY   TX_RESET    TX_REGULAR HTX_BLACK        //8
#define RESET       TX_RESET                                //r
#define UNDERSCORE  TX_UNDERSCORE "0"                       //n

#define MESSAGE_BUFFER 256
#define PREFIX_BUFFER 64
/*

    Should I make a separate helper header with functions like stripcolor / replacecodes ?
    Should I use a single log and pass the desired level or make separate functions?
    Man this idiomatic C is so confusing...

*/

//Helpers

void replaceColorCodes(char* str);

void stripColorCodes(char* str);

//Initialization

Logger* loggerCreate(const char* name, bool displayTime, LogLevel level);

//Deletion

void loggerDelete(Logger* logger);

//Levels
void loggerSetLevel(Logger* logger, LogLevel level); //is this even necessary if the struct is public?

//Output

//Note that the colors in the VA won't be applied
void loggerLog(Logger* logger, LogLevel level, const char* str, ...);

//Per-level output
void loggerInfo(Logger* logger, const char* str, ...);

void loggerWarn(Logger* logger, const char* str, ...);

void loggerErr(Logger* logger, const char* str, ...);

void loggerSevere(Logger* logger, const char* str, ...);

void loggerDebug(Logger* logger, const char* str, ...);

//Input
//Note: you can't insert VA in the question
void loggerScan(Logger* logger, const char* question, const char* format, ...); //NOTE: level is Info

#endif //LOG_H