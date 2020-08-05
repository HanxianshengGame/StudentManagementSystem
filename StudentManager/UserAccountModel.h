#pragma once
#define USR_ACT_LEN 30
#define USR_PWD_LEN 30

typedef enum {
  NULL_ROLE,
  ADMIN,
  STU
}USR_ROLE;

typedef struct usr_account_tag {
  char usr_act[USR_ACT_LEN];   //—ß∫≈
  char usr_pwd[USR_PWD_LEN];   //√‹¬Î
  USR_ROLE usr_role; 
  struct usr_account_tag *pNext_Usr_Account;
}USR_ACCOUNT, *pUSR_ACCOUNT;
