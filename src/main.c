/**
 * \file main.c
 * \brief Petit jeu du pendu
 * \author Karnaj
 *
 * Jeu du pendu en langage C. Le programme choisit un mot au hasard que le
 * joueur doit deviner en proposant des lettres (il a droit à 10 mauvaises
 * lettres). Un fichier « dico.dic » doit être placé dans un dossier « rsc»
 * à côté de l’exécutable. Celui-ci doit contenir un mot par ligne (chaque
 *  mot d’au plus 50 caractères), la première ligne contenant le nombre de
 * mots du fichier.
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
/**
 * \def SIZE_MAX Taille maximale d’un mot de la structure s_book.
 */
#define SIZE_MAX 50
/**
 * \def PATH Chemin du dictionaire à charger.
 */
#define PATH "rsc/dico.dic"

/**
 * \struct s_book
 * \brief Objet représentant un dictionnaire
 *
 * s_book est une structure contenant une liste de mots (de taille
 * maximale `SIZE_MAX`) et le nombre de mots de cette liste.
 */
struct s_book
{
	unsigned int size;
	char (*words)[SIZE_MAX + 1];
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

/**
 * \fn int main (void)
 * \brief Entrée du programme. Lance le jeu du pendu
 *
 * \return 0 - Arrêt normal du programme.
 */
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

/**
 * \fn double random(void)
 * \brief Tire au hasard un nombre entre 0 et 1.
 * \return Un flottant entre 0 et 1.
 */
double random(void)
{
	return (double) rand() / RAND_MAX;
}

/**
 * \fn int randint(const int a, const int b)
 * \brief Tire au hasard un entier entre a et b.
 * \param a Borne inférieure.
 * \param b Borne supérieure.
 * \return Un entier entre a et b.
 */
int randint(const int a, const int b)
{
	return (int)(a + (b - a) * random());
}

/**
 * \fn int replay(void)
 * \brief Demande à l’utilisateur s’il veut rejouer.
 * \return Une valeur non nulle si l’utilisateur veut rejouer, 0 sinon.
 */
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

/**
 * \fn struct s_book *freeDico(struct s_book *book)
 * \brief Libère une structure s_book allouée dynamiquement.
 * \param book Un pointeur sur la structure s_book allouée dynamiquement.
 * \return NULL.
 */
struct s_book *freeDico(struct s_book *book)
{
	free(book->words);
	free(book);
	return NULL;
}

/**
 * \fn int update(const char word[], char answer[], const char letter, const size_t len_word)
 * \brief Met à jour le mot-réponse de l’utilisateur en fonction d’une lettre et vérifie que cette
 *        lettre est bien dans le mot référent.
 * \param word Le mot référent, celui que l’utilisateur doit trouver.
 * \param answer Le mot-réponse de l’utilisateur.
 * \param letter La lettre a vérifier.
 * \param len_word La longueur du mot à vérifier.
 * \return NULL.
 */
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

/**
 * \fn void play(const char word[])
 * \brief Fonction de jeu du pendu.
 * \param word Le mot à trouver.
 *
 * La fonction prend en paramètre un mot que l’utilisateur doit trouver. À chaque tour,
 * il propose une nouvelle lettre jusqu’à avoir trouvé toutes les lettres du mot. Il a droit
 * à 10 échecs.
 */
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

/**
 * \fn void clearStdin(void)
 * \brief Vide le buffer.
 */
void clearStdin(void)
{
	int c;
	do
	{
		c = getchar();
	}while (c != '\n' && c != EOF);
}

/**
 * \fn char getLetter(void)
 * \brief Demande à l’utilisateur une lettre de l’alphabet.
 *
 * \return Une lettre de l’alphabet.
 *
 * La fonction redemande à l’utilisateur de rentrer une lettre tant que sa saisie
 * n’est pas valide.
 */
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

/**
 * \fn FILE *loadFile(const char path[])
 * \brief Ouvre le fichier dont le chemin est path.
 * \param path Chemin du fichier à ouvrir.
 * \return Un pointeur sur FILE correspondant au fichier ouvert (NULL en cas d’erreur).
 */
FILE *loadFile(const char path[])
{
	FILE *file = fopen(path, "r");
	if(NULL == file)
		perror("Erreur chargement file dictionnaire ");
	return file;
}

/**
 * \fn struct s_book *loadBook(void)
 * \brief Charge un dictionnaire depuis le fichier dont le chemin est PATH.
 * \return Un pointeur sur la structure s_book créée (NULL en cas d’erreur).
 */
struct s_book *loadBook(void)
{
	struct s_book *book = malloc(sizeof(book));
	if(NULL == book)
		perror("Erreur malloc struct s_book ");
	else
	{
		FILE *file = loadFile(PATH);
		book->words = NULL;
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

/**
 * \fn int loadWords(struct s_book *const book, FILE *const file)
 * \brief Charge les mots d’un fichier dans une structure s_book.
 * \param book Pointeur sur la structure s_book dans laquelle les mots doivent être chargés.
 * \param file Correspond au fichier contenant les mots.
 * \return 0 en cas de succès et -1 en cas d’erreur.
 *
 * La fonction suppose que le fichier est formaté de la manière suivante :
 *
 * - le nombre de mots en première ligner du fichier ;
 * - puis un mot par ligne.
 */
int loadWords(struct s_book *const book, FILE *const file)
{
	size_t i = 0;
	fscanf(file, "%u%*[^\n]", &book->size);
	book->words = malloc(book->size * sizeof(*book->words));
	if(book->words == NULL)
	{
		perror("Erreur malloc ");
		return -1;
	}
	for(i = 0; i < book->size; i++)
		fscanf(file, "%50s%*[^\n]", book->words[i]);
	return 0;
}

/**
 * \fn char *getWord(const struct s_book *const book)
 * \brief Tire un mot au hasrd dans un objet s_book.
 * \param book Pointeur sur la structure s_book dans laquelle tirer le mot.
 * \return Le mot tiré.
 */
char *getWord(const struct s_book *const book)
{
	int i = randint(0, (int)book->size);
	return book->words[i];
}
