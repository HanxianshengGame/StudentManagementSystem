#define _CRT_SECURE_NO_WARNINGS
#include "StudentInfoController.h"
#include "CommonOperator.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
static STU_INFO *stu_info_table[BULLET_COUNT]; //存放了所有账户的哈希表
static int stu_info_count;
static char m_stu_info_file[CONFIGURE_FILE_LEN];
static jmp_buf m_jmp_buf;
//初始化学生信息表
void InitStuInfoTable(const char *stu_info_file, jmp_buf env_buf) {
  FILE *usr_act_fp = fopen(stu_info_file, "r");
  if (usr_act_fp == NULL) {
	perror("usr_account_file: ");
	longjmp(env_buf, -1);
  }

  stu_info_count = 0;
  memset(m_jmp_buf, 0, sizeof(int)*16);
  memset(m_stu_info_file,0,sizeof(stu_info_file));
  memset(stu_info_table, 0, sizeof(stu_info_table));

  memcpy(m_jmp_buf, env_buf, sizeof(int)*16);
  strcpy(m_stu_info_file, stu_info_file);
  STU_INFO tmp_info = {0};
  while (fscanf(usr_act_fp, "%s%s%d%d\n", tmp_info.stu_id, tmp_info.usr_name, &tmp_info.usr_course_id, &tmp_info.usr_course_score) != EOF) {
	++stu_info_count;
	STU_INFO *head = stu_info_table[hash(tmp_info.stu_id)];
	HeadInsertInList(&stu_info_table[hash(tmp_info.stu_id)], &tmp_info);
	memset(&tmp_info, 0, sizeof(tmp_info));
  }

  fclose(usr_act_fp);
}

//释放学生表
void FreeStuInfoTable() {
  for (int i = 0; i != BULLET_COUNT; ++i) {
	STU_INFO *head = stu_info_table[i];
	STU_INFO *to_be_deleted = head;
	while (head) {
	  head = head->pNext_Usr_Infor;
	  free(to_be_deleted);
	  to_be_deleted = head;
	}
	stu_info_table[i] = to_be_deleted = head = NULL;
  }
}

void HeadInsertInList(STU_INFO **ppHead, const STU_INFO *tmp_stu_info) {
  STU_INFO *pNew = (STU_INFO *)calloc(1, sizeof(STU_INFO));
  strcpy(pNew->stu_id, tmp_stu_info->stu_id);
  strcpy(pNew->usr_name, tmp_stu_info->usr_name);
  pNew->usr_course_id = tmp_stu_info->usr_course_id;
  pNew->usr_course_score = tmp_stu_info->usr_course_score;

  if (*ppHead == NULL) {
	*ppHead = pNew;
  } else {
	pNew->pNext_Usr_Infor = *ppHead;
	*ppHead = pNew;
  }
}
void DeleteStuInfoInList(STU_INFO **ppHead, STU_INFO *toBeDelete) {
  if (*ppHead == toBeDelete) {
	free(toBeDelete);
	*ppHead = toBeDelete = NULL;
	return;
  }
  STU_INFO *head = *ppHead;
  while (head->pNext_Usr_Infor != toBeDelete)
	head = head->pNext_Usr_Infor;
  head->pNext_Usr_Infor = toBeDelete->pNext_Usr_Infor;
  free(toBeDelete);
  toBeDelete = NULL;
}

STU_INFO *FindStuInfoInList(STU_INFO *pHead, const char *stu_id) {
  while (pHead) {
	if (!strcmp(stu_id, pHead->stu_id))
	  return pHead;
	pHead = pHead->pNext_Usr_Infor;
  }
  return NULL;
}

STU_INFO *FindStuInfo(const char stu_id[]) {
  STU_INFO *res_list = stu_info_table[hash(stu_id)];
  STU_INFO *res = FindStuInfoInList(res_list, stu_id);
  return res;
}

void ReBuildStuInfoTable() {
  SaveUpdateToStuInfoFile(m_stu_info_file);
  FreeStuInfoTable();
  InitStuInfoTable(m_stu_info_file, m_jmp_buf);
}

void AddStuInfo() {
  system("cls");
  printf("please input account info:   1. stu_id  2. name  3. course_id  4. course_score \n");
  STU_INFO stu_info = {0};
  printf("Input: ");
  scanf("%s%s%d%d", stu_info.stu_id, stu_info.usr_name, &stu_info.usr_course_id, &stu_info.usr_course_score);

  if (FindStuInfo(stu_info.stu_id) != NULL) {
	printf("Add failure, The account already exists!\n");
  } else {
	HeadInsertInList(&stu_info_table[hash(stu_info.stu_id)], &stu_info);
	printf("Add success!\n");
	++stu_info_count;
  }
  system("pause");
}
void DeleteStuInfo() {
  system("cls");
  printf("please input the stu_id: ");
  char stu_id[USR_ACT_LEN] = {0};
  gets(stu_id);
  STU_INFO *toBeDeleted = FindStuInfo(stu_id);
  if (toBeDeleted == NULL) {
	printf("Delete failure, The stu_id not exists!\n");
  } else {
	DeleteStuInfoInList(&stu_info_table[hash(stu_id)], toBeDeleted);
	printf("Delete success!\n");
	--stu_info_count;
  }
  system("pause");
}

