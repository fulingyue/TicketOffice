
//
// Created by 傅凌玥 on 2019/5/5.
//

#ifndef TRAINTICKET_UTILITY_H
#define TRAINTICKET_UTILITY_H

#include <cstdio>
#include <cstring>
#include "constant.h"

namespace sjtu{

int getNextWord(char *str, char *word){
  int pos = 0, len = strlen(str);
  // printf("len = %d\n", len);
  while (pos < len && (str[pos] == ' ' || str[pos] == '\n' || str[pos] == '\t')) pos++;
  int base = pos;
  while (pos < len) {
    if(str[pos] == ' ' || str[pos] == '\n' || str[pos] == '\t')
      break;
    pos++;
  }
  // printf("%d %d\n", base, pos);
  memcpy(word, str + base, pos - base);
  word[pos - base] = 0;
  return pos;
}
int skipWhiteSpace(char *word){
  int pos = 0,len = strlen(word);
  while (pos < len && (word[pos] == ' ' || word[pos] == '\n' || word[pos] == '\t')) pos++;
  return pos;
}

int getID(char* trainname){
  int ret = 0;
  int len = strlen(trainname);
  for (int i = 0; i < len; i++)
    ((ret *= BIT) += trainname[i]) %= MOD;
  return ret;
}

short dateToInt(char *date){
  short ret = 0;
  (ret *= 10) += (date[8] - '0');
  (ret *= 10) += (date[9] - '0');
  return ret;
}
void intToDate(const int &inte,char *date){
  date[0] = '2';
  date[1] = '0';
  date[2] = '1';
  date[3] = '9';
  date[4] = '-';
  date[5] = '0';
  date[6] = '6';
  date[7] = '-';
  date[8] = (char)((inte / 10) % 10) + '0';
  date[9] = (char)(inte % 10) + '0';
}

short timeToInt(char *time){
  short hour = 0,minute = 0;
  if(time[0] == 'x')
    return -1;
  hour += (short)time[0] - '0';
  (hour *= 10) += (short)time[1] - '0';
  minute += (short)(time[3] - '0');
  minute += (short)(time[4] - '0');
  return (short)(hour * 60) + minute;
}
void intToTime(const int &inte,char *time){
  if(inte == -1)  strcpy(time,"xx:xx");
  else{
    int hour,minute;
    hour = inte / 60;
    minute = inte % 60;
    time[0] = (char)((hour / 10) + '0');
    time[1] = (char)((hour % 10) + '0');
    time[2] = ':';
    time[3] = (char)((minute / 10) + '0');
    time[4] = (char)((minute / 10) + '0');
    time[5] = '\0';
  }
}



}
#endif //TRAINTICKET_UTILITY_H


