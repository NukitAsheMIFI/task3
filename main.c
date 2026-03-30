#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUF_SIZE 64 

void plane(char **arr, size_t arr_size, int dir);
void lex(char **arr, size_t arr_size, int dir);

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

	char *token = strtok(text, "\n");
	int i = 0;
	while (token != NULL){

		if (i + 1 > (int)arr_size){
			arr_size *= 2;
			char **temp_arr = realloc(arr, arr_size *  sizeof(char*));
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
		token = strtok(NULL, "\n");
	}

	arr_size = i;
	char **temp_arr = realloc(arr, arr_size *  sizeof(char*));
	if (temp_arr == NULL){
		perror("allocation memory error");
		fclose(in_file);
		fclose(out_file);
		free(text);
		return EXIT_FAILURE;
	}	
	arr = temp_arr;

	if (strcmp(mod, "plane") == 0) {
		plane(arr, arr_size, 1);
	} 
	else if(strcmp(mod, "rplane") == 0) {
		plane(arr, arr_size, -1);
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
		fprintf(out_file, "%s ", arr[i]);
	}
	free(text);
	fclose(in_file);
	fclose(out_file);
	return EXIT_SUCCESS;
	
}

void lex(char **arr, size_t arr_size, int dir){
	for (size_t i = 0; i + 1 < arr_size; i++) {
		for (size_t j = 0; j + 1 < arr_size - i; j++) {
			int res = strcmp(arr[j], arr[j+1]);
			res *= dir;
			if (res > 0){
				char *temp = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = temp;		
			}
		}
	}
}

void plane(char **arr, size_t arr_size, int dir){ //сортировка пузырьком с параметром направления
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
