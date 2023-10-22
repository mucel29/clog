/*

    CLOG - C logging library - Cosmin Asavoae (mucel29) @2020

*/

#include "log.h"

const char* mappings[] =
{
    //full names

    [LV_INFO] = "&fINFO&p",
    [LV_WARN] = "&eWARN&p",
    [LV_ERROR] = "&cERROR&p",
    [LV_SEVERE] = "&4SEVERE&p",
    [LV_DEBUG] = "&6DEBUG&p",

    //prefixes

    [LV_INFO + LV_DEBUG + 1] = "&f",
    [LV_WARN + LV_DEBUG + 1] = "&e",
    [LV_ERROR + LV_DEBUG + 1] = "&c",
    [LV_SEVERE + LV_DEBUG + 1] = "&4",
    [LV_DEBUG + LV_DEBUG + 1] = "&6"
};

Logger* loggerCreate(const char* name, bool displayTime, LogLevel level)
{
    Logger* logger = (Logger*) malloc(sizeof(Logger));
    logger->name = (char*) malloc(sizeof(char) * strlen(name));
    strcpy(logger->name, name);
    logger->displayTime = displayTime;
    logger->level = level;

    return logger;
}

void loggerDelete(Logger* logger)
{
    free(logger->name);
    free(logger);
}

void loggerSetLevel(Logger* logger, LogLevel level)
{
    logger->level = level;
}

void loggerLog(Logger* logger, LogLevel level, const char* str, ...)
{
    if (level > logger->level)
        return;
    char* message = (char*) malloc(sizeof(char) * (PREFIX_BUFFER + MESSAGE_BUFFER + strlen(str)));
    memset(message, 0, sizeof(char) * (PREFIX_BUFFER + MESSAGE_BUFFER + strlen(str)));
    //Constructing prefix
    if(logger->displayTime) //display time
    {
        //Format time
        char frmt[9];
        const time_t t = time(NULL);
        struct tm* p = localtime(&t);
        strftime(frmt, 9, "%H:%M:%S", p);

        strcat(message, "[&e");
        strcat(message, frmt);
        strcat(message, "&p]");   
    }
    strcat(message, "[");
    strcat(message, logger->name);
    strcat(message, "/");
    strcat(message, mappings[level]);
    strcat(message, "] ");
    strcat(message, mappings[level + LV_DEBUG + 1]);
    strcat(message, str);

    replaceColorCodes(message);

    va_list vaptr;
    va_list vacpy;  //because if we use the other methods to call this one, we must pass char 127 to the va and the pointer to the other va, that way we can have 1 layer of indirection
    va_start(vaptr, str);
    va_copy(vacpy, vaptr);
    int check = va_arg(vacpy, int);
    if (check == 127)
    {
        va_list* pass = va_arg(vacpy, va_list*);
        vprintf(message, *pass);
    } else
        vprintf(message, vaptr);
    va_end(vaptr);
    printf(TX_RESET);
    free(message);

}

#define REPLACE_COLOR(dst, src)         \
{                                       \
    memset(dst, 0, 24);                 \
    memcpy(dst, src, strlen(src));      \
}

void replaceColorCodes(char* str)
{
    char* temp = (char*) malloc(sizeof(char) * (strlen(str) + MESSAGE_BUFFER));
    memset(temp, 0, strlen(str) + MESSAGE_BUFFER); //init to all 0

    char* p = strtok(str, "&");
    p = strtok(NULL, "&");

    char* start = str;
    char color[24];
    char last[24];
    char l2[24];

    REPLACE_COLOR(color, RESET);
    REPLACE_COLOR(last, RESET);
    REPLACE_COLOR(l2, RESET);

    strcat(temp, WHITE); //Because it's nice to have consistent colors i think
    strncat(temp, start, p - start); //copy from last to current token
    while (p != nullptr)
    {
        char buf[24];

        REPLACE_COLOR(buf, l2);     //Cache last 2 colors
        REPLACE_COLOR(l2, last);
        REPLACE_COLOR(last, color);

        bool code = true;
        switch (p[0])
        {
        case '4':
            REPLACE_COLOR(color, DARK_RED);
        break;
        case 'c':
            REPLACE_COLOR(color, RED);
        break;
        case '6':
            REPLACE_COLOR(color, GOLD);
        break;
        case 'e':
            REPLACE_COLOR(color, YELLOW);
        break;
        case '2':
            REPLACE_COLOR(color, DARK_GREEN);
        break;
        case 'a':
            REPLACE_COLOR(color, GREEN);
        break;
        case 'b':
            REPLACE_COLOR(color, AQUA);
        break;
        case '3':
            REPLACE_COLOR(color, DARK_AQUA);
        break;
        case '1':
            REPLACE_COLOR(color, DARK_BLUE);
        break;
        case '9':
            REPLACE_COLOR(color, BLUE);
        break;
        case '5':
            REPLACE_COLOR(color, DARK_PURPLE);
        break;
        case 'd':
            REPLACE_COLOR(color, L_PURPLE);
        break;
        case '7':
            REPLACE_COLOR(color, LIGHT_GRAY);
        break;
        case 'f':
            REPLACE_COLOR(color, WHITE);
        break;
        case '0':
            REPLACE_COLOR(color, BLACK);
        break;
        case '8':
            REPLACE_COLOR(color, DARK_GRAY);
        break;
        case 'r':
            REPLACE_COLOR(color, RESET);
        break;
        case 'n':
            REPLACE_COLOR(color, UNDERSCORE);
        break;
        case 'p':
        {
            REPLACE_COLOR(color, l2);
        }
        break;
        default:
            strcat(temp, "&");
            code = false;
            REPLACE_COLOR(last, l2); //Uncache last 2 colors cuz i am a stupid fuck and i can't write memory efficient code :(
            REPLACE_COLOR(l2, buf);
        break;
        }
        strcat(temp, color); //add last computed color
        start = p;
        start += code;
        p = strtok(NULL, "&");
        if (p != nullptr)
            strncat(temp, start, p - start); //copy from last to current token
    }
    //add last part of text
    strcat(temp, start); //copy from last to current token
    strcat(temp, TX_RESET);

    strcpy(str, temp);
    free(temp);
}

void loggerInfo(Logger* logger, const char* str, ...)
{
    va_list vaptr;
    va_start(vaptr, str);
    loggerLog(logger, LV_INFO, str, (char)127, &vaptr);
    va_end(vaptr);
}

void loggerWarn(Logger* logger, const char* str, ...)
{
    va_list vaptr;
    va_start(vaptr, str);
    loggerLog(logger, LV_WARN, str, (char)127, &vaptr);
    va_end(vaptr);
}

void loggerErr(Logger* logger, const char* str, ...)
{
    va_list vaptr;
    va_start(vaptr, str);
    loggerLog(logger, LV_ERROR, str, (char)230, &vaptr);
    va_end(vaptr);
}

void loggerSevere(Logger* logger, const char* str, ...)
{
    va_list vaptr;
    va_start(vaptr, str);
    loggerLog(logger, LV_SEVERE, str, (char)127, &vaptr);
    va_end(vaptr);
}

void loggerDebug(Logger* logger, const char* str, ...)
{
    va_list vaptr;
    va_start(vaptr, str);
    loggerLog(logger, LV_DEBUG, str, (char)127, &vaptr);
    va_end(vaptr);
}
void loggerScan(Logger* logger, const char* question, const char* format, ...)
{
    loggerLog(logger, LV_INFO, "%s: &3", question);
    va_list vaptr;
    va_start(vaptr, format);
    vscanf(format, vaptr);
    va_end(vaptr);
}