#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif // !_CRT_SECURE_NO_WARNINGS

#include "UserAccountController.h"
#include "UserAccountView.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

static pUSR_ACCOUNT act_table[BULLET_COUNT]; //存放了所有账户的哈希表
static int account_count = 0;
static char m_usr_account_file[CONFIGURE_FILE_LEN];
static jmp_buf m_jmp_buf;

void HeadInsertUsrActInList(USR_ACCOUNT **ppHead, const USR_ACCOUNT *tmp_account) {
  USR_ACCOUNT *pNew = (USR_ACCOUNT *)calloc(1, sizeof(USR_ACCOUNT));
  strcpy(pNew->usr_act, tmp_account->usr_act);
  strcpy(pNew->usr_pwd, tmp_account->usr_pwd);
  pNew->usr_role = tmp_account->usr_role;
  if (*ppHead == NULL) {
    *ppHead = pNew;
  } else {
    pNew->pNext_Usr_Account = *ppHead;
    *ppHead = pNew;
  }
}

void DeleteUsrActInList(USR_ACCOUNT **ppHead, USR_ACCOUNT *toBeDelete) {
  if (*ppHead == toBeDelete) {
    free(toBeDelete);
    *ppHead = toBeDelete = NULL;
    return;
  }
  USR_ACCOUNT *head = *ppHead;
  while (head->pNext_Usr_Account != toBeDelete)
    head = head->pNext_Usr_Account;
  head->pNext_Usr_Account = toBeDelete->pNext_Usr_Account;
  free(toBeDelete);
  toBeDelete = NULL;
}

//从哈希表的表格链表寻找账户信息节点。
USR_ACCOUNT *FindAccountInList(USR_ACCOUNT *pHead, const char *account) {
  while (pHead) {
    if (!strcmp(account, pHead->usr_act))
      return pHead;
    pHead = pHead->pNext_Usr_Account;
  }
  return NULL;
}

void InitUsrAccountTable(const char *usr_account_file, jmp_buf env_buf) {
  FILE *usr_act_fp = fopen(usr_account_file, "r");
  if (usr_act_fp == NULL) {
    perror("usr_account_file: ");
    longjmp(env_buf, -1);
  }
  account_count = 0;
  memset(m_jmp_buf, 0, sizeof(int) * 16);
  memset(m_usr_account_file, 0, sizeof(m_usr_account_file));
  memset(act_table, 0, sizeof(act_table));

  memcpy(m_jmp_buf, env_buf, sizeof(int) * 16);
  strcpy(m_usr_account_file, usr_account_file);
  USR_ACCOUNT tmp_account = {0};

  while (fscanf(usr_act_fp, "%s%s%d\n", tmp_account.usr_act, tmp_account.usr_pwd, &tmp_account.usr_role) != EOF) {
    ++account_count;
    USR_ACCOUNT *head = act_table[hash(tmp_account.usr_act)];
    HeadInsertUsrActInList(&act_table[hash(tmp_account.usr_act)], &tmp_account);
    memset(&tmp_account, 0, sizeof(tmp_account));

  }
  fclose(usr_act_fp);
}

void FreeActTabele() {
  for (int i = 0; i != BULLET_COUNT; ++i) {
    USR_ACCOUNT *head = act_table[i];
    USR_ACCOUNT *to_be_deleted = head;
    while (head) {
      head = head->pNext_Usr_Account;
      free(to_be_deleted);
      to_be_deleted = head;
    }
    act_table[i] = to_be_deleted = head = NULL;
  }
}


void ReBuildActTable() {
  SaveUpdateToActFile(m_usr_account_file);
  FreeActTabele();
  InitUsrAccountTable(m_usr_account_file, m_jmp_buf);
}

USR_ROLE GetUsrRole(char usr_act[], char usr_pwd[], USR_ACCOUNT *cur_usr) {
  USR_ACCOUNT *usr_account = FindAccount(usr_act);
  if (usr_account == NULL || strcmp(usr_account->usr_pwd, usr_pwd) != 0)
    return NULL_ROLE;

  strcpy(cur_usr->usr_act, usr_account->usr_act);
  strcpy(cur_usr->usr_pwd, usr_account->usr_pwd);
  cur_usr->usr_role = usr_account->usr_role;
  return usr_account->usr_role;
}
USR_ACCOUNT *FindAccount(const char usr_act[]) {
  USR_ACCOUNT *res_list = act_table[hash(usr_act)];
  USR_ACCOUNT *res = FindAccountInList(res_list, usr_act);
  return res;
}

void AddAccount() {
  system("cls");
  printf("please input account info:  1. account  2. password  3. role(admin or student) : \n");
  USR_ACCOUNT usr_account = {0};
  char role[10] = {0};
  scanf("%s%s%s", usr_account.usr_act, usr_account.usr_pwd, role);
  if(strcmp(role, "admin") == 0)
    usr_account.usr_role = 1;

  if (FindAccount(usr_account.usr_act) != NULL) {
    printf("Add failure, The account already exists!\n");
  } else {
    HeadInsertUsrActInList(&act_table[hash(usr_account.usr_act)], &usr_account);
    printf("Add success!\n");
    ++account_count;
  }
}

