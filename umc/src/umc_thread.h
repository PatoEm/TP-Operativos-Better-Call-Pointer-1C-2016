/*
 * umc_thread.h
 *
 *  Created on: 12/7/2016
 *      Author: utnso
 */

#ifndef UMC_THREAD_H_
#define UMC_THREAD_H_

#include "sys/select.h"
#include <pthread.h>
#include <commons/log.h>
#include <stdlib.h>



void newUmcThread();
bool umcThread();
void manageCpuRequest(Socket* socket, StrCpuUmc* scu);
void manageKernelRequest(Socket* socket, StrKerUmc* sku);


#endif /* UMC_THREAD_H_ */
