#include <stdio.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#define PROP_PATH "./build.prop"
#define dbgprint(format,args...) ({ \
		fprintf (stderr, "[%s] <%d>-->", __func__, __LINE__); \
		fprintf(stderr, format, ##args);})

static void property_set(char *key, char *value)
{
	dbgprint("%s=%s\n", key, value);
}

static void load_properties(char *data)
{
	char *key, *value, *eol, *sol, *tmp;

	sol = data;
	while((eol = strchr(sol, '\n'))) {
		key = sol;
		*eol++ = 0;
		sol = eol;

		value = strchr(key, '=');
		if(value == 0) continue;
		*value++ = 0;

		while(isspace(*key)) key++;
		if(*key == '#') continue;
		tmp = value - 2;
		while((tmp > key) && isspace(*tmp)) *tmp-- = 0;

		while(isspace(*value)) value++;
		tmp = eol - 2;
		while((tmp > value) && isspace(*tmp)) *tmp-- = 0;

		property_set(key, value);
	}
}

static void *_read_file(const char *filename, ssize_t *_size)
{
	int ret, fd;
	struct stat sb;
	ssize_t size;
	void *buffer = NULL;

	/* open the file */
	fd = open(filename, O_RDONLY);
	if (fd < 0)
		return NULL;

	/* find out how big it is */
	if (fstat(fd, &sb) < 0)
		goto bail;
	size = sb.st_size;
	dbgprint("standard size = %ld\n", size);

	/* allocate memory for it to be read into */
	buffer = malloc(size);
	if (!buffer)
		goto bail;

	/* slurp it into our buffer */
	ret = read(fd, buffer, size);
	if (ret != size)
		goto bail;

	/* let the caller know how big it is */
	*_size = size;

bail:
	close(fd);
	return buffer;
}

static char *read_file(const char *fn, unsigned *sz)
{
	FILE *fp = NULL;
	char *buffer = NULL;

	fp = fopen(fn, "rw");
	if (!fp) {
		dbgprint("fopen error\n");
		exit(-1);
	}

	fseek(fp, 0, SEEK_END);
	*sz = ftell(fp);
	dbgprint("my sz is %d\n", *sz);
	rewind(fp);

	buffer = (char *)malloc(sizeof(char) * (*sz));
	if (!buffer) {
		dbgprint("malloc buffer is failed...\n");
		goto fail;
	}
	fread(buffer, 1, *sz, fp);
	if (!buffer) {
		dbgprint("fread failed...\n");
		goto fail;
	}
	//dbgprint("buffer:\n%s\n", buffer);
	fclose(fp);

	return buffer;
fail:
	fclose(fp);
	exit(-1);
}

static void _load_properties(char *data)
{
	char *pbuf = data;
	char *delim = "\n";
	char *token, *ptoken, *key, *value, *tmp, *ops;

	for(token = strsep(&pbuf, delim); token != NULL; \
		token = strsep(&pbuf, delim)) {
		ptoken = token;

		while (isspace(*ptoken)) ptoken++;
		if (ptoken[0] == '#' || ptoken[0] == 0) {
			//dbgprint("%s\n", ptoken);
			continue;
		}
		key = ptoken;
		tmp = strstr(ptoken, "=");
		ops = tmp - 1;
		while (isspace(*ops)) ops--;
		tmp++;
		*++ops = 0; //for key
		while(isspace(*tmp)) tmp++;
		value = tmp;
		property_set(key, value);
	}
}

static void load_properties_from_file(const char *fn)
{
	char *data;
	unsigned sz;

	data = read_file(fn, &sz);

	if(data != 0) {
		_load_properties(data);
		free(data);
	}
}

static void load_persistent_properties()
{
}

int main()
{
	load_properties_from_file(PROP_PATH);
	load_persistent_properties();

	return 0;
}
