#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define TOTAL_SIZE 65536
#define CHILDREN 4
#define CHUNK_SIZE (TOTAL_SIZE / CHILDREN)

int main() {
  static int arr[TOTAL_SIZE];
  int pids[CHILDREN];
  int statuses[CHILDREN];
  int n;

  // הכנה - מילוי המערך
  for (int i = 0; i < TOTAL_SIZE; i++) {
    arr[i] = i; // אפשר גם arr[i] = i; אם אתה רוצה סכום שונה
  }
  printf("Array initialized with %d elements.\n", TOTAL_SIZE);
  // יצירת תהליכי בן
  int res = forkn(CHILDREN, pids);
  printf("forkn returned %d\n", res);
  if (res < 0) {
    printf("forkn failed\n");
    exit(-1, "forkn failed");
  }

  printf("forkn succeeded\n");

  if (res > 0) {
    // תהליך בן
    int start = (res - 1) * CHUNK_SIZE;
    int end = res * CHUNK_SIZE;
    int sum = 0;

    for (int i = start; i < end; i++) {
      sum += arr[i];
    }

    exit(sum, "child done");
  }

  // תהליך אב
  if (waitall(&n, statuses) < 0) {
    printf("waitall failed\n");
    exit(-1, "waitall failed");
  }

  int total = 0;
  for (int i = 0; i < n; i++) {
    total += statuses[i];
  }

  printf("Total sum is: %d\n", total); // אמור להיות 65536 אם arr[i] = 1
  exit(0, "done");
}