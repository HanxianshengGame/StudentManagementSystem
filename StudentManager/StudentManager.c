#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif // !_CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<setjmp.h>
#include <string.h>
#include <stdlib.h>
#include "UserAccountController.h"
#include "StudentInfoController.h"

static USR_ACCOUNT cur_usr;
static char usr_account_file[CONFIGURE_FILE_LEN] = {0};
static char stu_info_file[CONFIGURE_FILE_LEN] = {0};

//配置文件初始化
void InitConfigure(const char *configure_file, jmp_buf env_buf) {
  FILE *fp = fopen(configure_file, "rb");
  if (!fp) {
	perror("configure_file: ");
	longjmp(env_buf, -1);
  }

  fscanf(fp, "%s", usr_account_file);
  fscanf(fp, "%s", stu_info_file);
  
  fclose(fp);
  InitUsrAccountTable(usr_account_file, env_buf);
  InitStuInfoTable(stu_info_file, env_buf);
}
//登录
void Register() {
  system("cls");
  char usr_act[USR_ACT_LEN];
  char usr_pwd[USR_PWD_LEN];

PARDON_INPUT_ACT:
  memset(usr_act, 0, sizeof(usr_act));
  memset(usr_pwd, 0, sizeof(usr_pwd));
  
  printf("account: ");
  gets(usr_act);
  printf("password: ");
  for (int i = 0; i < USR_PWD_LEN; ++i) {
	usr_pwd[i] = getch();
	switch (usr_pwd[i]) {
	case '\b':
	  if (i >= 1) {
		printf("\b \b");
		usr_act[i--] = 0;
		usr_pwd[i--] = 0;
	  } else {
		usr_pwd[i--] = 0;
	  }
	  break;
	case '\r':
	  usr_pwd[i] = 0;
	  printf("\n");
	  goto PWD_OVER;
	default:
	  printf("%c", '*');
	  break;
	}
  }

PWD_OVER:;
  memset(&cur_usr, 0 ,sizeof(USR_ACCOUNT));
  USR_ROLE usr_role = GetUsrRole(usr_act, usr_pwd, &cur_usr); //用户权限已经确定
  if (usr_role == NULL_ROLE) {
	printf("Incorrect account or password, Press any key to retype  -。- \n");
	system("pause");
	system("cls");
	goto PARDON_INPUT_ACT;
  }
  printf("register success!\n");
  system("pause");
}

//展现管理界面
void OnTheView() {
  printf("\n\n\n\n");
  printf("\t\t***************************************************\n");
  printf("\t\t********   Welcome to the system view   ***********\n");
  printf("\t\t********                                ***********\n");
  printf("\t\t********     1. Account      Operator   ***********\n");
  printf("\t\t********     2. Information  Operator   ***********\n");
  printf("\t\t********     3. Quit&Save    Operator   ***********\n");


}

//运行系统（主）
void StartSystemManage(const char *configure_file, jmp_buf env_buf) {

  InitConfigure(configure_file, env_buf);
  Register();

  
  while (1) {
	system("cls");
	OnTheView();
	printf("Please input commad: ");
	char command = getch();
	switch (command) {
	case '1':
	  StartAccountOperator(&cur_usr);
	  break;
	case '2':
	  StartStuInfoOperator(&cur_usr);
	  break;
	case '3':
	  SaveUpdateToActFile(usr_account_file);
	  SaveUpdateToStuInfoFile(stu_info_file);
	  FreeActTabele();
	  FreeStuInfoTable();
	  return;
	default:
	  break;
	}
  }
  
}


int main(int argc, char **argv)
{
  jmp_buf env_buf;
  int return_val = setjmp(env_buf);
  if (return_val == -1) 
	return -1;
  StartSystemManage(argv[1], env_buf);

  printf("bye bye!!\n");
  return 0;
}