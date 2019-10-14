#include "opl.h"

Copl *createWoodyOpl(int rate, bool usestereo);
Copl *createDosboxOpl(int rate, bool usestereo);
Copl *createNukedOpl(int rate, bool usestereo);

Copl *createOpl(int model, int rate, bool usestereo)
{
    switch (model) {
    default:
    case 0:
        return createWoodyOpl(rate, usestereo);
    case 1:
        return createDosboxOpl(rate, usestereo);
    case 2:
        return createNukedOpl(rate, usestereo);
    }
}
