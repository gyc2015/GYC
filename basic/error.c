#include <stdarg.h>
#include <stdlib.h>
#include "input.h"

void Error(struct coord* pcoord, const char *format, ...)
{
	va_list ap;

	if (pcoord)
	{
		fprintf(stderr, "(%s,%dL,%dC):", pcoord->pcFileName, pcoord->line,pcoord->col);
	}
	fprintf(stderr, "error:");
	va_start(ap, format);
	vfprintf(stderr, format, ap);
	fprintf(stderr, "\n");
	va_end(ap);
}

void Fatal(const char *format, ...)
{
	va_list ap;

	va_start(ap, format);
	vfprintf(stderr, format, ap);
	fprintf(stderr, "\n");
	va_end(ap);

	exit(-1);
}

void Warning(struct coord* pcoord, const char *format, ...)
{
	va_list ap;

	if (pcoord)
	{
		fprintf(stderr, "(%s,%dL,%dC):", pcoord->pcFileName, pcoord->line,pcoord->col);
	}

	fprintf(stderr, "warning:");
	va_start(ap, format);
	vfprintf(stderr, format, ap);
	fprintf(stderr, "\n");
	va_end(ap);

	return;
}



