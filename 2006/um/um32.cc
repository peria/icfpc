#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdint>
#include <fstream>

typedef uint32_t Platter;

class UM {
 public:
  UM() {}
  ~UM() {}

  bool Load(const char* filename);
  void Run();

 private:
  Platter Alloc(Platter size);

  Platter* program_ = NULL;
  Platter r[8]{};
  Platter pc = 0;
};

Platter UM::Alloc(Platter size) {
  Platter* p = new Platter[size + 1];
  p[0] = size;
  for (Platter i = 1; i <= size; ++i)
    p[i] = 0;
  return reinterpret_cast<Platter>(p + 1);
}

bool UM::Load(const char* filename) {
  std::ifstream ifs;
  ifs.open(filename, std::ios::binary);
  if (!ifs.is_open())
    return false;

  ifs.seekg(0, std::ios_base::end);
  Platter size_byte = ifs.tellg();
  ifs.seekg(0, std::ios_base::beg);
  printf("Program size = %d Byte\n", size_byte);

  // Allocates 4x program size.
  program_ = new Platter[size_byte];
  ifs.read(reinterpret_cast<char*>(program_), size_byte);
  ifs.close();

  Platter size = size_byte / sizeof(Platter);
  // Change endian
  for (Platter i = 0; i < size; ++i) {
    Platter word = program_[i];
    word = ((word & 0x00ff00ff) << 8) | ((word >> 8) & 0x00ff00ff);
    word = ((word & 0x0000ffff) << 16) | ((word >> 16) & 0x0000ffff);
    program_[i] = word;
  }

  return true;
}

void UM::Run() {
  while (true) {
    Platter code = program_[pc++];
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
      case 0:  // Conditional Move
        if (r[c])
          r[a] = r[b];
        break;
      case 1:  // Read
        if (r[b])
          r[a] = (reinterpret_cast<Platter*>(r[b]))[r[c]];
        else
          r[a] = program_[r[c]];
        break;
      case 2:  // Write
        if (r[a]) {
          Platter* pointer = reinterpret_cast<Platter*>(r[a]);
          pointer[r[b]] = r[c];
        } else
          program_[r[b]] = r[c];
        break;
      case 3:  // Add
        r[a] = r[b] + r[c];
        break;
      case 4:  // Mult
        r[a] = r[b] * r[c];
        break;
      case 5:  // Div
        r[a] = r[b] / r[c];
        break;
      case 6:  // NotAnd
        r[a] = ~(r[b] & r[c]);
        break;
      }
      continue;
    }

    switch (ope) {
    case 7:  // Halt
      return;
    case 8:  // Alloc
      r[(code >> 3) & 7] = Alloc(r[code & 7]);
      break;
    case 9: {  // Free
      Platter* pointer = reinterpret_cast<Platter*>(r[code & 7]) - 1;
      free(pointer);
      break;
    }
    case 10:  // Output
      std::putchar(r[code & 7]);
      break;
    case 11:
      r[code & 7] = std::getchar();
      break;
    case 12: {
      int b = (code >> 3) & 7;
      if (r[b]) {
        Platter* pointer = reinterpret_cast<Platter*>(r[b]);
        int size = pointer[-1];
        program_ = (Platter*)realloc(reinterpret_cast<void*>(program_),
                                     size * sizeof(Platter));
        for (int i = 0; i < size; ++i)
          program_[i] = pointer[i];
      }
      pc = r[code & 7];
      break;
    }
    }
  }
}

int main(int argc, char* argv[]) {
  const char* filename = "sandmark.umz";
  if (argc > 1)
    filename = argv[1];

  UM um;
  if (!um.Load(filename))
    return 1;

  um.Run();

  return 0;
}
