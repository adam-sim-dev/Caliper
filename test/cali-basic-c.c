/*
 * cali-basic-c
 * Caliper C interface test
 */

#include <cali.h>

#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

enum phases { PHASE_MAIN = 0, PHASE_LOOP = 1 };
const char* phase_string[] = { "main", "loop" };

void print_context(cali_id_t env)
{
  size_t    size = cali_get_context_size(env);
  uint64_t* buf  = malloc(size * sizeof(uint64_t));

  size = cali_get_context(env, buf, size);

  printf("Context size: %u, ", size/2);
  for (size_t e = 0; e < size; e += 2)
    printf("%" PRIx64 ":%" PRIx64 " ", buf[e], buf[e+1]);
  puts("\n");

  free(buf);
}


int main(int argc, char* argv[])
{
  cali_id_t attr_phase = cali_create_attribute("phase", CALI_TYPE_STRING, CALI_ATTR_DEFAULT);
  cali_id_t env = cali_get_environment();

  cali_begin(env, attr_phase, phase_string[PHASE_MAIN], strlen(phase_string[PHASE_MAIN]));

  int count = argc > 1 ? atoi(argv[1]) : 4;

  cali_id_t attr_iter = cali_create_attribute("iter", CALI_TYPE_INT, CALI_ATTR_ASVALUE);

  cali_begin(env, attr_phase, phase_string[PHASE_LOOP], strlen(phase_string[PHASE_LOOP]));

  for (int64_t i = 0; i < count; ++i) {
    cali_set(env, attr_iter, &i, sizeof(int64_t));

    print_context(env);
  }

  cali_end(env, attr_iter); /* unset "iter" */
  cali_end(env, attr_phase); /* end "loop" */

  print_context(env);

  cali_end(env, attr_phase); /* end "main" */

  cali_write_metadata();
}