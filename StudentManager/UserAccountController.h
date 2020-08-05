#pragma once
#ifndef USER_ACCOUNT_CONTROLLER_H_
#define USER_ACCOUNT_CONTROLLER_H_

#include "UserAccountModel.h"
#include "CommonOperator.h"
#include <setjmp.h>


//�������˻���Ϣ
void HeadInsertUsrActInList(USR_ACCOUNT **ppHead, const USR_ACCOUNT *tmp_account);
void DeleteUsrActInList(USR_ACCOUNT **ppHead, USR_ACCOUNT *toBeDelete) ;

//�ӹ�ϣ��ı������Ѱ���˻���Ϣ�ڵ㡣
USR_ACCOUNT *FindAccountInList(USR_ACCOUNT *pHead, const char *account) ;

//��ʼ���˻���ϣ��
void InitUsrAccountTable(const char *usr_account_file, jmp_buf env_buf);
//�ͷŹ�ϣ��
void FreeActTabele();


void ReBuildActTable();

//��ȡ�û�Ȩ��
USR_ROLE GetUsrRole(char usr_act[], char usr_pwd[], USR_ACCOUNT *cur_usr) ;

USR_ACCOUNT *FindAccount(const char usr_act[]);

void AddAccount();
void DeleteAccount();

void UpdateAccount();
void SearchAccount();

void SaveUpdateToActFile(const char *user_act_file);

//��ʼ�˺Ų���
void StartAccountOperator(const USR_ACCOUNT *cur_usr);


#endif // !USER_ACCOUNT_CONTROLLER_H_

