#pragma once
#ifndef STUDENT_INFO_CONTROLLER_H_
#define STUDENT_INFO_CONTROLLER_H_
#include "StudentInfoModel.h"
#include "StudentInfoView.h"
#include "UserAccountModel.h"
#include <stdio.h>
#include <setjmp.h>



void HeadInsertInList(STU_INFO **ppHead, const STU_INFO *tmp_stu_info);
void DeleteStuInfoInList(STU_INFO **ppHead, STU_INFO *toBeDelete);


STU_INFO *FindStuInfoInList(STU_INFO *pHead, const char *stu_id);


void InitStuInfoTable(const char *usr_account_file, jmp_buf env_buf);

void FreeStuInfoTable();

void ReBuildStuInfoTable();
STU_INFO *FindStuInfo(const char usr_act[]);

void AddStuInfo();
void DeleteStuInfo();

void UpdateStuInfo();
void SearchStuInfo();
int CompareStuInfoById(const void *left, const void *right);
int CompareStuInfoByScore(const void *left, const void *right);
void SaveUpdateToStuInfoFile(const char *stu_info_file);




void StartStuInfoOperator(const USR_ACCOUNT *cur_usr);



#endif // !STUDENT_INFO_CONTROLLER_H_
