#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <zlib.h>

#if !defined(__ANDROID__) && !defined(__MACOSX__) && !defined(__LINUX__)

static void *memmem(const void* haystack, size_t hlen, const void* needle, size_t nlen)
{
	const unsigned char* p = haystack;
	size_t plen = hlen;

	if(!nlen) return NULL;

	long needle_first = *(unsigned char*)needle;

	while (plen >= nlen && (p = memchr(p, needle_first, plen - nlen + 1)))
	{
		if (!memcmp(p, needle, nlen))
		return (void *)p;

		p++;
		plen = hlen - (p - (const unsigned char*)haystack);
	}

	return NULL;
}

#endif

static const char placeholder[] = "</script>";

int main(int argc, char** argv)
{
	if(argc != 4)
	{
        printf("Usage: tic2html <html-template> <tic-input> <html-output>\n");
        return 0;
	}

    FILE* template = fopen(argv[1], "rb");
    if(!template)
	{
        printf("Can't open template file\n");
        return 0;
	}
	fseek(template, 0, SEEK_END);
	int htmlSize = ftell(template);
	fseek(template, 0, SEEK_SET);
	unsigned char* html = (unsigned char*)malloc(htmlSize);
    if(!html)
	{
        printf("Can't alloc memory\n");
        return 0;
	}
	fread(html, htmlSize, 1, template);
	fclose(template);
	unsigned char* ptr = memmem(html, htmlSize, placeholder, sizeof(placeholder)-1);

    FILE* tic = fopen(argv[2], "rb");
    if(!tic)
	{
        printf("Can't open input file\n");
        return 0;
	}
	fseek(tic, 0, SEEK_END);
	uLongf ticSize = ftell(tic);
	fseek(tic, 0, SEEK_SET);
	unsigned char* buffer = (unsigned char*)malloc(ticSize);
	unsigned char* output = (unsigned char*)malloc(ticSize);
    if(!buffer || !output)
	{
        printf("Can't alloc memory\n");
        return 0;
	}
	fread(buffer, ticSize, 1, tic);
	fclose(tic);
	if(compress2(output, &ticSize, buffer, ticSize, Z_BEST_COMPRESSION) != Z_OK)
	{
		printf("compression error\n");
        return 0;
	}
    free(buffer);

    FILE* out = fopen(argv[3], "wb");
    if(!out)
	{
        printf("Can't open output file\n");
        return 0;
	}
	fwrite(html, ptr-html, 1, out);
	fprintf(out, "\tvar cartridge = [");
	for(int i = 0; i < ticSize; i++)
		fprintf(out, "%d,", output[i]);
	fprintf(out, "];\n\t");
	fwrite(ptr, htmlSize - (ptr-html), 1, out);
	fclose(out);

	return 0;
}
