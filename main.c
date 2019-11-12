#include "includes/malloc.h"
#include <unistd.h>

#define M (1024*1024)
void print(char *s)
{
	write(1, s, strlen(s));
}
typedef struct s_test
{
	// int a;
	// int b;
	int c;
	void *e;

} 				t_test;

int main()
{

	t_block a;

	char *test = malloc(3089);
	char *test2 = malloc(3089);
	free(test);

	show_alloc_mem_ex();

	// ft_printf(" c : %p -:- *c :%p", &(test.c), *((test.e) - 1));
// char *str, *ptr;
// 	if (!(str = malloc(10)))
// 		exit (128);
// 	printf("Malloc OK\n");
// 	if (!(ptr = realloc(str, -17)))
// 		return 0;
// 	printf("Realloc OK\n");
// addr = malloc(3000);
// addr = malloc(5000);
// free(NULL);
// free((void *)addr + 5);
// if (realloc((void *)addr + 5, 10) == NULL)
// 	print("Bonjour\n");


// char *test = malloc(10);
// char *test1 = malloc(50);
// char *test2 = malloc(60);
// free(test);
// free(test1);
// realloc(test2, 5096);
// free(test2);
// realloc(test2, 11000);


// 	show_alloc_mem_ex();

	// char *t1 =malloc(1024 * 32);
	// char *t2 =malloc(1024 * 1024);
	// char *t3=malloc(1024 * 1024 * 16);
//	char *t4 = malloc((size_t)(1024 * 1024 * 128 * 1024 * 1024 * 128 * 1024 * 1024 * 128* 1024 * 1024 * 128 + 1));
//	printf("%zu, %zu\n", sizeof(t_block), sizeof(t_zone));
	// char *test = malloc(1);
//char *test;
//	free(test);
//	free(test);

	// write(1, "lalalala\n", 9);
//	char *test1 = malloc(5000);

//	char *test2 = malloc(3000);
//	free(test);


	//  ft_realloc(test, 20);
	//  free(test);
	// // test[0] = 's';
	// // test[1] = 'a';
	// // test[2] = 'l';
	// // test[3] = 'u';
	// // test[4] = 't';
	// // test[5] = '\0';
	// char *test1 = malloc(10);
	// char *test2 = malloc(100);
	// show_alloc_mem();
	// ft_realloc(test, 50);

	//show_alloc_mem();
	//show_alloc_mem_ex();

	// char *test3 = malloc(1000);
	// char *test4 = malloc(3000);
	// char *test5 = malloc(3000);
	// char *test6 = malloc(2000);
	// char *test7 = malloc(2000);
	// char *test8 = malloc(200);
	// char *test9 = malloc(60);
	// free(test2);
	// free(test);
	// free(test2);
	// free(test6);
	// free(test11);
	// printf("block size %lu\n", METABLOCK_SIZE);
	// printf("size of pointer=%lu, size size_t=%lu, size int=%lu\n", sizeof(test3), sizeof(size_t), sizeof(int));
	// printf("zone size %lu\n", METAZONE_SIZE);
	// printf("page size =%d\n", getpagesize());
//	return((int)test);
}
