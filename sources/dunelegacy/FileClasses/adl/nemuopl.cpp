#include "nemuopl.h"

Copl *createNukedOpl(int rate, bool usestereo)
{
    return new CNemuopl(rate, usestereo);
}
