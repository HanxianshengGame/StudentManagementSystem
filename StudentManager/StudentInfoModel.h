#pragma once
#define STU_ID_LEN 30
#define STU_NAME_LEN 30

typedef char id_type;
typedef char name_type;
typedef int course_id_type;
typedef int score_type;
typedef struct student_info_tag *pointer_type;

typedef struct student_info_tag { 

  id_type   stu_id[STU_ID_LEN];     //”Î user_account œ‡Õ¨
  name_type usr_name[STU_NAME_LEN];
  course_id_type usr_course_id;
  score_type usr_course_score;
  pointer_type pNext_Usr_Infor;

}STU_INFO, *pSTU_INFO;