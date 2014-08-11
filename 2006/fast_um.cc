#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdint>

typedef uint32_t Platter;

class UM {
 public:
  UM() {}
  ~UM() {}

  bool Init(int argc, char** argv);
  void Run();

 private:
  Platter Alloc(Platter size);
  
  Platter* prog = NULL;
  Platter r[8] {};
  Platter pc = 0;
};
  
Platter UM::Alloc(Platter size) {
  Platter* p = new Platter[size + 1];
  p[0] = size;
  for (Platter i = 1; i <= size; ++i)
    p[i] = 0;
  return reinterpret_cast<Platter>(p + 1);
}

bool UM::Init(int argc, char** argv) {
  const char* filename = (argc > 1) ? argv[1] : (char*)("sandmark.umz");
  FILE* fp = fopen(filename, "rb");

  if (fp == NULL)
    return false;

  fseek(fp, 0, SEEK_END);
  Platter size_byte = ftell(fp);
  rewind(fp);
  printf("Program size = %d Byte\n", size_byte);

  prog = new Platter[size_byte];
  fread(prog, 1, size_byte, fp);
  fclose(fp);

  Platter size = size_byte / sizeof(Platter);

  /* LE -> BE */
  for (Platter i = 0 ; i < size ; ++i) {
    Platter word = prog[i];
    word = ((word & 0x00ff00ff) << 8) | ((word >> 8) & 0x00ff00ff);
    word = ((word & 0x0000ffff) << 16) | ((word >> 16) & 0x0000ffff);
    prog[i] = word;
  }

  return true;
}

void UM::Run() {
  while (true) {
    Platter code = prog[pc++];
    Platter ope = (code >> 28) & 15;

    if (ope == 13) {
      int a = (code >> 25) & 7;
      r[a] = code & 0x1ffffff;
      continue;
    }

    if (ope < 7) {
      int a = (code >> 6) & 7;
      int b = (code >> 3) & 7;
      int c = code & 7;

      switch (ope) {
      case 0:
        if (r[c])
          r[a] = r[b];
        break;
      case 1:
        if (r[b])
          r[a] = (reinterpret_cast<Platter*>(r[b]))[r[c]];
        else
          r[a] = prog[r[c]];
        break;
      case 2:
        if (r[a])
          ((Platter*)r[a])[r[b]] = r[c];
        else
          prog[r[b]] = r[c];
        break;
      case 3:
        r[a] = r[b] + r[c];
        break;
      case 4:
        r[a] = r[b] * r[c];
        break;
      case 5:
        r[a] = r[b] / r[c];
        break;
      case 6:
        r[a] = ~(r[b] & r[c]);
        break;
      }
      continue;
    }

    switch (ope) {
    case 7:
      return;
    case 8:
      r[(code >> 3) & 7] = Alloc(r[code & 7]);
      break;
    case 9:
      free((Platter*)r[code & 7] - 1);
      break;
    case 10:
      putchar(r[code & 7]);
      fflush(stdout);
      break;
    case 11:
      r[code & 7] = getchar();
      break;
    case 12: {
      int b = (code >> 3) & 7;
      if (r[b]) {
        int size = *((Platter*)r[b] - 1);
        prog = (Platter*)realloc((void*)prog, size * sizeof(Platter));
        memcpy(prog, (void*)r[b], size * sizeof(Platter));
      }
      pc = r[code & 7];
      break;
    }
    default:
      printf("Unknown operation %u\n", ope);
      return;
    }
  }
}

int main(int argc, char** argv) {
  UM um;
  if (!um.Init(argc, argv))
    return 1;

  um.Run();

  return 0;
}
