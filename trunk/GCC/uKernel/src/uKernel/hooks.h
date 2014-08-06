#ifndef __HOOKS_H__
#define __HOOKS_H__

#if IDLE_TASK_HOOK_ENABLED == 1
void idleTaskHook();
#endif
#if CONTEXT_SWITCH_HOOK_ENABLED == 1
void contextSwitchHook(void);
#endif
#endif
