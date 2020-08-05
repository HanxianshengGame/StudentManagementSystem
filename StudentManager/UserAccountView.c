#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif // !_CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include "UserAccountView.h"
void PrintAllUsrAccount(USR_ACCOUNT *act_table[], int bullet_count, int account_count, int (*CompareUsrAccount)(const void *left, const void *right)) {
  system("cls");
  USR_ACCOUNT **pAct_arr = (USR_ACCOUNT **)calloc(account_count, sizeof(USR_ACCOUNT *));
  int cur_index = 0;
  for (int i = 0; i < bullet_count; ++i) {
	USR_ACCOUNT *head = act_table[i];
	while (head) {
	  pAct_arr[cur_index++] = head;
	  head = head->pNext_Usr_Account;
	}
  }
  qsort(pAct_arr, account_count, sizeof(USR_ACCOUNT *), CompareUsrAccount);

  for (int i = 0; i != cur_index; i++)
	printf("Account info : %-15s%-15s%-15s\n", pAct_arr[i]->usr_act, pAct_arr[i]->usr_pwd, pAct_arr[i]->usr_role == 1 ? "admin" : "student");
  free(pAct_arr);
  pAct_arr = NULL;
}

void PrintSingleUsrAccount(USR_ACCOUNT *account_info) {
 
  printf("Account info: \tact: %s\tpwd: %s\trole: %s\n", account_info->usr_act, account_info->usr_pwd, account_info->usr_role == 1 ? "admin" : "student");
}

void PrintAccountOperatorList(const USR_ACCOUNT *usr_account) {
  system("cls");
  if (usr_account->usr_role == ADMIN) {
	printf("1. Add     new account \n");
	printf("2. Delete  account \n");
	printf("3. Search  account \n");
	printf("4. Update  account \n");
	printf("5. Print   all account info \n");
	printf("6. Go back to the previous level\n");
  } else {
	printf("1. Update  password \n");
	printf("2. Go back to the previous level\n");
  }
}