void DeleteAccount() {
  system("cls");
  printf("please input the account: ");
  char usr_act[USR_ACT_LEN] = {0};
  gets(usr_act);
  USR_ACCOUNT *toBeDeleted = FindAccount(usr_act);
  if (toBeDeleted == NULL) {
    printf("Delete failure, The account not exists!\n");
  } else {
    DeleteUsrActInList(&act_table[hash(usr_act)], toBeDeleted);
    printf("Delete success!\n");
    --account_count;
  }
}

void UpdateAccount() {
  system("cls");
  printf("please input the account: ");
  char usr_act[USR_ACT_LEN] = {0};
  gets(usr_act);
  USR_ACCOUNT *update_account = FindAccount(usr_act);
  if (update_account == NULL) {
    printf("Update failure, The account not exists!\n");
    return;
  }
  PrintSingleUsrAccount(update_account);
  printf("\nSelect the modified information name： \n1. account\t2. password\t3. role(admin/student, Unless the instruction admin, all others are student )\n ");
  char command;
  while (command = getch()) {
    if (command >= '1' && command <= '3')
      break;
    printf("\nerror command!");
  }
  printf("\n");
  printf("Enter the modified information：");
  char update_info[30] = {0};
  gets(update_info);
  switch (command) {
  case '1':
	  if (FindAccount(update_info) == NULL) {
		strcpy(update_account->usr_act, update_info);
        ReBuildActTable();
	  }
	  else {
		printf("Update failure, The account exists!\n");
		return;
	  }
    break;
  case '2':
    strcpy(update_account->usr_pwd, update_info);
    break;
  case '3':
    if (strcmp(update_info, "admin") == 0)
      update_account->usr_role = ADMIN;
    else
      update_account->usr_role = STU;
    break;
  default:
    break;
  }
  printf("Update success!\n");
}
void UpdatePassword(const USR_ACCOUNT *cur_usr) {
  system("cls");
  USR_ACCOUNT *update_account = FindAccount(cur_usr->usr_act);
  printf("please input new password: ");
  char update_info[30] = {0};
  gets(update_info);
  memset(update_account->usr_pwd, 0 ,sizeof(update_account->usr_pwd));
  strcpy(update_account->usr_pwd, update_info);
  printf("\nupdate success！");
  system("pause");
}
void SearchAccount() {
  system("cls");
  printf("please input the account: ");
  char usr_act[USR_ACT_LEN] = {0};
  gets(usr_act);
  USR_ACCOUNT *res = FindAccount(usr_act);
  if (res == NULL) {
    printf("Search failure, The account not exists!\n");
    return;
  }
  PrintSingleUsrAccount(res);
}

int CompareUsrAccount(const void *left,const void *right) {
  USR_ACCOUNT **ppAct1 = (USR_ACCOUNT **)left;
  USR_ACCOUNT **ppAct2 = (USR_ACCOUNT **)right;
  
  if ((*ppAct1)->usr_role == (*ppAct2)->usr_role) {
    return strcmp((*ppAct1)->usr_act, (*ppAct2)->usr_act) > 0;
  } else {
    return (*ppAct1)->usr_role > (*ppAct2)->usr_role;
  }

}
void SaveUpdateToActFile(const char *user_act_file) {
  USR_ACCOUNT **pAct_arr = (USR_ACCOUNT**)calloc(account_count, sizeof(USR_ACCOUNT*));
  int cur_index = 0;
  for (int  i = 0; i < BULLET_COUNT; ++i) {
    USR_ACCOUNT *head = act_table[i];
    while (head) {
      pAct_arr[cur_index++] = head;
      head = head->pNext_Usr_Account;
    }
  }
  qsort(pAct_arr, account_count, sizeof(USR_ACCOUNT*), CompareUsrAccount);
  
  FILE *fp = fopen(user_act_file, "w");
  for (int i = 0; i != account_count; ++i) 
    fprintf(fp, "%s %s %d\n", pAct_arr[i]->usr_act, pAct_arr[i]->usr_pwd, pAct_arr[i]->usr_role);

  free(pAct_arr);
  pAct_arr = NULL;
  fclose(fp);
}

//开始账号操作
void StartAccountOperator(const USR_ACCOUNT *cur_usr) {
  char command;

  if (cur_usr->usr_role == STU)
    while (1) {
      system("cls");
      PrintAccountOperatorList(cur_usr);
      printf("Please input commad: ");

      command = getch();
      switch (command) {
      case '1':
        UpdatePassword(cur_usr);
        break;
      case '2':
        return;
      default:
        break;
      }
    } 
  else
      while (1) {
        system("cls");
        PrintAccountOperatorList(cur_usr);
        printf("Please input commad: ");

        command = getch();
        switch (command) {
        case '1':
          AddAccount();
          system("pause");
          break;
        case '2':
          DeleteAccount();
          system("pause");
          break;
        case '3':
          SearchAccount();
          system("pause");
          break;
        case '4':
          UpdateAccount();
          system("pause");
          break;
        case '5':
          PrintAllUsrAccount(act_table, BULLET_COUNT,account_count, CompareUsrAccount);
          system("pause");
          break;
        case '6':
          return;
        default:
          break;
        }
      }

}
