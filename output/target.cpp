
#include <Windows.h>
#include <stdio.h>
#include <stdint.h>
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include <chrono>

#define bitlen 3

/*
LONG64 makeSeed() {
	auto current = std::chrono::system_clock::now();
	auto duration = current.time_since_epoch();
	auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();

	return millis;
}

bool saveSeed(LONG64 seed) {
	// Todo : exception processing
	std::stringstream ss;
	ss << seed << std::endl;
	std::string str = ss.str();

	std::ofstream fSeed;
	fSeed.open("Z:\Seed903seed.txt", std::ios::app);
	fSeed.write(str.c_str(), str.size());
	fSeed.close();

	return true;
}
*/
PALETTEENTRY* paletteentryList[10];
BITMAPINFOHEADER* bitmapinfoheaderList[10];
RGBQUAD* rgbquadList[10];
BITMAPINFO* bitmapinfoList[10];
CHARSETINFO* charsetinfoList[10];
LOGFONTW* logfontwList[10];
LOGFONTA* logfontaList[10];
DEVMODEA* devmodeaList[10];
AXISINFOA* axisinfoList[10];
AXESLISTA* axeslistaList[10];
ENUMLOGFONTEXDVW* enumlogfontexdvwList[10];
LOGBRUSH* logbrushList[10];
POINT* lppointList[10];
SIZE* sizeList[10];
RECT* rectList[10];
LOGPEN* logpenList[10];
POINT* pointList[10];
void* ptrList[10];
FONTSIGNATURE* fontsignatureList[10];
TEXTMETRICA* TEXTMETRICAList[10];

HANDLE makeHANDLE();

uint16_t rand16()
{
	short ret;
	ret = (uint16_t)rand() & 0xffff;
	return ret;
}

uint8_t rand8()
{
	byte ret;
	ret = (uint16_t)rand16() & 0xff;
	return ret;
}

uint32_t rand32()
{
	uint32_t ret = (rand16() << 16) | rand16();
	return ret;
}

uint64_t rand64()
{
	uint64_t ret = (rand16() << 48) | (rand16() << 32) | (rand16() << 16) | rand16();
	return ret;
}

uint8_t mut_byte(uint8_t v) {
	uint8_t r;

	r = rand16();
	if (bitlen < 8) {
		return v ^ (r & ((1 << (8 - bitlen)) - 1));
	}
	else {
		return v ^ (r & 1);
	}

	return v;
}

uint16_t mut_short(uint16_t v) {
	uint16_t r;

	r = rand16();
	if (bitlen < 16) {
		return (v ^ (r & ((1 << (16 - bitlen)) - 1)));
	}
	else {
		return v ^ (r & 1);
	}

	return v;
}

uint32_t mut_int(uint32_t v) {
	uint32_t r = 0;


	r = (r << 16) | (uint32_t)rand16();

	if (bitlen < 32) {
		uint32_t buf = v ^ (r & ((1 << (32 - bitlen)) - 1));
		return buf;
	}
	else {
		uint32_t buf = v ^ (r & 1);
		return buf;
	}


	return v;
}

uint64_t mut_long(uint64_t v) {
	uint64_t r = 0;

	r = (r << 16) | (uint64_t)rand16();
	r = (r << 16) | (uint64_t)rand16();
	r = (r << 16) | (uint64_t)rand16();
	r = (r << 16) | (uint64_t)rand16();

	if (bitlen < 64) {
		return v ^ (r & (((uint64_t)1 << (64 - bitlen)) - 1));
	}
	else {
		return v ^ (r & 1);
	}
	return v;
}

uint64_t mut(uint64_t v)
{
	uint64_t result;

	if (v == 0) {
		result = 0;
	}
	else if (v == 1) {
		result = rand8() % 2;
	}
	else if (!((v & 0xffffffffffffffff) >> 8)) {
		//printf("byte : ");
		result = mut_byte(v);
	}
	else if (!((v & 0xffffffffffffffff) >> 16)) {
		//printf("short : ");
		result = mut_short(v);
	}
	else if (!((v & 0xffffffffffffffff) >> 32)) {
		//printf("int : ");
		result = mut_int(v);
	}
	else {
		//printf("qword : ");
		result = mut_long(v);
	}
	//printf("0x%llx", result);
	return result;
}

BYTE* makeSTR(int ran)
{
	BYTE* ret = (BYTE*)malloc(ran);
	int i = 0;

	for (i = 0; i < ran; i++)
	{
		ret[i] = rand8();
	}
	ret[ran] = 0;

	return ret;
}

