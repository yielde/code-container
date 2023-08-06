#include <math.h>
#include <stdio.h>
#include <string.h>

#include "globalerror.h"

int showresults() {
  double average;
  double caculated;
  int count;
  double err;
  int error;
  int gerror;
  double perr;
  double sum;

  if (((error = getcountandsum(&count, &sum)) != 0) ||
      ((error = geterror(&gerror)) != 0)) {
    fprintf(stderr, "Failed to get results: %s\n", strerror(error));
    return -1;
  }

  if (gerror) {
    fprintf(stderr, "Failed to compute sum: %s\n", strerror(gerror));
    return -1;
  }

  if (count == 0)
    printf("No values ware summed.\n");
  else {
    caculated = 1.0 - cos(1.0);
    average = sum / count;
    err = average - caculated;
    perr = 100.0 * err / caculated;
    printf("The sum is %f and the count is %d\n", sum, count);
    printf("The average is %f and error is %f or %f%%\n", average, err, perr);
  }
  return 0;
}