#pragma once
#include "opl.h"
#include "dbopl.h"
#include <cstdio>

class CDemuopl: public Copl
{
public:
  CDemuopl(int rate, bool usestereo)
    : stereo(usestereo)
  {
      opl.Init(rate);
      currType = TYPE_OPL2;
  }

  void update(short *buf, int samples) override
  {
#warning FIXME dosbox OPL

      while (samples > 0) {
          Bitu count = samples;

          constexpr Bitu MaxCount = 512;
          if (count > MaxCount)
              count = MaxCount;

          if (stereo) {
              opl.GenerateArr(buf, &count);

              samples -= count;
              buf += 2 * count;
          }
          else {
              Bit16s temp[2 * MaxCount];
              opl.GenerateArr(temp, &count);

              for (Bitu i = 0; i < count; ++i)
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

      opl.WriteReg(reg, val);
  }

  void init() override {};

private:
  bool            stereo;
  DBOPL::Handler  opl;
};
