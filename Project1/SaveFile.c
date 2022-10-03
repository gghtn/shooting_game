#include "SaveFile.h"
extern int status[5];

// 스탯 파일에 저장
void SaveFile() {
	FILE* out;
	out = fopen("savefile.bin", "wb");
	if (out == NULL) return;
	fwrite(status, sizeof(int), 5, out);
	fclose(out);
}

// 스탯 정보 로드
void RoadFile(FILE* in) {
	fread(status, sizeof(int), 5, in);
}