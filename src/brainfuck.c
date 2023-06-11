#include <stdio.h>

int main(int argc, char **argv) {
  if (argc < 2) {
    fputs("expected a file argument\n", stderr);
    return 1;
  }

  FILE *contents = fopen(argv[1], "r");
  if (!contents) {
    fprintf(stderr, "couldn't open file path '%s'. are you sure it exists?\n",
            argv[1]);
    return 1;
  }

  char mem[30000] = {0};
  const size_t max_index = sizeof(mem) - 1;

  char current = fgetc(contents);
  size_t pos = 0;

  // You can have a maximum of 100 nested loops
  size_t stack[100] = {0};
  size_t stack_size = 0;

  while (current != EOF) {
    switch (current) {
    case '>':
      if (++pos > max_index)
        pos = 0;
      break;
    case '<':
      if (pos == 0) {
        pos = max_index;
      } else {
        --pos;
      }
      break;
    case '+':
      ++mem[pos];
      break;
    case '-':
      --mem[pos];
      break;
    case '[':
      if (!mem[pos]) {
        if (stack_size > 0) {
          if (stack[stack_size - 1] == (size_t)ftell(contents)) {
            --stack_size;
          }
        }
        int open = 0;
        current = fgetc(contents);
        while (current != ']' || open > 0) {
          if (current == EOF) {
            fputs("hit eof prematurely\n", stderr);
            return 1;
          } else if (current == '[') {
            ++open;
          } else if (current == ']') {
            --open;
          }
          current = fgetc(contents);
        }
      } else {
        size_t tell = ftell(contents);
        if (stack_size > 0) {
          if (stack[stack_size - 1] == tell)
            break;
        }
        stack[stack_size] = tell;
        if (++stack_size >= 100) {
          fprintf(stderr,
                  "too many nested loops, overflowed stack (maximum loop depth "
                  "is 100). at: %lu\n",
                  tell);
          return 1;
        }
      }
      break;

    case ']':
      if (stack_size == 0) {
        fprintf(stderr, "unmatched closing bracket. at: %lu\n",
                ftell(contents));
        return 1;
      }
      fseek(contents, stack[stack_size - 1] - 1, SEEK_SET);
      break;
    case '.':
      putchar(mem[pos]);
      break;
    case ',':
      mem[pos] = getchar();
      break;
    }

    // next char
    current = fgetc(contents);
  }
}