/*
PALETTEENTRY* makePALETTEENTRY()
{
	PALETTEENTRY* struc = (PALETTEENTRY*)malloc(sizeof(PALETTEENTRY));
	struc->peRed = rand8();
	struc->peGreen = rand8();
	struc->peBlue = rand8();
	struc->peFlags = rand8();
	return struc;
}

LOGPALETTE* makeLOGPALETTE()
{
	/*
//	LOGPALETTE* struc = (LOGPALETTE*)malloc(sizeof(LOGPALETTE));
//	struc->palVersion = rand16();
	//struc->palNumEntries = rand16();
//	void* tmp = makePALETTEENTRY();
//	memcpy(struc->palPalEntry, &tmp, 8);
	DWORD pl[0x101];
	pl[0] = 0x01000300;
	memcpy(pl + 1, makeSTR(0x400), 0x400);
	return (LOGPALETTE*) pl;
}

RGBQUAD* makeRGBQUAD()
{
	RGBQUAD* struc = (RGBQUAD*)malloc(sizeof(RGBQUAD));
	struc->rgbBlue = rand8();
	struc->rgbGreen = rand8();
	struc->rgbRed = rand8();
	struc->rgbReserved = rand8();
	return struc;
}

BITMAPINFOHEADER* makeBITMAPINFOHEADER()
{
	BITMAPINFOHEADER* struc = (BITMAPINFOHEADER*)malloc(sizeof(BITMAPINFOHEADER));
	struc->biSize = sizeof(BITMAPINFOHEADER);
	struc->biWidth = mut(0x100);
	struc->biHeight = mut(0x100);
	struc->biPlanes = 1;
	BYTE bBitCountList[] = { 0, 1, 4, 8, 16, 24, 32 };
	struc->biBitCount = bBitCountList[rand8() % 7];
	struc->biCompression = 0;
	struc->biSizeImage = mut(0x440);
	struc->biXPelsPerMeter = mut(0x0);
	struc->biYPelsPerMeter = mut(0x0);
	struc->biClrUsed = mut(0x0);
	struc->biClrImportant = mut(0x0);
	return struc;
}

BITMAPINFO* makeBITMAPINFO()
{
	BITMAPINFO* struc = (BITMAPINFO*)malloc(sizeof(BITMAPINFO));
	struc->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	struc->bmiHeader.biWidth = mut(0x100);
	struc->bmiHeader.biHeight = mut(0x100);
	struc->bmiHeader.biPlanes = 1;
	BYTE bBitCountList[] = { 0, 1, 4, 8, 16, 24, 32 };
	struc->bmiHeader.biBitCount = bBitCountList[rand8() % 7];
	struc->bmiHeader.biCompression = 0;
	struc->bmiHeader.biSizeImage = mut(0x440);
	struc->bmiHeader.biXPelsPerMeter = mut(0x0);
	struc->bmiHeader.biYPelsPerMeter = mut(0x0);
	struc->bmiHeader.biClrUsed = mut(0x0);
	struc->bmiHeader.biClrImportant = mut(0x0);
	void* tmp = makeRGBQUAD();
	memcpy(struc->bmiColors, &tmp, 8);

	return struc;
}

CHARSETINFO* makeCHARSETINFO()
{
	CHARSETINFO* struc = (CHARSETINFO*)malloc(sizeof(CHARSETINFO));
	struc->ciCharset = rand32();
	struc->ciACP = rand32();
	struc->fs.fsUsb[0] = rand32();
	struc->fs.fsUsb[1] = rand32();
	struc->fs.fsUsb[2] = rand32();
	struc->fs.fsUsb[3] = rand32();
	struc->fs.fsCsb[0] = rand32();
	struc->fs.fsCsb[1] = rand32();
	return struc;
}

LOGFONTW* makeLOGFONTW()
{
	LOGFONTW* struc = (LOGFONTW*)malloc(sizeof(LOGFONTW));
	struc->lfHeight = rand32();
	struc->lfWidth = rand32();
	struc->lfEscapement = rand32();
	struc->lfOrientation = rand32();
	struc->lfWeight = rand32();
	struc->lfItalic = rand8();
	struc->lfUnderline = rand8();
	struc->lfStrikeOut = rand8();
	struc->lfCharSet = rand8();
	struc->lfOutPrecision = rand8();
	struc->lfQuality = rand8();
	struc->lfPitchAndFamily = rand8();
	int ran = rand8() % LF_FACESIZE;
	memcpy(struc->lfFaceName, makeSTR(ran), ran);
	return struc;
}

LOGFONTA* makeLOGFONTA()
{
	LOGFONTA* struc = (LOGFONTA*)malloc(sizeof(LOGFONTA));
	struc->lfHeight = rand32();
	struc->lfWidth = rand32();
	struc->lfEscapement = rand32();
	struc->lfOrientation = rand32();
	struc->lfWeight = rand32();
	struc->lfItalic = rand8();
	struc->lfUnderline = rand8();
	struc->lfStrikeOut = rand8();
	struc->lfCharSet = rand8();
	struc->lfOutPrecision = rand8();
	struc->lfQuality = rand8();
	struc->lfPitchAndFamily = rand8();
	int ran = rand8() % LF_FACESIZE;
	memcpy(struc->lfFaceName, makeSTR(ran), ran);
	return struc;
}

DEVMODEA* makeDEVMODEA()
{
	DEVMODEA* struc = (DEVMODEA*)malloc(sizeof(DEVMODEA));
	int ran = rand8() % CCHDEVICENAME;
	memcpy(struc->dmDeviceName, makeSTR(ran), ran);
	struc->dmSpecVersion = rand16();
	struc->dmDriverVersion = rand16();
	struc->dmSize = rand16();
	struc->dmDriverExtra = rand16();
	struc->dmFields = rand32();
	struc->dmOrientation = rand16();
	struc->dmPaperSize = rand16();
	struc->dmPaperLength = rand16();
	struc->dmPaperWidth = rand16();
	struc->dmScale = rand16();
	struc->dmCopies = rand16();
	struc->dmDefaultSource = rand16();
	struc->dmPrintQuality = rand16();
	struc->dmPosition.x = rand32();
	struc->dmPosition.y = rand32();
	struc->dmDisplayOrientation = rand32();
	struc->dmDisplayFixedOutput = rand32();
	struc->dmColor = rand16();
	struc->dmDuplex = rand16();
	struc->dmYResolution = rand16();
	struc->dmTTOption = rand16();
	struc->dmCollate = rand16();
	ran = rand8() % CCHFORMNAME;
	memcpy(struc->dmFormName, makeSTR(ran), ran);
	struc->dmLogPixels = rand16();
	struc->dmBitsPerPel = rand32();
	struc->dmPelsWidth = rand32();
	struc->dmPelsHeight = rand32();
	struc->dmDisplayFixedOutput = rand32();
	struc->dmNup = rand32();
	struc->dmDisplayFrequency = rand32();
	struc->dmICMMethod = rand32();
	struc->dmMediaType = rand32();
	struc->dmDitherType = rand32();
	struc->dmReserved1 = rand32();
	struc->dmReserved2 = rand32();
	struc->dmPanningWidth = rand32();
	struc->dmPanningHeight = rand32();
	return struc;
}

AXISINFOA* makeAXISINFO()
{
	AXISINFOA* struc = (AXISINFOA*)malloc(sizeof(AXISINFOA));
	struc->axMinValue = rand32();
	struc->axMaxValue = rand32();
	int ran = rand8() % MM_MAX_AXES_NAMELEN;

	memcpy(struc->axAxisName, makeSTR(ran), ran);

	return struc;
}

AXESLISTA* makeAXESLISTA()
{
	AXESLISTA* struc = (AXESLISTA*)malloc(sizeof(AXESLISTA));
	struc->axlReserved = rand32();
	//struc->axlNumAxes = rand32();
	struc->axlNumAxes = 1;
	void* tmp = makeAXISINFO();
	memcpy(struc->axlAxisInfo, &tmp, 8);
	return struc;
}

ENUMLOGFONTEXDVW* makeENUMLOGFONTEXDVW()
{
	ENUMLOGFONTEXDVW* struc = (ENUMLOGFONTEXDVW*)malloc(sizeof(ENUMLOGFONTEXDVW));

	struc->elfEnumLogfontEx.elfLogFont.lfHeight = rand32();
	struc->elfEnumLogfontEx.elfLogFont.lfWidth = rand32();
	struc->elfEnumLogfontEx.elfLogFont.lfEscapement = rand32();
	struc->elfEnumLogfontEx.elfLogFont.lfOrientation = rand32();
	struc->elfEnumLogfontEx.elfLogFont.lfWeight = rand32();
	struc->elfEnumLogfontEx.elfLogFont.lfItalic = rand8();
	struc->elfEnumLogfontEx.elfLogFont.lfUnderline = rand8();
	struc->elfEnumLogfontEx.elfLogFont.lfStrikeOut = rand8();
	struc->elfEnumLogfontEx.elfLogFont.lfCharSet = rand8();
	struc->elfEnumLogfontEx.elfLogFont.lfOutPrecision = rand8();
	struc->elfEnumLogfontEx.elfLogFont.lfClipPrecision = rand8();
	struc->elfEnumLogfontEx.elfLogFont.lfQuality = rand8();
	struc->elfEnumLogfontEx.elfLogFont.lfPitchAndFamily = rand8();
	int ran = rand8() % LF_FACESIZE;
	memcpy(struc->elfEnumLogfontEx.elfLogFont.lfFaceName, makeSTR(ran), ran);
	ran = rand8() % LF_FULLFACESIZE;
	memcpy(struc->elfEnumLogfontEx.elfFullName, makeSTR(ran), ran);
	ran = rand8() % LF_FACESIZE;
	memcpy(struc->elfEnumLogfontEx.elfStyle, makeSTR(ran), ran);
	ran = rand8() % LF_FACESIZE;
	memcpy(struc->elfEnumLogfontEx.elfScript, makeSTR(ran), ran);

	struc->elfDesignVector.dvReserved = rand32();
	//struc->elfDesignVector.dvNumAxes = rand32();
	struc->elfDesignVector.dvNumAxes = 1;

	void* tmp = makeAXISINFO();
	memcpy(struc->elfDesignVector.dvValues, &tmp, 8);

	return struc;
}

LOGBRUSH* makeLOGBRUSH() {
	LOGBRUSH* struc = (LOGBRUSH*)malloc(sizeof(LOGBRUSH));

	struc->lbStyle = rand8() % 9;
	struc->lbColor = rand32() % 2;
	struc->lbHatch = rand64() % 6;
	return struc;
}

POINT* makeLPPOINT() {
	POINT* struc = (POINT*)malloc(sizeof(POINT));
	struc->x = rand32();
	struc->y = rand32();

	return struc;
}

SIZE* makeSIZE() {
	SIZE* struc = (SIZE*)malloc(sizeof(SIZE));
	struc->cx = rand32();
	struc->cy = rand32();
	return struc;
}

RECT* makeRECT() {
	RECT* struc = (RECT*)malloc(sizeof(RECT));
	struc->left = rand16();
	struc->top = rand16();
	struc->right = rand16();
	struc->bottom = rand16();
	return struc;
}

LOGPEN* makeLOGPEN() {
	LOGPEN* struc = (LOGPEN*)malloc(sizeof(LOGPEN));
	struc->lopnStyle = rand32();
	struc->lopnWidth.x = rand32();
	struc->lopnWidth.y = rand32();
	struc->lopnColor = rand32();

	return struc;
}

POINT* makePOINT() {
	POINT* struc = (POINT*)malloc(sizeof(POINT));
	struc->x = rand32();
	struc->y = rand32();

	return struc;
}

void* makePTR()
{
	return malloc(50000);
}
*/

LOGPALETTE* makeLOGPALETTE()
{

	//   LOGPALETTE* struc = (LOGPALETTE*)malloc(sizeof(LOGPALETTE));
	//   struc->palVersion = rand16();
	   //struc->palNumEntries = rand16();
	//   void* tmp = makePALETTEENTRY();
	//   memcpy(struc->palPalEntry, &tmp, 8);
	DWORD pl[0x101];
	pl[0] = 0x01000300;
	memcpy(pl + 1, makeSTR(0x400), 0x400);
	return (LOGPALETTE*)pl;
}

LOGPALETTE* getLOGPALETTE()
{

	//   LOGPALETTE* struc = (LOGPALETTE*)malloc(sizeof(LOGPALETTE));
	//   struc->palVersion = rand16();
	   //struc->palNumEntries = rand16();
	//   void* tmp = makePALETTEENTRY();
	//   memcpy(struc->palPalEntry, &tmp, 8);
	DWORD pl[0x101];
	pl[0] = 0x01000300;
	memcpy(pl + 1, makeSTR(0x400), 0x400);
	return (LOGPALETTE*)pl;
}

PALETTEENTRY* makePALETTEENTRY()
{
	PALETTEENTRY* struc = (PALETTEENTRY*)malloc(sizeof(PALETTEENTRY) + 100);
	struc->peRed = rand8();
	struc->peGreen = rand8();
	struc->peBlue = rand8();
	struc->peFlags = rand8();
	return struc;
}

void makePALETTEENTRYList()
{
	int i = 0;
	while (i < 10) {
		paletteentryList[i] = (PALETTEENTRY*)malloc(sizeof(PALETTEENTRY) + 100);
		paletteentryList[i]->peRed = rand8();
		paletteentryList[i]->peGreen = rand8();
		paletteentryList[i]->peBlue = rand8();
		paletteentryList[i]->peFlags = rand8();
		i++;
	}
}

PALETTEENTRY* getPALETTEENTRY()
{
	int i = rand() % 10;
	return paletteentryList[i];
}

