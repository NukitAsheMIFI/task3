#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#define BUF_SIZE 64 

void plain(char **arr, size_t arr_size, int dir);
void lex(char **arr, size_t arr_size, int dir);
bool is_empty(char *token);

int main(int argc, char **argv){
	if (argc < 4){
		printf("недостаточно аргументов");
		return EXIT_SUCCESS;
	}
	char* in_name = argv[1];
	char* out_name = argv[2];
	char* mod = argv[3];

	FILE* in_file = fopen(in_name, "r");
	if (in_file == NULL){
		perror("fopen error");
		return EXIT_FAILURE;
	}
	FILE* out_file = fopen(out_name, "w");
	if (out_file == NULL){
		perror("fopen error");
		fclose(in_file);
		return EXIT_FAILURE;
	}

	fseek(in_file, 0, SEEK_END); //указатель в конец файла
	size_t len = ftell(in_file); //длина текста в файле
	rewind(in_file); //указатель в начало
	char *text = malloc(len + 1);
	size_t bytes_read = fread(text, 1, len, in_file); //считываем текст из файла
	if (bytes_read < len) {;
		perror("fread error");
		fclose(in_file);
		fclose(out_file);
		free(text);
		return EXIT_FAILURE;
	}

	text[len] = '\0'; //делаем строку
	
	size_t arr_size = 5;
	char **arr = malloc(arr_size * sizeof(char*));
	if (arr == NULL){
		perror("allocation memory error");
		fclose(in_file);
		fclose(out_file);
		free(text);
		return EXIT_FAILURE;
	}

	char *token = strtok(text, "\n"); //разделяем текст на слова по символу переноса строки
	int i = 0;
	while (token != NULL){
		if(!is_empty(token)){
			if (i + 1 > (int)arr_size){
				arr_size *= 2;
				char **temp_arr = realloc(arr, arr_size * sizeof(char*));
				if (temp_arr == NULL){
					perror("allocation memory error");
					fclose(in_file);
					fclose(out_file);
					free(text);
					return EXIT_FAILURE;
				}	
				arr = temp_arr;
			}
			arr[i++] = token;
		}
		token = strtok(NULL, "\n");
	}

	arr_size = i;
	if (i != 0) { //если все слова "пустые" realloc выдаст ошибку
		char **temp_arr = realloc(arr, arr_size * sizeof(char*));
		if (temp_arr == NULL){
			perror("allocation memory error");
			fclose(in_file);
			fclose(out_file);
			free(text);
			return EXIT_FAILURE;
		}	
		arr = temp_arr;
	}
	else{
		printf("все словы пустые\n");
		fclose(in_file);
		fclose(out_file);
		free(text);
		return EXIT_SUCCESS;
	}

	if (strcmp(mod, "plain") == 0) {
		plain(arr, arr_size, 1);
	} 
	else if(strcmp(mod, "rplain") == 0) {
		plain(arr, arr_size, -1);
	}
	else if(strcmp(mod, "lex") == 0){
		lex(arr, arr_size, 1);
	}
	else if(strcmp(mod, "rlex") == 0){
		lex(arr, arr_size, -1);
	}
	else {
		printf("неизвестная сортировка\n");
		fclose(in_file);
		fclose(out_file);
		free(arr);
		return EXIT_SUCCESS;
	}

	for (size_t i = 0; i < arr_size; i++){
		fprintf(out_file, "%s\n", arr[i]);
	}
	free(text);
	free(arr);
	fclose(in_file);
	fclose(out_file);
	return EXIT_SUCCESS;
	
}

void lex(char **arr, size_t arr_size, int dir){
	for (size_t i = 0; i + 1 < arr_size; i++) {
		for (size_t j = 0; j + 1 < arr_size - i; j++) {
			char *p1 = arr[j];
			char *p2 = arr[j + 1];
			while (*p1 != '\0' && *p2 != '\0'){
				int dif = tolower((unsigned char)*p1) - tolower((unsigned char)*p2); //unsigned char для символов кириллицы
				dif *= dir;
				if (dif > 0){ //слова нужно менять местами
					char *temp = arr[j];
					arr[j] = arr[j + 1];
					arr[j + 1] = temp;
					break;
				}
				else if (dif < 0){
					break;
				}
				p1++; //двигаем указатель дальше
				p2++;	
			}
		}
	}
}

void plain(char **arr, size_t arr_size, int dir){ //сортировка пузырьком с параметром направления
	for (size_t i = 0; i + 1 < arr_size; i++) {
		for (size_t j = 0; j + 1 < arr_size - i; j++) {
			size_t k = 0;
			while (arr[j][k] != '\0' && arr[j][k] == arr[j + 1][k]){	
				k++;
			}

			int swap = 0;

			if (dir == 1 && arr[j + 1][k] < arr[j][k]) {
				swap = 1;
			}
			else if (dir == -1 && arr[j][k] < arr[j + 1][k]) {
				swap = 1;
			}
			if (swap) {
				char *temp = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = temp;
			}
		}
	}
}

bool is_empty(char *token){
	while (*token) {
		if (!isspace((unsigned char)*token)){
			return false;
		}
		token++;
	}
	return true;
}


