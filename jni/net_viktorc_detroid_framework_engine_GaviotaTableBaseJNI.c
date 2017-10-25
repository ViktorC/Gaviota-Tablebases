/*
 * net_viktorc_detroid_framework_engine_GaviotaTableBaseJNI.c
 *
 *  Created on: Oct 23, 2017
 *      Author: Viktor Csomor
 */

#include "jni.h"
#include "net_viktorc_detroid_framework_engine_GaviotaTableBaseJNI.h"
#include "gtb-probe.h"
#include <stdio.h>
#include <stdbool.h>

/**
 * A struct for holding the board information required for probing.
 */
struct BOARD_POS {
	// The squares occupied by the white and black pieces respectively.
	unsigned* cWSqrs;
	unsigned* cBSqrs;
	// The pieces occupying those squares.
	unsigned char* cWPcs;
	unsigned char* cBPcs;
};

/**
 * Copies the contents of the Java integer and character arrays into an allocated BOARD_POS struct
 * and returns a pointer to it.
 */
struct BOARD_POS* getBoardPosition(JNIEnv* env, jintArray wSqrs, jintArray bSqrs, jcharArray wPcs,
		jcharArray bPcs);

/**
 * Releases the resources held by the struct containing the board position information.
 */
void releaseBoardPosition(struct BOARD_POS* posPtr);

/**
 * Creates and returns a Java integer array containing the probe results.
 */
jintArray getProbeResult(JNIEnv* env, unsigned info, unsigned dtm);

JNIEXPORT jstring JNICALL Java_net_viktorc_detroid_framework_engine_GaviotaTableBaseJNI_init
		(JNIEnv * env, jobject obj, jboolean verbose, jint compScheme, jstring paths) {
	const char* cPaths = (*env)->GetStringUTFChars(env, paths, false);
	const char** initPaths = tbpaths_init();
	initPaths = tbpaths_add(initPaths, cPaths);
	const char* cInitInfo = tb_init(verbose, compScheme, initPaths);
	jstring initInfo = (*env)->NewStringUTF(env, cInitInfo);
	(*env)->ReleaseStringUTFChars(env, paths, cPaths);
	return initInfo;
}

JNIEXPORT jstring JNICALL Java_net_viktorc_detroid_framework_engine_GaviotaTableBaseJNI_restart
		(JNIEnv* env, jobject obj, jboolean verbose, jint compScheme, jstring paths) {
	const char** initPaths = tbpaths_init();
	initPaths = tbpaths_done(initPaths);
	const char* cPaths = (*env)->GetStringUTFChars(env, paths, false);
	initPaths = tbpaths_init();
	initPaths = tbpaths_add(initPaths, cPaths);
	const char* cInitInfo = tb_restart (verbose, compScheme, initPaths);
	jstring initInfo = (*env)->NewStringUTF(env, cInitInfo);
	(*env)->ReleaseStringUTFChars(env, paths, cPaths);
	return initInfo;
}

JNIEXPORT jboolean JNICALL Java_net_viktorc_detroid_framework_engine_GaviotaTableBaseJNI_isInit
		(JNIEnv* env, jobject obj) {
	return tb_is_initialized();
}

JNIEXPORT jint JNICALL Java_net_viktorc_detroid_framework_engine_GaviotaTableBaseJNI_availability
		(JNIEnv* env, jobject obj) {
	return tb_availability();
}

JNIEXPORT jlong JNICALL Java_net_viktorc_detroid_framework_engine_GaviotaTableBaseJNI_indexMemory
		(JNIEnv* env, jobject obj) {
	return tb_indexmemory();
}

JNIEXPORT void JNICALL Java_net_viktorc_detroid_framework_engine_GaviotaTableBaseJNI_close
		(JNIEnv* env, jobject obj) {
	tb_done();
	tbpaths_done(tbpaths_init());
	return;
}

JNIEXPORT jboolean JNICALL Java_net_viktorc_detroid_framework_engine_GaviotaTableBaseJNI_initCache
		(JNIEnv* env, jobject obj, jlong size, jint wdlFraction) {
	return tbcache_init(size, wdlFraction);
}

JNIEXPORT jboolean JNICALL Java_net_viktorc_detroid_framework_engine_GaviotaTableBaseJNI_restartCache
		(JNIEnv* env, jobject obj, jlong size, jint wdlFraction) {
	return tbcache_restart(size, wdlFraction);
}

JNIEXPORT jboolean JNICALL Java_net_viktorc_detroid_framework_engine_GaviotaTableBaseJNI_isCacheOn
		(JNIEnv* env, jobject obj) {
	return tbcache_is_on();
}

JNIEXPORT void JNICALL Java_net_viktorc_detroid_framework_engine_GaviotaTableBaseJNI_clearCache
		(JNIEnv* env, jobject obj) {
	tbcache_flush();
	return;
}

JNIEXPORT void JNICALL Java_net_viktorc_detroid_framework_engine_GaviotaTableBaseJNI_closeCache
		(JNIEnv* env, jobject obj) {
	tbcache_done();
	return;
}

