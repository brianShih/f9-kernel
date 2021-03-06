/* Copyright (c) 2001-2004, 2006, 2008-2010 Karlsruhe University.
 * All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef __L4__THREAD_H__
#define __L4__THREAD_H__

#include <platform/link.h>
#include <l4/types.h>
#include __L4_INC_ARCH(syscalls.h)
#include __L4_INC_ARCH(vregs.h)

#define L4_ERROR_OK			(0)
#define L4_ERROR_NO_PRIVILEGE		(1)
#define L4_ERROR_INVALID_THREAD		(2)
#define L4_ERROR_INVALID_SPACE		(3)
#define L4_ERROR_INVALID_SCHEDULER	(4)
#define L4_ERROR_INVALID_PARAM		(5)
#define L4_ERROR_UTCB_AREA		(6)
#define L4_ERROR_KIP_AREA		(7)
#define L4_ERROR_NO_MEM			(8)

#define DECLARE_THREAD(name, sub) \
	void name(void) __attribute__ ((naked));	\
void __USER_TEXT name(void)			\
{						\
	register void *kip_ptr asm ("r0");	\
	register void *utcb_ptr asm ("r1");	\
	sub(kip_ptr, utcb_ptr);			\
	while (1);				\
}

/*
 * Thread states
 */
typedef struct L4_ThreadState {
	L4_Word_t raw;
} L4_ThreadState_t;

L4_INLINE L4_Bool_t L4_ThreadWasHalted(L4_ThreadState_t s)
{
	return s.raw & (1 << 0);
}

L4_INLINE L4_Bool_t L4_ThreadWasReceiving(L4_ThreadState_t s)
{
	return s.raw & (1 << 1);
}

L4_INLINE L4_Bool_t L4_ThreadWasSending(L4_ThreadState_t s)
{
	return s.raw & (1 << 2);
}

L4_INLINE L4_Bool_t L4_ThreadWasIpcing(L4_ThreadState_t s)
{
	return s.raw & (3 << 1);
}

/*
 * TCR related functions
 */
L4_INLINE L4_ThreadId_t L4_MyGlobalId(void)
{
	L4_ThreadId_t id;
	id.raw = __L4_TCR_MyGlobalId ();
	return id;
}

L4_INLINE L4_ThreadId_t L4_MyLocalId(void)
{
	L4_ThreadId_t id;
	id.raw = __L4_TCR_MyLocalId ();
	return id;
}

L4_INLINE L4_ThreadId_t L4_Myself(void)
{
	return L4_MyGlobalId ();
}

L4_INLINE L4_Word_t L4_ProcessorNo(void)
{
	return __L4_TCR_ProcessorNo();
}

L4_INLINE L4_Word_t L4_UserDefinedHandle(void)
{
	return __L4_TCR_UserDefinedHandle();
}

L4_INLINE void L4_Set_UserDefinedHandle(L4_Word_t NewValue)
{
	__L4_TCR_Set_UserDefinedHandle(NewValue);
}

L4_INLINE L4_ThreadId_t L4_Pager(void)
{
	L4_ThreadId_t id;
	id.raw = __L4_TCR_Pager();
	return id;
}

L4_INLINE void L4_Set_Pager(L4_ThreadId_t NewPager)
{
	__L4_TCR_Set_Pager(NewPager.raw);
}

L4_INLINE L4_ThreadId_t L4_ExceptionHandler(void)
{
	L4_ThreadId_t id;
	id.raw = __L4_TCR_ExceptionHandler();
	return id;
}

L4_INLINE void L4_Set_ExceptionHandler(L4_ThreadId_t NewHandler)
{
	__L4_TCR_Set_ExceptionHandler(NewHandler.raw);
}

L4_INLINE L4_Word_t L4_ErrorCode(void)
{
	return __L4_TCR_ErrorCode();
}

L4_INLINE const char *L4_ErrorCode_String(L4_Word_t err)
{
	switch (err) {
	case L4_ERROR_OK:
		return "l4_error_ok";
	case L4_ERROR_NO_PRIVILEGE:
		return "l4_error_no_privilege";
	case L4_ERROR_INVALID_THREAD:
		return "l4_error_invalid_thread";
	case L4_ERROR_INVALID_SPACE:
		return "l4_error_invalid_space";
	case L4_ERROR_INVALID_SCHEDULER:
		return "l4_error_invalid_scheduler";
	case L4_ERROR_INVALID_PARAM:
		return "l4_error_invalid_param";
	case L4_ERROR_UTCB_AREA:
		return "l4_error_utcb_area";
	case L4_ERROR_KIP_AREA:
		return "l4_error_kip_area";
	case L4_ERROR_NO_MEM:
		return "l4_error_no_mem";
	default:
		return "invalid error code";
	};
};

