#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#define SIZE_MAX 50
#define PATH "rsc/dico.dic"

struct s_book
{
	unsigned int size;
	char (*words)[SIZE_MAX];
};

struct s_book *freeDico(struct s_book *book);
FILE *loadFile(const char path[]);
struct s_book *loadBook(void);
int loadWords(struct s_book *const book, FILE *const file);
char *getWord(const struct s_book *const book);

void play(const char word[]);
int update(const char word[], char answer[], const char letter, const size_t len_word);
int replay(void);

double random(void);
int randint(const int a, const int b);

char getLetter(void);
void clearStdin(void);

int main(void)
{
	struct s_book *book = loadBook();
	srand((unsigned int)(time(NULL)));
	if(NULL != book)
	{
		do
		{
			const char *const word = getWord(book);
			play(word);
		}while(replay());
		book = freeDico(book);
	}
	return 0;
}

double random(void) 
{
	return (double) rand() / RAND_MAX;
}

int randint(const int a, const int b)
{
	return (int)(a + (b - a) * random());
}

int replay(void)
{
	int tmp = 0;
	do
	{
		printf("\n\nVoulez vous recommencer ?\n"
		       "1. Continuer\n"
		       "2. Quitter\n");
	   scanf("%d", &tmp);
	   clearStdin();
	}while(tmp != 1 && tmp != 2);
	return tmp == 1;
}

struct s_book *freeDico(struct s_book *book)
{
	free(book->words);
	free(book);
	return NULL;
}

int update(const char word[], char answer[], const char letter, const size_t len_word)
{
	int success = 0;
	size_t i = 0;
	for(i = 0; i < len_word; i++)
	{
		if(letter == word[i])
		{
			answer[i] = letter;
			success = 1;
		}
	}
	return success;
}

void play(const char word[])
{
	char answer[SIZE_MAX] = {0}, letter = 0; 
	size_t len_word = strlen(word), attempts = 10;
	memset(answer, '*', len_word * sizeof(char)); /* Peut être remplacé par une boucle for */
	while(attempts > 0 && strcmp(answer, word))
	{
		printf("\nIl vous reste %u essai%s\nLe mot est %s.\nEntrez une lettre : ",  
		       attempts, attempts == 1 ? "." : "s.", answer);
		letter = (char) toupper(getLetter());
		if(update(word, answer, letter, len_word))
			printf("Bravo, %c est bien dans le mot.\n", letter);
		else
			attempts --;
	}
	if(attempts > 0)
		printf("Bravo, vous avez gagne en %u essai%s, le mot etait bien %s.\n", 
		       10 - attempts, attempts == 10 ? "." : "s.", word);
	else
		printf("Vous avez perdu, looser. Le mot etait %s.", word);
}

void clearStdin(void)
{
	int c;
	do
	{
		c = getchar();
	}while (c != '\n' && c != EOF);
}

char getLetter(void)
{
	char c = 0;
	while(!scanf("%[a-zA-Z]c",&c))
	{
		clearStdin();
		printf("Erreur, votre saisie est invalide. Recommencez :");
	}
	clearStdin();
	return c;
}

FILE *loadFile(const char path[])
{
	FILE *file = fopen(path, "r");
	if(NULL == file)
		perror("Erreur chargement file dictionnaire : ");
	return file;
}

struct s_book *loadBook(void)
{
	struct s_book *book = malloc(sizeof(book));
	if(NULL == book)
		perror("Erreur malloc struct s_book : ");
	else
	{
		book->words = NULL;
		FILE *file = loadFile(PATH);
		if(NULL == file)
			book = freeDico(book);
		else
		{
			if(loadWords(book, file) < 0)
				book = freeDico(book);
			fclose(file);
		}
	}
	return book;
}

int loadWords(struct s_book *const book, FILE *const file)
{
	size_t i = 0;
	fscanf(file, "%u%*[^\n]", &book->size);
	book->words = malloc(book->size * sizeof(*book->words));
	if(book->words == NULL)
	{
		perror("Erreur malloc : ");
		return -1;
	}
	for(i = 0; i < book->size; i++)
		fscanf(file, "%100s%*[^\n]", book->words[i]);
	return 0;
}

char *getWord(const struct s_book *const book)
{
	int i = randint(0, book->size);
	return book->words[i];
}