JNIEXPORT void JNICALL Java_net_viktorc_detroid_framework_engine_GaviotaTableBaseJNI_getStats
		(JNIEnv* env, jobject obj, jlongArray intStats, jdoubleArray fpStats) {
	unsigned long int wdlEasyHits, wdlHardProb, wdlSoftProb, wdlCacheSize;
	unsigned long int dtmEasyHits, dtmHardProb, dtmSoftProb, dtmCacheSize;
	unsigned long int totalHits, memoryHits, driveHits, driveMisses, bytesRead, filesOpened;
	double wdlOccupancy, dtmOccupancy, memoryEfficiency;
	struct TB_STATS stats;
	tbstats_get(&stats);
	wdlEasyHits = (unsigned long int) stats.wdl_easy_hits[0] | ((unsigned long int) stats.wdl_easy_hits[1] << 32);
	wdlHardProb = (unsigned long int) stats.wdl_hard_prob[0] | ((unsigned long int) stats.wdl_hard_prob[1] << 32);
	wdlSoftProb = (unsigned long int) stats.wdl_soft_prob[0] | ((unsigned long int) stats.wdl_soft_prob[1] << 32);
	wdlCacheSize = stats.wdl_cachesize;
	wdlOccupancy = stats.wdl_occupancy;
	dtmEasyHits = (unsigned long int) stats.dtm_easy_hits[0] | ((unsigned long int) stats.dtm_easy_hits[1] << 32);
	dtmHardProb = (unsigned long int) stats.dtm_hard_prob[0] | ((unsigned long int) stats.dtm_hard_prob[1] << 32);
	dtmSoftProb = (unsigned long int) stats.dtm_soft_prob[0] | ((unsigned long int) stats.dtm_soft_prob[1] << 32);
	dtmCacheSize = stats.dtm_cachesize;
	dtmOccupancy = stats.dtm_occupancy;
	totalHits = (unsigned long int) stats.total_hits[0] | ((unsigned long int) stats.total_hits[1] << 32);
	memoryHits = (unsigned long int) stats.memory_hits[0] | ((unsigned long int) stats.memory_hits[1] << 32);
	driveHits = (unsigned long int) stats.drive_hits[0] | ((unsigned long int) stats.drive_hits[1] << 32);
	driveMisses = (unsigned long int) stats.drive_miss[0] | ((unsigned long int) stats.drive_miss[1] << 32);
	bytesRead = (unsigned long int) stats.bytes_read[0] | ((unsigned long int) stats.bytes_read[1] << 32);
	filesOpened = stats.files_opened;
	memoryEfficiency = stats.memory_efficiency;
	jlong cIntArray[] = { wdlEasyHits, wdlHardProb, wdlSoftProb, wdlCacheSize,
			dtmEasyHits, dtmHardProb, dtmSoftProb, dtmCacheSize,
			totalHits, memoryHits, driveHits, driveMisses, bytesRead, filesOpened };
	jdouble cFpArray[] = { wdlOccupancy, dtmOccupancy, memoryEfficiency };
	(*env)->SetLongArrayRegion(env, intStats, 0 , 14, cIntArray);
	(*env)->SetDoubleArrayRegion(env, fpStats, 0 , 3, cFpArray);
	return;
}

JNIEXPORT void JNICALL Java_net_viktorc_detroid_framework_engine_GaviotaTableBaseJNI_resetStats
		(JNIEnv* env, jobject obj) {
	tbstats_reset();
	return;
}

JNIEXPORT jintArray JNICALL Java_net_viktorc_detroid_framework_engine_GaviotaTableBaseJNI_probe
		(JNIEnv* env, jobject obj, jint sideToMove, jint enPassantSqr, jint castlingRights,
		jintArray wSqrs, jintArray bSqrs, jcharArray wPcs, jcharArray bPcs) {
	unsigned int info, dtm;
	jintArray out = NULL;
	struct BOARD_POS* posPtr = getBoardPosition(env, wSqrs, bSqrs, wPcs, bPcs);
	if (tb_probe_hard(sideToMove, enPassantSqr, castlingRights, posPtr->cWSqrs, posPtr->cBSqrs,
			posPtr->cWPcs, posPtr->cBPcs, &info, &dtm)) {
		out = getProbeResult(env, info, dtm);
	}
	releaseBoardPosition(posPtr);
	return out;
}

JNIEXPORT jintArray JNICALL Java_net_viktorc_detroid_framework_engine_GaviotaTableBaseJNI_probeSoft
		(JNIEnv* env, jobject obj, jint sideToMove, jint enPassantSqr, jint castlingRights,
		jintArray wSqrs, jintArray bSqrs, jcharArray wPcs, jcharArray bPcs) {
	unsigned int info, dtm;
	jintArray out = NULL;
	struct BOARD_POS* posPtr = getBoardPosition(env, wSqrs, bSqrs, wPcs, bPcs);
	if (tb_probe_soft(sideToMove, enPassantSqr, castlingRights, posPtr->cWSqrs, posPtr->cBSqrs,
			posPtr->cWPcs, posPtr->cBPcs, &info, &dtm)) {
		out = getProbeResult(env, info, dtm);
	}
	releaseBoardPosition(posPtr);
	return out;
}

