

#ifdef _MSC_VER
# include <windows.h>
#endif

#ifdef __cplusplus
# error "This file should be built as pure C to avoid name mangling"
#endif

#include <stdlib.h>
#include <string.h>

#include "dynamic_annotations.h"

/* Each function is empty and called (via a macro) only in debug mode.
   The arguments are captured by dynamic tools at runtime. */

#if DYNAMIC_ANNOTATIONS_ENABLED == 1

void AnnotateRWLockCreate(const char *file, int line,
                          const volatile void *lock){}
void AnnotateRWLockDestroy(const char *file, int line,
                           const volatile void *lock){}
void AnnotateRWLockAcquired(const char *file, int line,
                            const volatile void *lock, long is_w){}
void AnnotateRWLockReleased(const char *file, int line,
                            const volatile void *lock, long is_w){}
void AnnotateBarrierInit(const char *file, int line,
                         const volatile void *barrier, long count,
                         long reinitialization_allowed) {}
void AnnotateBarrierWaitBefore(const char *file, int line,
                               const volatile void *barrier) {}
void AnnotateBarrierWaitAfter(const char *file, int line,
                              const volatile void *barrier) {}
void AnnotateBarrierDestroy(const char *file, int line,
                            const volatile void *barrier) {}

void AnnotateCondVarWait(const char *file, int line,
                         const volatile void *cv,
                         const volatile void *lock){}
void AnnotateCondVarSignal(const char *file, int line,
                           const volatile void *cv){}
void AnnotateCondVarSignalAll(const char *file, int line,
                              const volatile void *cv){}
void AnnotatePublishMemoryRange(const char *file, int line,
                                const volatile void *address,
                                long size){}
void AnnotateUnpublishMemoryRange(const char *file, int line,
                                  const volatile void *address,
                                  long size){}
void AnnotatePCQCreate(const char *file, int line,
                       const volatile void *pcq){}
void AnnotatePCQDestroy(const char *file, int line,
                        const volatile void *pcq){}
void AnnotatePCQPut(const char *file, int line,
                    const volatile void *pcq){}
void AnnotatePCQGet(const char *file, int line,
                    const volatile void *pcq){}
void AnnotateNewMemory(const char *file, int line,
                       const volatile void *mem,
                       long size){}
void AnnotateExpectRace(const char *file, int line,
                        const volatile void *mem,
                        const char *description){}
void AnnotateBenignRace(const char *file, int line,
                        const volatile void *mem,
                        const char *description){}
void AnnotateBenignRaceSized(const char *file, int line,
                             const volatile void *mem,
                             long size,
                             const char *description) {}
void AnnotateMutexIsUsedAsCondVar(const char *file, int line,
                                  const volatile void *mu){}
void AnnotateTraceMemory(const char *file, int line,
                         const volatile void *arg){}
void AnnotateThreadName(const char *file, int line,
                        const char *name){}
void AnnotateIgnoreReadsBegin(const char *file, int line){}
void AnnotateIgnoreReadsEnd(const char *file, int line){}
void AnnotateIgnoreWritesBegin(const char *file, int line){}
void AnnotateIgnoreWritesEnd(const char *file, int line){}
void AnnotateIgnoreSyncBegin(const char *file, int line){}
void AnnotateIgnoreSyncEnd(const char *file, int line){}
void AnnotateEnableRaceDetection(const char *file, int line, int enable){}
void AnnotateNoOp(const char *file, int line,
                  const volatile void *arg){}
void AnnotateFlushState(const char *file, int line){}

static int GetRunningOnValgrind(void) {
#ifdef RUNNING_ON_VALGRIND
  if (RUNNING_ON_VALGRIND) return 1;
#endif

#ifndef _MSC_VER
  const char *running_on_valgrind_str = getenv("RUNNING_ON_VALGRIND");
  if (running_on_valgrind_str) {
    return strcmp(running_on_valgrind_str, "0") != 0;
  }
#else
  /* Visual Studio issues warnings if we use getenv,
   * so we use GetEnvironmentVariableA instead.
   */
  char value[100] = "1";
  int res = GetEnvironmentVariableA("RUNNING_ON_VALGRIND",
                                    value, sizeof(value));
  /* value will remain "1" if res == 0 or res >= sizeof(value). The latter
   * can happen only if the given value is long, in this case it can't be "0".
   */
  if (res > 0 && !strcmp(value, "0"))
    return 1;
#endif
  return 0;
}

/* See the comments in dynamic_annotations.h */
int RunningOnValgrind(void) {
  static volatile int running_on_valgrind = -1;
  /* C doesn't have thread-safe initialization of statics, and we
     don't want to depend on pthread_once here, so hack it. */
  int local_running_on_valgrind = running_on_valgrind;
  if (local_running_on_valgrind == -1)
    running_on_valgrind = local_running_on_valgrind = GetRunningOnValgrind();
  return local_running_on_valgrind;
}

#endif  /* DYNAMIC_ANNOTATIONS_ENABLED == 1 */
