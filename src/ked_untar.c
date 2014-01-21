#include<stdio.h>
#include<string.h>
#include<math.h>

struct tar_header{
	char filename[100];
	char file_mode[8];
	char owner_id[8];
	char group_id[8];
	char file_size[12];
	char last_modified[12];
	char checksum[8];
	char type_flag[1];
	char something[100];
	char ustar[6];
	char ustar_version[2];
	char owner_name[32];
	char group_name[32];
	char device_major_number[8];
	char device_minor_number[8];
	char filename_prefix[155];
};

int main(int argv, char* argc[]){
	if(argv < 2){
		printf("argv = %d\n", argv);
		return 1;
	}
	char* filename = argc[1];
	if(strlen(filename) >= 100){
		printf("Filename must not exceed 100 characters.");
		return 1;
	}

	char directory[200] = "./";
	if(argv > 2){
		strcpy(directory, argc[2]);
		//should check that dir exists/is writeable	
	}
	
	FILE* handle = fopen(filename, "r");
	if(handle == NULL){
		printf("Could not open file '%s'.\n", filename);
		return -1;
	}
	int offset = 0;
	while(feof(handle) == 0){	
		struct tar_header header;
		printf("offset = %d\n", offset);	
		fseek(handle, offset, SEEK_SET);
		fread(&header, sizeof(char), sizeof(struct tar_header), handle);
		fseek(handle, offset+512, SEEK_SET);
	
		int filesize;
		sscanf(header.file_size, "%o", &filesize);	
		
		char contents[filesize];
		fread(contents, sizeof(char), filesize, handle);
		if(strlen(header.filename) > 0){
			char filepath[300];
			sprintf(filepath, "%s/%s", directory, header.filename);	
			FILE* whandle = fopen(filepath, "w");
			fwrite(contents, sizeof(char), filesize, whandle);
			fclose(whandle);
	
			printf("Extracted file %s to %s\n", header.filename, directory);
		}
		int size_mult = ceil(filesize / 512.0);
		offset += 512 + 512*size_mult;
	}
	
	fclose(handle);

	return 0;
}