JNIEXPORT jint JNICALL Java_net_viktorc_detroid_framework_engine_GaviotaTableBaseJNI_probeWDL
		(JNIEnv* env, jobject obj, jint sideToMove, jint enPassantSqr, jint castlingRights,
		jintArray wSqrs, jintArray bSqrs, jcharArray wPcs, jcharArray bPcs) {
	unsigned int info;
	struct BOARD_POS* posPtr = getBoardPosition(env, wSqrs, bSqrs, wPcs, bPcs);
	if (!tb_probe_WDL_hard(sideToMove, enPassantSqr, castlingRights, posPtr->cWSqrs, posPtr->cBSqrs,
			posPtr->cWPcs, posPtr->cBPcs, &info)) {
		info = tb_UNKNOWN;
	}
	releaseBoardPosition(posPtr);
	return info;
}

JNIEXPORT jint JNICALL Java_net_viktorc_detroid_framework_engine_GaviotaTableBaseJNI_probeSoftWDL
		(JNIEnv* env, jobject obj, jint sideToMove, jint enPassantSqr, jint castlingRights,
		jintArray wSqrs, jintArray bSqrs, jcharArray wPcs, jcharArray bPcs) {
	unsigned int info;
	struct BOARD_POS* posPtr = getBoardPosition(env, wSqrs, bSqrs, wPcs, bPcs);
	if (!tb_probe_WDL_soft(sideToMove, enPassantSqr, castlingRights, posPtr->cWSqrs, posPtr->cBSqrs,
			posPtr->cWPcs, posPtr->cBPcs, &info)) {
		info = tb_UNKNOWN;
	}
	releaseBoardPosition(posPtr);
	return info;
}

struct BOARD_POS* getBoardPosition(JNIEnv* env, jintArray wSqrs, jintArray bSqrs, jcharArray wPcs,
		jcharArray bPcs) {
	int wLength = (*env)->GetArrayLength(env, wSqrs);
	int bLength = (*env)->GetArrayLength(env, bSqrs);
	jint* jWSqrs = (*env)->GetIntArrayElements(env, wSqrs, false);
	jint* jBSqrs = (*env)->GetIntArrayElements(env, bSqrs, false);
	jchar* jWPcs = (*env)->GetCharArrayElements(env, wPcs, false);
	jchar* jBPcs = (*env)->GetCharArrayElements(env, bPcs, false);
	unsigned cWSqrs[wLength];
	unsigned cBSqrs[bLength];
	unsigned char cWPcs[wLength];
	unsigned char cBPcs[bLength];
	for (int i = 0; i < wLength; i++) {
		cWSqrs[i] = jWSqrs[i];
		cWPcs[i] = jWPcs[i];
	}
	for (int i = 0; i < bLength; i++) {
		cBSqrs[i] = jBSqrs[i];
		cBPcs[i] = jBPcs[i];
	}
	struct BOARD_POS* posPtr = malloc(sizeof(struct BOARD_POS));
	size_t wSqrsSize = wLength*sizeof(unsigned);
	size_t bSqrsSize = bLength*sizeof(unsigned);
	size_t wPcsSize = wLength*sizeof(unsigned char);
	size_t bPcsSize = bLength*sizeof(unsigned char);
	posPtr->cWSqrs = malloc(wSqrsSize);
	posPtr->cBSqrs = malloc(bSqrsSize);
	posPtr->cWPcs = malloc(wPcsSize);
	posPtr->cBPcs = malloc(bPcsSize);
	memcpy(posPtr->cWSqrs, cWSqrs, wSqrsSize);
	memcpy(posPtr->cBSqrs, cBSqrs, bSqrsSize);
	memcpy(posPtr->cWPcs, cWPcs, wPcsSize);
	memcpy(posPtr->cBPcs, cBPcs, bPcsSize);
	(*env)->ReleaseIntArrayElements(env, wSqrs, jWSqrs, 0);
	(*env)->ReleaseIntArrayElements(env, bSqrs, jBSqrs, 0);
	(*env)->ReleaseCharArrayElements(env, wPcs, jWPcs, 0);
	(*env)->ReleaseCharArrayElements(env, bPcs, jBPcs, 0);
	return posPtr;
}

void releaseBoardPosition(struct BOARD_POS* posPtr) {
	free(posPtr->cWSqrs);
	free(posPtr->cBSqrs);
	free(posPtr->cWPcs);
	free(posPtr->cBPcs);
	free(posPtr);
	return;
}

jintArray getProbeResult(JNIEnv* env, unsigned info, unsigned dtm) {
	jintArray jArray =  (*env)->NewIntArray(env, 2);
	jint res[] = { info, dtm };
	(*env)->SetIntArrayRegion(env, jArray, 0 , 2, res);
	return jArray;
}
