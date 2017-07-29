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

static void load_properties_from_file(const char *fn)
{
	char *data;
	unsigned sz;

	data = read_file(fn, &sz);

	if(data != 0) {
		load_properties(data);
		free(data);
	}
}
