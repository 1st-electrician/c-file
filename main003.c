/*ヘッダファイルのインクルード*/
#include<stdio.h>
#include<stdlib.h>

/*マクロ定義*/
#define D_STRING_MAX 64
#define D_STRING_CHECK_RELEASE 1
#define D_FILE_COUNT_FUNCTION_RELEASE 1
#define D_SYSTEM_FUNCTION_RELEASE 1
#define D_SAME_FUNCTION_RELEASE 1
#define D_REMOVE_RELEASE 1 /*削除処理するときは0から1に変更して解放*/

/*文字列の比較を行う関数*/
/*引　数:比較する文字列情報*/
/*戻り値:文字列が等しければ０を返却*/
#if D_STRING_CHECK_RELEASE
int string_check(const char* s1,const char* s2)
{
  while(*s1==*s2){
    if((*s1==0x00)&&(*s2==0x00)){return 0;}
    s1++; s2++;
  }
  return (unsigned char)*s1-(unsigned char)*s2;
}
#endif

/*Linuxコマンドを実行、ファイルに書き出す関数*/
/*引　数:なし*/
/*戻り値:なし*/
#if D_FILE_COUNT_FUNCTION_RELEASE
void system_function()
{
  system("ls -1 > all_file_name.csv");
}
#endif

/*上記の関数(sysytem_function)で出力されたファイルの行数をカウントする関数*/
/*引　数:なし*/
/*戻り値:ファイルの中身の行数*/
#if D_SYSTEM_FUNCTION_RELEASE
int file_count_function()
{
  int file_count=0;
  int ret=0;
  FILE* all_file_pointer=NULL;
  char all_file_name[]="all_file_name.csv";
  char dummy_array[D_STRING_MAX];

  all_file_pointer=fopen(all_file_name,"r");
  if(all_file_pointer==NULL){printf("%sを開くことが出来ません\n",all_file_name); exit(1);}
  while((ret=fscanf(all_file_pointer,"%s",dummy_array))!=EOF){
    file_count++;
  }

  return file_count;
}
#endif

/*指定した文字列と同じサイズのファイルを削除する関数*/
/*引　数:ファイルの行数*/
/*戻り値:なし*/
#if D_SAME_FUNCTION_RELEASE
void same_file_size_remove_function(int file_count)
{
  FILE *file_pointer=NULL;
  char (*file_name)[D_STRING_MAX];
  file_name=(char(*)[D_STRING_MAX])malloc(sizeof(char*)*file_count*D_STRING_MAX);
  char input_name[D_STRING_MAX];
  unsigned int *file_size;
  file_size=(unsigned int*)malloc(sizeof(int)*file_count);
  int check_result=1;
  char all_file_name[]="all_file_name.csv";
  int count=0;
  int ret=0;

  file_pointer=fopen(all_file_name,"r");
  if(file_pointer==NULL){printf("%s:読み取り不可\n",all_file_name); exit(1);}
  while((ret=fscanf(file_pointer,"%s",file_name[count]))!=EOF){count++;}
  fclose(file_pointer);
  /*
  for(int i=0;i<file_count;i++){
    printf("%09d: %s\n",i+1,file_name[i]);
  }
  */
  for(int i=0;i<file_count;i++){
    file_pointer=fopen(file_name[i],"r");
    if(file_pointer==NULL){printf("%sを開くことが出来ません\n",file_name[i]); exit(1);}
    fseek(file_pointer,0,SEEK_END);
    file_size[i]=ftell(file_pointer);
    fclose(file_pointer);
  }

  for(int i=0;i<file_count;i++){
    printf("%09d: %s \tdata-size:  \x1b[7m%d[Byte]\x1b[0m\n",i+1,file_name[i],file_size[i]);
  }

  puts("入力したファイル名と同じサイズのファイルを削除");
  puts("残したいファイル名を入力してください");

  int k=0;
  int remove_number;
  do{
    printf("\x1b[35mファイル名を入力:\x1b[39m");
    scanf("%s",input_name);
    while((k<file_count)&&check_result){
      #if D_STRING_CHECK_RELEASE
      check_result=string_check(file_name[k],input_name);
      #endif
      check_result?k+=0:printf("\x1b[32m文字列一致\x1b[39m\n"); remove_number=k;
      k++;
    }
    k=0;
  }while(check_result);

  for(int i=0;i<file_count;i++){
    if(remove_number==i){continue;}
    if(file_size[remove_number]==file_size[i]){
      puts("削除");
      #if D_REMOVE_RELEASE
      //remove(file_name[i]);
      #endif
    }
  }
  printf("\x1b[36m削除処理完了\n");
}
#endif

/*C言語で最初に実行されるmain関数*/
/*引　数:なし*/
/*戻り値:OSに終了状態を伝達*/
int main(void)
{
  int file_count;
  char yes[]="YES";
  char no[]="NO";
  char yes_or_no[16];
  

  if(D_REMOVE_RELEASE){
    printf("\x1b[31m\x1b[7m自己責任でお願いします\n本当に実行しますか? [YES] or [NO]\n");
    do{
      printf("\x1b[31m\x1b[7m大文字で入力:");
      printf("\x1b[39m\x1b[0m");
      scanf("%s",yes_or_no);
      if((no[0]==yes_or_no[0])&&(no[1]==yes_or_no[1])&&(no[2]==yes_or_no[2]))
	{return 0;}
    }while(string_check(yes,yes_or_no));
  } 

#if D_SYSTEM_FUNCTION_RELEASE
  system_function();
#endif

#if D_FILE_COUNT_FUNCTION_RELEASE
  file_count=file_count_function();
  printf("このディレクトリのファイル数:%d個\n",file_count);
#endif
  
#if D_SAME_FUNCTION_RELEASE
  same_file_size_remove_function(file_count);
#endif
  return 0;
}
