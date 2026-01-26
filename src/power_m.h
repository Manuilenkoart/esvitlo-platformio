#ifndef power_m
#define power_m

void powerManagementSetup(void (*setDataSEE)(bool));
void powerManagementLoop(void (*setDataSEE)(bool));

#endif