#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<conio.h>
#include<stdlib.h>
#include<string.h>

typedef struct phno{
	char type[10];
	char number[12];
};
typedef struct record{
	char fname[10];
	char lname[10];
	int age;
	float ht;
	phno p[2];
};

int equals(char *s1, char *s2);
void lastname(FILE *fp, record *rec);
void firstname(FILE *fp, record *rec);
void age(FILE *fp, record *rec);
void height(FILE *fp, record *rec);
void phonenum(FILE *fp, phno *p);
void phnodet(FILE *fp, phno *p);
void phonetype(FILE *fp, phno *p);
void readKey(FILE *fp, record *rec);
int parse(char *file,record **rec);
void readDet(FILE *fp, record *rec);
void getDetails(record **rec, int cnt);

void main()
{
	char filename[20]; 
	int each=0,i=0,cnt = 0;
	record **rec=NULL;
	printf("\n\nEnter Input filename: ");
	scanf("%s",filename);
	rec = (record**)malloc(sizeof(record*));
	cnt = parse(filename,rec);
	printf("\n\n");
	while (each < cnt){
		printf("Fname:%s  Lname:%s  Age:%d  Ht:%f\n ", rec[each]->fname, rec[each]->lname, rec[each]->age, rec[each]->ht);
		for (i = 0; i < 2; i++)
			printf("\n%s-%s\n", rec[each]->p[i].type, rec[each]->p[i].number);
		printf("\n\n");
		each++;
	}
	getDetails(rec, cnt);
	_getch();
}

void getDetails(record **rec, int cnt)
{
	char fname[20];
	int each,i;
	printf("\nEnter: ");
	scanf("%s",fname);
	for (each = 0; each < cnt; each++)
	{
		if (strcmp(fname, rec[each]->fname) == 0)
		{
			for (i = 0; i < 2;i++)
			   printf("\n%s : %s", rec[each]->p[i].type, rec[each]->p[i].number);
		}
	}
}

int parse(char *file,record **srt)
{
	FILE *fp;
	char ch;
	record *rec=NULL;
	int c,i,first = 1,cnt = 0,each=0;
	fp = fopen(file, "r");
	while (!feof(fp)){
		if ((ch = fgetc(fp)) && ch == '\n')
		{
			continue;
		}
		ungetc(ch, fp);
		if ((ch = fgetc(fp)) && ch != '{'){
			break;
		}
		    srt[cnt] = (record*)malloc(sizeof(record));
			rec = (record*)malloc(sizeof(record) * 1);
			srt[cnt] = rec;
			cnt++;
		    readDet(fp, rec);
		}
	while (each < cnt){
		printf("Fname:%s  Lname:%s  Age:%d  Ht:%f\n ", srt[each]->fname, srt[each]->lname, srt[each]->age, srt[each]->ht);
		for (i = 0; i < 2; i++)
			printf("\n%s-%s\n", srt[each]->p[i].type, srt[each]->p[i].number);
		printf("\n\n");
		each++;
	}
	return cnt;
}
void readDet(FILE *fp, record *rec)
{
	char c;
	while ((c = fgetc(fp)) && c != '}')
	{
		ungetc(c, fp);
		readKey(fp, rec);
	}
}
void readKey(FILE *fp,record *rec)
{
	int c,i,count;
	char key[20];
	while ((c = fgetc(fp)!= '"'));
	for (count = 0; (c = fgetc(fp)) && c != '"'; count++)
	{
		key[count] = (char)c;
	}
	key[count] = '\0';
	if (equals(key, "FirstName\0"))
	{
		firstname(fp,rec);
	}
	else if (equals(key, "LastName\0"))
		lastname(fp, rec);
	else if (equals(key, "age\0"))
		age(fp, rec);
	else if (equals(key, "height\0"))
		height(fp, rec);
	else if (equals(key, "phoneno\0"))
	{
		phnodet(fp, rec->p);
		while ((c = fgetc(fp)) && c != '}');
		ungetc(c, fp);
	}
}
void phoneno(FILE *fp, record *rec)
{
	int c, i=0, count;
	char key[20];
	while ((c = fgetc(fp)) && c != '{');
	printf("\n%d\n", rec->p);
	phnodet(fp, rec->p);

}
void phnodet(FILE *fp, phno *p)
{
	int c, i = 0, count;
	char key[20];
	printf("\n%d\n", p);
	while ((c = fgetc(fp)) && c != '{');
	while ((c = fgetc(fp)) && c != ']')
	{
		ungetc(c, fp);
		while ((c = fgetc(fp)) && c != '}'){
			ungetc(c, fp);
		while ((c = fgetc(fp)) && c != '"');
		for (count = 0; (c = fgetc(fp)) && c != '"'; count++)
		{
			key[count] = (char)c;
		}
		key[count] = '\0';
		if (equals(key, "type\0"))
		{
			printf("%d\n", &p[i]);
			phonetype(fp,&p[i]);
			
		}
		else if (equals(key, "number\0"))
			phonenum(fp, &p[i]);
	}
		i++;
   }

}
void phonetype(FILE *fp, phno *p)
{
	printf("%d\n", *p);
	int i, c;
	while ((c = fgetc(fp)) != '"');
	for (i = 0; (c = fgetc(fp)) != '"'; i++)
		p->type[i] = c;
	p->type[i] = '\0';
}
void phonenum(FILE *fp, phno *p)
{
	int i, c;
	while ((c = fgetc(fp)) != '"');
	for (i = 0; (c = fgetc(fp)) != '"'; i++)
		p->number[i] = c;
	p->number[i] = '\0';
}
void height(FILE *fp, record *rec)
{
	int i, c;
	float f;
	while ((c = fgetc(fp)) != ':');
	fscanf(fp, "%f", &f);
	rec->ht = f;
}
void age(FILE *fp, record *rec)
{
	int i, c;
	while ((c = fgetc(fp)) != ':');
	fscanf(fp, "%d", &c);
	rec->age = c;
}
void firstname(FILE *fp, record *rec)
{
	int i,c;
	while ((c = fgetc(fp)) != '"');
	for (i = 0; (c = fgetc(fp)) != '"'; i++)
		rec->fname[i] = c;
	rec->fname[i] = '\0';
}
void lastname(FILE *fp, record *rec)
{
	int i, c;
	while ((c = fgetc(fp)) != '"');
	for (i = 0; (c = fgetc(fp)) != '"'; i++)
		rec->lname[i] = c;
	rec->lname[i] = '\0';
}
int equals(char *s1,char *s2)
{
	int i;
	for (i = 0; s2[i]; i++)
	{
		if (s1[i] != s2[i])
			return 0;
	}
	return 1;
	
}