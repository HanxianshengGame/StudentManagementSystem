#pragma once
#ifndef STUDENT_INFO_VIEW_H_
#define STUDENT_INFO_VIEW_H_

#include "StudentInfoModel.h"
#include "UserAccountModel.h"
#include <stdio.h>
#include <stdlib.h>


void PrintAllStuInfo(STU_INFO *stu_info_table[], int bullet_count, int account_count, int (*CompareStuInfo)(const void *left, const void *right));
void PrintSingleStuInfo(STU_INFO *stu_info);


void PrintStuInfoOperatorList(const USR_ACCOUNT *usr_account);


#endif