L4_INLINE L4_Word_t L4_XferTimeouts(void)
{
	return __L4_TCR_XferTimeout();
}

L4_INLINE void L4_Set_XferTimeouts(L4_Word_t NewValue)
{
	__L4_TCR_Set_XferTimeout(NewValue);
}

L4_INLINE L4_ThreadId_t L4_IntendedReceiver(void)
{
	L4_ThreadId_t id;
	id.raw = __L4_TCR_IntendedReceiver ();
	return id;
}

L4_INLINE L4_ThreadId_t L4_ActualSender(void)
{
	L4_ThreadId_t id;
	id.raw = __L4_TCR_ActualSender();
	return id;
}

L4_INLINE void L4_Set_VirtualSender(L4_ThreadId_t t)
{
	__L4_TCR_Set_VirtualSender(t.raw);
}

L4_INLINE L4_Word_t L4_WordSizeMask(void)
{
#if defined(__L4_TCR_WORD_SIZE_MASK)
	return __L4_TCR_WordSizeMask();
#else
	return (~((L4_Word_t) 0));
#endif
}

L4_INLINE void L4_Reset_WordSizeMask(void)
{
#if defined(__L4_TCR_WORD_SIZE_MASK)
	__L4_TCR_Set_WordSizeMask(~((L4_Word_t) 0));
#endif
}

/*
 * Derived functions
 */

/*
 * These are the functions derived from the exchange register syscall.
 * For normal C, function overloading is not supported.  Functions
 * with unique names (i.e., with the suffix inside <> appended) have
 * therefore been provided.
 *
 *   L4_GlobalId				(t)
 *   L4_LocalId					(t)
 *   L4_SameThreads				(l, r)
 *   L4_UserDefinedHandle<Of>			(t)
 *   L4_Set_UserDefinedHandle<Of>		(t, handle)
 *   L4_Pager<Of>				(t)
 *   L4_Set_Pager<Of>				(t, pager)
 *   L4_Start					(t)
 *   L4_Start<_SpIp>				(t, sp, ip)
 *   L4_Start<_SpIpFlags>			(t, sp, ip, flags)
 *   L4_Stop					(t)
 *   L4_Stop<_SpIpFlags>			(t, &sp, &ip, &flags)
 *   L4_AbortReceive_and_stop			(t)
 *   L4_AbortReceive_and_stop<_SpIpFlags>	(t, &sp, &ip, &flags)
 *   L4_AbortSend_and_stop			(t)
 *   L4_AbortSend_and_stop<_SpIpFlags>		(t, &sp, &ip, &flags)
 *   L4_AbortIpc_and_stop			(t)
 *   L4_AbortIpc_and_stop<_SpIpFlags>		(t, &sp, &ip, &flags)
 *
 */
L4_INLINE L4_ThreadId_t L4_GlobalIdOf(L4_ThreadId_t t)
{
	L4_Word_t dummy;
	L4_ThreadId_t dummy_id;

	if (L4_IsLocalId (t))
		return L4_ExchangeRegisters(
				t, 0, 0, 0, 0, 0, L4_nilthread,
				&dummy, &dummy, &dummy, &dummy, &dummy,
				&dummy_id);
	else
		return t;
}

L4_INLINE L4_Bool_t L4_SameThreads(L4_ThreadId_t l, L4_ThreadId_t r)
{
	return L4_GlobalIdOf(l).raw == L4_GlobalIdOf (r).raw;
}

L4_INLINE L4_ThreadId_t L4_LocalIdOf(L4_ThreadId_t t)
{
	L4_Word_t dummy;
	L4_ThreadId_t dummy_id;

	if (L4_IsGlobalId (t))
		return L4_ExchangeRegisters(
				t, 0, 0, 0, 0, 0, L4_nilthread,
				&dummy, &dummy, &dummy, &dummy, &dummy,
				&dummy_id);
	else
		return t;
}

L4_INLINE L4_Word_t L4_UserDefinedHandleOf(L4_ThreadId_t t)
{
	L4_Word_t dummy, handle;
	L4_ThreadId_t dummy_id;

	(void) L4_ExchangeRegisters(
			t, (1 << 9), 0, 0, 0, 0, L4_nilthread,
			&dummy, &dummy, &dummy, &dummy, &handle,
			&dummy_id);

	return handle;
}

