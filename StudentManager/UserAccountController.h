#pragma once
#ifndef USER_ACCOUNT_CONTROLLER_H_
#define USER_ACCOUNT_CONTROLLER_H_

#include "UserAccountModel.h"
#include "CommonOperator.h"
#include <setjmp.h>


//插入新账户信息
void HeadInsertUsrActInList(USR_ACCOUNT **ppHead, const USR_ACCOUNT *tmp_account);
void DeleteUsrActInList(USR_ACCOUNT **ppHead, USR_ACCOUNT *toBeDelete) ;

//从哈希表的表格链表寻找账户信息节点。
USR_ACCOUNT *FindAccountInList(USR_ACCOUNT *pHead, const char *account) ;

//初始化账户哈希表
void InitUsrAccountTable(const char *usr_account_file, jmp_buf env_buf);
//释放哈希表
void FreeActTabele();


void ReBuildActTable();

//获取用户权限
USR_ROLE GetUsrRole(char usr_act[], char usr_pwd[], USR_ACCOUNT *cur_usr) ;

USR_ACCOUNT *FindAccount(const char usr_act[]);

void AddAccount();
void DeleteAccount();

void UpdateAccount();
void SearchAccount();

void SaveUpdateToActFile(const char *user_act_file);

//开始账号操作
void StartAccountOperator(const USR_ACCOUNT *cur_usr);


#endif // !USER_ACCOUNT_CONTROLLER_H_

