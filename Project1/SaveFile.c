#include "SaveFile.h"
extern int status[5];

// ���� ���Ͽ� ����
void SaveFile() {
	FILE* out;
	out = fopen("savefile.bin", "wb");
	if (out == NULL) return;
	fwrite(status, sizeof(int), 5, out);
	fclose(out);
}

// ���� ���� �ε�
void RoadFile(FILE* in) {
	fread(status, sizeof(int), 5, in);
}