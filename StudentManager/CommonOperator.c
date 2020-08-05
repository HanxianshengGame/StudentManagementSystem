#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif // !_CRT_SECURE_NO_WARNINGS

#include "CommonOperator.h"

int hash(const char *key) {
 int h = 0, g;
 while (*key) {
   h = (h << 4) + *key++;
   g = h & 0xf0000000;
   if (g)h ^= g >> 24;
   h &= ~g;
 }
 return h % BULLET_COUNT;
}