void freePALETTEENTRYList()
{
	int i = 0;
	while (i < 10) {
		free(paletteentryList[i]);
		i++;
	}
}
////////////////////////////////////////////////////////////////////////////////////////
TEXTMETRICA* makeTEXTMETRICA()
{
	TEXTMETRICA* struc = (TEXTMETRICA*)malloc(sizeof(TEXTMETRICA));
	struc->tmHeight = rand32();
	struc->tmAscent = rand32();
	struc->tmDescent = rand32();
	struc->tmInternalLeading = rand32();
	struc->tmExternalLeading = rand32();
	struc->tmAveCharWidth = rand32();
	struc->tmMaxCharWidth = rand32();
	struc->tmWeight = rand32();
	struc->tmOverhang = rand32();
	struc->tmDigitizedAspectX = rand32();
	struc->tmDigitizedAspectY = rand32();

	struc->tmFirstChar = rand8();
	struc->tmLastChar = rand8();
	struc->tmDefaultChar = rand8();
	struc->tmBreakChar = rand8();

	struc->tmItalic = rand8();
	struc->tmUnderlined = rand8();
	struc->tmStruckOut = rand8();
	int list[4] = { 1,2,4,8 };
	int j = rand() % 4;
	struc->tmPitchAndFamily = list[j];
	int charset[19] = { 0,1 ,2,128,129,134,136,255,130,177,178,161,162,163,222 , 238,204,77,186 };
	j = rand() % 19;
	struc->tmCharSet = charset[j];

	return struc;		
}
void makeTEXTMETRICAList()
{
	int i = 0;
	while (i < 10) {
		TEXTMETRICAList[i] = (TEXTMETRICA*)malloc(sizeof(TEXTMETRICA));
		TEXTMETRICAList[i]->tmHeight = rand32();
		TEXTMETRICAList[i]->tmAscent = rand32();
		TEXTMETRICAList[i]->tmDescent = rand32();
		TEXTMETRICAList[i]->tmInternalLeading = rand32();
		TEXTMETRICAList[i]->tmExternalLeading = rand32();
		TEXTMETRICAList[i]->tmAveCharWidth = rand32();
		TEXTMETRICAList[i]->tmMaxCharWidth = rand32();
		TEXTMETRICAList[i]->tmWeight = rand32();
		TEXTMETRICAList[i]->tmOverhang = rand32();
		TEXTMETRICAList[i]->tmDigitizedAspectX = rand32();
		TEXTMETRICAList[i]->tmDigitizedAspectY = rand32();

		TEXTMETRICAList[i]->tmFirstChar = rand8();
		TEXTMETRICAList[i]->tmLastChar = rand8();
		TEXTMETRICAList[i]->tmDefaultChar = rand8();
		TEXTMETRICAList[i]->tmBreakChar = rand8();

		TEXTMETRICAList[i]->tmItalic = rand8();
		TEXTMETRICAList[i]->tmUnderlined = rand8();
		TEXTMETRICAList[i]->tmStruckOut = rand8();
		int list[4] = { 1,2,4,8 };
		int j = rand() % 4;
		TEXTMETRICAList[i]->tmPitchAndFamily = list[j];
		int charset[19] = { 0,1 ,2,128,129,134,136,255,130,177,178,161,162,163,222 , 238,204,77,186 };
		j = rand() % 19;
		TEXTMETRICAList[i]->tmCharSet = charset[j];
		i++;
	}
}
TEXTMETRICA* getTEXTMETRICA()
{
	int i = rand() % 10;
	return TEXTMETRICAList[i];
}
void freeTEXTMETRICAList()
{
	int i = 0;
	while (i < 10) {
		free(TEXTMETRICAList[i]);
		i++;
	}
}
/////////////////////////////////////////////////////////////////////////////////////////
FONTSIGNATURE* makeFONTSIGNATURE()
{
	FONTSIGNATURE*struc = (FONTSIGNATURE*)malloc(sizeof(FONTSIGNATURE));
	struc->fsUsb[0] = rand() % 126;
	struc->fsUsb[1] = rand() % 126;
	struc->fsUsb[2] = rand() % 126;
	struc->fsUsb[3] = rand() % 126;
	int list[31] = { 0,1,2,3,4,5,6,7,8,16,17,18,19,20,21,47,48,49,50,51,52,54,55,56,57,58,59,60,61,62,63 };
	int i = rand() % 31;
	struc->fsCsb[0] = list[i];
	struc->fsCsb[1] = list[i];
	return struc;
}
void makeFONTSIGNATUREList()
{
	int j = 0;
	while (j < 10) {
		fontsignatureList[j] = (FONTSIGNATURE*)malloc(sizeof(FONTSIGNATURE));
		fontsignatureList[j]->fsUsb[0] = rand() % 126;
		fontsignatureList[j]->fsUsb[1] = rand() % 126;
		fontsignatureList[j]->fsUsb[2] = rand() % 126;
		fontsignatureList[j]->fsUsb[3] = rand() % 126;
		int list[31] = { 0,1,2,3,4,5,6,7,8,16,17,18,19,20,21,47,48,49,50,51,52,54,55,56,57,58,59,60,61,62,63 };
		int i = rand() % 31;
		fontsignatureList[j]->fsCsb[0] = list[i];
		fontsignatureList[j]->fsCsb[1] = list[i];
		j++;
	}
}
FONTSIGNATURE* getFONTSIGNATURE()
{
	int i = rand() % 10;
	return fontsignatureList[i];
}
void freeFONTSIGNATUREList()
{
	int i = 0;
	while (i < 10) {
		free(fontsignatureList[i]);
		i++;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////

BITMAPINFOHEADER* makeBITMAPINFOHEADER()
{
	BITMAPINFOHEADER* struc = (BITMAPINFOHEADER*)malloc(sizeof(BITMAPINFOHEADER) + 100);
	struc->biSize = sizeof(BITMAPINFOHEADER);
	struc->biWidth = mut(0x100);
	struc->biHeight = mut(0x100);
	struc->biPlanes = 1;
	struc->biBitCount = mut(0x20);
	struc->biCompression = mut(0x0);
	struc->biSizeImage = mut(0x440);
	struc->biXPelsPerMeter = mut(0x0);
	struc->biYPelsPerMeter = mut(0x0);
	struc->biClrUsed = mut(0x0);
	struc->biClrImportant = mut(0x0);
	return struc;
}

void makeBITMAPINFOHEADERList()
{
	int i = 0;
	while (i < 10) {
		bitmapinfoheaderList[i] = (BITMAPINFOHEADER*)malloc(sizeof(BITMAPINFOHEADER) + 100);
		bitmapinfoheaderList[i]->biSize = sizeof(BITMAPINFOHEADER);
		bitmapinfoheaderList[i]->biWidth = mut(0x100);
		bitmapinfoheaderList[i]->biHeight = mut(0x100);
		bitmapinfoheaderList[i]->biPlanes = 1;
		bitmapinfoheaderList[i]->biBitCount = mut(0x20);
		bitmapinfoheaderList[i]->biCompression = mut(0x0);
		bitmapinfoheaderList[i]->biSizeImage = mut(0x440);
		bitmapinfoheaderList[i]->biXPelsPerMeter = mut(0x0);
		bitmapinfoheaderList[i]->biYPelsPerMeter = mut(0x0);
		bitmapinfoheaderList[i]->biClrUsed = mut(0x0);
		bitmapinfoheaderList[i]->biClrImportant = mut(0x0);
		i++;
	}
}

BITMAPINFOHEADER* getBITMAPINFOHEADER()
{
	int i = rand() % 10;
	return bitmapinfoheaderList[i];
}

void freeBITMAPINFOHEADERList()
{
	int i = 0;
	while (i < 10) {
		free(bitmapinfoheaderList[i]);
		i++;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////

RGBQUAD* makeRGBQUAD()
{
	RGBQUAD* struc = (RGBQUAD*)malloc(sizeof(RGBQUAD) + 100);
	struc->rgbBlue = rand8();
	struc->rgbGreen = rand8();
	struc->rgbRed = rand8();
	struc->rgbReserved = rand8();
	return struc;
}

void makeRGBQUADList()
{
	int i = 0;
	while (i < 10) {
		rgbquadList[i] = (RGBQUAD*)malloc(sizeof(RGBQUAD) + 100);
		rgbquadList[i]->rgbBlue = rand8();
		rgbquadList[i]->rgbGreen = rand8();
		rgbquadList[i]->rgbRed = rand8();
		rgbquadList[i]->rgbReserved = rand8();
		i++;
	}
}

RGBQUAD* getRGBQUAD()
{
	int i = rand() % 10;
	return rgbquadList[i];
}

void freeRGBQUADList()
{
	int i = 0;
	while (i < 10) {
		free(rgbquadList[i]);
		i++;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////

BITMAPINFO* makeBITMAPINFO()
{
	BITMAPINFO* struc = (BITMAPINFO*)malloc(sizeof(BITMAPINFO) + 100);
	struc->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	struc->bmiHeader.biWidth = mut(0x28);
	struc->bmiHeader.biHeight = mut(0x100);
	struc->bmiHeader.biPlanes = mut(0x100);
	struc->bmiHeader.biBitCount = 1;
	struc->bmiHeader.biCompression = mut(0x0);
	struc->bmiHeader.biSizeImage = mut(0x440);
	struc->bmiHeader.biXPelsPerMeter = mut(0x0);
	struc->bmiHeader.biYPelsPerMeter = mut(0x0);
	struc->bmiHeader.biClrUsed = mut(0x0);
	struc->bmiHeader.biClrImportant = mut(0x0);
	void* tmp = getRGBQUAD();
	memcpy(struc->bmiColors, &tmp, 8);
	return struc;
}

void makeBITMAPINFOList()
{
	int i = 0;
	while (i < 10) {
		bitmapinfoList[i] = (BITMAPINFO*)malloc(sizeof(BITMAPINFO) + 100);
		bitmapinfoList[i]->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		bitmapinfoList[i]->bmiHeader.biWidth = mut(0x28);
		bitmapinfoList[i]->bmiHeader.biHeight = mut(0x100);
		bitmapinfoList[i]->bmiHeader.biPlanes = mut(0x100);
		bitmapinfoList[i]->bmiHeader.biBitCount = 1;
		bitmapinfoList[i]->bmiHeader.biCompression = mut(0x0);
		bitmapinfoList[i]->bmiHeader.biSizeImage = mut(0x440);
		bitmapinfoList[i]->bmiHeader.biXPelsPerMeter = mut(0x0);
		bitmapinfoList[i]->bmiHeader.biYPelsPerMeter = mut(0x0);
		bitmapinfoList[i]->bmiHeader.biClrUsed = mut(0x0);
		bitmapinfoList[i]->bmiHeader.biClrImportant = mut(0x0);
		void* tmp = getRGBQUAD();
		memcpy(bitmapinfoList[i]->bmiColors, &tmp, 8);
		i++;
	}
}

BITMAPINFO* getBITMAPINFO()
{
	int i = rand() % 10;
	return bitmapinfoList[i];
}

void freeBITMAPINFOList()
{
	int i = 0;
	while (i < 10) {
		free(bitmapinfoList[i]);
		i++;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////

CHARSETINFO* makeCHARSETINFO()
{
	CHARSETINFO* struc = (CHARSETINFO*)malloc(sizeof(CHARSETINFO) + 100);
	struc->ciCharset = rand32();
	struc->ciACP = rand32();
	struc->fs.fsUsb[0] = rand32();
	struc->fs.fsUsb[1] = rand32();
	struc->fs.fsUsb[2] = rand32();
	struc->fs.fsUsb[3] = rand32();
	struc->fs.fsCsb[0] = rand32();
	struc->fs.fsCsb[1] = rand32();
	return struc;
}

void makeCHARSETINFOList()
{
	int i = 0;
	while (i < 10) {
		charsetinfoList[i] = (CHARSETINFO*)malloc(sizeof(CHARSETINFO) + 100);
		charsetinfoList[i]->ciCharset = rand32();
		charsetinfoList[i]->ciACP = rand32();
		charsetinfoList[i]->fs.fsUsb[0] = rand32();
		charsetinfoList[i]->fs.fsUsb[1] = rand32();
		charsetinfoList[i]->fs.fsUsb[2] = rand32();
		charsetinfoList[i]->fs.fsUsb[3] = rand32();
		charsetinfoList[i]->fs.fsCsb[0] = rand32();
		charsetinfoList[i]->fs.fsCsb[1] = rand32();
		i++;
	}
}

CHARSETINFO* getCHARSETINFO()
{
	int i = rand() % 10;
	return charsetinfoList[i];
}

void freeCHARSETINFOList()
{
	int i = 0;
	while (i < 10) {
		free(charsetinfoList[i]);
		i++;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////

LOGFONTW* makeLOGFONTW()
{
	LOGFONTW* struc = (LOGFONTW*)malloc(sizeof(LOGFONTW) + 100);
	struc->lfHeight = rand32();
	struc->lfWidth = rand32();
	struc->lfEscapement = rand32();
	struc->lfOrientation = rand32();
	struc->lfWeight = rand32();
	struc->lfItalic = rand8();
	struc->lfUnderline = rand8();
	struc->lfStrikeOut = rand8();
	struc->lfCharSet = rand8();
	struc->lfOutPrecision = rand8();
	struc->lfQuality = rand8();
	struc->lfPitchAndFamily = rand8();
	int ran = rand8() % LF_FACESIZE;
	memcpy(struc->lfFaceName, makeSTR(ran), ran);
	return struc;
}

void makeLOGFONTWList()
{
	int i = 0;
	while (i < 10) {
		logfontwList[i] = (LOGFONTW*)malloc(sizeof(LOGFONTW) + 100);
		logfontwList[i]->lfHeight = rand32();
		logfontwList[i]->lfWidth = rand32();
		logfontwList[i]->lfEscapement = rand32();
		logfontwList[i]->lfOrientation = rand32();
		logfontwList[i]->lfWeight = rand32();
		logfontwList[i]->lfItalic = rand8();
		logfontwList[i]->lfUnderline = rand8();
		logfontwList[i]->lfStrikeOut = rand8();
		logfontwList[i]->lfCharSet = rand8();
		logfontwList[i]->lfOutPrecision = rand8();
		logfontwList[i]->lfQuality = rand8();
		logfontwList[i]->lfPitchAndFamily = rand8();
		int ran = rand8() % LF_FACESIZE;
		memcpy(logfontwList[i]->lfFaceName, makeSTR(ran), ran);
		i++;
	}
}

LOGFONTW* getLOGFONTW()
{
	int i = rand() % 10;
	return logfontwList[i];
}

void freeLOGFONTWList()
{
	int i = 0;
	while (i < 10) {
		free(logfontwList[i]);
		i++;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////

LOGFONTA* makeLOGFONTA()
{
	LOGFONTA* struc = (LOGFONTA*)malloc(sizeof(LOGFONTA) + 100);
	struc->lfHeight = rand32();
	struc->lfWidth = rand32();
	struc->lfEscapement = rand32();
	struc->lfOrientation = rand32();
	struc->lfWeight = rand32();
	struc->lfItalic = rand8();
	struc->lfUnderline = rand8();
	struc->lfStrikeOut = rand8();
	struc->lfCharSet = rand8();
	struc->lfOutPrecision = rand8();
	struc->lfQuality = rand8();
	struc->lfPitchAndFamily = rand8();
	int ran = rand8() % LF_FACESIZE;
	memcpy(struc->lfFaceName, makeSTR(ran), ran);
	return struc;
}

void makeLOGFONTAList()
{
	int i = 0;
	while (i < 10) {
		logfontaList[i] = (LOGFONTA*)malloc(sizeof(LOGFONTA) + 100);
		logfontaList[i]->lfHeight = rand32();
		logfontaList[i]->lfWidth = rand32();
		logfontaList[i]->lfEscapement = rand32();
		logfontaList[i]->lfOrientation = rand32();
		logfontaList[i]->lfWeight = rand32();
		logfontaList[i]->lfItalic = rand8();
		logfontaList[i]->lfUnderline = rand8();
		logfontaList[i]->lfStrikeOut = rand8();
		logfontaList[i]->lfCharSet = rand8();
		logfontaList[i]->lfOutPrecision = rand8();
		logfontaList[i]->lfQuality = rand8();
		logfontaList[i]->lfPitchAndFamily = rand8();
		int ran = rand8() % LF_FACESIZE;
		memcpy(logfontaList[i]->lfFaceName, makeSTR(ran), ran);
		i++;
	}
}

LOGFONTA* getLOGFONTA()
{
	int i = rand() % 10;
	return logfontaList[i];
}

void freeLOGFONTAList()
{
	int i = 0;
	while (i < 10) {
		free(logfontaList[i]);
		i++;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////

DEVMODEA* makeDEVMODEA()
{
	DEVMODEA* struc = (DEVMODEA*)malloc(sizeof(DEVMODEA) + 100);
	int ran = rand8() % CCHDEVICENAME;
	memcpy(struc->dmDeviceName, makeSTR(ran), ran);
	struc->dmSpecVersion = rand16();
	struc->dmDriverVersion = rand16();
	struc->dmSize = rand16();
	struc->dmDriverExtra = rand16();
	struc->dmFields = rand32();
	struc->dmOrientation = rand16();
	struc->dmPaperSize = rand16();
	struc->dmPaperLength = rand16();
	struc->dmPaperWidth = rand16();
	struc->dmScale = rand16();
	struc->dmCopies = rand16();
	struc->dmDefaultSource = rand16();
	struc->dmPrintQuality = rand16();
	struc->dmPosition.x = rand32();
	struc->dmPosition.y = rand32();
	struc->dmDisplayOrientation = rand32();
	struc->dmDisplayFixedOutput = rand32();
	struc->dmColor = rand16();
	struc->dmDuplex = rand16();
	struc->dmYResolution = rand16();
	struc->dmTTOption = rand16();
	struc->dmCollate = rand16();
	ran = rand8() % CCHFORMNAME;
	memcpy(struc->dmFormName, makeSTR(ran), ran);
	struc->dmLogPixels = rand16();
	struc->dmBitsPerPel = rand32();
	struc->dmPelsWidth = rand32();
	struc->dmPelsHeight = rand32();
	struc->dmDisplayFixedOutput = rand32();
	struc->dmNup = rand32();
	struc->dmDisplayFrequency = rand32();
	struc->dmICMMethod = rand32();
	struc->dmMediaType = rand32();
	struc->dmDitherType = rand32();
	struc->dmReserved1 = rand32();
	struc->dmReserved2 = rand32();
	struc->dmPanningWidth = rand32();
	struc->dmPanningHeight = rand32();
	return struc;
}

void makeDEVMODEAList()
{
	int i = 0;
	while (i < 10) {
		devmodeaList[i] = (DEVMODEA*)malloc(sizeof(DEVMODEA) + 100);
		int ran = rand8() % CCHDEVICENAME;
		memcpy(devmodeaList[i]->dmDeviceName, makeSTR(ran), ran);
		devmodeaList[i]->dmSpecVersion = rand16();
		devmodeaList[i]->dmDriverVersion = rand16();
		devmodeaList[i]->dmSize = rand16();
		devmodeaList[i]->dmDriverExtra = rand16();
		devmodeaList[i]->dmFields = rand32();
		devmodeaList[i]->dmOrientation = rand16();
		devmodeaList[i]->dmPaperSize = rand16();
		devmodeaList[i]->dmPaperLength = rand16();
		devmodeaList[i]->dmPaperWidth = rand16();
		devmodeaList[i]->dmScale = rand16();
		devmodeaList[i]->dmCopies = rand16();
		devmodeaList[i]->dmDefaultSource = rand16();
		devmodeaList[i]->dmPrintQuality = rand16();
		devmodeaList[i]->dmPosition.x = rand32();
		devmodeaList[i]->dmPosition.y = rand32();
		devmodeaList[i]->dmDisplayOrientation = rand32();
		devmodeaList[i]->dmDisplayFixedOutput = rand32();
		devmodeaList[i]->dmColor = rand16();
		devmodeaList[i]->dmDuplex = rand16();
		devmodeaList[i]->dmYResolution = rand16();
		devmodeaList[i]->dmTTOption = rand16();
		devmodeaList[i]->dmCollate = rand16();
		ran = rand8() % CCHFORMNAME;
		memcpy(devmodeaList[i]->dmFormName, makeSTR(ran), ran);
		devmodeaList[i]->dmLogPixels = rand16();
		devmodeaList[i]->dmBitsPerPel = rand32();
		devmodeaList[i]->dmPelsWidth = rand32();
		devmodeaList[i]->dmPelsHeight = rand32();
		devmodeaList[i]->dmDisplayFixedOutput = rand32();
		devmodeaList[i]->dmNup = rand32();
		devmodeaList[i]->dmDisplayFrequency = rand32();
		devmodeaList[i]->dmICMMethod = rand32();
		devmodeaList[i]->dmMediaType = rand32();
		devmodeaList[i]->dmDitherType = rand32();
		devmodeaList[i]->dmReserved1 = rand32();
		devmodeaList[i]->dmReserved2 = rand32();
		devmodeaList[i]->dmPanningWidth = rand32();
		devmodeaList[i]->dmPanningHeight = rand32();
		i++;
	}
}

DEVMODEA* getDEVMODEA()
{
	int i = rand() % 10;
	return devmodeaList[i];
}

void freeDEVMODEAList()
{
	int i = 0;
	while (i < 10) {
		free(devmodeaList[i]);
		i++;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////

AXISINFOA* makeAXISINFO()
{
	AXISINFOA* struc = (AXISINFOA*)malloc(sizeof(AXISINFOA) + 100);
	struc->axMinValue = rand32();
	struc->axMaxValue = rand32();
	int ran = rand8() % MM_MAX_AXES_NAMELEN;
	memcpy(struc->axAxisName, makeSTR(ran), ran);
	return struc;
}

void makeAXISINFOList()
{
	int i = 0;
	while (i < 10) {
		axisinfoList[i] = (AXISINFOA*)malloc(sizeof(AXISINFOA) + 100);
		axisinfoList[i]->axMinValue = rand32();
		axisinfoList[i]->axMaxValue = rand32();
		int ran = rand8() % MM_MAX_AXES_NAMELEN;
		memcpy(axisinfoList[i]->axAxisName, makeSTR(ran), ran);
		i++;
	}
}

AXISINFOA* getAXISINFO()
{
	int i = rand() % 10;
	return axisinfoList[i];
}

void freeAXISINFOList()
{
	int i = 0;
	while (i < 10) {
		free(axisinfoList[i]);
		i++;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////

AXESLISTA* makeAXESLISTA()
{
	AXESLISTA* struc = (AXESLISTA*)malloc(sizeof(AXESLISTA) + 100);
	struc->axlReserved = rand32();
	//struc->axlNumAxes = rand32();
	struc->axlNumAxes = 1;
	void* tmp = makeAXISINFO();
	memcpy(struc->axlAxisInfo, &tmp, 8);
	return struc;
}

void makeAXESLISTAList()
{
	int i = 0;
	while (i < 10) {
		axeslistaList[i] = (AXESLISTA*)malloc(sizeof(AXESLISTA) + 100);
		axeslistaList[i]->axlReserved = rand32();
		//axeslistaList[i]->axlNumAxes = rand32();
		axeslistaList[i]->axlNumAxes = 1;
		void* tmp = getAXISINFO();
		memcpy(axeslistaList[i]->axlAxisInfo, &tmp, 8);
		i++;
	}
}

AXESLISTA* getAXESLISTA()
{
	int i = rand() % 10;
	return axeslistaList[i];
}

void freeAXESLISTAList()
{
	int i = 0;
	while (i < 10) {
		free(axeslistaList[i]);
		i++;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////

ENUMLOGFONTEXDVW* makeENUMLOGFONTEXDVW()
{
	ENUMLOGFONTEXDVW* struc = (ENUMLOGFONTEXDVW*)malloc(sizeof(ENUMLOGFONTEXDVW) + 100);

	struc->elfEnumLogfontEx.elfLogFont.lfHeight = rand32();
	struc->elfEnumLogfontEx.elfLogFont.lfWidth = rand32();
	struc->elfEnumLogfontEx.elfLogFont.lfEscapement = rand32();
	struc->elfEnumLogfontEx.elfLogFont.lfOrientation = rand32();
	struc->elfEnumLogfontEx.elfLogFont.lfWeight = rand32();
	struc->elfEnumLogfontEx.elfLogFont.lfItalic = rand8();
	struc->elfEnumLogfontEx.elfLogFont.lfUnderline = rand8();
	struc->elfEnumLogfontEx.elfLogFont.lfStrikeOut = rand8();
	struc->elfEnumLogfontEx.elfLogFont.lfCharSet = rand8();
	struc->elfEnumLogfontEx.elfLogFont.lfOutPrecision = rand8();
	struc->elfEnumLogfontEx.elfLogFont.lfClipPrecision = rand8();
	struc->elfEnumLogfontEx.elfLogFont.lfQuality = rand8();
	struc->elfEnumLogfontEx.elfLogFont.lfPitchAndFamily = rand8();
	int ran = rand8() % LF_FACESIZE;
	memcpy(struc->elfEnumLogfontEx.elfLogFont.lfFaceName, makeSTR(ran), ran);
	ran = rand8() % LF_FULLFACESIZE;
	memcpy(struc->elfEnumLogfontEx.elfFullName, makeSTR(ran), ran);
	ran = rand8() % LF_FACESIZE;
	memcpy(struc->elfEnumLogfontEx.elfStyle, makeSTR(ran), ran);
	ran = rand8() % LF_FACESIZE;
	memcpy(struc->elfEnumLogfontEx.elfScript, makeSTR(ran), ran);
	struc->elfDesignVector.dvReserved = rand32();
	//struc->elfDesignVector.dvNumAxes = rand32();
	struc->elfDesignVector.dvNumAxes = 1;
	void* tmp = makeAXISINFO();
	memcpy(struc->elfDesignVector.dvValues, &tmp, 8);
	return struc;
}

void makeENUMLOGFONTEXDVWList()
{
	int i = 0;
	while (i < 10) {
		enumlogfontexdvwList[i] = (ENUMLOGFONTEXDVW*)malloc(sizeof(ENUMLOGFONTEXDVW) + 100);
		enumlogfontexdvwList[i]->elfEnumLogfontEx.elfLogFont.lfHeight = rand32();
		enumlogfontexdvwList[i]->elfEnumLogfontEx.elfLogFont.lfWidth = rand32();
		enumlogfontexdvwList[i]->elfEnumLogfontEx.elfLogFont.lfEscapement = rand32();
		enumlogfontexdvwList[i]->elfEnumLogfontEx.elfLogFont.lfOrientation = rand32();
		enumlogfontexdvwList[i]->elfEnumLogfontEx.elfLogFont.lfWeight = rand32();
		enumlogfontexdvwList[i]->elfEnumLogfontEx.elfLogFont.lfItalic = rand8();
		enumlogfontexdvwList[i]->elfEnumLogfontEx.elfLogFont.lfUnderline = rand8();
		enumlogfontexdvwList[i]->elfEnumLogfontEx.elfLogFont.lfStrikeOut = rand8();
		enumlogfontexdvwList[i]->elfEnumLogfontEx.elfLogFont.lfCharSet = rand8();
		enumlogfontexdvwList[i]->elfEnumLogfontEx.elfLogFont.lfOutPrecision = rand8();
		enumlogfontexdvwList[i]->elfEnumLogfontEx.elfLogFont.lfClipPrecision = rand8();
		enumlogfontexdvwList[i]->elfEnumLogfontEx.elfLogFont.lfQuality = rand8();
		enumlogfontexdvwList[i]->elfEnumLogfontEx.elfLogFont.lfPitchAndFamily = rand8();
		int ran = rand8() % LF_FACESIZE;
		memcpy(enumlogfontexdvwList[i]->elfEnumLogfontEx.elfLogFont.lfFaceName, makeSTR(ran), ran);
		ran = rand8() % LF_FULLFACESIZE;
		memcpy(enumlogfontexdvwList[i]->elfEnumLogfontEx.elfFullName, makeSTR(ran), ran);
		ran = rand8() % LF_FACESIZE;
		memcpy(enumlogfontexdvwList[i]->elfEnumLogfontEx.elfStyle, makeSTR(ran), ran);
		ran = rand8() % LF_FACESIZE;
		memcpy(enumlogfontexdvwList[i]->elfEnumLogfontEx.elfScript, makeSTR(ran), ran);
		enumlogfontexdvwList[i]->elfDesignVector.dvReserved = rand32();
		//enumlogfontexdvwList[i]->elfDesignVector.dvNumAxes = rand32();
		enumlogfontexdvwList[i]->elfDesignVector.dvNumAxes = 1;
		void* tmp = getAXISINFO();
		memcpy(enumlogfontexdvwList[i]->elfDesignVector.dvValues, &tmp, 8);
		i++;
	}
}

ENUMLOGFONTEXDVW* getENUMLOGFONTEXDVW()
{
	int i = rand() % 10;
	return enumlogfontexdvwList[i];
}

void freeENUMLOGFONTEXDVWList()
{
	int i = 0;
	while (i < 10) {
		free(enumlogfontexdvwList[i]);
		i++;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////

LOGBRUSH* makeLOGBRUSH()
{
	LOGBRUSH* struc = (LOGBRUSH*)malloc(sizeof(LOGBRUSH) + 100);
	//	struc->lbStyle = rand() % 9;
	//	struc->lbColor = rand() % 2;
	//	struc->lbHatch = rand() % 6;
	struc->lbStyle = 0;
	struc->lbColor = 0;
	struc->lbHatch = 0;
	return struc;
}

void makeLOGBRUSHList()
{
	int i = 0;
	while (i < 10) {
		logbrushList[i] = (LOGBRUSH*)malloc(sizeof(LOGBRUSH) + 100);
		//		logbrushList[i]->lbStyle = rand() % 9;
		//		logbrushList[i]->lbColor = rand() % 2;
		//		logbrushList[i]->lbHatch = rand() % 6;
		logbrushList[i]->lbStyle = 0;
		logbrushList[i]->lbColor = 0;
		logbrushList[i]->lbHatch = 0;
		i++;
	}
}

LOGBRUSH* getLOGBRUSH()
{
	int i = rand() % 10;
	return logbrushList[i];
}

void freeLOGBRUSHList()
{
	int i = 0;
	while (i < 10) {
		free(logbrushList[i]);
		i++;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////

POINT* makeLPPOINT()
{
	POINT* struc = (POINT*)malloc(sizeof(POINT) + 100);
	struc->x = rand32();
	struc->y = rand32();
	return struc;
}

void makeLPPOINTList()
{
	int i = 0;
	while (i < 10) {
		lppointList[i] = (POINT*)malloc(sizeof(POINT) + 100);
		lppointList[i]->x = rand32();
		lppointList[i]->y = rand32();
		i++;
	}
}

POINT* getLPPOINT()
{
	int i = rand() % 10;
	return lppointList[i];
}

void freeLPPOINTList()
{
	int i = 0;
	while (i < 10) {
		free(lppointList[i]);
		i++;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////

SIZE* makeSIZE()
{
	SIZE* struc = (SIZE*)malloc(sizeof(SIZE) + 100);
	struc->cx = rand32();
	struc->cy = rand32();
	return struc;
}

void makeSIZEList()
{
	int i = 0;
	while (i < 10) {
		sizeList[i] = (SIZE*)malloc(sizeof(SIZE) + 100);
		sizeList[i]->cx = rand32();
		sizeList[i]->cy = rand32();
		i++;
	}
}

SIZE* getSIZE()
{
	int i = rand() % 10;
	return sizeList[i];
}

void freeSIZEList()
{
	int i = 0;
	while (i < 10) {
		free(sizeList[i]);
		i++;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////

RECT* makeRECT()
{
	RECT* struc = (RECT*)malloc(sizeof(RECT) + 100);
	struc->left = rand16();
	struc->top = rand16();
	struc->right = rand16();
	struc->bottom = rand16();
	return struc;
}

void makeRECTList()
{
	int i = 0;
	while (i < 10) {
		rectList[i] = (RECT*)malloc(sizeof(RECT) + 100);
		rectList[i]->left = rand16();
		rectList[i]->top = rand16();
		rectList[i]->right = rand16();
		rectList[i]->bottom = rand16();
		i++;
	}
}

RECT* getRECT()
{
	int i = rand() % 10;
	return rectList[i];
}

void freeRECTList()
{
	int i = 0;
	while (i < 10) {
		free(rectList[i]);
		i++;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////

LOGPEN* makeLOGPEN()
{
	LOGPEN* struc = (LOGPEN*)malloc(sizeof(LOGPEN) + 100);
	struc->lopnStyle = rand32();
	struc->lopnWidth.x = rand32();
	struc->lopnWidth.y = rand32();
	struc->lopnColor = rand32();

	return struc;
}

void makeLOGPENList()
{
	int i = 0;
	while (i < 10) {
		logpenList[i] = (LOGPEN*)malloc(sizeof(LOGPEN) + 100);
		logpenList[i]->lopnStyle = rand32();
		logpenList[i]->lopnWidth.x = rand32();
		logpenList[i]->lopnWidth.y = rand32();
		logpenList[i]->lopnColor = rand32();
		i++;
	}
}

LOGPEN* getLOGPEN()
{
	int i = rand() % 10;
	return logpenList[i];
}

void freeLOGPENList()
{
	int i = 0;
	while (i < 10) {
		free(logpenList[i]);
		i++;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////

POINT* makePOINT() {
	POINT* struc = (POINT*)malloc(sizeof(POINT) + 100);
	struc->x = rand32();
	struc->y = rand32();

	return struc;
}

void makePOINTList()
{
	int i = 0;
	while (i < 10) {
		pointList[i] = (POINT*)malloc(sizeof(POINT) + 100);
		pointList[i]->x = rand32();
		pointList[i]->y = rand32();
		i++;
	}
}

POINT* getPOINT()
{
	int i = rand() % 10;
	return pointList[i];
}

void freePOINTList()
{
	int i = 0;
	while (i < 10) {
		free(pointList[i]);
		i++;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////

void* makePTR()
{
	return malloc(50000);
}

void makePTRList()
{
	int i = 0;
	while (i < 10) {
		ptrList[i] = malloc(50000);
		i++;
	}
}

void* getPTR()
{
	int i = rand() % 10;
	return ptrList[i];
}

void freePTRList()
{
	int i = 0;
	while (i < 10) {
		free(ptrList[i]);
		i++;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////

void makeALL()
{
	makePALETTEENTRYList();
	makeBITMAPINFOHEADERList();
	makeRGBQUADList();
	makeBITMAPINFOList();
	makeCHARSETINFOList();
	makeLOGFONTWList();
	makeLOGFONTAList();
	makeDEVMODEAList();
	makeAXISINFOList();
	makeAXESLISTAList();
	makeENUMLOGFONTEXDVWList();
	makeLOGBRUSHList();
	makeLPPOINTList();
	makeSIZEList();
	makeRECTList();
	makeLOGPENList();
	makePOINTList();
	makePTRList();
	makeFONTSIGNATUREList();
	makeTEXTMETRICAList();
}

void freeALL()
{
	freePALETTEENTRYList();
	freeBITMAPINFOHEADERList();
	freeRGBQUADList();
	freeBITMAPINFOList();
	freeCHARSETINFOList();
	freeLOGFONTWList();
	freeLOGFONTAList();
	freeDEVMODEAList();
	freeAXISINFOList();
	freeAXESLISTAList();
	freeENUMLOGFONTEXDVWList();
	freeLOGBRUSHList();
	freeLPPOINTList();
	freeSIZEList();
	freeRECTList();
	freeLOGPENList();
	freePOINTList();
	freePTRList();
	freeFONTSIGNATUREList();
	freeTEXTMETRICAList();
}

void** makeDPTR()
{
	void* a = malloc(4000);
	return &a;
}

HDC makeHDC()
{
	if (rand16() % 2 == 1) return GetDC(NULL);
	else return CreateCompatibleDC(NULL);
}

HGDIOBJ makeHGDIOBJ()
{

	int i = rand16();

	switch (i % 12 + 1)
	{
		//Bitmap
	case 1: {
		return CreateBitmap(rand16(), rand16(), 1, 1, 0);
		break;
	}
	case 2: {
		return CreateCompatibleBitmap((HDC)makeHDC(), (int)rand16(), (int)rand16());
		break;
	}
	case 3: {
		return CreateDIBitmap((HDC)makeHDC(), (BITMAPINFOHEADER*)getBITMAPINFOHEADER(), 2, getPTR(), (BITMAPINFO*)getBITMAPINFO(), rand8() % 2);
		break;
	}
	case 4: {
		return CreateDIBSection((HDC)makeHDC(), (BITMAPINFO*)getBITMAPINFO(), 0, makeDPTR(), 0, 0);
		break;
	}
	case 5: {
		return CreateBrushIndirect((LOGBRUSH*)getLOGBRUSH());
		break;
	}
	case 6: {
		return CreateDIBPatternBrushPt((BITMAPINFO*)getBITMAPINFO(), (UINT)rand32() % 2);
		break;
	}
	case 7: {
		return CreateHatchBrush((int)rand32() % 6, (int)rand32());
		break;
	}
	case 8: {
		return CreateSolidBrush((int)rand32());
		break;
	}
			//Font
	case 9: {
		int cWeight[10] = { 0, 100, 200, 300, 400, 500, 600, 700, 800, 900 };
		DWORD iCharSet[17] = { 0, 1, 2, 128, 129, 134, 136, 255, 130, 177, 178, 161, 162, 163, 222, 238, 204 };
		DWORD iClipPrecision[8] = { 0, 1, 2, 15, 16, 32, 64, 128 };
		DWORD iPitchAndFamily[6] = { 0, 16, 32, 48, 64, 80 };
		return CreateFontW((int)rand32(), (int)rand32(), (int)rand32(), (int)rand32(), cWeight[rand32() % 10], (DWORD)rand32(), (DWORD)rand32(), (DWORD)rand32(), (DWORD)iCharSet[rand32() % 17], (DWORD)rand32() % 11, iClipPrecision[(DWORD)rand32() % 9], (DWORD)rand32() & 7, iPitchAndFamily[(DWORD)rand32() % 7], (LPCWSTR)makeSTR(rand8() % 100));
		break;
	}
	case 10: {
		return CreateFontIndirectW((LOGFONTW*)getLOGFONTW());
		break;
	}
	case 11: {
		return CreatePen((int)rand16() % 7, (int)rand16(), (int)rand32());
		break;
	}
	case 12: {
		return CreatePenIndirect((LOGPEN*)getLOGPEN());
		break;
	}
	}
}

HRGN makeHRGN()
{
	return CreateRectRgn(rand32() % 100000000, rand32() % 100000000, rand32() % 100000000, rand32() % 100000000);
}

HANDLE makeHANDLE()
{
	return makeHGDIOBJ();
}

/////////////////////////////////////////////////////////////////////////////////////////

HBITMAP makeHBITMAP()
{
	int i = rand16();

	switch (i % 4 + 1)
	{

	case 1: {
		return CreateBitmap(rand16(), rand16(), 1, 1, 0);
		break;
	}
	case 2: {
		return CreateCompatibleBitmap((HDC)makeHDC(), (int)rand16(), (int)rand16());
		break;
	}
	case 3: {
		return CreateDIBitmap((HDC)makeHDC(), (BITMAPINFOHEADER*)getBITMAPINFOHEADER(), 2, getPTR(), (BITMAPINFO*)getBITMAPINFO(), rand8() % 2);
		break;
	}
	case 4: {
		return CreateDIBSection((HDC)makeHDC(), (BITMAPINFO*)getBITMAPINFO(), 0, makeDPTR(), 0, 0);
		break;
	}
	}
}

int main(int argc, char* argv[])
{
	SetErrorMode(SEM_NOGPFAULTERRORBOX);

	if (argc != 2) {
		printf("please input argv[1]");
		exit(-1);
	}
	int i = 0;
	int seed;
	seed = atoi(argv[1]);
	srand(seed);

	
	while (TRUE) {		
		printf("[JJFUZZER] > (Loop) : %d\t(Seed) : %d \n", i, seed);

		std::stringstream ss;
		ss << "[JJFUZZER] > (Loop) : " << i <<" (Seed) : " << seed << std::endl;
		std::string str = ss.str();
		i++;
		std::ofstream fSeed;
		fSeed.open("Z:\SHARE\SEED.txt", std::ios::app);
		fSeed.write(str.c_str(), str.size());
		fSeed.close();

		makeALL();

	BITMAPINFO *BITMAPINFO_16 = getBITMAPINFO();BITMAPINFO *BITMAPINFO_18 = getBITMAPINFO();VOID *VOID_18 = getPTR();VOID *VOID_102 = getPTR();BITMAPINFO *BITMAPINFO_111 = getBITMAPINFO();VOID *VOID_114 = getPTR();HDC ret_0 = CreateICA((LPCSTR)"DISPLAY", 0, 0, 0x0);
HDC ret_1 = CreateCompatibleDC((HDC)ret_0);
HGDIOBJ ret_2 = GetCurrentObject((HDC)ret_1, (UINT)mut(0x7));
DWORD ret_3 = GetObjectType((HGDIOBJ)ret_2);
DWORD ret_4 = GetFontData((HDC)ret_1, (DWORD)mut(0x322f534f), (DWORD)mut(0x3e), (PVOID)rand(), (DWORD)mut(0x4));
HDC ret_5 = CreateCompatibleDC((HDC)ret_0);
HBITMAP ret_6 = CreateDIBitmap((HDC)ret_5, getBITMAPINFOHEADER(), (DWORD)mut(0x2), (VOID*)getPTR(), getBITMAPINFO(), (UINT)mut(0x0));
int ret_7 = GetObjectW((HANDLE)ret_6, (int)mut(0x18), (LPVOID)getPTR());
HRGN ret_8 = CreateRectRgn((int)mut(0x0), (int)mut(0x0), (int)mut(0x1), (int)mut(0x1));
BOOL ret_9 = TranslateCharsetInfo((DWORD*)getPTR(), (LPCHARSETINFO)getPTR(), (DWORD)mut(0x1000));
HDC ret_10 = CreateICA((LPCSTR)"DISPLAY", 0, 0, 0x0);
HGDIOBJ ret_11 = SelectObject((HDC)ret_5, (HGDIOBJ)ret_2);
HDC ret_12 = CreateICA((LPCSTR)"DISPLAY", 0, 0, 0x0);
int ret_13 = GetDeviceCaps((HDC)ret_5, (int)mut(0x26));
HDC ret_14 = CreateICA((LPCSTR)"DISPLAY", 0, 0, 0x0);
HGDIOBJ ret_15 = GetCurrentObject((HDC)ret_5, (UINT)mut(0x7));
int ret_16 = GetObjectA((HANDLE)ret_15, (int)mut(0x54), (LPVOID*)BITMAPINFO_16);
HBITMAP ret_17 = CreateDIBSection((HDC)ret_14, (BITMAPINFO*)BITMAPINFO_16, (UINT)mut(0x0), makeDPTR(), (HANDLE)makeHANDLE(), (DWORD)mut(0x0));
int ret_18 = GetObjectA((HANDLE)ret_17, (int)mut(0x54), (LPVOID*)VOID_18);
HGDIOBJ ret_19 = GetStockObject((int)mut(0xd));
HDC ret_20 = CreateICA((LPCSTR)"DISPLAY", 0, 0, 0x0);
DWORD ret_21 = GetFontData((HDC)ret_5, (DWORD)mut(0x322f534f), (DWORD)mut(0x3e), (PVOID)rand(), (DWORD)mut(0x4));
HFONT ret_22 = CreateFontW((int)mut(0xfffffff4), (int)mut(0x0), (int)mut(0x0), (int)mut(0x0), (int)mut(0x190), (DWORD)mut(0x0), (DWORD)mut(0x0), (DWORD)mut(0x0), (DWORD)mut(0x1), (DWORD)mut(0x0), (DWORD)mut(0x0), (DWORD)mut(0x5), (DWORD)mut(0x0), (LPCWSTR)"M");
HRGN ret_23 = CreateRectRgnIndirect((RECT*)getPTR());
BOOL ret_24 = SetRectRgn((HRGN)ret_23, (int)mut(0x0), (int)mut(0x0), (int)mut(0x0), (int)mut(0x0));
HBRUSH ret_25 = CreateBrushIndirect(getLOGBRUSH());
HRGN ret_26 = CreateRectRgn((int)mut(0x0), (int)mut(0x0), (int)mut(0x1), (int)mut(0x1));
int ret_27 = CombineRgn((HRGN)ret_23, (HRGN)ret_26, (HRGN)ret_26, (int)mut(0x5));
HPALETTE ret_28 = CreatePalette(getLOGPALETTE());
HPALETTE ret_29 = SelectPalette((HDC)makeHDC(), (HPALETTE)ret_28, (BOOL)mut(0x0));
HDC ret_30 = CreateICA((LPCSTR)"DISPLAY", 0, 0, 0x0);
HDC ret_31 = CreateICA((LPCSTR)"DISPLAY", 0, 0, 0x0);
HDC ret_32 = CreateICA((LPCSTR)"DISPLAY", 0, 0, 0x0);
HGDIOBJ ret_33 = GetCurrentObject((HDC)ret_5, (UINT)mut(0x7));
int ret_34 = GetObjectW((HANDLE)ret_17, (int)mut(0x18), (LPVOID)getPTR());
HDC ret_35 = CreateCompatibleDC((HDC)ret_32);
UINT ret_36 = GetTextAlign((HDC)ret_35);
HDC ret_37 = CreateICA((LPCSTR)"DISPLAY", 0, 0, 0x0);
HDC ret_38 = CreateCompatibleDC((HDC)ret_37);
UINT ret_39 = SetTextAlign((HDC)ret_38, (UINT)mut(0x0));
HDC ret_40 = CreateICA((LPCSTR)"DISPLAY", 0, 0, 0x0);
HDC ret_41 = CreateCompatibleDC((HDC)ret_40);
HBITMAP ret_42 = CreateBitmap((int)mut(0x8), (int)mut(0x8), (UINT)mut(0x1), (UINT)mut(0x1), (VOID*)getPTR());
BOOL ret_43 = DeleteObject((HGDIOBJ)ret_17);
HFONT ret_44 = CreateFontW((int)mut(0xfffffff4), (int)mut(0x0), (int)mut(0x0), (int)mut(0x0), (int)mut(0x190), (DWORD)mut(0x0), (DWORD)mut(0x0), (DWORD)mut(0x0), (DWORD)mut(0x1), (DWORD)mut(0x0), (DWORD)mut(0x0), (DWORD)mut(0x5), (DWORD)mut(0x0), (LPCWSTR)"M");
HRGN ret_45 = CreateRectRgn((int)mut(0x0), (int)mut(0x0), (int)mut(0x1), (int)mut(0x1));
HPALETTE ret_46 = SelectPalette((HDC)makeHDC(), (HPALETTE)ret_28, (BOOL)mut(0x0));
HDC ret_47 = CreateICA((LPCSTR)"DISPLAY", 0, 0, 0x0);
HDC ret_48 = CreateCompatibleDC((HDC)ret_47);
BOOL ret_49 = PatBlt((HDC)ret_48, (int)mut(0x0), (int)mut(0x0), (int)mut(0x18), (int)mut(0x14), (DWORD)mut(0xf00021));
int ret_50 = GetDeviceCaps((HDC)ret_47, (int)mut(0x26));
HGDIOBJ ret_51 = GetStockObject((int)mut(0xd));
HDC ret_52 = CreateICA((LPCSTR)"DISPLAY", 0, 0, 0x0);
BOOL ret_53 = GetViewportOrgEx((HDC)ret_48, getPOINT());
HRGN ret_54 = CreateRectRgnIndirect((RECT*)getPTR());
int ret_55 = CombineRgn((HRGN)ret_54, (HRGN)ret_54, (HRGN)ret_45, (int)mut(0x5));
BOOL ret_56 = GetTextMetricsW((HDC)ret_48, (LPTEXTMETRICW)getPTR());
HBRUSH ret_57 = CreateSolidBrush((COLORREF)mut(0x0));
HDC ret_58 = CreateCompatibleDC((HDC)ret_52);
int ret_59 = GetClipRgn((HDC)ret_58, (HRGN)ret_45);
HDC ret_60 = CreateICA((LPCSTR)"DISPLAY", 0, 0, 0x0);
HGDIOBJ ret_61 = GetCurrentObject((HDC)ret_58, (UINT)mut(0x7));
HPALETTE ret_62 = CreatePalette(getLOGPALETTE());
HRGN ret_63 = CreateRectRgnIndirect((RECT*)getPTR());
int ret_64 = CombineRgn((HRGN)ret_63, (HRGN)ret_63, (HRGN)ret_45, (int)mut(0x5));
HBITMAP ret_65 = CreateCompatibleBitmap((HDC)ret_60, (int)mut(0x1), (int)mut(0x1));
int ret_66 = GetDIBits((HDC)ret_60, (HBITMAP)ret_65, (UINT)mut(0x0), (UINT)mut(0x0), (LPVOID)mut(0x0), (LPBITMAPINFO)getPTR(), (UINT)mut(0x0));
HFONT ret_67 = CreateFontIndirectExW(getENUMLOGFONTEXDVW());
BOOL ret_68 = DeleteObject((HGDIOBJ)ret_67);
HDC ret_69 = CreateICA((LPCSTR)"DISPLAY", 0, 0, 0x0);
HDC ret_70 = CreateCompatibleDC((HDC)ret_69);
int ret_71 = IntersectClipRect((HDC)ret_70, (int)mut(0x0), (int)mut(0x0), (int)mut(0x2fffffff), (int)mut(0x2fffffff));
HRGN ret_72 = CreateRectRgn((int)mut(0x0), (int)mut(0x0), (int)mut(0x1), (int)mut(0x1));
int ret_73 = GetRandomRgn((HDC)ret_70, (HRGN)ret_72, (INT)mut(0x1));
HBITMAP ret_74 = CreateBitmap((int)mut(0x8), (int)mut(0x8), (UINT)mut(0x1), (UINT)mut(0x1), (VOID*)getPTR());
HDC ret_75 = CreateICA((LPCSTR)"DISPLAY", 0, 0, 0x0);
HDC ret_76 = CreateCompatibleDC((HDC)ret_75);
HDC ret_77 = CreateICA((LPCSTR)"DISPLAY", 0, 0, 0x0);
HDC ret_78 = CreateICA((LPCSTR)"DISPLAY", 0, 0, 0x0);
HBITMAP ret_79 = CreateDIBitmap((HDC)ret_76, getBITMAPINFOHEADER(), (DWORD)mut(0x2), (VOID*)getPTR(), getBITMAPINFO(), (UINT)mut(0x0));
HBITMAP ret_80 = CreateDIBSection((HDC)ret_78, (BITMAPINFO*)BITMAPINFO_18, (UINT)mut(0x0), makeDPTR(), (HANDLE)makeHANDLE(), (DWORD)mut(0x0));
HDC ret_81 = CreateICA((LPCSTR)"DISPLAY", 0, 0, 0x0);
HDC ret_82 = CreateCompatibleDC((HDC)ret_81);
HGDIOBJ ret_83 = GetCurrentObject((HDC)ret_82, (UINT)mut(0x7));
LONG ret_84 = SetBitmapBits((HBITMAP)ret_80, (DWORD)mut(0x4), (VOID*)VOID_18);
HDC ret_85 = CreateICA((LPCSTR)"DISPLAY", 0, 0, 0x0);
int ret_86 = GetDeviceCaps((HDC)ret_85, (int)mut(0x26));
HDC ret_87 = CreateCompatibleDC((HDC)ret_85);
HGDIOBJ ret_88 = GetCurrentObject((HDC)ret_87, (UINT)mut(0x7));
int ret_89 = GetObjectA((HANDLE)ret_88, (int)mut(0x54), (LPVOID)getPTR());
HDC ret_90 = CreateCompatibleDC((HDC)ret_85);
HDC ret_91 = CreateICA((LPCSTR)"DISPLAY", 0, 0, 0x0);
HDC ret_92 = CreateCompatibleDC((HDC)ret_91);
HDC ret_93 = CreateCompatibleDC((HDC)ret_91);
HBITMAP ret_94 = CreateBitmap((int)mut(0x8), (int)mut(0x8), (UINT)mut(0x1), (UINT)mut(0x1), (VOID*)getPTR());
int ret_95 = GetObjectA((HANDLE)ret_80, (int)mut(0x54), (LPVOID)getPTR());
HDC ret_96 = CreateCompatibleDC((HDC)ret_91);
HDC ret_97 = CreateICA((LPCSTR)"DISPLAY", 0, 0, 0x0);
HDC ret_98 = CreateCompatibleDC((HDC)ret_97);
HDC ret_99 = CreateICA((LPCSTR)"DISPLAY", 0, 0, 0x0);
HDC ret_100 = CreateCompatibleDC((HDC)ret_99);
int ret_101 = GetObjectA((HANDLE)ret_80, (int)mut(0x54), (LPVOID)getPTR());
int ret_102 = GetObjectA((HANDLE)ret_11, (int)mut(0x54), (LPVOID*)VOID_102);
LONG ret_103 = SetBitmapBits((HBITMAP)ret_80, (DWORD)mut(0x4), (VOID*)VOID_102);
HDC ret_104 = CreateICA((LPCSTR)"DISPLAY", 0, 0, 0x0);
HDC ret_105 = CreateCompatibleDC((HDC)ret_104);
HDC ret_106 = CreateICA((LPCSTR)"DISPLAY", 0, 0, 0x0);
HDC ret_107 = CreateCompatibleDC((HDC)ret_106);
HDC ret_108 = CreateICA((LPCSTR)"DISPLAY", 0, 0, 0x0);
HDC ret_109 = CreateICA((LPCSTR)"DISPLAY", 0, 0, 0x0);
HGDIOBJ ret_110 = GetCurrentObject((HDC)ret_107, (UINT)mut(0x7));
int ret_111 = GetObjectA((HANDLE)ret_110, (int)mut(0x54), (LPVOID*)BITMAPINFO_111);
HGDIOBJ ret_112 = SelectObject((HDC)ret_107, (HGDIOBJ)ret_80);
HBITMAP ret_113 = CreateDIBSection((HDC)ret_109, (BITMAPINFO*)BITMAPINFO_111, (UINT)mut(0x0), makeDPTR(), (HANDLE)makeHANDLE(), (DWORD)mut(0x0));
int ret_114 = GetObjectA((HANDLE)ret_113, (int)mut(0x54), (LPVOID*)VOID_114);
LONG ret_115 = SetBitmapBits((HBITMAP)ret_110, (DWORD)mut(0x4), (VOID*)VOID_114);
BOOL ret_116 = GetTextMetricsW((HDC)ret_107, (LPTEXTMETRICW)getPTR());
int ret_117 = GetClipRgn((HDC)ret_107, (HRGN)ret_72);
HDC ret_118 = CreateCompatibleDC((HDC)ret_109);
int ret_119 = GetRandomRgn((HDC)ret_118, (HRGN)ret_72, (INT)mut(0x1));
HGDIOBJ ret_120 = GetCurrentObject((HDC)ret_118, (UINT)mut(0x7));
BOOL ret_121 = DeleteObject((HGDIOBJ)ret_120);
HDC ret_122 = CreateICA((LPCSTR)"DISPLAY", 0, 0, 0x0);
HGDIOBJ ret_123 = GetCurrentObject((HDC)ret_118, (UINT)mut(0x7));


        freeALL();
	}
 
}
	