L4_INLINE void L4_Set_UserDefinedHandleOf(
		L4_ThreadId_t t, L4_Word_t handle)
{
	L4_Word_t dummy;
	L4_ThreadId_t dummy_id;

	(void) L4_ExchangeRegisters(
			t, (1 << 6), 0, 0, 0, handle, L4_nilthread,
			&dummy, &dummy, &dummy, &dummy, &dummy,
			&dummy_id);
}

L4_INLINE L4_ThreadId_t L4_PagerOf(L4_ThreadId_t t)
{
	L4_Word_t dummy;
	L4_ThreadId_t pager;

	(void) L4_ExchangeRegisters(
			t, (1 << 9), 0, 0, 0, 0, L4_nilthread,
			&dummy, &dummy, &dummy, &dummy, &dummy,
			&pager);

	return pager;
}

L4_INLINE void L4_Set_PagerOf(
		L4_ThreadId_t t, L4_ThreadId_t p)
{
	L4_Word_t dummy;
	L4_ThreadId_t dummy_id;

	(void) L4_ExchangeRegisters(
			t, (1 << 7), 0, 0, 0, 0, p,
			&dummy, &dummy, &dummy, &dummy, &dummy,
			&dummy_id);
}

L4_INLINE void L4_Start(L4_ThreadId_t t)
{
	L4_Word_t dummy;
	L4_ThreadId_t dummy_id;

	(void) L4_ExchangeRegisters(
			t, (1 << 8) + 6, 0, 0, 0, 0, L4_nilthread,
			&dummy, &dummy, &dummy, &dummy, &dummy,
			&dummy_id);
}

L4_INLINE void L4_Start_SpIp(L4_ThreadId_t t, L4_Word_t sp, L4_Word_t ip)
{
	L4_Word_t dummy;
	L4_ThreadId_t dummy_id;

	(void) L4_ExchangeRegisters(
			t, (3 << 3) + (1 << 8) + 6, sp, ip, 0, 0,
			L4_nilthread,
			&dummy, &dummy, &dummy, &dummy, &dummy,
			&dummy_id);
}


L4_INLINE void L4_Start_SpIpFlags(
		L4_ThreadId_t t, L4_Word_t sp,
		L4_Word_t ip, L4_Word_t flags)
{
	L4_Word_t dummy;
	L4_ThreadId_t dummy_id;

	(void) L4_ExchangeRegisters(
			t, (7 << 3) + (1 << 8) + 6, sp, ip, flags, 0,
			L4_nilthread,
			&dummy, &dummy, &dummy, &dummy, &dummy,
			&dummy_id);
}

L4_INLINE L4_ThreadState_t L4_Stop(L4_ThreadId_t t)
{
	L4_Word_t dummy;
	L4_ThreadId_t dummy_id;
	L4_ThreadState_t state;

	(void) L4_ExchangeRegisters(
			t, 1 + (1 << 8) + (1 << 9), 0, 0, 0, 0,
			L4_nilthread, &state.raw, &dummy, &dummy,
			&dummy, &dummy, &dummy_id);

	return state;
}

L4_INLINE L4_ThreadState_t L4_Stop_SpIpFlags(
		L4_ThreadId_t t,
		L4_Word_t *sp,
		L4_Word_t *ip,
		L4_Word_t *flags)
{
	L4_Word_t dummy;
	L4_ThreadId_t dummy_id;
	L4_ThreadState_t state;

	(void) L4_ExchangeRegisters(
			t, 1 + (1 << 8) + (1 << 9), 0, 0, 0, 0,
			L4_nilthread, &state.raw, sp, ip, flags,
			&dummy, &dummy_id);

	return state;
}

L4_INLINE L4_ThreadState_t L4_AbortReceive_and_stop(L4_ThreadId_t t)
{
	L4_Word_t dummy;
	L4_ThreadId_t dummy_id;
	L4_ThreadState_t state;

	(void) L4_ExchangeRegisters(
			t, 3 + (1 << 8) + (1 << 9), 0, 0, 0, 0,
			L4_nilthread, &state.raw, &dummy, &dummy,
			&dummy, &dummy, &dummy_id);

	return state;
}

L4_INLINE L4_ThreadState_t L4_AbortReceive_and_stop_SpIpFlags(
		L4_ThreadId_t t,
		L4_Word_t *sp,
		L4_Word_t *ip,
		L4_Word_t *flags)
{
	L4_Word_t dummy;
	L4_ThreadId_t dummy_id;
	L4_ThreadState_t state;

	(void) L4_ExchangeRegisters(
			t, 3 + (1 << 8) + (1 << 9), 0, 0, 0, 0,
			L4_nilthread, &state.raw, sp, ip, flags,
			&dummy, &dummy_id);

	return state;
}

