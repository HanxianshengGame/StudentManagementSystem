#pragma once
#ifndef USER_ACCOUNT_VIEW_H_
#define USER_ACCOUNT_VIEW_H_

#include "UserAccountModel.h"


void PrintAllUsrAccount(USR_ACCOUNT *act_table[], int bullet_count, int account_count, int (*CompareUsrAccount)(const void *left, const void *right));

void PrintSingleUsrAccount(USR_ACCOUNT *account_info) ;
void PrintAccountOperatorList(const USR_ACCOUNT *usr_account);

#endif