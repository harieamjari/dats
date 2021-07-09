#include "filter.h"
#include <string.h>

extern DFFilter f_reverb;

#include "list_filters.c"

const DFFilter *get_dfilter_by_name(const char *name) {

  if (!*name || name == NULL)
    return NULL;

  for (int i = 0; filter_list[i] != NULL; i++)
    if (!strcmp(filter_list[i]->name, name))
      return filter_list[i];

  return NULL;
}

void print_filters() {
  for (int i = 0; filter_list[i] != NULL; i++) {
    printf("%s\n  description: %s\n  options:\n", filter_list[i]->name,
           filter_list[i]->description);
    for (int ii = 0; filter_list[i]->options[ii].option_name != NULL; ii++) {
      printf("        %s: %s\n", filter_list[i]->options[ii].option_name,
             filter_list[i]->options[ii].description);
    }
    putchar('\n');
  }
}
