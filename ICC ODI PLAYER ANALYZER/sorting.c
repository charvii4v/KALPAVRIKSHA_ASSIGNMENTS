#include "sorting.h"
#include "team.h"

int compareTeamsBySR(const void* a, const void* b) {
    const Team* A = *(const Team**)a;
    const Team* B = *(const Team**)b;

    double A_SR = (A->battingSRCount == 0) ? 0 : A->battingSRSum / A->battingSRCount;
    double B_SR = (B->battingSRCount == 0) ? 0 : B->battingSRSum / B->battingSRCount;

    if (B_SR > A_SR) return 1;
    if (B_SR < A_SR) return -1;
    return 0;
}
