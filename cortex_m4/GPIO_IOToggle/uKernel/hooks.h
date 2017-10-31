#ifndef __HOOKS_H__
#define __HOOKS_H__

#if IDLE_TASK_HOOK_ENABLED == 1
void hooks_idleTaskHook();
#endif
#if CONTEXT_SWITCH_HOOK_ENABLED == 1
void hooks_contextSwitchHook(void);
#endif
#endif
