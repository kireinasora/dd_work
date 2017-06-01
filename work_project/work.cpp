#include <stdio.h>
#include <string.h>
#include <windows.h>

int new_Names(char oldNames[][MAX_PATH], char newNames[][MAX_PATH], int num_of_files, char curDir[MAX_PATH]);

int main(void) {
	char curDir[MAX_PATH];
	char firstArg_FindFirstFile[MAX_PATH];
	char oldNames[500][MAX_PATH] = { 0 };
	char newNames[500][MAX_PATH] = { 0 };
	int i;
	int num_of_files;

	HANDLE hFile;
	WIN32_FIND_DATA FileData;

	GetModuleFileName(NULL, curDir, MAX_PATH);
	char *point = curDir + strlen(curDir) - 1;
	while (point[0] != '\\') point--;
	point[1] = '\0';
	strcpy_s(firstArg_FindFirstFile, curDir);
	strcat_s(firstArg_FindFirstFile, "*.*");


	hFile = FindFirstFile(firstArg_FindFirstFile, &FileData);
	if (hFile == INVALID_HANDLE_VALUE) {
		printf("Cannot find any files!\n");
		return 0;
	}

	i = 0;
	while (FindNextFile(hFile, &FileData)) {
		if (FileData.cFileName[0] == '.')
			continue;
		strcpy_s(oldNames[i], FileData.cFileName);
		i++;
	}
	num_of_files = i;

	new_Names(oldNames, newNames, num_of_files, curDir);
	


	printf("\nPress Any Key to Exit\n");
	getchar();
	return 0;
}


int new_Names(char oldNames[][MAX_PATH], char newNames[][MAX_PATH], int num_of_files, char curDir[MAX_PATH]) {
	int i, j, k, string_length, new_file_number = 0;
	char *point;
	char tmp[MAX_PATH];
	char tmp1[MAX_PATH];
	char *extension=0;
	char pre_newNames[500][MAX_PATH];
	char ans;
	int dup = 0;

	j = 0;
	for (i = 0; i < num_of_files; i++) {
		string_length = strlen(oldNames[i]);
		extension = &oldNames[i][string_length - 1];
		while(extension[0] != '.')
			extension--;

		point = &oldNames[i][0];
		
		while (point[4] != '\0' && point[0]!= '\0' &&point < &oldNames[i][0]+MAX_PATH) {
			if (point[0] == 'E' && isdigit(point[1]) && point[2] == '-' && isdigit(point[3]) && isdigit(point[4])) {
				for (k = 0; k < 5; k++)
					newNames[j][k] = point[k];
				strcat_s(newNames[j], extension);
				strcpy_s(pre_newNames[j], oldNames[i]);
				new_file_number++;
				j++;
			}
			point+=1;
		}
	}

	for (i = 0; i < new_file_number; i++) {
		for (j = i + 1; j < new_file_number; j++) {
			if (!strcmp(newNames[i], newNames[j])) {
				printf("Error!\n\n");
				printf("%s\n", pre_newNames[i]);
				printf("AND\n");
				printf("%s\n\n\n", pre_newNames[j]);
				printf("***THESE TWO FILES WILL HAVE THE SAME NAMES***\n\n");
				printf("\"%s\"\n", newNames[i]);
				return 0;
			}
		}
	}


	printf("Are you sure to rename the following files:\n\n");
	for (i = 0; i < new_file_number; i++) {
		printf("    %-80s    --->    %-20s\n", pre_newNames[i], newNames[i]);
	}
	printf("\n\nPlease enter Y/N:\n");
	scanf_s("%c", &ans);
	switch (ans) {
		case 'Y': {
			for (i = 0; i < new_file_number; i++) {
				strcpy_s(tmp, curDir);
				strcpy_s(tmp1, curDir);
				strcat_s(tmp, pre_newNames[i]);
				strcat_s(tmp1, newNames[i]);
				rename(tmp, tmp1);
			}
			printf("Changed!\n");
			break;
		}
		case 'N':
			printf("See you!\n");
			break;

		default:
			printf("What do you want?!\n");
			break;
	}
	return 0;
}