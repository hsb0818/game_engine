#ifndef _INSTANCE_RETURNER_H_
#define _INSTANCE_RETURNER_H_

static SOC_APP& R_SOC_APP() { return SOC_APP::GetInstance(); }

#endif