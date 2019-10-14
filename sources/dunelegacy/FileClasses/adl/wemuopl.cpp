#include "wemuopl.h"

Copl *createWoodyOpl(int rate, bool usestereo)
{
    return new CWemuopl(rate, usestereo);
}
