#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif // !_CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "StudentInfoView.h"

void PrintAllStuInfo(STU_INFO *stu_info_table[], int bullet_count, int account_count, int (*CompareStuInfo)(const void *left, const void *right)) {
  system("cls");
  STU_INFO **pStu_info_arr = (STU_INFO **)calloc(account_count, sizeof(STU_INFO *));
  int cur_index = 0;
  for (int i = 0; i < bullet_count; ++i) {
	STU_INFO *head = stu_info_table[i];
	while (head) {
	  pStu_info_arr[cur_index++] = head;
	  head = head->pNext_Usr_Infor;
	}
  }
  qsort(pStu_info_arr, account_count, sizeof(STU_INFO *), CompareStuInfo);
  printf("stu info : %-15s%-15s%-15s%-15s\n","id", "name","course_id","course_score");
  for (int i = 0; i != cur_index; i++)
	printf("stu info : %-15s%-15s%-15d%-15d\n", pStu_info_arr[i]->stu_id, pStu_info_arr[i]->usr_name, pStu_info_arr[i]->usr_course_id, pStu_info_arr[i]->usr_course_score);
  free(pStu_info_arr);
  pStu_info_arr = NULL;
  system("pause");
}
void PrintSingleStuInfo(STU_INFO *stu_info) {
  printf("stu info : \tid: %s\tname: %s\tcourse_id: %d\tcourse_score:%d\n", stu_info->stu_id, stu_info->usr_name, stu_info->usr_course_id, stu_info->usr_course_score);
}


void PrintStuInfoOperatorList(const USR_ACCOUNT *usr_account) {
  system("cls");
  if (usr_account->usr_role == ADMIN) {
	printf("1. Add     new stu_info \n");
	printf("2. Delete  stu_info \n");
	printf("3. Search  stu_info \n");
	printf("4. Update  stu_info \n");
	printf("5. Print   all stu_info by id\n");
	printf("6. Print   all stu_info by score\n");
	printf("7. Go back to the previous level\n");
  } else {
	printf("1. look my info \n");
	printf("2. Go back to the previous level\n");
  }
}