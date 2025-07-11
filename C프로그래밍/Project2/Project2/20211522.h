//구조체 선언
typedef struct _major {
	char major[30];
	struct _major* link;
}MAJOR;
typedef struct _postcode {
	int postcode;
	struct _postcode* link;
}POSTCODE;
typedef struct _address {
	char address[20];
	struct _address* link;
}ADDRESS;
typedef struct _student {
	int std_id;
	char name[20];
	int year;
	struct _student* link;
	struct _major* m_link;
	struct _address* a_link;
	struct _postcode* p_link;
}STUDENT;

//함수 선언
void insert(STUDENT* s_head, MAJOR* m_head, ADDRESS* a_head, POSTCODE* p_head, STUDENT* new);
void delete(STUDENT* s_head, int n);
void search_neighbor(STUDENT* s_head, char name[], int n);
void search_family(STUDENT* s_head, char name[], int n);
void search_friend(STUDENT* s_head, char name[], int n);
void search_major(STUDENT* s_head, char name[], int n);
void change_major(STUDENT* s_head, MAJOR* m_head, char name[], int n, char m[]);