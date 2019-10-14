#include "demuopl.h"

Copl *createDosboxOpl(int rate, bool usestereo)
{
    return new CDemuopl(rate, usestereo);
}