void UpdateStuInfo() {
  system("cls");
  printf("please input the stu_id: ");
  char usr_act[STU_ID_LEN] = {0};
  gets(usr_act);
  STU_INFO *update_stu_info = FindStuInfo(usr_act);
  if (update_stu_info == NULL) {
	printf("Update failure, The stu_id not exists!\n");
	system("pause");
	return;
  }
  PrintSingleStuInfo(update_stu_info);
  printf("Select the modified information name： \n1.stu_id \t2.name \t3.course_id \t4.course_score\n");
  char command;
  while (command = getch()) {
	if (command >= '1' && command <= '4')
	  break;
	printf("\nerror command!");
  }
  printf("Enter the modified information：");
  switch (command) {
	case '1': {
	  char update_info[30] = {0};
	  gets(update_info);
	  if (FindStuInfo(update_info) == NULL) {
		strcpy(update_stu_info->stu_id, update_info);
		ReBuildStuInfoTable();
	  }
	  else {
		printf("Update failure, The stu_id  exists!\n");
		system("pause");
		return;
	  }
	}
	break;
	case '2': {
	  char update_info[30] = {0};
	  gets(update_info);
	  strcpy(update_stu_info->usr_name, update_info);
	}
	break;

	case '3': {
	  int update_info = 0;
	  scanf("%d", &update_info);
	  update_stu_info->usr_course_id = update_info;
	}
    break;

	case '4': {
	  int update_info = 0;
	  scanf("%d", &update_info);
	  update_stu_info->usr_course_score = update_info;
	}
	break;
	default:
	  break;
  }
  printf("Update success!\n");
  system("pause");
}



void SearchAllByName(const char *name) {
  for (int i = 0; i != BULLET_COUNT; ++i) {
	STU_INFO *pHead = stu_info_table[i];
	while (pHead) {
	  if (!strcmp(name, pHead->usr_name))
		printf("Account info : %s %s %d %d\n", pHead->stu_id, pHead->usr_name, pHead->usr_course_id, pHead->usr_course_score);
	  pHead = pHead->pNext_Usr_Infor;
	}
  }
}

void SearchStuInfo() {
  system("cls");
  printf("Select query mode： 1. by stu_id  2. by name  3. search all : \n");
  char command;
  while (command = getch()) {
	if (command >= '1' && command <= '3')
	  break;
	printf("error command!\n");
  }

  printf("Input the query info: ");
  switch (command) {

  case '1':
  {
	char query_info[30] = {0};
	gets(query_info);
	STU_INFO *query_res = FindStuInfo(query_info);
	PrintSingleStuInfo(query_info);
	system("pause");
  }
  break;
  case '2':
  {
	char query_info[30] = {0};
	gets(query_info);
	SearchAllByName(query_info);
	system("pause");
  }
  break;
  case '3':
	PrintAllStuInfo(stu_info_table, BULLET_COUNT, stu_info_count, CompareStuInfoById);
	break;
  default:
	break;
  }
}

int CompareStuInfoById(const void *left, const void *right) {
  STU_INFO **ppStuInfo1 = (STU_INFO **)left;
  STU_INFO **ppStuInfo2 = (STU_INFO **)right;

  return strcmp((*ppStuInfo1)->stu_id, (*ppStuInfo2)->stu_id) > 0;
}

int CompareStuInfoByScore(const void *left, const void *right) {
  STU_INFO **ppStuInfo1 = (STU_INFO **)left;
  STU_INFO **ppStuInfo2 = (STU_INFO **)right;

  return (*ppStuInfo1)->usr_course_score < (*ppStuInfo2)->usr_course_score;
}

void SaveUpdateToStuInfoFile(const char *stu_info_file) {
  STU_INFO **pStu_info_arr = (STU_INFO **)calloc(stu_info_count, sizeof(STU_INFO *));
  int cur_index = 0;
  for (int i = 0; i < BULLET_COUNT; ++i) {
	STU_INFO *head = stu_info_table[i];
	while (head) {
	  pStu_info_arr[cur_index++] = head;
	  head = head->pNext_Usr_Infor;
	}
  }
  qsort(pStu_info_arr, stu_info_count, sizeof(STU_INFO *), CompareStuInfoById);

  FILE *fp = fopen(stu_info_file, "w");
  for (int i = 0; i != stu_info_count; ++i)
	fprintf(fp, "%s %s %d %d\n", pStu_info_arr[i]->stu_id, pStu_info_arr[i]->usr_name,
	  pStu_info_arr[i]->usr_course_id, pStu_info_arr[i]->usr_course_score);

  free(pStu_info_arr);
  pStu_info_arr = NULL;
  fclose(fp);
}


//开始账号操作
void StartStuInfoOperator(const USR_ACCOUNT *cur_usr) {
  char command;
  if (cur_usr->usr_role == STU)
	while (1) {
	  system("cls");
	  PrintStuInfoOperatorList(cur_usr);
	  printf("Please input commad: \n");

	  command = getch();
	  switch (command) {
	  case '1':
	  {
		STU_INFO *my_info = FindStuInfo(cur_usr->usr_act);
		PrintSingleStuInfo(my_info);
		system("pause");
	  }
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
		PrintStuInfoOperatorList(cur_usr);
		printf("Please input commad: ");

		command = getch();
		switch (command) {
		case '1':
		  AddStuInfo();
		  break;
		case '2':
		  DeleteStuInfo();
		  break;
		case '3':
		  SearchStuInfo();
		  break;
		case '4':
		  UpdateStuInfo();
		  break;
		case '5':
		  PrintAllStuInfo(stu_info_table, BULLET_COUNT, stu_info_count, CompareStuInfoById);
		  break;
		case '6':
		  PrintAllStuInfo(stu_info_table, BULLET_COUNT, stu_info_count, CompareStuInfoByScore);
		  break;
		case '7':
		  return;
		default:
		  break;
		}
	  }

}