L4_INLINE L4_ThreadState_t L4_AbortSend_and_stop(L4_ThreadId_t t)
{
	L4_Word_t dummy;
	L4_ThreadId_t dummy_id;
	L4_ThreadState_t state;

	(void) L4_ExchangeRegisters(
			t, 5 + (1 << 8) + (1 << 9), 0, 0, 0, 0,
			L4_nilthread, &state.raw, &dummy, &dummy,
			&dummy, &dummy, &dummy_id);

	return state;
}

L4_INLINE L4_ThreadState_t L4_AbortSend_and_stop_SpIpFlags(
		L4_ThreadId_t t,
		L4_Word_t *sp,
		L4_Word_t *ip,
		L4_Word_t *flags)
{

	L4_Word_t dummy;
	L4_ThreadId_t dummy_id;
	L4_ThreadState_t state;

	(void) L4_ExchangeRegisters(
			t, 5 + (1 << 8) + (1 << 9), 0, 0, 0, 0,
			L4_nilthread, &state.raw, sp, ip, flags,
			&dummy, &dummy_id);

	return state;
}

L4_INLINE L4_ThreadState_t L4_AbortIpc_and_stop(L4_ThreadId_t t)
{
	L4_Word_t dummy;
	L4_ThreadId_t dummy_id;
	L4_ThreadState_t state;

	(void) L4_ExchangeRegisters(
			t, 7 + (1 << 8) + (1 << 9), 0, 0, 0, 0,
			L4_nilthread, &state.raw, &dummy, &dummy,
			&dummy, &dummy, &dummy_id);

	return state;
}

L4_INLINE L4_ThreadState_t L4_AbortIpc_and_stop_SpIpFlags(
		L4_ThreadId_t t,
		L4_Word_t *sp,
		L4_Word_t *ip,
		L4_Word_t *flags)
{
	L4_Word_t dummy;
	L4_ThreadId_t dummy_id;
	L4_ThreadState_t state;

	(void) L4_ExchangeRegisters(
			t, 7 + (1 << 8) + (1 << 9), 0, 0, 0, 0,
			L4_nilthread, &state.raw, sp, ip, flags,
			&dummy, &dummy_id);

	return state;
}

#define L4_EXREGS_CTRLXFER_CONF_FLAG    	(1UL <<  9)
#define L4_EXREGS_CTRLXFER_READ_FLAG    	(1UL << 10)
#define L4_EXREGS_CTRLXFER_WRITE_FLAG    	(1UL << 11)
#define L4_EXREGS_EXCHANDLER_FLAG		(1UL << 12)
#define L4_EXREGS_SCHEDULER_FLAG		(1UL << 13)

L4_INLINE L4_Word_t L4_AssociateInterrupt(
		L4_ThreadId_t InterruptThread,
		L4_ThreadId_t HandlerThread)
{
	return L4_ThreadControl(
			InterruptThread, InterruptThread,
			L4_nilthread, HandlerThread, (void *) -1);
}

L4_INLINE L4_Word_t L4_DeassociateInterrupt(
		L4_ThreadId_t InterruptThread)
{
	return L4_ThreadControl(
			InterruptThread, InterruptThread,
			L4_nilthread, InterruptThread, (void *) -1);
}

L4_INLINE L4_Word_t L4_ConfCtrlXferItems(L4_ThreadId_t dest)
{
	L4_Word_t dummy, old_control;
	L4_ThreadId_t dummy_tid;
	L4_ExchangeRegisters(
			dest, L4_EXREGS_CTRLXFER_CONF_FLAG, 0, 0 , 0, 0, L4_nilthread,
			&old_control, &dummy, &dummy, &dummy, &dummy, &dummy_tid);
	return old_control;
}

L4_INLINE L4_Word_t L4_ReadCtrlXferItems(L4_ThreadId_t dest)
{
	L4_Word_t dummy, old_control;
	L4_ThreadId_t dummy_tid;
	L4_ExchangeRegisters(
			dest, L4_EXREGS_CTRLXFER_READ_FLAG, 0, 0 , 0, 0, L4_nilthread,
			&old_control, &dummy, &dummy, &dummy, &dummy, &dummy_tid);
	return old_control;
}

L4_INLINE L4_Word_t L4_WriteCtrlXferItems(L4_ThreadId_t dest)
{
	L4_Word_t dummy, old_control;
	L4_ThreadId_t dummy_tid;
	L4_ExchangeRegisters(
			dest, L4_EXREGS_CTRLXFER_WRITE_FLAG, 0, 0 , 0, 0, L4_nilthread,
			&old_control, &dummy, &dummy, &dummy, &dummy, &dummy_tid);
	return old_control;
}

#endif /* !__L4__THREAD_H__ */
