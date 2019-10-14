#pragma once
#include "opl.h"
#include "nukedopl3.h"
#include <cstdio>

class CNemuopl: public Copl
{
public:
  CNemuopl(int rate, bool usestereo)
    : stereo(usestereo)
  {
      OPL3_Reset(&opl, rate);
      currType = TYPE_OPL2;
  }

  void update(short *buf, int samples) override
  {
      if (stereo)
          OPL3_GenerateStream(&opl, buf, samples);
      else {
          while (samples > 0) {
              constexpr unsigned MaxCount = 512;

              unsigned count = samples;
              if (count > MaxCount)
                  count = MaxCount;

              int16_t temp[2 * MaxCount];
              OPL3_GenerateStream(&opl, temp, count);

              for (unsigned i = 0; i < count; ++i)
                  buf[i] = temp[2 * i];

              samples -= count;
              buf += count;
          }
      }
  }

  // template methods
  void write(int reg, int val) override
  {
      if (currChip != 0)
          return;

      OPL3_WriteRegBuffered(&opl, reg, val);
  }

  void init() override {};

private:
  bool       stereo;
  opl3_chip  opl;
};
