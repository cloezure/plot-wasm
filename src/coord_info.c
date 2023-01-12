#include "coord_info.h"

struct coord_info* coord_info_build(void) {
  struct coord_info* info = malloc(sizeof *info);
  return info;
}

void coord_info_free(struct coord_info* coord_info) {

}
