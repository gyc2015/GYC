#ifndef __ERROR_H_
#define __ERROR_H_

void Error(struct coord* pcoord, const char *format, ...);
void Fatal(const char *format, ...);
void Warning(struct coord* pcoord, const char *format, ...);

#endif

